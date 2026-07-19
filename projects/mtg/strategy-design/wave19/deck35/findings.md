# deck35 (Mighty Slivers) — REVISED-GUIDE ROUND 2 (Step 0-ter) — findings

Seat: deck35, GUR Sliver lord-stack swarm-race. AGGRO. **Second corpus with a guide; first with the
wave-18 revision** (two-sided-trust + forced-lethal-escape clause targeting the block-seam tax).
Record **3/6** (down from 4/6 — pool leader last wave). Binary /tmp/wagic-72b05535d.
Corpus matchups-20260719-092058.

WON: 62, 14, 27.  LOST: 110, 135 (adj), 49.

## HEADLINE 1: the FC1 model-durdle is STILL DEAD (second consecutive validation).

No game this corpus shows the model *choosing* to turtle, flip to full defense, or chump its swarm
away one creature at a time. Every game in which deck35 was **given an attack step** it attacked
aggressively: vs14 attacked t2->t12 every own-turn; vs49 attacked t3/t5/t7/t9/t11 (raced opp to 1);
vs62 t5/t9/t11/t13 (raced opp to -4); vs27 t2/t8/t10 (opp to -21). The defensive-spiral choice class
the guide was built to countermand did not fire once. **Do not trim the anti-durdle spine.**

## HEADLINE 2: the one 0-attacker game (vs135, the adj loss) is ENGINE-caused, NOT a model durdle.

This is the most important finding of the seat and it REFRAMES the durdle instrument.

vs135 (adj LOSS, deck35 16 / opp 18 at the turn-10 cap): deck35 issued **zero `attackers`
decisions** and dealt **zero combat damage** all game (opp's two -1s were its OWN fetchlands;
grep for "Damage: N dealt by ...Sliver" in the opp translog returns EMPTY). On the durdle instrument
this reads as a 0-attacker durdle. **Root-cause says otherwise:**

- The draw was slow and **blue-screwed**: opening hand had three Winged Slivers ({1}{u}) and NO blue
  source; first creature (Horned) did not land until **turn 6** (on the draw), Gemhide (blue fixing)
  turn 8.
- On **turn 8**, Horned Sliver (cast t6) was **unsick and untapped** -- an eligible attacker -- and
  **no `attackers` decision was issued** (the game continued to t9/t10, so this is not a turn-cap
  artifact).
- On **turn 10**, deck35's own stated PLAN (seq11 prompt) was explicitly aggressive: *"I am the
  aggressor in a race ... three slivers on the battlefield that are currently unblocked and can
  attack ... create an evasive swarm the opponent cannot stop."* It cast its board and **again got
  no attack step** before the cap.

The model WANTED to attack and had eligible bodies; the engine did not offer the declare-attackers
decision. This is **wave-18 notes ledger #2 CONFIRMED**, and it is now REINFORCED by a second,
independent game -- see below. The FC1 instrument correctly counted a 0-attacker game, but the
0-attacker cause here is an ENGINE non-issuance, not the model's defensive instinct. (Instrument
sharpening in skill.md S2.)

## HEADLINE 3: attack-step non-issuance is reproduced in a WON game too (vs27) -- so it is real.

vs27 (WIN): on **turn 4** (board: Striking t0 + Spinneret t2, both unsick) and **turn 6** (seq11
prompt renders FOUR slivers -- Striking, Spinneret, Gemhide, Winged -- all `[flying, first strike,
reach]`, **none `[tapped]`**, plan: *"attack with all four creatures to push for lethal"*), **no
`attackers` decision was issued**. Yet the same game DID issue attack steps on t2, t8, t10. The
non-issuance is **inconsistent** -- it fires some own-turns and not others -- which is exactly why it
needs engine-code confirmation rather than a misplay verdict (see notes.md #1; per doctrine I do not
assert a bug, I escalate a now-2-game-convergent WATCH with repro seqs). It cost only tempo in vs27
(won anyway); it cost the game in vs135.

## HEADLINE 4: the block-seam fallback DIED; the tax RELOCATED to trivial land drops.

Wave-18's dominant residual was the block-math tax (it produced that corpus's only fallback, 62 s16
-> unparsed). The wave-18 revision **held**: this corpus has **0 block fallbacks** (was 1). vs49 seq17
-- a block at **1 life** facing lethal, 10,071 chars -- PARSED cleanly ("BLOCKS: B1:A1, B2:A2",
correct survival gang-block). The reasoning is still long (the tax persists) but it no longer
self-poisons into an unparsed reply.

