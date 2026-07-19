# Wave-20 reviewer brief (shared context for all 7 deck agents + synthesis)

Sixth per-deck-rotation cycle. deck110 rotated out (THIRD converged exemplar; its guide stays
deployed as a canary). deck102 (Tergrid discard/sacrifice) entered GUIDELESS. deck27 (Zombies)
plays its FIRST GUIDED corpus (wave-19 wrote its initial guide). decks 14/62/35 play revised-guide
round N per their wave-19 continuity; deck49 is on a one-cycle rotation HOLD (its flood off-case
never got exercised — watch for it); deck135 continues on its frozen guide.

Binary 95e89c204 — delta since wave-19's 72b05535d:
(a) **ENGINE-R1 reveal partition fix**: driveInteractiveReveal now finalizes option-one picks
    SAME-TICK. The old bug: a fully-satisfied `<anyamount>` chooser got reaped before its
    deferred finalize -> zero-target decline -> optiontwo swept EVERYTHING to the graveyard
    (Glacial Revelation lost its whole partition). Live-verified 3/3 pre-fix broken, post-fix
    correct. The model's picks should now LAND as chosen.
(b) **Attacker-sparing mana payment**: both payment paths now prefer swing-neutral sources —
    the engine no longer auto-taps would-be attackers (e.g. Gemhide slivers as mana) when lands
    can pay. Root cause of the wave-19 "0-attacker durdle" at creature-mana seats was PAYMENT,
    not the combat gate.
