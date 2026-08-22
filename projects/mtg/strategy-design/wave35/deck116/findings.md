# deck116 (Time of Need, GU cheat-into-play) — wave-35 guide-review findings

Inputs: 00-reviewer-brief.md; prior seat reviews `wave35-review/deck116-tier-review.md` (arms
A+B) and `deck116-fp8-review.md` (arm C); deployed guide `bin/Res/ai/baka/deck116_strategy.txt`;
strategy-writing skill wave-33 edition. Arm weighting per brief: **arm C (Qwen3.6-35B-A3B-FP8,
`matchups-20260822-111102`) is the pilot the guide teaches**; arm B (122B) marks
under-specification via justified audibles; arm A (retired 4.75-bit quant) is background only.

## State of the guide at this seat (adherence recap, cited from the two prior reviews)

- Arm C: 120/127 = 94.5% adherent-or-justified. R1 never violated; R2 clean everywhere (both
  Fabricate casts verified no-enabler-in-hand: vs36 seq7, vs105 seq22); R3 perfect on all 8
  cheats; **R4 quoted VERBATIM at both Piper seams** (vs139 seq15 t9: "Rule 4 is explicit:
  'ELVISH PIPER NEVER ATTACKS AND NEVER BLOCKS. NOT EVEN ONCE.' So definitely just A1"; vs139
  seq21 t12: "Check rule 4 again... Correct"); R6 6/6 keeps adherent; R7 near-perfect
  (deck158 chain Emrakul->Kozilek->Ulamog seq11/13/17).
- Arm B: 115/116 = 99%, zero comprehension failures, three justified audibles.
- Arm A's two signature failures (Piper attack R4, deathtouch block) are QUANT DAMAGE — gone at
  arm C per the fp8 review; not re-tuned against here.

## Guide edits this wave (each with its evidence)

1. **R4 FROZEN VERBATIM — and its wording style adopted as the seat's standard.** The verbatim
   quote at both arm-C Piper seams (vs139 seq15/seq21) is the corpus's cleanest proof that a
   short, ALL-CAPS, decision-point-anchored absolute is what this pilot retrieves at the seam.
   Every rule headline was audited for the same quotable form; the one rewrite is R3's headline
   (was the descriptive "HOW THE CHEAT WORKS, AND WHAT IT DOES NOT DO"), now the imperative
   'WHEN YOU SEE "PUT IN PLAY", TAKE IT AND TARGET THE BIGGEST CREATURE IN YOUR HAND' — the
   body sentences are unchanged (R3 executed 8/8; only the title moved to the quotable form).

2. **NEW Rule #9 (blocking): one-blocker-one-attacker stated in the guide's own voice +
   printed-branch trust.** The illegal multi-block loss (arm C vs105 seq25 t12: reply
   `BLOCKS: B1:A1, B1:A2, B1:A3`, trace QUOTES the "AT MOST ONE attacker" constraint then
   dismisses it as "standard MTG rules"; engine applied only B1:A1, 5 poison connected, game
   lost at 28 life) is HARNESS-lane per the brief — the re-ask docket item stands (notes.md).
   The guide's contribution is the constraint in its own voice with the observed wrong reply
   named ("B1:A1, B1:A2 ... the game keeps only the FIRST pair") plus the survival pick rule
   (biggest poison number — B1:A3/Skithiryx was the printed, trivial line). Second half of the
   rule targets the FP8 residual risk the fp8 review names (re-deriving combat outcomes
   alongside the printed lines: vs146 seq10's discarded deathtouch-ignoring branch): the
   parenthetical branch text is taught as the COMPUTED RESULT with first strike/deathtouch
   included — trust-doctrine-conform (teaches trust in a true surface, never doubt).
   Render strings verified in arm-C deck116 prompts: "you kill it, your blocker lives" 11 hits,
   "your blocker dies, attacker lives" 14, "Assign each blocker to AT MOST ONE attacker" 23.

3. **R5 step-2: the no-land-in-hand -> Farseek-first exception.** Arm B vs146 seq8 t7 chose
   Farseek over Explore/ToN against the guide's static ranking — JUSTIFIED (hand held Amulet +
   no land; Farseek guarantees the 4th land -> Amulet t9, won the game). That audible marks the
   ranking as under-specified; the condition is now encoded (no land in hand -> the guaranteed-
   mana spell first), echoed in R7's Farseek entry and the situation table.

4. **R5 residue (persists all three arms, minor): two wording changes.**
   (a) The named wrong thought now matches what the pilot actually generates: arm C vs158 seq15
   t9 committed "Cast nothing" with ToN/Farseek castable, PLAN verbatim "Pass this turn due to
   lack of enablers" — the guide's old named phrase ("None of the available spells are Enablers
   or Creatures, so I will pass") never appeared verbatim in any arm; the interception is keyed
   to the executor's own phrasing per the skill, so the observed sentence replaces the authored
   one. (b) A main-phase-2 release: the same game cast BOTH spells in main-2 of the same turn
   (seq16/18, net-zero card loss), and the justified hold audibles (arm A vs36 seq13/25; arm C
   vs105 seq20) were all hold-then-spend-same-turn shapes — so the rule's letter now matches
   its spirit: "a spell cast in Main phase 2 still counts; the mistake is ending YOUR TURN with
   a castable search spell unspent." The lethal-on-board violations (arm C vs36 seq18/27/29/34)
   stay unlegislated: all harmless, and a rule against them buys nothing.

