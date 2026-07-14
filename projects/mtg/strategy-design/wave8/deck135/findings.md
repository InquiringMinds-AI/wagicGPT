# Deck-135 (Modern Snow) — Wave-8 game review

Corpus: PRIMARY control arm `matchups-20260714-044131` (wave-7 guide live, wave-7 engine
ledger shipped, unchanged core prompt). All six deck135 seat games read decision-by-decision
from the translogs (new `gamestart`/`events`/`defer` fields used). Game->file map at bottom.
Doctrine applied: decision-level opportunity-conversion audit; the win column is CONTEXT ONLY.

## RECORD (context only — not a verdict) — 0W / 4L / 1 genuine TO / 1 engine-wedge TO

results.tsv rows (winner column = deck-slot that won, NOT deck135):

| game    | opp (name)              | outcome (deck135)         | my_life | opp_life | turn |
|---------|-------------------------|---------------------------|---------|----------|------|
| 135v110 | 110 Etched Affinity     | LOSS                      | 0       | **17**   | 11   |
| 135v140 | 140 Wipe Them Out!      | TIMEOUT (behind 16v21)    | 16      | 21       | 28   |
| 135v131 | 131 Mind Control        | LOSS                      | -2      | **18**   | 17   |
| 135v109 | 109 Hellrider           | LOSS                      | -3      | **20**   | 9    |
| 135v133 | 133 Phyrexian Asphodel  | TIMEOUT (T2, 20/20)       | 20      | 20       | 2    |
| 44v135  | 44 Faerie Archmage      | LOSS                      | -5      | **18**   | 10   |

- **All FOUR losses end with the opponent at/near 20 (17, 18, 20, 18)** — the skill's
  "deck never had a plan running" discriminator. This is an OBEYED-BUT-LOSING wave dominated by
  two causes a guide line only partly touches: a blue-thin manabase that strands the deck's own
  2-color creatures, and a slow clock. **This wave adds a genuinely GUIDE-ADDRESSABLE layer on
  top (Q3): development paralysis when no creature is castable.**
- **135v133 is the KNOWN ENGINE WEDGE** (Arcum's Astrolabe turn-2 livelock, brief item 7 —
  16 KB file, turn 2, 20/20). EXCLUDED from all play-quality claims. Fix built post-corpus.
- Reply parser clean (0 desyncs). One reply-protocol head-number artifact (Q5, vs131 s48),
  self-corrected. No `defer`/`fallback` records in any deck135 seat.

## Q1 — Rule #1 (deploy-before-value) obeyed? overshoot? value-over-creature dig-miss recur?

**OBEYED, no overshoot, and the wave-7 dig-miss class did NOT recur.**

- **Creature offered -> taken: high.** Every game the pilot took the creature when one was in
  the cast list (Icehide Golem on sight; Boreal Druid, Coatl, Treefolk when payable). The only
  window that looks like a creature declined for value is **vs131 s48**, and that is a
  reply-protocol head-number artifact (Q5), not a durdle — its PLAN reasons all the way to
  "cast Icehide Golem, Option 2" and the pilot DID cast Golem one phase later (s51).
