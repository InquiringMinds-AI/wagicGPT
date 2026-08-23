# Wave-36 validation — deck146 (Orzhov Dungeons) + deck139 (Mutants)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-155508` (FP8 pilot, revised wave-35
guides, binary bb3cc9b12). Deployed `bin/Res/ai/baka/deck146_strategy.txt` and
`deck139_strategy.txt` verified byte-identical to the wave-35 revisions before review.
Seat games: deck146 6 (5W-1L: W vs139/vs116/vs36/vs152/vs105, L vs158); deck139 6 (2W-4L:
W vs36/vs105, L vs146/vs116/vs152/vs158). Record counts: 146 seat 273, 139 seat 233.
**Fallbacks: 0 / 0 across all 506 records** (arm C had 3 at the 139 seat, 0 at 146).
Rubric: owner ruling #1 — adherence + divergence quality; win tables context only.

## deck146 — wave-35 fix-by-fix

1. **Mulligan-license fix + bottoming rule: UNEXERCISED.** 6/6 games kept the opening 7
   (every hand: engine-counted 3 lands with {W}>=2 and {B}>=1 — all rule-correct keeps).
   Zero "(keeping N)" asks, zero bottom asks, zero both-rules-quoted mull-downs. The
   authored-gap fixes drew no witnesses; nothing refuted, nothing proven.
2. **Silverquill Command timing/target — 4 casts, 3 clean, 1 timing recurrence.**
   - vs158 s13-s15 (t7): pump+sac in MAIN 1, own creature (Triumphant Adventurer)
     targeted, attacked same turn — the guide's exact line.
   - vs139 s16-s18 (t11): pump+sac in main 1, attacked; sole-creature target auto.
   - vs158 s29-s30 (t12): return+sac with Adventurer in yard — legal, note-guided (see 4).
   - **vs152 s23-s25 (t14, MAIN 2): pump+sac with the plan "Pump Silencer to 6/5 ...
     I will attack with Silencer next turn"** — the forbidden plan shape verbatim
     (pump is ueot; Silencer was summoning-sick, no combat existed). Sac half still
     2-for-1'd a Wolf and the game was won, but edit 3's teach did not bind. BELIEF MISS.
3. **compoundModeTargetNote (lane-A item 2): VALIDATED — prediction 2 holds.** Both fused
   pump-target asks carried the full note ("Each part that targets asks for its target
   SEPARATELY ... picking an enemy creature HELPS the enemy ... chosen BY THE OPPONENT"):
   vs158 s14, vs152 s25. Own creature picked 2/2; zero enemy pumps; zero whole-spell-
   ambiguity traces (the arm-C s32 failure class is gone from this corpus).
4. **Agadeem do-not: UNEXERCISED (Agadeem never drawn/offered — 0 prompt hits).** The
   `dynamicMagnitudes` graveyard-return clause fired instead on Silverquill Command's
   cast line 4x (vs139 s10/s13/s16, vs158 s12): "{right now: returns NOTHING - your
   graveyard has no creature cards, so the return part cannot happen at any X}".
   Consumed correctly every time — vs139 s16 trace: "Graveyard has no creatures. Return
   mode does nothing" -> pump+sac chosen; and at vs158 s30, note absent (creature now in
   yard), return mode taken. The clause is doing real steering work on compound casts.
   Minor new defect: "at any X" is Agadeem-phrasing rendered on a spell with no X.
5. **Numeric hold-back trigger: armed ~4x, fired 0x.** The clear miss: vs152 s29 (t16,
   6 life) — trace computed "Opponent has 7 power but tapped. I have 6 life. Safe to
   attack" and attacked; the rule's text pre-empts exactly this ("the tapped ones too,
   they untap"). Took 5, survived at 1 on topdecked removal. The other armed cases are
   defensible divergences: vs152 s42 (2 life) was the lethal push that won the game;
   vs152 s35 (1 life) attacked but kept the Goblin home and chump-blocked Katilda —
   partial compliance that worked; vs158 s40 (2 life) the only attacker was the animated
   Hive, which is not a blocker next turn anyway (zero defensive cost; racing = only out).
6. **One-blocker-stops-a-non-trampler teach: 1 failure, 1 defensible.** vs152 s26 (10
   life, forecast "you would be at 6"): declined the Silencer chump on the FALSE model
   "Block: ... I take 4 damage / No block: I take 4 damage. So blocking is strictly
   worse" — a blocked non-trampler deals nothing; edit 5's exact sentence did not bind.
   BELIEF MISS. vs152 s30 (forecast "at 1"): declined but with a live removal line
   (Verse on the Wolf next turn, executed s31-s32) and won — defensible audible.

