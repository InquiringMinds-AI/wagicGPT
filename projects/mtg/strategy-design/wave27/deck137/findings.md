# deck137 -- Selesnya (GW) Adventures -- wave 27 (guided ROUND 5)

Binary ffec79fd8, corpus matchups-20260725-193608, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: 1785*-ai_baka_deck137-0x*-vs-{131,18,134,93,136,22}.jsonl.

Outcomes: **2/6, 0 timeouts, 0 draws** (DOWN from wave-26's 3/6).
- WIN vs134 (KILL, 16/0 t12) -- deck134 is the corpus 0/6 mono-U tron durdler; killed with adventure bodies + the one Loxodon that resolved.
- WIN vs93 (adj-ahead, 6/2 t16) -- Relentless Rats; ahead with Lovestruck 5/5 + fliers.
- LOSS vs18 (KILLED -8/13 t11) -- Kithkin anthem-aggro; board stranded at 2 creatures, raced 20->dead.
- LOSS vs22 (KILLED -1/17 t11) -- mono-R Giants; outraced, payoffs never fired.
- LOSS vs131 (adj-behind, 11/15 t19) -- Izzet/Elixir grind; could not close.
- LOSS vs136 (adj-behind, 9/12 t10) -- mono-B Rankle/Knight; ground out.

Corpus health at my seat: **0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo, 0 priority
fallback.** All 22 of my fallbacks are convoke defers (job 3 -- and this wave they are NOT
harmless; see section 3).

## (1) d1 INSTANCE HANDLES -- VALIDATED AT THE MOTIVATING SEAT. The wave-26 breach class did NOT recur.

The wave-26 breach (vs18 s36: two same-named Lovestruck Beasts in mixed tap states, no instance
handle, model resolved the name collision against itself and DECLINED a free engine-offered block)
was the direct motivator for the d1 fix. This corpus, the `#N` handles RENDER everywhere and the
model BINDS them correctly.

**Renders present on every surface I checked:** battlefield lists, A-lines, B-lines, target lines,
and land tags all carry `#N` for same-named permanents -- `Faerie Guidemother // Gift of the Fae #1`
/`#2`, `Relentless Rats #1`/`#2` (opp), `Blind-Spot Giant #1`/`#2`/`#3` (opp), `Soldier #1`/`#2`/`#3`,
`Edgewall Innkeeper #1`/`#2`, `Prophetic Prism #1`/`#2`/`#3`, `Forest #1..#5`, etc. The model
REFERENCES the handles in its reasoning (not just echoes them).

**The exact successor of the wave-26 breach, now CORRECT -- vs131 s27 t12 (blockers, life 13/15):**
deck137 controls TWO Faerie Guidemothers, one tapped (attacked last turn), one untapped. The engine
offered only the untapped copy: `B1. Faerie Guidemother // Gift of the Fae #2 (1/1) ... - may block
A1 (both die)`. Reply: `BLOCKS: B1:A1` with reasoning "If I block with my untapped Faerie
Guidemother #2, both ... die ... My other Faerie Guidemother is tapped from attacking last turn and
cannot block." It bound `#2`->untapped copy, took the trade. This is the wave-26 breach scenario
(duplicate name, mixed tap, blockers seam) resolved. **d1 = VALIDATED.**

**Additional correct binds this corpus:**
- vs22 s29 t9 (blockers): opp attacks with `Blind-Spot Giant #1` (A1) and `Blind-Spot Giant #2` (A3);
  model assigns distinct Soldiers to the distinct instances (`BLOCKS: B1:A2, B2:A1, B3:A3`) and puts
  Trostani on the double-striker (neither-dies). Legal, sound, distinct-instance aware.
- vs93 s29/s41/s52/s65 (attackers): both `Faerie Guidemother #1` and `#2` declared as attackers each
  time; s65 recognized lethal ("opponent at 6 ... 1+1+1+1+5=9 ... lethal, win") and swung all.

**One NEW minor dent -- vs18 s58 t11 (blockers, life 2/13), ILLEGAL one-onto-three, parser-recovered,
board unwinnable:** four attackers (two 5/5 first-strike, a 3/3, a 1/3 = 14 power) vs 2 life with
only Loxodon 4/4 + Faerie 1/1. Reply: `BLOCKS: B1:A2, B1:A3, B1:A4, B2:none` -- piled Loxodon onto
THREE attackers (the illegal shape the BLOCKING clause forbids), and did NOT self-correct this time.
Parser recovered to `choice=2` = "Venerated Loxodon blocks Field Marshal" (took the first legal
B:A pair, dropped the illegal duplicates). The board was UNWINNABLE regardless (can block <=2 of 4
attackers; minimum unblocked >= 4 > 2 life), so the illegal shape cost nothing -- deck137 was already
dead from tempo. Compare vs22 s39 (same illegal open, `B3:A1..A4`), where the model DID self-correct
("Wait, I cannot assign B3 to four different attackers"). So the illegal-gang reflex persists under
lethal pressure; the c4 parser floor is what saves it. Low severity; WATCH, not a regression of the
d1 fix (a legality-comprehension dent under a lost board, not a duplicate-name bind failure).

## (2) B-LIST-AUTHORITATIVE CLAUSE (first corpus) + BLOCKERS FLOOR ROUND 3: floor HELD; clause EXERCISED and CORRECT.

Eight blockers records this corpus (vs131 x2, vs18 x2, vs93 x1, vs22 x3; vs134/vs136 x0). Seven are
short, legal, and correct:
- vs131 s16 (Innkeeper trades into Young Pyromancer -- removes their engine), s27 (the d1 bind above).
- vs18 s36 (correct NON-block: chumping a 1/1 into 3/3s while not lethal = pass, take 6 -> 10).
- vs93 s22 (gang-block a 3/3 Rat with three 1/2s to kill it -- legal, avoids a 3-for-nothing).
- vs22 s10 (Emmara 2/2 kills Universal Automaton 1/1, survives), s29 (multi-block above), s39
  (Lovestruck 5/5 block-and-kill a Giant, after self-correcting an illegal open).

The **B-list-authoritative clause** (shipped wave-26 from the s36 breach) was directly exercised at
vs131 s27: the model trusted the offered `#2` untapped copy despite controlling a same-named tapped
copy, and did NOT re-derive tapped state from the log. The clause + the d1 `#N` handle worked
together -- the handle disambiguated, the clause told the model to trust it. **Clause: CORRECT at
first corpus.** The ONLY dent is vs18 s58 (illegal shape, unwinnable, parser-caught, above). Blockers
floor for the LETHAL case it targets: HELD.

## (3) CONVOKE DEFERS 15 -> 23: DECOMPOSED. The class is NO LONGER "harmless by-design" -- CONVOKE IS BROKEN.

This is the headline finding. The defer RISE is a SYMPTOM of a real defect.

**Count reconciliation:** 22 of the 23 corpus defers are deck137's (vs18 x9, vs134 x7, vs93 x2,
vs136 x2, vs22 x2; vs131 x0). The **23rd is NOT deck137** -- it is a deck134 defer
(1785032563-ai_baka_deck134-vs-deck131, seq39 t19, empty prompt, options=0). deck134 is mono-U tron
with NO convoke card, so the defer class is a GENERAL "deferred_to_heuristic on an optionless step,"
not convoke-exclusive; the brief's "all 23 = deck137 convoke" is off by one. (Minor; routed to notes.)

