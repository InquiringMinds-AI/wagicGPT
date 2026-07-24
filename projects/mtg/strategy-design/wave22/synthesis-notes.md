# Wave-22 synthesis notes — EIGHTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-22 PRIMARY corpus
`matchups-20260723-173843` (21 games round-robin over 135/62/27/102/109/44/140, binary `/tmp/wagic-1d8ed8194`).
**The pool SWAPPED** (49/35/14 rotated out to canary last cycle; 109 Mono-R Aggro / 44 Faerie Tempo / 140 RBW Control
rotated IN as multi-generation-old-guided re-validations), so cross-wave record comparison is WEAK evidence this cycle
— the pool swap changes matchup difficulty for everyone, and the newcomers have no same-pool baseline. Binary
1d8ed8194 carried the wave-22 step-1 batch (ENGINE-F1 fetch fix + HARNESS-N9 reference-scoping + echo (c) fixes +
ENGINE-R6 transform annotation + R-PAINLAND surfacing); THIS CORPUS is that batch's live validation.

Corpus health: fallbacks **0.6% (8/1368)** — composition: **2 retracted_choice (BOTH CORRECT fires — deck102 owns
both, real self-disavowals; the N9 over-fire class is closed), 6 stale_echo, 0 unparsed, 0 empty.** 9th consecutive
clean corpus (0 segfaults, 0 timeouts, 0 draws, **8 life-adjudicated at cap** — audited legitimate: turns 10-35,
31-67 decisions/seat, deck140-the-grinder in 5 of 8; latency median 16.9s p90 77.6s at j3). Per-seat fallbacks:
**140:4 (all stale_echo, HARM=0), 102:3 (2 retracted correct + 1 stale_echo), 135:1 (stale_echo, HARM=0); 27/62/109/44:
0.** Decision kinds: ask 827, priority 270, attackers 143, blockers 38, reveal 22 (135:20, 102:2); 0 prose salvages
fired at the CHOICE seam. **Core (general prompt): PASS, 17th consecutive.** Skill METHOD gained rungs (crutch-lifecycle
4-seat maturation; rotation-in-as-new-leak-hunt; FP-suppression-fix validation; attacker declared-vs-recorded scoring;
answer-first commit-lock guide-authoring rung) — guide-authoring/reviewer-instrument method, which does not reset the
core PASS count.

## Per-seat verdict table

| deck | archetype / status | record (Δ, context-only) | fallbacks | guide disposition | rotation |
|------|--------------------|--------------------------|-----------|-------------------|----------|
| 135 | Modern Snow · frozen veteran | 4/6 (↑ from 1/6) | 1 (stale_echo — transform dual-face FP, HARM=0) | **FROZEN** (byte-identical, `cmp`-verified) | **ROTATE OUT → canary** (owned charge cleared) |
| 62 | Enchantresses · frozen round 6 | 4/6 (flat) | **0** (was 4) | **FROZEN** (byte-identical, `cmp`-verified) | **ROTATE OUT → canary** (N9 witness discharged) |
| 27 | Zombies UB · revised round 3 | 1/6 (↓ from 5/6) | **0** | **REVISE** (+6 lines: free-kills-first block-allocation) | STAY (durdle unvalidated 3rd corpus + commit-lock new-work) |
| 102 | Tergrid mono-B · revised round 2 | 2/6 (↓ from 3/6) | 3 (2 retracted CORRECT + 1 stale_echo DFC) | **REVISE — ALREADY DEPLOYED live** (2 edits, verified) | STAY (keystone 0/18, R-DFC-FLIP blocks it) |
| 109 | Mono-R Aggro · rotated-in | 3/6 (no baseline) | **0** | **REVISE** (crutch-RETIREMENT: hybrid + cost engine-carried) | STAY one more (confirm retirement not load-bearing) |
| 44 | Faerie Tempo UB · rotated-in | 4/6 (no baseline) | **0** | **REVISE** (crutch-demotion + DELETE + Cruel-Edict clause) | **ROTATE OUT → canary** (re-validation discharged) |
| 140 | RBW Control · rotated-in | 3/6 (no baseline) | 4 (all stale_echo, HARM=0) | **REVISE** (crutch-demotion + NEW forced-discard rule) | STAY one more (validate new rule + top-ledger witness) † |

