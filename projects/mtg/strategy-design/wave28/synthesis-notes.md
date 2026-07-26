# Wave-28 synthesis notes (FOURTEENTH per-deck-rotation cycle)

Corpus `matchups-20260726-050449`, binary `ad8930961`, -T 3000, REPPENALTY=1.05 (6th corpus). 21 games,
0 timeouts / 0 draws, 4 cap-adjudications (34 gameend logs = 42-8, integrity clean). 1,072 decisions,
18 fallbacks (1.7%). Retry 0.

## Verdict table

| Seat | Archetype | Rec | Guide action | New work | Rotation |
|------|-----------|-----|--------------|----------|----------|
| 18 | Kithkin mono-W anthem-aggro | 6/6 | BYTE-FROZEN | none (L-18-1 + N-18e -> ledger) | **OUT (converged, 2nd deck59 EXIT arc)** |
| 22 | "Giants!" mono-R tribal ramp | 3/6 | BYTE-FROZEN (2 corpora) | none (echo-parse CLOSED) | **OUT (converged, final)** |
| 93 | Relentless Rats mono-B aggro | 4/6 | frozen (validated) | N-93a summoning-sick tag (MED) | STAY (near-terminal) |
| 134 | mono-U/G Urza tron control | 0/6 | frozen (correct; terminal) | construction-only | **OUT (CONSTRUCTION-TERMINAL -> owner; separate list)** |
| 136 | Yarok-of-Pranks BW walker-midrange | 2/6 | REVISED (MV-fabrication fix + more) | Ugin/Rankle/-4 unexercised | STAY |
| 137 | GW convoke + Adventures | 3/6 | REVISED (convoke softening) | N-137 March X-convoke (HIGH) | STAY (sole convoke post) |
| 146 | Orzhov Dungeons WB (Step-0) | 3/6 | NEW initial guide | N-146a window gap (HIGH) + venture faces | STAY (Step-0 debut) |

Core (general) prompt: **PASS x23** (7/7 seats). deck18/134/146 mislabeled it "22nd" holding the wave-27
count; reconciled to 23rd (wave-26=21st, wave-27=22nd, wave-28=23rd).

## The corpus story

**The e-batch split-decides.** e1 convoke fix = PARTIAL: Venerated Loxodon fixed-cost convoke now resolves
end-to-end (live vs93 trace -- chosen freely, arrives, blocks as a 4/4; 0 defers vs wave-27's 23), but March
of the Multitudes X-convoke is STILL 0/17 tokens with all 16 defers, and the resolution-rung caught a NEW
mechanism detail -- the PLAIN X-cast also fails (cascades back into the broken convoke sub-path), so the wave-29
fix target is the X-ANNOUNCEMENT remainder float, not the convoke choice. e2 phantom-chooser SOLVED->RETIRED
(card-script fix verified 3 ways; 6 clean Legion's End casts; the refusal-tax class is gone). e3 flip-thrash
livelock RESOLVED (probe-verified end-to-end; no Kaldheim DFCs in the main pool -> steady-state watch). e4 land
tags KILLED deck93 mulligan land-blindness (6/6 snap-keeps, guide-cited). e5 echo-tolerance CLOSED and
GENERALIZED (5 echoes incl. a new `[land]` shape, all parsed clean, 0 fallbacks).

**The e4 double-lever nuance (guide-attributed at 93 vs annotation-attributed at 22).** deck93's land-blindness
kill is GUIDE-attributed (the mulligan replies cite the guide RULE, not the tag), while deck22's changeling
kill was ANNOTATION-attributed (the model cited the tag directly). deck93 supplies the isolation deck22 could
not: L2 phantom attackers is the SAME perception-leak class treated with guide prose ONLY and it SURVIVED
(11/15 over-lists). Two same-class leaks, one annotated (died) one not (survived) = the natural experiment
behind the promotion-grade perception-vs-strategy routing rung (skill headline 1).

**The exit wave.** deck18 completed the SECOND full Step-0->guided->validated->EXIT arc (after deck59): 6/6 on
a frozen guide, all three targeted leaks re-validated on a corpus the author did not touch, every new signal
routed off the guide. This promotes the frozen-guide AUDIT corpus as the positive proof-of-completion gate --
and deck18's 6/6 STILL hid a worsened latency tail and one truncation->stale-line slippage, the literal meaning
of "6/6 hides taxes." deck22 exits alongside it (guide frozen 2 corpora, all teaches re-validated, the record
drop 5/6->3/6 fully explained by pool-strengthening + variance with every deck22 decision correct).

**The fabrication incident.** deck136's wave-27 guide invented Relentless Rats = MV4 (engine `mana={1}{B}{B}`
= MV3), inverting BOTH removal teaches: it benched Ritual of Soot (the real MV3 answer) and recommended
Legion's End (targets MV<=2, cannot hit Rats). The pilot spent five turns planning the impossible line and
self-corrected against its own guide mid-game. FOURTH hallucinated card fact of the campaign, FIRST shipped
inside a DEPLOYED guide -- invisible in the win column, visible only in PLAN prose. Fixed this wave; promoted
to a verify-oracle HARD RAIL at the guide-DRAFTING layer (skill headline 3).

**The new terminal category.** deck134's guide WORKS (Map crack 6/6 first-window, tron 67-100%, zero bad
evokes, fogs held on own turns and fired on lethal, the lock teach lands cognitively) and the deck STILL cannot
win (untutorable 2-card lock never reached a castable hand in 6 games; no clock so flyers correctly block and
never attack; cap-adjudication scores a clockless control deck a loss). CONSTRUCTION-TERMINAL: the guide
instrument hit its ceiling in one corpus -- flagged to the roster owner, NOT iterated further, and kept on a
SEPARATE list from converged canaries (the guide did not win; the deck lost -- keep the distinction).

