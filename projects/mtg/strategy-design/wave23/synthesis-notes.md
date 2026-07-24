# Wave-23 synthesis notes — NINTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-23 PRIMARY corpus
`matchups-20260724-013710` (21 games round-robin over 27/102/109/140/137/133/131, binary
`/tmp/wagic-0e6361732`). **The pool changed again** (135/62/44 rotated out to canary; 137 GW Adventures
rotated in GUIDELESS + 133 mono-B Devotion / 131 UR Spellslinger = the roster's LAST two
guided-never-cycled decks), so cross-wave record comparison is WEAK — judge decisions, not the win column.
Binary 0e6361732 carried the wave-23 step-1 batch (forced-self-discard FORCED-LOSS framing + R-DFC-FLIP
flip-annotation/castable-faces + Echo INDEX-WINS + ATTACK-line CoT-hijack fix) AND the FIRST-EVER sampling
pilot (repetition_penalty=1.05); THIS CORPUS is that batch's live validation.

## Corpus health

Fallbacks **1.6% (20/1237)** — UP from wave-22's 0.6%, composition shifted:
- **10 empty_reply** — a **transient corpus-start endpoint outage, PROVEN NOT pilot-caused** (deck131's
  I-3 timeline: all 4 latency ≈ 2502ms uniform HTTP fast-fails; deck131's vs140 game is the earliest-started
  of all 21, empties cluster in the earliest games; later requests WITHIN the same games succeeded with
  rep_penalty=1.05 in payload; post-corpus the endpoint accepts the param fine). Distribution: 131 s3-6 (4) +
  140 s5-7 (3) + 133 s6,s28 (2) + 109 s19 (1). **Board impact NIL at every owner** (all on low-stakes
  land-drops / single-option decisions; the two games with empties that mattered — 140 vs131, 133 vs131 —
  were both WON). Pilot-correlation formally OPEN per the brief but nothing at any seat implicates the sampler.
- **4 unparsed_reply** — 3 are deck27's decode-collapse token-garbage (vs137 s12/s13/s14, parser-correct
  rejections — nothing to recover); 1 is deck109's vs27 s13 out-of-range parent-menu conflation (`CHOICE: 3`
  on a 2-option sub-prompt, INDEX-WINS correctly REFUSED an out-of-range index + out-of-context label). **NONE
  need parser work** (reconciliation c confirmed).
- **3 retracted_choice** — all at deck133 (see reconciliation a).
- **3 deferred_to_heuristic** — all at deck137, all `kind=defer` convoke tap-payment sub-choices (Loxodon
  deck131 s21/s23 + March deck102 s19), **by-design** (the harness routes "which creatures to tap for convoke"
  to baka; engine taps eligible creatures; mechanically harmless).

Integrity clean: 21 games, **0 timeouts / 0 draws**, 7 cap-adjudications (14 no-gameend logs = exactly those
7 games). Win table (context-only): 133:5/6, 27:4/6, 109:4/6, 102:3/6, 140:2/6, 137:2/6, 131:1/6. Latency
median 17.4s p90 80s. Forced-loss framing fired on 463 renders; DISPLAY-TOGGLE flip annotation on 60 (0 at
deck137 — adventures use a SEPARATE surfacing path, not the DFC flip family). **Core (general prompt): PASS,
18th consecutive.**

## Per-seat verdict table