But the tax did not leave the corpus -- it **moved seams**. Both of deck35's fallbacks this wave are
**trivial LAND-DROP `ask` decisions** spiraled by blue-mana-screw anxiety:
- **vs135 seq3 (t2), 14,958 chars -> UNPARSED (Baka fell back).** The decision was
  `Play Mountain / Play Forest / Play no land`. The model oscillated for ~15k chars --
  *"I'll play a Forest. Actually a Mountain ... It doesn't matter. I'll play a Forest. Wait, I need
  blue ... I have no Island ..."* -- driven entirely by "I cannot cast my Winged Slivers without an
  Island," and **never emitted a committed `CHOICE:` line**. salvageLoopedChoice had nothing to
  salvage (no well-formed choice was ever produced).
- **vs135 seq6 (t7), 1,664 chars -> STALE_ECHO.** Same blue anxiety ("I need blue mana for Winged
  Sliver ... I have no Island ... I cannot cast Winged Sliver yet"), on a `Play Forest / Hold Forest`
  land drop; it echoed its prior plan instead of deciding.

Fingerprint: **a color-screwed swarm agonizing over which basic land to play.** This is a NEW
tax-relocation (Step 0-ter rule 4), distinct from wave-18's block tax. Guide + core-prompt response
below.

## Wave-19 engine-batch validation (my seat)

- **TRADE OUTCOME annotations: PRESENT, ACCURATE, and they IMPROVED play.** Block prompts now carry
  per-attacker "deal up to N -- you would be at M -- LETHAL/NOT lethal" plus per-blocker naive-trade
  tags. vs27 seq13: "Vodalian Zombie (4/4) **deals 4**" and "Bonesplitter Sliver (4/2) ... may block
  A1 (**you kill it, your blocker lives**)" -- the model took the clean first-strike profitable block
  (kills the 4/4, takes nothing) exactly as the guide's block exception wants. vs49 seq17: annotations
  correctly showed both blockers die but block-to-survive; model complied. **No misleading annotation
  found** (no wrong outcome printed at my seat). This also delivers the wave-18 G1 fix: the attacker
  line now states "deals 4" as POWER, separate from the (4/4) -- the toughness-as-power misread that
  drove wave-18's spiral is pre-empted in the representation.
- **Combat name tolerance / block discards:** 0 block replies discarded at my seat (was 1). The
  block seam produced no fallback of any shape.
- **salvageLoopedChoice:** did not fire usefully at my seat -- the one unparsed reply (seq3) never
  contained a well-formed CHOICE to salvage; it was pure indecision, not a mangled-but-present
  choice. Characterization of what still rambles: trivial land drops under color screw (above).

## Pool-lead 4/6 -> 3/6: variance + one engine loss, NOT a guide regression

Shared opponents vs wave-18: **62 flipped LOSS->WIN** (blue online this time -- "Island" renders
across the board, Winged cast; last wave's 62 loss was blue-screw). **49 flipped WIN->LOSS** but as a
near-dead-heat race (deck35 raced opp to 1, died at -3 on t12 -- variance in a coin-flip race, model
attacked every turn). **135 flipped WIN->LOSS(adj)** = the engine attack-step gap + a slow blue-screwed
draw (Headline 2). **110 LOSS held** = turbo-Affinity speed (Galvanic Blast x2 t1, then an 8-power
Etched Champion t5, dead t6 before any attack window -- a legitimate speed loss to the pool's fastest
deck, same as wave-18; not a durdle, killed too fast to attack). New entrant **27 = WIN**. Net -1 win
across the four shared decks, all attributable to variance (49), an engine gap (135), and a matchup
speed wall (110) -- none to the model turtling.

## What went RIGHT (do not re-teach)
- Attack-every-turn is the default posture wherever an attack step is offered.
- Multi-anthem buff math off the rendered [X/Y] stays clean (vs62 Muscle+Bonesplitter+Horned lethal
  reads; vs27 flying+first-strike stacked team).
- Block quality is now GOOD and cheap-to-confirm thanks to the trade annotations (vs27 seq13
  profitable first-strike block; vs49 seq17 survival block).
- Heart Sliver led when drawn (vs14 t2, vs49 t3); selective blocking at healthy life (vs135 seq9
  no-block at 18).
