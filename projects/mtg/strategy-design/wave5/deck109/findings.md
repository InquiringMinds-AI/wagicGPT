# Wave-5 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** `matchups-20260713-145947`, one run, 6 games (round-robin, deck109 seat).
**Record: 5/6** — won vs 133, 135, 140, 44, 131; lost only to 110. (Same 5/6 as last corpus.)

| deck109 seat-file epoch | result | opp | end (my / opp life, turn) |
|---|---|---|---|
| 1783975561 | WIN | 133 | 20 / -1, T8 |
| 1783975954 | WIN | 135 | 20 / -2, T11 |
| 1783977326 | **LOSS** | 110 | 0 / 6, T8 |
| 1783977821 | WIN | 140 | 18 / 0, T29 |
| 1783980437 | WIN | 44 | 16 / -5, T11 |
| 1783981630 | WIN | 131 | 20 / -3, T11 |

**Headline.** The deck plays its aggro game cleanly and the two wave-4 engine bugs have split:
the game-losing **reply-parser desync is FIXED** (0 desyncs / all 6 games — the head-first parser
landed), while the **Hellrider cast no-op still persists** (140 seq54/seq56) but the guide's
fallback rule contained it into a slow win rather than a loss. The only genuine BAD-PLAY leak this
corpus is **reflexive/over-blocking by a racing deck at high life** — every one of the 4 blocker
decisions fired at >=18 life, and at least three spent attackers to kill things that were not
threatening us. It cost no games this corpus but is the deck's clearest remaining play error and
the one thing the guide should add. The loss to 110 was a close race against a faster deck on a
mana-light draw — variance/matchup, not guide-fixable.

---

## Harness-defect sweep (run first, per skill Step 2)

- **DESYNC (parser mis-grab): 0 across all 6 games.** Head-of-reply integer == recorded `choice`
  in every record. Wave-4's game-losing bug §1 (8 desyncs, one lethal-burn onto a 1/1) is GONE —
  the head-first parser fix is confirmed live. Consequence: the guide's output-format stopgap block
  (deck109_strategy.txt lines 6-14) is a stopgap whose defect is now fixed → DELETE it (see
  notes.md stopgap ledger).
