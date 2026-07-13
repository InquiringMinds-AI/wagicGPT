# Deck-140 wave-4 notes — everything that doesn't live in the other four files

Companion to findings.md (review), strategy.txt (revised guide), skill.md (method proposals),
general-suggestions.md (core-prompt/layer routing). Organized by audience.

---

## For the ENGINE owner

### Work order 1: X-menu presentation reversal (the wave headline fix)
- **Where the menu is built:** `src/DecisionContract.cpp` ~lines 165-197 — two sites build
  `req.optionTexts` ascending (`"X = 0"`, `"X = 1"`, ...) with the explicit contract
  invariant **option index == X value** (also documented at `include/DecisionContract.h:41`
  and `:85`). `applyMenuChoice` (same file, ~line 224) validates and consumes `act.choice`
  under that invariant, as does the underlying AAWhatsX menu.
- **Where the model sees it:** `src/AIPlayerGPT.cpp:1760-1774`,
  `AIPlayerGPT::chooseMenuAction`, ANNOUNCE_X branch — `askModel(...)` renders optionTexts
  as a 1-based numbered list, so the model sees `1. X = 0` ... `N+1. X = N`.
- **Recommended change — reverse at the ASK, not in the contract:** do NOT reorder
  optionTexts in DecisionContract (index==X is a load-bearing invariant with at least three
  consumers). Instead, in the ANNOUNCE_X branch of `chooseMenuAction`, present the list to
  the model LARGEST-FIRST and map the picked index back to the true X before setting
  `act.choice`. ~6 lines, GPT-seat-only, no contract or Baka impact, staleness gate
  unaffected (it compares the original optionTexts).
- **Why largest-first is correct, not just convenient:** the project's own prompt-ordering
  convention is "the usually-correct option first, declines LAST" (shipped P4, 2026-07-09).
  The current X menu inverts it — X=0 (the worst legal value, a guaranteed blank) is option 1,
  where the model's measured first-option bias lands. Observed cost: realized X below the
  model's own PLAN-stated intent in 15/17 X decisions across 12 games; 4 collapsed to X=0
  (details + game citations in findings.md Finding 1). After reversal, the same two failure
  modes (pick option 1; reply the value as the index) both land on LARGE X instead of blanks.
- **Belt-and-suspenders (do together):** append one fixed protocol line to the ANNOUNCE_X
  ask text in the same function: "Reply with the OPTION number, not the X value itself."
  With both in place, the per-guide X teaching (deck140 strategy.txt THE #1 RULE) becomes
  removable next wave — check the next corpus's PLAN-vs-chosen_text table first.
- **While there:** audit other numeric menus for the same worst-option-first shape (damage
  splits, "choose a number" effects, counter distribution) — anything built ascending from
  zero has the identical trap.

### Smaller engine note
- Staff of Nin-class pings: no engine defect — the targeting waste (10/50 pings at
  2-toughness creatures) is prompt-layer; see general-suggestions.md Proposal 3 (N >=
  toughness clause) before considering any engine-side target filtering.

## For the HARNESS owner

- **Stalemate detector + results.tsv columns (wave-3 ask, now blocking analysis):** end the
  game as `draw` with `end_reason=stall` after N (suggest 8) consecutive full turns with no
  change in life totals, battlefield card count, and hand sizes. Add `end_reason`,
  `final_life0`, `final_life1`, `turns` columns. Without it, timeout conflates three things
  I had to separate by hand: genuine can't-close stalls, latency starvation, and slow-but-live
  games.
- **The -j latency interaction, measured this corpus:** two of deck140's six "timeouts" were
  latency-starved games progressing normally — R2v133 died at t14 averaging 14.9 s/decision
  with a 52 s spike; R1v133 at t8 with a 27 s spike. These runs' concurrency pushed
  per-decision latency far above the ~4-5 s solo baseline.
- **Budget arithmetic for the 2400 s cap:** control mirrors generate 90-100 decisions/game
  (R2v131 = 98 records, R2v135 = 91) vs 24-36 for aggro games. At 2400 s, a 95-decision
  control mirror affords ~25 s/decision TOTAL (both seats share the wall clock, so ~12 s per
  seat-decision) — meaning any -j level that pushes median latency past ~10-12 s structurally
  times out every control mirror regardless of play quality. Either size the cap by decisions
  consumed (e.g. cap = N_decisions x budget) or pin decision-quality corpora at -j <= 3.
- **Translog schema asks (beyond end_reason):** (a) emit the option TEXT ARRAY per record —
  `options` is still just a count; offered-vs-taken tables need the texts regexed out of the
  prompt (wave-3 ask, still open, still the single most annoying gap); (b) in `gameend`,
  include per-kind decision counts and fallback count so per-game summaries need no second
  pass; (c) a `realized` field on ask records that resolve with a magnitude (the X actually
  announced, the mode taken) — the PLAN-vs-realized comparison that found the X trap would
  become a one-liner.

## For the DECK BUILDER (deck140's reach problem)

Even with the X fix, expect deck140 to stay the pool's slowest closer — judge it after a
post-fix corpus, but the structural math: win conditions are 2x Blightsteel {12} (drawn and
cast in only 2/12 games), 4x Staff of Nin (1 dmg/turn — a 14-20 turn clock from stabilized),
2x Rakdos's Return (the only burst; X fix restores it). In all 5 losses the opponent finished
at 14-20 life; the one win needed t50 and 181 life. If post-fix win rate stays floor-level,
the deck needs teeth, not text. Options in spirit ("Wipe Them Out" - creatureless RBW
control): a 3rd/4th Rakdos's Return-class X finisher, 2x more Staff (already 4 - so e.g. a
planeswalker-free repeatable drain), or one cheap resilient threat that survives its own
sweepers (Blightsteel is the template but {12} with zero ramp is drawn too late; 2 more
copies OR any {4}-{6} indestructible/recursive attacker changes the clock more than any
guide line can). The 14-sweeper density is proven right for this pool — don't cut wipes,
add reach.

