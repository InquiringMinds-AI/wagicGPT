# deck139 -- wave-31 engine / harness items

Corpus `matchups-20260727-025026`, binary `cf7cb2e53`. All seq refs are the deck139-seat translog
unless a stderr file is named.

---

## N-139h (carried) -- ManaCost::remove OOB crash fix: HOLDS, but UNEXERCISED. PROBE REQUESTED.

**Status: no regression. No positive revalidation.**
- 0 crash/abort/assertion/core-dump signatures in all six deck139-seat stderr files
  (`game-137v139-1785138630`, `game-146v139-1785141172`, `game-139v148-1785141597`,
  `game-139v158-1785146142`, `game-139v152-1785149552`, `game-139v122-1785150987`).
- 4/4 offered-and-chosen mutates ARRIVED (arrival table in findings.md section 1). Zero silent
  no-ops, zero defers, 0/154 fallbacks.
- **The crashing path was never re-driven.** The wave-30 crash needed Pollywog Symbiote's
  `auto=lord(creature[mutate]|mycastingzone) altercost(colorless,-1)` to be ACTIVE on a mutate
  being PAID. This corpus: Pollywog reached the battlefield in 1 of 6 games (vs146 seq10-19); the
  only discounted mutate it produced (vs146 seq17, `Cast Gemrazer with its mutate cost {g}{g}` --
  note the `{1}` already stripped) was DECLINED; Pollywog died at seq19; the mutate actually taken
  at seq21 rendered at full `{1}{g}{g}`.

**Requested probe (deterministic, cheaper than another rotation slot).** Scripted board: Pollywog
Symbiote on the battlefield + one non-Human host + Nethroi, Apex of Death in hand with
`{4}{GW}{B}{B}` payable. Assert the discounted mutate is offered, chosen, paid, and that the
`[mutated pile]` line appears. Repeat for Snapdax `{2}{BR}{W}{W}` (the multi-symbol/hybrid shape
the wave-30 report fingered). A release-build run matters here: with assertions off the same
underflow computes a wrong cost silently instead of aborting.

---

## N-139j (NEW, HIGH) -- attacker options carry no power annotation; 0-power creatures attack

The blockers menu annotates a powerless blocker; the attackers menu does not annotate a powerless
attacker. Live pair from the same corpus:

    blockers (vs152 seq22):
      4. B1. Arboreal Grazer #1 (0/3) [reach] [deals 0 - this block kills nothing, it only
         absorbs damage] - may block A1 ...

    attackers (vs137 seq18):
      1. A1. Arboreal Grazer #1 (0/3) [reach]
      2. A2. Arboreal Grazer #2 (0/3) [reach]
      3. A3. Arboreal Grazer #3 (0/3) [reach]

Repro: `1785138635-ai_baka_deck139-...-vs-ai_baka_deck137.jsonl` seq16 (T5), seq18 (T7), seq19 (T9),
seq21 (T11) -- four attack declarations sending 0-power creatures, `power=0` per
`primitives/borderline.txt name=Arboreal Grazer`. seq21 reply: *"Attacking with all three 0/3
Grazers deals 3 damage, bringing the opponent from 16 to 13 life."* seq19 reply (208,327 ms) reaches
the right answer AFTER the answer-first line has already committed:
*"Wait, Arboreal Grazer is 0/3. It has 0 power. Attacking with 0 power creatur"*.

**Proposed fix (mirrors the shipped blocker annotation, deck-agnostic):** on an attacker option whose
effective power is 0, append `[deals 0 - this attack deals no damage to the opponent]`. Same
precedent class as the wave-30 g4 tapped-tag reword, which validated cleanly.

---

## N-139k (NEW, HIGH) -- `[summoning sick - ...]` is read as "cannot block"; it decided a game

The tag states only the attack restriction:

    Sauron, the Lidless Eye {3}{r}{b} (4/4) [summoning sick - cannot attack this turn]

vs158 seq26 (T10, my life 7, opponent 6), the pilot wrote *"The opponent has no untapped creatures
to block (their Orc army and Sauron are tapped or just entered), meaning my attackers will deal 6
damage directly to the opponent, reducing their life to 0 and winning the game immediately"*, then
declined a castable Migratory Greathorn (both variants offered) on that belief. Sauron was untapped
and blocked at seq28; Sacred Foundry died; the opponent lived at 3 and won on the next turn
(`WAGIC_SELFPLAY_RESULT winner=1 life0=0 life1=3 turn=11`). The neighbouring creature on the same
line, `Orc army (3/3) ... [tapped - cannot attack or block this turn]`, spells out BOTH restrictions
-- so the pilot reasonably read the shorter tag as the same class of statement.

