# Wave-5 synthesis notes (development notes for step 4 "review and address")

Corpus: one round-robin run `matchups-20260713-145947` (6 games/deck across decks 44, 109, 110,
131, 133, 135, 140). n=6/deck, single run — win-rate is noise; behavior metrics are the signal.
This file carries the cross-deck convergences, the engine/harness ledger reviewers agreed on,
outcome-accounting recommendations, and what the next wave inherits. The skill and general-strategy
revisions are the other two deliverables; `core-prompt-revision.md` has the exact core edits.

## Cross-deck convergences (the strongest merge signals)

1. **Representation beats instruction — DOUBLE-confirmed; the wave's dominant theme.** Three seats
   show the pilot correctly DERIVING a fact and then acting against it: deck110 seq84 (reads "already
   attached... waste of mana," equips anyway), deck44 (pending spell in the log tail, fabricates "the
   spell moved to the battlefield"), deck133 (plan/list disconnect: plans Gray Merchant not in the
   list, picks Cast nothing over an available Fatal Push). When wording is read and disobeyed,
   instruction has failed at full strength — route to representation, do not escalate. The stack
   surfacing that SHIPPED this wave is exactly the deck44 fix (vindicating the pattern); the
   attachment marker (task #26) is the pending deck110 A/B. The backlog should re-rank toward
   representation work — this is the second such confirmation after the mana line.

2. **THREAT-before-VALUE / cheapest-cast-satisfies-the-floor — 3 seats → shipped to core.** deck135
   (durdle on Search/fetch/Astrolabe over a body; opp ended 18-23 every loss), deck133 (discard-flood:
   cheapest Thoughtseize/Inquisition while raced), deck131 (Prism Ring value-sink in every durdle
   loss). deck133 explicitly predicted a core line justified at 3+ decks. Core Edit 2 completes the
   existing deploy/impact lines: the act-floor is satisfied by the highest-impact play, deploy the
   threat first.

3. **Transport layer is CLEAN across all 7 seats.** 0 head-vs-choice desyncs in every deck's corpus
   (deck110 0/247, deck131/135/133/44/109/140 all clean). The head-first parser is fully validated —
   do NOT revert. Guides shed their interim output-format stopgaps this wave (deck109 removed its
   block). Plan-echo splice is gone (deck135 scanned 0). No fallbacks in most seats.

4. **X-menu reversal validated (deck140).** 6/7 max-X picks, zero X=0 blanks (was 4/17 in wave 4);
   vs44's X=12 Rakdos's Return kill is the direct cause of deck140's 1/12 → 2/6 turnaround. Keep it.
   BUT it exposed a stale-stopgap hazard: guides still saying "pick the LAST line" now name X=0 —
   deck140 won only because the pilot IGNORED the guide. All X-deck guides must FLIP to value-anchored
   phrasing (skill P1 / TEACH THE MENU step 3).

## Engine / representation ledger (converged; ranked by breadth × cost)

- **[TOP, converged 3+ seats] Fetch/activation re-offer loop.** deck135 E1 (Flooded Strand re-asked
  5× across phases; contributes directly to a timeout), deck133 (single-option re-ask multiplier
  inflates decision count + wall clock), deck131 (pervasive, contributes to the vs135 timeout),
  deck44 #3 (Sleep-Cursed untap offered 29× in one game, incl. Blockers/Combat-damage phases). The
  life payment / land arrival lands only on the LAST answer of each run. Fix: trace the crack's click
  choreography at first offer, and/or suppress a re-ask when an identical option set was answered
  within the same turn and only the phase changed. Highest inference-budget drain in the pool.
- **[Attachment markers — task #26, deck110 primary witness] RE-EQUIP QUANTIFIED.** 35 equips / 15
  waste (~43%; was ~57% wave 4). The decisive repro is `1783980343` seq83/84 (read-then-disobey).
  Fix: annotate the option line `[ALREADY ATTACHED]` and/or add board markers `[equipped: Cranial
  Plating x2]` / `[attached to Steel Overseer]`, or suppress the already-attached equip line. This
  ALSO fixes the two-power-number heuristic's FALSE POSITIVES (lords, +1/+1 counters, battle cry all
  render two numbers with no equipment attached — deck110 general-suggestions). Resolves the line-35
  vs line-27 tension (a re-offered attached equip is a DONE action still offered).
- **[Mana-ability mislabel — 3 seats] "Cast Card Normally with Mox Opal / Mountain" reads as a spell
  cast.** deck110 E3, deck131 (vs110 seq8: `Cast Card Normally with Mountain`), deck133 E3 (Collective
  Brutality escalate menu emits malformed/duplicated-index lists with an opaque `Cast Card Normally`
  label). Relabel mana-ability activations to say what they do ("Tap Mox Opal for one mana"); clean
  up the modal-menu assembler. Guides carry the defusal stopgap until then.
- **[Gray Merchant drain undercount — deck133 E2] `{right now: drains N}` under-counts by 2** (excludes
  Gray Merchant's own {B}{B} devotion, since he is still in hand when the option is built; verified +2
  in 3/3). Render the drain as-if-resolved (+ the card's own contribution). Removes the guide's "real
  drain = shown N + 2" stopgap at once — and any similar per-deck correction on board-scaling payoffs.
- **[Hellrider cast no-op — deck109, PERSISTS] Clean A/B this corpus:** `1783977821` seq54/56 no-op
  (mana 7→7, stays in hand) vs seq57 resolve (7→4). Consistent with the wave-4 board-width hypothesis
  (no-op when creature-light). Keep the core cast-happened `hand -> stack` anchor and the guide
  fallback until fixed. Candidate: the cast path for a haste creature whose only trigger is on-attack
  when there is nothing else to attack with.
- **[Menu noise, low severity] {B}{B} attach line offered with no black source** (deck110 E2); empty
  `[counters:]` bracket renders on Sleep-Cursed Faerie once stun counters hit 0 (deck44 #4). Bundle
  with the #26 menu cleanup.
- **[Cheap] Turn-ownership stamping** on the choice-list header ("Your legal actions (YOUR Main Phase
  1):") — deck135 E2; the phase line sits far above the option list. One narration change.
- **Menu-adjacency index carryover** (deck140 P2): a bare number carried from the prior back-to-back
  menu onto the next (seq74→75). Immaterial this corpus; per-ask reset / distinct second-menu label if
  cheap. Low priority.

**NOW-SHIPPED — close these ledger items:** stack surfacing (deck44 #3 half → ON THE STACK section),
reaction-option labeling (deck44 #4 → "can target on the stack"), X-menu reversal (deck140), plan-echo
splice (deck135 E4 — confirm on full corpus then close), negative-life SBA (deck133 E1), head-first
parser (all seats). The counterspell reaction-TIMING half of #25 also appears closed: all 27 deck44
counter windows were live-pending (27/27 tell audit), and the wave-4-E3 "died with counters" defect was
ruled NOT a bug — it is mana discipline (window opens only when a response is payable; tapping out on
your own turn forecloses it). Update the ledger: #25 is fully resolved (representation + timing);
"died with counters in hand" is now a per-deck mana-discipline teaching item, not an engine item.

## Translog schema asks (converged across deck135, deck131, deck140, deck133)

- **`options` as a STRING ARRAY, not an int count** (all four seats) — the biggest per-wave toil
  saver; every reviewer re-regexes cast options AND the `A#.` attacker list out of the prompt.
- **`reask_of: <seq>` marker** when the same activation repeats (deck135) — makes the fetch re-offer
  loop measurable without prompt-scraping.
- **`realized` field** on ask records that resolve with a magnitude (deck140) — the actual X/drain
  announced; turns the X-regression check into a one-liner.
- **Per-seat game-id / opponent / result header** (or a pairing table in the run dir) — deck44 and
  deck131 both re-derived seat→game→outcome by epoch arithmetic (offset 1-5) + opponent-card signature
  guessing; ~7× duplicated toil per wave.

## Outcome accounting — timeout-as-loss adjudication (converged, actionable)

Reviewers separated three things `timeout` conflates, by hand, every wave. The fix is a consistent
adjudication rule + schema support:
- **Adjudicate capped games by life at cap:** ahead → WIN, behind → LOSS, static → DRAW. Under this
  rule, deck135's vs131 timeout (behind 9v23) is a LOSS → deck135's real record is 1W-5L; deck131's
  vs135 timeout (ahead 23v8) is a WIN. deck140 vs110 (ahead, opp ground to 7) is winning-but-slow;
  deck133 vs140 (my6/opp2 with a lethal attack on the last record) is a WIN. Latency-starved games
  (both players live, low turn, high latency — deck140 vs133 my2/opp7 at t20; deck133 vs140 latency
  spikes 64-74s) are NOT play defects — exclude or re-run, don't score as losses.
- **Emit a `gameend` record (or results.tsv columns) on timeouts** (deck131, deck133, deck140): add
  `end_reason` (stall/latency/cap), `final_life0`, `final_life1`, `turns`. Timeout games currently
  write NO closing record.
- **Add a stalemate detector** (deck140): end as `draw` with `end_reason=stall` after ~8 consecutive
  full turns with no change in life totals, battlefield card count, and hand sizes.
- **Latency vs the cap is a structural confound** (deck140, deck133): control mirrors generate 87-98
  decisions/game vs 24-46 for aggro; at the 2400s cap that is ~12s/decision, so any `-j` pushing
  Spark median past ~10s times out every control mirror regardless of play. EITHER size the cap by
  decisions consumed (cap = N_decisions × budget) OR pin decision-quality corpora at `-j ≤ 3`.
  deck140/deck133 (control) eat the timeout column for this reason, not for passivity.

## Deck-construction flags (for the user / decklist owner)

- **deck44** — 6 reactive cards (2 Counterspell + 2 Arcane Denial + 2 Go for the Throat) clog a
  "kill by turn 10" flyer beatdown; counter seam fires ~7%. Manabase skew 16 Islands / 10 Swamps vs a
  black-heavy spell suite → color-screw losses (vs133, vs109). Both WINS came from racing with bodies,
  zero useful counters. Trim counters toward cheap flyers; rebalance mana.
- **deck131** — #1 flag: 14 Island / 8 Mountain but every win-con is RED (Guttersnipe/Young
  Pyromancer); 2 of 6 losses had NO red source when it mattered. Move ~3-4 Islands → Mountains
  (blue spells are forgiving, mostly single-{U}). Threat density 6/60 is the structural ceiling; 4
  Prism Ring is pure lifegain filler (the flex to cut). Cannot beat lifegain (deck140) at any pilot
  quality — fine if deck tiers are intended.
- **deck133** — Gray Merchant density 4/60 with no tutors reaches castability ~half the games; it is
  the deck's only blocker-independent damage AND its stabilizer. Consider +1 Merchant or cheap
  selection. 8 one-mana discard spells (4 Thoughtseize at 2 life) are self-damage vs a racing pool.
- **deck135** — manabase is the standing tax: 2 Snow-Covered Islands / 2 Plains while {U} touches
  Coatl/Treefolk/Search/Force and Diamond Faerie needs {W}{U}{G}. A 3rd Island (over a 2nd Plains)
  defuses both the wave-4 Island-hunt and this wave's uncastable Faerie. Diamond Faerie ({2}{G}{W}{U},
  UEOT pump) is arguably mis-slotted for the GPT pilot.
- **deck140** — reach starvation is THE residual: 14 sweepers (proven right — do NOT cut), ~2 finishers
  (2 Blightsteel {12} land too late; 2 Rakdos's Return; 4 Staff of Nin = 14-20-turn clock). "Add
  teeth, not text": a 3rd/4th X-finisher, a cheaper repeatable drain, or a {4}-{6} recursive attacker.
  The pilot now plays it well; the clock is the ceiling.
- **deck109** — well-formed RDW, no changes needed (5/6). Soft note: no card advantage / mana sink
  once burn is spent; 20 lands slightly high, but the mana-light loss argues against cutting.
- **deck110** — toughness fragility is a hard RPS weakness (every creature toughness 1-2; Cranial
  Plating pumps power only). 2 Lightmine Field reduced it to a 33-turn timeout. Keep for roster
  tiering, but flag: deck110 is NOT a safe "always strong" slot. (Glimmervoid is a LAND, not an
  artifact — does not count for metalcraft; keep in the generator's card-notes.)

## What the next wave's reviewers inherit

- **The counter-tell is RETIRED core-side.** DELETE every guide copy of the counter offer-is-the-signal
  / log-tail tell; teach reading the ON THE STACK section instead. "Died with counters in hand" is now
  a MANA-DISCIPLINE leak — counter-deck guides teach holding counter mana open (a window opens only
  when a response is payable).
- **All X-deck guides must FLIP** the X-teaching's positional pick ("pick the LAST line" → the
  value-anchored "line whose value is largest / verify X = biggest number"). The reversal shipped; a
  positional line now names X=0. This is a live guide-sync task (deck140, and any other X deck).
- **Attacker-participation denominator = the engine's `A#.` offered list, never the battlefield**
  (deck131 P1) — or you manufacture a phantom subset-attack leak.
- **Factor out the fetch re-offer loop before reading timeouts / decision counts** — it inflates both,
  worst against lifegain decks (deck131/140 run Elixir/Prism Ring).
- **Representation-beats-instruction is now the default suspicion** for any read-then-disobey pattern;
  file the representation ask, don't escalate wording. The attachment-marker A/B (#26) and the
  Gray-Merchant display fix are the two clean representation experiments queued.