| deck | archetype / status | record (Δ, context-only) | fallbacks | pilot verdict | guide disposition | rotation |
|------|--------------------|--------------------------|-----------|---------------|-------------------|----------|
| 133 | mono-B Devotion · rotated-in (guided-never-cycled) | 5/6 (top seat, no baseline) | 5 (3 retracted_choice + 2 empty) | **SAME** (no prose degradation) | **REVISE** (2 ADDs: Bloodghast forced-loss fodder + Liliana +1 caution) | STAY (validate the 2 adds; unique retracted_choice witness) |
| 27 | UB Zombies aggro · guided round 4 | 4/6 (↑ from 1/6; EXOGENOUS) | 3 (all unparsed decode-collapse, vs137) | **WORSE** (0→3 decode-collapse; rep_penalty=1.05 insufficient) | **FROZEN** (byte-identical, `cmp`-verified) | STAY (thin free-kills-first N=1 + decode-collapse new-work + durdle off-case unexercised 4th corpus) |
| 109 | mono-R Aggro · revised round 2 | 4/6 (no baseline) | 2 (1 empty + 1 unparsed conflation) | **BETTER** (identical-line loops suppressed) | **REVISE** (Pillar {R} cost restoration — belief-correcting crutch regressed) | STAY (validate the restored cost line = the belief-correcting crutch's 2nd confirming corpus) |
| 102 | mono-B Tergrid control/steal · revised round 2 | 3/6 (no baseline) | **0** | **SAME / slightly BETTER** (p95 5749 < corpus 7784) | **REVISE** (4 edits: DFC demote + steal-take + Smallpox nuance + keep-bombs) | STAY (rotation IMMINENT — re-validate 4 edits + R-DFC-BACKFACE resolution) |
| 140 | RBW board-wipe control · rotated-in | 2/6 (no baseline) | 3 (all empty, transient outage vs131) | **SAME** (p95 7277 / max 13107 < corpus + wave-22) | **REVISE** (forced-discard crutch DEMOTE + 3 observed-friction refinements) | **ROTATE OUT** (deploy revised first) |
| 137 | GW Adventures go-wide · GUIDELESS Step-0 | 2/6 (no baseline) | 3 (all deferred_to_heuristic, by-design convoke) | **SAME / slightly WORSE** (convoke tax spikes 12-13k) | **NEW initial guide** (hypothesis) | STAY (newcomer never rotates on pass 1) |
| 131 | UR Spellslinger burn · rotated-in (guided-never-cycled) | 1/6 (no baseline) | 4 (all empty, incl. s3-6 outage cluster) | **SAME** (attacker declarations pure-index, no CoT-hijack) | **REVISE** (Guttersnipe attack-side [tapped]-read guard) | STAY (unvalidated guard + mulligan-verify open) |

Record notes (win column is NOT a verdict): **deck27's 1/6→4/6 up-swing is EXOGENOUS** — guide byte-frozen,
the revision touched exactly one decision, the swing is pool-swap + variance (three earned win-path wins; the
two losses to its two known worst archetypes: vs102 Tourach un-interactable protection, vs140 lifegain-control
now a 1-life adjudication miss). Do NOT credit free-kills-first for the up-swing (symmetric to wave-22's
caution against reading its DOWN-swing as a batch failure). **deck131's 1/6** decomposes to construction, not
play: threat density 6/60, no reach vs lifegain (vs140 — its life never dropped from 20 yet it lost, Guttersnipe
resolved T27 after the adjudication turn), no early defense vs aggro (vs109 killed it by T8), a manabase leaning
wrong for two red win-conditions (vs133 flooded on Islands, drew no red); the one win (vs102) was an unmolested
YP+token race, not a play change. **deck133's 5/6** is real top-seat performance but the pool is new (no baseline).

## Batch-item validation (a)–(e) — CLOSE / OPEN

- **(a) forced-self-discard FORCED-LOSS framing — CLOSE the render/framing (2-seat validated); residual
  bomb-pitch stays a WATCH (deck140 owns).** The header ("pick the card you can best afford to LOSE") rendered
  on every own-card forced choice; deck140 picked its LEAST valuable every time (a LAND when offered — vs102 s5
  Akoum Refuge over 5 spells, s8 Plains over sweepers; its most-redundant SWEEPER when no land was in the set —
  s13 Pyroclasm, s14 BSZ, keeping Damnation + both finishers). **The wave-22 catastrophe (pitching Damnation
  then Pyroclasm — its two live sweepers — with a land available, then dying to that board) did NOT recur against
  the same archetype.** deck102 confirms the render is correct on its own Smallpox self-discards (2-seat). RESIDUAL
  WATCH: one clean bomb-pitch (deck102 vs140 s14, hand {Necrogen Mists, No Mercy, Phyrexian Obliterator} —
  discarded the 5/5 trample Obliterator, kept two situational enchantments despite the "KEEP your best spells"
  render). This is the JUDGMENT-surface signature (per Method headline 1): the framing raises the floor but a
  generic prior can still MISVALUE a specific bomb — the deck-specific ADD (keep-your-bombs paragraph) is the
  coexisting complement, shipped at deck102 (edit 3) and deck140. The framing itself is CLOSED.
- **(b) DFC — CLOSE the perception half (R-DFC-FLIP flip-thrash); NEW defect R-DFC-BACKFACE-RESOLVES-FRONT stays
  OPEN.** The DISPLAY-TOGGLE annotation + castable-faces + 2-flip cap works: Flip-Side pseudo-action consumed
  exactly ONCE all corpus (deck102 vs27 seq7) vs wave-22's 11; the wave-22 phantom-"Activate Tergrid's Lantern"
  invented-name stale_echo did NOT recur (0 fallbacks at deck102). **MILESTONE: the Tergrid keystone finally
  exercised end-to-end** (0/18 across three corpora → drawn t4, CAST t9, steal seam fired t11, WON t13, vs27) —
  the deck's namesake engine reached the battlefield in a real game for the first time in project history, and
  it happened in the RANDOM pool with no probe (Method headline 5). NEW: **R-DFC-BACKFACE-RESOLVES-FRONT** —
  casting the {3}{B} Tergrid's Lantern back-face deploys the {3}{B}{B} God FRONT-face creature instead of the
  Legendary Artifact (ledger #3).
- **(c) Echo INDEX-WINS — CLOSE; the N9 residual s29 shape stays open.** deck140's X-menus this corpus (vs27 s36
  BSZ X=5, s48 Rakdos X=6, s52 Rakdos X=8) all recorded choice=1 TRUSTED, 0 fallback — the wave-22
  `CHOICE: N (Cast <spell> with X=N)` echo-vs-consumed-parent FP is ABSENT (better than wave-22, where the
  heuristic argmax merely masked the FP). deck109's vs27 s13 confirms the refusal side (INDEX-WINS correctly did
  NOT trust an out-of-range index). The label-match over-fire family is closed at the parser; the remaining
  retracted_choice over-fire (deck133 vs137 s29) is a DIFFERENT detector (N9), addressed in ledger #5.