† **Synthesis MODERATION of deck140's reviewer verdict** (reviewer said rotate-out-but-deploy-first). Reconciled to
STAY-one-more for consistency with the wave-21 precedent (an UNVALIDATED guide revision keeps the seat — cf. 27's durdle
revision and 102's edits both held on that basis) AND because deck140 is the WITNESS for the corpus's #1 engine ledger
item (forced-self-discard, game-deciding, 2-seat convergent): holding it keeps that seam exercised at-seat (102 discards
140, 8 windows last corpus) while the engine fix is pending, and validates the interim forced-discard guide rule — the
same "one confirming corpus" logic the 109 reviewer applied to itself. This is a NAMED, specific hold, not a manufactured
one. See Rotation decisions.

Record note (win column is NOT a verdict; per Step 0 it is context-only): the pool swapped, so records isolate to
matchup + draw. **deck27 1/6 is EXOGENOUS** — guide frozen (bar the +6-line edit), tax 0, ZERO exposure to any batch
item (no fetch/N9/echo/transform/painland at this seat); the swing is pool difficulty (two of its worst archetypes,
mono-R aggro + lifegain-control, arrived at once) + 3 continuing-opponent flips all with clean variance causes.
**deck135 4/6 splits honestly** between the ENGINE-F1 fetch fix (its wave-21 1/6 was fetch-fizzle incidence; the fix
resolved 10/10 cracks) and pool change; do not credit the guide. Newcomers 44/109/140 have no same-pool baseline.

## Batch-item validation (brief items a–f)

- **(a) ENGINE-F1 fetchland-fizzle FIX — CLOSE (deck135 owns).** Detector run on EVERY fetch-crack across 6 games: 10
  cracks, 10 clean resolutions, 0 fizzles; the exact 2016-guard trigger (crack after a same-turn land drop from hand) is
  directly confirmed on two cracks (vs27 s3 Misty Rainforest, vs140 s3 Windswept Heath — the wave-21 vs35 s6 repro shape,
  now resolving). ENGINE-F1b (silent fizzle) never arose (no fizzles to perceive). Regression-watch only next corpus.
- **(b) HARNESS-N9 retraction detector reference-scoping — CLOSE (the over-fire class).** Witness deck62: the wave-21 FP
  shape (single first-line CHOICE + downstream "Wait,"/"Actually,") recurred **27× — 10 in the EXACT land-drop FP
  context** — and ALL parsed clean; 0 fires at deck62 (was 3), genuine revise-and-replace preserved (vs27 s16 second
  coded line → last-wins). The **2 remaining corpus retracted_choice are BOTH at deck102 and BOTH CORRECT FIRES** (real
  self-disavowals): s21 vs62 (`CHOICE: 4 Smallpox` first, body abandons it dozens of times, truncates, no PLAN); s29
  vs140 (`CHOICE: 1 Arcane Signet` first, body commits to Mox Jet repeatedly, never re-commits, truncates). Both correctly
  fell to heuristic. N9's over-fire is closed; the route now fires only on true disavowals.
- **(c) Echo (c) source-prefix-strip + consumePlan label-filter — OPEN (family NOT closed; escalated).** 6 stale_echo
  this corpus. The (c) fixes target the `targeting` token (target menus) and combat-line label-hijack; they do NOT cover
  the three NEW label-mismatch shapes below (reconciliation a). The wave-21 target-menu spell-prefix shape was UNTESTED
  (0 target-menu reveals hit a stale_echo; deck62 had 0 reveals/stale_echo). Family escalated with an INDEX-WINS root fix.
- **(d) ENGINE-R6 transform-eligibility annotation — CLOSE (deck135 owns).** `Transform:azcanta [available NOW - ...]`
  renders verbatim; **3/3 transform windows transformed** (vs140 s24 cites the annotation, vs44 s36, vs27 s27
  intended-accept). Wave-21's 0/2 decline (manual-graveyard-recount vacuum) is GONE — the model now trusts the offer.
  Re-confirms the thesis: annotate the OFFER > add prompt text. Side-effect: the confident accepts exposed
  HARNESS-ECHO-Transform (dual-face echo) — a NEW harness shape, folds into the echo family.
- **(e) R-PAINLAND self-damage surfacing — CLOSE (deck102 owns).** Both durable surfaces render (mana-line CAUTION +
  `Ancient Tomb [tapping for mana deals 2 damage to its controller]` battlefield tag); the wave-21 self-death class
  STOPPED (vs135 PASSED at 4 life across 3 priority windows instead of self-tapping; vs44 survived 4 Ancient Tomb taps to
  adjudication). Guide crutch DEMOTED, and the now-FALSE clause ("the Mana available line does NOT warn you") DELETED.
- **(f) Carried wave-21 items — holding, one still unexercised.** Answer-first: VALIDATED holding cross-seat (0 unparsed /
  0 empty over 1368; coded-first line 100% of sampled p95+ replies — deck44 192/192, deck62 149/149, deck27 135/135,
  incl. 15.8k-char rambles that locked the correct line first). Colored-needs-first payment / combat: deck44 confirms
  clean 2nd-corpus (C4 — counters fired on opponents' turns, 0 color fizzles). Reveal pick-ONE: deck135 ENGINE-R1 n=3
  clean (snow→hand, non-snow→graveyard, 0 all-to-graveyard). Wither trade annotations: deck27 round-2 validated 6
  annotations against real math, 0 false positives — stays CLOSED. Bounce-on-stack: deck44 C2 confirmed (model countered
  Pyroclasm, did not reach for GFTT-on-stack). **N8 0-power blocker heuristic: still UNEXERCISED** (deck62, 0 block
  fallbacks — answer-first commits a first-line BLOCKS, so the heuristic block path never runs; likely never validates at
  answer-first seats → needs a probe deck).

## Cross-seat reconciliations (task items a–e)

**(a) Echo/stale_echo family — HOW MANY distinct harness items? The (c) fixes did NOT close it; the family is TWO parser
items + one representation contributor, unified by an INDEX-WINS root.** Three NEW label-mismatch SHAPES surfaced, all
one ROOT — the stale_echo detector keys on a LABEL-string match between the model's CHOICE parenthetical and the option
text, and over-fires whenever the INDEX is correct-and-in-range but the label diverges:
  - deck140 (owner): **ANNOUNCE_X child-menu bare-parameter echo** — on the Black Sun's Zenith X-menu the model answers
    `CHOICE: 1 (Cast Black Sun's Zenith with X=N)` vs the bare option `X = N`; the "Cast <spell> with" survives the
    `targeting`-only strip, and `X=N` != `X = N` fails the name match. **4/4 BSZ X-menus, HARM=0** (heuristic argmax
    matched intent). Repros: vs62 s27, vs27 s26, vs102 s12.
  - deck135 (owner): **transform dual-face-name echo** — `CHOICE: 1 (Transform: Search for Azcanta)` vs option
    `Transform:azcanta, the sunken ruin` — correct index, but the echoed label names the PRE-transform face while the
    option names the POST-transform face. HARM=0. Repro: vs27 s27. (A side-effect of the (d) annotation landing.)
  - deck102 → routed to deck140: **DFC invented-verb echo** — `CHOICE: 1 (Activate Tergrid's Lantern)` vs offered
    `Flip Side with Tergrid's Lantern`. Repro: vs109 s42. **ROOT = R-DFC-FLIP** (representation) — fixing the DFC render
    removes this shape upstream; index-wins removes it at the parser.
  **Verdict:** these three shapes are ONE harness item — a label-match over-fire — and the robust fix is INDEX-WINS
  (trust the in-range numeric CHOICE index on an uncontested first-line answer, regardless of the trailing label),
  which collapses ANNOUNCE_X + transform + DFC-verb (and the wave-21 target-menu spell-prefix) into one change. This is
  the wave-21 headline-1 doctrine applied at the FAMILY level. **A SECOND, distinct harness item:** deck140 #2, the
  **absent-card CHOICE bookend** (vs102 s9) — a hallucinated middle `CHOICE: 1` naming a card ABSENT from the option set
  bookended by the correct `CHOICE: 0 (pass)`; stale_echo fired on the whole reply instead of last-well-formed-wins
  taking the final in-range CHOICE. Fix: drop an absent-card CHOICE line and keep parsing for an in-range one. A
  contributing **representation** cause (not itself an echo item): deck140 #4, the X-announce mana display
  (`no untapped sources | Already in pool: {...}`) drives the 40-line mana re-derivation that re-anchors the model on
  "Cast <spell> with X=N" — fixing it cuts both the worst latency spikes and the ANNOUNCE_X echo fuel.

**(b) Answer-first commit-lock — ONE watch item; promotion threshold = a 2nd HARMFUL seat (or the decode guard).** 1st
HARMFUL instance: deck27 vs109 s15 (blockers) — snap line committed a lord-chump misplay + a hallucinated 3rd blocker,
the model reasoned correctly that the chump was bad, then TRUNCATED (7.6k) before emitting a corrected BLOCKS line → the
parser locked the snap misplay. Benign counterparts: deck62 vs27 s16 (snap-chump → reasoned `none`, last-wins rescued,
REAL stakes) and wave-21 vs35 s18. The floor property of answer-first INVERTS when the snap is wrong and completed
reasoning would fix it. Guide-lane mitigation applied (deck27: front-load a free-kills-first allocation rule so the snap
is right). PARSER-side stays a WATCH: promote to a parser change only on a 2nd HARMFUL seat (single-seat now); the
STRUCTURAL close is the decode-time repetition/length guard, since the harmful lock only occurs when the spiral
truncates the reply before the correction — linking this item to the repetition_penalty pilot. OPEN as a WATCH.

**(c) deck109 A2 prose-salvage override — FLAG FOR CODE VERIFICATION (not asserted).** vs62 s21 (attackers): the
answer-first line `ATTACK: A1, A2, A3, A4` (all 4 eligible) was RECORDED as a 2-subset (A1+A4; A2+A3 silently dropped,
~3 power), no `fallback` field. The reply then spiraled to max_tokens (15× waffle) with NO later well-formed ATTACK line,
so last-well-formed-ATTACK-wins should have kept A1-A4. The 2-subset looks like a **combat prose-intent salvage arm
partially overriding a valid coded line** — IF confirmed, that is an integration-order bug in the wave-21 combat salvage
(it must fire ONLY when NO coded line parsed; the answer-first coded line is authoritative, salvage is fallback-only).
Repro is cheap and self-contained (seq 21 of the vs62 seat file off `/tmp/wagic-1d8ed8194`); contrast s23 (same game,
`ATTACK: A1..A6`, long but NOT a repetition spiral, all 6 recorded) isolates the trigger to the max_tokens
truncation/waffle-salvage, not length. NON-DECISIVE (the game was lost to an unraceable 5/8 flier regardless) — do not
inflate, do not downgrade. HIGH-priority code-verify at the AIPlayerGPT combat parse site. OPEN.

**(d) Ranking the two game-relevant representation defects.**
  - **#1 — forced-self-discard rendered with the detrimental-INVERTING generic target template (deck140 GAME-DECIDING +
    deck102 spiral, 2-SEAT CONVERGENT).** deck140 (vs102, Archon of Cruelty) discarded Damnation then Pyroclasm — its two
    live sweepers — into a go-wide board and died T12 a game it was winning at 23 life (8 such prompts). deck102's
    R-POX-DISCARD-TARGET (vs62 s21) is the SAME seam from the other side: a forced self-discard rendered as
    "TARGET CHOICE for discard cards ... pick target 1 of exactly 2" with Hymn's text embedded, which spiraled 16,752
    chars (one of the 2 correct retracted_choice fires). The target template is calibrated for choosing what a spell
    AFFECTS (an opponent's permanent → pick the MOST valuable); for a self-discard the value logic INVERTS (pick the
    LEAST valuable) and the template gives no cue. Ranked #1: game-deciding IN-corpus, 2-seat convergent, and the fix is
    CHEAP and LOW-RISK — a representation-layer detrimental-self-choice framing header (analogous to the reveal-framing
    header; "you are choosing which of your own things to GIVE UP — pick the least useful") PLUS stop rendering it as a
    "TARGET CHOICE" — cannot harm a seat that never sees the seam.
  - **#2 — R-DFC-FLIP (deck102, keystone-blocking, cross-deck).** Tergrid surfaces only as a repeatable phase-agnostic
    "Flip Side with Tergrid, God of Fright / Tergrid's Lantern" pseudo-action, never a clean "Cast (front/back)" — the
    model toggled it 11 times vs109 (each a no-op), and it spawns the s42 invented-verb echo. It BLOCKS validation of
    deck102's keystone Tergrid steal (0/18 across three corpora) and generalizes to any MDFC/DFC deck. Ranked #2 (below
    forced-discard): NOT itself game-deciding in-corpus (deck102 was mana-screwed at 3 mana; the DFC gap compounded but
    the screw was proximate), and the fix is heavier (option-generation: present one castable option per legal face,
    phase/mana-gated). Still HIGH — it has broad downstream cleanup (kills the s42 echo + the 11 wasted toggles + unblocks
    the keystone). Sequence: fix R-DFC-FLIP, THEN probe the steal (deck102's probe-sequence rung).

**(e) Crutch-retirement wave (44/109/140/102) — skill-rung material, ADOPTED.** All four seats retired or demoted
engine-carried crutches at four distinct representation-fix shapes (option-line annotation, marker-string,
CAUTION-line + battlefield-tag, printed count) → a 4-SEAT convergence that matured Method headline 1 (four sharpenings:
DELETE now-false clauses / SITUATIONS-DELETE vs mechanics-DEMOTE / generalize CLOSED-criteria to any representation
crutch / retirement PACING wait-n=2). The two deck140 rungs (rotation-in-as-new-leak-hunt + demotion-debt audit on
frozen guides) → Method headline 2. deck62's shape-present validation rung → Method headline 3. All folded into the
skill; see the Wave-22 changelog.

## Core prompt: PASS (17th consecutive) — no change; repetition_penalty pilot RECOMMENDED

Every seat routed core to PASS except deck109's Proposal 1 (a "commit-and-stop" anti-spiral nudge for the general
prompt). **Judged DECODE-LANE, not core-worthy — core PASS holds.** Reasons: (1) single-seat (vs62 s21 only); (2) 6 of 7
seats route the repetition spiral to the decode sampler, not core prose (the model quotes correct guide lines mid-loop —
prose cannot reach a decode loop); (3) direct precedent — wave-21 REJECTED the identical deck49 G-49a commit-brake for
core; (4) COMPOSITION-SEAM risk — a core "never re-open a decision you coded" line fights the harness's deliberate
last-well-formed-CHOICE-wins mind-change affordance (deck109 itself flagged the caveat; vs62 s16 used last-wins correctly
to self-correct Cast-nothing → Cast Pillar), and the sanctioned fix reaches the mechanism the prose cannot; (5) the fix
that DOES reach it is the repetition_penalty pilot.

