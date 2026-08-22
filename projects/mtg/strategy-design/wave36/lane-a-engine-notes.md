# Wave-36 step-1 LANE A (engine bugs) — per-item record

Branch: worktree lane-A. Evidence corpora: arm C = `matchups-20260822-111102`,
arm B = `matchups-20260821-033000`, arm A = `matchups-20260820-192210`.

## 1. Shockland enters "untapped" on payment-decline path (139 N1) — VERDICT: engine RULES-CORRECT; the RENDER lied. Fixed at the GPT seam.

Ground truth pulled from the arm-C translog itself (139v116 seq8-10): at the ask
the board already reads "Steam Vents [tapped]" (Arboreal Grazer's `and!(tap)!`
applied at move time) and "Mana available: 0"; after the pilot paid, life went
20->18 and the land STAYED tapped (seq9 shows it untapping normally at the next
untap step). So the engine already implements the real rule (payment does not
undo the put-tapped instruction). The defects were surface lies: the pay
option's unconditional "[enters the battlefield UNTAPPED - usable this turn]"
annotation, and the consumed-decision narration "It entered untapped (you paid
2 life)". Fix (AIPlayerGPT):
- `annotateEtbPayOrTapMenu` now takes `alreadyTapped`; when the context
  permanent is already tapped the pay option reads NO-OP/will-NOT-untap and the
  tap option reads "stays tapped either way".
- When the pay-or-tap menu arms on an ALREADY-TAPPED permanent, the seat
  auto-answers the decline with no model call (strictly-dominated payment;
  narrated truthfully). This also retires the ask class that owned all three
  arm-C fallbacks at the 139 seat (N2 cluster: derails on a pay/tap menu whose
  stakes were imaginary).
- Guarded on `ctx->isTapped()`: if the menu's context instance is unreadable
  (the known stripped-copy case), behavior is byte-identical to before.
PARSETEST: +5 cases ([A-shockland] alreadyTapped variants + negatives).
Caveat for the validation corpus: ctx tap-state readability on the LIVE path is
unverified (the stripped-copy comment at the seat suggests it may not carry
state); if the arm-C fallback cluster and the "entered untapped" line recur,
ctx is a copy and the fix must move to the menu-arming flow.

## 2. Silverquill Command fused-mode target ask (146 #1, ESCALATED) — FIXED (attribution note at the target seam)

Engine truth (borderline.txt): the battlefield-creature target feeds ONLY the
pump half (`target(creature) transforms(...) ueot`); the sacrifice half is
`notaTarget(creature|mybattlefield) ... opponent` — opponent-chosen, never
asked of the caster. Oracle allows targeting ANY creature with the pump, so the
candidate list (own + enemy creatures) is legal — the missing fact was
attribution. Fix: `compoundModeTargetNote()` — when the acting ability's name
is a compound (" and "), the target ask states that each targeted part asks
separately, that THIS pick lands only on the part it belongs to (a pump helps
the picked creature's controller), and that opponent-assigned parts are chosen
by the opponent. Fires on the live shape (verified against the arm-C s32 ask:
abilityName renders as "creature gains 3/3 and sacrifice creature").
PARSETEST: +5 ([W36-N146q]).

## 3. Elite Spellbinder "exile menu offered only lands" (152 tier-review B-vs105 s24) — VERDICT: review claim IMPRECISE; script under-implemented the reveal. Script fixed.

The exile line's filter is CORRECT (`target(*[-land]|opponenthand)`, fires only
when a nonland exists — contrast A-vs-158 s11/14 where nonlands rendered fine).
What fired at s24 was the SECOND auto line — the all-lands fallback "Reveal
hand" (the ask even says 'its "reveal hand" ability') — which was scripted as
`target(*|opponenthand)`: a pointless pick of ONE card to reveal-and-return,
and an under-implementation of Oracle's "look at target opponent's hand" (whole
hand). Fix (script, data-only): fallback line now `all(opponenthand)
moveto(opponentreveal) and!( moveto(opponenthand) )!` — whole hand, no target
ask. The same line is shared verbatim by Check for Traps, Grief, and Humiliate,
whose Oracle texts all reveal the whole hand; all four spliced. Fixture:
`ai/spellbinder_allland_hand_n152j.txt` (registered; green: ETB resolves, no
card stranded, nothing exiled).

