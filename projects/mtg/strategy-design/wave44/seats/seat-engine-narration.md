# Wave-44 engine+narration validation seat (harvested 2026-08-26)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-111937/` — 42 seat logs, 21 games
(20 timestamp-paired + one pair split across `1787761198`/`1787761199`), 2119 records of
which **2035 are decisions** (1442 ask, 455 priority, 91 attackers, 37 blockers, 6 reveal,
4 bottom). Delta corpus: 2178 unique history lines / 11111 instances.

Instrument note up front: several sweeps in this file needed a bracket/brace-aware splitter.
A naive comma split reports ~181 "blind" removal targets and ~838 battlefield count defects;
both vanish under a correct splitter. Numbers below are the corrected ones. Where I state a
count, the parse it came from is described.

---

## PREDICTION SET B — adjudication

### Fair hands (lane A) — **PASS**
Opening-hand land counts across the 41 recorded keeps: **2 lands x13, 3 x17, 4 x9, 5 x2**.
The rigged 3-land signature is gone. (42nd seat: `deck123-0x55a59c735ea0` keeps at seq 1;
its keep record has the header, my first regex missed it — no missing window.)
Mulligans are LIVE and behaved: **4 seats mulliganed, each exactly once, then kept**
(`deck146-0x563ae3950310`, `deck162-0x56513f82e9e0`, `deck125-0x558033705f60`,
`deck152-0x5580323c50b0`); 4 matching `bottom` records, 7 cards offered each, all bottomed a
real card. **Two of the four mulligans were 0-land hands** (deck162, deck152) — correct.
**No mull-to-zero, no repeated mulligan, no guide-floor collision → no OWNER QUESTION from
this seat.** No aggressivity-pin artifact found (no forced ability use on GPT seats).

### #W44-2 X-pricing headline opponent-first — **render PASS, behaviour PASS (n=3, low power)**
- Render: the opponent-first headline SHIPPED. 9/9 windows where any named X kills an
  opponent creature lead with `smallest X that kills an OPPONENT creature: X=N (M mana
  total) - kills THEIRS: ...; YOURS: ...`, and windows where nothing of theirs dies say so
  explicitly (`even at X=4 NOTHING the OPPONENT controls dies. At X=1 it kills only YOURS: ...`).
- Behaviour, strict metric (windows where some named X kills >=2 OPPONENT creatures):
  **2 of 3 cast = 67%**, clears the >=60% bar. **n=3. This is not a powered result** — one
  different decision flips it to 33%. Do not close the lane on it.
- Behaviour, >=1 opponent kill: **3 of 9 = 33%** — *identical to wave 43's 33%*. The render
  fix did not move the loose metric.
- Residual (new): **3 of the 6 Starstorm casts happened in windows the render explicitly
  said kill nothing of theirs** — `deck130-0x5653dcbb8` seq 104 (`there is no creature on
  the battlefield for it to damage`, cast at X=7), `deck130-0x55d5df63c` seq 15, and
  `deck130-0x55894935d` seq 76 (`kills only YOURS: Dwarven Blastminer, Goblin x2` — it cast
  it anyway and killed three of its own). Surface was true and explicit; STRATEGY, route to
  the deck-130 guide seat.

### #W44-3 removal `{target text:}` — **PASS**
304 option rows carry `legal targets right now:`; **862 card-target entries** (excluding the
257 `the opponent`/`you` player targets). **368 carry an explicit `{target text: ...}`;
313 are same-name repeats of an entry that already showed its text** (`Mountain #1 ... {target
text: "R"}, Mountain #2 (land)` — a deliberate dedup); the remaining **181 are legitimately
textless**: 149 tokens (Human/Goblin/Vampire/Spider/Wolf — verified: they carry their keyword
tags inline, e.g. `Vampire (creature 1/1) [lifelink]`) and 32 dual lands whose primitive has
no `text=` line at all (verified `Scrubland` in `mtg.txt:100808` — `type=Land`,
`subtype=Plains Swamp`, no `text=`). **Zero suppressed-text entries. 0 blind removal choices.**
One real gap falls out of this (see NEW-2 below): a dual with no `text=` renders as
`Scrubland (land)` with no colour info, while `Woodland Cemetery` beside it renders its full
mana line — asymmetric information on land-destruction target menus.

