# deck137 — Selesnya (GW) Adventures — Step-0 GUIDELESS baseline review (wave 23)

Binary 0e6361732. deck137 played this corpus with NO guide (core prompt only) and NO prior
review. This is its INITIAL guide pass (Step 0-bis) + the wave-23 adventure/modal-face stress seat.
My 6 pilot logs: 1784*-ai_baka_deck137-0x*-vs-{131,109,102,133,140,27}.jsonl.

## Deck read (Step 1) — engine-derived, not Oracle memory

GW go-wide beatdown built on the ADVENTURE package + lifelink-Soldier tokens + anthems.
Engine card facts (from bin/Res/sets/primitives/*.txt; costs/stats/mechanism verified):
- Edgewall Innkeeper {G} 1/1 — auto=@movedTo(creature[adventure]|myStack):draw:1 — draw a
  card whenever you cast a creature spell that HAS an adventure. THE card-advantage engine.
- Adventure five (each = one card with two castable faces):
  - Faerie Guidemother {W} 1/1 flying // Gift of the Fae {1}{W} = target creature +2/+1 & flying ueot
  - Giant Killer {W} 1/2 ({1}{W}{T}: tap a creature) // Chop Down {2}{W} = destroy creature power>=4
  - Lovestruck Beast {2}{G} 5/5, cantattack unless you control a creature with power=1 AND one with
    toughness=1 (a 1/1 satisfies both; a 1/2 does NOT) // Heart's Desire {G} = make a 1/1 white Human
  - Shepherd of the Flock {1}{W} 3/1 // Usher to Safety {W} = return a permanent YOU control to hand
  - Flaxen Intruder {G} 1/2 (combat-dmg -> may sac -> destroy artifact/enchantment) // Welcome Home
    {5}{G}{G} = three 2/2 Bears
- Convoke payoffs: Venerated Loxodon {4}{W} convoke = 4/4, ETB puts +1/+1 on each creature that
  convoked (tapped, non-attacking); March of the Multitudes {G}{W}{W}{X} convoke = X 1/1 lifelink Soldiers.
- Token/anthem: Emmara {G}{W} 2/2 (becomes-tapped -> 1/1 lifelink Soldier); Trostani Discordant
  {3}{G}{W} (+1/+1 anthem, ETB two 1/1 lifelink Soldiers); Castle Ardenvale ({2}{W}{W}{T}: 1/1 Human);
  Unbreakable Formation {2}{W} instant (indestructible ueot; main-phase addendum: +1/+1 counter + vigilance);
  Flower {GW} sorcery (search basic Forest/Plains) // Flourish {4}{G}{W} (all your creatures +2/+2 ueot);
  Once Upon a Time {1}{G}, free as first spell (look top 5, take a creature/land).
- Clock ~turn 6-8. Manabase 8 Forest / 8 Plains / 4 Temple Garden / 1 Castle Ardenvale — healthy GW.

## Outcomes (2/6, matches brief)

- vs131 WIN (kill, opp -2 / me 27, t12); vs140 WIN (adjudicated ahead, 16/12, t13).
- vs109 LOSS (raced by mono-R aggro, dead t8); vs102, vs133, vs27 LOSSES (adjudicated behind:
  9/15, 6/25, 1/14). The losing pattern is TEMPO: durdled on setup (Once Upon a Time, Flower,
  lands) and under-attacked while an aggressive opponent got ahead. This is a go-wide deck that
  is not pressing its beatdown fast enough — the guide targets the attack floor + the tax that
  slows its turns.

## STRESS ITEM — adventure / modal-face surfacing verdict

Both faces surface cleanly as two separate, correctly-costed cast options. For every
affordable adventure card the option list carries:
- "Cast NAME // ADV {base cost} (P/T)"  = the CREATURE face, and
- "Cast NAME // ADV with its adventure cost {adv cost} (P/T)" = the ADVENTURE SPELL face.
Costs are engine-correct (Lovestruck creature {2}{g} vs Heart's Desire {g}; Giant Killer {w} vs
Chop Down {2}{w}; etc.). When a face is unaffordable it is correctly omitted (Welcome Home {5}{g}{g}
and Flaxen's creature-only offers; Lovestruck adventure-only at 1 mana). The target sub-menu for an
adventure spell renders properly ("TARGET CHOICE for Shepherd // Usher to Safety" listing the
bounce targets, deck131 s16). No Tergrid-style flip thrash, no dead-ends, no unparsed loops at the
adventure seam. The model navigates the two faces and mostly picks correctly; the answer-LAST
protocol even handled a mid-reply revision (deck27 s19: opened "CHOICE: 3", reasoned through
Lovestruck's can't-attack rule, correctly revised to "CHOICE: 4" = cast Heart's Desire to make a 1/1
first; parser took the last CHOICE). Adventures do NOT use the R-DFC-FLIP / DISPLAY TOGGLE
annotation family at all — those fired on 0 of deck137's 197 records; adventures use the dual-cast-
option surface instead (see notes.md).

THE ONE DEFECT (representation, ledger): the adventure-cost option prints the CREATURE's (P/T)
and carries NO effect text. "... with its adventure cost {g} (5/5)" reads as "pay {G}, get a 5/5,"
but casting it does the adventure spell (make a 1/1) and exiles the card. Two costs observed:
1. A CONFIRMED MISFIRE (not just tax): deck131 s15. Only {W} available; options were "1. Cast
   Shepherd of the Flock // Usher to Safety with its adventure cost {w} (3/1)" and "2. Cast nothing".
   The model's reply reasoned "Shepherd of the Flock is a 3/1 creature ... develops my board, adding
   another body" and chose option 1 — believing the (3/1) meant it was deploying a 3/1. It actually
   cast Usher to Safety (return a permanent you control to hand); the game log confirms Shepherd ->
   graveyard -> exile, then at s16 it was forced to pick a bounce target and returned its OWN Faerie
   Guidemother (a deployed 1/1 flyer) to hand. Net: a self-bounce tempo loss driven purely by the
   mislabel. (Non-fatal — deck131 was weak — but a clear wrong outcome.)
2. Reasoning tax: deck27 s19 (9,315 chars). The model reached the right play but had to talk
   itself past the label ("The (5/5) refers to the creature side ... The option text is slightly
   confusing but clearly indicates casting the adventure").
Across 6 games the model cast the adventure SPELL face only twice (deck131 s15 Usher — the misfire;
deck27 s19 Heart's Desire — correct, sophisticated). It overwhelmingly and correctly defaults to the
CREATURE face. So the guide's job here is Step-0-bis rule 3 (kill the misleading belief) + rule 2
(short-circuit the tax): a capability table naming what each face does + the "(P/T) is the creature
you get LATER, not now" fact. Durable fix is engine-side (notes.md).

## Reasoning-tax map (Step 0-bis rule 2 — ranks the teaching points)

deck137 owns the CORPUS's single longest reply (deck27 s21, 15,901 chars). Top sinks, ranked:
1. ATTACKERS seam (15,901 / 15,334 / 13,051). Two shapes: (a) genuine PASSIVITY — deck27 s21
   declined a free 2-damage Flaxen Intruder (1/2) swing into an all-tapped board and spiraled 15.9k
   chars to "ATTACK: none"; (b) a verbatim REPETITION LOOP — deck102 s32 looped the exact sentence
   "maybe I should attack because I want to force the opponent to block and then... no, nothing
   happens. So ATTACK: none. Wait..." for 15.3k chars (outcome still correct: parser took the head
   "ATTACK: A1", attacked). -> the attack floor + commit rule is the #1 behavioral lever.
2. CONVOKE X-cost cards (March/Loxodon: 13,305 / 13,028 / 12,129). The model spirals on whether
   it has mana, whether to convoke, tapping already-tapped lands, and X selection. -> the convoke
   "you can afford it, the engine taps for you, don't recompute" rule.
3. Adventure two-face (P/T) mislabel (9,315 at deck27 s19) — the capability table pre-answers it.
4. Stale-log-tail / Flower contradictions (14,739 at deck140 s13) — the model saw "Flower: hand
   -> stack" in the log tail vs Flower in current hand and declared "a contradiction in the prompt
   data." Representation (stale log tail), not adventure-specific; low deck-priority.

## Fallback classification (3/3 — brief says 137:3, all deferred_to_heuristic)

All three of deck137's fallbacks are fallback=deferred_to_heuristic on kind=defer records with
empty prompt, empty reply, options=0, latency_ms=-1 — i.e. the LLM was never called. These are
the CONVOKE tap-payment sub-decisions (which creatures to tap to pay convoke), which the harness
routes to the baka heuristic BY DESIGN:
- deck131 s21 + s23: convoke tap for Venerated Loxodon (chosen_text "Venerated Loxodon", t6).
- deck102 s19: convoke tap for March of the Multitudes (chosen_text "March of the Multitudes", t7).
None are unparsed_reply — the corpus's 4 NEW post-INDEX-WINS unparsed_reply shapes are NOT at
this seat (they belong to other decks; the parser owner should look elsewhere). None are
empty_reply or retracted_choice. Board impact: nil — deferring the convoke-creature-tap choice to
the heuristic is mechanically harmless. Note for the corpus: deck137's convoke cards appear to be
the SOLE source of the corpus's 3 deferred_to_heuristic fallbacks.

## Pilot verdict (e) — repetition_penalty=1.05, per-seat

SAME to slightly WORSE at this seat. Evidence: (1) deck137 holds the corpus MAX reply length
(15,901 chars, deck27 s21) — the penalty did not tame this seat's worst spiral; (2) a genuine
VERBATIM repetition loop still occurred under the penalty (deck102 s32 repeated an identical ~20-word
sentence many times for 15.3k chars) — repetition_penalty=1.05 is too weak to break a structured
decode loop at the attackers seam; (3) prose QUALITY is otherwise intact — the reasoning is coherent
and correct (the adventure mechanic, Lovestruck's restriction, protection rules all reasoned
soundly), no degradation or word-salad from the penalty. So: no prose harm, but no measurable spiral
relief at the seat that most needs it. The spiral driver here is the ATTACKERS decision under a
single-small-attacker / evasive-opponent board — a guide/representation problem, not one a 1.05
penalty fixes. Recommend the decode-side guard stay paired with the guide's commit rule.
