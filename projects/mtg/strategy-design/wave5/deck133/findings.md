# Wave 5 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: run `matchups-20260713-145947`, 6 games, deck133 was **deck1** in every pairing.
Record **3/6** (up from wave-4's 3/12 ≈ effective 2/6). Game→seat-file→outcome:

| opp | seat jsonl (epoch) | result | end life me/opp | last turn | recs | med lat | max lat |
|-----|--------------------|--------|-----------------|-----------|------|---------|---------|
| 44  | 1783972792 | **WON**    | 13 / **-13** | 13 | 36 | 11.4s | 73.6s |
| 140 | 1783972794 | TIMEOUT    | 6 / 2        | 21 | 68 | 10.5s | 74.2s |
| 109 | 1783975562 | LOST       | -1 / **20**  | 8  | 22 | 10.5s | 53.1s |
| 131 | 1783976205 | **WON**    | 29 / -1      | 17 | 46 | 10.6s | 69.4s |
| 135 | 1783979342 | **WON**    | 21 / -3      | 11 | 45 | 9.8s  | 72.1s |
| 110 | 1783980403 | LOST       | -1 / **18**  | 12 | 44 | 14.7s | 69.8s |

Corpus behavioral stats (harness sweep clean — see below):
- **Desyncs: 0/261** (first-int-of-reply vs `choice` across all records). Parser is clean; no play was mis-executed.
- **Cast-nothing:** 31/78 casting decisions overall, but **24 of those 31 are in the single vs140 game** (targetless Fatal Push re-offered every window). Excluding vs140: **7/45 ≈ 16%** (wave-4 was 36%, wave-3 75%). The act-floor is holding; passivity is no longer this deck's problem.
- **Gray Merchant:** reached the cast list in only 3 of 6 games; offered 4 times, cast 3, **missed once (vs110, the decisive misplay below)**.
- **Discard casts:** 15 total; 6 on turn ≥4 (40%, ~unchanged from wave-4's 42%). The late ones cluster in the aggro loss (vs109) where they were dead.

---

## The 4 findings that matter (read first)

### 1. [NEW ENGINE FACT — highest-leverage] The Gray Merchant option now DISPLAYS its drain, but under-counts by exactly 2.
The option text is now e.g. `Cast Gray Merchant of Asphodel {3}{b}{b} (2/4) {right now: drains N}`.
Verified across all three casts, displayed N vs the life swing that actually landed:
- vs131 seq40: `drains 4` → my life 15→**21** (+6), opp 15→**9** (−6). Actual **6**.
- vs131 seq44: `drains 6` → 21→**29** (+8), opp 7→**-1** (−8). Actual **8**.
- vs135 seq43: `drains 8` → 11→**21** (+10), opp 7→**-3** (−10). Actual **10**.

Every case is **actual = shown N + 2.** Cause is mechanical and reliable: `auto=lifeleech:...:mybattlefield`
counts devotion to black from permanents on the battlefield, and Gray Merchant himself ({3}{B}{B} = 2
devotion) is still in HAND when the option is rendered, so his own 2 is added only on resolution. This
supersedes the guide's hand-computed devotion table (`2 + 2 per Bloodghast + 3 per Geralf…`): the pilot
should READ the shown number and add 2, not sum permanents (which it fumbles — see finding 3). Also a
representation flag: the displayed value should include Gray Merchant's own devotion (see notes.md E2).

### 2. [PRIMARY LOSS — persisting from wave-4 finding 3] Against aggro the pilot floods dead discard instead of interacting with the board.
**vs109 (red aggro: Stromkirk Noble, Ash Zealot, Krenko's Command) — LOST with the opponent at 20, never losing a single life point.** deck133 went 20→16→11→5→-1 in 8 turns. The record:
- Cast **Inquisition T1, Inquisition T5, Thoughtseize T5, Thoughtseize T7** (seq2, seq8, seq10, seq18) — four discard spells while being raced. By T5 the opponent had 1-2 cards; stripping a near-empty hand does nothing about the two creatures killing it.
- **Collective Brutality's `creature gets -2/-2` mode was available (seq8, seq17) and never used on Stromkirk Noble** — the one piece of creature interaction in hand went unused while the pilot chose the discard mode instead.
- Self-inflicted life: Thoughtseize ×2 (−4) + fetch cracks accelerated its own death.
- Role never flipped in time: the DEFENDER trigger ("your life falling while theirs is not") was visibly true from T3 (life 16, opp 20), but the pilot only verbalized "I am losing the race" at seq20, T7, life 5 — too late.

This is the wave-4 leak, not fixed. The guide's DEFENDER branch exists but (a) fires too late, (b) names Fatal Push / Liliana / Obliterator but omits the Collective Brutality removal mode the pilot actually held, and (c) does not rank discard *below* interaction when racing.

### 3. [NEW — decisive misplay] At 1 life with Gray Merchant CASTABLE, the pilot cast Fatal Push on a 0/2 instead of stabilizing.
**vs110 (affinity: Etched Champion, Steel Overseer, Cranial Plating) — LOST at -1, opp at 18.** The crux is seq34, T11, **my life 1**: the option list was `1. Cast Gray Merchant {3}{b}{b}` / `2. Cast Fatal Push` / `3-4. Collective Brutality` / `5. nothing`. Gray Merchant was on offer (devotion ~7 → drain ~7 → my life 1→~8, a full stabilize). The pilot chose **Fatal Push (seq35) on an untapped 0/2 Ornithopter**, then Collective Brutality's drain-2 mode (seq38) — spending the mana that would have cast Gray Merchant. It even reasoned "my only win condition is Gray Merchant" at seq38 yet had already committed the mana elsewhere. It died two turns later, cracking a fetch for its last life at seq42.
- Compounding it: the **plan/list mismatch** (wave-4 finding 5) recurred at seq26 (T9, life 5): PLAN "Cast Gray Merchant immediately" while GM was NOT in the list (only 4 lands); it chose "Cast nothing" over the available Fatal Push.
- Note a correct sub-observation the pilot made but couldn't act on: **Etched Champion has protection from black — Fatal Push and Collective Brutality's -2/-2 can't target it.** The deck's real answer to it (Liliana of the Veil -2, an edict that bypasses protection) wasn't drawn. See guide correction below.

The lesson: Gray Merchant is the deck's **stabilizer**, not only its finisher — casting it at low life gains life = drain. The guide frames it as a lethal closer (drain ≥ opp life); it needs a second, equally loud trigger: **when YOUR life is low and Gray Merchant is listed, cast it to gain the life.**

### 4. [TIMEOUT TRIAGE — winning-but-slow, not a guide bug] vs140 hit the cap while declaring lethal.
The flagged game (1783972794). Final record seq67, T21, my 6 / opp 2: the pilot **declared both attackers (Geralf's Messenger 4/3 + Bloodghast 2/1 = 6 power) at opp 2 = lethal** — the cap fell on the winning swing. This is winning-but-slow, matching wave-4's timeout pattern. Drivers, in order:
- **Latency, not passivity.** SIX calls at 64-74s (seq0 74.2s, seq19 65.7s, seq28 64.7s, seq44 65.9s, seq55 67.4s, seq61 69.0s) on top of a 68-record game ≈ ~19 min of deck133-side inference alone (`-j` concurrency saturating Spark). The flagged "67-second call" is one of six such spikes.
- **Matchup + variance.** deck140 is control/lifegain (Elixir of Immortality, Venser's Journal, Wrath of God). Opp life *rose* (20→21→22) repeatedly; **Wrath of God at seq52 wiped Obliterator + Bloodghast when opp was at 6**, resetting the clock. deck133 drew a hand flooded with 2× Fatal Push + Collective Brutality and no creature until Bloodghast T5, no second threat until Obliterator T13.
- **The 24/33 cast-nothing rate is not a durdle** — nearly all were correct targetless Fatal Push passes on an empty opponent board (seq8/9/10/14/16/23/24/32/34/40/42/44/45), each re-offered by the single-option re-ask multiplier.

Per the skill: this is infra + matchup + variance. **Write no anti-passivity pressure on its account.** If anything it argues for finding 1's faster close.

---

## OVERSHOOT AUDIT (mandatory)

Wave-4's loud additions, checked against this corpus for locally-obedient / globally-bad application:

- **`CAST A SPELL EVERY TURN - YOU ALWAYS HAVE THE MANA` (the act-floor opener) — MILD OVERSHOOT into low-impact discard.** In vs109 the floor was satisfied every turn by casting the cheapest thing available — discard — even while being raced (seq8/10/18, discard at life 11→5). The floor is net-positive (cast-nothing 36%→16%) and must stay; the overshoot is in *what* satisfies it. Fix is an **impact-ranked** floor when racing (removal/Obliterator/Gray Merchant rank above discard; discard is turns 1-3 or a known sweeper only), not a weaker floor. This is finding 2's fix, framed positively.
- **`ATTACK with every untapped creature when you are the beatdown` — NO overshoot.** The pilot correctly declined suicidal Bloodghast attacks into live blockers (vs109 seq13/seq20; vs110 seq28/seq40) and swung with everything when clear (vs44 seq31 three attackers; vs140 seq43/seq67). No attack-floor suicide this corpus.
- **`GRAY MERCHANT… cast him the same turn you reach 5 mana and every time he is in the list` — NO overshoot; if anything, UNDERSHOOT.** No premature low-devotion casts; the three casts were at devotion 6-10. The failure was the *opposite* — a missed cast at low life (finding 3).
- **DEFENDER role branch — NO passivity overshoot.** Where it fired (vs44) it produced the win, not a stall.

No wave-4 rule ran past its purpose into a durdle this corpus (contrast deck135's Island-fetch, deck110's re-equip loop). The one overcorrection is the act-floor feeding discard, addressed by ranking, not braking.

---

## Win narratives (preserve what worked)

- **vs44 (WON, opp -13) — the wave-4 DEFENDER template working, and it beat the exact deck (Faeries) deck133 lost to twice at opp-20 in wave-4.** Fatal Push on their creatures early (seq10/seq14 on Faerie Miscreant), **Phyrexian Obliterator cast as a 5/5 wall+attacker** (seq18), a second Obliterator, swung with all three for 12 (seq31, opp→1), closed with Liliana the Last Hope + attacks. The finding-8 template from wave-4 (deploy Obliterator + point removal at their board + attack) executed cleanly. **Also confirms the negative-life SBA fix:** the game ended promptly via a `gameend` record at opp -13 (contrast wave-4's vs140 stuck at -10). vs135 (-3) and vs131 (-1) likewise ended cleanly — the wave-4 E1 bug is resolved for this deck's drain kills.
- **vs131 (WON, opp -1) — the Gray Merchant plan working twice.** GM cast at seq40 (drain 6) to stabilize 15→21, then Liliana the Last Hope -2 returned it and GM cast AGAIN at seq44 (drain 8) for the kill. The recursion line the guide names actually happened. Yawgmoth/Liliana grind piloted correctly.
- **vs135 (WON, opp -3) — the lethal kill-check working.** seq43: GM listed `drains 8`, pilot read devotion 10, opp at 7 → cast for exact lethal (-3). Textbook use of the finisher.

The lever separating wins from losses is unchanged from wave-4: **wins deploy Obliterator + removal + Gray Merchant; losses flood discard and miss the Gray Merchant stabilize.** Half of that is the pilot (findings 2, 3); half is what got drawn (deck construction — see notes.md).

## What the pilot did WELL (do not regress)
- Impact-ranked discard/removal *target* picks stayed strong (vs140 Thoughtseize took Pyroclasm to protect its board seq28; vs109 Inquisition took Krenko's Command over token-flood; vs110 Thoughtseize took Cranial Plating).
- Multi-attacker lethal math is reliable (vs44 seq31; vs140 seq43/seq67).
- Fetch-cracking on first offer; Yawgmoth loop; Gray Merchant recursion via Liliana. Combat arithmetic is not the problem — *pre-combat deployment and the stabilize decision* are.

---

## Wave-4 findings — status this corpus

| Wave-4 finding | Status | Note |
|---|---|---|
| #1 mana-pool misread (P0) | **STAYS FIXED** | 0 vetoes; cast-nothing 16% ex-vs140. |
| #2 timeouts = winning-but-slow | **CONFIRMED** | vs140 declared lethal at cap; latency + lifegain, not passivity. |
| #3 discard-flood + can't-block Bloodghast vs aggro | **PERSISTING** | vs109 (opp 20). Primary guide-fixable leak this wave (finding 2). |
| #4 Gray Merchant barely deployed | **IMPROVING** | 3 casts, but now `drains N` shown; the miss (vs110) is a *decision*, not under-draw. |
| #5 plan/list mismatch on unaffordable cards | **PERSISTING (milder)** | vs110 seq26 (planned GM not in list → cast nothing over available Fatal Push). |
| #6 negative-life-without-game-end (E1) | **FIXED** | vs44 ended at -13, vs135 -3, vs131 -1 all via prompt `gameend`. |