### #W44-4 Day/Night line in CURRENT SITUATION — **FAIL (unchanged, not shipped)**
**0 of 2035** decision prompts carry a Day/Night state line anywhere in the situation frame.
229 records render a daybound/nightbound card. The state DOES exist in the game log — the
delta corpus contains `- It became Night` and `- It became Day` — so the model can only
recover current day/night by scanning history for the last transition. Wave-43 measured
0/2190; this is 0/2035. Repro: `1787761199-ai_baka_deck152-0x561b794e74e0-vs-ai_baka_deck146.jsonl`
seq 20 (Tovolar's Packleader / Moonrage Brute live on board, no state line).

### #W44-5 teferieffect + dungeon zone tags — **SPLIT: dungeon PASS, teferieffect FAIL**
- Dungeon `[your zone]` tag: **0 occurrences** corpus-wide (was 84). 13 unique dungeon
  history lines, all clean (`- You used: choose a new dungeon with Nadaar, Selfless Paladin
  targeting Lost Mine of Phandelver`). **PASS.**
- `teferieffect`: **29 prompt-instances, 3 unique lines, all in one seat**
  (`1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl`, seq 52 / 55 / 62),
  all the stack row `N: ability: teferieffect Counter Removed [triggered/activated ability]`.
  Identical count to wave 43 (29). **Not fixed.** Same seat, same path.

### #W44-6 reveal false frames — **PASS**
All 6 `reveal` records checked line by line. Library counts are honest and self-reconciling:
`Your library: 28 cards (3 of them are the cards listed in the search/reveal below - they are
still in your library until this decision resolves)`. Opponent hand size is a real number in
all 6 (5,4,4,6,5,5) — **no hand-size-0 mid-reveal frame, no library-count lie**. The
whole-library searches correctly state `all 49 cards of it are listed below. This is a search,
not a look at the top of your library`.

### #W44-7 plan-note gate — **PARTIAL PASS; the attackers half PASSES, precision does not**
- Predicted half: **attackers 0/91, blockers 0/37, reveal 0/6, bottom 0/4.** The gate no
  longer fires on combat windows at all. **PASS.**
- Overall fire rate **870/2035 = 42.8%** (wave 43: 34%, all-false). It fires only on
  `ask` (726) and `priority` (144).
- **Cross-tab (fire vs actual mismatch)**, using a 27,691-name primitive vocabulary to test
  whether any card the plan names appears anywhere in the option list:

  |                        | plan-named card IS in the list | is NOT in the list |
  |------------------------|-------------------------------:|-------------------:|
  | note FIRED             | **165** (false fire)           | 705 (correct)      |
  | note SILENT            | 817 (correct)                  | **269** (missed)   |

  Precision **705/870 = 81%**, recall **705/974 = 72%**. (36 further windows had a plan
  naming no card at all.) The overlap proxy is loose — a plan-named card can appear as a
  *target* rather than as the action — so I also ran a **strict** test: gate fired although an
  option row *literally begins* `Cast <exact name the plan says to cast>`. That yields
  **5 hard false fires**: `deck146-0x5559288b224` seq 84, `deck126-0x5603808fbfc` seq 9,
  `deck126-0x563b923035e` seq 39, `deck146-0x563b90fca49` seq 24 and seq 72. A sixth by
  inspection: `deck125-0x5653db89c3b` seq 25 — plan says *"Clear the board with Supreme
  Verdict now"*, option 2 is `Cast Supreme Verdict {1}{u}{w}{w}`, and the note fires anyway.
- **Land-drop over-fire: 327 of 419 land-drop windows.** Technically true (a land-drop menu
  can never contain a cast) but it directly contradicts the block three lines above it in the
  same prompt: *"it is never listed in a casting, ability or target menu, so its absence from
  the choices below does not mean it is gone."* One prompt says absence is meaningless and the
  next says the absence is worth flagging.

### Lethal basis / forecast bRemaining — **PASS 37/37**
Every blockers window carries `Your life: L. Unblocked, these attackers deal up to D - you
would be at R - LETHAL/NOT lethal`. **37/37 satisfy L-D==R; 37/37 have L == the record's
`my_life`; 37/37 have the correct verdict polarity, including the R==0 case** which renders
`at 0 life you LOSE - 0 is not survival`. 7 windows flagged LETHAL, 30 NOT lethal.
Trade verdicts use remaining toughness with marked damage folded in (spot-checked against
`(printed X/Y)` annotations); the known who-dies-preview-on-marked-damage deferral produced
no new instance I could isolate in 31 real blocks — logged as UNTESTED this corpus, not
re-docketed.

### Loyalty "lost (now N)" — **PARTIAL; root cause is sharper than wave 43 recorded**
Wave 43 filed this as "lost lines carry no (now N); got-lines do". That is **not** the split.
The split is **plural vs singular**:
- `lost 3 loyalty counters (now 1)` — 4/4 unique lines carry `(now N)`. OK.
- `got 4 loyalty counters (now 4)` — 12/12 carry it. OK.
- `lost a loyalty counter` — **0/6 carry it.**
- `got a loyalty counter` — **0/4 carry it.**
So the defect is the **singular-count code path** (`a loyalty counter`), on *both* directions,
not the loss direction. 10 lines affected. Repro: `deck146-0x563ae3950310` seq 20
(`- Opponent's Ob Nixilis, the Hate-Twisted lost a loyalty counter`);
`deck126-0x55df6d9a1f20` seq 23 (`- Your Sorin, Lord of Innistrad lost a loyalty counter`).

### Lifelink / deathtouch clauses in trade verdicts — **PASS**
Wave-43 E-3 (`(both die)` omits blocker lifelink) is **fixed**: blocker rows now read
`A2 (both die (lifelink: you gain 1))`, `A3 (your blocker dies, attacker lives (lifelink: you
gain 1))`, and attacker rows carry the mirrored `(both die (lifelink: they gain 1))`.
Repro of the fixed shape: `deck126-0x563b923035e` seq 20 B2; `deck146-0x563b90fca49` seq 25.
Deathtouch: 37 rows render `[first strike, deathtouch]` and the paired verdicts are correct
(4/4 Sigarda blocking a 1/1 first-strike deathtoucher → `your blocker dies, attacker lives`,
verified at `deck152-0x561b794e74e0` seq 20).

---

## FALLBACKS — 6 / 2035 = **0.29%** (wave 43: 4/2317 = 0.17%)

| # | file | seq | mode | window |
|---|------|-----|------|--------|
| 1 | `...deck162-0x5652b65f6280-vs-...deck123` | 17 | stale_echo | damage-order, 3 identical `Vampire (2/2)` rows |
| 2 | `...deck146-0x563b90fca490-vs-...deck126` | 35 | stale_echo | Silverquill Command modes (`creature gains 3/3 and you draw`) |
| 3 | `...deck152-0x5580323c50b0-vs-...deck125` | 52 | stale_echo | Teferi (`choose a land` / `Decline`) |
| 4 | `...deck152-0x5580323c50b0-vs-...deck125` | 53 | stale_echo | Teferi (`choose your land` / `choose opponent land`) |
| 5 | `...deck152-0x5580323c50b0-vs-...deck125` | 62 | stale_echo | Teferi (`choose a land` / `Decline`) |
| 6 | `...deck162-0x5589480413b0-vs-...deck130` | 37 | retracted_choice | 6-way casting menu |

**Mechanism of all 5 stale_echo: the wave-43 plan-name leak, unchanged.** Every one is an
**anonymous option menu** — rows with no card name to copy (`choose a land`, `Decline - do
nothing`, `creature gains 3/3 and you draw`, three byte-identical `Vampire (2/2)` rows). The
reply format demands `CHOICE: N (SHORT NAME)`; with no name in the row the model back-fills
the name from its own PLAN line, and the echo check rejects it:
- seq 17 reply `CHOICE: 1 (Cast Fate Unraveler)` — plan line reads *"Cast Fate Unraveler {3}{b}
  with remaining mana"*.
- seq 35 reply `CHOICE: 6 (Silverquill Command)` — plan names Silverquill Command.
- seq 52/53/62 replies `CHOICE: 1 (Tap Staff of Nin)` / `CHOICE: 2 (Staff of Nin)` — plan line
  reads *"Tap Staff of Nin to stop its damage"*.
5/5 plan-name leaks. Same mechanism as wave 43's 4/4. Two further `parse_note:
echo_index_conflict` records (recovered, not fallbacks) sit in the same two seats
(`deck130-0x55894935dbc` seq 57; `deck152-0x5580323c50b0` seq 64, 75), same cause.

**#6 is a genuinely new mode.** The reply's first line is a valid `CHOICE: 3 (Cast Howling
Mine)`, then ~2400 characters of open deliberation follow it, ending *"So the first choice is
Underworld Dreams… CHOICE: 4"*. The harness saw the retraction and correctly refused to commit
a contradictory answer. Failure of the "Write nothing else" constraint, not of the echo check.

**Is 0.17% → 0.29% a regression? NO — not demonstrable, and the mechanism did not change.**
6 events on n=2035 against a 0.17% base rate has an expectation of 3.5; Poisson P(X>=6)≈0.16.
One low-power sample cannot separate a moderate effect from noise. More decisive than the
statistic: **3 of the 6 sit in a single seat, all produced by one card** (Teferi, Who Slows
the Sunset, which is also the sole source of all 29 `teferieffect` labels). This is a
per-card concentration, not a rate drift. Verdict: **rate NOT regressed; the plan-name-leak
item (#W44-7 core-prompt half) is undischarged and now has a precise trigger — anonymous
option rows.**

## THE 679s DECISION — found
`1787761215-ai_baka_deck126-0x563b923035e0-vs-ai_baka_deck146.jsonl` **seq 14**, 679,138 ms.
- Prompt size is **not** the cause: 7,232 chars, *below* the corpus median of 8,512.
- Cause is **output**: an 11,372-char reply on a three-option **land drop** — the model dumped
  its whole mana-arithmetic derivation into the answer channel, re-deriving Sanguine Bond's
  cost five times, then re-emitted `CHOICE: 2 (Play Woodland Cemetery)` at the end.
- **No fallback, no retraction** — the closing CHOICE matched the opening one, so the answer
  committed correctly. It cost ~11 minutes of a 21-way-parallel run.
- Same root as fallback #6: the reply-format rule is stated but not enforced by a decode stop.
  Corpus-wide, `post_plan_overrun > 0` on 27 records; the top overruns are 5065, 3606, 2667,
  2341 chars.
- Contributing: the plan-note gate fired on this window too (a land drop — see #W44-7), which
  is what set the model re-litigating what its plan should be.
- Latency context: median 13.5s, mean 20.0s, p95 47.7s. This one is 14x p95. The next five
  slowest (485s/462s/414s/352s/277s) are all the same shape: reply 6k-26k chars.

---

## WAVE-43 CLEAN ITEMS — re-checked, all still clean
- rules-text tails in history: **0** unique delta lines carrying `{card text:`/`{text:`/"rules text".
- mana-hint tails in history: **0**.
- `put a put`: **0**. token rules-text leak: **0**.
- damage lines: 412 unique, all carry `(now N)` on life changes; life lines 187, all `(now N)`.
- DFC / Flip Side: **123 rows, 123 labelled** — 122 with the land wording (`DISPLAY TOGGLE
  only …`), 1 with the hand-card wording (`display toggle only - no game effect: switches
  which face this hand card shows`, `deck146-0x5559288b224` seq 62). Different case, honest text.
- battlefield frame integrity: **3839/3839 lines** have `permanents listed` == the true item
  count once run-collapse is expanded, and **1173/1173 collapsed runs** have span == the `xN`
  multiplier (`Mountain #1-#3 [tapped] x3`). Run collapse is now live (wave 43 saw 0 runs) and
  it is honest.
- cross-seat observer parity: **0 defects / 21 games** — both seats' `gameend` agree on
  life totals (mirrored), turn number, and complementary win flags.
- menace ATTACK side: still correct — 6+ attacker rows carry `[menace - cannot be blocked by
  fewer than two creatures; they have 0 untapped creatures that could join …]`.
- ANNOUNCE_X: only 1 alt-cost window arose (`deck130-0x5653dcbb835` seq 105, cycling) —
  **UNTESTED** this corpus, no re-offer defect.

---

## NEW -> wave-45 docket

**HIGH-1. Menace BLOCK side is a live false affordance — lane #W44-8 is no longer
corpus-unreachable, discharge it with the real fix, not a probe deck.**
3 blockers windows contained a menace attacker. **8 of 8 blocker-row entries offering that
attacker render a bare 1-on-1 trade verdict with no menace caveat**, e.g.
`B2. Bloodline Keeper (5/5) [flying] - may block … A4 (you kill it, your blocker lives)`
where A4 is `Hive of the Eye Tyrant (4/4) [menace]`. The attacker line above it *does* carry
the caveat, so the same prompt contradicts itself. The model duly declared `B2:A4` alone; the
engine silently dropped it (`dropped_assignments: 1`) and Hive's 4 damage went through. The
Pride Guardian defender tag makes it worse — `[deals 0 - this block kills nothing, but it
STOPS all of the damage from whichever attacker it blocks]` is flatly false for a solo block
on a menace attacker.
Repro: **`1787761186-ai_baka_deck123-0x5557c3baf5d0-vs-ai_baka_deck146.jsonl` seq 17** (also
`deck126-0x563b923035e` seq 47 and seq 57 for the render half without a committed solo block).
Fix shape: the block-side row must either suppress the menace attacker's 1-on-1 verdict or
prefix it exactly as the attack side does. Trust doctrine: this is a false surface -> HIGH.

**HIGH-2. Plan-note gate asserts a falsehood about the very menu on screen.**
5 strictly-verified false fires (option row literally begins with the plan-named `Cast <name>`)
plus a name-overlap population of 165. The note reads *"this decision's list does not contain
the actions your plan names"* — when it is wrong it actively steers the model off the correct
line. Precision 81%. Repro: **`1787761192-ai_baka_deck146-0x5559288b2240-vs-ai_baka_deck125.jsonl`
seq 84** (`Cast Acererak the Archlich` present in list, plan names it, note fires); also
`deck126-0x5603808fbfc` seq 9, `deck126-0x563b923035e` seq 39, `deck146-0x563b90fca49`
seq 24 / 72; and by inspection `deck125-0x5653db89c3b` seq 25 (Supreme Verdict).
Sub-item, same predicate: **327/419 land-drop windows fire**, contradicting the land-drop
block's own *"its absence from the choices below does not mean it is gone"* three lines
earlier. Suppress the note on land-drop and on menus whose kind structurally excludes casts.

**HIGH-3. New lexicon leak: raw `Transform:backside` in option rows AND history.**
Not recorded in wave 43. **57 prompt-instances, 13 unique lines**, e.g.
`4. Transform:backside with Bloodline Keeper [cost: {b}]` and
`- You used: Transform:backside with Bloodline Keeper #1`.
Verified against the primitive: `mtg.txt:12582` Bloodline Keeper has `backside=Lord of Lineage`
and `auto={B}:flip(backside)` — `backside` is an internal placeholder, and the card's real
`text=` says *"{B}: Transform Bloodline Keeper."* The engine already resolves this placeholder
on ONE path (`AIPlayerGPT.cpp:8494`, `if (otherName == "backside")`) — which is why the Flip
Side rows read correctly — but the ability-name/stack-label path does not. Same code locus and
same one-line shape as the still-open `teferieffect` item; **fix them together.**
Repro: `1787761184-ai_baka_deck123-0x55e881efea80-vs-ai_baka_deck126.jsonl` seq 36 (option row),
seq 43 (history line).

**MED-4. `teferieffect` stack label — carried forward unfixed (29 instances, 3 lines).**
Repro: `1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl` seq 52 / 55 / 62.
The wave-43 one-liner at `stackAbilityName` was not shipped. Bundle with HIGH-3.

**MED-5. No lethal headline on the ATTACK side (0 / 91 attacker windows).**
The blockers window gets `Your life: L. Unblocked, these attackers deal up to D - you would be
at R - LETHAL/NOT lethal` and it is 37/37 correct. The attackers window gets **nothing
equivalent** — the model must hand-derive whether its own swing is lethal, and does so in
prose (`"deal 12 damage to win"`, `"reducing opponent to 8 life"`). Given how much weight the
prompt puts on *"Do not re-derive these outcomes; use them"* for the block side, forcing
manual arithmetic on the attack side is the same class of exposure, on the side that wins
games. Repro: `1787761180-ai_baka_deck152-0x557785f52f40-vs-ai_baka_deck162.jsonl` seq 26
(opponent at 19, A1 8/8 + A3 3/1, no headline).

**MED-6. `[defender]` on the battlefield line has no gloss — it seeded 7 false model beliefs.**
The CURRENT SITUATION board row renders `Fog Bank {1}{u} (0/2) [flying, defender]`. The option
row for the same card renders the gloss: `Defender (This creature can't attack.)`. The blockers
window renders a third, fuller gloss. From the bare tag the model concluded **"Fog Bank cannot
block"** and attacked into it on that basis — 7 replies in one seat
(`deck152-0x557785f52f40` seq 7, 8, 9, 10, 25, 26 and the seq-26 plan), corpus-wide 14 replies
assert `<X> cannot block` for a card that could block. The surface is *honest but under-glossed*,
and the honest gloss already exists two renderers away. Cheapest fix in this docket.
Repro: `1787761180-ai_baka_deck152-0x557785f52f40-vs-ai_baka_deck162.jsonl` seq 7.

**MED-7. INSTRUMENT: `dropped_assignments` conflates a declined blocker with a refused one.**
4 blockers records carry a non-zero value. Only **one** is a real drop
(`deck123-0x5557c3baf5d` seq 17, the solo menace block). The other three count the model's
explicit `Bn:none` declarations — which the prompt itself teaches
(`"BLOCKS: B1:A2, B3:A1, B2:none"`) — as drops: `deck162-0x557787294490` seq 17 (`B4:none`),
`deck130-0x55882f2454d0` seq 32 (`B1:none`), `deck162-0x5589480413b0` seq 17 (two `:none`).
Wave 43's "dropped-assignment accounting: corpus total 1, non-menace, correctly accounted" was
computed on this same conflated metric and should be re-read as unverified. This is a
metric-not-behaviour defect; it costs review accuracy, not games.

**MED-8. Anonymous option rows are the fallback generator — the `SHORT NAME` echo has nothing
to copy.** All 5 stale_echo fallbacks and both `echo_index_conflict` recoveries are on rows
carrying no card name (`choose a land`, `Decline - do nothing`, `creature gains 3/3 and you
draw`, N byte-identical `Vampire (2/2)` rows). Two independent fixes exist and either would
close it: (a) name the source on such rows (`choose a land for Teferi, Who Slows the Sunset`,
`Vampire (2/2) [blocker #3 of 6]`), or (b) accept a numeric-only echo when the row has no
proper name. Repro: `1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl`
seq 52 / 53 / 62.

**MED-9. Reply channel has no decode stop — one decision cost 679s and one lost its answer.**
Same root, two symptoms: seq 14 of `deck126-0x563b923035e0` (11.4k-char reply on a land drop,
679s, answer survived) and the corpus's only `retracted_choice`, `deck162-0x5589480413b0`
seq 37 (~2.4k chars of deliberation after a valid CHOICE line, ending on a contradictory
CHOICE, answer discarded). 27 records have `post_plan_overrun > 0`, up to 5065 chars.
A stop sequence after the PLAN line converts both into normal decisions.

**MED-10. Dual lands with no `text=` render as bare `(land)` in target menus.**
`Scrubland (land)` beside `Woodland Cemetery (land) {target text: "…{T}: Add {B} or {G}."}` in
the same Molten Rain target list. Verified in the primitive: `mtg.txt:100808` Scrubland has
`type=Land`, `subtype=Plains Swamp`, and no `text=` line — so the *renderer* is honest, but the
engine knows the subtypes and the model cannot tell which land is worth destroying. 32 entries.
Synthesize a mana line from subtype when `text=` is absent.
Repro: `1787761212-ai_baka_deck130-0x55df6ecbf9d0-vs-ai_baka_deck126.jsonl` seq 12.

**LOW-11. `(now N)` missing on the SINGULAR loyalty path, both directions.**
Not "lost lines" as wave 43 recorded — `lost a loyalty counter` 0/6 and `got a loyalty counter`
0/4, while the plural forms are 4/4 and 12/12 correct. 10 lines.
Repro: `1787761194-ai_baka_deck146-0x563ae3950310-vs-ai_baka_deck162.jsonl` seq 20.

**LOW-12. Damage/history lines identify the source by bare card name with no owner prefix.**
402 history lines lack a `Your`/`Opponent's` prefix, e.g. `- Luminarch Aspirant dealt 1 damage
to the opponent (now 19)`. Harmless in the seats I checked, but the corpus contains mirrors
(both deck123 and deck146 run Nadaar and Hive of the Eye Tyrant), where the line is genuinely
ambiguous. This is wave-43 LOW item 5 restated with a concrete ambiguity case; it still needs
the owner's ruling on log-shape parity vs owner tags rather than a patch.

**LOW-13. Dedup in target lists is unmarked.** `Mountain #1 (land) {target text: "R"}, Mountain
#2 (land)` — the second entry is indistinguishable in shape from a genuinely textless card.
Inferable by name, but a `(text as above)` marker would remove the guess.

---

## Lategame specimen candidate
`1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl` **seq 108** — turn ~50 of
the corpus's longest game (52 turns, 112 records), 13-permanent board, empty hand, Teferi with
bookkeeping counters, and it is the seat that carries the teferieffect labels and 3 of the 6
fallbacks — one prompt exercising several open lanes at once.
Alternate, if a maximal frame is wanted over a maximal lane count:
`1787761215-ai_baka_deck126-0x563b923035e0-vs-ai_baka_deck146.jsonl` seq 57 — the corpus's
largest prompt at 30,710 chars, a turn-26 blockers window that also contains the menace
false-affordance render (HIGH-1).

---

## Falsifiable predictions for wave 45
- **P-E1** After the HIGH-1 fix: blocker rows offering a menace attacker that carry a bare
  1-on-1 verdict **0/N**; solo blocks declared on a menace attacker **0**.
- **P-E2** After the HIGH-2 fix: plan-note fires on land-drop windows **0/~400**; strict false
  fires **0**; overall fire rate falls to **<25%** with precision **>=95%**.
- **P-E3** After HIGH-3 + MED-4: `Transform:backside` **0** prompt-instances, `teferieffect`
  **0**.
- **P-E4** After MED-6: replies asserting `<X> cannot block` for a card that can block
  **<=1** (from 14).
- **P-E5** After MED-8: `stale_echo` fallbacks **0**; total fallback rate **<=0.15%**.
- **P-E6** After MED-9: `post_plan_overrun > 0` records **<=3** (from 27); max latency
  **<120s**; `retracted_choice` **0**.
- **P-E7** Day/Night: if the MED fix ships, a `Day/Night` state line present in **>=229**
  records (every record rendering a daybound/nightbound card). If it does not ship, this is
  its third consecutive FAIL and should be re-ranked or explicitly deferred by the owner.
- **P-E8** X-pricing behaviour needs power, not another render change: **do not re-run the
  >=60% metric on n=3.** Wave 45 should either accumulate kills>=2 windows across corpora
  until n>=15, or drop the numeric prediction and keep only the render check.

---

## What I did NOT check
- I did not adjudicate prediction **set A** (wave-43 guide edits) — no decks assigned to this
  seat, no `deckN/` outputs written, per the task framing.
- I did not verify the who-dies preview against marked damage in a case where they diverge;
  no isolable instance arose in 31 real blocks. Logged UNTESTED, not re-docketed.
- I did not open `AIPlayerGPT.cpp` beyond locating the `backside` placeholder resolution
  (lines 8489-8500, 10211, 10272) and confirming the one-path-only fix; I did not trace the
  ability-name renderer or propose a patch site.
- I did not check the Baka menace gang-block pin, the mana-window auto-pass economics
  (#W44-9), CS-024a, or the manarestriction sweep — carried wave-45 seeds, out of this seat.
- Observer parity was checked at `gameend` only (life/turn/win mirror across 21 games), not
  line-by-line through each game's history.
- Cross-seat history-line parity, and the wave-43 "targeting clauses unowned 231/365" recount,
  were not re-run in the same form; LOW-12 restates the item from the delta corpus instead.

## ORCHESTRATOR CORRECTION (2026-08-26, verified against corpus + code)
#W44-4 Day/Night verdict FAIL is WRONG -> **PASS**. The shipped line (lane B, ee0cd3f83,
`dayNightStateLine`) renders as `It is currently Day.` / `It is currently Night.`, not a
literal "Day/Night" token. Corpus count: 236 records across 4 seat logs carry the state
line (`/usr/bin/grep -c 'It is currently \(Day\|Night\)'`), against 229 records rendering
a daybound/nightbound card; transition narration `It became Day/Night` = 1040 lines.
Prediction P-E7 is therefore already met; drop the "unshipped" docket entry. Lesson for
reviewer briefs: adjudicate a render prediction by the EMITTER's actual string (read the
code or a PARSETEST CHECK), never by the ledger's shorthand name.