- **ENGINE CAST NO-OP (Hellrider): PERSISTS.** In the 140 win, "Cast Hellrider" was chosen and
  no-op'd twice — `1783977821` **seq54 (T27 MP1)** and **seq56 (T27 MP2)**: the `Mana available:`
  line reads `... 7 untapped sources` BEFORE and `... 7 untapped sources` AFTER (a real {2}{R}{R}
  cast would drop it by ~4), and Hellrider stayed in hand. It resolved later (seq57, mana 7->4).
  This is the same class as wave-4 §2 (`#26` region / Hellrider-class cast no-ops). Fresh A/B repro:
  no-op `1783977821` seq54/seq56 vs resolve seq57. **The fallback rule worked** — the model kept
  deploying/attacking and won at T29 instead of durdling out. Keep the fallback stopgap; the bug is
  still live. (Minor: the model retried Hellrider twice in T27 rather than obey "one retry per
  turn," but by T27 its board was built and there was little else to cast, so it was harmless.)
- **NUMERIC-OFFSET (X menus): N/A.** This deck has no X-spells; no numeric menu decisions occur.
- **Fallback / empty-reply: 0.** No `fallback` records, no `choice==-1`, across all 6 games.

---

## What the pilot did WELL (preserve — freeze these lines)

Quantified over the 6-game corpus:

- **Aggression is total: 31 attacker decisions, 0 declared "no attackers."** The deck never sat
  home. Wide swings closed 5 games with the opponent at or below 0.
- **Burn is a clock: 14/14 damage targets went to "The opponent (player, life N)"**, 0 at own
  creatures/face. The burn-face-by-default rule holds perfectly.
- **Unleash counter always taken: 11/11** "1/1 Counter" picks. No hesitation.
- **Cast-nothing is 2 of 117 ask records (~1.7%).** Durdle is essentially gone; the deck deploys
  a creature or throws burn nearly every main phase (see the 44 win, T11, opp -5).
- **Development every turn** — MP1 land + a cast, then MP2 follow-ups (the MP2 checklist fires,
  e.g. 110 loss seq7-14, casting a body every turn under pressure).

## The one BAD-PLAY leak: reflexive / over-blocking at high life

A racing deck's creatures are its clock; blocking spends the clock. **All 4 blocker decisions this
corpus happened at >=18 life** (life was never in danger), and at least three were wasteful:

- **135 win, `1783975954` seq7 (T4, my 20 / opp 18):** a Goblin blocks a 2/2 Icehide Golem while
  at 20 life and AHEAD on the race. Reply: "trade 1-for-1, preventing 2 damage." A Goblin that
  should be attacking traded for 2 saved life we did not need.
- **135 win, seq13 (T6, my 20 / opp 16):** **two** Goblins gang-block one 2/2 Icehide Golem at 20
  life. Two clock-pieces spent to kill a single 2/2 while we were winning the race.
- **135 win, seq31 (T10, my 20 / opp 7):** Boros Reckoner (a 3/3, one of our best attackers) is
  held back to block a **Boreal Druid** (a 1/1 mana dork) at 20 life while the opponent is at 7.
  Blocking a ~0-1-power dork with our best body while we are one turn from killing them.
- **Contrast — the ONE correct block: 131 win, `1783981630` seq9 (T6, my 20 / opp 24):** two
  Goblins gang-block **Young Pyromancer** to kill it (2 dmg vs 1 toughness). This is the token-maker
  exception working as intended — Young Pyromancer must die on sight, and burn was not in hand.

The guide's only blocker guidance is the terminal negative line "Do not block or play a control
game - race." It is NOT firing (the model blocked reflexively four times), and per the
strategy-writing skill a racing deck's blocker seam needs a POSITIVE per-item floor, not a
tail-end DON'T. The fix (in strategy.txt): a blocker-seam rule stating "above 8 life, declare NO
blockers — keep your creatures free to attack," with the token-maker/lifegain exception preserved
(so the good 131 block still happens) and the survival release at <=8. This is the deck's clearest
remaining play error even though it cost no game this corpus.

---

## Overshoot audit (mandatory)

Wave-4 added loud rules to this guide. Checked each against this corpus for locally-obedient,
globally-bad play:

- **"ATTACK EVERY TURN WITH EVERYTHING" / "Do not keep blockers home" / "Trading a cheap attacker
  into a blocker is fine."** No overshoot into suicide attacks: every declared attack this corpus
  was into an empty or tapped-out board (e.g. 110 loss seq6/seq12/seq16 all note "opponent has no
  untapped blockers"). No attack-floor suicide observed.
- **"BURN - FACE BY DEFAULT."** No overshoot: burn went 14/14 face and the token-maker that
  appeared (Young Pyromancer, 131) was removed by combat rather than left alive, so face-default
  did not cause a token engine to survive.
- **MAIN PHASE 2 CHECKLIST / "cast something every turn."** No "one X is enough" undershoot and no
  waste-loop: the 44 win's 27 priority windows were all correct passes (no instant in hand to
  hold), and no repeated-activation loop (Boros Reckoner {RW} first-strike) appeared.
- **The FALLBACK RULE (Hellrider no-op).** Fired correctly in the 140 game; no overshoot (it does
  not prohibit casting Hellrider, so Hellrider still resolved when the engine let it).
- **Under-correction, not overshoot, at the blocker seam** (above): the terminal "Do not block"
  negative is the rule that failed to fire. This is the one rule to convert to a positive floor.

Net: no wave-4 rule over-corrected into globally-bad play this corpus. The guide's loud spine is
working; the gap is a missing positive blocker floor.

---

## Win/loss narratives

- **WIN vs 133 (T8, opp -1).** Clean curve-out; Phyrexian Obliterator not drawn, so the matchup
  section never had to fire. Aggro did its job.
- **WIN vs 135 (T11, opp -2).** Won comfortably but this is where the reflexive-blocking leak lives
  (seq7/seq13/seq31). We were ahead the whole game; the blocks were unnecessary clock-spends that
  the wide board absorbed.
- **LOSS vs 110 (T8, my 0 / opp 6).** A close race against a faster aggressive deck on a mana-light
  draw (never got past ~4-5 lands). Play was clean: cast a body every turn (Stromkirk, Gore-House
  w/ unleash, Krenko's, Cackler w/ unleash), threw both burn spells at the face on T5 (opp 19->14),
  attacked into open boards every turn, got the opponent to 6 — and died one turn short. Life fell
  20->17->9->5->0 as deck110 hit ~2.5/turn. No superior line is visible: burn was correctly spent
  as reach, no blocks were available (our extra bodies were summoning-sick each turn), and a
  racing deck switching to "defense" here would only have slowed its own already-losing clock.
  Attribute to variance + matchup speed + mana-light draw; NOT guide-fixable. (See notes.md for a
  soft deck-construction observation on the 20-land / no-mana-sink curve.)
- **WIN vs 140 (T29, opp 0).** A grind against a lifegain deck (opp climbed 4->6 at T27 before
  dying at T29). Our life sat at 18-20 the whole game — never in danger. Slow only because (a)
  deck140 gains life and (b) the Hellrider no-op cost several turns of the ping engine
  (seq54/seq56). Winning-but-slow against lifegain — different owners (engine no-op + opponent
  lifegain), not a guide bug. We won; the fallback rule kept us deploying.
- **WIN vs 44 (T11, opp -5).** Fast, clean overrun; 27 correct priority passes (no instants to
  hold), deploy-and-attack every turn.
- **WIN vs 131 (T11, opp -3).** The one game with a correct block (double-block to kill Young
  Pyromancer, seq9) plus normal aggro. Opponent's Elixir lifegain was outraced.

---

## Wave-4 findings status

- **§1 reply-parse desync — FIXED** (0/6 this corpus). Head-first parser confirmed live. Retire the
  guide's output-format stopgap block.
- **§2 Hellrider cast no-op — PERSISTS** (140 seq54/seq56, mana 7->7). Fallback rule contained it.
  Keep the fallback; re-file the engine repro.
- **§3 mana "attacking taps my lands" delusion — STAYS FIXED.** 0 real instances; the mana-count
  representation line + interface contract carry it. The guide's MANA block is now redundant with
  the general prompt → compress to a one-liner or drop.
- **§4 Phyrexian Obliterator (deck133) matchup — not exercised** (Obliterator not drawn in the 133
  game we won). Keep the section as a matchup note; it is still in the pool and remains correct.
- **§5 blockers seam — now the leak.** Exercised 4x, all at high life, all but one wasteful. This
  wave's guide-fixable item.

## Net guidance for the reviser (me -> strategy.txt)

Small revision (5/6 turnaround-preservation). FREEZE: identity/beatdown spine, attack-every-turn,
burn-face-by-default, unleash counter, MP2 checklist, Phyrexian Obliterator section, fallback rule.
ADD: a positive blocker-seam floor (no blocks above 8; block only to kill a token-maker/lifegain
creature you cannot burn, or to survive at <=8). DELETE: the output-format stopgap block (defect
fixed) and compress the redundant MANA/interface restatement. Convert the terminal DO-NOT list to a
positive situation->play recap.