## RANKED WAVE-29 LEDGER

Top 5 (work these between waves):
1. **N-137 March X-convoke non-resolution [HIGH, engine].** March 0/17 tokens; the plain X-cast ALSO cascades
   into the broken convoke sub-path -> the fix must handle X-ANNOUNCEMENT remainder float from convoke, not
   re-run the Loxodon-style fixed-cost fix. deck137 is the SOLE observation seat. Highest-value engine item;
   its death is the mechanistic root of both deck137 losses. Repros: vs18 seq21-24/27-31, vs136 seq30-33/36-39,
   vs134 seq21-44 (10 defers t4-t8).
2. **N-146a no cast/priority window on the back foot [HIGH, engine/harness -- REPRO FIRST].** In both deck146
   blowouts the model cast NOTHING all game -- received ONLY land-drop asks, never a cast/priority window, while
   holding castable Vanishing Verses (with legal targets) and a deathtouch blocker, and died. Hypothesis:
   FindCardToPlay/computeActions not re-invoked in the same main phase after a land drop resolves. MUST be
   reproduced on the current binary before any fix (pin deck146 on the draw vs an aggro deck with a castable
   2-drop + instant; confirm from the translog whether a cast ask ever follows a land-drop ask in one main
   phase). Repro files: 1785068327-...-vs-deck18.jsonl (seq3+), ...-vs-deck93.jsonl. A priority/ask-issuance
   suspect -- the single largest Step-0 result, UNFIXABLE by guide prose.
3. **N-93a summoning-sick board tag [MED, engine/representation -- infrastructure exists].** No summoning-
   sickness marker on the board render; sick Rats render identically to attack-ready ones, so the model counts
   the board (11/15 attack over-lists; plausibly cost the vs18 game). Add a per-creature `[summoning sick --
   cannot attack this turn]` tag (same emit path as the existing `[tapped]` tag). Expected to kill the L2
   phantom-attacker class by the land-tag analogy. Repro: vs18 seq18, vs134 seq8, vs22 seq24.
4. **N-146b dungeon room-branch effect text [MOD, engine/render -- deciding-fact-rides-the-option].** The
   room-BRANCH advancement menu (the actual strategic venture decision) shows ROOM NAMES ONLY, zero effect text
   (deck134 seq36 "1. veils of fear / 2. oubliette"), while the dungeon-selection and effect-modal faces DO
   carry text. Annotate each branch option with its room effect (the definitions are already shown truncated in
   selection and full in the effect modal).
