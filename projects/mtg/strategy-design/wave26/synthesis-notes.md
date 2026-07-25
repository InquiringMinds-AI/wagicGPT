# Wave-26 synthesis notes -- TWELFTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-26 PRIMARY corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-115216` (binary `77e91ef4e` = wave-26 step-1
batch, commits `79e39511f` + `77e91ef4e`; -T 3000, REPPENALTY=1.05, 4th corpus). Pool = 7:
27 UB Wither / 102 BW edicts+Tergrid / 133 mono-B Devotion / 131 UR Guttersnipe burn /
137 GW convoke+Adventures + the two GUIDELESS Step-0 debuts **18 Kithkin (mono-W tribal
anthem-aggro)** and **22 "Giants!" (mono-R tribal ramp)** replacing 59 + 109. Cross-wave record
comparison is context-only -- judge decisions, not the win column.

## THE CORPUS-LEVEL STORY

**The parser/reply-protocol stack held for a 2nd consecutive corpus -- but read the claim
precisely.** Wave-25 was zero-true-fallback across 1,299 decisions. Wave-26 is NOT a repeat of
that: 16 / 1,322 fallbacks = 15 by-design deck137 convoke tap-payment defers (harmless, 4th
consecutive corpus) + **ONE non-defer fallback** (deck133 vs102 s22, kind=priority,
retracted_choice). That one is the **c4 line-anchoring firing AS DESIGNED**: the reply led with
`CHOICE: 0 (pass)`, reasoned, explicitly retracted it, and recoded to option 3 -- but the recode
`So, CHOICE: 3` was NOT line-leading, so the hardening (correctly) refused to salvage a non-line-
anchored coded index and fell back SAFE-DIRECTION to the heuristic. It is structurally
INDISTINGUISHABLE from the harmful answer-then-drift shape the detector is built to catch, and it
was non-deciding (game won 14/-5). So: the c4 corpus-gate PASSED corpus-wide (every seat's c4
decomposition clean; deck27's 187-decision seat exercised natural-stop + truncated + ATTACK
answer-first all in one corpus with zero misfires) -- but deck133 CORRECTS the wave-25 read that
retracted_choice was "extinct at 0": it recurred once here on a DIFFERENT trigger (genuine self-
correction), independent of the de-fanged example-echo. **Do NOT claim a zero-fallback repeat;
claim a working, corpus-gated hardening with one safe-direction retraction.**

**0 timeouts / 0 draws -- a RETURN to norm, NOT a first (verified against prior waves).** Nine
corpora had zero timeouts before wave-24's 2 timeout-DRAWS (the first in 9 corpora); wave-25 had
1. Wave-26 returns to zero. The -T 3000 floor + the pool losing its two densest grind matchups
(109 aggro, 59 grind rotated out) absorbed the density: deck137's densest game (vs131, 63 LLM
calls / 2,250s) WON with no cap starvation. 4 cap-adjudications, integrity clean (34 gameend logs
= 42 - 8, exactly the 4 adj games x both seats + ... = clean).

**This is a THREE-SEAT EXIT wave -- the largest single-wave rotation to date (prior max 2,
wave-25's 59+109).** 27, 102, and 133 all rotate OUT converged with byte-frozen guides. The
campaign is visibly accelerating toward NEW-WORK EXHAUSTION at the guide/deck-review layer: three
seats reached it simultaneously, and a fourth (131) is projected for wave-27. Every rotate-in is
now a fresh guideless Step-0 newcomer (the previously-guided review roster is exhausted).

**A PRIOR FINDING WAS OVERTURNED (surfaced, not smoothed):** the deck199 probe (pre-corpus) found
Tergrid's steal ability NEVER PARSED for the entire campaign (ABILITYFACTORY NULL on the
`@sacrificed`/`@discarded` lines, poisoned by an adjacent `doubleside` line) -- overturning the
wave-25 deck102 read "the line is correct and un-fired, not broken." The 3-wave "zero steal
windows" validation debt was a real BUG. It is now card-script-fixed (faithful-Oracle rewrite +
trigger-line reordering, 3 fixtures) AND the may-ask render fixed (names the moved object + origin
zone + identifies Tergrid as SOURCE), and on the fixed build the model TOOK the steal. Steal chain
CLOSED end-to-end: parse -> fire -> legible ask -> correct model play. R-MAY-ASK-UNNAMED-OBJECT
CLOSED this wave. Lesson: a multi-wave "un-fired, structurally undischargeable" mechanic is a probe
candidate, and a probe can reveal it was broken all along -- do not assume un-fired == correct.