- **Value-over-creature "dig for a bigger threat" (wave-7's mild residual) is GONE** — no window
  this corpus shows a castable creature passed over to cast Glacial Revelation/Astrolabe/Search
  "to find more." The wave-7 Rule #1 sharpening ("the dig is the losing thought") held.
- Freeze Rule #1 verbatim (the develop-branch add in Q3 is an ELSE-branch, not a Rule #1 change).

## Q2 — Force of Negation calibration (wave-7 resolved it; watch for regression)

**HOLDS. No face-burn over-fire this wave.** Force fired in only one game (vs131), all targets
NONCREATURE and defensible:

| seq (vs131) | target      | verdict |
|-------------|-------------|---------|
| s34/s36 T11 | Aetherize   | CORRECT — Aetherize bounces ALL my creatures = tempo sweeper; Forcing it protects the board |
| s55/s56/s57 T15 | Downsize | DEFENSIBLE — noncreature combat trick; kept to protect the lone threat's damage |

(Multiple records per fire = the cost-target seam + cast seam of the SAME Force, not distinct
fires.) The wave-7 lone over-fire (Galvanic Blast to face) did NOT recur. Board-wipe scoping +
face-burn redirect obeyed. **Free-reaction-card-cost watch CONTINUES — still single-seat
(deck135 only); no 2nd free-alt-cost deck in this corpus, so it cannot be promoted. Keep the
per-deck line.** Freeze the Force line.

## Q3 — NEW guide-addressable lever: DEVELOPMENT PARALYSIS when no creature is castable

The wave's one model-fixable lever, and it partially REFRAMES the wave-7 "manabase strand"
attribution as ALSO a piloting failure.

**vs44 (LOSS, opp 18): the pilot Cast nothing on 4 own-turn windows while its own BLUE FIX was
castable and declined.** Opening hand: Search for Azcanta {1}{U}, Astrolabe, Misty Rainforest,
Boreal Druid, Snow-Covered Forest, Into the North, Prismatic Vista — fixing + one mana dork, no
threat. The deck needed blue from turn 1 (Search for Azcanta {1}{U}). It was fetchable the whole
game (Misty Rainforest / Prismatic Vista / Flooded Strand all offered "targeting Snow-Covered
Island" from T1 on). Yet:

| seq | turn | pilot chose | declined-but-castable | note |
|-----|------|-------------|-----------------------|------|
| s15 | T5   | Cast nothing | Into the North {1}{g}, Glacial Revelation {2}{g} | "keep mana open... avoid wasting mana on non-threats" |
| s18 | T5 MP2 | Cast nothing | Into the North {1}{g} | same turn, still holding |
| s23 | T7   | Cast nothing | Into the North {1}{g}, Glacial Revelation {2}{g} | "casting value... leaves me no threats" — but it HAS no threat to cast |
| s27 | T7 MP2 | Cast nothing | Into the North {1}{g} | |

`Into the North` is the pilot's blue FIX (fetches a Snow-Covered Island). It was castable at T5
and T7 and declined FOUR times to "hold mana for a creature" — a creature it could not cast and
did not draw until Ice-Fang Coatl arrived T9. It finally cast Into the North at s32 (T9) ->
Island, by which point it was at 5 life and died T10. **The blue screw in vs44 was substantially
self-inflicted: the fix was in hand, castable, and declined.**

Root: Rule #1's "value cards wait" framing HAS an else-branch — "(or none is castable) do you
spend leftover mana on Astrolabe / a fetch crack / Glacial Revelation / Into the North" — but it
is phrased as a PERMISSION, and the weak executor's invented instinct ("hold mana for a creature
/ don't waste mana on non-threats," verbatim s23) OVERRODE it. When the deck cannot deploy a
threat, the pilot FREEZES instead of developing its own fixing/engine. This is the DISPLACEMENT
sibling of Rule #1: the deploy rule is obeyed and the leak moved to the else-branch.

**Scope check (join the board state — do not over-count):** filtered to genuinely-useful declined
spells (excluding redundant 2nd Astrolabe copies), the pattern is CONCENTRATED in vs44 (4
windows). vs110's superficial "6 paralysis windows" were FALSE POSITIVES — the only castable
value there was a second Arcum's Astrolabe (correctly declined; the pilot WANTED Coatl but was
blue-screwed with Coatl unofferable). vs131/vs109 had <=1 and are noise. So the fix is scoped
tightly to "no creature in your cast list AND a real fixing/dig spell is castable" — it must NOT
license casting value over a listed creature (that would break Rule #1). BOTH branches are stated
together in the guide (creature listed -> cast it; none listed -> develop, don't Cast nothing).

## Q4 — Fetch re-offer tax (engine item 4 verified FIRED; no strategy damage)

**The tax DROPPED, confirming decline-suppression fired.** Held-fetch decline windows per game
(fetch-crack offered, correctly not taken):

| game | wave-7 | wave-8 |
|------|--------|--------|
| vs140 (TO) | 97 | **54** |
| vs44  | 71 | 22 |
| vs133 | 51 | 0 (wedge) |
| vs109 | 49 | 19 |
| vs110 | 44 | 2  |
| vs131 | 5  | 38 |

Avg over the 5 non-wedge games ~= **27/game, down from wave-7's ~=62/game**. Item 4 fired.
- **No wanted crack made impossible.** Suppression is decision-cache-based (board-state +
  question key); a fetch re-appears each turn as the board changes (hence the residual 22-54
  re-offers), so nothing is permanently withdrawn within a game. The vs44 non-cracking was
  PARALYSIS (Q3), not suppression — the pilot cracked Into the North at T9 the moment it chose
  to; the option was there. **No fetch declined-twice-then-needed pattern exists.** No strategy
  damage from item 4.
- The tax persists in the grind games (vs140 54) and still inflates decision counts -> **engine
  ledger continues** (see notes.md); NOT guide-fixable.

## Q5 — reply-protocol single instance (model/harness, layer-routed away from the guide)

**vs131 s48 (T14):** the reply's first character is `4` (Cast nothing), but its PLAN reasons in
full — "Rule #1 says deploy a creature... I can pay for Icehide Golem right now... I must cast a
creature first... I will choose Option 2 as the standard cast." The head-first parser committed
`4` while the reasoning concluded `2`. This is answer-before-reasoning INTENT-COLLAPSE: the
"number first, then PLAN" protocol forces the commit before the model finishes reasoning inside
the PLAN. **Self-corrected at s51 (Main phase 2, same turn) — the Golem was cast ~1 phase late.**
A REPLY-PROTOCOL/model artifact, NOT a guide-wording or judgment defect — route to the
reply-protocol/core layer, never to guide text. Single instance this corpus -> watch, not a
promotion. See notes.md + general-suggestions.md.

## Q6 — combat seams (inherited blocker-seam watch #5)

**Clean for deck135 this wave.** Combat was sparse (tiny boards). Blocks observed were correct
(vs110 s27 Ice-Fang Coatl blocks the Vault Skirge flyer; vs109 s19 Icehide Golem blocks Rakdos
Cackler). Attacks correct (Boreal Druid attacked in vs44 only as the last/only body — the
guide's mana-creature carve-out held). NO reflexive high-life chump, NO missed must-block. The
blocker-seam arithmetic riders did not need to fire for this deck; nothing to add.

## WAVE-7 LEDGER STATUS
CONFIRMED / HELD:
- Phantom-lethal board-read anchor — no on-board hallucination this corpus (0 recurrences).
- Rule #1 deploy-before-value — obeyed, no overshoot; dig-miss class gone.
- Force scoping — holds; no face-burn over-fire.
- Fetch decline-suppression (engine item 4) — FIRED; tax ~62->~27/game; no strategy damage.

NEW / PERSISTING:
- **Development paralysis when no creature is castable** (vs44 x4) — NEW guide lever (Q3).
- Manabase strands 2-color creatures — standing construction flag, sharpened: the deck can fix
  its own blue via fetches/Into the North; the pilot underuses that fixing.
- Slow clock — structural (construction: threat size/count); feeds every timeout.
- Held-fetch re-offer tax — reduced but present (vs140 54) -> engine ledger.
- Reply-protocol head-number artifact (vs131 s48) — model/harness, single instance.

## GAME -> FILE MAP (run = matchups-20260714-044131)
| jsonl epoch | opp | outcome | turns | final (me v opp) |
|-------------|-----|---------|-------|------------------|
| 1784022094 | vs110 | LOSS    | 11 | 0 v 17  |
| 1784022096 | vs140 | TIMEOUT | 28 | 16 v 21 |
| 1784022098 | vs131 | LOSS    | 17 | -2 v 18 |
| 1784024496 | vs109 | LOSS    | 9  | -3 v 20 |
| 1784024870 | vs133 | TIMEOUT | 2  | 20 v 20 (ENGINE WEDGE — excluded) |
| 1784026790 | vs44  | LOSS    | 10 | -5 v 18 |
