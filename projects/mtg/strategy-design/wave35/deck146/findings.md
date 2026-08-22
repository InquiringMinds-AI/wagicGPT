# deck146 "Orzhov Dungeons" — wave-35 guide review (all three corpora; arm C weighted)

Rubric: owner ruling #1 (adherence + divergence quality; win tables context only). Arms A/B
findings are pre-digested in `wave35-review/deck146-tier-review.md` (cited, not re-derived);
this file adds the ARM C read (matchups-20260822-111102, Qwen3.6-35B-A3B-FP8, the standing
pilot the revised guide serves) and the guide verdicts across all three.

Arm C games (record context only, 2/6): W vs105 t12 (20/0), W vs139 t16 (3/-5); L vs152 t15,
L vs116 t11, L vs36 t17 (0/3 race), L vs158 t11. **Zero fallbacks in 178 records, zero
parse/comprehension fallbacks — the FP8 pilot's reply discipline is clean at this seat.**

## Arm C adherence — what held (kept verbatim in the revision)

- **Lost Mine by name 5/5** (vs105 s18, vs152 s13+s24, vs116 s9, vs139 s18); with A 11/11 and
  B 9/9 that is 25/25 across the wave. Room branches 8/8 body/drain over scry (goblin lair
  s24/s15/s12/s23; dark pool s34/s17/s20/s28 across the four games that ventured).
- **Verse on-time, target-list-obeyed, every game**: vs105 s9 (Blight Mamba t4), s14, s16
  (Hand of the Praetors); vs139 s7/s11/s14 (Grazer, Pollywog, Dryad — the ramp engine, on
  curve); vs152 s10 (Ranger Class over Wolf — the token engine, same pick arm A validated);
  vs158 s7 (Bowmasters); vs36 s4 (Salvage Slasher). Zero own-side casts, zero list arguments.
- **Verse-is-dead case executed**: vs116 s26/s28 held 3 dead Verses against the colorless
  Kozilek board — the guide's exact "win with bodies" case, stated in trace.
- **Lolth 3/3 first-use -3 Spiders** (vs116 s17-18, vs36 s16-17, vs139 s25-26), +0 draws on
  later quiet turns (vs116 s27, vs36 s24, vs139 s34) — the guide's exact planeswalker lines.
- **Agadeem's do-not HELD at ~10 cast offers** (vs105 s21/s25/s29, vs139 s14/s17/s20/s25/s31,
  vs36 s16/s20): the FP8 pilot never cast it, repeatedly choosing Command/Lolth/nothing over
  it. The arm-B B3 seq74 violation (cast it "returning Kaya", turn burned, lost from 21/8
  ahead) is the one cross-arm counterexample — see edit 5.
- **Annihilator 4 answered by sacrificing lands + the token, preserving real creatures**
  (vs116 s21-s24), same shape as arm B's praised B5 line.
- **Silencer namings all sensible**: Hand of the Praetors, Jin-Gitaxias, Nissa/Dryad,
  Vedalken Archmage, Intrepid Adversary, Orcish Bowmasters.

## Arm C failures — guide-attributable, each driving an edit