REPPENALTY=1.05 (4th corpus): KEEP -- same at every seat, no prose degradation.

## Per-seat verdict table (win column is NOT a verdict)

| deck | archetype / status | record | c1-c5 at seat | rotation | guide changed |
|------|--------------------|--------|---------------|----------|---------------|
| 133 | mono-B Devotion -- guided | 6/6 | c1 VALIDATED (7 removal, 0 trunc); c2/c3/c5 structurally untestable (fetches only); c4 = THE one retracted_choice (safe-direction, designed) | **ROTATE OUT** (converged; Arena edit held; Yawgmoth discharged; 6/6 = variance+pool-shift, no hidden tax) | N (BYTE-FROZEN, cmp-identical 10190 b) |
| 27 | UB Wither -- guided rd5 | 4/6 | c1/c2/c3/c5 N/A; c4 PASS (3 natural-stop ASK rescues + 2 truncated + 1 ATTACK, clean) | **ROTATE OUT** (converged; 3 clean ASK-seam b2 isolations resolve the wave-25 caveat; bottom clause UNEXERCISED, does not hold the seat) | N (BYTE-FROZEN, cmp-identical 10948 b) |
| 18 | Kithkin mono-W tribal aggro -- Step-0 debut | 4/6 | c1/c2/c3/c5 N/A at seat; c4 PASS (the illegal flyer-block was a CORRECT rules-drop, not a misfire) | **STAY** (Step-0; initial guide authored -> validate next corpus, deck59 pipeline) | Y (NEW initial guide, 5940 b) |
| 137 | GW convoke+Adventures -- guided rd4 | 3/6 | c2 VALIDATED (7 shockland menus self-describing -> CLOSE R-SHOCKLAND-ETB-UNLABELED); c4 PASS (11 reversals); c1/c3/c5 no exercising seat | **STAY** (guide modified: B-list-authoritative clause; live new-work = R-DUPLICATE-NAME-INSTANCE breach + closing-speed play gap) | Y (+453 b: 8038 -> 8491) |
| 22 | "Giants!" mono-R tribal ramp -- Step-0 debut | 2/6 | c1 VALIDATED (Inferno Titan enum); c3 VALIDATED (firebreathing delta); c4 clean (0 mis-picks / 74) | **STAY** (Step-0; initial guide authored; new-work = KEYWORD-TYPE-CONSEQUENCE face + R-CHANGELING-TYPE-INFERENCE) | Y (NEW initial guide, 5385 b) |
| 131 | UR Guttersnipe burn -- guided | 2/6 | c1/c2/c3/c5 UNEXERCISED; c4 gate PASS (0 fallbacks) | **STAY** (guide modified; near-terminal, projected wave-27 rotate; both wave-25 edits validated) | Y (+1103 b: 12161 -> 13264) |
| 102 | BW edicts+Tergrid -- guided rd4 | 0/6 | c1 VALIDATED (Thoughtseize opp-first); c4 VALIDATED (3 positive reversals); steal discharged by probe (pre-corpus) | **ROTATE OUT** (converged; 0/6 = pool-rise+variance, sound decisions; mulligan edit validated) | N (BYTE-FROZEN, cmp-identical 9545 b) |

