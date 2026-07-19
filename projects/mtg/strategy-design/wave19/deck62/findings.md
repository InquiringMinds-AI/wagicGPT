# deck62 (Enchantresses) — REVISED-GUIDE ROUND 3 validation findings (wave 19)

Corpus: `matchups-20260719-092058`, binary `/tmp/wagic-72b05535d`. deck62's THIRD guided corpus. The
wave-18 revision (round 2) is live byte-identical at `Res/ai/baka/deck62_strategy.txt` (rule-6
off-case anti-spiral branch + A-list sharpen + redundant-Primal-Rage card note). 6 seat translogs,
182 decisions (ask 136, attackers 27, blockers 19).

Record **2/6** (same as wave-18, different pool result). Per Step 0-ter the record is context.
Bottom line: **the round-2 taught classes HELD, and the biggest story is an ENGINE WIN — the
wave-19 combat name-tolerance batch closed the exact A-list-disobedience->unparse class deck62 filed
last wave (N2).** deck62 had ZERO attacker fallbacks and ZERO block-name-rejection fallbacks this
wave. The 7 residual unparses are ALL a DIFFERENT class from last wave (decode-repetition loops +
ahead/close lethal-hunt + one channel-confusion); NONE is the old computed-P/T / Blanchwood-recount
pocket, which stayed dead into round 3.

## Record: 2/6 -- matchup-shape result, no taught-class regression

| vs | result | life (me/opp) | turn | read |
|---|---|---|---|---|
| deck135 | **WIN (adj1)** | 18 / 13 | 12 | Ahead on life+board (6/6 Yavimaya, 6/9 Birds), multiple alpha strikes (s15/s22/s31). One AHEAD lethal-hunt UNPARSE (s32) did not cost the game. |
| deck27  | **WIN** | 3 / -4 | 14 | Clean go-wide vs Zombies; full alpha strike s26/s31 (Druid+2 Sapro+Yavimaya, all trample). Taught win line fired. |
| deck35  | LOSS | -4 / 17 | 13 | Outraced by go-wide Slivers (6 flying-menace-trample bodies). At 6 life by T12; go-wide mirror lost on tempo. |
| deck14  | LOSS (adj1) | 10 / 11 | 16 | **RAZOR** adj loss vs Deep Blue; opponent stole deck62's Yavimaya with Control Magic. Went the distance, 1 life behind. |
| deck49  | LOSS | -1 / 14 | 15 | Raced out by red aggro/Dragons (Bloodmark Mentor + Thunderbreak Regent). At 2 life by T7; matchup-floor. |
| deck110 | LOSS | -9 / 20 | 10 | Affinity blowout (Cranial Plating / Master of Etherium). Matchup-floor, same as prior waves. |

