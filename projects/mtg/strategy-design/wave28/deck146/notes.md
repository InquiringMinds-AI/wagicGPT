# deck146 dev notes (wave 28) -- engine / harness / render items with seq repros
Corpus matchups-20260726-050449, binary ad8930961. Step-0 seat -- items never rotate.

## N-146a [HIGH, engine/harness -- the headline] -- NO CAST/PRIORITY WINDOW WHEN ON THE BACK FOOT
In both blowout losses the model cast NOTHING all game -- it received ONLY land-drop asks and
zero cast/priority windows, while holding castable threats and instant-speed removal and dying.
REPRO (deck18, file 1785068327-ai_baka_deck146-...-vs-deck18.jsonl -- 7 records total: 1
gamestart + 5 ask + 1 gameend; ZERO priority, ZERO cast asks):
  - seq3, own Main phase 1, turn 3, life 19. Hand: Triumphant Adventurer {B}{W} (1/1) + 4x
    Vanishing Verse (instant). Opponent board: all MONO-WHITE creatures (Goldmeadow Dodger {w},
    Cenn's Heir {1}{w}) = every one a legal Vanishing Verse target. The ask is titled "Land drop:
    which land do you play now, if any?" and offers ONLY {Play Hive / Play Swamp / Play no land}.
    The model's own PLAN reads "Play Shineshadow Snarl, then cast Triumphant Adventurer." It was
    never given the cast window. seq4 (turn 5) and seq5 (turn 7) are again land-only. Life 20 ->
    19 -> 14 -> 6 -> dead t8 with 4 Vanishing Verse and a deathtouch blocker in hand.
  - Same pattern deck93 (file ...-vs-deck93.jsonl, 6 records): turns 1/3/5 land-only asks; died
    t10 having cast nothing.
HYPOTHESIS (must be reproduced on the CURRENT binary before any fix -- do not code from logs):
the land-drop ask fires while only 1 land is untapped (legalCasts empty -> land-only menu); after
the land resolves, FindCardToPlay/computeActions appears NOT to be re-invoked to offer the now-
affordable casts in the same main phase, and the turn advances. Contrast: deck134 (50 records) /
deck136 (42 records) surface many cast asks -- starvation correlates with short/back-foot games.
This is the single largest STEP-0 result and is UNFIXABLE by guide prose. Suggested repro: pin a
selfplay game where deck146 is on the draw vs an aggro deck with a castable 2-drop + instant in
the opening hand; confirm from the translog whether a cast ask ever follows a land-drop ask in the
same main phase. If confirmed, the fix is in the c5c FindCardToPlay/main-phase re-entry seam.
(NOTE: partial mana-color constraints exist too -- see N-146d -- but the deck18 case had a valid
{W}{B} line and legal targets and STILL got no window, so the window gap is the dominant factor.)

## N-146b [MODERATE, engine/render] -- DUNGEON ROOM-BRANCH menu lacks effect text
When advancing WITHIN a dungeon (choosing the next room), the option menu shows ROOM NAMES ONLY,
no effect text. REPRO deck134 seq36: "A choice is required - choose an option: 1. veils of fear /
2. oubliette" -- the model must recall from memory that veils-of-fear = each player loses 2 or
discards vs oubliette = discard + sac creature/artifact/land. This is a DECIDING-FACT-RIDES-THE-
OPTION gap (standing P1/P4) at the venture seam. The dungeon SELECTION ask (deck134 seq25) DOES
show room text (truncated), and the room EFFECT modal (seq38: "lose 2 life / discard a card")
renders the effect -- only the BRANCH menu is bare. FIX: annotate each branch option with its
room effect text (the dungeon card's room definitions are available -- they are shown truncated in
selection and full in the effect modal). Engine/render lane.

## N-146c [MODERATE, render] -- DUNGEON SELECTION room text TRUNCATED + mis-framed as targeting
deck134 seq25: the first-venture dungeon SELECTION renders as "TARGET CHOICE for <source> ... pick
the ONE target it will affect" and shows each dungeon's room text truncated at ~1-2 rooms ("...").
Two issues: (a) the model cannot see deeper/completion rooms, so it chooses a dungeon on partial
info (it cannot see Lost Mine completes in 3 rooms and ends in a draw vs Tomb's 4 / Mad Mage's 8);
(b) framing a dungeon SELECTION as "pick the ONE target it will affect" is misleading (it is
choosing which dungeon to venture into, not targeting a permanent). Model coped, but a dedicated
"venture: choose a dungeon" framing + a compact room-count / completion-reward summary would help
value-aware picks. Lower priority than N-146b (the branch menu). Render lane.

## N-146d [LOW-MODERATE, render] -- "Mana available:" summary collapses a dual land to one color
deck93 seq4: "Mana available: {b}{b} from 2 untapped sources" when one of the two sources is
Shineshadow Snarl (taps for {W} OR {B}). The HAND render is correct ("Shineshadow Snarl (land:
taps for {W}{B})") but the running mana-available SUMMARY under-reports Snarl's second color,
which could make the model believe it cannot pay a {W} pip it can actually pay. Verify the mana-
summary aggregation treats a dual/any-color source as contributing both colors (or renders it as
{W/B}). Render lane.

## N-146e [WATCH, engine -- pre-proven ledger 'dungeon completion 309.5b'] -- UNEXERCISED live
No dungeon completed in any game this corpus (max depth = Tomb room 2). The 309.5b completion
defect therefore did NOT bite live -- but completion is LOAD-BEARING for this deck (Nadaar's team
+1/+1 anthem and Barrowin's reanimation both key off "you've completed a dungeon"). Recommend a
PROBE deck (stack Triumphant Adventurer + Nadaar + only Lost Mine of Phandelver as the #DNG,
against a passive opponent), venture 3x, and confirm from the translog + board that: the dungeon
COMPLETES, it leaves the command zone / is marked complete, and Nadaar's anthem turns the team to
+1/+1. If completion silently fails to register the payoff, the 309.5b ledger item is confirmed.

## Carry / clean
- retracted_choice deck134 seq40 = menu-drift stale-answer drop, self-healed at seq41 (sanctioned
  ~2% path). NOT convoke, NOT a hard fallback. No action. 1/149 own-seat decisions.
- Kaya the Inexorable loyalty menu UNEXERCISED (drawn 3x, cast 0x) -- render/decision unvalidated,
  carry forward.
- Land tags at a two-color seat: clean, no prompt-noise. No shocklands in this deck (e4 shockland
  ETB question N/A here). Snarl's "enters tapped unless reveal Plains/Swamp" ETB condition is not
  surfaced in the tag (engine auto-handles; model can't plan around a tapped Snarl) -- LOW.