## For the NEXT deck140 REVIEWER (wave 5)

- **Day-one scripts** (mine are in this session's scratchpad; ~30 lines each, rewrite fresh):
  (1) per-game digest: outcome map from results.tsv + kinds/fallbacks/maxturn/gameend per
  file; (2) the X-table: every `chosen_text` starting "X =" vs the PLAN's stated number —
  THE regression check for the headline fix; (3) Staff-ping tally (face vs creature-name);
  (4) "Cast nothing" reply-reason grep (no-mana/tapped substrings) — the pool-panic gauge,
  was 55% wave 3, 11% wave 4.
- **What to expect if the fixes land:** X values at max -> watch for the NEXT bottleneck
  downstream (skill.md P8): Rakdos's Return sized right but aimed when? Cast at 6+ lands or
  hoarded? Blightsteel cast but does it survive the opponent's answers? Follow the chain to
  the effect resolving.
- **Do not re-litigate:** payability/pool-panic (fixed, keep the compressed guide line),
  stale-plan poisoning (not observed wave 4), sweeper-table choices (followed correctly
  every observed sweep, two waves running).
- **Timeout column:** read latency_ms before attributing any timeout to play (findings.md
  Finding 3).

## For the MODEL-EXPERIMENTS track (qwen35 pilot profile, from 12 games)

- **The signature defect is plan-then-unexecute, and it is CHANNEL-shaped, not
  reasoning-shaped.** The model's Magic reasoning in PLAN prose was consistently sound:
  correct sweeper selection every observed time, correct X sizing ("X=3 ... destroying them
  immediately", correct lethal identification), correct target logic. The failures happen at
  the reply-encoding step — mapping an intended VALUE onto a menu INDEX. Implications:
  (a) a structured-reply protocol (e.g. `CHOICE: <n> X: <v>` with harness-side
  reconciliation, or response_format json_schema — the Spark stack supports it) would likely
  eliminate the class without any model change; (b) when evaluating other models, score the
  PLAN-vs-consumed delta separately from decision quality — a "smarter" model with the same
  index confusion will look identical in win rate.
- **First-option bias is real and measurable here:** 4/17 X collapses to option 1
  specifically; also visible in ask records where option 1 is taken with thin reasoning.
  Menu ORDER is an active lever on this model — any menu's option 1 should be the best
  default (the engine convention already says so; enforce it on new menus).
- **Instruction-following is strong enough to carry token-level rules:** wave-3's
  quote-and-forbid line ("NEVER pick Cast nothing with the excuse 'I have no mana'") cut the
  behavior 55% -> 11%; the sweeper table is followed verbatim. Guides work on this model —
  invest in them.
- **Latency tail matters more than the median for game outcomes** (52 s spikes killed a
  game); if experimenting with bigger/slower models, the harness budget math above binds
  first.

## Process notes for the coordinator (next wave)

- Pre-supply per deck: the outcome map (results.tsv already parsed to file-epoch ->
  opponent/result), and the translog option-texts fix if it lands — those were the two
  costliest setup steps.
- The five-file deliverable set (findings / strategy / skill / general-suggestions / notes)
  worked; the sequencing (review before guide before method) is right — each task consumed
  the previous one's evidence directly.
- One flag for synthesis: my skill.md P6 (negative-framing bleed) and the coordinator's
  controlled A/B are the same lesson from two sources — merge them as one rule with both
  evidence lines.
