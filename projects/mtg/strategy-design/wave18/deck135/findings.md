# Deck-135 (Modern Snow) — Wave-18 game review

Corpus: PRIMARY `matchups-20260716-185930` (21-game round-robin over 110/135/21/62/49/35/14;
binary `/tmp/wagic-f664539a3` = wave-17 engine batch: TARGET CHOICE sub-menu framing; hand-card
PUT_INTO_PLAY dead-end filter (Mox E6 fix); beneficial opponent-only-target cast suppression
(E-49c); explicit "BLOCKS: none" example; "(player, life N)" echo-suffix strip). deck17 rotated
OUT (first converged exemplar); deck14 (Deep Blue) entered GUIDELESS. All six deck135 seat games
read decision-by-decision. Doctrine: win column CONTEXT ONLY; unit of analysis is the DECISION
with seq repros, layer-attributed; adopt-a-revision bar is HIGH (veteran seat, guide FROZEN since
wave-11). deck135 owns this cycle: (a) did the TARGET CHOICE sub-menu framing unlock its control
game (best record 1/6 -> 3/6)?; (b) Force play-quality watch (face-burn-Force class, 4th corpus);
(c) Azcanta/optionone fingerprint NOTES ONLY (known-open); (d) stale `[attacking]` standing watch;
(e) 3 unparsed — characterize.

## RECORD (context only — NOT a verdict) — 3W / 3L (best since entering the pool)

