# deck27 (Zombies UB tribal) — wave-22 findings

**Seat:** guided round 3. Binary /tmp/wagic-1d8ed8194. Corpus matchups-20260723-173843, 6 games
round-robin. **Record 1/6 (down from 5/6) across a POOL SWAP** (3 new opponents: 109 Mono-R Aggro,
44 Faerie Tempo, 140 RBW Control; continuing: 135, 62, 102). Guide byte-identical to wave-20/21.
**135 decisions across 6 games, 0 fallbacks (0.0%).** Every decision returned a clean coded reply.

## Record — 1/6 (W vs44; L vs135/62/102/109/140)

| vs  | new? | wave-21 | wave-22 | life (me/opp) | turn | shape |
|-----|------|---------|---------|---------------|------|-------|
| 44  | NEW  | —       | **WIN**  | 20 / -4  | 10 | Textbook: Zombie Outlander + Vodalian islandwalk + Lord of Atlantis beatdown; faeries have no ground wall |
| 135 | cont | WIN     | **LOSS** | -4 / 11  | 12 | COLOR/MANA SCREW — couldn't cast lords ("no Blue mana / insufficient total"), ground out by 6/6 Abominable Treefolk |
| 62  | cont | WIN     | **LOSS** | -4 / 12  | 8  | Opponent assembled a **17/21 trampler by t8** — clean play, lost to opponent curve |
| 102 | cont | WIN     | **LOSS (adj)** | 8 / 15 | 12 | Lost the race behind (Tourach deck); life-adjudicated behind |
| 109 | NEW  | —       | **LOSS** | -4 / 3   | 13 | CLOSE race vs Mono-R aggro — got opp to **3**, died t13; one real misplay (below) |
| 140 | NEW  | —       | **LOSS (adj cap)** | 17 / 28 | 24 | STRUCTURAL: no-removal/no-lifegain evasion deck vs dedicated lifegain-control (→28) + board sweeper. Unwinnable clock |

## Honest decomposition of 5/6 → 1/6 — NO shipped change caused the drop

**Rule out regression first (all confirmed):**
- Guide is **byte-frozen** (diff vs deployed = 0 before my revision) — no guide change could have hurt.
- **0 fallbacks** at this seat (0 stale_echo, 0 retracted_choice, 0 unparsed, 0 empty) — no parser/
  harness regression bit deck27.
- **Zero exposure to any wave-22 batch annotation**: deck27 plays no fetchlands (item a irrelevant),
  no transforms (item d never rendered at this seat), no painlands (item e never rendered — deck27's
  lands are Island/Swamp/Underground Sea, a pain-free dual; the "Refuge" grep hit is deck140's Akoum
  Refuge in *opponent* lines; the "fetch" hit is the generic rules primer, seq 1 only). N9/echo fixes
  (items b/c) had nothing to fire on (0 retractions, 0 echoes). **The batch changes were net-neutral
  to positive at this seat.**

**So the swing is pool-swap difficulty + draw/opponent variance, per game:**
- **vs44 W (new):** base-guide evasion/protection path executed perfectly — Zombie Outlander (pro-green,
  though vs faeries the pro is moot; still an unkillable beater), Vodalian Zombie with ACTIVE islandwalk,
  Lord of Atlantis. Swung the whole team every turn (seq 10/12/15/18), 20→-4 by t10. The win-path works.
- **vs135 L (was W):** wave-21 win was a Vodalian islandwalk beatdown. This corpus deck27 was **color/
  mana screwed** — the seq16 (15.7k) and seq18 giants both show it stuck at 6 life unable to cast any
  lord ("cannot cast … due to lack of Blue mana", "insufficient total mana"), ground out by a 6/6 snow
  Abominable Treefolk it could only wall with pro-green Vodalian. Draw variance.
- **vs62 L (was W):** wave-21 win was "life untouched" pro-green + evasion. This corpus deck62
  **assembled a 17/21 trampler by t8** (seq18 combat) — deck27 chipped opp 20→12 but died -4 first.
  deck27 played clean (correct pro-green block s6, correct Saproling double-block s18). Opponent-curve
  variance, not pilot error.
- **vs102 L (was W):** lost the race behind (8/15), life-adjudicated. Tourach-class deck (flagged wave-21
  as an un-interactable threat for this no-removal deck). Variance.
