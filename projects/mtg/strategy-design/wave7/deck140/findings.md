# Deck-140 review — wave 7 findings

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Plan: sweep, deploy a finisher,
grind their face to 0 on reach.

Corpus: `matchups-20260713-222831`, 6 deck140 games. **Record COLLAPSED: 0 W / 4 L / 2 timeouts.**
- vs110 **L** (t11, my-8/opp19) — held Pyroclasm/BSZ/Edict while affinity creatures grew & killed it
- vs44  **L** (t12, my-9/opp19) — held BSZ vs 4 flyers 2 turns (shroud-conflation), raced out
- vs109 **L** (t15, my0/opp15)  — swept 4x CORRECTLY, bled out to aggro (reach starvation)
- vs133 **L** (t11, my0/opp17)  — fast Bloodghast blitz + one broken reply; 9 asks total
- vs135 **T** — **19/34 fallbacks, 32s median latency** = pure `-j` infra, not play
- vs131 **T** — deck140 AHEAD 22-14 vs a creatureless burn deck; slow clock + 112s latency spikes

Wave-6 record for the same slate was ~1 W / 2 L / 1 adjW / 1 adjL / 1 draw. The one clean win
(vs131 Blightsteel) is now a timeout; vs133 (was adj-ahead) is now a loss. This is the exact
displacement shape the Step-0 audit exists for: **wave-6 fixed early-waste by WIDENING the
hold; wave-7 shows the hold overshot into hold-EVERYTHING.**

Severity: [GUIDE] model-fixable — [STRUCTURAL] deck/infra.

---

## Harness / executability sweep (run FIRST)
- **Fallbacks: vs110=1, vs44=0, vs109=0, vs135=19, vs133=0, vs131=1.** vs135 is a latency-starved
  fallback cascade (median latency 32.5s, spikes 46.7s) — 19 empty replies out of 34 decisions.
  **Score vs135 as INFRA, write nothing against its play.** vs133 t6 emitted a malformed reply
  `#1. #  #` (parsed as a broken choice) — one bad decision, infra-adjacent.
- **Latency:** heavy `-j` saturation everywhere — vs131 spiked to **111.9s**, vs110/vs44 to ~47s,
  median 6-8s on the fast games (solo baseline ~4-5s). The two timeouts are latency-bound + slow-clock.
- **X-menu offset:** clean. 8 X-announcements, every one option 1 = largest X (Rakdos X=6 vs131,
  BSZ X=4 vs44, etc.). The wave-4 index/value trap is dead three waves running.
- **Desyncs:** none observed (reply head integer == choice on spot checks).

The corpus is clean at the harness layer except vs135 (infra). Every leak below is bad play or
deck construction.

---

## TOP FINDING

### 1. [GUIDE — the wave-6 fix OVERSHOT] The pilot now HOLDS sweepers while the opponent HAS creatures
Wave-6 widened the hold in two places: (i) the cast-nothing rule case (a) — "hold when every
offered spell is a sweeper/Edict AND the opponent has NO creatures"; (ii) the new corrective block
"YOUR SWEEPERS HIT CREATURES ONLY ... against a board of only lands and artifacts with NO creatures
... HOLD it." Both are correct in the abstract. In practice the weak executor bridged "NO creatures"
to boards that clearly HAD creatures, via three false beliefs the widened rule licensed. **Sweeper
held while the opponent had 1+ creature showing (X/Y): ~8 decisions, all in the two decisive
losses** (vs110 x6, vs44 x2). This is the single biggest model-fixable leak and it caused the
collapse.

