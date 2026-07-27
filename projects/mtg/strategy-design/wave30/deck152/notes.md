# deck152 wave-30 -- engine / representation ledger items (Step-0 never rotates)

## N-152a (NEW) -- MDFC Pathway play-LAND menu offers only the FRONT face; blue near-inaccessible

The play-land menu for a modal double-faced Pathway lists only the FRONT-face color:
- vs136 seq17 options_text: "1. Play Hengegate Pathway / 2. Play Forest / 3. Play no land right now"
- Other Pathway land plays: "Play Barkchannel Pathway", "Play Branchloft Pathway" -- always the
  front (G/W) name; the back color face (Mistgate {U}, Tidechannel {U}, Boulderloft {W}) is NEVER
  offered as a land play.
To play a Pathway's non-default color you must first use the separate "Flip Side" DISPLAY TOGGLE
(a no-op cosmetic switch), whose annotation says "you usually do NOT need it: the Cast menu ...
lists every face you can afford" -- but that is about the CAST menu for MDFC SPELLS, and is
misleading for a LAND, where you play via the play-land menu and the blue side is otherwise hidden.
CONSEQUENCE: blue was near-inaccessible; Teferi ({2}{W}{U}, the deck's only blue card) resolved
0/6 across the corpus. Deserted Beach ({W}/{U}, no flip) is the only clean blue source.
FIX: the play-land menu for an MDFC land should list BOTH faces with their colors, e.g.
"Play Barkchannel Pathway (taps {G})" and "Play Tidechannel Pathway (taps {U})", so the pilot can
choose the color side at play time without a display toggle. (Guide stopgap: lead on Deserted
Beach or flip a Pathway before playing when you want Teferi; removal condition = both faces offered
in the play-land menu.)

## N-152b (NEW, benign) -- bare-priority window surfaces only the cosmetic Flip-Side toggle

vs136 seq27: kind=priority, Main phase 1, your turn, empty stack, options=1, the ONE option being
"Flip Side with Barkchannel Pathway -> DISPLAY TOGGLE only ... casts nothing". The real casts are
offered at the adjacent kind=ask main-phase points at the SAME mana ({w}x2): Fateful Absence was
offered at seq26 and seq28. So this is NOT an offer-gap; the correct answer is 0 (pass). But
offering ONLY a useless display toggle as a "legal action" invites the model to fabricate a phantom
play (here it hallucinated "CHOICE: 2 (Cast Briarbridge Tracker)" -> the one unparsed fallback).
SUGGESTION: at a bare-priority pass with no real action, suppress the lone Flip-Side toggle (or
render the window as "nothing to do this priority -- pass (0)"), so the option list never presents
a cosmetic-only action that reads as a play. Low priority; the fallback safety-nets it.

## Confirmed-CLEAN this corpus (no action -- recorded so a later reviewer does not re-open)

- Daybound werewolf DAY-side render coherent: name/mana/(P/T)/[daybound] tag + a "Day" designation
  pseudo-permanent on the battlefield; creature count excludes it (vs137 seq17). No isFlipped
  desync -- but note the day->night FLIP itself never triggered (no spell-less turn), so the
  flip-thrash fix's live-flip steady state is UNVALIDATED at this seat; it needs a slower/controlled
  matchup (or a probe) to exercise a real flip.
- Brutal Cathar exile-until-leaves fires and reverses correctly (vs137 seq17 exile Edgewall
  Innkeeper -> seq27 Cathar dies -> Innkeeper returns).
- Ranger Class level-up costs render correctly ({1}{g} L2, {3}{g} L3); level shown as "[counters:
  Nx level]".
- Dual-land / "Mana available" rendering accurate under 3 colors (color set + source count; duals
  show "(land: taps for {W}{G})" etc.). The one mana-related fallback was model perception, not the
  render (see findings fallback decomposition + N-152b).
- Tapped-tag reword "[tapped - cannot attack or block this turn]" reads cleanly; no misread.
