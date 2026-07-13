# Deck-140 wave-5 notes — everything that doesn't live in the other four files

Companion to findings.md / strategy.txt / skill.md / general-suggestions.md. Corpus:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-145947/` (6 games, single round-robin).
Record 2W/2L/2T (up from 1/12 in wave 4).

---

## For the ENGINE owner

### The X-menu reversal WORKED — close the loop, don't re-open it
The wave-4 headline fix (present the ANNOUNCE_X list largest-first + append "option 1 is the LARGEST
X" to the ask) is confirmed shipped and is the direct cause of this deck's turnaround. Regression
evidence: 7 X announcements this corpus, realized X at MAX in 6/7 (values 1,2,3,3,3,5,12), zero X=0
blanks (was 4/17 in wave 4); vs44 t35 fired Rakdos's Return X=12 for the game-winning kill (opp
12 -> -2). Do not revert or reorder.

### Small follow-up: menu-adjacency index carryover (low priority)
- **Repro:** file `1783980343-*-deck140-*.jsonl` (vs110), seq74 -> seq75, turn 26. At seq74 the model
  chose the cast menu's option 2 (Wrath of God). The X menu for Black Sun's Zenith armed immediately
  after; at seq75 the model replied a bare "2" again -> X=5 on a `1. X = 6 ... 7. X = 0` menu, one
  below max. Immaterial here (target was a 13/1 Skirge, toughness 1 — X=1 already kills), but it is a
  distinct shape from the value/index trap: a bare number carried from the prior back-to-back menu.
- **If cheap:** reset/relabel so a second menu arming in the same tick can't inherit the prior index
  (or make the X ask text distinctive enough that the prior cast index doesn't pattern-match). Not
  worth risk; watch the next corpus's PLAN-vs-chosen_text table before doing anything.

### No engine defect in Staff-of-Nin targeting
The wave-4 ping-waste is a solved prompt-layer item (35/36 pings to face this corpus). No engine-side
target filtering needed.

## For the HARNESS owner (both still open from wave 3/4 — now actively costing analysis)

### Stalemate detector + results.tsv columns
End the game as `draw` with `end_reason=stall` after N (suggest 8) consecutive full turns with no
change in life totals, battlefield card count, and hand sizes. Add `end_reason`, `final_life0`,
`final_life1`, `turns` columns. Without it, `timeout` conflates three things I separated by hand this
wave: latency-starved games (vs133), winning-but-slow games (vs110), and genuine can't-close stalls.

### The -j latency interaction is biting control mirrors hard
- Median per-decision latency this run was ~8-10s across all deck140 games (baseline ~4-5s solo), with
  spikes to **72s / 67s / 63s** (vs133) and 64-74s elsewhere — the `-j` concurrency saturating Spark.
- **vs133 (repro `1783972794-*`)** died at t20 with 87 records, both players live (my2/opp7). This is a
  latency-starved pseudo-timeout, NOT a play defect.
- Budget math: a control mirror generates 87-98 decisions/game vs 24-46 for aggro; at the 2400s cap
  that is ~12s per seat-decision, so any -j level pushing median past ~10s structurally times out
  every control mirror. **Either size the cap by decisions consumed (cap = N_decisions x budget) or
  pin decision-quality corpora at -j <= 3.** deck140 is a control deck and eats the timeout column for
  this reason regardless of play.

### Translog schema asks (unchanged from wave 4, still the costliest gaps)
- Emit the option TEXT ARRAY per record (`options` is still just a count — offered-vs-taken tables
  need the texts regexed out of the prompt).
- In `gameend`, include per-kind decision counts and fallback count.
- A `realized` field on ask records that resolve with a magnitude (the X actually announced) — would
  make the X-regression check a one-liner instead of a prompt-scrape.

## For the DECK BUILDER — deck140's reach is the residual (unchanged from wave 4, now better bounded)
With the X fix landed, deck140 played WELL and still went 2/6 — both losses and the winnable timeout
were reach starvation, not piloting:
- **vs109 (L):** opp at 20 until t14, finished at 18; the deck swept ~8 times and never had a clock.
- **vs110 (T):** winning-but-slow, opp ground to 7 by Staff pings alone; Blightsteel ({12}) arrived
  t32, too late to swing.
- **vs135 (L):** the only sweeper in hand vs a 9/9 Treefolk was Pyroclasm (2 dmg) — pure variance.
- The two WINS both needed the reach to actually arrive: vs44 (Rakdos's Return X=12 kill),
  vs131 (Blightsteel at t32).

Win conditions: 2x Blightsteel {12} (drawn+cast in 2/6 games, always too late without ramp), 4x Staff
of Nin (1 dmg/turn — a 14-20 turn clock from stabilized), 2x Rakdos's Return (the only burst; the
guide now conserves it for the kill). The 14-sweeper density is proven right for this pool — do not cut
wipes. **Add teeth, not text.** Options in spirit (creatureless RBW control): a 3rd/4th
Rakdos's-Return-class X finisher; a cheaper repeatable drain/damage engine to complement Staff; or a
{4}-{6} resilient/recursive attacker that survives its own sweepers (Blightsteel is the template but
{12} with zero ramp lands too late). Any of these moves the clock more than a guide line can. Expect
deck140 to stay the pool's slowest closer until it gets faster reach.

## For the MODEL-EXPERIMENTS track (qwen35 pilot profile, this corpus)
- **The wave-4 channel-shaped failure (plan-then-unexecute at the menu index step) is resolved for X
  menus** by the engine reversal — the model's PLAN prose was sound and now the reply-encoding lands on
  max X. 0 desyncs / 399, 0 fallbacks. When evaluating other models, the X trap is no longer a
  differentiator on this deck; score PLAN-vs-realized separately as before, but expect it clean here.
- **First-option bias remains real and now HELPS:** with the menu reversed, option 1 = the best value,
  so the bias lands correctly. This is the P4 "usually-correct option first" convention paying off —
  keep every new numeric menu ordered best-first.
- **Instruction-following is strong enough that a STALE guide line is a live hazard, not just dead
  weight:** the pilot fortunately followed the engine surface over the guide's inverted "pick the LAST
  line", but a model with tighter guide-adherence would have blanked its kills. Keep guides in sync
  with engine surfaces (skill.md P1).
- **Latency tail dominates outcomes** (72s spikes killed vs133); the harness budget math above binds
  before any model-quality question.

## For the NEXT deck140 REVIEWER (wave 6)
- **Day-one scripts** (rewrite fresh, ~30 lines each): (1) per-game digest (outcome map from
  results.tsv + kinds/fallbacks/maxturn/gameend per file); (2) the X-table — every `chosen_text`
  starting "X =" with the menu's option-1 value and the PLAN's stated number (the regression check for
  the reversal — confirm max X still lands); (3) Staff-ping tally (face vs creature-name); (4)
  Rakdos's-Return timing table (turn cast, X chosen, opp life at cast) — the wave-5 hold-for-kill
  check.
- **Do not re-litigate:** X value/index trap (engine-fixed), Staff targeting (35/36 face), pool-panic
  (correct declines), sweeper-table choices (followed correctly three waves running), stale-plan
  poisoning (not observed).
- **Watch next:** did the hold-for-kill rule (strategy.txt THE #1 RULE) stop the early-chip
  Rakdos's Return, and did it convert winnable timeouts (vs110-class) to wins — WITHOUT tipping into
  hoarding? Read the Rakdos's-Return timing table first.
- **Timeout column:** read latency_ms before attributing any timeout to play (findings.md Finding 5).