5. **N-18e truncation -> stale-line fallback [LOW->watch, harness].** A ceiling-truncated reply with no
   terminator falls back to a stale early coded line that CONTRADICTS the reply's own conclusion (vs93 seq20 --
   blocked after concluding do-not-block; silent because it parsed legal and the game was won). Fix candidate:
   finish_reason=length AND no terminator after the last coded line -> safe phase default (blockers: "no
   blockers") or bounded re-ask. Compounds with L-18-1 (model non-termination near the ceiling).

Then: N-146c (dungeon-selection room text truncated + mis-framed as targeting), N-146d ("Mana available"
collapses a dual land to one color), N-146e (dungeon-completion 309.5b UNCONFIRMED -- probe rec), N-134b
(Moment's Peace flashback option shows base {1}{G} not flashback {2}{G} cost -- REPRODUCED), N-134d (Mulldrifter
double cast-mode ask -- CONFIRMED, benign this wave, de-dupe candidate), 'Flip Side' menu no-op suppression
(sanctioned no-op class, AIPlayerGPT owner), Rankle combat-damage may-ask WATCH (deck136, unverified when
Rankle connects), L-18-1 planning-tax migration (MODEL, cross-seat), then carries.

CLOSE list (discharged this wave -- do NOT re-open):
- **R-RESOLVED-SPELL-PHANTOM-CHOOSER (e2)** -- RETIRED; card-script fix verified 3 ways, 6 clean casts, 0 seat
  fallbacks.
- **R-ANNOTATION-ECHO-PARSE (e5)** -- CLOSED; 5 echoes incl. new `[land]` shape all parsed, 0 fallbacks;
  generalized past the shape it was built for.
- **N-A land-blindness class** -- DEAD; e4 land tags, 6/6 keeps, guide-cited.
- **Loxodon fixed-cost convoke (e1 half)** -- CLOSED; live end-to-end resolution trace (chosen, arrives,
  blocks). (March X-convoke stays OPEN -- see ledger #1.)
- **R-FLIPTHRASH-LIVELOCK (e3)** -- RESOLVED; probe-verified, both games natural-end, 0 isFlipped oscillation.
- **N-18c (Mobilization discoverability)** -- exercised fluently, stays closed.
- **N-134a (Moment's Peace {5}{G} claim)** -- remains struck (wave-27 verify-first reversal; engine {2}{G}
  flashback is correct).

## Cross-seat contradictions / reconciliations called out plainly

- **Core-PASS counting drift.** deck18/134/146 wrote "22nd"; deck22/93/136/137 wrote "23rd". Reconciled to
  **23rd** (wave-26=21st, wave-27=22nd). The three low-counters held the prior total instead of incrementing;
  not a substantive disagreement.
- **Perception-vs-strategy is the resolution of the wave-27 "double-lever" ambiguity.** deck22's wave-27 kill
  could not isolate guide-from-annotation (model cited the guide). deck93 resolves it with a within-wave
  negative control (L2, guide-only, failed) -- so the deck22 face and the deck93 rung do NOT conflict; deck93
  supplies the isolation deck22 lacked. No contradiction, a completion.
- **"First-guided record delta" cuts both ways and is NOT a guide verdict.** deck136 dropped 4/6->2/6 and
  deck18 rose 5/6->6/6 in the SAME round-robin; both are largely opponent/pool-driven (shared-fate). Do not
  read deck136's drop as a guide regression or deck18's rise as guide-only -- decompose swings (skill headline
  6). deck22's 5/6->3/6 drop is the same shape and correctly did NOT hold the seat.
- **deck134 is OUT but NOT a converged canary.** The loop default rotates it out, but it must NOT join the
  canary list -- the guide did not win, the deck lost. It goes to a separate construction-terminal list flagged
  to the roster owner. Conflating the two would corrupt the canary set's meaning (converged = guide works AND
  wins).
- **Layer-routing held under three temptations** (N-146a loss column, N-137 March death, N-93a phantom
  attackers): none became guide/skill/core prose -- all are engine/representation items. A model never ASKED to
  cast cannot be taught to cast; a perception surface that contradicts the guide wins over the guide.