FROZEN doctrine held: Lost Mine named by name at every dungeon list (all 6 games, incl.
repeats — vs139 s26, vs116 s12/s34, vs36 s16/s27/s41/s58, vs152 s28, vs105 s9/s33, vs158
s8); room branches all body/drain over scry (goblin lair / dark pool / mine tunnels picks
throughout); Verse casts obeyed the target list every time (Thragtusk s43, Beast s48 at
vs116; no own-side casts, no list arguments); Soul Shatter only with creatures present;
Acererak cast only on quiet turns (vs152 s33 after Verse resolved); Lolth -3 Spiders first
use then +0 draws (vs116 s32/s35, vs105 s27/s34, vs36 s31); Kaya cast on curve and
activated every turn she lived (vs158 s19/s31, vs152 s18).

## deck139 — wave-35 fix-by-fix

1. **Mutate over/under (G1 rewrite): 7/8 rule-correct — largely VALIDATED, one miss.**
   - over (5/5 correct): Brokkos-onto-Dryad (v116 s22), Greathorn-onto-Grazer (v36
     s33-s34; v152 s29 — trace: "Biggest printed P/T goes on top. 3 > 0 ... over.
     Correct."), Gemrazer-onto-Pollywog (v152 s23), Gemrazer-onto-Dryad (v36 s43-s44),
     Brokkos-onto-the-2/4-pile (v105 s32 — over restored the 6/6 top, repairing the miss
     below).
   - under (2/3 correct): **Snapdax-under-the-6/6-Brokkos-pile (v116 s30) — the exact
     wave-35 failure shape, now played correctly with the guide cited: "The guide
     explicitly says to mutate under when the host is bigger."** Everquill-under-Brokkos
     (v105 s46) also correct.
   - The miss: **v105 s21-s22 — Illuna (6/6) mutated UNDER Dryad (2/4)**; the pile
     rendered 2/4 (s33 confirms "Dryad ... (2/4) [trample, flying, mutate]") while the
     plan claimed "a 6/6 flying trample threat". The rule's over-branch ("the card you
     are CASTING is the bigger half -> mutate over; an Apex onto a Pollywog: over")
     directly covers this; execution miss, not a text gap. BELIEF MISS, self-repaired
     two turns later by Brokkos-over.
   - Host choices: small engine hosts throughout (Grazer/Pollywog/Dryad); no 3/4 was
     ever put over a 6/6.
2. **Dryad-on-curve exception: UNEXERCISED** — no casting menu in the corpus listed
   "Cast Dryad" and a mutate line together. Dryad itself cast on curve in every game she
   appeared (v152 s7 t2, v105 s8 t2, v158 s8 t4, v116 s13 t4).
3. **Poison full-coverage procedure: UNEXERCISED at its trigger** — the M>=10 lethal
   forecast never occurred (max "3 of 10", v105). Sub-lethal poison blocks were sound:
   both attackers covered when two Myrs attacked (v105 s26 — Grazer absorb + Greathorn
   kill), single-Myr chumps taken with the [deals 0] absorb annotation read correctly
   (s38, s42). Seat WON the infect game at 16 life / ~2 poison.
4. **Shockland asks (named lands in options: yes, all six shock names grep-confirmed).**
   20 genuine pay/tap asks: 14 tap / 6 pay-decisions of consequence.
   - Fallbacks on the class: **0 (prediction 1's fallback half VALIDATED; arm C had 3/3
     here)** — though see the engine finding below: the fix never engaged, so the credit
     plausibly belongs to the FP8 pilot's reply discipline.
   - Rule-justified pays 3 (same-turn casts: v116 s3 t0 Grazer, v116 s12 t4 Dryad, v152
     s3 t0 Grazer — textbook). Unjustified normal pays 3 = 6 life (v146 s11 t5 and s15
     t9, v105 s10 t2 — nothing cast those turns; verified against the following logs).
     The 12-or-less line held (no pay at <=12 anywhere). The leak class D2 is WORSE this
     corpus (3 vs arm C's 1) with unchanged rule text — watch item.
   - **No-op pays on Grazer-put tapped lands: 2 = 4 more life (prompt-defect-
     contaminated, not scored against the guide)** — see engine finding.
5. **Discard seam (D3): EXECUTED.** Land listed -> land pitched 6/6 (v105 s19/s20
   Plains, s30/s31 Stomping Ground, s44/s45 Sacred Foundry). Landless else-branch 4/4
   sensible: duplicate Pollywog pitched per the order (v36 s32); Greathorn kept-vs-
   pitched judgments defensible (v116 s21 kept Snapdax and in fact mutated it t10).
6. Attack floor + [deals 0] obeyed (no-attackers only on 0-power walls: v116 s10/s14,
   v36 s14/s28, v152 s8/s11/s17/s20); mulligans 6/6 rule-correct keeps (3 lands, {G}>=1
   every hand); G3's {G}=0 gate untested.

## BOTH seats — trust checks

- **Trade-parenthetical trust: holds.** Every sampled block into a printed "your blocker
  dies" line carried explicit trace acknowledgment (139v158 s26 "Grazer dies, Orc
  lives"; 139v152 s25; 139v146 s20 quoting the engine line verbatim; 146v36 s35). Zero
  prior-override incidents (no trace re-derived a trade against the parenthetical). The
  one damage-model error (146v152 s26 above) trusted the parenthetical but mis-modeled
  unblocked damage — a guide-teach failure, not annotation distrust.
- **becomes-blocked "+2/+2" annotation: consumed correctly.** It rides inside the
  engine-computed trade ("includes its +2/+2 when-blocked trigger", all Ichorclaw asks);
  the printed results (e.g. "you kill it, your blocker lives (infect shrinks your
  blocker to 0/1)") matched later board states and no trace second-guessed them.
- Lane-A prediction 5: no combat-suppression stall — 0 `[combattrace]` lines in all 24
  .stderr files and no >=2-consecutive-own-turn attacker-ask gaps at either seat.
- Lane-A prediction 8: "BOTH your main phases" fact present (21 prompts across the two
  seats); zero numberless pre-main "CHOICE: Cast X" hallucinations.

## NEW DEFECTS (engine/docket — routed, not guide)

1. **Lane-A item 1 (shockland seam) — prediction 1 REFUTED on the annotation half; the
   stripped-copy caveat is confirmed live.** At least 7 pay/tap asks armed on
   already-tapped Grazer-put lands and every one reached the model with the OLD
   unconditional "pay 2 life - enters UNTAPPED [usable ... this turn]" text; no NO-OP
   annotation, no auto-decline. Two were PAID (139v105 s6 t0 Stomping; 139v36 s8 t4
   Overgrown Tomb #2) — 4 life for nothing (139v36 s9-s10: OT#2 still `[tapped]` on the
   following turn). The lying narration also recurred verbatim: "Overgrown Tomb entered
   untapped (you paid 2 life ...)" (139v36 s10 log; same line 139v105 s7). Per the
   lane-A caveat: `ctx->isTapped()` is unreadable at the seam (stripped copy) — **the
   fix must move to the menu-arming flow**, and the consumed-decision narration needs
   the same truth source.
2. `dynamicMagnitudes` graveyard-return wording: "cannot happen at any X" renders on
   non-X compound spells (Silverquill Command). Behavior correct and beneficial;
   wording is Agadeem-specific. Cosmetic — reword to drop the X clause when the cast
   has no X.
3. No regression found from compoundModeTargetNote, the whole-hand reveal splice, or
   the blockers-declaration hold (no missing-blockers combats at either seat: every
   defender combat with a legal block shows a blockers record).

## Verdicts and recommendations

- **deck146: adherence ~97% of ~250 scoreable decisions (0 fallbacks). NOT freeze.**
  The two headline wave-35 authored-gap fixes (mulligan license, bottoming) drew zero
  witnesses, and the combat-value edits are the ones that misfired where armed (s26
  chump decline on a false damage model; s29 tapped-power override; s23 main-2 pump
  plan). Command target/note + returns-NOTHING behavior can be considered settled. Next
  corpus should decide whether the combat teaches need wording escalation (the
  one-blocker-stops-all sentence failed at its exact target shape) — hold text as-is
  for one more corpus first; all three misses were non-fatal and two games were won
  through them.
- **deck139: adherence ~97% of ~200 scoreable decisions (0 fallbacks; arm C had 3).
  NOT freeze yet — but close, and no text changes recommended.** The G1 rewrite is
  executing (7/8, including the exact prior failure shape played correctly with the
  guide cited); the single Illuna-under miss and the 3 shockland leak pays are
  execution misses against text that already states the right rule. Dryad-conflict,
  lethal-poison, and {G}=0 gates remain unexercised — do not hold freeze hostage to
  them; the freeze gate should be: one corpus with zero over/under misses and <=1
  shock leak. Engine item 1 above must land first, since the put-land ask class keeps
  injecting contaminated pay decisions into this seat's ledger.