**The forced-self-discard framing (deck140, filed as a core-prompt proposal) belongs at the REPRESENTATION layer, not
core** — its home is the same as the wave-21 reveal-framing header (an engine-fired seam header), so it does not become a
core-prompt line either; it is the #1 engine ledger item. Core stays PASS at 17.

**repetition_penalty config recommendation for wave-23: ENABLE a MODEST pilot (2nd consecutive wave recommending it;
evidence WIDER this cycle).** The decode-repetition spiral is now the demonstrated COMMON UPSTREAM CAUSE of the wave's
only harmful/latent-harmful parser events: the A2 attacker under-record (salvage fired on truncated waffle), the
answer-first commit-lock harmful instance (truncation before the correction), BOTH deck102 retracted_choice fires
(looping body coherently disavows the committed choice), the deck102 s29 6×-repeated mana block, the deck140 40-line
mana re-derivations that fuel the ANNOUNCE_X echo, and the p90 77.6s latency tax at every verbose seat. It no longer
buys correctness (answer-first did that) but it buys LATENCY, token cost, AND pre-empts every truncation-fed failure
above. Keep it a PILOT (conservative penalty, keep the max_reply_tokens guard); re-check the ahead-position priority
giants and legitimate long combat-math replies, back off if degraded. Keep it at the decode-sampler layer, NEVER the
guide.

