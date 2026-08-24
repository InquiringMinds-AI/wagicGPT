# Wave-39 rotation selection

Selected per the standing criteria (dev-loop-and-campaign.md): LLM value = (deck power ceiling) − (what Baka already extracts). All three enter FIRST PASS GUIDELESS (Step 0). Candidate pool = 164 stock decks minus commander (19), canaries, ever-seated, current seats (146/139/152/158/105/36/116), and construction-list (134/148). Archetype diversity target met: one draw-go control shell, one tutor-assembled two-card combo, one asymmetric prison/punish engine — none of the three overlaps an existing guide (dungeons, mutate, bant midrange, amass, infect, artifact aggro, cheat-into-play) or a canary lane (counterspell tempo, snow, sweepers control, spells-matter, affinity, discard, RDW) in its CORE mechanic. Key cards verified present in `Res/sets/primitives/` (Peer into the Abyss lives in `borderline.txt`; all others in `mtg.txt`).

## Pick 1 — deck125 "Revelation Fracture" (Modern UW Control — draw-go)

- **Archetype**: pure draw-go permission control. 1 creature (Emrakul, the Aeons Torn as the lone finisher), 27 spells, 4 artifacts, 2 enchantments.
- **Ceiling cards**: Sphinx's Revelation ×4 ({X}{W}{U}{U}, gain X + draw X — verified `life:X && draw:X`), Supreme Verdict ×4 (uncounterable wrath), Path to Exile ×4, Essence Scatter/Dream Fracture/Cancel/Fall of the Gavel (11 counters), Final Judgment ×2, Elixir of Immortality ×2 (graveyard-recursion inevitability loop), Staff of Nin, Lightmine Field, Emrakul.
- **Why Baka fumbles it**: Baka cannot hold mana across turns for instants — it main-phases counters into nothing and taps out in front of threats; it cannot size an X-spell (Revelation for the right X at end of turn), cannot time a sweeper against its own board plan (deck has almost no board), and has no concept of the Elixir shuffle-loop as a win condition or of protecting a single Emrakul as the entire clock.
- **What the LLM+guide extracts**: end-step Revelation sizing, counter-mana discipline (which threats to counter vs. Verdict away), wrath timing, the Elixir inevitability line, and one-threat protection. Mechanics the corpus has NOT covered: X-spell sizing, shuffle-loop inevitability, mono-finisher control. (Sweeper timing and counterspell holds are canary-adjacent (140/44) but here they compose into a full draw-go frame neither canary runs.)

## Pick 2 — deck126 "Sanguine Blood" (Modern WGB Combo — Exquisite Blood + Sanguine Bond)

