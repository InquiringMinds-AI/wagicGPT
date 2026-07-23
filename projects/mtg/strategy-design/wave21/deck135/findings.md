# Deck-135 (Modern Snow) — Wave-21 game review

Corpus: PRIMARY `matchups-20260723-084938` (21-game round-robin over 135,62,49,35,14,27,102;
binary `/tmp/wagic-3be7f5a67` = wave-21 engine batch, LIVE-UNVALIDATED). Pool UNCHANGED from wave-20
(clean repeated-opponent control — every opponent identical, so the record is directly comparable
game-for-game). deck135 guide FROZEN (byte-identical to live `Res/ai/baka/deck135_strategy.txt`,
`cmp`-verified). deck135 OWNS: ENGINE-R1 second confirmation, Azcanta activation window, HARNESS-1
retracted-CHOICE, and THE REGRESSION QUESTION (4/6 -> 1/6). Doctrine: win column CONTEXT ONLY; unit
of analysis is the DECISION with seq repros.

## RECORD (context only — NOT a verdict) — 1W / 5L  (DOWN from wave-20's 4/6)

| game    | opp                | wave-20            | wave-21              | my/opp | turn | flip? |
|---------|--------------------|--------------------|----------------------|--------|------|-------|
| 135v62  | 62 Enchantresses   | WIN (real 13/0)    | WIN (adj 17/14)      | 17/14  | 11   | same  |
| 135v35  | 35 Mighty Slivers  | WIN (real 4/-8)    | LOSS (real -18/10)   | -18/10 | 9    | FLIP  |
| 135v102 | 102 Tergrid        | WIN (real 9/-4)    | LOSS (adj 5/19)      | 5/19   | 13   | FLIP  |
| 135v14  | 14 Deep Blue       | WIN (adj 15/3)     | LOSS (adj 13/18)     | 13/18  | 13   | FLIP  |
| 135v49  | 49 Dragons         | LOSS (adj 2/14)    | LOSS (0/18)          | 0/18   | 9    | same  |
| 135v27  | 27 Zombies         | LOSS (real -7/3)   | LOSS (real -4/19)    | -4/19  | 11   | same  |

Win table assigns deck135 1/6; matches (only vs62, adjudicated ahead, is a win). THREE flips, all
former wins: vs35, vs102, vs14. The two losses that stayed losses (vs49, vs27) are the same matchup
losses as wave-20. So the whole 4->1 swing lives in the three flipped former-wins.

GAME -> FILE MAP (run = matchups-20260723-084938; owner seat = first name):
- vs62  `1784817890-ai_baka_deck135-0x556e157c48a0-vs-ai_baka_deck62.jsonl`  (WIN adj)
- vs35  `1784814581-ai_baka_deck135-0x557b7757e950-vs-ai_baka_deck35.jsonl`  (LOSS)
- vs102 `1784822734-ai_baka_deck135-0x558a4ea24920-vs-ai_baka_deck102.jsonl` (LOSS)
- vs14  `1784823476-ai_baka_deck135-0x55853041fb30-vs-ai_baka_deck14.jsonl`  (LOSS)
- vs49  `1784816251-ai_baka_deck135-0x5652bf9de9e0-vs-ai_baka_deck49.jsonl`  (LOSS)
- vs27  `1784823985-ai_baka_deck135-0x561b50440a90-vs-ai_baka_deck27.jsonl`  (LOSS)

---

# HEADLINE: THE 4/6 -> 1/6 REGRESSION IS DRIVEN BY AN ENGINE BUG, NOT DRAW VARIANCE / NOT ANSWER-FIRST / NOT THE GUIDE.

## Mechanism: FETCHLAND ACTIVATIONS INTERMITTENTLY FIZZLE (item-8(c) class, caught on fetchlands). deck135 is the most fetch-dependent deck, so it bleeds the most.