## WAVE-23 ENGINE LEDGER (ranked; seat notes.md carry the seqs)

1. **Forced-self-discard rendered with the detrimental-INVERTING target template (deck140 GAME-DECIDING + deck102
   R-POX-DISCARD-TARGET, 2-SEAT CONVERGENT, HIGHEST).** deck140 pitched its two live sweepers into a go-wide board and
   died a game it was winning at 23 life; deck102's Pox self-discard spiraled 16.7k chars (a correct retracted_choice).
   Fix (REPRESENTATION, low-risk): stop rendering forced self-discard/sacrifice as a "TARGET CHOICE," and fire a
   detrimental-self-choice framing header ("you are choosing which of your own things to GIVE UP — pick the LEAST
   useful"), analogous to the reveal-framing header. Interim guide crutch shipped at deck140. Repros: deck140 vs102 s13/s14;
   deck102 vs62 s21.
2. **R-DFC-FLIP — DFC face-toggle blocks the keystone (deck102, HIGH, cross-deck for any MDFC/DFC deck).** DFC surfaces
   only as a repeatable phase-agnostic "Flip Side" pseudo-action (11 no-op toggles vs109), never a clean per-face cast;
   spawns the s42 invented-verb echo; blocks the Tergrid steal (0/18). Fix: present one castable option per legal face,
   phase/mana-gated. Sequence: fix this BEFORE probing the steal. Repros: deck102 vs109 seq8, seq9-44, seq42.
3. **Echo/stale_echo family — label-match detector over-fires on a CORRECT in-range index (deck140 owner; 135/102
   shapes; 3-SEAT convergent; HARM=0 but latent-consequential).** Root fix = INDEX-WINS (trust the in-range numeric
   CHOICE index on an uncontested first-line answer regardless of trailing label), collapsing ANNOUNCE_X + transform
   dual-face + DFC-verb (+ the wave-21 target-menu spell-prefix). SECOND item in the family: absent-card CHOICE bookend
   (deck140 #2, vs102 s9 — drop an absent-card line, keep parsing for an in-range CHOICE instead of firing stale_echo).
   The DFC-verb shape (deck102 s42) also folds away when R-DFC-FLIP (ledger #2) is fixed upstream. Repros: deck140 vs62
   s27 / vs27 s26 / vs102 s12 (ANNOUNCE_X), deck135 vs27 s27 (transform), deck102 vs109 s42 (DFC), deck140 vs102 s9
   (bookend).
4. **Combat prose-salvage OVERRIDES a valid answer-first ATTACK line (deck109 A2, FLAG — CODE-VERIFY, HIGH-if-confirmed).**
   A clean `ATTACK: A1-A4` was recorded as a 2-subset when the reply truncated; the salvage arm must fire ONLY when no
   coded line parsed (answer-first is authoritative). Non-decisive to the loss; do not assert without verifying the
   AIPlayerGPT combat parse-site precedence. Repro: vs62 s21 (self-contained).
5. **X-announce follow-up mana display self-contradictory (deck140 #4, MEDIUM — latency + echo twofer).** On a child/
   announce menu the parent cast already floated the pool but the line reads "no untapped sources," driving 40-line
   re-derivations (58-199s) that fuel the ANNOUNCE_X echo. Fix: on a child menu, state the floated pool as the available
   mana (or suppress "no untapped sources" when a pool is floated). Cuts the worst latency spikes AND echo fuel. Repros:
   deck140 vs62 s27 / vs27 s26.
6. **Answer-first commit-lock (deck27, WATCH — MEDIUM, 1 HARMFUL instance).** A truncation locks a wrong snap answer
   before the model's own correction lands (vs109 s15). Guide-lane mitigation applied (front-load the snap heuristic).
   Promote to a parser change only on a 2nd HARMFUL seat; the decode-time repetition guard is the structural close. OPEN.
7. **repetition_penalty pilot (cross-seat, MEDIUM-by-frequency but HIGH-leverage — DECODE-SAMPLER config, NOT code).**
   ENABLE a modest pilot next corpus (see Core section). Now the common upstream cause of items 4/6 truncations + both
   deck102 retracted_choice + the ANNOUNCE_X re-derivation + the p90 latency tax. Keep max_reply_tokens guard.
8. **ENGINE-R4 Azcanta ACTIVATION path — targeted-probe TODO (deck135, CARRY — orphaned by 135 rotating out).** The
   transform is now REACHABLE + OFFERED (6 windows) but the model rationally PASSES a {2}{U} card-draw in aggro-tempo;
   a model-ELECTED top-4 to-hand pick remains UNOBSERVED across 4 waves. Best validated by a TARGETED PROBE (a control
   deck with {2}{U} up and no better play), not organic selfplay — and since deck135 (the only snow/Azcanta seat) rotates
   out, this now REQUIRES a probe or a future 135 re-rotation. Hand-off.
9. **N8 baka blocker-fallback gang-chumps a 0-power body (deck62, CARRY — code-shipped, UNEXERCISED 3rd corpus).**
   Answer-first structurally prevents block fallbacks at answer-first seats (0 block fallbacks again), so N8 likely never
   validates in the active pool — needs a PROBE deck that forces a block fallback with a valuable 0-power body present.

**Carries (unchanged/low):** the decode-repeat-loop tax (subsumed by item 7); `#N` non-combat duplicate-name target-menu
disambiguation (parse-side, still UNEXERCISED end-to-end); Force of Negation reflexive-off-whitelist WATCH (deck135,
single low-signal firing); Gravelgill Axeshark / Counsel of the Soratami mana-cost divergences (deck14, Oracle-verify,
roster owner — deck14 now canary); double-block rules-slip (deck62 vs135 s22, model error, engine-handled first-legal,
LOW). **Construction flags (roster owner, not guide/engine):** deck27 has NO answer to dedicated lifegain-control (vs140
unwinnable — a 60-card gap, not pilot/guide); deck44 two-sided color pinch (black-screw loss recurs, vs27); deck140 needs
a cheap early-survival tool to live to its Staff-of-Nin grind (all 3 wins deployed a clock, all 3 losses never did);
deck102 mana-base fragility (Tergrid needs 4-5 mana, screw recurs); deck109 RDW reach/card-advantage ceilings.

**CLOSED / RESOLVED this wave:** ENGINE-F1 fetch-fizzle (deck135, 10/10 incl. same-turn-land-drop repro) + ENGINE-F1b
(never arose); ENGINE-R6 Azcanta transform annotation (3/3 transformed, multi-wave decline block broken); ENGINE-R1
reveal-partition (n=3, regression-clean); R-PAINLAND self-death class (deck102, PASSED at 4 life + survived 4 taps);
HARNESS-N9 retracted_choice OVER-FIRE (deck62 witness, 27× shape re-exposed / 0 fires; the 2 remaining fires are CORRECT);
wither trade-annotation round 2 (deck27, 0 FP); self-GFTT friendly-only annotation + stack-vs-battlefield targeting
(deck44 C1/C2, fired 28× / obeyed 0/28); colored-needs-first payment 2nd corpus (deck44 C4); options-as-list translog
serialization (deck44 C3). **CARD Ashenmoor Liege (wave-21 ledger #6): RESOLVED — NOT A BUG** (Scryfall: printed 4/1;
the engine render is CORRECT; the deck49 reviewer's "4/4" was a hallucinated Oracle value; the vs27 loss reverts to
matchup/draw — per the wave-21 post-synthesis integrator correction).

## Rotation decisions

Standing test: a seat is a rotation candidate when its guide takes NO substantive modification AND no seat-specific,
GUIDE-LANE new-work signal survives; where the two gates disagree because the new-work is HARNESS/ENGINE-lane, NAME which
gate holds the seat; candidacy is triggered by the DISCHARGE of a specific NAMED keep-reason, and a NAMED keep-reason
that discharges with nothing seat-specific to replace it → rotate (do not manufacture a fresh hold). Applying honestly:

- **135 — ROTATE OUT → canary.** Guide FROZEN (byte-identical, `cmp`-verified, zero decision leak); the multi-wave owned
  charge is CLEARED (ENGINE-F1 closed 10/10; ENGINE-R1 n=3; ENGINE-R6 transform block broken; the sole unmet sliver — a
  model-elected ENGINE-R4 top-4 pick — is rationally deprioritized in aggro-tempo and better served by a probe). The one
  fresh item (HARNESS-ECHO-Transform) is a small localized harness fix, handed off (folds into the echo-family
  index-wins). Textbook rotation profile. HAND-OFF: the ENGINE-R4 targeted-probe TODO and the transform-echo — note that
  NO other pool deck exercises the snow/fetch/transform/reveal seams, so those validations now require a probe or a future
  135 re-rotation.
- **62 — ROTATE OUT → canary.** Guide CONVERGED (byte-frozen two rounds; edit D validated across two corpora; every
  taught class held). Its sole hold — the NAMED N9 harness-witness role — DISCHARGED this corpus (27× shape re-exposed / 0
  fires). The residual open items (N8 unexercised, snap-then-revise cross-seat watch, N6 latency) are NOT deck62-specific
  and do not replace the keep-reason (deck62 structurally never produces a block fallback under answer-first, so holding it
  cannot advance N8). Rotate.
- **44 — ROTATE OUT → canary.** The rotated-in re-validation is DISCHARGED: the aging guide was audited against the
  current skill, its two stale crutches DEMOTED/DELETED per the crutch-lifecycle rung (self-GFTT durable warning confirmed
  fired 28× + obeyed 0/28), + 1 Cruel-Edict clause added; response-window / counter / held-mana / combat / answer-first
  all clean; both losses route off-seat (color-screw construction; lifegain-race matchup). The guide-mod this wave is
  TERMINAL (the demotion the rotation existed to perform) and no new-work-signal replaces it. Deploy the revised guide,
  then rotate out.
- **140 — STAY one more (synthesis moderation; see per-seat table †).** The reviewer verdict was rotate-out-but-deploy-
  first; reconciled to STAY for: (i) consistency with the precedent that an UNVALIDATED guide revision keeps the seat (the
  NEW forced-discard rule is unvalidated — cf. 27 durdle, 102 edits); (ii) deck140 is the WITNESS for the #1 engine ledger
  item (forced-self-discard) and holding it keeps that seam exercised at-seat (102 discards 140) while the engine fix is
  pending; (iii) the "one confirming corpus" logic the 109 reviewer applied to itself. A named, specific hold. The core
  guide (SWEEP trigger / #1-Rakdos-hold / finisher order) is proven load-bearing across 6+ corpora — rotate means "stop
  re-reviewing," never "remove the guide." Deploy the revised guide THIS wave regardless.
- **109 — STAY one more.** Reviewer = rotation candidate (crutch-RETIREMENT only; spine byte-frozen in behavior) but
  recommends ONE confirming corpus to verify the retired crutches (hybrid-mana + pillar-cost) were truly redundant, not
  load-bearing under a different draw. Adopted: stay to confirm the retirement; the only live items (A2 attacker-salvage,
  the model-behavior spiral) are ENGINE/decode, not guide.
- **27 — STAY.** Guide REVISED (+6-line free-kills-first block-allocation edit) — new-work by construction — and the
  deployed late-stall durdle revision is UNVALIDATED for a 3rd corpus (its walled-ground-stall trigger again did not
  occur; vs140 deck27 attacked every turn). Plus the answer-first commit-lock 1st harmful instance is a new watch. Not
  stable-proven → stay. Re-flag: bias next opponents toward ground-wall decks to draw the durdle trigger.
- **102 — STAY.** REVISED round 2 (two edits — painland-crutch DEMOTE + Tergrid-DFC awareness ADD, ALREADY DEPLOYED live)
  = new-work by construction; and the keystone Tergrid steal is STILL unexercised (0/18 across three corpora), now blocked
  by the newly-found R-DFC-FLIP that must be fixed before the steal can be validated. Re-validate the edits next wave +
  attempt the steal via a probe once R-DFC-FLIP ships.

**Roster / campaign scope.** The whole 164-deck roster is in scope. Waves have cycled 13 guided decks
(110/135/21/17/62/49/35/14/27/102/109/44/140). **Guided decks NEVER yet cycled: 131 (UR Spellslinger, Guttersnipe) and
133 (mono-B Devotion, Gray Merchant)** (deck8 Inquisitor is load-bearing content, not a review seat). The remaining
~148 roster decks are guideless Step-0-bis newcomers, incl. the DFC/Adventure-heavy deck137 (Selesnya Adventures) and
several control/wrath shells (deck59 Persistence, deck123).

**Rotate-out (3):** 135, 62, 44 → canary (guides stay deployed byte-identical as controls).
**Rotate-in (3):** chosen for archetype diversity + exercising open ledger items at LOW authoring cost:
- **deck137 (Selesnya Adventures, GW go-wide/tokens — GUIDELESS, Step-0-bis)** — the pool's R-DFC-FLIP stress test (5-6
  Adventure `//` cards, exactly the DFC face-toggle class); adds a GW go-wide/tokens axis absent from the pool. Higher
  authoring cost (fresh initial guide), but directly exercises ledger #2.
- **deck133 (mono-B Devotion Midrange, Gray Merchant — GUIDED-never-cycled, re-validate)** — low authoring cost; adds a
  devotion/lifedrain/midrange-grind axis; Gray Merchant magnitude-tracking; some discard content (secondary
  forced-discard coverage). A lifegain-grind that stresses the adjudication/N8 surfaces.
- **deck131 (UR Spellslinger, Guttersnipe — GUIDED-never-cycled, re-validate)** — low authoring cost; adds a
  spellslinger/burn-engine/instant-heavy axis (very different from creature combat); heavy cast/priority windows and a
  prime verbose-CoT / repetition_penalty-pilot witness.

**WAVE-23 POOL: 27, 102, 109, 140, 137, 133, 131** (4 stayers + 3 rotated-in). Archetype spread: UB Zombies aggro (27),
mono-B Tergrid control/steal/discard (102), mono-R aggro (109), RBW board-wipe control (140), GW Adventures go-wide
(137), mono-B Devotion midrange (133), UR Spellslinger burn-engine (131). Exercises: forced-self-discard (102 → 140),
R-DFC-FLIP (137 Adventures + 102 Tergrid), symmetric-trap (102 Pox + 140 wipes), creatureless-cast-filter (140),
decode/repetition_penalty (131). **Owner degree of freedom:** any rotate-in may be swapped — e.g. a control/wrath
guideless deck (deck59) if the wrath/creatureless-filter axis wants doubling, or a fresh newcomer for pure coverage.

Per-seat wave-23 focus:
- 27: draw the durdle trigger (bias toward ground-wall opponents); watch the commit-lock for recurrence; repetition_penalty
  pilot re-check on the ahead-position loop.
- 102: re-validate the two edits (does the DFC guide edit help the pilot commit to a face once R-DFC-FLIP is fixed; does
  the painland demotion hold); attempt the Tergrid steal via a probe after R-DFC-FLIP ships.
- 109: confirm the retired hybrid/cost crutches were redundant not load-bearing (one confirming corpus); verify the A2
  attacker-salvage precedence fix if it ships.
- 140: validate the new forced-discard guide rule at-seat (102 discards it); confirm the demotions hold; the
  detrimental-self-choice header (ledger #1) validation when it ships.
- 137: Step-0-bis initial guide; exercise R-DFC-FLIP hard (Adventure face-toggles).
- 133/131: baseline the never-cycled guides against the current engine batch (crutch-lifecycle audit per Method headline
  1); 131 is the repetition_penalty-pilot witness.