1. **Hard-floor breach via the true-mulligan license (vs152 s3, BELIEF MISS, guide-caused).**
   At "keeping 5" with a 1-land hand the floor commands KEEP ("the bar is ONE LAND, nothing
   else"). The trace QUOTES both rules — the floor AND "Only these are true mulligans, and
   only while you are still at 7 or 6" — matches the no-black-source pattern, and mulligans,
   keeping 4 (s4-s5). The license's scope qualifier "at 7 or 6" is unanchored: under London
   every hand SHOWN is 7 cards, so the qualifier reads as satisfied on every ask. This is the
   skill's HL2 bare-binding defect — the rule's number must be named by the render substring
   "(keeping N)". Contrast vs158 s3: same pilot, keeping-5 ask, hand NOT matching the license
   pattern -> quoted the floor and kept correctly. The license, when its pattern matches,
   beats the floor. Loss followed (t15).
2. **Bottoming is an untaught seam and it undid a correct keep (vs158 s4; also vs152 s5).**
   Kept 5 with 3 lands (Swamp + two Hives — the engine's count, the basis of the keep), then
   bottomed BOTH Hives, keeping one land and three {W} spells, with the plan "Draw a white
   land source immediately". Screwed turns 3-7 (zero castable decisions, verified — see
   notes.md), dead t11. vs152 s5 likewise bottomed a Swamp. The guide never mentions the
   bottom ask; the pilot treats lands as the expendable cards. Every mulligan keep passes
   through this seam.
3. **Post-combat pump: sorcery timing not taught (vs105 s25-s27, BELIEF MISS).** Main 1 t10
   (opp at 3): held Command ("Cast nothing... attack to win", s21). The attack fell short
   (blocked); in MAIN 2 it cast Command, mode "gains 3/3 and you draw" (s26), pump target
   Goblin, with the committed plan "Attack with the buffed 4/4 flying Goblin to win the
   game" — a second combat that does not exist. Pump wasted; won two turns later anyway.
4. **Command's one-creature target ask: opponent's creature picked for the PUMP (vs105
   s31-s32).** Mode pump+sacrifice; the target ask lists own creatures AND Plague Stinger
   with no mode attribution (docket item 1, now recurring at arm C); trace: "It implies one
   target for the whole spell... I'll stick with 4 [Plague Stinger]". Engine-verified
   (`borderline.txt` auto lines): the battlefield-creature target feeds `target(creature)`
   on the PUMP half — the sacrifice half is `notaTarget`, opponent-chosen. So this +3/+3'd
   the opponent's infect flyer. Prompt-defect-contaminated (not scored as pure model error),
   but a rules-true guide rule fully routes around it: the pump is the only battlefield
   target the spell ever wants, and it is always YOURS. (Arm C's other Command target,
   vs139 s33, picked its OWN Silencer — correct.)
5. **Hold-back rule violated twice in the vs116 loss chain (s25, s29 — BELIEF MISSES).**
   s25: sole blockers ask, Kozilek 12/12 (no trample) attacking, two untapped 2/1 Spiders,
   21 life; forecast line "deal up to 12"; trace: "they die anyway... Let it through" ->
   21->9. One Spider in front stops all 12. s29: at 8 life with Kozilek+Finks (15 power) on
   their board, attacked with ALL THREE untapped creatures, plan "Survive Kozilek's next
   attack by drawing removal" — died t11 taking 15. The guide's hold-back rule has no
   number and no chump-block value teach; "chip damage is worth nothing" did not bind.

Divergences scored defensible: vs105 s26 draw-mode aimed at SELF at 20 life (guide says aim
at opponent; but the mode's target also DRAWS — handing an infect deck a card at opp-3 while
holding lethal plans is worse; the guide line is miscalibrated, see edit 4b — arm B's B2
self-draw dig at 3 life is the same under-specification from the other end). vs36 s23 attack
at 7 life (crack-back computed in trace, opp offense ~2-3/turn — the rule's "next attack
would kill you" trigger genuinely not met). vs116 s22 sacrificing the Goblin token over the
4th land at the Annihilator ask — kept a land, token was Lolth fodder anyway.

## Focus item (brief): the arm-B Agadeem loss — wording, placement, or pilot?

Split verdict: the rules misread (Kaya-is-returnable) is pilot-owned comprehension — B3's
trace re-checked "creature cards" twice and still targeted a planeswalker. But the guide left
the door open: its do-not gives the OUTCOME ("nothing happens") with no CLASS FACT, so a
pilot holding a graveyard "target" can (and did) reason the premise stale ("now I DO have
something to return"). Per the skill's collapsed-absolute rule the guide must close the
argument: state creature-cards-ONLY, name Kaya and Lolth as never-returnable, and keep the
flat behavioral no-op. Placement is secondary but real: the do-not sits mid-paragraph inside
CLOSE THE GAME; it is hoisted to its own line. Arm C's 10/10 held offers say the current text
is sufficient for the standing pilot — the hardening is insurance against exactly the
argued-past failure the stronger pilot produced, at ~3 lines' cost.

## Guide verdicts (revision = constrained diff, turnaround-preservation mode)

FROZEN (obeyed, unreworded): engine-count mulligan doctrine; Verse block incl. target-list
truth; Kaya -3 decision tree (validated verbatim at arm A A1, per brief); Soul Shatter;
Acererak block (quiet-turn casts checked correct at A1/A4/B6); Lost Mine + room-branch
rules; Hive 4-mana line; MDFC not-lands teach ("never offers the land face" re-checked
against arm C: zero land-face offers in any Play/cast menu — the `// Pelakka Caverns`
strings are card-text dividers; the Flip Side toggle was handled correctly unaided, vs36
s11, so no guide text spent on it); planeswalkers-every-turn.

EDITED (each tied to evidence above): (1) mulligan floor/license re-anchored to the rendered
"(keeping N)"; (2) NEW bottoming rule; (3) Command: main-1-before-attack timing + (4a)
own-creature-only target rule + (4b) draw-mode recalibrated (drawback stated: the target
also draws); (5) combat: numeric hold-back trigger (their total creature power vs your
life) + one-blocker-stops-a-non-trampler teach; (6) Agadeem's hardened + hoisted.

Retroactive card-fact audit (HL1): all costs/P/T/loyalty/targeting claims in the revised
guide re-checked against `Res/sets/primitives/borderline.txt` this wave — Command modes and
per-mode targets, Agadeem's X-tier `creature[manacost=N]|myGraveyard` targets, Hive
`{3}{B}`, Kaya/Lolth loyalty numbers, Verse monocolored clause, Snarl/Pathway/Hive mana.
No false facts found in the deployed text; the draw-mode line was INCOMPLETE (omitted the
target's draw), fixed in 4b.

## Rotation-relevant observation

Adherence at the standing pilot is high (~95-97% of ~170 scoreable decisions; 0 fallbacks, 0
comprehension failures of the arm-B class) and every arm-C loss traces to authored guide gaps
(mulligan license, bottoming, combat value) rather than pilot ceiling. Not rotation-ready:
this wave's edits are substantive and the bottoming + hold-back teaches need one validating
corpus. Guide maturity: mid — core doctrine frozen three waves running, edges still being
authored.