Two wins (a go-wide mirror + an adj race held ahead), four losses of which one is razor-adj (vs14),
one razor (vs49 T7-onward), two matchup-floor blowouts (Slivers go-wide, affinity). The wins vs49
and vs35 from wave-18 flipped to losses and vs27/vs135 came in -- pool variance, not play. deck62
remains a turn-8-13 ground/go-wide deck that floors against faster/wider clocks. **The guide cannot
draw a faster clock; routes to construction, not prose (skill's rule).**

## HEADLINE: the wave-19 combat name-tolerance batch CLOSED deck62's N2 (A-list-disobey->unparse)

Last wave deck62 filed N2 (general-suggestions #2): *"when an ATTACK line names creatures, DROP the
ones not on the eligible A-list rather than unparsing the whole declaration."* That fix SHIPPED as
the wave-19 engine batch (`parseBlockAssignments` name->label second pass; ineligible-only
ATTACK->none). It is CONFIRMED WORKING at deck62, three live saves that would each have UNPARSED to
heuristic last wave:

- **vs14 s8 (attackers, mixed form):** `ATTACK: A1, Saproling, Saproling` -- the two named Saprolings
  were summoning-sick (just made by Fists this turn, per events). Engine dropped both, kept A1 ->
  `chosen_text: Canopy Spider`. Clean parse, correct maximal-legal attack.
- **vs35 s33 (attackers, over-named mixed form):** `ATTACK: A1, A2, A3, Druid of the Cowl, Saproling,
  Saproling` -- the eligible A-list held ONLY the three (pumped 1/2, Anthem+Rage) Argothian
  Enchantresses; Druid/Saprolings were ineligible (summoning-sick). Engine dropped the three
  ineligible names, kept A1/A2/A3 -> attacked with the three 1/2 tramplers into an all-tapped
  opponent board. Legal, non-terrible (3 trample dmg, opp had no untapped blockers). NOT a misfire --
  a name-tolerance save. (Board verified: engine genuinely has 3 Argothians; the model's real
  threats simply weren't eligible.)
- **vs27 s23 (blockers, NAME:NAME form):** `BLOCKS: Yavimaya Enchantress: Walking Dead` -- a
  card-name block assignment (not B#:A# codes). Engine resolved it via the name->label pass ->
  `Yavimaya Enchantress blocks Walking Dead`. Killed a 1/1, blocker survived.

The A-list DISOBEDIENCE BEHAVIOR persists (the model still over-names absent creatures in
must-attack mode -- vs14 s8, vs35 s33) but the engine now ABSORBS it. **The class that cost 2
fallbacks last wave cost ZERO this wave.** The guide's wave-18 A-list sharpen is now
belt-and-suspenders -- keep it frozen (do not trim; the durable layer is the engine fix).

## TRADE-OUTCOME annotations: accurate, legible, no misleads -- enabled the value blocks

First corpus with `(both die)`-class trade annotations on block options. Every block prompt now
leads with a lethal gate ("Unblocked, these attackers deal up to N - you would be at M - NOT lethal
/ LETHAL: ...") plus per-blocker-per-attacker naive trades ("(both die)", "(neither dies)", "(you
kill it, your blocker lives)", "(your blocker dies, attacker lives)", "[deals 0 - only absorbs
damage]"). Audited all 19 blocker decisions:

- **Accuracy: no mislead found.** Every annotation matched the board math (e.g. vs135 s25 B3
  Saproling 1/3 vs A1 Ice-Fang 1/1 deathtouch flyer = "(both die)" -- correct; vs14 s23 Spider vs
  Coral Merfolk = "(you kill it, your blocker lives)" -- correct).
- **Helped the value blocks:** vs135 s25 the "(both die)" label let the model trade a 1/3 reach
  Saproling for a deathtouch flyer; vs14 s23 the "you kill it, blocker lives" label produced the
  clean Spider-eats-Merfolk block.
- **RESIDUAL (2 instances, behavioral, NOT an annotation defect):** the model still CHUMP-BLOCKS its
  0/1 shroud engine to absorb NON-lethal damage while ahead, against the explicit "[deals 0 - only
  absorbs damage]" tag AND the "taking damage while ahead is often correct" line. vs135 s25 (at 18
  life, chumped an Argothian to Ohran Viper to save 1) and vs14 s36 (at 10, chumped an Argothian to
  a non-lethal 6). This is the protect-the-engine rule failing on DEFENSE (the guide only framed it
  for ATTACKING). Guide addition proposed (edit B).
- **One malformed block:** vs14 s36 `BLOCKS: B1:A1, B2:A2` named a phantom A2 (only A1 was
  attacking); engine took the valid B1:A1 and ignored the phantom. Non-fatal.

## The 7 unparses -- NEW pocket(s), NOT the computed-P/T / Blanchwood-recount pocket

Answer to the brief's question: **none are the old distrust pocket.** Three distinct shapes:

1. **DECODE-REPETITION LOOPS (4) -- the dominant unparse driver.** A short phrase repeated verbatim
   dozens-to-hundreds of times to truncation, carrying no advancing reasoning (the deck135
   REPEAT-LOOP signature). NOT guide-addressable -- a model/decode pathology.
   - vs35 s10 (blockers, T5, 20/20): "If the blocker is destroyed, the attacker deals its damage to
     the player? No, the attacker deals damage to the blocker." x dozens. A block-math loop at EVEN
     life. (This is the only residual block-seam unparse -- but it is NOT a name-rejection; the reply
     never emitted a BLOCKS line, so name-tolerance cannot rescue it.)
   - vs135 s14 (ask, T7, 19/17): "But I need to give Birds power. I have no power-boosting spells...
     So I should cast Argothian..." x dozens. Trivial decision (heuristic cast Argothian correctly).
   - vs14 s21 (ask, T10, 14/19): Canopy-Spider vs guide-quote loop; amplified by a redundant Primal
     Rage in hand (already in play -- "redundant but harmless").
   - vs110 s1 (ask, T1, 20/20, MULLIGAN): over-PLANS future turns ("play Forest and Birds, then Web
     the Pest... No...") instead of answering keep/mulligan; amplified by TWO Primal Rages in the
     opener. NEW shape (mulligan over-planning).

2. **AHEAD/CLOSE LETHAL-HUNT (2) -- a GUIDE-SCOPE GAP.** Genuine (verbose) lethal-hunt reasoning while
   the model is AHEAD or even, which the wave-18 anti-spiral line does NOT cover (it is scoped "WHEN
   YOU ARE BEHIND"):
   - vs135 s32 (ask, T11, 18/13, AHEAD): winning board (6/9 Birds, 6/6 Yavimaya), spirals hunting
     exact 13-this-turn ("I need 13 damage... 6 is not enough... Web adds no power...") -> truncate.
     Game WON anyway (adj), but an unparse in a winnable game is a real cost.
   - vs14 s40 (ask, T16, 10/11, close): prospective Blanchwood cast-value + full block-outcome
     enumeration ("if opp blocks Spider with Yavimaya I deal 10...") -> truncate.

3. **CHANNEL CONFUSION (1) -- NEW.** vs27 s27 (ask, Main phase 2, 10/4): the ask was a CAST menu
   ("Cast Primal Rage / Cast nothing") but the model (already past its attack this turn) emitted an
   `ATTACK: ...` line, and the whole reply is DUPLICATED (sampling artifact) -> no CHOICE/Cast line
   -> unparse. Correct answer was "Cast nothing" (Primal Rage redundant -- the model even said so).

## Giant replies (>8k): 8 -> 13, but the increase is decode-loops + ahead-hunt, not behind-spiral

Bucketed 13 giants by mechanism:
- **Decode-repetition loops (4):** vs35 s10, vs135 s14, vs14 s21, vs110 s1 -- all truncate -> unparse.
  Not guide-fixable.
- **Ahead/close/even lethal-hunt (4):** vs135 s32 (unparse), vs135 s17 (parsed), vs14 s40 (unparse),
  vs14 s12 (parsed, even). The guide-scope gap.
- **Behind, RESOLVED CORRECTLY (3):** vs49 s17/s23/s27, all at 2 life vs 14-16, ALL parsed to the
  correct survival play (cast Argothian / Play Forest / ATTACK: none with a reasoned "I'll likely
  still lose but this minimizes damage"). **This is the wave-18 anti-spiral line WORKING** -- the
  behind-case that TRUNCATED to unparse last wave now reaches the right choice and parses. Tax
  survives as length, fallback does not (skill's tax-relocation).
- **Block giant (1):** vs14 s36 (close chump-block).
- **Prospective-math giant (1):** vs14 s37 (parsed; note the engine injected a stale-plan correction
  here -- "Canopy Spider is tapped... you have three untapped Argothians" -- and the model then blocked
  correctly; the correction machinery worked).

Net: the giant count rose because the pool put deck62 in more ahead-but-can't-close and
decode-loop-prone spots, NOT because the behind-spiral regressed. The behind-spiral (the wave-18
target) actually IMPROVED (parses instead of truncating).

## Per-taught-class: died / persisted / mutated

| Taught class | Verdict | Evidence |
|---|---|---|
| Auras DRAW/PUMP, not removal | **DIED (held)** | 0 recurrences; every aura framed as pump/token/draw. |
| Enchant-opponent GIFT (targeting) | **DIED (held, perfect)** | Every aura target sub-menu that offered an opponent creature picked an OWN creature. Zero gifts. The "legal targets right now: <names>" labels remain load-bearing. |
| Argothian-first sequencing | **DIED (held)** | Cast promptly on curve; Argothian prioritized in every casting spiral (even the loops resolve to "cast Argothian"). |
| TRUST printed [X/Y] / Blanchwood recount-ban | **HELD DEAD (round 3)** | No recount-of-already-printed-P/T. The only Forest-counting (vs14 s40, vs49 s23) is PROSPECTIVE cast-value math (Blanchwood not yet attached) -- same verdict as wave-18 N3. Watch stays retired. |
| Attack every turn / no durdle | **PERSISTED (working)** | Alpha strikes in the wins (vs27 s26/s31, vs135 s31); correct ATTACK: none only when eligible set was empty/sick. No 0-attacker durdle in the adj games (vs135 win, vs14 adj loss both attacked every eligible turn). |
| Attack from the A-list (eligibility) | **DISOBEYED-but-ENGINE-ABSORBED** | Model still over-names absent creatures under must-attack pressure (vs14 s8, vs35 s33) BUT the wave-19 name-tolerance now drops them -> 0 unparses. Behavior persists, cost is gone. |
| Off-case anti-spiral (BEHIND) | **WORKING (behind case)** | vs49 s17/s23/s27 at 2 life reach the correct play and parse (vs wave-18 truncation). BUT the AHEAD/close lethal-hunt is uncovered (vs135 s32, vs14 s40) -> guide-scope gap, edit A. |
| Protect the engine (attacking) | **PERSISTED on offense, FAILS on defense** | Never attacks with the 0/1 Argothian on offense; but CHUMP-BLOCKS it to absorb non-lethal while ahead (vs135 s25, vs14 s36) -> edit B. |
| Redundant Primal Rage note | **AWARE-but-INSUFFICIENT** | Model quotes awareness ("redundant but harmless") yet still burns tokens on it inside decode loops (vs14 s21, vs110 s1). Guide can't do more; engine annotation item (N-redundant-global) re-filed. |

## Guide disposition: SURGICAL REVISION (round 3), two evidence-driven edits

1. **(edit A) GENERALIZE the anti-spiral line from "behind" to "cannot win THIS turn, ahead OR
   behind."** Evidence: the lethal-hunt spirals this wave are dominantly while AHEAD/even (vs135 s32
   unparse, vs135 s17 giant, vs14 s40 unparse), which the "WHEN YOU ARE BEHIND" scoping misses. The
   behind-case is already handled (vs49 giants parse correctly). Low-risk broadening of an
   already-validated behavioral branch.
2. **(edit B) Add a DEFENSIVE protect-the-engine clause.** Evidence: vs135 s25 + vs14 s36 chump-block
   the 0/1 shroud engine to absorb non-lethal damage while ahead, ignoring the "[deals 0 - only
   absorbs]" annotation. The guide framed protect-the-engine only for ATTACKING; add the DEFENSE
   face ("don't chump-block the 0/1 engine when the prompt says NOT lethal and you're comfortable").

Everything else byte-frozen. NOT a rotation candidate: guide-mod = yes, new-work-signal = yes
(decode-loop class + channel-confusion need another corpus; edits A/B need validation).