**What the deck137 defers actually are, and why they don't work:** each defer follows an ASK whose
chosen_text is `Cast Venerated Loxodon with its convoke cost` or `Cast March of the Multitudes with
its convoke cost`. The defer is the convoke tap-payment sub-step (latency -1, empty prompt/reply).
Then I traced whether the spell actually RESOLVED:

  card                    | convoke picks (corpus) | resolutions
  ------------------------|------------------------|------------
  Venerated Loxodon       | 23                     | **2**
  March of the Multitudes | 17                     | **0**

March of the Multitudes -- deck137's go-wide + lifelink payoff -- resolved ZERO times in ~17 cast
attempts across the corpus; not a single Soldier token was ever created by it. Venerated Loxodon
resolved 2 of 23. **deck137's two core payoff spells are functionally DEAD.**

**Proof it is a real defect, not mana-screw or model error:**
- vs18 s16 t4: `{g}{w}{w}` (3 mana) + 2 untapped creatures (Flaxen 1/2, Giant Killer 1/2) = 5,
  exactly Loxodon's {4}{W}. Affordable. Model picked convoke. Next record (s18): only ONE land tapped,
  the two creatures STILL untapped, Loxodon STILL in hand. The convoke payment did not tap creatures;
  the cast aborted. deck137 then re-picked Loxodon at s19/s24/s28/s39/s54; Loxodon entered the
  battlefield exactly ONCE (t11, per the mirror log) -- after ~8 failed attempts -- while its board
  stayed at 2 creatures and it was raced 20 -> dead. This IS the vs18 loss mechanism.
