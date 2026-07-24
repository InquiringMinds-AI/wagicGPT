# deck140 wave-23 — development notes (engine/harness; layer-routed, repros) + ROTATION VERDICT

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-013710`, 6 `*ai_baka_deck140-0x*`
seat files. Binary `0e6361732`. `choice` 1-based; `-1` = heuristic; `CHOICE: 0` = pass.
Opp creature count = `Opponent battlefield (creatures: N)`.

## ENGINE DELTA VALIDATED (route: confirmation, no action)

1. **[VALIDATED — (a) forced-loss framing] the FORCED discard/sacrifice OF YOUR OWN CARD
   header works.** Rendered on every own-card forced choice in vs102 (Smallpox/Necrogen
   Mists Pox deck) as an option prefix. The model picked its LEAST valuable every time: a
   LAND when offered (vs102 s5 Akoum Refuge over 5 spells, s8 Plains over sweepers/finishers),
   its most-redundant SWEEPER when no land was in the set (s13 Pyroclasm, s14 BSZ), keeping
   Damnation + both finishers. The wave-22 catastrophe (pitching Damnation then Pyroclasm — its
   two live sweepers — with a land available, then dying to that board) did NOT recur against
   the same archetype. Death was slow discard-attrition (24->-4 over ~T17-26), unrelated to any
   pitch; deck140 kept + cast sweepers throughout (Cruel Edict s9, Wrath s21). CONSEQUENCE: the
   guide's interim forced-discard crutch is now DOUBLE COVERAGE -> demoted in strategy.txt.
   REPROS: vs102 s5, s6, s8, s13, s14, s16 (all `chosen_text` are own-hand cards).

2. **[VALIDATED — (c) INDEX-WINS] wave-22 BSZ stale-echo cluster is GONE.** My X-menus this
   corpus (vs27 s36 BSZ X=5, s48 Rakdos X=6, s52 Rakdos X=8) all recorded `choice=1` TRUSTED,
   zero fallback. The wave-22 `CHOICE: N (Cast <spell> with X=N)` echo-vs-consumed-parent FP is
   absent — the well-formed in-range index is trusted directly. Better than wave-22 (there the
   heuristic argmax masked the FP; here no FP at all). REPROS: vs27 s36/s48/s52 (options `X = N`,
   choice=1, fallback null).

## ENGINE — mana-display friction (route: engine batch; the #1 remaining friction at this seat)

3. **[HIGH — latency + a real mis-read] "Mana available" does not expose black-source COUNT or
   true affordable X; the model re-derives from the land list, at length, and over-reads.**
   Two faces of the same representation gap:
   - **Black-source count unreadable.** vs137 (LOST 12-18): hand held Damnation {2}{B}{B} +
     BSZ {B}{B}{X} but only ONE black source in play (Badlands; the rest Plateau/Plains =
     W/R). The `Mana available` line does not say "1 black source", so the model re-derived
     its color base from the raw land list FOUR times inside a single reply before correctly
     concluding neither black sweeper was castable. These re-derivations ARE my seat's longest
     spirals (vs137 s7 12.5k, s12 11.2k chars; vs27 s13 13.1k). The conclusion was always
     correct — the cost is latency + the pilot's apparent "long tail," not wrong play.
   - **Over-optimistic lethal X.** vs27 s46 (T23): the model fired Rakdos's Return convinced
     it could set **X=20** for lethal (opp at 20), citing plenty of mana — but the X MENU then
     offered a max of **X=6** (its real affordable X after {R}{B}). It fired below-lethal on a
     mis-count. (No loss — it won the grind 7-6 — but a wasted-ish kill shot on bad mana math.)
   FIX (both faces): surface derived color availability on the `Mana available` line (e.g.
   "black sources: 1", or list producible colored mana counts), and/or ensure the pre-cast
   lethal reasoning sees the same X cap the menu will enforce. This is the corpus-standing
   "Mana available hides dual second colors" item — reaffirmed with fresh repros; it is now the
   dominant driver of BOTH my latency tail and the one Rakdos mis-read. REPROS: vs137 s7/s12,
   vs27 s46 (reply asserts X=20 lethal; s48 menu caps X=6).

## HARNESS (route: harness batch)

4. **[INFO — no action at my seat] empty_reply cluster = transient endpoint outage.** vs131
   s5/s6/s7 (`reply` len 0, `choice=-1`, latency ~2502-2503ms). All 3 on low-stakes opening
   decisions (2 land-drops + 1 single-option Elixir); deck140 WON that game 27-20; zero board
   impact. Post-corpus the endpoint accepts repetition_penalty fine, so server-side/transient.
   Pilot-correlation OPEN per brief, but nothing at my seat implicates the model or the pilot
   (HTTP-level fast-fail, not content). No fix owned here; noted for the fallback census.

## KNOWN / CONFIRMED — account for, do not re-diagnose
5. **Adjudication regime is legitimate.** 3 of 6 games hit the cap (vs131 +7 WON, vs27 +1 WON,
   vs137 -6 LOST); vs102/vs133/vs109 were REAL board deaths. Cap outcomes reflect life reality
   and robbed deck140 of no win. deck140 remains the corpus grinder (high decision counts / high
   latency, driven by item #3) but latency caused no loss.
6. **Pilot (e) at this seat: SAME.** No spiral lengthening (my p95 7277 / max 13107 are below
   both this corpus's 7784/15901 and wave-22's 8186/16882); no prose degradation; long tail is
   item-#3 mana re-derivation, not repetition. Full evidence in findings.md.

## DECK-CONSTRUCTION (user owns; flag only)
7. **The loss column is three structural mismatches, unchanged from wave-22:** (a) no early
   interaction vs hasty burn — vs109 mono-R raced it dead ~T8 (at 11 life by T5); (b) no answer
   to dedicated discard-ATTRITION — vs102 Pox emptied its hand to hellbent and ground it out
   despite correct sweeper play; (c) a FRAGILE BLACK BASE — vs137 lost purely because its two
   black sweepers ({B}{B}) couldn't be cast off one black source. Priorities unchanged: cheap
   early interaction / a stabilizer that survives to the Staff-of-Nin grind the deck WINS with
   (both wins this corpus reached the grind; all four losses did not), plus more black sources
   or fewer double-black sweepers. 14 sweepers is right; the deck needs to LIVE to turn 6-8 and
   reliably make {B}{B}.

## PER-DECK ROTATION VERDICT (brief-mandated; I was held as the top-ledger witness)
**ROTATE OUT — (a) validated and the guide converges. Honest verdict.** The rotation-in from
wave-22 earned exactly one durable payoff: it surfaced the forced-discard leak, the engine
absorbed it (the FORCED-LOSS header now teaches least-valuable at the decision point), and this
corpus proves the header WORKS against the same archetype that beat deck140 last cycle — the
model pitched lands then redundant sweepers, kept its finishers, and did not lose to a pitched
sweeper. With that fix landed in the engine, the wave-22 interim guide crutch is demoted to a
6-line marker nod, and NOTHING left at this seat is guide-refinement: the residual is ENGINE
(mana-display friction, item #3), HARNESS (a transient outage, item #4 — already resolved
server-side), and DECK-CONSTRUCTION (item #7, user-owned). The guide's CORE is again OBEYED and
load-bearing — the model cites SWEEP TRIGGER, #1-Rakdos-hold, and the forced-loss order verbatim
and wins both its non-structural games. "Rotate" means STOP RE-REVIEWING a converged seat, NOT
remove the guide (it is demonstrably load-bearing). Recommend synthesis DEPLOY the wave-23
strategy.txt (the demotion + three small observed-friction refinements), then rotate deck140 out.
