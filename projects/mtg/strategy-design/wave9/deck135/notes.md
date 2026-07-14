# Deck-135 wave-9 — development notes (engine / harness / model / construction)

Run: PRIMARY `matchups-20260714-094942`. deck135 jsonl epochs + opponents in findings.md
game->file map. Doctrine: win column context-only; items below are decision-level or
platform-level with self-contained repros (PROJECT PAUSE after this wave — every item stands
alone with full context, no wave-10 dependency).

## ENGINE LEDGER

- **135v133 turn-2 Astrolabe livelock — FIXED, CONFIRMED RESOLVED.** The matchup that was a T2
  20/20 wedge in wave-8 (seat 1784024870, 16 KB) COMPLETED to T11 this wave (seat 1784046413,
  40 records, final -1 v 27). Arcum's Astrolabe cast normally via its `alternative cost {1}` in
  EVERY deck135 game this corpus (vs133 s18; vs140 s5/s17/s53/s73; vs110 s16/s22; vs44 s3/s12;
  vs109 s28; vs131 s3). No restriction-gated no-op, no cached-ask replay, no livelock. The
  cast-restriction gating + cast-seam livelock breaker in binary /tmp/wagic-a40671057 did the job.
  Detect any regression via: a seat file that ends at T2 20/20 with a repeating identical
  Astrolabe ask. RETIRE the wedge caveat from the deck135 review checklist.

- **Fetch re-offer tax — REDUCED again, still on the ledger.** Declined fetch-offer windows per
  game (a fetch-crack offered in options, not the chosen action): vs140 45, vs44 24, vs133 22,
  vs109 16, vs110 15, vs131 6. Avg 21.3/game all-in; 16.6/game excluding the vs140 grind — down
  from wave-8's ~27 and wave-7's ~62. No strategy damage (the crack re-appears each turn as the
  board changes, which is why the residual persists; nothing permanently withdrawn; no
  declined-twice-then-needed pattern). Still the #1 engine contributor to the vs140 grind decision
  count (45 of the seat's priority windows). Residual fix unchanged: once a crack activation is
  CHOSEN, consume/withdraw it; do not re-list a held fetch's crack every priority window (offer at
  most once per main-phase window; suppress on opponent turns while tapped out). Detect via the
  recurring `Put in Play with <fetch> targeting <land>` / `search basic land with <fetch>
  targeting <land>` strings across consecutive seqs.

## MODEL / REPLY-PROTOCOL (route to model-experiments / reply-protocol layer)

