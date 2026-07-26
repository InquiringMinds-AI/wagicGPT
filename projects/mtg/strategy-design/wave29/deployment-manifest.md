# Wave-29 deployment manifest

Deploy target: `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (gitignored -> `git add -f`).
Source of truth for each proposed guide: `strategy-design/wave29/deckN/strategy.txt`.
The ORCHESTRATOR performs the deploy/commit/push -- this manifest documents WHAT to deploy and the
cmp/md5 evidence. Verified against the live tree this session (binary 50eeed8be).

## Actions

| Deck | Action | Deployed md5 | Proposed md5 | Bytes (dep -> prop) | Evidence |
|------|--------|--------------|--------------|---------------------|----------|
| 122 | **NEW initial deploy** | (none) | 537a20f16acac0c33eec21a35c679a76 | -- -> 6575 | no deployed file; Step-0 debut guide |
| 139 | **NEW initial deploy** | (none) | 8e7131f4046a8490d50a2577d062feab | -- -> 8320 | no deployed file; Step-0 debut guide |
| 148 | **NEW initial deploy** | (none) | d2086fceb9145f766b52546a7bb4a719 | -- -> 5709 | no deployed file; Step-0 debut guide |
| 137 | **REVISION** | 72893c81d34119472c4d2f5afd9fbf35 | 413133bda07d1804a3e2a7a1c12b3718 | 9074 -> 9522 | X>=2-3 affordability threshold (8 changed lines) |
| 136 | **REVISION** | 296773bdbe70bec31050f86a4e285f2d | f96f6e77bffae8caba7ddde2bd2d0d46 | 9583 -> 10367 | Mausoleum Secrets constraint + Rats over-tutor caution (17 changed lines) |
| 146 | **REVISION** | 547561f2a09bc673458084ee4bd4f543 | 636e29cfbc5c7f5eb65c240d6d906f1d | 7182 -> 8482 | fact-drift corrections (68 changed lines: room counts, no-ordinals, branch-text, walker) |
| 93 | BYTE-FROZEN | 1a27e8c4cc93c2884c52ee3dbc60265f | 1a27e8c4cc93c2884c52ee3dbc60265f | 5124 (identical) | cmp+md5 identical; rotating OUT, no redeploy |

Cross-wave check: deployed md5 for 137/136/146 == wave-28 PROPOSED md5 (the wave-28 deploy is live). deck93
deployed == wave-28 frozen value (5124 b). All wave-29 seat strategy.txt files pass `U+FFFD == 0`.

## deck122 (NEW -- WB Eternal Life lifegain / Felidar alt-win / Sorin, Step-0 initial guide)

First guide for this seat. Content anchored to engine card facts (verify-oracle at draft): Felidar Sovereign's
40+-life alt-win elevated to a NAMED first-class plan (exact threshold + "this WINS, not a bonus" + a bridge
from stabilize to climb); LIFE-AS-ENGINE resource-reframe at the top (life is the engine, not a cushion);
Serra Ascendant / Divinity of Pride threshold discipline (stay above 30 to keep the flyers big -- the
load-bearing vs93 loss raced its own life DOWN through the threshold); Sorin +1-every-turn default, gate the
minus on target quality. Deploy as a new file, `git add -f`.

## deck139 (NEW -- Temur/Sultai Mutants mutate, Step-0 initial guide)

First guide for this seat. Content anchored to primitives (`other=`/`text=`/`auto=`) + Scryfall: mutate the
Apex creatures as the DEFAULT (their entire value is gated on "Whenever this creature MUTATES"; a hardcast fires
nothing) and name the exception (no host / need a raw blocker); "a mutate pile is ONE creature" stopgap (the
board render splits it into two lines until N-139c lands); Migratory Greathorn ramps on MUTATE ONLY (verify-
oracle SAVE -- memory said on-enter-or-mutate); loot trap-warning stopgap (the "target it will affect" render
is actually a discard, until N-139b lands); read mutate/alt costs from the BASE `other=` line, not the cost-
reducer-adjusted option number. Deploy as a new file, `git add -f`.

## deck148 (NEW -- mono-W Kor Army equipment, Step-0 initial guide)

First guide for this seat. Content anchored to primitives: ATTACK FLOOR keyed on a board-readable fact (durdled
to turn 11 with bodies down since turn 3); GEAR-THE-LORD-FIRST sequence (a bare Armament Master is a blank --
no gear=0 line; cheap equipment on the lord > on an attacker); Armament Master's engine-DOUBLED +2/+2 per
equipment attached to ITSELF (paper is +1/+1 -- guide uses engine behavior); close-fast (the shell is
CONSTRUCTION-CEILINGED -- low power, loses close). Deploy as a new file, `git add -f`.

## deck137 (REVISION -- verified diff, 8 changed lines)

One substantive change: the March bullet gains an X>=2-3 affordability threshold, grounded in the card's own
`auto=this(X=0) donothing` (verified in bin/Res/sets/primitives/borderline.txt): X=0 March makes ZERO Soldiers
= wasted card; only cast at X>=2-3 payable, else deploy a creature. This is variant-agnostic strategy (does NOT
name the engine bug) that also steers off the exact unaffordable picks driving the residual defer. The wave-28
Loxodon "either cast works reliably" softening is KEPT (validated this wave: 2 arrivals, 0 defers, both routes).

## deck136 (REVISION -- verified diff, 17 changed lines)

Mausoleum Secrets constraint stated in the units the pilot can COUNT at the table (creature cards in graveyard,
NOT a "late = bombs" time axis) with the arithmetic for its key targets ("MV4 Ritual needs 4 dead creatures"),
and a Rats-matchup "do not over-tutor for Ritual" caution -- the GUIDE half of the N-136a defense-in-depth (the
engine seam fix is separate; the guide lowers the pilot's DEMAND for the illegal option). MV3 facts unchanged
(validated this wave). Grounded in borderline.txt `target(<1>*[black;manacost<=type:creature:mygraveyard])`.

## deck146 (REVISION -- verified diff, 68 changed lines: a fact-drift correction pass)

Four corrections against the live render (verify-oracle render-side rail): (1) removed "Lost Mine completes in
3 rooms"/"Tomb slowest" -- render says Lost Mine 7 / Tomb 5 / Mad Mage 9 -> reframed the pick around VALUE
(all-upside rooms + clean completion vs Tomb's self-damage); (2) removed "branches show only the room NAME with
no text" (now FALSE -- branches carry {room effect}) -> teach choosing ON the effect text; (3) removed all
OPTION-NUMBER references -> answer-by-NAME (Lost Mine was option 2 first, option 3 on re-selection); (4)
strengthened the walker line to CAST-when-affordable (Kaya/Lolth unexercised 2 corpora). Larger byte delta is
the multi-fact reflow, not scope creep.

## Post-deploy verification (orchestrator)

- [ ] deck122/139/148 new files present; `git add -f` staged; cmp vs wave29/deckN/strategy.txt == identical.
- [ ] deck137/136/146 redeployed; cmp vs wave29/deckN/strategy.txt == identical; md5 matches proposed above.
- [ ] deck93 UNTOUCHED (md5 still 1a27e8c4cc93c2884c52ee3dbc60265f, 5124 b; no redeploy, no restage) -- it is
      rotating OUT to the canary list with its guide frozen.
- [ ] `git diff | /usr/bin/grep -c U+FFFD == 0` on every edited/new Res guide (all pass at author time).
- [ ] No AI attribution in the commit (verify-oracle: all deck-fact numbers grounded before deploy).