Definitive repro — vs35 s6 (turn 2, priority): model chose `Put in Play with Windswept Heath targeting
Snow-Covered Forest` (self-contained activated ability, cost `[Tap, Life, Sacrifice]`). Cumulative
event log (s21), verbatim:
```
- Your Windswept Heath: hand -> battlefield
- You: Put in Play with Windswept Heath targeting Snow-Covered Forest [cost: Tap, Life, Sacrifice] ...
- Your life -1 (now 19)                <-- LIFE COST PAID
                                        <-- NO "Windswept Heath: battlefield -> graveyard" (sacrifice MISSING)
                                        <-- NO "Snow-Covered Forest: ... -> battlefield" (land NEVER entered)
```
The fetch fizzled after paying Tap+Life but NOT completing the Sacrifice+search. The fetchland stayed
(tapped), the snow land never entered, and the option RE-ARMED (options grew 2->4 as the 2nd failed
fetch, Misty Rainforest s12, piled on). At the final record both Windswept Heath AND Misty Rainforest
sit uncracked with all four `Put in Play` options re-offered. deck135 spent the whole game GREEN-SCREWED:
hand at s20 held `Abominable Treefolk {2}{g}{u} (5/5)`, `Ice-Fang Coatl {g}{u}`, 3x `Into the North
{1}{g}` — ALL uncastable for lack of green, because the green fetch fizzled. Died -18/10 with finisher +
card-draw creatures stranded. In wave-20 this exact matchup was a WIN with `Treefolk 9/9 -> opp -8` —
the only difference is the fetch resolving.

INTERMITTENT, not total — proven by contrast with the WIN. vs62 (sole win) shows a fully-resolved fetch
(s28 log): `Put in Play with Flooded Strand targeting Snow-Covered Island` -> `Your Flooded Strand:
battlefield -> graveyard` (SACRIFICE completed) -> `Your Snow-Covered Island: library -> battlefield`
(LAND entered). deck135 developed, cast `Treefolk (13/13)` + Coatl, won. vs62 s14->s15 the model
RE-ATTEMPTED the same Flooded Strand fetch on consecutive priority windows until it stuck — re-attempt
is what saved it.

Fizzle scan (fetch-crack CHOICEs vs successful fetchland-sacrifice events; "snowlands entered" also
counts Into-the-North sorcery searches so it over-reads success):

| game  | result | fetch-crack choices | successful sacrifices | snowlands entered |
|-------|--------|---------------------|-----------------------|-------------------|
| vs62  | WIN    | 2                   | 1                     | 3                 |
| vs35  | LOSS   | 2                   | 0                     | 0                 |
| vs49  | LOSS   | 3                   | 0                     | 0                 |
| vs27  | LOSS   | 5                   | 0                     | 0                 |
| vs102 | LOSS   | 3                   | 0-1                   | 1                 |
| vs14  | LOSS   | 5                   | 2                     | 3                 |

Three total-fizzle games (vs35, vs49, vs27) had ZERO fetches resolve. The one win had working fetches.
deck135's whole plan is "fetch a color, cast a {G}{U} threat"; when the fetch engine fizzles the deck
cannot function.

## Why the model never recovers: it CANNOT PERCEIVE the fizzle, so it stops re-cracking.

