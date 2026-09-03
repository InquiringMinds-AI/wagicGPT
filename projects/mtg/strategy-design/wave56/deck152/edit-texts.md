# deck152 - wave-56 guide edits (before -> after)

Base: live `bin/Res/ai/baka/deck152_strategy.txt` (64,112 B). Result:
`wave56/deck152/strategy.txt`, 65,261 B (+1,149). Two edits.

## 152-A - the "FLIP SIDE" Pathway block: the other face now has its OWN row

FINDING. Lane D's D8 gave every Pathway back face a direct row on the same land menu.
`152v123` seq 24 is the whole mechanism in one window - the ask reads

```
Choose an option for Branchloft Pathway:
1. Play Land
2. Flip Side [DISPLAY TOGGLE only - this is a modal double-faced land. ... you do not need it ...]
3. boulderloft pathway [PLAY THIS AS A LAND: puts "Boulderloft Pathway" onto the battlefield as a
   land (taps for {W}). It costs no mana and uses no stack, and it USES YOUR LAND DROP for this turn.]
4. Decline - do nothing
```

The seat answered `CHOICE: 3` and the log reads `You played Boulderloft Pathway` /
`Paid {4}{g}{g} for Tovolar's Huntmaster with Branchloft Pathway; Barkchannel Pathway; Hengegate
Pathway; Plains; Boulderloft Pathway; Overgrown Farmland` - a six-mana creature cast on the same
turn off the sixth land. The guide's old "take Flip Side FIRST - once, then Play Land" recipe now
costs an extra decision and buys nothing: **`Flip Side` rendered 109 rows in 90 windows corpus-wide
and was taken 0 times**, which is correct behaviour the guide was arguing against.

BEFORE
```
  Default: play the shown face and move on. Take "Flip Side" FIRST - once, then "Play Land" -
  only when the OTHER face's colour is one your stuck hand needs and the shown face's is not
  (green cards stranded with a white face showing: flip to the {G} face; Teferi wanting {U}
  with Hengegate showing: flip to Mistgate). One flip, then play; never flip anything twice.
```
AFTER - both faces are rows; read the colour in each row and take the face your hand is short of;
`NEVER TAKE "Flip Side" ON A LAND MENU` (display only, its own row says so). Full text in
`strategy.txt`. The three face pairs and their colours are unchanged and were re-read off the
rendered rows (`taps for {W}` / `{G}` / `{U}`) and off `borderline.txt:13565-13567`
(Branchloft Pathway `auto={T}:add{G}`, back face Boulderloft Pathway).

## 152-B - the HOLD block gets the anti-anchor half

FINDING. deck152's HOLD take rate rose 16 of 358 (4.5%) -> **24 of 143 (16.8%)**, but at the
declined-note N>=2 it is **2 of 35**, and **32 of the other 33 answered `Cast nothing right now`**
- the row two places above HOLD on the same menu. Concentrated in `152v125` (gameend
`hold_windows_skipped: 72`) and `152v162` (16).

BEFORE - the block ended at "...the row exists to make that answer stick."
AFTER - adds: `"Cast nothing right now" IS THE ANSWER FOR A LIST YOU HAVE NOT DECLINED YET.`
plus the explicit anti-anchor instruction (read the N line first, then pick the row - do not let
the HOLD row's position at the bottom decide it) and the 35/33 count as its WHAT THIS COST.

---
## What did NOT change and why
- **P4 (152-A, the trust rule) PASSES 0/N**: 6 partial attack windows, no reply asserted a printed
  A-line was tapped or unable to attack. All 6 holds are `152v126`'s Sanguine Bond + Exquisite
  Blood windows, where every held A-line carried the new
  `(their life LOOP is in play: any life they gain or you lose in this combat chains without limit
  - fatal to you ...)` clause. The seat read it and refused; the render is doing the work.
- **P5 (152-B, the counterspell lane) does not recur.** Every creature cast this corpus was made
  with the new `Their untapped sources: N` line reading 0-3; there is no cast at 5+. `152v125` was
  won 20/-1 on T15 (wave 55: the same matchup lost -11/18 on T65). No edit earned.
- The mulligan floor stays: 6 keeps, 0 mulligans, 0 colour-family renders.