- **vs134 s34/s36/s55: the disambiguator.** Here the engine offered BOTH cast options --
  `Cast Venerated Loxodon {4}{W}` (plain) AND `Cast Venerated Loxodon with its convoke cost`, and
  `Cast March of the Multitudes {G}{W}{W}{X}` (plain) AND the convoke variant. At s34 (5 mana in pool)
  and s55 (6 untapped mana) the PLAIN cast was fully affordable and would have resolved. The model
  chose the CONVOKE variant both times (steered by the guide's old "just pick the convoke option"),
  and it FAILED. March sat in hand from t2 (s10) to t12 (s61), picked ~10 times, never once leaving
  the hand. So: the convoke-cost cast variant does not resolve; the plain cast does.

**Layer routing:** the resolution failure is ENGINE/HARNESS (notes.md, HIGH, step-1 candidate --
the convoke tap-payment deferred to the heuristic does not complete, especially for the X-spell
March). The BEHAVIOR half is guide-fixable and shipped this wave (section 6): prefer the plain cast
when mana covers it; use convoke only when short; if a card stays in hand after a pick, stop
re-picking. This routes the model onto the working path when one exists (recovers late-game casts);
early tempo convoke remains dead until the engine is fixed.

**Verdict on the 4-corpus "harmless by-design" claim: REFUTED.** Prior corpora checked "did deck137
still attack on defer turns" (yes) but never checked whether the deferred CAST actually RESOLVED. It
mostly does not. This is the single highest-value finding at this seat in several waves.

## (4) CLOSING-SPEED FRONTIER: it is a CONSTRUCTION/ENGINE ceiling, NOT a guide lever, at this deck right now.

2/6, and deck137 CLOSED exactly one game by kill (vs134, the 0/6 durdler). Its other win was
adjudicated (vs93). All four losses are races/grinds it lost while its comprehension was clean.
Wave-26 named closing speed as "the live new-work signal ... deck-layer play work
(curve/sequencing/commit-timing)." This corpus REFINES that: the dominant cause of the slow close is
now identified -- **the board-flood/pump plan the guide is built around (March floods lifelink
Soldiers, Loxodon pumps the team) does not execute** because convoke is broken. With ~half its
payoff dead, deck137 can only grind with 1/1 fliers and a single 5/5 Lovestruck, which loses every
real race.

The attack doctrine is already SOUND -- the model swings full boards when unblocked (vs93 s29/41/52/65),
recognizes lethal, and correctly holds blockers when behind (vs18 s36). There is no attack-doctrine
tweak that fixes dead payoffs. So NAME IT: **closing speed at deck137 is ENGINE-BOUND first (fix
convoke), CONSTRUCTION-adjacent second (curve/redundancy of non-convoke closers); it is NOT a GUIDE
lever this corpus.** Re-evaluate the guide lever only after convoke resolves.

## (5) c/d BATCH + ADVENTURE/SHOCKLAND STEADY STATE.

- **d1 instance handles**: validated (section 1).
- **d2 changeling**: Universal Automaton renders `[changeling: counts as Giant]` on the opponent
  battlefield and `[changeling]` on its A-line at my seat (vs22 s10, s29-context). Clean; no misread.
- **d3 parser stash-leak**: no card behaved unexpectedly at this seat (convoke aside, which is a
  resolution failure, not a stash leak). No wither/adventure/lifelink anomaly.
- **d4 DFC backface**: no Kaldheim-god DFC in pool; no flip-thrash observed. Steady state.
- **d5 deck-load loud rejection**: deck137 loaded clean; no stderr rejection expected or seen.
- **c2 ETB pay-or-tap (shockland)**: all Temple Garden menus render self-describing options
  (`pay 2 life [this permanent then enters the battlefield UNTAPPED - usable ...]`), all chose=1 (pay
  2 life), all parsed (vs131 s3/s58, vs18 s15, vs93 s3, vs22 s3/s12, vs134 Temple Garden). Steady.
- **c1/c3/c5**: no target-preview / activated-pump / may-ask seat exercised at deck137 this corpus
  (Giant Killer's Chop Down tapper did not fire here). Nothing anomalous, nothing to validate.
- **c4 parser hardening**: no first!=final coded-line reversal DROP this corpus. The two illegal-shape
  blocker replies (vs18 s58, vs22 s39) were recovered by taking the first legal B:A pair. Gate: PASS.
- **Adventure-body fix (wave-25)**: STEADY. Lovestruck 5/5 and Faerie/Giant Killer battlefield bodies
  attack and block as real bodies throughout (vs93, vs22 s39); zero "not a real body" declines.

## (6) GUIDE CHANGE THIS WAVE: convoke paragraph rewritten (prefer plain cast; stop re-picking unresolved casts).

The old convoke paragraph said "you CAN afford it -- just pick the convoke option ... do NOT recompute
mana." Section 3 shows that instruction is now actively harmful: it steers the model into the broken
convoke path even when a working plain cast is offered, and its "just pick it" framing drove the
re-pick loop that wasted whole turns (vs18 Loxodon x8, vs134 March x10). The rewrite: PREFER the plain
cast when mana covers the full cost (reliable + leaves creatures untapped); choose convoke only when
short of mana; and if a card is still in hand on the next decision, STOP re-picking it and deploy /
attack / move on. This is a behavior fix that routes the model onto the working path -- NOT a paper-over
of the engine bug (which is separately routed to notes.md as HIGH / step-1).

## (7) ROTATION VERDICT: does NOT rotate.

Fresh, high-value new-work signal this corpus: the CONVOKE resolution defect (HIGH, step-1 candidate)
-- deck137's two payoff spells are dead, which is the mechanistic root of its 2/6 and its closing-speed
gap. A guide change also shipped (convoke preference). The comprehension surface HAS converged and d1
is now validated at the motivating seat, but a major play-affecting engine defect is freshly surfaced
here and only observable at the convoke seat (deck137 is the sole convoke deck). Unsentimental read:
this seat just produced the corpus's most important finding. KEEP in pool.

## repetition_penalty=1.05 -- fifth-corpus verdict: SAME. No prose harm, no spiral cure. Reasoning stays
coherent across the 12k-13k replies. Unchanged.
