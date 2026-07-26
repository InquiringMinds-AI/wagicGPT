# deck122 (Eternal Life -- WB lifegain / Felidar alt-win / Sorin) -- wave 29 findings

STEP-0 GUIDELESS DEBUT. Record 2/6. Binary 50eeed8be, corpus matchups-20260726-115018,
-T 3000, REPPENALTY=1.05. 6 games, ZERO fallbacks across all deck122 seats (clean).

Deck (bin/Res/ai/baka/deck122.txt): 34 creatures + 2 Sorin, Lord of Innistrad + 24 lands.
Lifegain engine (Soul Warden, Suture Priest, Lone Missionary, Kitchen Finks, Kabira
Crossroads, lifelink beaters) -> payoffs (Ajani's Pridemate, Serra Ascendant, Divinity of
Pride, Felidar Sovereign, Blood Artist drain).

## Record decomposition (2/6)

results.tsv (deck122 is the SECOND deck in each row):
- 93 v 122  -> 93 wins, 122 at -11, turn 12  (LOSS -- aggro race)
- 146 v 122 -> adj0 (146), 16 v 15, turn 13   (LOSS -- razor cap-adjudication, -1 life)
- 139 v 122 -> 122 wins, 18 life, turn 15     (WIN  -- grind)
- 136 v 122 -> adj0 (136), 12 v 6, turn 18    (LOSS -- long grind, cap-adjudication)
- 137 v 122 -> 137 wins, 122 at -9, turn 14   (LOSS -- aggro race)
- 148 v 122 -> 122 wins, 18 life, turn 15     (WIN  -- grind)

Pattern: beats slower midrange in the grind (139 Mutants, 148 Kor), loses to aggro
(93 Rats, 137 GW convoke) and loses two razor-close adjudicated grinds (146, 136). The
lifegain is piloted as pure DEFENSE (a cushion to spend), never as a WIN AXIS. Neither the
anthem-threshold payoffs nor the Felidar 40-life win are pursued. Max life across the whole
corpus = 31 (vs93); the deck never climbed toward 40.

## SURFACE 1 -- Felidar Sovereign alt-win (engine + model)

ENGINE: IMPLEMENTED and correctly wired.
- mtg.txt:39654 `name=Felidar Sovereign`
  `auto=@each my upkeep:this(controllerlife > 39) wingame`
  `text=Vigilance, lifelink -- At the beginning of your upkeep, if you have 40 or more
  life, you win the game.` -- oracle-faithful (40+, own upkeep).
- `wingame` parses to AAWinGame (MTGAbility.cpp:4915). AAWinGame::resolve
  (AllAbilities.cpp:7590) sets the OPPONENT as loser, honoring CANTLOSE (opp board) and
  CANTWIN (own board). Keyword reused by 14 primitive cards (Coalition Victory / Test of
  Endurance class) -- battle-tested, not a one-off.

VERDICT: the upkeep 40+-life win check is real. Campaign's FIRST alt-win-condition deck;
mechanism code-confirmed but UNEXERCISED in live play this corpus -- Felidar never resolved
to the battlefield (drawn vs93/vs137 but game ended before 6 mana + a safe window; vs148
the model BOTTOMED it on the mulligan to keep engine pieces), and life never exceeded 31.
Route a probe-deck validation to notes.md (N-122a) for one live confirmation.

MODEL: does NOT see or pursue the win line. No PLAN across 6 games mentions reaching 40 or
winning with Felidar. Life is reasoned about only as "drop to X, stay alive." The 40 line
must be TAUGHT as a first-class plan (guide) -- the model will not infer it.

## SURFACE 2 -- Sorin, Lord of Innistrad loyalty (renders + decisions)

Definition: planeswalkers.txt:3057. Loyalty 3; +1 create 1/1 black lifelink Vampire token;
-2 emblem "creatures get +1/+0" (`lord(creature|mybattlefield) 1/0` forever); -6 destroy up
to 3 creatures/planeswalkers and reanimate to your battlefield. All three oracle-faithful.