- **(d) ATTACK-line CoT hijack — CLOSE (Ledger A2, deck109 owns).** chooseAttackers now takes the FIRST parseable
  ATTACK: line; validated 26/26 attacker windows (recorded set == first-line's in-eligible tokens, ZERO eligible
  attacker dropped, no prose-salvage override reproduced). 7 windows over-declared (over-range/summoning-sick
  tokens), all correctly ignored; 3 spiral windows (vs133 s23 @103s, vs102 s19 @56s, vs137 s7 @157s) all took
  the first line correctly — the exact case the fix targets. The wave-22 A2 silent under-count is CLOSED.
- **(e) repetition_penalty=1.05 pilot — VERDICT: KEEP at 1.05, but it is INSUFFICIENT for the decode-collapse
  class; route that class to a SEPARATE, stronger lever.** Per-seat: 109 BETTER (the wave-22 signature ~15×
  identical-waffle loop did NOT recur; worst intra-reply exact-line repeat = 6, a quoted game-log line), 102
  SAME/slightly-better (no verbatim-loop truncation), 140/131/133 SAME (no spiral lengthening, no prose
  degradation), 137 SAME/slightly-worse (convoke tax spikes, but those are re-derivation not repetition), **27
  WORSE** (the ahead-position degeneration watch recurred and turned HARMFUL — 3 clustered unparsed
  decode-collapses, vs137 s12-14, one with 1354× verbatim-line repeat, 82-122s each). **Argument from the
  evidence:** 1.05 is net-positive-or-neutral on the DIVERGENT-spiral / latency / tight-loop axis it targets
  (it bought the deck109 win and degraded prose nowhere), so DROPPING it loses a real gain for nothing and
  RAISING it uniformly risks the ahead-position priority giants + legitimate long combat-math (the wave-22
  caveat). But it did NOT prevent the DECODE-COLLAPSE class (deck27's cluster + the noted 137 verbatim loop) —
  a full token-collapse is a distinct failure that a modest sampler nudge cannot reach. So: **KEEP 1.05 as the
  floor; the collapse class is a SEPARATE ledger item (#4) needing a max-tokens/repetition-tripwire fast-fail
  and/or a higher-penalty pilot on top.** The pilot verdict is a decode/sampler output, NOT a skill or core line.

## Cross-seat reconciliations

**(a) N9 retraction detector — 1 genuine fire + 1 engine-caused + 1 residual over-fire; adopt deck133's
discriminator for the ledger.** deck133 owns ALL 3 corpus retracted_choice. Discriminator = count coded
`CHOICE: N` indices and check whether the FINAL differs from the first-line one:
  - **vs140 s9 — GENUINE (indices [2,1]):** `CHOICE: 2 (Decline)` … `So CHOICE: 1.` A real two-index
    contradiction → retracted_choice CORRECT.
  - **vs137 s26 — ENGINE-CAUSED (index [1], sustained):** the disavowal prose was a CORRECT refusal of the
    UNPAYABLE Yawgmoth option (ledger #1), not a change of intent; heuristic-block was safe.
  - **vs137 s29 — RESIDUAL OVER-FIRE (index [1], sustained):** `CHOICE: 1 (Play Polluted Delta)` held
    throughout; "Correction:"/"Wait," revised only a sub-point (Bloodghast haste), never the coded choice.
  **Ledger discriminator (ADOPT, ledger #5):** gate retracted_choice on (a) an actual contradictory SECOND coded
  CHOICE index (s9's 2→1), OR (b) an explicit disavowal of the SPECIFIC coded action's legality/payability (s26).
  Do NOT fire on generic "Correction:"/"Wait," phrases while the coded index stands unchanged (s29); where one
  in-range coded index is sustained, INDEX-WINS should trust it. Latent risk: in a window where the heuristic's
  guess did NOT match the sustained coded intent, the s29-style over-fire would flip a correct decision.

**(b) decode-collapse class (deck27 garbage + the 137 loop) — parser correctly rejects; route to the DECODE lane;
the pilot did NOT fix it.** deck27's classification rung (adopted as skill Method headline 3): a failed reply with
NO coded line is a decode-collapse (owner: sampler/harness), distinct from commit-lock (a WRONG coded line frozen
by truncation; owner: guide/parser). The parser is correct to return unparsed — INDEX-WINS/echo recovery need a
well-formed index or a sibling CHOICE line, and a token-collapse has neither. rep_penalty=1.05 was the theorized
cheap fix and the loop recurred WITH it active and HARDER → **1.05 is insufficient for this class** (ledger #4).

**(c) the 4 unparsed shapes — NONE need parser work.** 3 = deck27 decode-collapse garbage (parser-correct
rejections, route to the sampler lane); 1 = deck109 vs27 s13 out-of-range parent-menu conflation (a genuinely
unrecoverable garbage answer — no in-range signal existed; INDEX-WINS correctly refused). Confirmed: no
parser change is indicated by any of the 4.

**(d) crutch-lifecycle refinements — MERGED coherently into the skill (Wave-23 refinement + Method headline 1).**
deck109's information-supplying-vs-belief-correcting distinction (2-corpora bar for belief-correcting) +
deck133's FACT-vs-JUDGMENT surface split + deck102's oracle-truth-vs-engine-behavior split are THREE complementary
angles on one rung — adopted as a CLASSIFY-FIRST step on the crutch-lifecycle terminus (the load-bearing,
3-seat-convergent merge this wave). See wave23/strategy-writing-skill.md Step-0 Wave-23 refinement.

## Core prompt: PASS (18th consecutive) — no change; repetition_penalty pilot KEEP-with-escalation

All seven seats routed core to PASS. No general-prompt line added or cut. The long exploratory spirals + the
decode-collapse are the same DECODE-LANE class the wave-22 changelog explicitly declined to fix at core (a
"commit-and-stop / never-re-open" core line composes BADLY with the harness's deliberate
last-well-formed-CHOICE-wins mind-change affordance — a composition seam; 6-of-7 seats route the spiral to the
decode sampler, not core prose). The sanctioned levers are the repetition_penalty pilot (KEEP at 1.05) and, for
the collapse class it cannot reach, a decode-time max-tokens/repetition tripwire (ledger #4). The one decision
error routed toward core (deck109 vs137 s12 affordable-Pillar decline) is a DECK-LEVEL cost conflation → guide,
not core. Core stays PASS at 18.

## WAVE-24 ENGINE LEDGER (ranked; seat notes.md carry the seqs)

1. **Yawgmoth "Sacrifice another creature" activated ability OFFERED AS LEGAL with no other creature to sac —
   a LEGALITY-ORACLE INVARIANT VIOLATION (deck133, HIGH, cross-card).** vs137 seq26: the engine listed
   `-1/-1 Counter with Yawgmoth … [cost: Life, Sacrifice]` while deck133's battlefield read `(creatures: 1)`
   (only Yawgmoth itself; Liliana is a planeswalker, Bloodghast in graveyard) → the activation cost is UNPAYABLE,
   yet it was offered. The model correctly diagnosed the impossibility, spiraled ~85s, flagged retracted_choice
   (the engine-caused fire in reconciliation a). **This directly violates the shipped priority/legality-oracle
   invariant "illegal choices are structurally impossible" (options come from the engine's legal set) — ranked
   #1 because a broken foundational invariant of the redesign outranks a beneficial-side bug.** Fix: gate the
   Yawgmoth (and any "Sacrifice another creature") activated-ability offer on `count(other controlled creatures)
   >= 1`. Non-deciding this game (deck133 won) but it is a legality-filter GAP that produced a fallback + spiral.
2. **R-ADV-FACE — the alternative/adventure-cost cast option prints the WRONG face's (P/T) and no effect text
   (deck137, HIGH, cross-deck for any adventure/alt-cost deck; CONFIRMED MISFIRE).** The adventure-SPELL option
   renders `Cast NAME // ADV with its adventure cost {cost} (P/T)` where (P/T) is the CREATURE face's stats and
   carries NO effect text; the executor reads (P/T) as the body it deploys. Confirmed wrong outcome: deck131 s15
   (`1. Cast Shepherd of the Flock // Usher to Safety with its adventure cost {w} (3/1)` → reasoned "3/1
   creature, adds a body" → cast the BOUNCE spell, self-bounced its own Faerie Guidemother); deck27 s19 a 9.3k
   tax. Fix (mirror the reveal menu's `{text:}`): on the "with its adventure cost" option, drop the creature
   (P/T) and print the adventure spell's short effect text (or label it as the effect). Guide teaches the
   per-face capability table as the INTERIM stopgap; remove once the option carries effect text.
3. **R-DFC-BACKFACE-RESOLVES-FRONT — casting the DFC back-face deploys the FRONT-face permanent (deck102, MED —
   correctness; currently benefit-side but rules-wrong; blocks finalizing the Lantern oracle description).** vs27
   seq34: mana {b}{b}{b}{b}=4 (God's 5 unaffordable), model chose the affordable `Cast Tergrid's Lantern {3}{b}`
   (Legendary Artifact face) → narration deployed `Tergrid, God of Fright` (the 4/4 God, {3}{B}{B} front face).
   Mechanical discount, BENEFICIAL this game (enabled the steal + win) but rules-incorrect. Open questions for the
   DFC/modal-cast seam owner: does Wagic implement the Lantern artifact face at all, or does its DFC model collapse
   both faces to the front permanent? Probe path: a deck199 stacked with Tergrid + cheap mana to reliably reach a
   Lantern-affordable/God-unaffordable turn and inspect what resolves. Until confirmed, deck102's guide keeps the
   Lantern description generic ("cheaper face"), per Method headline 1(c).
4. **Decode-collapse mitigation — rep_penalty=1.05 is INSUFFICIENT (cross-seat: deck27 vs137 s12-14 + the 137
   verbatim loop; MED, growing class).** The ahead-position degeneration produced 3 unparsed token-collapses
   (no coded line; 82-122s each burning wall-time before falling back). Candidates for the between-wave sampler
   ledger: (a) a HIGHER rep_penalty pilot layered on top (re-check the ahead-position priority giants + combat-math
   for degradation), (b) a max-tokens / repetition-tripwire FAST-FAIL to the heuristic (reclaims ~5 min wall-time
   across the 3 deck27 collapses and is the STRUCTURAL close for the commit-lock WATCH too — a wrong snap can only
   freeze if the reply truncates before the correction), and/or (c) an answer-locked retry. Sampler/harness, NOT
   parser, NOT guide. Non-deciding this corpus (heuristic covered all 3, deck27 won) but promote to a corpus-wide
   pilot finding if other seats show collapse-while-ahead under any sampler change.
5. **N9 retracted_choice over-fire discriminator (deck133, MED-LOW, parser/harness).** Gate retracted_choice on
   (a) an actual contradictory SECOND coded CHOICE index OR (b) an explicit disavowal of the SPECIFIC coded
   action's legality/payability; do NOT fire on generic "Correction:"/"Wait," while the coded index stands
   (reconciliation a). All 3 fires non-deciding this corpus; latent-risk if the heuristic ever mismatches a
   sustained coded intent. deck133 uniquely produces this class → keep it as the seat's witness role.
6. **X-menu lethal-trust + Mana-available color-count representation (deck140, MED — latency tail + one
   below-lethal misfire, non-deciding).** vs27 s46: fired Rakdos's Return convinced it could set X=20 for lethal
   (opp at 20), but the X menu then capped X=6 (its real affordable X after {R}{B}) — a mis-count. Same root as
   the black-source-count being unreadable on the `Mana available` line (deck140 re-derived its color base 4×
   inside single replies = its longest spirals). Fix: surface derived color availability on the `Mana available`
   line (e.g. "black sources: 1"), AND ensure the pre-cast lethal reasoning sees the same X cap the menu enforces.
   This is the corpus-standing "Mana available hides dual second colors" item (subsumes wave-22 ledger #5), now
   the dominant driver of deck140's latency tail.
7. **mulligan-not-offered VERIFY (deck131, cheap, harness).** deck131 vs133 kept a STEP-1 mulligan hand (no red,
   flooded) with NO `ask` record containing "Mulligan decision" in its prompt — every OTHER deck131 game logged
   one; vs133 did not. VERIFY: was 131 denied a mulligan step (engine/harness), or is the mulligan simply not
   emitted to the log under some condition (on-the-play vs on-the-draw, or a heuristic auto-resolve)? If the pilot
   never gets to answer the mulligan on some games, the guide's most-cited STEP-1 gate is silently skipped there.
   Repro: grep each deck131 game for an `ask` whose prompt contains "Mulligan decision"; vs133 has none.
8. **Nameless option-label serialization (deck133, LOW, cross-deck).** The "cast this permanent" binary
   serializes with the card name ONLY in the prompt header, never in the option label (`Put in Play` /
   `Decline - do nothing`; vs140 s9, vs27 s17, vs140 s22) — same degenerate-label class as wave-16's `Becomes `.
   Combined with a stale prior-PLAN it contributed to the s9 retraction spiral. Fix: emit the card name in the
   label (`Put Bloodghast in Play`).

**Carries:**
- **N8 baka 0-power blocker gang-chump heuristic — UNEXERCISED (4th corpus).** Answer-first structurally prevents
  block fallbacks at answer-first seats (0 block fallbacks again), so N8 likely never validates in the active
  pool → needs a PROBE deck that forces a block fallback with a valuable 0-power body present.
- **ENGINE-R4 Azcanta ACTIVATION probe — orphaned by deck135's rotate-out (wave-22).** The transform is
  reachable + offered but the model rationally passes {2}{U} card-draw in aggro-tempo; a model-elected top-4
  to-hand pick remains UNOBSERVED. Requires a targeted probe (a control deck with {2}{U} up and no better play)
  or a future 135 re-rotation. Hand-off.
- **Answer-first commit-lock (deck27 WATCH) — no 2nd HARMFUL instance this corpus (stays single-seat, 1 harmful:
  wave-22 vs109 s15).** The ≥2-seat promotion threshold to a parser change is NOT met. The structural close is
  ledger #4's decode-time guard.
- **Construction flags (roster owner, not guide/engine):** deck131 threat-density 6/60 + no-reach-vs-lifegain +
  no-early-defense + manabase-leans-wrong-for-two-red-WCs; deck133 no-reach/no-flyer-answer + no-early-blocker
  (Bloodghast can't block) + self-damage suite; deck140 needs cheap early survival to live to the Staff-of-Nin
  grind (both wins reached it, all 4 losses did not) + a fragile black base (vs137 lost purely to {B}{B} off one
  black source); deck27 no answer to dedicated lifegain-control (now a 1-life adjudication miss vs140, not a
  blowout — a little reach/disruption likely flips it); deck102 mana-base fragility (Tergrid needs 4-5 mana).

**CLOSED / RESOLVED this wave:** R-DFC-FLIP flip-thrash (deck102, 11→1 toggle, annotation renders, keystone
Tergrid steal cast+fired+won 0/18→1); Tergrid STEAL SEAM (render + parse clean, VALIDATED); Echo INDEX-WINS
(deck140, X-menus trusted 0 fallback, wave-22 FP shapes gone); ATTACK-line CoT-hijack / Ledger A2 (deck109,
26/26, fix d); forced-self-discard FORCED-LOSS framing render (deck140 owns, 2-seat, catastrophe class did not
recur — residual bomb-pitch is a WATCH); R-PAINLAND (deck102, still held — Ancient Tomb priced, no self-death);
wither trade-annotation round 3 (deck27, 0 FP, 3rd clean corpus).

## Rotation decisions

Standing test: a seat is a candidate when its guide takes NO substantive modification AND no seat-specific
GUIDE-LANE new-work signal survives; candidacy triggers on the DISCHARGE of a specific NAMED keep-reason.

- **140 — ROTATE OUT.** (a) forced-loss framing VALIDATED (the header works against the same archetype that
  beat it last cycle — pitched lands then redundant sweepers, kept finishers, did NOT lose to a pitched sweeper),
  the guide CONVERGES (interim crutch demoted to a 6-line marker nod), and NOTHING left is guide-refinement — the
  residual is ENGINE (mana-display, ledger #6), HARNESS (the transient outage, resolved server-side), and
  DECK-CONSTRUCTION (user-owned). The reviewer's honest verdict is rotate-out; the named keep-reason (top-ledger
  forced-loss witness) is DISCHARGED. Deploy the revised guide THIS wave, then rotate to canary (guide stays
  deployed byte-identical as a control). "Rotate" = stop re-reviewing a converged seat, NOT remove the guide (it
  is demonstrably load-bearing — the model cites SWEEP TRIGGER / #1-Rakdos-hold / forced-loss order and wins both
  non-structural games).
- **109 — STAY.** The confirming corpus FAILED for one crutch: the Pillar {R} cost retirement REGRESSED (vs137
  s12 mis-cost). Per Method headline 1(b), a belief-correcting crutch needs 0 failures across TWO corpora with
  the belief-triggering shape present; the restored lean cost line is UNVALIDATED → STAY to confirm it closes the
  Pillar-{R}-vs-Spear-{1}{R} conflation (the hybrid-mana retirement HELD and is done). Engine side (Ledger A2)
  CLOSED, needs no further deck109 attention.
- **133 — STAY.** Guide REVISED (2 adds) AND a genuine new-work signal (the rotated-in pool exposed forced-self-
  loss + Liliana +1-while-defending, uncovered by the old frozen guide). Validate the Bloodghast-fodder + Liliana
  +1 adds next corpus; deck133 uniquely produces retracted_choice → keep it as the N9-discriminator witness.
- **131 — STAY.** Guide REVISED (Guttersnipe attack-side [tapped]-read guard) filling a GAME-DECIDING gap; the
  fix is UNVALIDATED (needs a matchup that presents a partly-tapped blocker board — Zombies/aggro), and two
  harness/engine items are open at this seat (the tapped-misread cross-seat WATCH, the missing-mulligan VERIFY).
- **27 — STAY.** Guide byte-FROZEN, but new-work-signal present: the ahead-degeneration decode-collapse turned
  HARMFUL for the first time (3 unparsed, pilot-correlated), the free-kills-first revision is thin (N=1
  multi-attacker case, vs109 s13 resolved correctly), and the durdle off-case is UNEXERCISED (4th corpus). Not
  stable-proven → stay. Re-flag: bias next opponents toward ground-wall decks to draw the durdle trigger.
- **102 — STAY (rotation IMMINENT).** REVISED round 2 (4 edits) = new-work by construction, AND R-DFC-BACKFACE-
  RESOLVES-FRONT (ledger #3) must be resolved before the Lantern oracle description can be finalized. But the
  keystone is now PROVEN (cast + steal fired + won), the flip-thrash is CLOSED, and 3 of the 4 edits are
  refinements. If next wave (a) the DFC demotion holds, (b) R-DFC-BACKFACE resolves so the Lantern description
  stops being provisional, and (c) the steal-take line lands, deck102 is a STRONG rotation candidate.
- **137 — STAY (newcomer never rotates on pass 1).** The Step-0 guideless baseline produced an initial guide
  that is a HYPOTHESIS; validate it next corpus (adventure surfacing, convoke defers, the capability-table stopgap).

**Roster / campaign scope.** Guided review seats are now EXHAUSTED — all cycled (110/135/21/17/62/49/35/14/27/
102/109/44/140/131/133; deck8 Inquisitor is load-bearing content, not a review seat). Per the campaign scope,
the pool now draws GUIDELESS Step-0 newcomers (like deck137). 164 baka deckN.txt files exist; the roster's
~148 unreviewed decks are the guideless pool.

**Rotate-out (1):** 140 → canary (guide stays deployed byte-identical as a control).
**Rotate-in (1):** **deck59 (WB Persistence — control/wrath, GUIDELESS Step-0).** Chosen for archetype balance:
140's departure removes the pool's CONTROL/WRATH/board-wipe axis, and deck59 restores it (Damnation ×4, Day of
Judgment ×4, Wrath of God ×4) while adding a NEW persist-recursion axis (Kitchen Finks, Safehold Elite, Rendclaw
Trow, Restless Apparition) and directly exercising open seams: the creatureless-cast-filter (601.2c), the
symmetric-sweeper trap (its own persist creatures survive its wraths — a mass-effect the pilot must learn to
fire WITH its board out, the deck133/102 SYMMETRIC-cost face), and a fresh forced-loss surface. Low authoring
cost is not available (it is a fresh initial guide, Step-0-bis) but the axis coverage justifies it; the owner
retains the degree of freedom to swap in a different guideless deck (e.g. deck123 Intruders of Thraben WBU
combo) if a combo axis is preferred over pure wrath.

**WAVE-24 POOL: 27, 102, 109, 133, 131, 137, 59** (6 stayers + 1 rotated-in). Archetype spread: UB Zombies
aggro (27), mono-B Tergrid control/steal (102), mono-R aggro (109), mono-B Devotion midrange (133), UR
Spellslinger burn (131), GW Adventures go-wide (137), WB Persistence control/wrath (59). Exercises:
R-DFC-BACKFACE resolution (102), R-ADV-FACE adventure mislabel (137), Yawgmoth legality-filter (133), the
decode-collapse mitigation witness (27 + 131), belief-correcting-crutch 2nd-corpus confirm (109), Guttersnipe
tapped-read validation (131), and the creatureless-cast-filter + symmetric-sweeper (59).

Per-seat wave-24 focus:
- 27: draw the durdle trigger (bias toward ground-wall opponents); watch the ahead-degeneration collapse under
  whatever sampler change ships (ledger #4); free-kills-first thickening (more multi-attacker cases).
- 102: re-validate the 4 edits; confirm R-DFC-BACKFACE resolution (or benign) so the Lantern description stops
  being provisional; confirm the steal-take line + DFC demotion hold.
- 109: confirm the restored Pillar cost line closes the vs137-s12 conflation shape (belief-correcting crutch's
  2nd confirming corpus). If 0 Pillar mis-costings WITH the shape present, it converges → rotate.
- 133: validate the Bloodghast-fodder + Liliana +1 adds; re-witness the retracted_choice class + the N9
  discriminator if it ships.
- 131: does the pilot now HOLD Guttersnipe back when a blocker lacks the [tapped] tag? (needs a creature-blocker
  matchup); does a mulligan decision get offered every game (ledger #7 VERIFY)?
- 137: validate the Step-0 initial guide (capability table, convoke-trust line, attack passivity); exercise
  R-ADV-FACE hard; if a second alternative-cost deck appears, weigh promoting the rule-3 fifth face + convoke rung.
- 59: Step-0-bis initial guide; exercise the creatureless-cast-filter + symmetric-sweeper (fire the wrath WITH
  persist creatures out) + the new forced-loss surface.
