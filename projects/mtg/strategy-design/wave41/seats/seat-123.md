# Wave-41 seat report: deck123 "Intruders Of Thraben" (harvested 2026-08-24)

[Seat-returned report, orchestrator-filed. Seating verified vs gameend; 276 decisions,
1 fallback (empty_reply, deck146 seq 2, t1 land choice). 1/6 — NOT an obedience failure:
the guide is executed near-perfectly and is losing.]

## Repeat-annotation coverage
Emitter: kRepeatActivationFloor=2; "you control M creatures" clause only when
makesCreatureToken(). Token-maker activation lines rendered: 64; repeat-eligible (>=2): 42;
annotated 42/42 = 100%; M-clause 42/42 = 100%. Sequencing audit clean (10 chains, +1 per
offer, M tracks board). COVERAGE HOLE: first two activations of any instance per turn render
NO M at all — cite deck130 t12 seq 41/44/47 (no tail while controlling 13-15 creatures;
M=16 only at seq 49); Doomsayer offered 4x same turn, never carried M. Guide stop rule
literally unevaluable there.

## Stop-rule obedience: 42/42 = 100%
Annotated M>=12: 10/10 passed. Annotated M<12: 32/32 took. Four passes on UNANNOTATED lines
at 13-16 creatures were unwritten judgment (right play, unreachable rule). All other rules
obeyed: fetch 6/6, Damnation 2/2, Tragic Slip 2/2 (correct Morbid read), edicts 3/3,
Greaves 3/3, Idyllic Tutor 2/2 -> Intruder Alarm, mulligan 6/6.

## Decision failures (guide content, with cites)
- F1 cast-list ranks Intruder Alarm above token-makers; Alarm alone does nothing
  (deck162 seq 11: cast Alarm over Doomsayer, no creature ever hit board, dead t12).
- F2 no blocking policy for the token-maker; got it backwards twice (deck152 seq 7: blocked
  at 18 life, lost engine; seq 19: declined at 5 life, died t13).
- F3 Rule 5 edict fires with no value gate (deck126 seq 6/7: Devour Flesh on a 0/4 defender,
  handed a lifegain deck 4 life t3; opp finished 44).
- F4 summoning sickness absent from guide; "attack with all 12 for 49" plans on
  fresh armies (deck126 seq 30 — 1 legal attacker offered; also deck130 seq 38, deck125
  seq 58; render IS truthful: "[summoning sick - cannot attack this turn, but CAN block]").
- F5 Vision Skeins autopilot into a draw-punish deck (deck162 seq 4/6 with Howling Mine
  visible — guide warning scoped only to damage-for-draw permanents).
- F6 whole chain fired post-combat into open mana; Supreme Verdict ate 14 creatures that
  never attacked (deck125 seq 71-82 -> seq 88). MEDIUM confidence — honest null: no
  demonstrably winning alternate line (drew no creature for 17 turns after).

## Render ledger item (core prompt, not guide)
Collapse byte-identical token battlefield entries to one ranged line — measured 1,619/19,750
chars (8%) of deck126 seq 28 prompt; largest deck123 prompt 22.2KB.

## Recommended edits: SEVEN, exact texts in seat transcript & synthesis
1. Stop rule reads M from annotation OR battlefield "of which M are creatures" line
   (closes the coverage hole).
2. Cast list: makers above Alarm; Alarm gated on maker-on-battlefield.
3. Blocking ladder for token-makers (lethal-proximity threshold life-4).
4. Summoning-sickness paragraph (count only non-sick creatures in attack plans).
5. Edict value gate (no edicts on defenders/power<=1; skip vs lifegain-reward decks).
6. Vision Skeins: dig-not-default; never into draw-punish/extra-draw boards.
7. Fire the chain at the FIRST window, never post-combat Main 2 (medium confidence).

## Falsifiable predictions (next corpus)
1. Stop-rule coverage 100% of DECISIONS (incl. unannotated lines) — falsified by any take
   at battlefield >=12 creatures.