5. **R7: skip-legends-already-in-hand made explicit.** Arm C vs152 seq10 t7 picked Ulamog (#5)
   with Emrakul in hand while Kozilek (#13) was eligible in the same reveal — the only R7 miss
   in the arm; every other pick across arms already followed the in-hand-exclusion implicitly.
   One clause + the observed worked example ("with Emrakul in hand, the pick is Kozilek").

6. **R3: the decline-Piper-target exception encoded.** Both stronger behaviors converge: arm B
   vs146 seq20 and arm C vs139 seq13/17 ("This is definitely a trap") declined "Put in Play
   ... targeting Elvish Piper" as the only creature in hand — scored JUSTIFIED in both reviews.
   R3's letter previously commanded the take; the exception now states it (a cheated Piper
   never attacks or blocks, so hold the activation for a real monster).

## What was KEPT verbatim (freeze check)

R1 (incl. THE FORBIDDEN THOUGHT), R2 with both corollaries (clean at every arm), R3 body
sentences, R4 in full, R5 step 1 and the mana-carry-over falsity fact, R6 in full, R8 in full,
all prior situation-table rows. No obeyed line's wording changed; new text is additive or (R5
first falsity fact) a quote-swap to the observed phrase.

## Card-fact audit (HL1 hard gate — full-guide ledger vs `Res/sets/primitives/*.txt`)

| claim | primitive | verdict |
|---|---|---|
| Kozilek {10} 12/12 annihilator 4, cast-trigger draw 4 | mtg.txt mana={10} power=12 text | TRUE |
| Ulamog {11} 10/10 indestructible annihilator 4, cast-trigger destroy | mtg.txt | TRUE |
| Emrakul {15} 15/15 flying annihilator 6, prot. from coloured spells, extra-turn cast trigger | mtg.txt abilities=nofizzle,flying,protectionfromcoloredspells | TRUE |
| Jin-Gitaxias, Core Augur {8}{U}{U} | mtg.txt mana={8}{U}{U} | TRUE |
| Sheoldred {5}{B}{B} 6/6, upkeep return / opp sacrifice | mtg.txt | TRUE |
| Elvish Piper {3}{G} 1/1, {G},T: put creature | mtg.txt | TRUE |
| Quicksilver Amulet cast {4}, {4},T: put creature; artifact | mtg.txt | TRUE |
| Kitchen Finks {1}{g/w}{g/w} 3/2 persist, gain 2, returns 2/1 | mtg.txt mana={1}{GW}{GW} | TRUE |
| Thragtusk {4}{g} 5/3 gain 5 | mtg.txt | TRUE |
| Pelakka Wurm {4}{g}{g}{g} 7/7 gain 7 | mtg.txt | TRUE |
| Time of Need {1}{G} search legendary creature -> hand | mtg.txt | TRUE |
| Fabricate {2}{U} search artifact -> hand; only artifact = Amulet | mtg.txt + deck116.txt | TRUE |
| Explore {1}{G} draw 1 + extra land | mtg.txt (exact name=Explore) | TRUE |
| Urban Evolution {3}{G}{U} draw 3 + extra land | mtg.txt | TRUE |
| Farseek {1}{G} finds Plains/Island/Swamp/Mountain, tapped, no Forest, no draw | mtg.txt | TRUE |
| Farseek's deck targets = Island / Tropical Island | deck116.txt (Tropical Island subtype=Forest Island; Island ZEN) | TRUE |
| 24 lands, no black source, cheapest creature 3 mana | deck116.txt | TRUE |

Teaches FIRED (behavioural: R1-R4, R6-R8 all engaged at arm C) AND teaches are TRUE (ledger
above) — both freeze lines satisfied for the kept core.

## Quoted-render-string sweep (arm-C deck116 prompts)

"Put in Play with Elvish Piper targeting" 3 / "...Quicksilver Amulet targeting" 7; "counted by
the engine" 13; "Cheapest spell in this hand" 12; "Mana available:" 282; "Cast nothing right
now" 75; block branch strings as in edit 2. ZERO-HIT string: R6's "already taken a mulligan"
phrasing — UNEXERCISED, not falsified (no game in any of the three arms took a single mulligan:
all 18 opening hands were engine-counted 3-land keeps). Kept at full prominence per the
pause-aware demotion precondition (a guardrail with zero opportunities is untested, not
vindicated).

## Adherence numbers (for the record; win rate irrelevant per ruling #1)

Arm A 121/126 = 96% (2 comprehension failures) - Arm B 115/116 = 99% (0) - Arm C 120/127 =
94.5% (1, the multi-block — harness-recoverable). Arm C results context: won vs146 (t11), vs36
(t15); lost vs139/vs152/vs105/vs158.

## Rotation view (adherence/maturity-shaped, per ruling #1)

The seat is near-mature: R1/R2/R3/R6/R7 essentially converged across arms, R4 is the corpus's
exemplar of a teach retrieving at its seam, and the one game-losing failure at arm C is
harness-recoverable (re-ask docket). Recommend ONE more corpus after the wave-36 harness batch
to validate Rule #9 + the re-ask at the block seam and the Farseek condition; if the block seam
holds, this guide is a freeze candidate. Witness caveat (tier review, HL10-style): Farseek was
offered 36 / cast 0 in wave-33 under the old demotion — the new no-land condition re-opens that
door, so deck116 becomes usable again as a witness for Farseek-path render items only if the
condition actually fires; check its firing count next corpus.