**Proposed fix:** `[summoning sick - cannot attack this turn, but CAN block]`. One clause, same
shape as the g4 creature/land tapped-tag split that validated at 131 reads with no misread.

---

## N-139i (NEW, MED) -- London bottoming: mis-stated mulligan count + N-requested/1-consumed, and
## it under-bottoms (CR 103.5 violation)

Repro: `game-139v122-1785150987.stderr` lines 144-152 and translog seq8-11.

stderr, authoritative:

    PREGAME: player 0 (seat 0) KEEPS after 7 mulligan(s); will bottom 7 card(s)
    AIPlayerGPT: bottom prose-reconciled to the final stated set
    PREGAME: player 0 (seat 0) bottoms 'Plains' (1 of 7); library bottom card now 'Plains'
    AIPlayerGPT: bottom prose-reconciled to the final stated set
    PREGAME: player 0 (seat 0) bottoms 'Arboreal Grazer' (2 of 6); ...
    AIPlayerGPT: bottom prose-reconciled to the final stated set
    PREGAME: player 0 (seat 0) bottoms 'Everquill Phoenix' (3 of 5); ...
    AIPlayerGPT: bottom prose-reconciled to the final stated set
    PREGAME: player 0 (seat 0) bottoms 'Plains' (4 of 4); ...

Three defects in one seam:

1. **The prompt mis-labels the mulligan count.** seq8 says *"you kept after 7 mulligans, so you must
   put EXACTLY 7 cards"*; seq9 *"kept after 6 mulligans ... EXACTLY 6"*; seq10 *"after 5"*; seq11
   *"after 4"*. The player kept after 7 in all four. The count text is the remaining-to-bottom
   number wearing a mulligan label -- a factual contradiction the model reasons against out loud
   (seq11: *"the prompt states I 'kept after 4 mulligans,' meaning I am on a 3-card hand"*).
2. **N requested, 1 consumed.** The prompt demands N card numbers; the engine applies exactly one
   per ask and re-prompts. The model complies literally (`PUT: 1, 2, 3, 4, 5, 6, 7`) and the harness
   logs `bottom prose-reconciled to the final stated set`.
3. **It stops early.** 7 bottoms were required; 4 events fired; the seat kept a 3-card hand instead
   of 0. The loop's own counter `(4 of 4)` shows it believed itself complete.

Also: seq11 cost 200,461 ms / 12,497 chars -- a pregame decision in the corpus's top-5 latency tail,
fed by the contradictory instruction.

**Proposed fix:** state the true mulligan count and the true per-ask requirement -- either ask once
for exactly N names, or ask N times for exactly ONE name and say so (`put ONE card on the bottom;
3 more after this`). Then hold the loop to the full N.

---

## N-139l (NEW, LOW) -- merged-pile cost pip render drift

After a merge the pile line shows a single-pip cost where the pre-merge line showed the full cost:

    vs146 seq23 (pre-merge):  Gemrazer {3}{g} (4/4) [trample, reach, mutate]
    vs146 seq25 (merged):     Gemrazer {g} (4/4) [trample, reach, mutate] [mutated pile - ONE
                              merged creature ...; 1 card underneath: Arboreal Grazer]

Same shape at vs148 seq13+, vs158 seq16+, vs152 seq11+ (`Migratory Greathorn {3}{g}` ->
`Migratory Greathorn {g}`). No decision was affected -- a permanent's printed cost is not a live
input -- but the number shown is wrong and the mutate cost is what a reader will assume it is.

---

## Not defects (checked and cleared)

- **The Snapdax / Illuna hardcast-only offers were CORRECT.** vs146 seq27 offered no Snapdax mutate:
  `other={2}{BR}{W}{W}` needs two white and the seat controlled one white source. vs152 seq26
  offered no Illuna mutate: `other={3}{RG}{U}{U}` = 6 mana off 5 untapped sources. The
  misidentification is the pilot's (guide-lane, see findings.md 2a), not an offerability bug.
- **The double normal/mutate ask is by design and consistent.** Cost-line pick then
  `Cast Card Normally / mutate / Decline` (vs146 seq21->22, vs152 seq19->20). Both consistent this
  corpus. Latent trap only; covered in the guide rather than raised as an engine item.
- **Gemrazer's mutate trigger not firing at vs146 seq25 is correct** -- `target(*[artifact;
  enchantment]|opponentBattlefield)` with no artifact or enchantment on the opponent's board.
- **Auto-pass on display-toggle-only priority (h5) observed at this seat**:
  `AIPlayerGPT[ph8]: only display-toggle (Flip Side) options; auto-passing without a model call`
  in `game-139v152-1785149552.stderr`. Working.