## 4. Intrepid Adversary valor menu lists unaffordable options (152 #1) — FIXED (header semantics note)

The counts are the CHOOSE_MODE answer space and the staleness key, so
filtering/capping them is off the table. The engine partial-pays (verified
live: "add 10" on ~3 spendable mana resolved exactly 2 counters), and that
semantics was undocumented — the trap was believing the pick had to be
affordable. Fix: `payRepeatModeNote()` — a menu with >=2 "Add N counter(s)"
modes gets a header note stating per-counter payment, that an over-ask never
fails and stops at what the mana covers, and that with no spendable mana every
option adds 0. PARSETEST: +5 ([W36-N152h]). (A numeric "at most N affordable"
cap was deliberately NOT emitted: both potential-mana bounds available at this
seam can over- or under-state N, and a wrong number stated as ground truth is
the worse defect per the trust doctrine.)

## 5. No-attacker asks for 9 turns (146 tier-review B3 t12-28) — DIAGNOSED + INSTRUMENTED; root cause narrowed, not yet closed

Forensics (both game logs, B3):
- Last attackers ask t10 (146 numbering), next t30. In the gap the seat held
  untapped legal attackers and asked/answered cast decisions normally.
- The trigger event is the t12 fused Silverquill Command (pump+sacrifice). The
  opponent-granted sacrifice chooser (139's pick) surfaced NINE real turns
  late — 139's seq39 t22, the same turn 146 cast its SECOND Silverquill — and
  139 then itself stopped receiving attacker asks t23-27 (second grant
  pending), both seats recovering t29-31.
- The arm-C sibling (146v105) never surfaced the sacrifice ask at all before
  the game ended.
- Scripted probe (`_probe_silverquill.txt`, run + deleted): the grant machinery
  itself is FINE — a menu of the receiver's creatures arms and one click
  resolves the sacrifice immediately. So the defect is that on live AI seats
  nothing routes the RECEIVER to that pending chooser at grant time (the
  interrupt hand-off that menus normally get); it lingers until an unrelated
  menu event re-pumps the layer, and while it pends, combat issuance is
  suppressed for (at least) the caster's seat.
- Open question: which pendingCombatDecision gate is the suppressor. The four
  global gates (unresolved stack / open menu / live chooser / extra payment)
  are shared-observer state, yet 139 attacked at t19/t21 inside 146's
  suppression window — so either the gate state toggles per-seat in a way the
  logs cannot show, or the suppressor is hasLegalAttacker mis-reading the
  caster's board.
Shipped: N-146s combat-suppression tracer in
`GameObserver::pendingCombatDecision` — dev-builds-only (`_DEBUG` /
`WAGIC_DEVLOGS`, compiled out of release per the diagnostics rule), logs once
per (turn, phase, seat) WHICH gate suppressed a combat decision at the combat
phases, to stderr (the harness already harvests per-game .stderr). The next
corpus occurrence of the stall will carry its own diagnosis. Engine-fix
direction docketed: hand the receiver an interrupt at grant time (the
setIsInterrupting idiom the engine already uses for menus).

## 6. Kitchen Finks hybrid {1}{g/w}{g/w} absent from cast list (116 tier-review #3, "low-confidence") — VERIFIED REAL; engine FIXED (red->green)

Verified from the arm-A translog (116v105 s17): untapped Forest + Tropical
Island + 2x Glimmerpost — TWO G-capable sources plus generic — so Finks was
affordable and absent. Root cause, two stacked defects in ManaEngine:
(a) potentialMana is one-ability-per-card (each dual contributes only its
first-listed colour), so pMana->canAfford saw one G where two G/W pips needed;
(b) planPayment's hybrid pass ran AFTER the generic fill had swallowed sources
in layer order, and its gate compared accumulated colour against a SINGLE
pip's value (`result->getCost(c1) < 1`), so a second identical hybrid pip
could never receive a second source of the same colour. payable() therefore
failed on both routes and legalCasts silently dropped the card. Fix: a
HYBRID-PIPS-FIRST walk in planPayment — each pip claims one unused producer of
either colour side, before the generic fill, preferring sources that pay none
of the cost's plain colored pips; the final `result->canAfford(cost)`
validation is unchanged, so shapes the walk cannot complete stay unpayable
exactly as before (no over-offer route). Fixture
`ai/hybrid_double_pip_cast_n116h.txt` (registered): RED on the pre-fix archived
binary `/tmp/wagic-f0689f56f`, GREEN post-fix.

## 7. Agadeem's Awakening no-op cast offered (146 #2 / B3 s74-75) — FIXED (deciding fact rides the option); cast stays offered BY RULE

Oracle: "return ... ANY NUMBER of target creature cards" — zero targets is a
legal cast, so 601.2c does NOT forbid the offer; filtering it would suppress a
legal (if bad) play. The defect is trust-doctrine shaped: the surface offered a
cast that returns nothing and never said so (B3 burned 10 mana + the turn
"returning" Kaya, a planeswalker). Fix: `dynamicMagnitudes` gains a
graveyard-return clause — a cast/option whose script moves
`target(creature...|mygraveyard)` to `mybattlefield` while the controller's
graveyard holds NO creature card renders "{right now: returns NOTHING - your
graveyard has no creature cards, so the return part cannot happen at any X}".
Any creature present suppresses the tag (per-X legality is not cheaply
provable; a false "returns nothing" would be the worse lie). Witness seat for
validation must NOT be deck146 (HL10.1 — its guide do-not suppresses the
exercise); the probe/annotation is board-state-dependent so cover is the
corpus prediction below.