- **vs109 L (new):** **CLOSE** race vs Mono-R aggro — deck27 got the opponent to **3 life** and died at
  t13 (−4), ~3–4 damage short. Matchup speed (mono-R is faster) + one misplay that was plausibly
  game-relevant in a race this tight (see below).
- **vs140 L (new, adj at cap):** **STRUCTURAL** — the opponent had **0 creatures the entire game**, a
  pure lifegain-control shell (2× Elixir of Immortality + Venser's Journal → gained to 28) with a
  Black Sun's Zenith −1/−1 sweeper that kept resetting deck27's board (the −1/−1 counters on the Glens
  at seq9 are its work). deck27's evasive clock (2–4/turn) cannot outrace dedicated lifegain, and it has
  no way to disrupt the Elixirs/Journal. Unwinnable clock — a deck-construction gap, NOT a durdle.

**Verdict:** this is the unfavorable side of the same high-variance surface wave-21 flagged (wave-21's
5/6 was decomposed as "mostly variance, 3 favorable flips"). deck27 is a no-B-plan evasion deck: when it
draws its evasion/protection path and isn't outraced it wins (vs44); when it's color-screwed (135),
out-curved (62), out-raced (102/109), or structurally out-classed (140 lifegain) it loses. The pool
swap loaded it with two of its worst archetypes at once (fast aggro + lifegain-control). The durable
signal is unchanged: **structural fragility (no removal, no reach, no B-plan) — a construction item,
not a guide-fixable one.**

---

## THE one real misplay — vs109 s15 (t9, 12/15): chumped a LORD instead of taking two free kills

Full board: incoming A1 Goblin (1/1), A2 Goblin (1/1), A3 Hellrider (3/3, haste). Blockers available:
B1 Lord of Atlantis (2/2), B2 Sanguine Guard (2/2 first strike). Both blockers were annotated
"(you kill it, your blocker lives)" vs BOTH Goblins, and "(your blocker dies, attacker lives)" vs
Hellrider (neither 2/2 can kill a 3/3).

**Correct line:** B1:A1, B2:A2 — kill BOTH Goblins for free, keep both blockers, take 3 from Hellrider
(→9). **Chosen line:** `BLOCKS: B1:A3, B2:A1` — chumped **Lord of Atlantis into Hellrider** (lord dies,
Hellrider lives), killed one Goblin, let the other Goblin connect for 1. Net: traded an islandwalk-
granting LORD + took 1, to save ~2 life. This violates the guide's explicit "never chump with a LORD"
AND the "take the free favorable block" principle (should have taken two free kills).

**Mechanism — an answer-first DOWNSIDE (new, harmful twin of wave-21 vs35 s18):** the reply's FIRST
line committed the bad snap block (`B1:A3, B2:A1, B3:A2` — it even hallucinated a nonexistent B3). It
THEN reasoned correctly toward realizing the chump was bad ("blocking A3 costs me a creature and leaves
A3 alive … In all cases A3 survives") — but the reply **truncated (7.6k) before emitting a corrected
BLOCKS: line**, so answer-first locked the misplay. In wave-21 the same commit-first→reason interaction
was BENIGN (vs35 s18: the model emitted an improved *last* line and last-wins honored it). Here it was
HARMFUL: the snap answer was a misplay, the reasoning would have fixed it, but truncation froze the
snap. **1st harmful instance of the answer-first commit-lock interaction.** Routed to notes.md
model/parser ledger; my strategy.txt revision front-loads a free-kills-first ALLOCATION rule so the
*snap* line gets multi-attacker allocation right (the layer answer-first actually commits).

---

## OWNED VALIDATION — Wither annotations round 2: CONFIRMED, ZERO false positives (regression clean)

Complete set of Oona's Gatewarden / wither block annotations this corpus, all verified against real
lethality math (Oona 2/1 base, power 2 → 2 wither = −2/−2; Oona dies to any attacker power ≥1):

1. **vs44 s6 (t3, 20/20)** & **vs44 s16 (t9, 20/7)** — Oona 2/1 vs Faerie Miscreant (1/1) → "(both die)".
   CORRECT (2 ≥ 1 kills it; Oona's 1 toughness dies to power 1). Model TOOK both — good, walling a 1/1
   flyer with a defender for free-ish.
2. **vs62 s6 (t4, 20/20)** — Oona 2/1 vs Canopy Spider (1/3, trample, reach). Two annotations:
   - B1 Oona blocks → "(your blocker dies, attacker lives (wither shrinks it to 0/1))". CORRECT: Spider
     1/3 −2/−2 = 0/1 (toughness 1 survives → "attacker lives"); Oona's 1 toughness dies to Spider's 1.
   - B2 Vodalian Zombie (2/2, **pro-green**) blocks → "(neither dies)". CORRECT and SOPHISTICATED: the
     Spider is green, so pro-green negates its damage (Vodalian takes 0), and Vodalian's 2 power < the
     Spider's 3 toughness (Spider lives). **The annotator correctly accounts for protection.** Model
     chose B2:A1 — the strictly better block (free, removes the trampler for the turn, blocker survives).
3. **vs62 s18 (t8, 13/12)** — Oona now 4/3 (Glen-buffed) + Glen 2/3, vs a 17/21 trampler (A1) + two
   Saprolings. Oona 4/3 vs A1 → "(your blocker dies, attacker lives, **14 tramples** to your face
   (**wither shrinks it to 13/17**))". CORRECT on every axis: Oona 3 toughness < 17 power → dies;
   A1 21 toughness → lives; trample = 17 power − Oona 3 toughness = **14**; shrink = 17/21 −4/−4 =
   **13/17**. Glen 2/3 vs A1 → "14 tramples" (no wither, 17−3=14) — correct. Model chose to kill the two
   1/1 Saprolings with Oona+Glen and eat the unstoppable trampler — correct (chumping Oona saves 0
   trample; the free Saproling kills are the value). deck27 died to the 17/21 anyway (race loss).

**Round-2 verdict: no regression, zero false positives, and two non-trivial interactions handled
correctly (pro-green "neither dies", big-trample+shrink math). Item stays CLOSED.**

---

## Ahead-position degeneration loop (focus): did NOT recur — no 4th instance

deck27 was rarely ahead this corpus (won only vs44; roughly even at 20 vs 19–21 in the vs140 grind).
A full scan for consecutive-line verbatim repetition across all 6 games (135 decisions) found **ZERO**
degeneration loops. The one truncation was **deck135 s16 (15.7k, priority, BEHIND me6/opp14)** — a
verbose *behind-position* over-derivation about a future Treefolk block that truncated mid-sentence
("…The log says") — NOT the ahead-loop, and fully saved by answer-first (line 1 `CHOICE: 2 (Put in
Hand … Walking Dead)` locked the recursion play). The decode-layer ahead-loop watch stays at 3
instances (wave-19 deck110 s20, wave-20 deck27 vs49 s16, wave-21 deck27 vs135 s18), unpromoted.

## Answer-first / 0 fallbacks (focus): holding, with the one caveat above

100% coded first lines on every reply, incl. the giants: deck135 s16 (15.7k → `CHOICE:`), deck102 s14
(9.7k → `CHOICE: 0 pass`), deck109 s24 (8.0k → `BLOCKS:`), deck109 s20 (7.6k → `BLOCKS:`). 0 fallbacks
across 135 decisions. The robustness win is intact. **The one qualifier** is vs109 s15: answer-first's
commit-first is a truncation-safe *floor* only when the snap answer is CORRECT; when the snap is a
misplay the model would self-correct, truncation locks the misplay. This is a harness/parser-layer
interaction (ledger item), and my guide revision addresses the specific allocation error at the snap
layer.

## Durdle late-stall revision (focus): STILL UNVALIDATED — 3rd corpus, trigger did not fire

vs140 was the best grind window yet (control opponent, cap at t24). But deck27 **attacked every turn it
had an available attacker** (seq 9/15/19/21) — it NEVER held buffed attackers back. The revision's
TRIGGER state (walled stall + holding eligible attackers) did not occur; instead deck27 followed the
revision's *prescription* (swing 2/turn with a lone flyer while grinding) and **still lost, because the
clock was structurally too slow vs lifegain-control**. That is a construction gap, not a durdle. Per
wave-21 skill.md's own note: an off-case may take many corpora to draw its trigger — hold it, don't cut
it, keep re-flagging. Guide kept frozen on this axis (my only edit is the block-allocation rule).