(c) **Perception bundle**: salvageLoopedChoice extended to BLOCKS/ATTACK/PUT replies; `#N`
    ordinal disambiguation for duplicate creature names (the wave-19 deck14 "dropped block" was
    a model hallucination, but the real defect — discarded #N ordinals — is now fixed); combat
    prompts now carry options_text in the translog; reveal prompts surface ELIGIBILITY.
    Items (a)-(c) were code-path-verified only — THIS CORPUS IS THEIR LIVE VALIDATION.
(d) files[1024] registry-overflow fix (infra; no gameplay effect).

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260719-151849` — 21 games,
  round-robin over 135,62,49,35,14,27,102. Binary /tmp/wagic-95e89c204. Translogs are FLAT
  in the run dir: `<epoch>-ai_baka_deckN-<ptr>-vs-ai_baka_deckM.jsonl` (owner seat = first name).
- Win table: 135:4/6, 49:4/6, 35:3/6, 27:3/6, 14:3/6, 62:3/6, 102:1/6.
  0 timeouts, 0 draws, 5 life-adjudicated at cap (adj), 0 segfaults (7th clean corpus).
  (Wave-19 for reference: 49:5/6, 35:3/6, 62:2/6, 27:2/6, 135:2/6, 14:2/6.)
- Fallbacks 3.0% (32/1067): 29 unparsed_reply, 3 stale_echo, 0 defer, 0 empty.
  Per-seat: 135:7, 62:7, 102:6, 14:6, 27:3, 49:3, **35:0 (the new zero seat)**.
  **deck49's two-wave zero streak BROKE** — 3 unparsed, ALL at ask seq 8, vs three different
  opponents (27/35/62): almost certainly ONE recurring prompt shape. Identify it.
- TARGET CHOICE framing shown on 486 decisions.
- Decision kinds: ask 769, attackers 147, priority 82, blockers 61, gamestart 42, gameend 32,
  reveal 8. Latency median 11.6s, p90 78s.

## VALIDATION FOCUS (wave-20 engine batch — every reviewer)

- **Reveal partition fix (deck135's reviewer owns)**: 5 reveal records at 135 (incl one 'none').
  For EACH: chosen_text vs the engine `events` that follow — picks must now LAND (wave-19's
  suspect was engine moved index 1 when model chose 2,3). Any residual mismatch: seq + both
  sides verbatim. Note reveal volume dropped 18 -> 8 — draw variance or a regression in reveal
  TRIGGERING? Check whether Glacial Revelation was even cast in the 135 games.
- **Attacker-sparing (deck35's reviewer owns)**: Gemhide-sliver mana shapes. Did 0-attacker
  turns collapse vs wave-19? Any game where a would-be attacker STILL got tapped for mana with
  lands available: seq required.
- **#N ordinals / combat options_text / salvage extension (all combat seats)**: 8 combat
  fallbacks remain (blockers 6, attackers 1 + 1 priority-adjacent). For each at your seat:
  was it loop-salvaged (check the record's chosen_text vs reply), and is it a shape the
  salvage SHOULD have caught? Duplicate-name boards: did #N disambiguation fire correctly?
- **NEW SURFACE — discard-pick reveals (deck102's reviewer owns)**: 3 reveal records at 102
  are CHOOSE-FROM-OPPONENT'S-REVEALED-HAND picks (targeted discard): "Boomerang, Boomerang"
  (vs 14), "Glen Elendra Liege" (vs 27), "Arcum's Astrolabe, Ice-Fang Coatl, Into the North"
  (vs 135). Judge pick QUALITY (did it strip the best card?) and mechanical integrity
  (did the named cards actually leave the opponent's hand — check events).

## KNOWN CORPUS-WIDE (do not re-diagnose)

- Scry is still heuristic (aicode) for GPT BY DESIGN — do not flag scry quality.
- The 5 adj games ended at the turn cap by life adjudication — check whether YOUR seat was
  durdling in those; the cap itself is not a bug.
- Latency p90 78s is Spark queueing under 3 concurrent games — not a model-quality signal.

## Seat assignments

- **deck135 (4/6, frozen guide):** OWNS THE REVEAL PARTITION CHECK (above). Record 2/6 -> 4/6
  on a frozen guide — engine batch (reveal fix?) or matchups? 7 fallbacks again (5 in ONE game
  vs 27) — the giant-reply tax pocket or a new shape? Frozen-guide seat: verdict on whether
  the freeze holds.
- **deck62 (3/6, revised round 4):** 7 fallbacks incl 3 blockers-class — did combat salvage
  fire? Computed-P/T distrust pocket: dead or alive? Blanchwood/A-list carryover check.
- **deck49 (4/6, revised guide, ROTATION HOLD):** streak broke — characterize the ask-seq-8
  triple. Flood off-case: did any game exercise the mana-flood plan ("burn is your deck")?
  If still unexercised, say so — that's the hold's exit question. 5/6 -> 4/6 context.
- **deck35 (3/6, revised guide):** OWNS ATTACKER-SPARING CHECK (above). New zero-fallback
  seat — note what changed (the guide revision? options_text?). FC1 chump-durdle: still dead?
- **deck14 (3/6, revised round 2, Deep Blue):** 2/6 -> 3/6. Which wave-19 revisions fired?
  6 fallbacks incl 2 blockers — salvage check. TEMPO countermand doctrine: settled or still
  contested? The wave-19 hallucinated-Dragon class: any recurrence of invented board objects?
- **deck27 (3/6) — FIRST GUIDED (Zombies):** wave-19's initial guide validation — which taught
  classes fired/died? 2/6 -> 3/6. Recursion/grind read: did the guide's engine lines get used?
  Beat 135 AND 35 AND 49 — real lift or variance? Revise the guide only on decision evidence.
- **deck102 (1/6) — GUIDELESS FIRST PASS (Tergrid):** Step 0-bis doctrine. OWNS the
  discard-pick surface (above). Its one win was an adjudication over 27. Characterize the
  deck's raw failure modes (Tergrid = punish sacrifice/discard — did the model ever leverage
  Tergrid's steal trigger?); write the initial guide (format-match a live guide). 6 fallbacks
  — characterize.

## Doctrine (BINDING)

Win-rate is context; the unit of analysis is the DECISION with seq repros. Layer-routing
enforced. Single-seat items cannot add/cut core lines. NEVER propose hard-suppressing a legal
play on a strategy judgment (owner ruling — annotations only). Per-deck rotation: state your
seat's verdict explicitly in notes.md (no-guide-mod AND no-new-work-signal = rotation
candidate). deck49's reviewer must ALSO answer the hold's exit question explicitly.

## Output contract (per deck agent, sequential, in strategy-design/wave20/deckN/)

findings.md -> strategy.txt (byte-identical if frozen, REVISED if warranted; newcomer 102:
the NEW initial guide) -> skill.md -> general-suggestions.md -> notes.md (engine items +
rotation verdict). CURRENT skill = strategy-design/wave19/strategy-writing-skill.md. Guided
seats read their wave-19 deckN continuity files (strategy-design/wave19/deckN/).
