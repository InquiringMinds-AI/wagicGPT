# deck146 -> development notes (wave-29)

Corpus: matchups-20260726-115018, binary 50eeed8be. Seat: deck146, 3/6, 0 fallbacks.

## LEDGER: CLEAR / RETIRE

### N-146e "dungeon completion 309.5b probe" -> ANSWERED, RETIRE
Completion is now EXERCISED LIVE and clean. deck146 vs deck122: venture re-offered "CHOOSE A
DUNGEON" 3x (s10 initial + s24 + s37) = ~2 completions of Lost Mine. Engine board state at
t13 s54 confirms the payoff fired:
  - Nadaar, Selfless Paladin (5/5) (printed 3/3) [counters: 1x +1/+1]   <- base 3/3 + 1
    Storeroom counter (+1/+1) + completed-dungeon anthem (+1/+1) = 5/5
  - Triumphant Adventurer #1 (2/2) (printed 1/1) [deathtouch]           <- anthem alone
Game ran to t13 and was a WIN. No crash, no hang, anthem applied to the whole team. The probe's
open question (does the engine implement the 309.5b completion + does completion crash) is
resolved NEGATIVE for the crash and POSITIVE for the mechanic. Recommend dropping the probe from
the ledger and the wave-28 probe-deck plan (stack Adventurer+Nadaar+Lost Mine) is no longer
needed - the random pool now exercises it.

## LEDGER: CONFIRM (fixes validated at this seat)

### f2 dual-land oracle fix -> CONFIRMED CLEAN
0 mana-collapse lines across all 6 games (WB deck, grep {b}{b}/{w}{w} = 0). Single Shineshadow
Snarl renders "{b}{w} from 1 untapped source" (vs93 s6). The wave-28 collapse ("{b}{b} from 2
sources") is gone. Downstream: Vanishing Verse now fires ON CURVE in 5/6 games (was hoarded /
window-less in wave-28). Instant-speed priority windows restored (35 in vs139, 17 in vs122; 0
in the wave-28 losses). vs93 flipped from a wave-28 clean-kill LOSS to a wave-29 clean-kill WIN.

### f4 dungeon renders -> CONFIRMED CLEAN
Selection: full room path + completion reward + room count, framing "VENTURE ... NOT targeting a
permanent ... answer with its name" (fixes both wave-28 gaps: truncation + wrong TARGET framing).
Branch: {room effect} text present on every option. Navigation: Lost Mine 7/7, value-aware room
picks (goblin lair / dark pool / storeroom over scry). Option-1-Tomb bias broken.

## NEW ENGINE / RENDER ITEMS (low priority)

### N-146f (RENDER, low) -- no explicit "dungeons completed" status line
The CURRENT SITUATION render carries no "dungeons completed: N" / "completed-dungeon anthem
ACTIVE" line. The model can only INFER completion from anthem'd P/T and audibly second-guesses
it (deck122 s37 PLAN, verbatim: "the anthem applies *after* the dungeon is completed? 'As long
as you've completed a dungeon'."). The engine applies the anthem correctly regardless, so this
is cosmetic-to-mild: a one-line status ("You have completed 2 dungeon(s); your creatures get
+1/+1 from Nadaar") would remove the ambiguity and let the model attack/block with the boosted
stats confidently. Repro: any deck146 game that completes a dungeon (deck122 seq >= 24).

## GUIDE FACT CORRECTIONS APPLIED (strategy.txt this wave)
1. Removed "Lost Mine completes in 3 rooms" / "Tomb is slowest" (render: Lost Mine 7, Tomb 5,
   Mad Mage 9) -> reframed pick around value (all-upside rooms + clean completion vs Tomb's
   self-damage every room).
2. Removed "branches show only the room NAME with no text" (now false; they carry {room effect})
   -> new line teaching the model to choose ON the effect text.
3. Removed all OPTION-NUMBER references ("option 2", "option 1") -> answer-by-NAME (ordering is
   not fixed: Lost Mine was option 2 at first selection, option 3 at re-selections).
4. Strengthened the walker line to CAST-when-affordable (both unexercised 2 corpora running).

## CARRY (unexercised, WATCH)

### Planeswalker / Kaya decision quality -- UNEXERCISED across TWO corpora
Lolth + Kaya: 0 casts this corpus (both). Kaya drawn to hand vs93/vs137, never cast. Their
loyalty-menu decision quality is UNVALIDATED (wave-28 exercised Lolth once, Kaya never). This is
deck146's one remaining unproven surface. If it stays unexercised NEXT corpus too, consider a
probe deck (stack Kaya + Lolth + enough mana) to validate the loyalty menu renders + the model's
+/- discipline before certifying the deck clean. This is the strongest single reason to keep
deck146 in rotation.

## ROTATION PROJECTION (per the pipeline pattern)
KEEP deck146 for wave-30. Reasoning: (a) it just cleared its blocking engine gaps (no-cast-window
+ dungeon renders + completion) and needs one guided corpus to confirm the strategy.txt fact
corrections landed; (b) its UNIQUE teaching value - dungeon navigation, venture value picks, and
completion payoffs - is now well-rendered and worth keeping in the pool as the reference
campaign-first seat; (c) planeswalker/Kaya play remains unexercised across two corpora, so it is
NOT near-terminal (unlike a deck whose surfaces are all validated + guide frozen). Named exit
path: if wave-30 exercises the walkers with acceptable decision quality AND the guide freezes
byte-identical, deck146 becomes a rotation candidate. Not before.
