# Deck-135 (Modern Snow) — Wave-6 game review

Corpus: one GPT-vs-GPT round-robin run, deck135 seat files in
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-190011/`. All six games read
decision-by-decision from the translogs; game→file map at the bottom.

## RECORD — attribute precisely (the task's prior was wrong)

results.tsv rows, read carefully (winner=N means deckN-slot won, NOT deck135):

| row | deck0 | deck1 | winner | life0 | life1 | turn | deck135 outcome |
|-----|-------|-------|--------|-------|-------|------|-----------------|
| 135v133 | 135 | 133 | **0** | 1 | -1 | 14 | **WIN** (opp -1) |
| 44v135  | 44  | 135 | timeout | - | - | - | **adj. WIN** (18 v 10 @T22) |
| 135v110 | 135 | 110 | timeout | - | - | - | incomplete (16 v 19 @T7) |
| 135v109 | 135 | 109 | **1** | -11 | 20 | 9 | **LOSS** (opp 20) |
| 135v131 | 135 | 131 | **1** | -4 | 12 | 11 | **LOSS** (131 won) |
| 135v140 | 135 | 140 | timeout | - | - | - | draw (16 v 16 @T18) |

**Effective record: 2 wins (1 raw vs133 + 1 adjudicated vs44), 2 losses (vs109, vs131),
1 draw (vs140), 1 incomplete/latency-starved (vs110). Up from wave-5's 1W-4L-1TO.**
Note: deck135 has NO win over deck131 — the 135v131 row is winner=1, i.e. **131 won**;
and the 135v109 row is likewise a LOSS. There is exactly ONE raw win (vs133).

Deck plan (verified vs `borderline.txt`/`mtg.txt` last wave, unchanged): base-green snow
tempo-value. Cheap self-replacing snow creatures (Ice-Fang Coatl {G}{U} 1/1 flash flyer ETB
draw, Ohran Viper {1}{G}{G} 1/3 kills-its-blocker + draws-on-connect, Icehide Golem {1} 2/2)
grind while Abominable Treefolk {2}{G}{U} (power=snow permanents, trample, ETB freeze) is the
one real closer. Diamond Faerie {2}{G}{W}{U} 3/3 flyer with a UEOT team pump. Large non-creature
value suite: 4 Astrolabe, 3 Search, 4 Glacial Revelation, 4 Into the North, 11 fetch/ramp lands.

## CENTRAL QUESTION — did Rule #1 (deploy-a-creature-before-value) fire, and did it fix the durdle?

**FIRED, and NOT overshooting.** The deploy-before-value ordering is obeyed at high rate:

- **Creature offered in 16 ask-windows across 6 games; creature taken 14/16 (87.5%).**
- **Direct-conflict windows (a creature cast AND a value/fetch cast BOTH offered same decision):
  10; creature chosen 8/10.** Icehide Golem **8/8**, Ohran Viper 2/2, Abominable Treefolk 2/2,
  Ice-Fang Coatl 2/4.
- The **2 conflict misses are BOTH vs131 (s54, s59)** and are NOT value-durdle — see the
  hallucinated-lethal finding below. There is **no sign of overshoot**: the pilot never crammed
  a creature ahead of a genuinely-better value play, and it never cast a redundant/off-plan
  creature. Rule #1's origin-deck applied its own P1 doctrine cleanly — displacement fix landed,
  no inversion.

**Did it fix the durdle? PARTIALLY — where creatures were drawn AND castable, yes; the residual
is now manabase/clock, not durdle-on-value.** Opponent life at game end vs wave-5's 18–23 floor:

| game | wave-5 opp-life @ end | wave-6 opp-life @ end | read |
|------|----|----|----|
| vs133 | 21 (loss) | **-1 (WIN)** | durdle fixed — killed them |
| vs44  | 20 (loss) | **10 (ahead)** | dealt real damage, but slow clock ran the cap |
| vs131 | 23 (TO)   | **12 (loss)** | dealt damage; lost to a hallucination, not durdle |
| vs109 | 20 (loss) | **20 (loss)** | UNFIXED — but cause is color-screw, not durdle |
| vs110 | 18 (loss) | 19 (incomplete@T7) | game barely started (latency) |
| vs140 | 0 (WIN)   | **16 (draw)** | REGRESSED to a grind — variance + slow clock |

Three of six opponents dropped below the wave-5 18–23 floor (vs133 -1, vs44 10, vs131 12). The
durdle-on-value leak that dominated wave-5 is materially reduced. **What replaced it as the loss
cause is two things Rule #1 cannot touch: (1) a manabase that strands the deck's own creatures,
and (2) a slow clock that turns even non-durdle games into decision-count/latency timeouts.**

## MANDATORY AUDIT 1 — displacement vs overshoot on Rule #1

Covered above. Verdict: **OBEYED (14/16 offers, 8/10 conflicts), NOT overshooting, durdle
partially fixed.** The skill's P1 displacement doctrine, applied to its origin deck, worked as
designed — the fix did not invert into "creatures crammed out ahead of better value." Core +
guide double-teaching (threat-before-value in BOTH the core prompt and Rule #1) did **not** cause
overshoot; the two reinforced correctly. No demotion needed yet — keep Rule #1 at full prominence
(the residual losses aren't at this seam), but re-tune the KEY-CARDS spotlight (Diamond Faerie).

**NEW BAD-PLAY LEAK (the vs131 loss, and the 2 Rule#1 misses): hallucinated-board phantom lethal.**
vs131 s52–s60: the pilot's PLAN asserts *"a lethal board state (Abominable Treefolk 5/5 and
Icehide Golem 2/2 for 7 total damage)... I will win immediately next turn"* and on that basis
declines Ice-Fang Coatl **twice** (s54, s59 → "Cast nothing") and passes every priority window,
then attacks with only the 2/2 Golem (opp 14→12) and dies at T11. **Abominable Treefolk was NEVER
cast and NEVER offered all game** (verified: 0 offers, 0 casts in vs131) — the pilot hallucinated
a permanent onto its own battlefield and carried a phantom lethal across 9 consecutive decisions.
The visible `Your battlefield:` line shows only Icehide Golem; the deciding fact IS surfaced. This
is stale-PLAN poisoning with a fabricated board state — the general prompt's "trust the board over
the plan / count damage only from listed creatures" is PRESENT and LOSING here. Route: model-
quality residual (own-board hallucination), with a small guide reinforcement tying the lethal
check to the actual battlefield/attackers list. Do NOT write a louder deploy rule — the pilot
wasn't durdling, it thought it had already won.

## MANDATORY AUDIT 2 — timeout triage ×3 (latency_ms evidence)

Corpus-wide the run carried the flagged 60–80s Spark latency spikes; per-seat medians ~9–11s.

- **vs44 — WINNING-BUT-SLOW (adjudicated WIN, 18 v 10 @T22).** 116 decisions, latency sum
  **1731s, median 10.1s, 15 decisions >20s, max 74.9s**; several 47–74s spikes in the T20–T22
  tail (s108 48s, s109 74s, s110 60s). The cap was hit by **decision COUNT × latency**, not play
  passivity — the pilot was ahead the whole back half (opp stuck at 10). BUT it also shows a real
  **failure-to-close**: at opp 10 / me 20 it spent T21–T22 playing a land and cracking fetches
  (s110 Scrying Sheets, s111–113 crack Windswept Heath ×3, s114 crack Prismatic Vista, paying
  life 20→18) instead of pushing lethal. Mixed: infra (latency) + engine (fetch re-offer loop,
  below) + a genuine slow clock. Not a guide-passivity bug; adjudicated win.
- **vs110 — LATENCY-STARVED (incomplete, 16 v 19 @T7).** Only **48 decisions, reached T7**, sum
  530s. A 48-decision / T7 game does not reach a 2400s wall on its own seat's 530s — it timed out
  because the shared Spark server (−j3) serialized this seat's inference behind the opponent's and
  the other two concurrent games; deck110 is affinity (dense trigger/decision count). Game barely
  started, life roughly even. **Infra artifact — write nothing to the guide.** (2 latency spikes
  >20s, max 68.4s.)
- **vs140 — GRIND / EVEN (draw, 16 v 16 @T18).** 96 decisions, sum 989s, only **1 decision >20s**
  — so NOT latency-driven and NOT durdle: the pilot deployed (Icehide s5, Ohran Viper s47/s59) and
  attacked (s18, s49, s75). It timed out on a **slow clock**: opp life crawled 20→20→20→18→17→16
  over 18 turns because the deployed threats are tiny (2/2, 1/3) and got chipped/blocked, and the
  one real closer (Treefolk) did little. Regressed from wave-5's 15-0 win on this exact matchup —
  **variance (different draw) + the deck's inherent slow clock**, not a guide break.

Summary: **1 winning-but-slow (adj. win), 1 latency-starved (infra), 1 slow-clock grind (draw).**
None is a Rule#1-passivity failure. Two of three are dominated by latency/engine/clock factors a
guide line cannot fix.

## MANDATORY AUDIT 3 — convergence watch: reflexive high-life blocking?

**NO for deck135.** Only **2 bundled `blockers`-kind decisions** in the whole corpus:
- vs133 s63 (my **6** opp10): Abominable Treefolk blocks Bloodghast — a LOW-life survival block,
  correct.
- vs44 s104 (my **20** opp10): Ice-Fang Coatl blocks a 4/4 Surveilling Sprite — a HIGH-life block,
  but it is a **favorable trade-up** (a 1/1 with snow-deathtouch kills the 4/4), not a wincon
  chumped away. Not the deck109 pattern.

deck135 provides **no support** for promoting reflexive-high-life-blocking to a sharper core
sentence (the wave-5 deck109 single-seat proposal on the convergence watch). Keep it per-deck /
on watch. (Most of deck135's defensive decisions are single-creature no-blocks at priority, not
reflexive commits.)

## MANDATORY AUDIT 4 — fetch re-offer loop (still UNFIXED; fresh repro pointers)

The engine re-offers a fetch-crack activation at EVERY priority window until the land is
sacrificed, and the pilot re-picks it across consecutive decisions — one crack costs multiple
model calls, and holding a fetch re-prompts the crack every window. Fresh wave-6 repros
(run `matchups-20260713-190011`):

- **vs44 s111→s112→s113: "Put in Play with Windswept Heath targeting Snow-Covered Plains" chosen
  3 consecutive times** (Main1 → Main2 → next Upkeep), then s114 cracks Prismatic Vista — a
  4-decision fetch chain at T21–T22 paying life 20→18, the dominant contributor to that game's
  decision-count timeout.
- **vs110 s2→s3 and s13→s14: same crack option chosen twice consecutively** (~2 model calls per
  crack), at T0 and early — inflating an already latency-starved game.
- Holding-a-fetch re-offer (the crack option present, pilot correctly passes tapped-out) runs of
  8–13 consecutive records: vs44 s68–76 (9), vs110 s25–37 (13), vs109 s41–53 (13), vs140 s19–29
  (11). These are attention/latency tax even when the pilot passes correctly.

**Options_text makes this trivial to spot: the identical option string
`"Put in Play with Windswept Heath targeting Snow-Covered Plains [cost: Tap, Life, Sacrifice]"` /
`"search basic land with Prismatic Vista targeting Snow-Covered Forest [cost: Life, Tap, Sacrifice]"`
recurs verbatim across consecutive seqs.** ENGINE-LEDGER item (not a guide fix): the crack
activation should be consumed/withdrawn once chosen, and a held fetch's crack should not re-prompt
every window. See notes.md for the ledger update.

## MANDATORY AUDIT 5 — manabase flag refresh (2 Snow-Covered Islands / Diamond Faerie)

- **Diamond Faerie ({2}{G}{W}{U}): offered 0 times, cast 0 times — for the SECOND straight wave.**
  This is now a STANDING CASTABILITY FACT, not variance: a GWU 5-drop off a base of 2 Snow-Covered
  Plains (+ Windswept Heath→Plains, Flooded Strand→Plains, Prismatic Vista→any, Astrolabe fixing)
  needs G+W+U+5 mana simultaneously and the deck almost never assembles it before games end. Its
  hero treatment in the guide (a Rule#2 spotlight last wave, a KEY-CARDS entry, a lethal-pump line)
  is **dead weight** — demote to a one-line "cast it the rare turn you can pay G, W, AND U; do not
  plan around it." (Deck-construction flag, not a guide-length fix.)
- **The deeper manabase problem is the 2-color CREATURES, and it decided vs109.** Ice-Fang Coatl
  needs {G}{U}; Ohran Viper needs {1}{G}{G}. In vs109, **Ice-Fang Coatl sat in hand from T0 through
  T8 and Ohran Viper from T4 — and NEITHER was ever offered as castable** (verified: 0 creature
  cast options all game) because the pilot could not assemble G+U / G+G. Blue for Coatl comes from
  only **2 Snow-Covered Islands + Misty/Flooded/Prismatic fetches + Astrolabe**; against a T2–T9 RB
  aggro clock the fixing was too slow. Opp stayed at **20 all game** (the skill's "deck never had a
  plan running" discriminator) while me fell 20→-11. This is a **manabase-skew construction flag**:
  the creatures the deck wins with are gold/2-color, but the base is green-heavy and the blue count
  is thin. Wave-6 loss vs109 = color screw + fast aggro, NOT a piloting error.

## SECONDARY LEAK — Force of Negation over-fired (5/5, never at a board wipe)

Force of Negation counters NONCREATURE spells for free on the opponent's turn (exile a blue card).
The guide says *"Fire it at a board wipe (Damnation, Pyroclasm)."* Observed: cast **5/5 times, none
at a board wipe** — vs44 s78/s83 (opponent's Counterspell / Arcane Denial), vs109 s31/s32/s59
(Krenko's Command, Pillar of Flame, Searing Spear). In vs109 (RB aggro, creatures already scarce)
it **exiled blue cards to 2-for-1 itself against 1-mana burn/token spells while being run over** —
"free" reads to the model as "always fire." Minor relative to the color-screw that lost that game,
but the guide line is not sharp enough: it should scope firing to (a) a board wipe, or (b) a spell
that would kill your ONLY threat — and never spend a blue card the deck needs to cast its creatures
to counter a small spell against aggro. Low priority; tie it to the named cards.

## WAVE-5 LEDGER STATUS

FIXED / IMPROVED:
- **Deploy-before-value (Rule #1) landed** — 14/16 creature offers taken, 8/10 conflicts; Icehide
  Golem 8/8. Durdle-on-value materially reduced; 3 opponents pushed below the 18–23 floor.
- Attack seam still healthy — no "no attackers" freezes observed; the free-case Boreal-Druid chip
  overshoot did not recur meaningfully (Druid offered 2 / attacked 1).
- Reply parser clean — 0 head-integer-vs-choice desyncs.
- Diamond Faerie 22/0 refusal did not recur (offered 0) — but now a standing castability fact.

PERSISTING:
- **Fetch re-offer loop** — still unfixed (Audit 4). Engine item.
- **Slow clock** — even wins/draws take T14–T22; tiny creatures grind; Treefolk is the only real
  closer and rarely lands early. Feeds every timeout.

NEW:
- **Hallucinated-board phantom lethal** (vs131) — new bad-play/model residual; cost that game.
- **Manabase strands 2-color creatures** (vs109) — construction flag, sharpened from wave-5.
- **Force of Negation over-fire** — minor guide-scoping leak.

## MODEL-QUALITY RESIDUALS (route to model-experiments track)
- **Own-board hallucination / phantom lethal** (vs131 s52–60): the pilot fabricated a Treefolk on
  its battlefield and a lethal count over the visible board, declining development for 9 decisions.
  The deciding fact (no Treefolk in play) is fully surfaced; instruction ("count only listed
  creatures") is present and lost. Litmus case for the stale-plan/hallucination class — not
  guide-fixable.
- No desyncs, no numeric-offset menu traps (Diamond Faerie's X-pump never reached), no degenerate
  blocker replies.

## GAME → FILE MAP (run = matchups-20260713-190011)
| epoch | opp | outcome | turns | final (me v opp) |
|-------|-----|---------|-------|------------------|
| 1783987218 | vs133 | WIN | 14 | 1 v -1 |
| 1783989289 | vs44  | TIMEOUT (adj. WIN) | 22 | 18 v 10 |
| 1783991383 | vs110 | TIMEOUT (latency, incomplete) | 7 | 16 v 19 |
| 1783994677 | vs109 | LOSS | 9 | -11 v 20 |
| 1783995005 | vs131 | LOSS | 11 | -4 v 12 |
| 1783995180 | vs140 | TIMEOUT (grind draw) | 18 | 16 v 16 |