Record notes: **deck133's 1/6 -> 6/6 is variance + pool-shift, NOT a decision leap** (Obliterator
deployed 5/6 games vs offered 0x in wave-25's three flipped losses; the 6/6-hides-taxes audit found
no deciding tax; two hardest matchups 109/59 left the pool). **deck102's 4/6 -> 0/6 is the mirror**:
pool-rise + draw-variance with sound decisions (full deciding-factor trace in its findings). **The
two Step-0 debuts land in the deck59 band** (18 at 4/6, 22 at 2/6) -- both produced genuine new-work
initial guides. **deck131's 0->1->2 across three waves is the SAME construction-terminal deck under
draw variance, not a trend** (its two wins are a variance flip vs102 + a free win vs un-guided
Step-0 deck22; 0/4 vs the guided field).

## Core prompt: PASS (21st consecutive) -- 7-of-7 seats

All seven seats route core to PASS; no general-prompt line added or cut. **Count reconciliation:**
deck27/102/131/133 numbered this "21st"; deck18/22/137 referenced the wave-25 bar ("20 consecutive")
and called it the 20th/candidate-20th -- a naming artifact (they held the wave-25 figure rather than
incrementing). Reconciled to **21st** per the wave-25 lineage (wave-24=19th, wave-25=20th,
wave-26=21st), consistent with the task brief. **Convergent REJECTION (deck102 + deck133, 2 seats):**
both independently considered and rejected a reversal-spiral / "reason THEN commit your CHOICE line"
core brake -- the spiral reaches the strictly better answer, c4 already absorbs the cost (0 misfires),
and the nudge would fight the answer-first design that floors decode-collapse spirals. WATCH only
(does a spiral ever reverse PAST the correct answer / into an out-of-range index). Below-bar core
candidates stay LEDGERED, not proposed: R-DUPLICATE-NAME-INSTANCE + "trust the offered set both ways"
(both gated on a SECOND DECK); affirmative untapped-blocker tagging (deck131, evidence did not
strengthen); answer-first self-coherence wobble (deck131 I-4 + deck133, model-side); universal
flying-block mechanics fact (deck18, single-seat).

## RANKED WAVE-27 ENGINE/CARD LEDGER (seat notes.md carry the seqs)

Ranked by the owner's standing rule (fixable perception/correctness flaws that suppress or corrupt a
LEGAL play go first), then game-cost, then card-script/oracle-fidelity, then cosmetic. Card-script
fixes precede engine changes (owner rule: express Oracle faithfully in card script before touching
the engine).

1. **R-DUPLICATE-NAME-INSTANCE (deck137, HIGH, representation, correctness-breach, corpus-general).**
   The ONLY converted CORRECTNESS breach this wave: deck137 vs18 s36 -- two same-named Lovestruck
   Beasts (one tapped, one untapped, NO instance handle), the engine correctly offered only the
   untapped copy as a legal block, but the model could not bind the offered B-line to the untapped
   instance, spiraled 10,817 ch re-deriving tapped state from the log, and DECLINED a free
   kill-and-survive block. Owner rule #1 class (perception suppresses a legal play). Fix: give
   same-named permanents a stable per-instance handle in the battlefield list AND in every
   A/B/target/activation option line. Corpus-general by construction (any deck with 2+ same-named
   creatures or same-named tokens in mixed states). Single-witness so far; the deck137 guide clause
   ("the offered B-list is authoritative -- do not re-derive tapped state from the log") is the
   INTERIM, the instance handle is the durable fix. **The wave-27 rotate-in pool is chosen to force
   the 2nd witness immediately (Relentless Rats = 36 identically-named bodies; goblin/token decks).**

2. **R-CHANGELING-TYPE-INFERENCE (deck22, MED-HIGH, representation, quality, GAME-COSTING).** A
   changeling body renders `[changeling] "This card is every creature type."` but the pilot does not
   infer the tribal-role consequence -- it mulliganed keepable hands and bottomed its own enablers
   citing "no other Giant" while holding Universal Automaton (a Giant). Cost GAMES at deck22 (vs131
   s1/s4, vs18 s1), not just latency. Fix: on a changeling permanent/spell, when the deck or a
   battlefield card has a type-keyed restriction/anthem/count, annotate the changeling option/
   battlefield line with the relevant type (`[changeling: counts as Giant]`). Interim = the guide
   rule-#1 identity line. Single-seat; recurs for any changeling/tribal deck.