- **Archetype**: turtle-behind-walls, tutor-assembled two-enchantment infinite-loop combo kill.
- **Ceiling cards**: Exquisite Blood ×2 + Sanguine Bond ×2 (verified as reciprocal life-loss/life-gain triggers → any life exchange with both on the battlefield loops the opponent to death), Idyllic Tutor ×4 (fetches either half), 16 defensive walls (Perimeter Captain, Pride Guardian, Overgrown Battlement, Wall of Omens), Sorin, Lord of Innistrad, Staff of Nin (a loop-starter once the pair is assembled), Chromatic Lantern.
- **Why Baka fumbles it**: assembling a two-card enchantment combo via tutor is exactly what a greedy one-ply evaluator cannot do — it has no representation of "these two do-nothing enchantments win the game together," will tutor for the wrong piece or none, and will misuse the wall shell (chump-attacking 0/4s) instead of stalling toward the combo. The deck's #HINT combos paper over some of this for Baka, which is precisely the signal that the plan is beyond its native evaluation.
- **What the LLM+guide extracts**: tutor sequencing (which half first vs. what's drawn), recognizing assembly = win and finding the loop-starter (Staff ping, Sorin emblem, any lifegain), stall-and-defend discipline, and when to deploy vs. hold the second piece. Nothing in the guide corpus covers a tutor-assembled combo kill.

## Pick 3 — deck162 "Draw and Die!" (UB forced-draw punish — prison/engine)

- **Archetype**: asymmetric engine-prison — force the opponent to draw, punish every draw.
- **Ceiling cards**: Underworld Dreams ×4 + Fate Unraveler ×4 + Liliana's Caress ×2 + Ob Nixilis, the Hate-Twisted ×3 (the punish layer), Howling Mine ×4 + Dictate of Kruphix ×3 + Forced Fruition ×2 + Teferi's Puzzle Box ×2 (the feed layer), Peer into the Abyss ×1 (half-their-library one-shot finisher with any punisher out), Fog Bank/Shield Sphere/Master of the Feast.
- **Why Baka fumbles it**: the whole plan is "helping the opponent is good" — giving the opponent cards is normally nonsensical and only becomes correct ONCE punishers are down (the owner's never-suppress ruling in miniature). Baka has no sequencing concept of punishers-before-feeders, will run symmetric feeders out naked (donating card advantage), and can't see Peer into the Abyss as lethal-with-punisher rather than a self-draw spell. Master of the Feast's drawback is the same inversion.
- **What the LLM+guide extracts**: strict punishers-first sequencing, evaluating symmetric effects by board context, the Peer lethal calculation (opponent library/2 × punisher count vs. their life), and defensive-body discipline (Fate Unraveler/Fog Bank hold walls per the deck's own #HINTs). A wholly uncovered mechanics area: symmetric/asymmetric engine management.

## Runners-up

- **deck123 "Intruders Of Thraben"** (WBU Intruder Alarm combo) — Alarm + Thraben Doomsayer/Bloodline Keeper = infinite tokens (verified: Alarm's ETB-untap trigger works); held back only because one tutor-combo seat (126) already fills the lane this wave; strongest candidate for wave-40.
- **deck56 "Enchanted Coatl"** (GU enchantress engine) — Argothian/Verduran/Enchantress's Presence draw chains + Lorescale Coatl payoff + Serra's Sanctum; cast-order sequencing (enchantress before enchantments) is pure LLM value, but engine-management overlaps pick 3 this wave.
- **deck121 "Reckoner's Chain"** (RW Boros Reckoner/Stuffy Doll + self-sweep redirect combo) — Blasphemous Act into your own Reckoner/Doll as a burn plan is a rich inversion, but the wall-stall shell mirrors 126's seat shape.
- **deck130 "Budde's Ponza"** (mono-R land destruction) — LD is an uncovered mechanic and Baka's LD targeting is naive, but the shell is otherwise a linear red deck; moderate ceiling.

## ROTATION RECORD (wave-39, per the standing per-deck rule)

EXIT → canaries: 105, 158, 116 (no guide mods wave-37 + no seat signal). Canary list
grows to 26. HOLD: 152, 146 (guides modified wave-37), 139 (host-carry validation
pending), 36 (crash-fix live validation pending). ENTER (Step 0, FIRST PASS GUIDELESS):
125 Revelation Fracture, 126 Sanguine Blood, 162 Draw and Die!. Wave-39 pool:
146, 139, 152, 36, 125, 126, 162.

## Wave-39 adjudication (synthesis, 2026-08-23)

- EXIT -> canary: deck36 (guide unmodified two waves; crash-fix validated with positive
  control; zero seat-scoped ledger items). Guide stays deployed as canary.
- HOLD: 139 (party to the Affinity crash + owed 139v125 rerun + mutated-pile render item),
  152 (two surgical guide edits shipped this wave, validation owed), 146 (hold-back gate
  NULL on n=1 — named arming requirement: report arming counts from 146v139 and 146v152
  specifically, or retire the line), 125/126/162 (Step-0, initial guides deployed,
  one validation corpus owed).
- ENTER: deck123 "Intruders Of Thraben" (60 cards verified, no guide file — first pass
  GUIDELESS per Step-0).
- Wave-40 pool: 146, 139, 152, 125, 126, 162, 123.
- deck126 construction: owner ordered the Forest*16 -> *1 fix (75/39 -> 60/24) 2026-08-23;
  committed with this wave. Seat-126 wave-40 deltas must be attributed across BOTH the new
  guide and the construction change (wave-35 attribute-deltas-correctly rule).

## Wave-40 adjudication (synthesis, 2026-08-23)

- EXIT -> canary: deck139 (crash + rerun discharged, host-carry closed W39; #7 mutate
  render re-keyed to a probe deck, not a held seat). Guide stays deployed as canary.
- HOLD: 152 (edits un-adjudicated - probe-deck discharge runs IN wave-41 alongside the
  corpus), 146 (gate kept, chump-anchor regression -> guide mods), 125/126/162 (Step-0
  validation done, guide mods shipped this wave; 162 at corpus 1 of the 2-corpus crutch
  bar), 123 (Step-0, owes validation corpus).
- ENTER: deck130 "Budde's Ponza" (guideless first pass). 121 = wave-42 front-runner;
  56 deferred while 162 holds the engine-management lane.
- Wave-41 pool: 146, 152, 125, 126, 162, 123, 130.

## Wave-41 rotation adjudication (2026-08-24)
NO EXIT. Exit requires the exiting deck's guide to be settled; all seven pool decks ship
new wave-41 edits with falsifiable predictions requiring the next corpus (deck130 first
guided corpus; 123 EDITs1-7; 125 R1-R5; 126+162 rewrites, R4-prediction needs 162 held;
152+146 ladder/mulligan rewrites). Wave-42 pool unchanged: 146,152,125,126,162,123,130.
Canaries: deck123 construction question (if behaviors correct and rate flat at 1-2/6,
next lever is deck construction, not the guide); deck130 passivity counter-risk vs 125.

## Wave-42 rotation adjudication (2026-08-24)
NO EXIT. Every guide ships fresh wave-42 edits (25 across 7 decks); menace engine fix +
owner narration fixes need their first corpus; E7 rework + deck123 obedience restructure
unvalidated. Wave-43 pool unchanged: 146,152,125,126,162,123,130. Canaries: deck123
obedience rate at M>=12 (was 72%); deck130 Starstorm release; deck125 W1/W2 vs punishers.