## 8. Noise-only upkeep window (152 #3) — VERDICT: do NOT auto-pass; scoped fact fix shipped

The s32 window's options included two REAL activated abilities (Lair of the
Hydra animation, Katilda mass-counters) alongside the two Flip-Side toggles —
auto-passing would suppress legal instant-speed plays (the N-152b auto-pass
correctly stays scoped to ALL-toggle windows). The actionable defect was a
scope gap in the turn-structure facts: at UPKEEP the ask said only "Still
ahead ... your SECOND MAIN PHASE", and the pilot answered with its planned
creature cast ("CHOICE: Cast Tovolar's Huntmaster" -> unparsed_reply). Fix:
`kMainPhasesAheadFact` — pre-first-main windows on the model's own turn now
state that BOTH mains are ahead and that main-phase cards are NOT castable in
this window (their own Cast menu comes later this turn); between the mains the
second-main fact stands unchanged. PARSETEST: +3 ([N-152i] cases inside
W35-combat).

## Falsifiable predictions for the wave-36 validation corpus

1. (Item 1) The shockland pay/tap ask class produces ZERO fallbacks at the 139
   seat (arm C had 3/3 there), and no "entered untapped (you paid ...)" line
   ever appears in a narration where the land was put in tapped. If the class
   RECURS, ctx->isTapped() is unreadable at the seam (stripped copy) — move the
   fix to the menu-arming flow.
2. (Item 2) Any fused Silverquill pump-target ask carries the compound note; no
   seat pumps an enemy creature while its trace cites whole-spell ambiguity.
3. (Item 3) No "Reveal hand" one-card target ask appears for
   Spellbinder/Grief/Humiliate/Check for Traps; all-land hands log a whole-hand
   reveal with no model call.
4. (Item 4) Every Add-N-counters menu ask contains "over-ask never fails"; no
   trace reasons that a large N is unaffordable-and-therefore-illegal.
5. (Item 5) If any seat again shows >=2 consecutive own-turns with legal
   attackers and no attackers ask, its .stderr carries [combattrace] lines
   naming the suppressing gate (the diagnosis the fix needs).
6. (Item 6) Hybrid multi-pip casts (Kitchen Finks class) appear in cast lists
   whenever two same-colour-capable sources + generic are untapped; deck116's
   "only Farseek offered" shape does not recur with Finks affordable.
7. (Item 7) Any Agadeem cast option offered over a creatureless graveyard
   carries "{right now: returns NOTHING...}"; no seat casts it in that state.
8. (Item 8) The upkeep hallucinated-cast class (numberless "CHOICE: Cast X" at
   a pre-main window) does not recur; pre-main priority asks contain "BOTH your
   main phases".