| game    | opp                    | outcome (deck135)          | my/opp life | turn | class |
|---------|------------------------|----------------------------|-------------|------|-------|
| 135v62  | 62 Enchantresses       | **WIN** (real, kill)       | 3 / -2      | 14   | raced with Golems+Viper+Coatl; NO Azcanta activation this time (last wave's loss cause absent) |
| 135v14  | 14 Deep Blue (new)     | **WIN** (real, kill)       | 4 / -3      | 14   | curved Golems -> Diamond Faerie flying finish; clean race |
| 135v110 | 110 Etched Affinity    | **WIN** (adj, ahead)       | 14 / 13     | 8    | the CONTROL game working: Treefolk ETB tap + Gelid Shackles on Master of Etherium |
| 135v35  | 35 Mighty Slivers      | LOSS (real, dead)          | -7 / 17     | 9    | Force DEAD vs all-creature aggro (repro); slow ground clock overrun |
| 135v21  | 21 Goblin Gang         | LOSS (real, dead)          | -3 / 20     | 9    | fast Goblin burn outraced a slow start; Treefolk landed T6 too late |
| 135v49  | 49 Dragons             | LOSS (real, dead)          | -1 / 16     | 9    | blue-screwed early (Heath can't fetch Island) + Dragons/burn draw; was a WIN last wave |

Win table assigns deck135 3/6; matches. Record path: 1/6(w13)->0/6(w14)->2/6(w15)->1/6(w16)->
1/6(w17)-> **3/6(w18)** = best since entering the pool. The two wave-17 wins/losses REVERSED
(beat 62, lost 49) — draw/matchup variance around an unchanged construction floor — but the three
wins now trace to the CONTROL/TEMPO game EXECUTING cleanly (correct target sub-menu picks + finisher
deployment), which is the real, framing-attributable improvement (charge a).

---

## (a) TARGET CHOICE SUB-MENU FRAMING — DID IT UNLOCK THE CONTROL GAME? Verdict: YES. Framing produced correct, high-value target picks in EVERY instance (8 target decisions, 0 stale_echo, 0 wrong pick); the old confusion seam at THIS seat is CLOSED and the wins ride on it.

The framing was exercised **8 times** at this seat (Into-the-North fetches + two combat-relevant
taps/enchants) and parsed CORRECTLY every time — the exact sub-menu that produced 2 stale_echo
fallbacks LAST wave (vs62 s22/s23 Into-the-North "Snow-Covered Forest" echo) is now CLEAN (0
stale_echo at this seat this corpus).

**The control game working — vs110 (adj WIN), the cleanest data point:**
- **s11 (Abominable Treefolk ETB tap):** `TARGET CHOICE for Abominable Treefolk ... -> Signal Pest`.
  The framing named the pending ETB tap; the model correctly picked the opponent's biggest threat
  (Signal Pest, 5/1 with Battle Cry via Cranial Plating) over the tapped Ornithopter. Right pick.
- **s13/s14 (Gelid Shackles):** `TARGET CHOICE for Gelid Shackles ... -> Master of Etherium (12/6)`.
  The model reasoned the full block math (Master survives a 5-damage Treefolk and swings back for
  lethal; shackle it so it can't block) and picked the 12/6 — the single highest-value target on the
  board. Textbook control removal.
- Result: Treefolk + Boreal Druid swung for 7 (opp 20->13), Master neutralized, deck135 held at 14
  and WON on adjudication (14 v 13). This is the deck's control/tempo plan executing end-to-end,
  enabled by the framing turning two opaque "pick a target" menus into correctly-resolved decisions.

**Into-the-North fetch sub-menus (the old stale_echo seam) — all CLEAN this corpus:**
vs21 s11 (Forest), vs35 s22 (Forest), vs49 s14 (Island) / s19 (Forest), vs62 s14 (Forest), vs14 s23
(Plains) — every fetch resolved to the correct color the model needed, with the framing's "this is
NOT a cast or phase step" line preventing the prior-wave echo of an unoffered basic. 0 stale_echo,
0 misfire.

**No NEW confusion shape from the framing itself.** Where the model still spiraled (the 3 unparsed,
charge e) the trigger was a LOSING/ambiguous board, NOT the target-choice framing. VERDICT: the
sub-menu framing removed the old confusion seam and the three wins are built on correctly-resolved
target decisions. Charge (a) answered YES at this seat.

## (b) FORCE PLAY-QUALITY WATCH (owned). Verdict: the specific FACE-BURN NEVER-line was NOT violated this corpus; but the broader reflexive-Force class MUTATED and recurred a 4TH corpus — two off-guide 2-for-1 casts on non-whitelist noncreatures, BOTH non-fatal (won both games).

deck135 drew Force in three games (vs35, vs62, vs14); it was OFFERED and CAST twice (vs62 s12,
vs14 s14), both the FREE exile-pitch mode on the OPPONENT's stack, both resolved cleanly
(stack->exile, ENGINE #1a regression watch CLEAN). NEITHER was face-burn — so the guide's hardest
verbatim "NEVER Force a burn spell pointed at your own face" line was NOT triggered this corpus. BUT
both casts violate the guide's GENERAL Force gate ("spend it ONLY on a board wipe, a mass-bounce
that clears your board, or a spell that would kill your ONLY threat"):

| seq | window | Force target | pitched | assessment |
|-----|--------|--------------|---------|------------|
| vs62 s12 | T5, 19 life, racing | **Primal Rage** (enchantment: gives opp creatures trample) | **Abominable Treefolk** (its finisher!) | OFF-GUIDE: a pump/trample enchantment is not a wipe/mass-bounce/only-threat-kill. WORSE — the model's PLAN said it would pitch "Into the North (a blue card)"; Into the North is `{1}{g}` = GREEN, so the engine auto-pitched the only real blue card, **Treefolk the finisher**. Spent Force + finisher to counter a minor enchantment. Won anyway (raced with Golems). |
| vs14 s14 | T5, 18 life | **Unsummon** (bounce) targeting its own Icehide Golem | Search for Azcanta | OFF-GUIDE: a bounce does NOT kill — the Golem returns to hand, recastable for `{1}`. Force is a 2-for-1 (Force + Search) to prevent a temporary tempo loss on a 2/2. Won anyway (Deep Blue slow). |

**Class read:** this is the SAME underlying error as the 3 prior corpora — reflexive Force on a
low-impact noncreature — now MUTATED to a pump-enchantment and a single-target bounce (neither on
the guide's NEVER-list, which enumerates only face-burn + discard). It is MODEL over-application of
an already-general rule (the whitelist governs; both targets clearly fail it), NOT a face-burn
recurrence and NOT (per prior-wave reasoning) worth a 4th restatement in the guide — the line is
already maximal and BOTH instances cost ZERO games. Route to model-watch + general-suggestions #1.

**Annotation-design feedback (per brief's invitation).** The owner ruling replaces the beneficial
opponent-only-target suppression with a WARNING ANNOTATION on the option line. vs62 s12 / vs14 s14
are exactly the spots that annotation should catch: the option `Cast Force of Negation with its
exile a blue card ... can target on the stack: <X>` should carry a warning when `<X>` is a low-value
noncreature (pump/anthem enchantment, single-target bounce, face burn) — i.e. warn on off-whitelist
Force targets, and (vs62 s12) name/warn WHICH card will be exiled when the free pitch would eat the
caster's only blue card / its finisher. Here the suppression did NOT hide a correct play; the
opposite — the model made a bad-but-legal Force a warning annotation would have flagged. -> notes
ENGINE #1d (NEW) + strengthens the annotation design.

## (c) AZCANTA / OPTIONONE FINGERPRINT (NOTES ONLY — known-open, not re-diagnosed).

**ENGINE #4 (Azcanta "Choose target N of exactly 4" ORDERING menu) did NOT recur** — deck135 never
transformed Search for Azcanta into Azcanta, the Sunken Ruin this corpus, so the shrinking-ordering
menu that whiffed twice last wave had no trigger. Fix remains UNCONFIRMED (no exercise). The
**optionone reveal auto-decline** fingerprint DID appear at Search-for-Azcanta-style upkeep
top-look: vs35 s14 events `Your Misty Rainforest is revealed (from library) / ... goes to library /
Phase: Draw / ...: library -> hand` (a LAND revealed, the optional put-to-X auto-declined back to
library, then drawn normally on the draw step); vs35 s19 same shape with Force of Negation. Matches
the KNOWN-OPEN engine item — the engine auto-declines the optional reveal chooser before the model
can answer; the model never had the choice. Notes only. -> notes ENGINE #4b.

## (d) STALE `[attacking]` BOARD-TAG (owned): REPRODUCED again, benign, 5th+-wave watch.

Own TAPPED creatures render `[attacking]` in Main phase 2 (post-combat, YOUR turn, no active
combat): vs62 s26/s32/s33/s34 (Golems/Viper/Coatl that just attacked), vs110 s18 (Treefolk+Druid
post-swing). Opponent-battlefield `[attacking]` at Blockers-step windows (vs21 s4, vs49 s21, vs14
s15) are LEGIT in-combat, not stale. Every stale own-creature instance was on a tapped/inert
creature and drove NO wrong decision. Identical wave-13/15/16/17 defect stands on the ledger,
cosmetic, 0 cost. FIX (unchanged): clear `[attacking]` on any creature not in the current
declared-attackers set. -> notes ENGINE #2.

## (e) THE 3 UNPARSED — characterized: ALL degenerate REPEAT-LOOPS (decode-time repetition spirals) on hard spots; NOT stale-plan spirals; NONE flipped a winnable game.

Exactly 3 unparsed_reply fallbacks (0 stale_echo, 0 defer, 0 empty — an improvement on the seat's
prior profile). All the SAME pathology: the model repeats one or two sentences dozens-to-hundreds of
times until the token limit truncates the reply, leaving no parseable CHOICE line.

| seq | spot | loop | consequence |
|-----|------|------|-------------|
| vs49 s22 | 5 life vs lethal Dragons/burn, no creature in hand, binary "Play Prismatic Vista / Hold" | "I must cast Prismatic Vista... No, I need a creature. I don't have one." ~100x | game already being lost (died T9); no play lost |
| vs110 s19 | 14 life, opp T9 upkeep, only a Windswept Heath fetch offered; game ALREADY WON on adjudication at T8 | "So I need to pass and hope they don't attack. But they will..." ~60x (panicking whether shackled Master can still attack) | POST-adjudication — harmless, game already won |
| vs62 s31 | Ohran Viper "Draw 1 / Decline" combat-damage trigger; confused by the log showing "Opponent's life -1 (now 2)" twice | loop reconciling the life accounting | whiffed a free draw; game won anyway (killed opp to -2) |

Common trigger: a LOW-LIFE or ambiguous-board spot on a phase-restricted / binary / trigger menu
where the model recognizes a hopeless or confusing situation and spirals. Confirmed NOT the
PLAN-caveat stale-plan class (the brief says that class should be absent — it is; none of the three
carry a stale prior-turn PLAN). NONE flipped a winnable game (two in already-lost games, one
post-adjudication in a won game). A 4TH repeat-loop (vs35 s19, "activate Misty Rainforest... another
green source..." ~15x) PARSED (choice=1) so it is not a fallback, but it is the SAME pathology — the
repetition-loop tax is the DOMINANT residual failure shape at this seat now, broader than the 3
unparsed. Model/decode item, NOT guide-fixable. -> general-suggestions #2, notes HARNESS #2.

## RECORD — CONSTRUCTION vs PLAY SPLIT (owned). Verdict: WINS trace to the control/tempo game executing (framing + finisher deployment); LOSSES trace to CONSTRUCTION/matchup + draw variance, NOT to play errors.

**WINS (control/tempo executing):**
- vs110 (adj): Treefolk ETB tap Signal Pest + Gelid Shackles on Master of Etherium (both correct
  sub-menu picks) neutralized both blockers and won the race. The framing directly enabled it.
- vs62 (real kill): deployed Golem T2/T3, Ohran Viper, Ice-Fang Coatl; raced Enchantresses' Birds
  of Paradise (grew to 4/9 via Web + Ancestral Mask) to death — NO Azcanta activation this time, so
  last wave's loss cause was simply absent. The off-guide Primal Rage Force (b) cost the Treefolk
  finisher but the small-creature race still closed.
- vs14 (real kill): kept deploying Icehide Golems (Rule #1), landed Diamond Faerie as the flying
  finisher, alpha-struck over Lord of Atlantis for the kill. Clean curve-out + correct finisher read.

**LOSSES (construction/matchup + variance):**
- vs35 (real): flooded on Force of Negation (DEAD vs an all-creature Sliver deck — Force counters
  only NONCREATURE) + fetches; only Boreal Druid + one Ice-Fang Coatl deployed; slow ground clock
  overrun T9. Construction — the Force-dead-vs-aggro flag STANDS (repro'd again).
- vs21 (real): fast Goblin burn (Spark Elemental, Lightning Bolt x2, Volcanic Hammer, Ram-Gang)
  outraced a slow start; Golem traded T1, Treefolk landed T6 (7 life) but too late; died T9.
  Construction/matchup (slow clock vs fast burn). Minor play nit: paid 1 life to crack Prismatic
  Vista at 7 life vs a burn deck (s16) — marginal, not decisive.
- vs49 (real): blue-screwed early — the only fixing was Windswept Heath, which can fetch ONLY
  Forest/Plains, not the Island deck135 needed for its blue half (model correctly diagnosed this
  after a long ramble at s7); Dragons drew Dragonmaster Outcast + burn (Lava Spike + Bolt) and
  raced it out. Defended reasonably (Ice-Fang Coatl deathtouch block killed the Whelp). Matchup/draw
  variance — this was a WIN last wave, flipped this wave on who curved out.

**PLAY errors this corpus:** the 2 off-guide Forces (b, both non-fatal wins) + the 3 repeat-loop
fallbacks (e, 2 in lost games, 1 post-adj). NO Rule #1 hard creature-skip; NO wrong target pick; the
control game (target sub-menus, finisher deployment, Shackles) executed WELL. Split verdict:
construction/matchup-dominant losses + variance, with the three WINS attributable to the framing +
clean control execution. The record improvement 1/6 -> 3/6 is real and partly framing-attributable
(0 stale_echo, clean target play) + favorable matchup draws.

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide; `cmp`-verified).

No new GUIDE-fixable decision class. The reflexive-Force class (b), though mutated to two new shapes
(pump-enchantment, single bounce), is the SAME model over-application of an already-maximal general
rule — prior waves explicitly rejected further Force restatements ("the line is already maximal") and
BOTH instances this corpus cost ZERO games. Per wagicgpt doctrine (fix at the engine/representation
layer first), this class is best addressed by the INCOMING option-line warning annotation (owner
ruling), NOT a 7-wave-freeze-breaking guide edit — the annotation sits where the model actually reads
the Force target, and vs62 s12 / vs14 s14 are precisely the spots it should flag (notes ENGINE #1d).
Every other residual routes off the guide: stale `[attacking]` -> engine (ENGINE #2); Azcanta ordering
menu (unexercised) + optionone auto-decline -> engine (ENGINE #4/#4b); Force pitch auto-exiles the
finisher with no card-selection surfaced -> engine/representation (ENGINE #1d, NEW); reflexive-Force
target choice + repeat-loop spirals -> model-watch -> general-suggestions #1/#2. Guide diff = 0 bytes.

## GAME -> FILE MAP (run = matchups-20260716-185930)
| opp | outcome | turns | final (me v opp) | deck135 seat file |
|-----|---------|-------|------------------|-------------------|
| vs62  | WIN (real)  | 14 | 3 v -2  | `1784248080-ai_baka_deck135-0x55c2b905a1f0-vs-ai_baka_deck62.jsonl` |
| vs14  | WIN (real)  | 14 | 4 v -3  | `1784253245-ai_baka_deck135-0x559a26af4210-vs-ai_baka_deck14.jsonl` |
| vs110 | WIN (adj)   | 8  | 14 v 13 | `1784252806-ai_baka_deck135-0x56309f9031d0-vs-ai_baka_deck110.jsonl` |
| vs35  | LOSS (real) | 9  | -7 v 17 | `1784246373-ai_baka_deck135-0x55acb952e1a0-vs-ai_baka_deck35.jsonl` |
| vs21  | LOSS (real) | 9  | -3 v 20 | `1784250236-ai_baka_deck135-0x56293bd4a210-vs-ai_baka_deck21.jsonl` |
| vs49  | LOSS (real) | 9  | -1 v 16 | `1784250958-ai_baka_deck135-0x55a14eeab2e0-vs-ai_baka_deck49.jsonl` |
