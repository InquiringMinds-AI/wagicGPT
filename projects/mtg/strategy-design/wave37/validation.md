# Wave-37 validation — focused pass: deck152 / deck146 / deck139 + mechanical arrivals

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-194911/` — binary 5104a2433,
pilot qwen36-35b-a3b (thinking), 42 seat logs, 1,261 decisions. 20/21 games completed
naturally; the 152v36 game CRASHED at t6 (engine bug, `wave37/crash/README.md`) and is
EXCLUDED from completeness claims — its partial log (152 seat s1-s10) was read as decision
evidence only. A rerun of 152v36 launched 22:51 (`matchups-20260822-225149/`) was still
IN FLIGHT at review time and is not included; fold it into the 152/36 ledgers when it lands.

Fallbacks: 3/1,261 (0.24%) — unparsed_reply 1 (146v152 s21, priority: prose reply, no
CHOICE line), reasoning_only 1 (105v152 s11, empty reply after thinking), stale_echo 1
(146v36 s24: Command MODE menu answered with an echo of the prior cast answer — the
staleness gate caught it, heuristic answered; see NEW OBSERVATIONS). parse_note: 1
corpus-wide (158v105 s4 `echo_index_conflict` — lane-B visibility working as designed).
reasoning_degenerate: 0 (all scores ≤0.009). No degenerate loops at any seam.

## Mechanical arrival confirmations (the wave-37 code-lane predictions)

1. **Grazer-put tapped shocks: CONFIRMED, 0 model asks (predicted 0).** 2 put-events
   corpus-wide, both auto-declined with the truthful narration — citation (139v152 t1):
   "Stomping Ground entered tapped (the effect that put it onto the battlefield entered
   it tapped; paying life could not untap it, so the payment was declined)". The 18
   surviving pay/tap model asks are ALL hand-played land drops where "enters UNTAPPED"
   is true (checked 18/18 for put context: 0 hits). The wave-36 lying-narration class
   (≥7 occurrences) is at 0. deck139's ledger is no longer contaminated.
2. **damageTargetVerdict: CONFIRMED, 9 renders (wave-36: 0).** All on target asks where
   damage targets creatures — citation (158v152 s11 t7): "Wolf (3/3) ... {right now:
   takes 5 damage - DIES}". Distribution: 158 seats 7, 139v116 2.
3. **Intrepid partial-pay note: CONFIRMED, 2/2 renders where exercised.** Both Intrepid
   Adversary valor menus (152v146 s20 t11; 152v36 s9 t4): "NOTE: the counters are paid
   for ONE AT A TIME ..." present; pilot chose "add 1 counter" both times. The lowercase
   "add N counter(s)" matcher fires live.
4. **[eligible] header/tag match: CONFIRMED, 9/9.** Every prompt with the eligibility
   header also carries the per-card `[eligible for "<option one>"]` tags (0 headers
   promise a marker that doesn't render — the wave-36 10/10 defect is dead). Citation
   (116v152 s12): header "Pick ONLY from the cards marked [eligible for \"choose
   card\"] below" + matching per-card tags.

## deck152 (NEW revision under test) — verdict: FREEZE WITH WATCH

- **Trade-drift (restate-then-override): 0/5 blockers asks.** The two dies-line
  confrontations both agreed with their parentheticals: vs158 s21 t9 (Orc army 9/9,
  B1 dies-line) → "BLOCKS: none" + Fateful Absence plan; vs139 s23 t14 → a DELIBERATE
  chump (reasoning: "Katilda dies (1 vs 2)... I go to 12" — parenthesis restated and
  OBEYED, no override). Tell-phrases: 0 occurrences. Target 0 met.
- **RETURN-SWING: 1/1 armed instances handled correctly.** Sole arming (vs139 s38 t19,
  life 2 vs Everquill Phoenix 4/4): pilot attacked for 18 vs opp life 7 — the
  lethal-now release, and it WON that turn. No death-to-swingback anywhere (152: 4-1-
  crash record). Growth-term armings: 0 occasions (no counter-grown enemy reached the
  threshold; vs158 the check ran unforced at s14/s20/s29 anyway). Un-exercised in its
  hard form; nothing contradicts it.
- **Katilda-vs-infect: UNEXERCISED.** vs105 was won 20-0 with zero blockers asks —
  the precedence sentence never had an occasion. No deliberation-loop side effects at
  the attackers seam (0 degenerate, latencies normal).
- WATCH (not a drift-class event): vs139 s23 chumped Katilda into Dryad to save 2 at
  life 14 while the standing plan wanted Katilda attacking — value-poor chump, honest
  parenthesis handling. One line of Katilda-chump scoping if it recurs.

## deck146 (NEW revision under test) — verdict: FREEZE

- **Hold-back gate: 1/1 fired at its ask with the current-power sum.** vs116 s33 t14
  (life 14): reasoning ran "Total opponent power: Emrakul (15) + Beast (3) = 18" at the
  attackers ask and answered "ATTACK: none" to preserve blockers. 21 other attackers
  asks: no unsafe attack into a standing armed board (the s29-class miss is gone; the
  tapped-inclusion language appears in traces corpus-wide).
- **Chump anchor: fired, "either way" banned phrase 0 as self-derived claim.** Both
  "either way" reasoning hits are the pilot QUOTING the guide's ban and complying:
  vs116 s15 t9 (forecast 9 ≤ 10 → blocked the biggest non-trampler, rule cited
  verbatim); vs158 s20 t13 (life 5 → blocked, "NEVER write 'I take the damage either
  way'" quoted). All ≤10-forecast blocker asks blocked (vs36 s18/s22/s33, vs158
  s17/s20/s25, vs116 s40): 7/7.
- **MAIN-2 pump: 0 recurrences, 3/3 Command casts clean.** All three casts in MAIN 1
  (vs139 t10 pump+draw → pumped Adventurer, attacked same turn; vs36 t16 M1 cast,
  attack followed at s26; vs158 t12 return+sacrifice, no pump). No "pump now, attack
  next turn" plan anywhere.

## deck139 — verdict: FREEZE (its gate is met)

Gate was: engine shockland fix live + one clean corpus. Fix confirmed (item 1 above);
deck139 played 6 complete games. **Mutate over/under: 8/9 correct** including the exact
wave-35 shape (Gemrazer 4/4 OVER onto Greathorn 3/4, vs152 s34-36 ✓; Greathorn UNDER
onto 4/4 pile ✓ twice; UNDER onto Snapdax pile ✓; Grazer-class unders vs105 ✓). The 1
miss (vs152 s30-31) is NOT an over/under rule failure: reasoning correctly derived
"over if Dryad host, under if Gemrazer host", chose "over" intending Dryad — then the
NEXT ask's host pick took Gemrazer, shrinking the 4/4 pile to 3/4. Root: the engine
asks over/under BEFORE host, and the intent didn't survive one ask. See NEW
OBSERVATIONS; deck-lane text already teaches this ("Decide which host you will pick
(that question comes next)") — this is an ask-ordering/intent-carry residual, not a
guide gap worth un-freezing over.

## Frozen seats (105/158/36/116) — grep-level scan: NO un-freeze candidates

- deck105: ≤13→BLOCK held 3/3 (t18 life 7 ✓, t15 life 7 ✓, t16 life 8 ✓); every ≤13
  blockers ask blocked. Ichorclaw class: opposing seats also handle it (146v105 s15
  declined the Ichorclaw feed correctly).
- deck116: 0 dropped_assignments corpus-wide (all multi-block declarations legal).
- deck36: 16 Academy-near-land reasoning contexts, 0 misuse of the taps-for-spells
  clause. deck158: no doctrine failure visible; 4-1 record, no fallbacks in its seats.

## NEW observations / defects

1. **152v36 crash (t6, ALord/AAMover/Player::opponent SEGV)** — already docketed with
   core + repro plan in `wave37/crash/README.md`; the wave's one completeness loss.
2. **Command mode-menu stale echo** (146v36 s24): after answering the cast ask, the
   pilot re-emitted "CHOICE: 1 (Cast Silverquill Command)" at the 8-option MODE menu.
   Caught by the staleness gate (fallback, self-healing) — 1 occurrence; watch item,
   not actionable at n=1.
3. **Mutate host-intent carry** (139v152 s30-31): over/under asked before host choice;
   a correct over/under conclusion predicated on host A was followed by picking host B.
   Candidate engine-lane item: bundle host+over/under, or render the pending over/under
   answer into the host ask. n=1 this corpus (8/9 clean).

No regression from any wave-37 fix. All four code-lane predictions confirmed.