The guide already says to re-crack if no land entered ("if you still have no extra land in play, you
have NOT cracked yet — crack it now"). But in vs35 the model's PLAN narrated the fetch as successful —
s6 plan: "Activate Windswept Heath to fetch a Snow-Covered Forest. Once the Forest is in play, I will
tap it and cast Icehide Golem." Believing the Forest was in play, at every later priority window that
re-offered the fetch (s8/s9/s14/s15/s18/s19/s21) it chose PASS, never re-cracking. The guide instruction
is correct and obeyed-in-principle; the model just can't tell the fetch FAILED because the failure is
SILENT (no negative event, no "cast failed" line) and its own plan asserts success. MODEL-PERCEPTION +
ENGINE-SILENCE, NOT a guide gap. See notes ENGINE-F1.

## Honest causation framing (evidence-led, not defensive).

- item-8(c) — the click-refusal / activated-ability fizzle class — was DIAGNOSED FROM WAVE-20 DATA (see
  `wave21/item8-diagnosis.md`), so the bug EXISTED in wave-20. wave-21 shipped item-8 (a)+(b)
  (disabled-producer counting + clickstream abort) but those target mana-PRODUCER refusals; the fetch
  Sacrifice-cost sub-click was NOT fixed. So the 4->1 swing is substantially the SAME persistent
  intermittent bug landing on DIFFERENT games this corpus (variance in WHICH games the fizzle strikes),
  not a new wave-21 side-effect and not a play-quality degradation.
- Flag for engine devs: wave-21 (b) ("on refusal abort the remaining plan") is IN this code path, and
  the vs35 signature is "pay-life-then-fizzle" (a real cost paid, then aborted). It is possible (b)
  changed the failure MODE from item-8's original "float mana / loop" into "pay a partial cost then
  abort." Cannot discriminate without wave-20 fetch-level data or a code probe. Either way the fetch
  does not resolve. Route: notes ENGINE-F1 (corpus's top new engine finding).
- Fully consistent with win-column-is-context + small-n variance discipline: a 3-game swing on n<=6 with
  an adj-heavy slate, driven by an intermittent engine bug hitting a fetch-dependent deck, is NOT a
  decision-quality regression. Core prompt is symmetric and unchanged; guide frozen.

## Per-flip decomposition

- vs35 (FLIP, dominant cause = FETCH-FIZZLE): both fetches paid life and fizzled -> permanent green
  screw -> Treefolk + Coatl + 3x Into the North stranded -> raced out -18/10. The final all-in at s20
  (both Golems at 4 life) was FORCED/correct — the reply assessed the 9-damage swingback and read the
  game as already lost ("I cannot block effectively against four attackers with only two 2/2s"). No
  decision error; lost at the mana base.
- vs102 (FLIP, cause = fetch-fizzle (partial) + value-heavy draw vs Tergrid): kept life high early (20
  through turn 6) but only `Boreal Druid` (1/1 dork) was ever castable through turn 6 — NOT a Rule#1
  durdle (scan of s7-s16 shows NO skipped `Cast <creature>` option; there were none). Treefolk (8/8)
  landed too late; ground 20->18->13->5. Slow development from mana clunk + a value-heavy draw against
  the pool's grindiest deck.
- vs14 (FLIP, cause = AZCANTA TRANSFORM DECLINED TWICE + grind; fetches mostly worked): see Azcanta
  section. Here fetches largely resolved (2 sac / 3 enter) but the model declined a beneficial, available
  Azcanta transform twice due to a graveyard miscount, then was ground out 13/18.
- vs49 / vs27 (NOT flips, repeat matchup losses): vs49 raced by the Dragon clock (LOSS both waves;
  Diamond Faerie stranded needing G/W/U — construction), vs27 ground out by Zombies (LOSS both waves).
  Both also suffered total fetch-fizzle, which worsened but did not create the loss.

---

# ENGINE-R1 SECOND CONFIRMATION — CONFIRMED FIXED. n now = 2. Item can CLOSE.

wave-20 R1 proof was n=1. This corpus fired a second, and it LANDED.

vs14 s23 (turn 8, Main phase 1): model cast Glacial Revelation, revealed top 6, chose `PUT: 1,2,3,4,5,6`
(all six). All six ARE snow permanents (Snow-Covered Forest [snow land]; Arcum's Astrolabe [snow
artifact]; Scrying Sheets [snow land]; Ohran Viper x3 — Coldsnap Ohran Viper is a Snow creature).
Next-record events (s24), verbatim:
```
- You revealed 6 and put Snow-Covered Forest, Arcum's Astrolabe, Ohran Viper, Ohran Viper, Scrying Sheets, Ohran Viper to get snow
- Your revealed Snow-Covered Forest goes to hand
- Your revealed Arcum's Astrolabe goes to hand
- Your revealed Ohran Viper goes to hand
- Your revealed Ohran Viper goes to hand
- Your revealed Scrying Sheets goes to hand
- Your revealed Ohran Viper goes to hand
```
All six "get snow" picks -> hand. ZERO in a "goes to graveyard" list. R1 regression detector NEGATIVE.
With wave-20 vs27 s18 (4 snow -> hand, 2 non-snow fetches -> graveyard), ENGINE-R1 is now confirmed on
TWO independent corpora, correct partition on both the mixed-eligibility (wave-20) and all-eligible
(wave-21) cases. CLOSE ENGINE-R1.

(Eligibility surfacing: `ELIGIBILITY:` header + `[eligible for ...]` marks fired on reveal prompts
corpus-wide. On vs14 s23 the numbered option list carried no per-card marks, but since all six were
eligible it was immaterial and the partition was correct.)

---

# AZCANTA ACTIVATION — a TRANSFORM window occurred (vs14) but the model DECLINED it TWICE; the ACTIVATION path is STILL UNTESTED. Also a real MISPLAY + a representation gap.

vs14 seq 29 (turn 10 upkeep) and seq 40 (turn 12 upkeep): engine offered `["Transform:azcanta, the
sunken ruin", "Decline - do nothing"]`. The engine only offers the transform when Search for Azcanta's
7-cards-in-graveyard threshold is met — so the transform WAS available and beneficial (flipping into
Azcanta, the Sunken Ruin = a repeatable card-advantage engine, exactly what deck135 wants in a grind vs
Deep Blue). The model chose "Decline - do nothing" BOTH times.

Why: both replies manually recounted the graveyard from the event log, arrived at 5-6 cards, and
concluded "I do not have 7, I must Decline." seq 29 was a 15k-char graveyard-counting spiral ("...I
don't have 7, 'Decline - do nothing' is the most logical choice to avoid a failed action"); seq 40
(5.3k): "Given I have 6 cards in graveyard, I cannot transform. I must Decline." The manual count was
WRONG (the engine's offer proves >=7), so the model talked itself out of a free available upgrade twice
in a game it then lost by grinding. Real misplay.

Root: a REPRESENTATION gap. The `Transform:...` option carries no eligibility annotation telling the
model the threshold is already satisfied; the error-prone manual graveyard recount fills the vacuum and
gets it wrong. Same class the reveal eligibility-surfacing fix solved — extend that surfacing to the
Azcanta transform option. Route: notes ENGINE-R6 + general-suggestions. NOT guide-fixable (engine layer;
the frozen guide does not mention Azcanta transform).

Consequence for validation: because the transform was declined, Azcanta never became active, so the R4
ACTIVATION path (`{2}{U},{T}: look at top 4, put a noncreature-nonland to hand`; eligibility of the
to-hand slot + whether an R1-style all-sweep lurks in the activation optiontwo) got NO test this corpus
either. UNTESTED, not vindicated. My wave-20 exit condition "Azcanta activation observed resolving
correctly" is NOT met.

---

# HARNESS-1 — the wave-21 retracted_choice route OVER-CORRECTS: FALSE-POSITIVED on my one fallback, discarding a valid, unretracted, Rule#1-correct choice. (Harmless this time by heuristic luck.)

My seat's single fallback: vs27 s26 (turn 10, Main phase 1, 8/20, 13417 chars, `fallback:
retracted_choice`, `choice: -1`). The reply contains EXACTLY ONE `CHOICE:` line —
`CHOICE: 2 (Cast Ice-Fang Coatl)` on line 1 — and it is NEVER retracted. Options were
`[Cast Glacial Revelation, Cast Ice-Fang Coatl, Cast nothing]`; casting the creature is exactly Rule #1.
The reply then spirals ~13k chars (a HARNESS-3 decode-repetition loop) about Islandwalk / Flying / Gelid
Shackles, repeatedly REAFFIRMING "I will attack with Ice-Fang Coatl." The only retraction-class token in
the whole reply is a single `Correction:` — and it is about ISLANDWALK MECHANICS, not the choice.

So the parser saw a retraction-shaped token (`Correction:`), found no SECOND CHOICE line after it, and
concluded CHOICE:2 was retracted -> routed to heuristic with `retracted_choice`. FALSE POSITIVE. The
model's single choice stood and was correct; the parser threw it away.

Correct routing? No — the choice should have been HONORED. Harmful? Not this time: the heuristic
independently cast Ice-Fang Coatl (s27 declares it as attacker, so it reached the battlefield at s26).
The heuristic aligned with intent, so no play was lost — but that is luck, not correctness.

This is the INVERSE over-swing of wave-20 HARNESS-1: wave-20 the parser LOCKED IN a truly-retracted
choice (took a disavowed `CHOICE: 4`); wave-21 now DISCARDS a NOT-retracted choice because a
retraction-shaped token appears downstream. FIX: the retracted_choice route must only fire when the
retraction targets the CHOICE/decision itself (or a later CHOICE line supersedes); a single
un-superseded CHOICE line should be HONORED even if `Correction:`/`Wait` tokens appear later
reconsidering downstream combat/mechanics. Route: notes HARNESS-1.

Underlying disease unchanged: still a 13k-char decode-repetition spiral (HARNESS-3). The parser change
only altered its CLASSIFICATION (retracted_choice instead of wave-20's unparsed/hidden-lock-in). Decode-
loop mitigation is still the real fix.

---

# ANSWER-FIRST (item b) — WORKING; NO deliberation-quality regression at this seat.

- Sampled every p95+ reply (all >8k chars, 18 records): the coded line is FIRST in 100% of them
  (`CHOICE:`/`ATTACK:` on line 1). ZERO `unparsed_reply` and ZERO `empty_reply` this seat (wave-20 had 7
  unparsed here) — the giant decode spirals that broke parsing in wave-20 now parse cleanly from line 1.
  Genuine measurable improvement.
- "Last well-formed CHOICE wins" PRESERVES reconsideration: vs49 s26 committed `CHOICE: 0 (pass)` first
  but the model reconsidered and the parser logged the reconsidered fetch-crack; vs62 s9 committed
  `CHOICE: 1 (Play Scrying Sheets)` first but logged `Play Flooded Strand`. The model was NOT hard-locked
  to its first answer.
- The one bad decision among long replies (vs14 s29 transform decline) committed "Decline" first, but
  its reasoning ALSO landed on decline (the miscount), so answer-first is NOT the culprit — the
  representation gap is. No decision degraded because the model committed before reasoning. Answer-first
  validated at this seat.

---

# OBEDIENCE CHECK (verify before crediting or churning — the record FELL, so check the guide isn't leaking)

- Rule #1 (deploy a creature): obeyed. Every "Cast nothing"/value-spell turn inspected was correct — no
  castable real creature in the option list at those windows (vs102 s7-s16 had only Boreal Druid; vs35
  was green-screwed). The dropped-creature turns are FETCH-FIZZLE mana starvation, not a Rule#1 leak.
- Rule #2 (attack when profitable): obeyed. Attacks at every profitable window (vs35 s13/s17/s20; vs49
  s17/s22; vs62; vs14). The s20 all-in at 4 life was a correct forced race read. No 0-attacker durdle.
- The losses do NOT trace to any guide line. They trace to (a) the fetch-fizzle engine bug, (b) the
  Azcanta transform-representation gap, (c) matchup/draw. Zero decision-level leak attaches to guide
  text. Guide stays FROZEN.

---

# CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- Fetch-color dependence is now DOUBLY punishing. The thin blue base + Windswept-Heath-can't-fetch-Island
  fragility (7th-corpus standing flag) is bad enough alone, but the fetch-FIZZLE engine bug turns "one
  bad fetch" into "no mana all game." Construction flag and engine bug COMPOUND: a deck that must fetch
  to function is maximally exposed to a fetch-resolution bug. Fixing the engine bug (notes ENGINE-F1) is
  higher-leverage than any decklist change.
- Under-powered small-creature clock — STANDS (7th corpus). Treefolk / Diamond Faerie the only real
  closers; Diamond Faerie (G/W/U) still strands (vs49). When mana cooperates (vs62) the plan closes with
  a real kill; when it doesn't, small bodies can't race.
- Force of Negation: cast ONCE (vs49 s28, off-whitelist "exile a blue card", in a game already lost, no
  board wipe present). Low-signal single firing in a dead game; the reflexive-off-whitelist watch is
  neither cleared nor strongly re-triggered. Stays at WATCH.

# GUIDE DISPOSITION: FROZEN (byte-identical to live; `cmp`-verified). strategy.txt is a byte-copy. See notes for the layer-routed ledger and rotation verdict.
