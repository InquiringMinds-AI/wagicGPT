# Deck-135 wave-13b — development notes (engine / harness / model / construction)

Run: PRIMARY `matchups-20260715-183228`. Binary `/tmp/wagic-a75e2625e` (Force round-2 cost-target
fallback + Force round-3 offer-legality/dead-end removal + auto-tap two-pass + name-echo protocol +
BLOCKS:none + 240s timeout). deck135 jsonl epochs + opponents in findings.md game->file map. Doctrine:
win column context-only; layer-routing enforced (engine items live HERE with repros, never in prompt/
guide text). Each item self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 (HIGH PRIORITY, CARRIED — still next cycle's top fix) — Force of Negation's FREE ALTERNATIVE
  COST never counters; the targeted noncreature spell resolves anyway.** UNCHANGED this corpus, and NO NEW
  RESOLUTION DATA: Force was offered as a cast exactly ONCE (deck44 s22) and correctly DECLINED, so it
  never reached the stack (grep of all seat `events` for "Force of Negation" = draws/reveals/discard only,
  0 casts). The round-3 INTERFACE fix (offer legality) validated (ENGINE #1b); the RESOLUTION layer is
  still unvalidated end-to-end.
  FIX DIRECTION (unchanged): Force's counter-target resolution must bind the stack noncreature spell as
  its legal target (the `can target on the stack: X` hint already knows it exists). DETECT REGRESSION: a
  Force cast where the targeted stack spell's EVENTS show `stack -> graveyard/battlefield` (resolved)
  instead of a counter. VALIDATION TODO next run: engineer a NON-adjudicated Force-on-board-wipe window
  and confirm the wipe is COUNTERED and the blue card is spent for real effect.

- **ENGINE #1b (FIXED — VALIDATED this corpus) — Force round-3 offer legality / dead-end removal
  (a75e2625e).** CONFIRMED: (1) **0 standalone "exile a blue card" dead-end actions** anywhere in the
  corpus; (2) the one alt-cast offer (deck44 s22) is correctly GATED on a payable exile target and
  correctly formed: `Cast Force of Negation with its exile a blue card from hand cost  - can target on the
  stack: Go for the Throat` (deck135 held 4 blue cards → payable). The model DECLINED correctly (opponent
  was killing its OWN Sleep-Cursed Faerie; a Force = 2 cards to stop the opponent hurting itself). This is
  the round-2 (eb5145082) + round-3 (a75e2625e) shipment validated at the seat that owns the seam.

- **ENGINE #2 (NEW, HIGH VALUE) — stale `[attacking]` tag + inverted/absent freeze-state annotation on a
  Treefolk-frozen creature; a self-contradictory BOARD line that inflated reasoning to a decision
  TIMEOUT.** REPRO (deck109 seat 1784170894): s16 deck135 casts Abominable Treefolk; s17 chooses Boros
  Reckoner as Treefolk's ETB tap-and-freeze target (Reckoner was the opponent's only untapped creature).
  At s19 (the fatal Blockers window) the battlefield line renders Reckoner as
  `Boros Reckoner (3/3) [tapped - untaps and can attack next turn] [attacking]` — but (a) Reckoner is NOT
  in the numbered Attackers list (A1 Stromkirk / A2 Cackler / A3 Ash only) and the harness `up to 8`
  damage line correctly EXCLUDES its 3 power, so the `[attacking]` tag is STALE; and (b) a Treefolk-frozen
  creature does NOT untap, yet it is annotated `untaps and can attack next turn` — the freeze is not
  represented, it is INVERTED. The model spent its ENTIRE s19 reply oscillating to reconcile 4
  attacking-tagged creatures vs a 3-attacker list vs `up to 8`, hit the token/latency wall, and produced
  `unparsed_reply` → heuristic fallback (choice=0) on a combat decision. SAME SHAPE as the wave-10 Force
  "NO legal target" contradiction (a self-contradictory representation multiplying a weak model's
  derivation length), at a NEW locus (board line) with a WORSE terminal cost (a TIMEOUT, not just tax,
  under the latency cap). Outcome-neutral here (position lethal: 8 power incoming vs 4 life, one blocker),
  but in a NON-lethal position this pushes a critical block to a heuristic fallback.
  FIX DIRECTION: (1) clear the `[attacking]` tag on any creature NOT in the current declared-attackers
  set; (2) represent Treefolk-frozen state (`frozen - will not untap`) and SUPPRESS `untaps and can attack
  next turn` for frozen permanents. DETECT: a creature carrying `[attacking]` while absent from the
  combat's numbered attacker list; a state annotation contradicting a known effect on the same permanent.

- **ENGINE #3 (minor, CARRIED from wave-11) — fetch chosen-but-unresolved re-ask + duplicate-identical
  option lines.** Not re-audited in depth this corpus (the auto-tap two-pass / round-3 batch did not
  target it). The 2-window `choose -> re-offered once -> resolve` pattern and the duplicate targeting
  entries (deck holds 4 copies of a snow basic) persist as decision-count inflation only, zero resource
  loss. Carried unchanged; low priority.

## HARNESS

- **HARNESS #1 (NEW, LATENT — brief's flagged candidate) — attackers name→index parse gap.** The model
  emits mixed `ATTACK: A1, <Name>` declarations; the parser takes the leading A-index and DROPS the
  trailing NAME. INSTANCES: vs110 s16 `A1, Ice-Fang Coatl`; vs110 s25 `A1, Abominable Treefolk`; vs133 s25
  `A1, Diamond Faerie`; vs44 s19 `A1, Boreal Druid`. EVERY dropped name this corpus was an ILLEGAL
  attacker (a just-cast summoning-sick creature, or a mana-only Boreal Druid) that was NOT among the
  offered A-options, so the drop cost ZERO damage — the parser clamped to the legal attacker. Latent-
  dangerous: if a NAMED creature is ever a legal A2+ attacker, it would be silently dropped. FIX: an
  attackers name→index reconcile arm mirroring parseChoice's name-echo (map the name to its A-index; drop
  explicitly if not a legal attacker). DETECT: an `ATTACK:` line whose tokens mix `A<n>` indices with bare
  creature names. Note instances, do NOT inflate — 0 outcome cost this corpus.