3. **Card-script fixes: Zealous Guardian + Thistledown Liege (deck18, MED, oracle-fidelity, cheap
   data edit -- but CO-DEPENDENT with a deck18 guide revision, FLAG).** Engine-vs-Oracle divergences
   in `primitives/mtg.txt`: **Zealous Guardian** = engine `{W/U}` 1/1 Kithkin Soldier flash, NO
   defender vs real `{W}` 2/2 Defender w/ flash (the DROPPED defender materially changes the role --
   a Defender cannot attack); **Thistledown Liege** = engine 1/3 vs real 4/4 (base P/T, lord function
   intact). No LIVE misplay today because the deck18 guide correctly teaches ENGINE behavior. **CO-
   DEPENDENCY:** fixing these INVALIDATES deck18 guide lines -- Zealous Guardian is taught as a
   Soldier ATTACKER (lines 31/33) and cast off Plains via its `{W/U}` pip (lines 15-16); a `{W}` 2/2
   Defender neither attacks nor uses a hybrid pip. **If seed 3 lands at wave-27, deck18's guide MUST
   be revised in the SAME step** (remove Zealous Guardian from the attacker/Soldier lists and the
   hybrid-castability line; correct Thistledown's 1/3). Ship them together or not at all.

4. **doubleside parse-poisoning GENERALITY + Aclazotz verify (probe, MED, correctness, cheap
   grep+fixture).** The Tergrid steal was DEAD campaign-wide because a battlefield `doubleside(...)`
   line (NULL-by-design) poisoned the parse of the NEXT ability line -- a silent-ability-drop class.
   Now fixed for Tergrid by trigger-line reordering. Aclazotz is a same-structure suspect (unverified);
   any other card interleaving a doubleside line above a real ability could be silently broken. Cheap
   to sweep (grep primitives for `doubleside` with a following ability line) + guard with a fixture.

5. **R-DFC-BACKFACE-RESOLVES-FRONT (deck102 carry, MED, correctness, benefit-side; LIVE-PROBE now
   warranted).** Cast Tergrid's Lantern `{3}{B}` back face -> engine deploys Tergrid, God of Fright
   `{3}{B}{B}` FRONT permanent (paid the cheap cost, got the better card). Fix map on ledger
   (AllAbilities.cpp:5422 resets isFlipped for AI controllers; autostack flip keys on
   paid(alternative) a normal cast never sets); un-re-exercised 4th consecutive wave in the pool.
   **Assessment (brief's question): a live-probe-verified fix is NOW warranted** -- the deck199 probe
   harness reproduced the bug twice and is proven, so the "no suite coverage" blocker is softened:
   build the fix -> deck199 (or a synthetic `WAGIC_TEST_PRIMITIVES_FILE` DFC card) probe -> observe
   the correct back-face deploy, and add the synthetic-DFC suite fixture in the same change. Touches a
   shared DFC/MDFC cast path (regression risk) -> gate on the live probe + fixture, do not ship blind.

6. **primitives-only card + real set hint = silent drop (MTGDeck.cpp:1043, LOW-MED, robustness,
   loud-rejection candidate).** A card that exists only in `primitives/*.txt` (INTERNAL_SET) is
   silently dropped from a main deck when the decklist gives a real set hint (setId != hint) -- caused
   the deck199 game-1 dud (6 Tergrid copies vanished). Fix: loud rejection (log the dropped card)
   instead of a silent drop. deck102's `(KHM)` works only in the `#CMD:` commander slot (different
   load path).

7. **ETB standalone-menu empty subject (deck137, LOW, cosmetic residual).** c2 CLOSED the option-level
   annotation (7 shockland menus now self-describing: `pay 2 life [... enters UNTAPPED - usable this
   turn]` / `tap [... enters TAPPED ...]`, all parsed, no double-annotation). Only leftover = the
   `Choose an option for :` empty-subject line on the standalone case. Fill the empty menu subject
   with the entering land's name; pure cosmetic, no observed harm.

8. **N-18c Mobilization activated-ability discoverability (deck18, LOW, interface, single-seat).** An
   enchantment's activated ability (`{2}{W}: make a Soldier`) surfaces only at priority, not in the
   main-phase cast list; the pilot burned 3.5k ch concluding "the interface doesn't offer an Activate
   button" before finding it at upkeep priority. Tax only, no game lost. Interim = the guide interface
   note. Durable angle if it recurs across enchantment/artifact activated-ability decks: surface an
   activatable permanent's ability in the SAME decision window as casts when payable, or narrate that
   activated abilities appear at priority.

9. **retracted-recode PLAN-corroboration discriminator (deck133, LOW, parser, SAFE-direction, offered
   not urged).** The one retracted_choice differs from the harmful answer-then-drift shape only in
   that HERE the `PLAN:` action corroborates the RECODE, not the line-1 index. Candidate: when a
   natural-stop reply's line-leading coded index contradicts its own PLAN action while a non-line-
   leading recode matches the PLAN, prefer the recode. Requires fragile PLAN-prose -> option-action
   matching the parser deliberately avoids, and the harm-shape is structurally indistinguishable from
   the case it protects against -- **argues AGAINST loosening.** 1/1,322, non-deciding, safe-direction.

**Carries (unexercised / un-re-exercised / WATCH):**
- N8 baka 0-power blocker gang-chump heuristic -- UNEXERCISED (6th corpus; answer-first structurally
  prevents block fallbacks at answer-first seats; needs a probe forcing a block fallback with a
  valuable 0-power body).
- ENGINE-R4 Azcanta ACTIVATION probe -- orphaned (a model-elected top-4-to-hand transform pick
  unobserved; control-deck probe).
- undying trade-annotation -- sibling of the shipped persist return clause; extend when an undying
  deck enters the pool.
- mulligan surface now 3 corpora unexercised (deck27 zero mulligans this corpus; the bottom-phase
  rule-#1 clause carries UNVALIDATED, like the deck27 durdle off-case at 7 corpora).
- commit-lock regression WATCH (b2 shipped + validated; watch for an answer-first re-freeze via the
  `choice`-field check).
- prose-only answer reversal convergence (parser b2-extension WATCH -- take the prose conclusion when
  it clearly supersedes an earlier coded line on a natural-stop reply; correct per current spec).
- affirmative untapped-blocker tagging (deck131, below-bar, 0 recurrence 4th corpus).
- Guttersnipe tapped-read misread (deck131 single-seat WATCH, 0 recurrence this corpus).
- answer-first self-coherence wobbles (deck131 I-4 + deck133; model-side, non-decisive, NOT parser
  misfires -- do not report as c4 defects).
- Bloodghast forced-sac fodder line UNEXERCISED (no edict opponent at deck133).
- Guttersnipe all-tapped/Guttersnipe WATCH; harness single ~200s per-game latency outlier under -j3
  (server-saturation queue event, not actionable at review layer; noted for concurrency tuning).

**Recommend CLOSE this wave:**
- **R-SHOCKLAND-ETB-UNLABELED** -- c2 VALIDATED (7 self-describing menus, deck137); only the cosmetic
  empty-subject residual remains (demoted to ledger item 7).
- **R-TRUNCATED-TARGET-PREVIEW** -- c1 VALIDATED at 3+ seats (deck102 Thoughtseize opp-first, deck22
  Inferno Titan enum, deck133 7 removal decisions, all 0 truncation, no prompt-length side effect).
  This was the wave-25 HIGHEST-priority perception fix; closed.
- **R-RESTLESS-PUMP-OPTION-AMBIGUOUS** -- c3 VALIDATED (deck22 firebreathing `+1/+0 until EOT (6/6 ->
  7/6)`, no stat-conflation; the wave-25 render fix shipped and killed the conflation spiral).
- **example-leak / wave-24 parser-hardening ship-decision** -- c4 SHIPPED + corpus-gated; note it is
  LIVE and working (it surfaced the deck133 retracted_choice AS its designed safe-direction behavior).
- **Yawgmoth SacrificeCost acceptance** -- DISCHARGED (5 clean payable activations at deck133, no
  unpayable-cost class).
- **R-MAY-ASK-UNNAMED-OBJECT** -- CLOSED this wave (c5 probe FINAL PROBE; steal ask now names the
  moved object + origin zone + Tergrid as SOURCE, model took the steal).

## Rotation reconciliation

Standing test: candidate for rotation OUT when the guide takes NO substantive modification AND no
seat-specific new-work signal survives. Verdicts are UNSENTIMENTAL; termination gates on NEW-WORK
EXHAUSTION at the guide/deck-review layer (deck supply is unbounded).

- **27 -- ROTATE OUT (converged).** Guide BYTE-FROZEN (cmp-identical to deployed). Both losses
  (133/18) matchup/dead-anyway, no misplay; no engine/parser bug; c4/b2 clean; the 3 ASK-seam b2
  isolations RESOLVE the wave-25 caveat (closed, not new work). The bottom-phase rule-#1 clause was
  UNEXERCISED (zero mulligans) -- per the reframed doctrine an unexercised low-frequency clause does
  not hold the seat.
- **102 -- ROTATE OUT (converged).** Guide BYTE-FROZEN. The 0/6 decomposes entirely to pool-rise +
  draw-variance with sound decisions (deciding-factor trace); the mulligan edit's first corpus
  VALIDATED clean (6/6 correct keeps, over-mulligan class did not recur); the steal chain is
  DISCHARGED by the probe (pre-corpus). Remaining opens are ENGINE/master-ledger (R-DFC-BACKFACE,
  set-hint drop), not per-wave guide work.
- **133 -- ROTATE OUT (converged).** Guide BYTE-FROZEN. Arena edit HELD (0 low-life casts, facts
  cited -- weak-by-construction but clean, precedent-consistent with the b1/deck59-109 rotations);
  Yawgmoth DISCHARGED (round 2); the ONE new signal (retracted_choice) is a general model x parser
  HARNESS-lane item worked from the ledger regardless of pool membership, not deck133-bound. 6/6 is
  partly pool-inflated (109/59 gone) but the construction flags are upstream/owner-lane, not guide-
  fixable.
- **18 / 22 -- STAY (Step-0 debuts; Step-0 seats never rotate).** Both authored genuine new-work
  initial guides.
- **131 -- STAY (guide MODIFIED).** Favorable-block exception + lethal-attack reconciliation; a
  revised guide must re-validate. Projected wave-27 ROTATE: the never-block absolute is now fully
  recalibrated (survives-block + lethal-chump + never-trade), the bottom ladder complete, the
  construction ceiling fixed -- guide-work is near-exhausted.
- **137 -- STAY (guide MODIFIED + live new-work).** B-list-authoritative clause (from the s36 breach);
  live new-work = R-DUPLICATE-NAME-INSTANCE (ledger #1) + the CLOSING-SPEED deck-layer play gap (2 of
  3 wins adjudicated-ahead; late-stabilization losses). Its COMPREHENSION surface has converged; its
  PLAY quality (curve/sequencing/commit-timing) has not -- that keeps it in.

**Converged CANARY list grows to 15:** 110, 21, 17, 14, 35, 49, 135, 62, 44, 140, 59, 109, **27,
102, 133** (the three added this wave).

### Rotate-in picks (3 slots -- fill the wave-27 pool back to 7)

Wave-27 holdovers = 131 (UR Guttersnipe burn), 137 (GW convoke/adventure), 18 (mono-W Kithkin tribal
anthem-aggro), 22 (mono-R Giants tribal ramp). The exiting seats (27 UB Wither control, 102 BW
edicts/Tergrid, 133 mono-B devotion) take BLUE, BLACK, CONTROL, edicts/steal, and devotion with them,
leaving those gaps. Picked for archetype coverage ORTHOGONAL to the four holdovers AND to maximize
UNEXERCISED engine-surface coverage (may-asks, ETB choices, activated abilities, tokens/duplicates
for R-DUPLICATE-NAME-INSTANCE). Commander decks EXCLUDED (singleton/highlander + the `#CMD` slot is a
different load path than the campaign's 60-card constructed pool). Decklists read to confirm.

1. **deck134 "Urza's Control" -- mono-U artifact/tron CONTROL.** Counterspell, Condescend, Impulse,
   Mystical Teachings, Forbidden Alchemy, Mulldrifter, Ghostly Flicker (ETB-blink), Mnemonic Wall,
   Stonehorn Dignitary, Expedition Map, Prophetic Prism, Urza tron. Fills the BIGGEST holes (BLUE +
   CONTROL, both absent after 27 leaves) and exercises the most unexercised surfaces at once: ETB
   choices / blink may-triggers (Ghostly Flicker -> re-ETB Mulldrifter/Stonehorn/Mnemonic Wall),
   artifact activated abilities (Expedition Map, Prism), and instant-speed response windows
   (counterspells). Zero overlap with any holdover (all four are creature-aggro).

2. **deck93 "Relentless Rats" -- mono-B swarm (36 Relentless Rats + 24 Swamp).** THE duplicate-name
   deck: every creature is identically named and scales with the count, guaranteeing many same-named
   bodies in MIXED tap states at every combat -- the maximal stress test for R-DUPLICATE-NAME-INSTANCE
   (ledger #1, the witnessed correctness breach) and the intended 2nd witness that would graduate it
   to a core proposal. Also fills BLACK and exercises trust-the-rendered-number (Rats' P/T scales).
   Deliberately targets the highest-priority ledger item with the deck that stresses it hardest.

3. **deck136 "Yarok of Pranks" -- mono-B planeswalker midrange.** Rankle Master of Pranks (modal
   MAY-ASKS), Liliana Dreadhorde General + Ugin the Ineffable (planeswalker LOYALTY activations -- a
   wholly UNEXERCISED activated-ability class; no planeswalker deck has been in the pool), Cavalier of
   Night / Midnight Reaper / Yarok's Fenlurker (ETBs), Murderous Rider / Foulmire Knight (adventures),
   Command the Dreadhorde (mass reanimation). Covers the surfaces 134/93 leave open (may-asks +
   planeswalkers) and keeps the freshly-fixed c5 may-ask render under exercise. Distinct archetype
   from 93 (degenerate swarm vs planeswalker midrange) despite the shared color.

Trade-off named honestly: 93 and 136 are both mono-black, a minor color redundancy accepted because
their archetypes are maximal opposites and together they cover black's two poles while 134 brings the
blue/control axis; the surface-coverage gain (duplicate-swarm + may-asks + planeswalkers) outweighs
the color overlap. DFC is the one named surface left uncovered -- acceptable: R-DFC-BACKFACE is a
deferred engine bug best exercised by a targeted probe (ledger #5), not by forcing a niche DFC deck
into the pool.

**Remaining guideless count:** 146 guideless decks currently sit in `bin/Res/ai/baka/` (deckN.txt
with no `_strategy.txt`), INCLUDING 18 and 22 (their wave-26 initial guides are not yet deployed to
live Res). Once the wave-26 guides for 18 + 22 deploy (step 4), the guideless pool drops to **144**.
The three wave-27 rotate-ins (134, 93, 136) are drawn from those 144 and will author their initial
guides at wave-27, dropping it to **141**. Deck depletion remains NON-binding; NEW-WORK EXHAUSTION is
the termination constraint. This wave still produced heavy new-work (2 promoted/extended Method
headlines, 2 new rungs, a full 9-item ranked ledger + 6 CLOSEs, 2 initial guides, 2 revisions, 3
rotations), so the loop is NOT terminating -- but the three-seat exit is the clearest acceleration
signal yet.

**WAVE-27 POOL: 131, 137, 18, 22, 134, 93, 136** (27 + 102 + 133 rotated OUT to canary; deck134
Urza's Control + deck93 Relentless Rats + deck136 Yarok-of-Pranks black-midrange rotate IN as
guideless Step-0).
