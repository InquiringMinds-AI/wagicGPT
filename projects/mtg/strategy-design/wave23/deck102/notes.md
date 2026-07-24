# deck102 dev notes - wave 23 (engine/harness/representation ledger + rotation verdict)

## MILESTONE this corpus: the Tergrid keystone finally exercised end-to-end (0/18 -> cast -> steal fired).
vs27 (binary 0e6361732): Tergrid drawn t4-ish, CAST t9 (seq34), attacked/blocked, steal seam fired t11
(seq38-39), model WON (opp -> 0, t13). First time in project history the deck's namesake engine reached the
battlefield in a real game. Details below.

## CLOSED / VALIDATED this corpus

### R-DFC-FLIP (wave-22 HIGH) -> flip-thrash CLOSED / VALIDATED by batch (b).
The DISPLAY-TOGGLE annotation + castable-faces + 2-flip cap shipped in binary 0e6361732 works. Flip-Side
pseudo-action consumed exactly ONCE all corpus (vs27 seq7, t1) vs wave-22's 11. The annotation text renders
in full ("DISPLAY TOGGLE only... It does NOT cast anything and uses no stack... the Cast menu is where you
cast, and it lists every face you can afford"). After one early toggle the model used the Cast menu and
cast Tergrid. The wave-22 phantom-"Activate Tergrid's Lantern" invented-name stale_echo (s42) did NOT
recur (0 fallbacks at this seat). Recommend closing the flip-thrash half of R-DFC-FLIP in the master
ledger. Guide crutch DEMOTED (strategy edit 1).

### R-PAINLAND: still CLOSED / HELD.
Ancient Tomb drawn vs131; CAUTION line + "[tapping for mana deals 2 damage to its controller]" tag both
render (sampled seq19); model priced the Tomb damage in its Smallpox reasoning; no self-death. Durable.

### STEAL SEAM: EXERCISED and VALIDATED (render + parse clean).
vs27 seq38-39: model cast Liliana's Triumph with Tergrid on the battlefield; opponent sacrificed Oona's
Gatewarden; Tergrid's trigger rendered "Choose an option for Oona's Gatewarden: 1. put on battlefield /
2. don't put on battlefield"; model answered CHOICE: 2 (don't put on battlefield) coherently, no fallback.
The steal seam WORKS mechanically. (Decision was a decline of a free 2/1 flying defender while on offense —
defensible but usually you take it; addressed by guide strategy edit 1, not an engine item.) Recommend
marking the Tergrid steal seam VALIDATED in the master ledger.

## NEW representation / engine item

### R-DFC-BACKFACE-RESOLVES-FRONT (MEDIUM - correctness; benefit-side in play, but rules-wrong).
Casting the modal-DFC BACK face ("Cast Tergrid's Lantern {3}{b}", the {3}{B} Legendary Artifact option in
the Cast menu) deploys the FRONT face permanent instead ("Tergrid, God of Fright", the {3}{B}{B} 4/4
creature). Repro: vs27 seq34, turn 9, mana available exactly {b}{b}{b}{b} (=4; the God's 5 was unaffordable,
so the model chose the affordable Lantern face). Narration sequence:
  `Choose an option for Tergrid's Lantern: -> Cast Card Normally`
  `Your Tergrid's Lantern: hand -> stack`
  `Your Tergrid, God of Fright: stack -> battlefield`
For a Kaldheim modal DFC (Tergrid, God of Fright // Tergrid's Lantern) the two faces are DIFFERENT
permanents; casting the Lantern face should yield the Legendary Artifact (a repeatable {T}: drain), not the
God creature. Here the player paid {3}{B} and received the 4/4 God — a mechanical discount and, this game,
BENEFICIAL (it enabled the steal and won). But it is rules-incorrect. Open questions for the engine-seam
owner: (1) does Wagic implement the Lantern artifact face AT ALL, or does its DFC model collapse both faces
to the front permanent? (2) if the Lantern-as-artifact is not deployable, the guide's oracle description of
the Lantern (a repeatable drain) describes an outcome the engine never produces — the guide has been kept
generic ("Lantern = cheaper face") pending this answer, and should be corrected to match engine behavior
once confirmed. Routed to whoever owns DFC / modal-cast resolution. Probe path: a deck199 stacked with
Tergrid + cheap mana to reliably reach a Lantern-affordable, God-unaffordable turn and inspect what
permanent resolves.

## Cross-seat data point (for deck140's owned forced-loss item a)
Forced-loss framing renders correctly at deck102 (exact template on my own Smallpox self-discards, sampled
vs133 s18 / vs140 s14-16 / vs131 s20-21 / vs137 s17-18). Pick quality mostly good (pitched spare
lands/dead cards), but ONE clean bomb-pitch: vs140 s14, hand {Necrogen Mists, No Mercy, Phyrexian
Obliterator}, the model discarded Phyrexian Obliterator (5/5 trample bomb) and kept two situational
enchantments despite the "KEEP your best spells" render. Data for deck140's efficacy verdict: the framing
RENDER is correct but did not prevent a bomb-pitch here (matches wave-22's Damnation+Pyroclasm class).
Reinforced in deck102's own guide (strategy edit 3) since this deck casts its own symmetric spells.

## Pilot (repetition_penalty=1.05) - seat data for synthesis
No degradation at deck102. Seat reply-length n=205: p50 1106 (== corpus), p95 5749 (< corpus 7784), max
14514 (< wave-22 seat max 16752). No spiral lengthening, coherent prose in long replies, no verbatim-loop
truncation observed (contrast wave-22's 16K flip-flop loops). 0 unparsed / 0 empty / 0 fallback at this
seat. Verdict: SAME / slightly BETTER.

## Strategy edits applied this wave (4)
1. DEMOTE the DFC flip-thrash crutch (engine now annotates it) + trim the over-specific Lantern-drain oracle
   text to "cheaper face" (pending R-DFC-BACKFACE-RESOLVES-FRONT) + ADD an explicit "take the steal"
   instruction for the "put on battlefield / don't put on battlefield" trigger (the model declined it vs27).
2. Add a narrow, gated exception for CHEAP Smallpox while slightly behind (kill an opponent's key engine
   creature when not being raced) and re-scope the hard rule from "NEVER while behind on life" to "NEVER
   while being RACED and low on life" — matching the disciplined vs131 s19 play.
3. Add a "forced discard -> pitch a spare land, KEEP your bombs" paragraph (the vs140 s14 bomb-pitch).

## ROTATION VERDICT
**NOT a rotation candidate THIS wave — but convergence is imminent.** This wave produced real, corpus-
evidenced guide work (4 edits) = a new-work signal, AND surfaced a new engine defect (R-DFC-BACKFACE-
RESOLVES-FRONT) that must be understood before the guide's Lantern description can be finalized. So the
rotation test (no-guide-mod AND no-new-work-signal) is NOT met. HOWEVER: the keystone is now proven
(cast + steal fired + won), the flip-thrash is closed, and three of the four guide edits are refinements
rather than gap-fills. If next wave (a) the DFC crutch demotion holds, (b) R-DFC-BACKFACE-RESOLVES-FRONT is
resolved (or confirmed benign) so the Lantern description stops being provisional, and (c) the steal-take
line lands cleanly, deck102 will be a STRONG rotation candidate (guide converged, keystone validated).
Re-validate once more, then rotate.