- **Answer-before-reasoning intent-collapse (vs131 s14) — 2nd instance across two waves, cleanest
  repro.** REPRO: seat 1784047950, s14, T8, life 20 v 17. Mana available {g}{u}{w}{w} (4 sources);
  Ice-Fang Coatl {g}{u} is option #3 (castable). Reply head char = `4` (= "Cast nothing right
  now"), then the PLAN reasons IN TEXT and reverses itself: "...a creature I can't cast yet
  (Ice-Fang Coatl needs {g}{u} but I have {g}{u}{w}{w} available, wait, I *can* cast Ice-Fang
  Coatl! Let me re-evaluate...". The head token committed before the PLAN reasoning finished, so
  the corrected conclusion never reached the parser. Self-corrected next phase: s18 (T8 MP2) cast
  Coatl. ~1 phase tempo; won the game 17 to -4. This is identical to wave-8 vs131 s48. TWO
  confirmed instances at the same seat over two waves discharges the "single datum, don't act"
  hold — the reply-protocol A/B is now DUE. Candidates (symmetric-core, judge by decision-quality
  = does the recorded choice match the reply's own concluded option): accept the LAST bare number;
  OR move the number AFTER the PLAN; OR reconcile a trailing named-option index against the head.
  Metric to arm across the corpus: count records where reply head != the option the PLAN's final
  sentence names. Do NOT change the protocol without running the A/B; do NOT touch guide text.

- **"Offered != payable" / "colored can't pay generic" false belief (vs140 s24).** REPRO: seat
  1784042098, s24, T6 MP2, life 20 v 17, creatures: 0. Option #0 = "Cast Arcum's Astrolabe with
  its alternative cost {1}"; option #2 = "Cast nothing right now" (chosen). PLAN: "the only legal
  action listed is to cast Arcum's Astrolabe for {1}, but I have no generic mana available (my only
  land is tapped for {G})." FALSE: {G} pays a {1} generic cost, and the engine OFFERED the cast
  (offered = payable). The pilot Cast nothing rather than dig. Guide-patched locally (strategy.txt
  point 3b names Astrolabe {1} paid by green). General false-belief family (same as core's "Mana
  pool (none)" correction) — see general-suggestions.md OBSERVATION 1 for the core-promotion
  trigger (needs a 2nd seat).

- No on-board hallucination (phantom-lethal class did not recur; board-read anchor held). No
  desyncs (0/6). No numeric-offset menu traps. No degenerate blocker replies. No `defer`/`fallback`
  records in any deck135 seat (zero defer records corpus-wide per the brief).

## GUIDE-VALIDATED BUT RESIDUAL (tracked for the next run after the pause, self-contained)

- **Fetch-crack-as-development not recognized when short GREEN or the fix needs an intermediate
  step — ADDRESSED IN GUIDE this wave (Rule #1 point 3 rewrite), tracked here for validation.**
  REPRO 1 (vs133, seat 1784046413): T6->T10 the pilot sat creatures:0 with Boreal Druid {g} in
  hand and NO green source (mana {u} only). Options at s23/s24/s32 were ONLY fetch-cracks (a
  Misty->Snow-Covered-Forest crack offered every window). It PASSED s23, s24, s32 (and passed the
  intervening turns), quoting Rule #1: "Do not fetch lands if I have a creature to cast... adhering
  to Rule #1 which prioritizes creatures over land development" — treating Druid-in-HAND as
  castable and letting the creature-before-fetch order veto the enabling fetch. Finally cracked a
  fetch at s33 then cast Druid s34, ~4 dead turns vs a drain deck that ran to 27. REPRO 2 (vs140,
  seat 1784042098, T6 s22/s23/s24): creatureless, mana {g}; wanted Into the North (needs {1}{g}=2,
  only 1 up) and refused to crack a fetch to ramp ("I will not sacrifice my fetch lands yet as I
  can cast Into the North first"). Guide fix: point 3 now gates on the OPTION LIST not the hand,
  makes cracking-a-fetch-for-the-missing-color a first-class develop action (Forest for green,
  Island for blue), and names offered=payable / green-pays-generic. VALIDATION after the pause:
  own-turn windows with creatures:0, a fetch-crack offered, and no directly-castable creature,
  where the pilot PASSES, should drop toward 0 (especially cracking for a FOREST when short green).

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own 2-color win creatures — STANDS.** Ice-Fang Coatl {G}{U}, Ohran
  Viper {1}{G}{G}, Abominable Treefolk {2}{G}{U}, Diamond Faerie {2}{G}{W}{U} are gold/double-pip
  off a base with only 2 Snow-Covered Island + 2 Snow-Covered Plains + fetches for blue/white.
  vs109 (LOSS): green-screwed — had {u}/{c} sources, Coatl and Into the North (both need green)
  stranded; 1 creature deployed all game. vs110 (LOSS): blue-clog draw, Coatl the only creature,
  never mana-enabled before dying T7. The pilot-side layer (underusing its own fixing) is now
  addressed by the point-3 rewrite; the construction ceiling remains. Direction (user's call):
  raise effective sources of the double-pip colors, or shave a pip off the creature suite.

- **Slow clock is structural.** Bodies are 1/1-2/2; Abominable Treefolk is the only closer and
  lands T11+ if at all. Feeds every timeout (vs140 to T23, opp gaining to 36 vs the Wipe-Them-Out
  lifegain). Construction (threat size/count), not guide text.

- **vs110 marginal keep (WATCH, one datum).** REPRO: seat 1784042117 s1 — kept "Search for Azcanta;
  Misty Rainforest; Flooded Strand; Arcum's Astrolabe; Prismatic Vista; Ice-Fang Coatl; Arcum's
  Astrolabe" (3 fetches + 2 Astrolabe + Search + 1 gold creature). A KEEP by the current rule
  (2-4 lands incl. fetches + one creature) but a blue-clog with one double-color creature and no
  proactive early play; died to Etched Affinity T7 without deploying a body. Low-priority: could
  sharpen the mulligan rule to ship a hand whose only creature is a double-pip GOLD creature with
  no on-board way to make both its colors AND no early proactive spell vs unknown aggro. One datum;
  did NOT touch the mulligan rule this wave (frozen). Flag only.

## STOPGAP / DEFUSAL TRIPLES (line, defect + citation, removal condition)

1. RULE #1 board-read anchor ("if a creature is not printed on your battlefield line it is NOT in
   play") — patches on-board hallucination (wave-6 vs131 s52-60). Removal: phantom-lethal class
   resolved model-side across waves. KEEP (0 recurrences this wave).
2. RULE #1 point 3 develop-branch — patches development paralysis. REWRITTEN this wave: gates on
   the OPTION LIST (not hand), makes fetch-crack-for-missing-color a first-class develop action
   (de-biased from blue), states offered=payable + green-pays-generic. Patches wave-9 vs133
   (s23/s24/s32) + vs140 (T6). Removal: manabase fixed OR the pilot reliably cracks a fetch to fix
   its missing color when creatureless, across a run.
3. RULE #1 / KEY-CARD uncastable-creature anchor ("if <creature> is NOT listed you can't pay yet;
   deploy what IS listed, else fix toward the color you lack") — patches stale-plan-around-
   uncastable. Extended this wave from Treefolk/Coatl to name Druid and route to the develop-branch
   / fetch-crack. Removal: manabase fixed OR model reliably reconciles a stale plan vs the legal
   list.
4. Force NEVER-list ("never Force a burn spell at your face; never Force a discard spell") —
   patches wave-7 face-burn over-fire AND wave-9 vs133 s13 Force-on-Inquisition misfire. Removal:
   Force 2-for-1 over-fire on a 1-for-1 target stays 0 across waves. KEEP (discard clause NEW).
5. Fetch re-offer guide line ("don't re-pick a fetch-crack you already took; a reappearing line
   does not mean you cracked it") — INTERIM stopgap for the engine re-offer defect (ledger). KEEP;
   removal when the engine consumes/withdraws a chosen crack.

## FREEZE-CHECK (turnaround-preservation mode)

Revised as a constrained diff off the live deployed guide. Frozen lines and where they survived:
- **RULE #1 block** — point 1, point 2, and the "develop next turn is the losing thought" +
  board-read anchor sentences FROZEN verbatim. The "Cast nothing / hold mana... BOTH WRONG" and
  "NOT license to skip a listed creature" sentences (the load-bearing tokens) are verbatim. Point 3
  sub-structure rewritten (option-list gate + fetch-crack shape + offered=payable) targeting the
  vs133/vs140 residual only.
- **RULE #2 block** — FROZEN verbatim (attack seam healthy; Boreal-Druid carve-out held).
- **WHO IS THE BEATDOWN** — FROZEN verbatim.
- **MANA** — FROZEN; added one symmetric clause (crack for a FOREST when short green, mirroring the
  existing blue clause). MULLIGAN / LETHAL CHECK — FROZEN verbatim.
- **Force of Negation KEY-CARD line** — FROZEN through the board-wipe/mass-bounce/kill-only-threat
  scoping + face-burn redirect; added ONE clause (never Force a discard spell, same 2-for-1
  rationale). Aetherize clause retained.
- **KEY CARDS + DECIDING SITUATIONS** — kept all prior lines; added the option-list/fetch-crack
  develop situation, extended the uncastable-key-card situation to name Druid/Coatl + fetch-crack,
  and added the discard clause to the Force situation. All positive situation->play form.
No frozen line was dropped; new text targets only the fetch-crack develop residual and the Force
discard exclusion.

## VALIDATION TODO (before this guide is credited; not gated on a next wave — for the resume)
Re-run self-play with this guide and confirm: (a) creatureless own-turn windows with a fetch-crack
offered and no directly-castable creature where the pilot PASSES drop toward 0 (especially cracking
for a FOREST when short green — the vs133 class); (b) Rule #1 point 1 still obeyed (no creature
passed for value); (c) Force fires stay confined to board-wipe/mass-bounce/kill-only-threat (0
discard or face-burn fires); (d) no phantom-board recurrence. The record is construction-bound —
do NOT read a flat win-rate as a guide regression until the manabase flag is addressed. The
reply-protocol A/B (vs131 intent-collapse, now 2 instances) is a SEPARATE core experiment, now DUE,
not gated on this guide.
