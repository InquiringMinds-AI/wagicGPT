# deck49 (Dragons) — FIRST-GUIDED VALIDATION findings (wave 18)

Mono-red Dragons aggro-midrange, playing its FIRST corpus WITH the wave-17 Step-0-bis initial
guide (+ the wave-17 engine batch f664539a3). Corpus: 6 games, **2/6** (W vs deck135 Modern Snow,
deck21 Goblin Gang; L vs deck110 Etched Affinity, deck62 Enchantresses, deck14 Deep Blue, deck35
Mighty Slivers). 125 decisions, 0 timeouts. Binary /tmp/wagic-f664539a3.

This is a VALIDATION, not a freeze audit or blind revise (Step 0-ter). Report per taught class
DIED / PERSISTED / MUTATED before the record; the record is a coin-flip surface (rule 1).

## Headline: the tax/fallback collapse — 9 → 0

| metric | wave-17 (guideless) | wave-18 (guided) |
|---|---|---|
| fallbacks at this seat | **9** (7 stale_echo + 2 unparsed) | **0** |
| CHOICE-vs-executed mismatches | (the 9) | **0 / 125** |
| fabricated mana-locks (C1) | #1 tax epicenter, multiple | **0** |
| burn target menus resolved to FACE | leaky (C3: bolted Ornithopter/dorks) | **17 / 17** (9 with an enemy creature offered-and-declined) |
| War Paint cast on own creature | looped/failed on enemy-only | **5 / 5** own-target (see E-49c note) |
| Crucible into empty board | 2 misfires | **0** (one guide-CITED decline) |

Every wave-17 fallback is gone; every taught class either DIED or PERSISTED-correct. The record
fell 3/6→2/6, but that is pool/variance (rule 1): vs the wave-17 pool deck49 FLIPPED vs110 (W→L)
and vs35 (W→L), FLIPPED vs135 (L→W), held vs62 (L) and vs21 (W), and met a NEW deck14 (L). deck17
(a wave-17 win) rotated out. The guide's JOB — kill the taught classes, collapse the tax — is met.

## STEP 0-ter TABLE — died / persisted / mutated, per taught class

| taught class | guide lever | verdict | evidence |
|---|---|---|---|
| **C1 mana false-belief** ("I have 0 mana / land tapped → can't cast") | Rule #1 mana-is-trivial | **DIED** | 0 fabricated locks in 125 decisions. All 8 grep hits are LEGIT affordability accounting (real can't-afford-yet: 1-land T1, tapped-out, 6-drop on 4 lands). The #1 wave-17 tax epicenter is gone. |
| **C2 target sub-menu** (echo spell/invent "Cast nothing"/index mismatch → stale_echo) | Rule #2 + engine TARGET CHOICE framing | **DIED** | 0 stale_echoes (was 7). All target menus answered by matching index. Model explicitly narrates "TARGET CHOICE for Lava Spike... I need to target" (vs110 s11). |
| **C3 burn mis-targeting** (spent clock on 0-power walls / dorks) | BURN-GOES-FACE rule | **DIED** | 17/17 burn/player target menus → the opponent's face, INCLUDING 9 where an enemy creature (Ornithopter 0/2, Saprolings, Icehide, Lord of Atlantis, Raging Goblin) was on the menu and declined. Zero wasted bolts. |
| **C4 War Paint enemy-only loops** (fatal, 2 unparsed wave-17) | never-aura-opponent rule + engine E-49c suppression | **PERSISTED-correct, but suppression masked the hard case** | 5/5 War Paint casts targeted own creatures; model twice cites "I will not give the opponent's creature a buff" (vs14 s8, vs135 s6). BUT no enemy-ONLY offering appeared all corpus — E-49c suppressed them (vs35 s9 shows the annotation "the only legal targets are YOUR OWN right now"). The fatal loop had no chance to fire. See E-49c note. |
| **C5 Crucible into empty board** (dead card, 2 misfires) | Crucible-needs-dragons rule | **DIED** | vs35 s15: declined Crucible with no Dragon, reply cites "strategy guide explicitly advises against casting it into an empty board." Every actual Crucible cast (vs21 s15, vs35 s17) had/was-landing a Dragon (Firespitter Whelp is a Dragon). |
| **C6 durdle / deploy floor** ("Cast nothing" spam) | DEPLOY-AND-ATTACK rule | **DIED** | Only 6 "Cast nothing" execs in 125 decisions; 4 clearly correct (forced by 0 mana, or guide-compliant Crucible/lethal-setup). No durdle spiral. Losses are construction, not passivity. |

## Apportioning the fallback collapse (engine framing vs guide rules #1/#2)

The 9→0 fallback collapse is **~fully engine-attributable**; the guide's distinct win is play-quality
+ the mana-belief death:

- **7 stale_echoes → ENGINE.** All 7 lived at the target-sub-menu seam. The wave-17 batch's TARGET
  CHOICE framing (names the pending effect, "not a cast or phase step") + the "(player, life N)"
  suffix-strip in the echo matcher (item e / old E-49a) directly dissolve them. Rule #2 reinforces
  but the *representation* is what removed the fallbacks — the seat shows the classic
  representation-beats-instruction pattern.
- **2 unparsed (War Paint loops) → ENGINE.** E-49c suppression removed enemy-only War Paint from the
  menu; the loop had no fuel.
- **Guide's standalone win = C1 mana-belief death.** The engine did NOT touch mana representation
  (the "Mana available" line was always there). Rule #1 is the only deck49-specific lever on C1, and
  C1 — the #1 wave-17 tax epicenter — is dead with 0 recurrence. Credit Rule #1.
- **Guide's other win = play correctness at non-fallback seams**: burn-to-face went leaky→100%
  (Rule BURN-GOES-FACE), Crucible-empty went 2-misfires→guide-cited-decline, never-aura obeyed 5/5.

**Consequence for Rule #2**: on the FALLBACK dimension it is now redundant with the engine framing
(the stale_echoes it targeted are gone via representation). Its menu-MECHANICS prose ("this is a
second menu / don't Cast nothing / don't re-type the spell") is the trim candidate — see the guide
disposition in strategy.txt. The never-aura-opponent STRATEGIC rule is NOT redundant and must stay
(the owner ruling replaces E-49c suppression with a warning annotation next corpus → enemy-only War
Paint reappears).

## Residual reasoning-tax — the tax RELOCATED (Step 0-ter rule 4)

The fallback tax collapsed but big replies did not vanish: 20/125 decisions ran ≥600 words, topping
out at **2,800** (vs21 s5) and **2,523** (vs110 s11). Crucially **none produced a fallback** — every
spike resolved to a correct, parsed choice. The successor tax is NOT a taught deck49 class; it is:
- **combat/sequencing math under pressure** (vs110 s11 at 2 life: a 2,500-word lethal-math loop that
  correctly lands on Lava-Spike-the-Signal-Pest); and
- **stale-plan re-derivation** (vs21 s5: "My previous plan to 'play land then wait' was incorrect
  because…" — a PLAN-caveat re-plan that spirals to 2,800 words but stays parsed).
This is a CORE / general-prompt concern (routed to general-suggestions.md), not a guide item. The
reply-length gauge survives the belief swap exactly as Step 0-ter predicts.

## Loss shapes — construction pre-flag CONFIRMED (top-heavy curve / mana screw)

The wave-17 notes pre-flagged the losses as substantially deck-CONSTRUCTION (top-heavy Dragon curve,
only two {6} finishers, thin 3-cost body). This corpus's four loss shapes confirm it:

- **vs62 Enchantresses (T12):** flooded on a 1/1 + burn. Board was **Bloodmark Mentor (1/1) and
  nothing else from T4 to T9**; the only threat drawn was a 6-drop Rorix, stuck uncastable at ~5
  lands. Burn ground the opp to 8 but a lone 1/1 can't close. Textbook top-heavy: no body between
  Bloodmark Mentor and the {6} finishers.
- **vs14 Deep Blue (T15):** **held Rorix {3RRR} + Shivan {4RR} in hand from T8 to T14** at 4-5 lands
  vs mono-blue control that answered every creature (own board = 0 creatures at s14/s16/s19/s22/s23/
  s24). Mana-screwed-on-6-drops + a control matchup that outlasts a slow clock. The pre-flag's exact
  shape.
- **vs110 Etched Affinity (T6, died at 2):** OUT-RACED by explosive Affinity (Galvanic Blast x2 to
  face T1, a 6/6 Master + 7/2 Signal Pest by T5). deck49 developed fine but its early clock is a 1/1
  Dragonmaster that needs 6 lands to matter; the race was lost before the engine turned on.
- **vs35 Mighty Slivers (T11, 7 life):** out-WIDTHED by a go-wide lord deck; deck49's board was a
  lone Bloodmark Mentor for most of the game, Shivan arrived only in time to chump-block (s23).

Through-line across all four: deck49's early pressure is a 1/1 (Dragonmaster, worthless pre-6-lands)
+ a 1/1 anthem (Bloodmark Mentor); the real threats sit at 4-6 mana. When it can't curve into
Thunderbreak/Dragons, it has no early clock and loses the race or gets ground out. This is NOT
guide-fixable (strategy text cannot re-slot the curve). The one guidable sliver: in the two grind
losses the model twice held a face-able Lightning Bolt for "cast nothing" at 1 mana (vs14 s10, vs35
s10) — a mild burn-hold against the BURN-GOES-FACE rule. Addressed with a thin flood/no-curve
off-case line (the losing corpus lives in the off-case, Step 0-ter rule 6).