RENDERS: excellent. Loyalty asks show every legal ability with full card text, the
`[cost: Counters]` tag, and the target (seq32 vs139 offered "-6: destroy up to three and
reanimate ... targeting Arboreal Grazer"). Cast line: `Cast Sorin, Lord of Innistrad
{2}{b}{w}`.

DECISION QUALITY: strong. vs139 (WIN) -- Sorin cast t9, then +1 activated EVERY turn (t9,
t11, t13, t15), building a chain of 1/1 lifelink Vampires (lifegain triggers + Blood Artist
fodder + Pridemate growth). At t15 offered -6 targeting a 0/3 Arboreal Grazer and correctly
kept +1 (a pointless reanimation declined). vs137 -- same +1 chain; play was fine, deck was
out-raced. -2 emblem correctly ignored (weak here); -6 correctly gated on target quality.

VERDICT: loyalty class (partial at 146's Lolth) now well-validated for a build-loyalty value
walker. No engine or perception issue. POSITIVE result.

## SURFACE 3 -- lifegain-matters payoffs (primitives verified vs Oracle)

All decision-relevant primitives faithful:
- Serra Ascendant (102671): `this(controllerlife >29) 5/5` + `flying` -- +5/+5 and flying
  at 30+. Correct.
- Divinity of Pride (30466): `this(controllerlife > 24) 4/4` -- +4/+4 at 25+. Correct.
- Ajani's Pridemate (1983): `@lifeof(player) from(*[-lifefaker]|*):counter(1/1)` -- +1/+1
  counter on any lifegain. Correct.
- Blood Artist (11981): on ANY creature death, target player loses 1 / you gain 1. Correct
  (0/1; #HINT dontattackwith correct).
- Soul Warden gain 1 on any creature ETB; Suture Priest may-gain-1 (own ETB) / may-drain-1
  (opp ETB); Lone Missionary ETB gain 4; Kitchen Finks ETB gain 2 + persist; Kabira
  Crossroads ETB gain 2 (tapped land); Vampire Nighthawk flying/deathtouch/lifelink. All
  correct.

MODEL comprehension is GOOD: chains "Lone Missionary gain 4 triggers Suture Priest +1",
grows Pridemate, handles Suture Priest may-triggers ("Life"/"Life Loss" chosen correctly,
seq22/seq29). The gap is not the cards -- it is the LIFE-AS-WIN-AXIS thesis.

## SURFACE 4 -- load-bearing loss class: threshold-race (STRATEGY leak)

vs93 (LOSS, -11 t12) is the decomposable game and the key finding. Trace:
- t7 life 28: Serra Ascendants render `(1/1)` -- correct, below 30.
- t8 life 31 (seq14 blockers): both Serras render `(6/6) (printed 1/1) [flying, lifelink]`
  -- the engine APPLIES +5/+5 and flying, and the render shows LIVE effective P/T with a
  `(printed 1/1)` annotation. Divinity likewise renders `(8/8) (printed 4/4)` at 26 life
  (vs148). The perception surface is CORRECT.
- t9-t11: the model attacked with everything, dropping its own life 31 -> 25 -> 13, turning
  its 6/6 flyers back into 1/1s, then got alpha-struck by Relentless Rats (13 -> -11 on the
  crack-back). It raced with a board that only existed above 30, spending the very resource
  that powered it.

Per the wave-28 perception-vs-strategy rung: render reports the board correctly (6/6 at 31,
1/1 below) -> STRATEGY leak, route to the GUIDE. The model does not weigh the life
THRESHOLD; it treats life as a cushion to burn. Same failure leaves the deck unable to
stabilize vs aggro and never climbing toward Felidar 40. Guide's #1 teach must reframe life
as an ENGINE.

POSITIVE representation finding (record, no engine work): the `(printed X/Y)` live-P/T
annotation FULLY COVERS the conditional-static-boost class (Serra, Divinity) -- the model
CAN see the 6/6 / 8/8. That is why threshold play is a guide teach, not a notes.md
annotation request.

## Fallbacks / parser

Zero fallbacks in all 6 deck122 seats. No retracted/unparsed/deferred records, no parser
contradictions. Blocker/attacker bundles parsed clean (seq33 vs139 -> 6 attackers accepted).