- **HARNESS #2 — fallback class this corpus is `unparsed_reply` (long-reasoning-ramble guard); `empty_reply`
  = 0.** 8 `unparsed_reply` at seat: deck131 s12/s32, deck140 s15/s24, deck44 s32/s36, deck133 s9,
  deck109 s19. All self-healing; NONE changed a game. The two in even-life early positions (deck140 s15
  T8 20/20, deck131 s12 T6 19/20) were LAND-PLAY windows where the heuristic picked a land and the model's
  NEXT window cast on curve (deck140 s16 Treefolk, deck131 s13 Into the North); the rest are in
  lost/already-won positions. The wave-12/13a `empty_reply` flood (Spark dying) is CLOSED (0 at seat).
  NOTE: s19's ramble was CAUSED by ENGINE #2 (the board contradiction), i.e. a representation bug feeding a
  harness fallback via the latency cap — see ENGINE #2.

- **HARNESS #3 (CARRIED) — GPU_MEM_UTIL=0.75 latency → 2400s cap → LIFE-adjudication mis-scores board-
  winning positions.** Seat latency med 12.2s / p90 104s / max 210s (KV preemption under 3 concurrent
  games). Corpus-wide item (brief owns it); deck135's vs110 is the canonical "won-board-lost-adjudication"
  fixture (8/8 trample + deathtouch flier, opp at 10 with a lone tapped blocker, adjudicated a LOSS at
  9/10 T12). Do NOT read the seat's adjudicated losses as play-quality; DO read vs110 as a false loss.

## MODEL (route to model-experiments; NOT guide text)