2. Alarm-before-maker vanishes (deck162 seq 11 repro shape).
3. Makers block at unblocked-damage >= life-4, hold above.
4. Zero attacker ids in replies absent from options (baseline 1 of 4 attackers records).
5. Zero edicts on power<=1/defender sole creatures (baseline 1 of 3).
6. Win rate NOT predicted to move (1-2/6 expected); if behaviors correct and rate flat,
   next lever is DECK CONSTRUCTION (8 makers / 8 Alarms / 24 lands / 17 removal shell),
   not the guide.

---
## VERBATIM EDIT TEXTS (deploy-ready)

EDIT 1 — RULE 1 stop rule, replace the "[repeat: ...]"-keyed sentence with:
Take it. Then take it again the next time it appears, and again, until you control 12 or more
creatures - then answer 0 (pass).
Read your creature count in this order and use the FIRST one that is on the screen:
  1. the option line's own note, "[repeat: ... you control M creatures]" - M is your count.
  2. no such note on the line? Then read your battlefield line: "Your battlefield (K
     permanents listed, of which M are creatures)" - that M is your count, always printed,
     every window.
The note is only printed from the third activation of a card onward, so the first two
activations of every card, every turn, have no note - the battlefield line is your count there.

EDIT 2 — cast list entries 1-3, replace with:
 1. "Cast Bloodline Keeper {2}{b}{b}" - when no token-maker is on your battlefield.
 2. "Cast Thraben Doomsayer {1}{w}{w}" - when no token-maker is on your battlefield.
 3. "Cast Intruder Alarm {2}{u}" - when no Intruder Alarm is on your battlefield.
    Intruder Alarm on an empty battlefield does NOTHING. A token-maker with no Alarm still
    makes one token a turn. The maker goes first, every time.

EDIT 3 — COMBAT blocking, replace the single tokens-first clause with:
Blocking, in this order:
 - Block with your Human and Vampire TOKENS first - they are free and you make more every turn.
 - Add up the damage of every attacker with no blocker on it. If that total is your life total
   minus 4 or more, block with EVERYTHING that can block, Thraben Doomsayer and Bloodline
   Keeper included. A token-maker you keep alive at 1 life makes tokens for a game you already
   lost.
 - Otherwise your token-makers do NOT block. Take the damage. A blocker line reading "(both
   die)" on Thraben Doomsayer or Bloodline Keeper is a trade you decline while your life is
   comfortable - the maker is worth more than any one attacker.

EDIT 4 — new paragraph at end of RULE 1:
A token you make this turn CANNOT ATTACK this turn. Its battlefield line says so:
"[summoning sick - cannot attack this turn, but CAN block]". It can block right away, and it
attacks on your NEXT turn.
So when you plan, count only the creatures whose lines do NOT say "summoning sick". A plan that
says "attack with all 12 for lethal" is wrong on the turn you made those 12 - the attack list
will offer you one creature. Make the tokens anyway; they block this turn and they swing next
turn.

EDIT 5 — RULE 5, insert before "Both cards let the SACRIFICING player pick":
Do not spend an edict on a creature that is not hurting you. If their only creature is a
defender, or has power 0 or 1, or its line says it cannot attack, keep the edict and take
another action - you paid a card and handed them life equal to its toughness to remove
something that was never going to kill you.
Both edicts give life to the player who sacrifices. If the OPPONENT'S deck list contains a card
that rewards them for gaining life, "Cast nothing right now" beats either edict at the
opponent.

EDIT 6 — KEY CARDS, Vision Skeins, replace "Cast it when you want cards." with:
Cast it only when you have no token-maker and no Intruder Alarm you could cast this window -
it is your dig, not your default. They draw two as well, so it is the last card you cast, never
the first.
When the OPPONENT'S battlefield holds any card that draws them extra cards or punishes card
draw - Howling Mine and Liliana's Caress on deck162's board both count - do not cast Vision
Skeins at all.

EDIT 7 — RULE 1, replace "Each token is a free body. There is no cost to weigh." with:
One exception to "there is no cost": do not fire the whole chain in your own Main phase 2,
after combat has already happened. Tokens made there cannot block until the opponent's turn and
cannot attack until the turn after that - a board wipe in between costs you every one of them
for zero damage. Fire the chain at the FIRST window of the turn you get it.