The three bridges (quoted from the pilot's own PLANs, each checked against the printed board):

- **(a) "TAPPED creatures = no threat, wait." (vs110 — decisive)** At t6/t8/t10 the opponent's
  Memnite + Steel Overseer + Etched Champion were on the board but **tapped from attacking**. The
  pilot: *"The opponent has no untapped creatures ... tapped from attacking last turn ... no immediate
  threat to remove"* -> **Cast nothing**, six turns running. Steel Overseer pumped Memnite 1/1 -> 2/2
  -> 3/3 across those held turns; deck140 fell 18 -> 16 -> 2 -> **-8 dead at t11** holding castable
  Wrath/Damnation/BSZ. A sweeper is a sorcery cast on YOUR main phase — tapped-ness is irrelevant,
  and tapped attackers untap and swing again. This false belief is fatal and the guide never denies it.

- **(b) "Affinity's artifact-heavy board = my sweepers are useless."** Same game, the pilot reads a
  board of Mox Opal / Glimmervoid / Darksteel Citadel **plus creatures** as an artifact board:
  *"The opponent has 6 artifacts and 0 untapped creatures ... my sweepers are ineffective."* It is
  **over-applying wave-6's own new fact** ("sweepers do nothing to artifacts, hold vs artifact
  boards"). Artifacts sitting next to a creature do not make a sweeper useless — it still kills
  every creature. The wave-6 fix taught the exact overgeneralization now losing games.

- **(c) "Creatures have shroud = all my removal, including sweepers, is useless." (vs44)** At t7 the
  pilot held Black Sun's Zenith against 4 flyers: *"the opponent's creatures have Shroud, making
  them untargetable ... Black Sun's Zenith is a valid play to clear the board"* — it **named BSZ as
  correct in the same PLAN** and chose Cast nothing anyway, then cast it two turns later (t9) after
  falling 20 -> 12. **Verified against `auto=`:** BSZ `counter(-1/-1,X) all(creature)`,
  Pyroclasm `damage:2 all(creature)`, Wrath/Damnation `bury all(creature)` — **none target**, so
  shroud is irrelevant. And Cruel Edict `auto=... targetedplayer` with `notaTarget(creature)` —
  it targets the PLAYER, so shroud on the creatures does NOT stop it either. Every "shroud"
  justification is a false belief across the board.

**Fix (calibrate back to the MIDDLE, do not swing to always-cast):** replace the false-belief-
bridgeable "NO creatures" with a HARD, board-readable counter and kill all three bridges inline —
"count the (X/Y) tokens on the opponent's battlefield; that number, tapped or untapped, IS your
sweep trigger; artifacts and shroud do not change it." Give the sweep a countable FIRE trigger
(1+ creature and your life <=16 or they attacked you = sweep THIS turn with any castable sweeper).
The hold survives ONLY for a literally 0-creature board. See strategy.txt block SWEEP TRIGGER.

### 2. [GUIDE routing + DECK-CONSTRUCTION] "No red mana" for Pyroclasm freezes the whole turn
Across vs110/vs131/vs109 the pilot repeatedly names **Pyroclasm {1}{R}** as "the sweeper," finds
it uncastable for lack of red, and **holds the entire turn** — ignoring that Wrath {2}{W}{W},
Damnation {2}{B}{B}, and Black Sun's Zenith {B}{B}{X} are castable off the white/black mana it DOES
have. Red is 14 of 26 lands (Plateau/Badlands/Akoum/Clifftop) so it is not systemically starved,
but Pyroclasm wants red on a specific early turn and the pilot treats one uncastable sweeper as
"no sweep available." **Fix (guide):** teach sweeper SELECTION as "cast the first sweeper you can
PAY for" and list the castable-color of each, so a red-light hand routes to Wrath/Damnation/BSZ
instead of a passed turn. Modest deck-construction note: a couple more untapped red sources would
make Pyroclasm the reliable cheap sweeper it is meant to be (notes.md).

### 3. [STRUCTURAL — unchanged] Reach starvation vs aggro (vs109, vs133)
These two losses are NOT hold-overshoot. vs109: the pilot swept **correctly four times** (Cruel
Edict t2/t8, Wrath t6 on 3 creatures, Pyroclasm t10 on Ash Zealot+Legion), held only on genuinely
empty boards, and still bled 21->16->12->9->4->2->dead because a mono-red aggro deck rebuilds faster
than one-for-one sweeps and deck140 has no early lifegain, no blocker, and no fast clock. vs133:
mono-B Bloodghast blitzed it dead by t11 in 9 decisions. Faithful execution, no reach.
**Deck-construction flag, not a guide bug** — do not answer with louder prose. One partial guide
lever: Lightmine Field (damages all attackers each combat, `auto=` verified) is the deck's anti-aggro
tool and was never deployed in these games; a "vs a deck attacking you, Lightmine Field is a priority
deploy" line is worth one slot (it punishes go-wide aggro every combat, which one-shot sweeps cannot).

### 4. [INFRA / STRUCTURAL] Timeout triage x2 — NOT hold-overshoot
- **vs131 (TIMEOUT, 123 records, deck140 AHEAD 22-14):** the opponent (mono-red Guttersnipe) was
  **creatureless almost the entire game**, so the ~118 "sweepers offered / held" were **mostly
  CORRECT holds** — do not read this raw count as the overshoot. deck140 removed Young Pyromancer
  with Edict (t4, t18) and Wrath (t18) the moment creatures appeared, deployed Staff (t14), fired
  Rakdos X=6 (t16, opp 18->12), and led on life the whole way. It timed out on the **slow clock**
  (Blightsteel stuck at "need 12, have 6-8" for 10 turns) + 112s latency spikes at the cap. This
  was a wave-6 WIN; the collapse to timeout is slow-clock + infra, not a piloting regression.
- **vs135 (TIMEOUT):** 19/34 decisions were empty-reply fallbacks at 32s median latency. Pure `-j`
  saturation. Infra, write nothing.
Control mirrors still generate 3-4x the decisions of aggro games, so at a fixed wall-clock cap they
eat the timeout column regardless of play (the cap-by-decisions / stalemate-detector ask is open —
notes.md).

### 5. [KEEP — obeyed, demote to compressed cores] Rakdos hold, X-menu, Staff targeting
- **Rakdos's Return hold is NOW largely working** — the wave-6 #1 fix landed. It held Rakdos on
  creatureless/non-lethal turns (vs131 held ~10 offers, fired one X=6 clock shot at t16; vs109 held
  5/6, fired X=6 at 2 life as a last swing). The early-chip disease of wave-6 (6/7 wasted) did NOT
  recur. **Do not re-widen or re-shout it** — keep the compressed hold rule; the one X=6-not-lethal
  fire in vs131 is a minor, non-decisive tempo call, not a leak to legislate.
- **X-menu:** 8/8 max-X by intent. Keep the one-line value-anchored confirm.
- **Staff of Nin:** pinged the face by default every game. Keep the one compressed line.

---

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. **Wave-6 cast-nothing widening + "sweepers hit creatures only" (guide lines 28-49).** **OVERSHOT.**
   The widened hold + the artifact-board fact bridged, via three false beliefs, into holding
   sweepers against real creatures (Finding 1). This is the wave. Fix = a countable fire trigger
   that the false beliefs cannot satisfy, not a swing back to always-cast (which recreates wave-5's
   early-waste — the pendulum the middle-calibration avoids).
2. **Rakdos hold (#1 rule).** Landed and OBEYED this wave (Finding 5). Freeze, compress.
3. **X-menu / Staff targeting.** Obeyed. Compress.
4. **No attack-floor issue** — deck runs ~no creatures; Blightsteel attacked correctly when reached.

## STRUCTURAL residual (unchanged, still real) — reach starvation + slow clock
vs109 and vs133 (and the vs131 timeout) share a precondition the guide cannot create: **no fast
castable clock and no early stabilizer.** 14 sweepers is the right density; the deck needs teeth
(a cheaper/second recurring clock, or a bit of early lifegain/defense vs aggro) — teeth, not text.
Do not answer reach starvation with louder guide prose.

## Wave-6 findings status
- **W6 #1 hold-vs-cast-nothing widening** — **OVERSHOT** into false-belief-bridged hold-everything
  (Finding 1). This wave's #1 fix: re-anchor to a countable creature trigger.
- **W6 #2 "sweepers hit creatures only" corrective fact** — **backfired**: over-generalized to
  "artifact/shroud board = sweeper useless" even with creatures present (Finding 1b/1c). Reframe it
  as a COUNT, not a board-archetype.
- **W6 Rakdos hold** — **now working** (Finding 5). Freeze.
- **Reach starvation** — **PERSISTS as STRUCTURAL** (vs109, vs133). Deck-construction flag.

## Priority for the guide reviser (next task)
1. **Kill the three false-belief bridges** (Finding 1): tapped-still-kills, artifacts-don't-shield-
   creatures, shroud-doesn't-stop-sweepers/Edict — each as a flat FACT tied to the counted (X/Y).
2. **Make the sweep a COUNTABLE fire trigger** (1+ creature and life <=16 or attacked = sweep now),
   and tighten the hold to a literal 0-creature board only.
3. **Route sweeper selection by castable color** so a red-light hand uses Wrath/Damnation/BSZ (2).
4. **Add one Lightmine-Field-vs-aggro deploy line** (3).
5. **Freeze/compress** Rakdos hold, X-menu, Staff targeting (5). Set expectations that vs109/vs133/
   vs131-timeout residual is deck reach + infra (notes.md).