- **NEW: summoning-sickness blindness in attack declaration.** The model repeatedly declares a just-cast
  creature (or a mana-only Boreal Druid) as an attacker the turn it entered (the 4 HARNESS #1 instances).
  The engine's "creatures that can attack" list correctly excludes them and the model TRUSTS that list at
  every genuine multi-attacker window elsewhere, so this is a minor representation tic, harmless (clamped
  every time). Cheapest durable fix is model-side; no guide line (the guide already says "count ONLY the
  creatures offered in the Attackers list").

- **colored-pays-generic false belief — FIRED then SELF-CORRECTED this corpus (softer than wave-10's
  actual decline, harder than wave-11's clean pass).** REPRO deck110 s5 (T4 cast window): first-draft plan
  *"I cannot cast Icehide Golem because I have no generic mana available… {W}"*, then within the same
  reply *"Wait… {1} is a generic cost. {W} can pay {1}. My previous thought process was flawed"* → cast
  Icehide Golem (choice=1). No wrong action. Model-UNSTABLE, not un-taught. CORE candidate stays PARKED;
  cross-seat grep for an ACTUAL offered-payable decline is the promotion trigger (general-suggestions #3).

- **Stale YOUR PLAN echo drives a reasoning-tax window when the plan's named action was already executed.**
  deck110 s6: after casting Icehide Golem at s5, the fetch-priority window still carries a YOUR PLAN that
  says "cast Golem"; the model burns a large reply confused about why the cast isn't offered (suspecting
  the engine "thinks {W} can't pay {1}") before correctly fetching a Forest. Outcome correct; a latency
  contributor. Same family as the wave-11 synthesis stale-echo churn note. Model/harness-side (the YOUR
  PLAN echo could be refreshed post-action); no guide line.

- **Azcanta-as-CREATURE hallucination (wave-11 Q5 watch) — did NOT recur.** In vs140 the model transformed
  Search for Azcanta (s33) and used Azcanta, the Sunken Ruin CORRECTLY as a card-selection land (`look at
  the top four`, s34-s36). The KEY CARDS micro-note watch does NOT harden; not adopted.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own gold / double-pip win creatures — STANDS.** Ice-Fang Coatl {G}{U},
  Ohran Viper {1}{G}{G}, Abominable Treefolk {2}{G}{U}, Diamond Faerie {2}{G}{W}{U} off a base with only
  2 Snow-Covered Island + 2 Snow-Covered Plains + fetches. This corpus: vs109 (real loss, T9) mull-6 with
  no early defense and a color-screwed hand; vs131/vs44 small bodies couldn't race lifegain / an evasive
  6/6. Pilot-side develop is addressed and validated (Rule #1 point 3; creatures on curve); the
  construction ceiling remains. Direction (user's call): raise effective double-pip sources, or shave a
  pip off the creature suite.
- **Slow clock is structural.** Abominable Treefolk is the only real closer and lands T6-T11; small bodies
  can't race lifegain (vs131 opp to 25) or fast aggro (vs109/vs110). Construction, not guide.

## FREEZE-CHECK (turnaround / OBEYED-BUT-LOSING mode; guide is a BYTE-IDENTICAL copy of the live guide)

The guide was NOT edited (`cmp`: byte-identical, 13357 bytes). No new guide-fixable class — every residual
routes to engine (Force resolution #1; stale-[attacking]/freeze #2; fetch re-ask #3), harness (attackers
name-parse; latency-adjudication), model (summoning-sickness; colored-pays-generic self-correct; stale
YOUR PLAN echo; Azcanta), or construction. Where each block stood this corpus:
- **RULE #1** — point 1 (creature on sight) OBEYED across all games (Golem T4 vs110, Treefolk on curve
  elsewhere). Point 3 develop-branch held (no dead-board class). Board-read/phantom-lethal anchor: 0 test
  windows = UNTESTED, KEEP at full prominence (pause-aware, do not demote off absence). FROZEN verbatim.
- **RULE #2 + WHO IS THE BEATDOWN** — combat healthy (Treefolk+Diamond Faerie closed vs133; profitable
  early blocks; Boreal-Druid carve-out held). FROZEN verbatim.
- **MANA** — fetch-crack clauses exercised and obeyed both ways (Forest short-green, Island short-blue).
  FROZEN.
- **KEY CARDS — Force of Negation** — strategically correct (the one offer declined correctly; round-3
  offer-legality now validated), CARD still engine-broken at RESOLUTION (ENGINE #1). Deliberately NOT
  changed (layer-routing). FROZEN verbatim.
- **KEY CARDS — Treefolk/Coatl uncastable anchors, MULLIGAN, LETHAL CHECK, DECIDING SITUATIONS** — no
  counterexamples this corpus (the develop/uncastable anchors did not leak; losses were construction/
  latency). FROZEN verbatim.
No frozen line dropped, moved, or demoted. Guide diff = zero bytes.

## VALIDATION TODO (for the next run)
1. ENGINE #1: engineer a NON-adjudicated Force-on-board-wipe window; confirm the wipe is COUNTERED and the
   blue card is spent for real effect (still 0 resolution data — 0 casts this corpus).
2. ENGINE #2: confirm the stale-`[attacking]`/freeze fix (when it ships): a Treefolk-frozen creature
   renders `frozen`/no `[attacking]` tag and NO reasoning-tax blowup at the next combat window.
3. HARNESS #1: watch for an `ATTACK: A1, <Name>` where <Name> IS a legal A2+ attacker (the first
   outcome-coupled instance) → promotes the name→index reconcile arm from latent to engine-batch.
4. colored-pays-generic: watch for a cross-seat 2nd deck's ACTUAL offered-payable decline (deck135 self-
   corrected this corpus; still parked).
The record is construction-bound + latency-adjudicated — do not read a 1/6 win-rate as a guide regression;
vs110 was a board-winning game robbed by the life tiebreak.
