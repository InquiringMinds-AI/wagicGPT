# Wave-54 review carry-list (assembled 2026-09-03 at vpk13 build / corpus prep)

Corpus: eleventh fair-hand corpus. Binary = hermetic `make -B` of master **caf023dbc** (wave-54
step-1: engine lanes A e21f81de5, B 351ad0098, C a2bc290d0, D 6c1555bb1, E 5fe998414,
F 3329c40f1; wave-53 lane AA 37b7951d3; audit lanes G 6dee5d2cd, H e078c2501, I 85c703453,
J fbd46fa04, K afd88acd0, L 27f867152, M ba784277f, N c2f34b05a; owner O1/O2 1afe6c8f6).
Gate on caf023dbc: suite 1226 (2 failed = lifeline + merrow_reejerey, the known concurrency-only
pair), 51 AI/0, 0 timeouts, PARSETEST 2489/0. Archived `archives/wagic-caf023dbc-w54step1`.
Model: qwen36-35b-a3b on Spark :8084. Guides = wave-53 reviewer revisions (baf36b936) + boundary
pass (a0067bae5). Same pool/recipe as wave 53 (-j 21, -T 0, WAGIC_GPT_TIMEOUT=900,
MAXTOKENS=6000, REPPENALTY=1.05). vpk13 = psp-port at the same commit.

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Adjudicate
render predictions by the emitter's actual string. Baselines from wave 53 (20/21 — the 152v125
hang is a FAILED test, excluded from every rate): 3,572 records / 3,483 decisions, 15 fallbacks
(4 reached Baka), max seat 607, max prompt 32,068, 496 `- Paid` / 0 no-source, 1,052
opponent-turn casting windows / 58 casts, identical-declined runs 26.2% of decisions (147.4
min), HOLD row 1,173 renders / 29 takes, `hold_windows_skipped` non-zero on 2 of 40 gameends.
**D1 discharge = 21/21 natural.** Lane F's no-progress supervisor now stops a provably dead game
alone (verdict `hang`, run ends CORPUS INCOMPLETE) — a `hang` verdict is a FAILED test, never a
result (invariant 00).

## Standing obligations from wave 53 (synthesis-notes "For the next wave" + skill #187-#208)
- #188 three-number audit per (literal, card family) read from the ROWS, before scoring.
- The B1 REVERSE audit: not "did my literal change" but "did a lane make one of my SENTENCES
  false" — every guide, every lane below.
- #177 per-EDIT prose-vs-DECIDING-SITUATIONS diff with the #190 ask-kind step 0.
- #136 checklist BEFORE the corpus for the six re-keys guides were waiting on: D4 (player in the
  kill summary), D5 (modal live/dead clause), D10 (cost-form DRAW PRICE tail), D17 (blocker-count
  header), D18 (menu-fit clause), D21 (the ahead-on-life clause is now gated — sentences that
  quote it as unconditional are stale). B3-B9 name the sentences.
- deck130 mulligan section: #132-UNTESTABLE-AT-THIS-SEAT (skill #200) — report offers/ships.
- Report opponent-turn declines and casts SEPARATELY at every seat, plus `hold_windows_skipped`
  and HOLD takes in windows carrying the declined-note at N >= 3.
- Guide sizes: pool 39.6-70.2 KB, no file in the skill's stale 27-38 KB band (amendment 200(c)
  at skill:11995) — synthesis owns the band sentence; measure per seat.

## Set B - engine changes since the wave-53 corpus, each with its falsifiable prediction
- A (D2 a-d, D12): `CHOICE: 0 (Hold priority)`-shape echoes execute the hold row or are stamped
  (`hold_row_named` / `pass_hold_ambiguous`) N/N (was 0/2); HOLD takes tripping
  `plan_choice_conflict` 0 (was 5); `hold_windows_skipped > 0` on every gameend where the row was
  taken N/N (was 2/6); HOLD takes in declined-note N>=3 windows >= 20% (was 0.4%);
  identical-declined runs < 15% (was 26.2%); opponent-turn casts >= 90% of 58. Row text is now
  `Pass priority, and do not ask me again this turn unless the board changes (any change
  re-opens this window; you give up no cast)`. Served PLAN blocks > one paragraph 0; stale-plan
  decisions with no `"<card>" is no longer on your menu` clause 0 (was 2). WATCH:
  `pass_hold_ambiguous` has no field evidence — a shape created by the reword.
- B (D13/D14/D15/D9): `latched_row_mismatch` on every record whose index AND parenthetical both
  differ from the executed row (N small; 0 also passes; a stamp on a legitimate name-over-index
  remap is a FAIL); `plan_contradicts_noop_row` on no-op-row + contrary-PLAN records N/N (was 0/4),
  none on live-magnitude rows; replies naming a card that occurs only inside an annotation
  executing a row 0 (was 1) — watch `stale_echo_in_range`/`named_row_not_offered` for a rise on
  legitimate names; answered replies at >= 95% of the deadline without `long_reply`/`deadline_pct`
  0 (was 1 of 6).
- C (D4/D10/D11/D18/D5): rows whose enumeration includes a player and whose `{kills ...}` summary
  does not say so 0 (was 26); summaries saying "legal targets" meaning creatures 0; player /
  planeswalker rows in a damage target ask with no `{right now: ...}` verdict 0; `[DRAW PRICE:`
  rows of either form with no `- you would be at K` 0 (was 58/85), Forced Fruition lethal tail
  must stay 16/16, casts from a `KILLS you` row 0; unconditional-removal rows with `{kills:}` /
  `{removes:}` N/N (was 0/244), none on an INDESTRUCTIBLE target; priced rows on a multi-priced
  menu with no fit clause 0; modal cast rows with `{modes live right now: ...; Dead: ...}` N/N
  (was 0/28), CHOOSE_MODE sub-menus unmarked 0. NOTE: D18's fit clause is a COUNT claim, not a
  colour claim. REFUTED on disk by lane C: D4 repro B (`130v162` seq 62 DID list the opponent;
  the defect was the denominator's scope). NEW ITEM: the `Choose one mode for Silverquill
  Silencer:` surface is `auto=chooseaname` (a card-NAME menu), not modes — unannotated, 9/9.
- D (D3/D6/D8/D25): loss-framed asks whose destination is the battlefield/hand 0 (was 8/8; new
  header `CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD ... Pick the card you MOST NEED`);
  `your own effect` on a permanent the seat does not control 0 (was 18/26; the opponent's
  annihilator now prints the `FORCED SACRIFICE` branch); renders repeating one ordinal-free row
  > 2x 0; de-duplicated-length-1 asks 0 (was 16) with `identical_option_asks_resolved > 0` on the
  gameend; a land below a non-land in a forced-loss list 0. NOT SHIPPED: the two-dimensional
  ability-menu collapse (28-row Staff of Nin = 2 sources x 14 targets) — carry as its own item.
  Lane D notes the forced-sacrifice mega-menu was already collapsing on c628dd531; those prompts'
  size is the game-log share (D32).
- E (D21/D17/D16/D20/D19): `while ahead on LIFE` at a record where `my_life <= opp_life` 0 (the
  corpus's only render FALSEHOOD); attackers prompts without `They have N untapped creatures
  able to block ...` 0 (N is an upper bound — counts creatures able to block SOMETHING);
  `would not cover any spell` renders without a cause clause 0; colourless fetch rows 0 except
  lands whose colour flags read empty; `ability: X's X` name-twice stack lines 0 (was 53).
- F (D7 a-c, D38): records with `choice: -1` and no `fallback` 0 (was 1; class
  `engine_answered`); any game whose two seats write nothing for > 60 min while the harness runs
  is stopped by the supervisor with verdict `hang` (0 expected); reveal force-close after 20,000
  no-progress ticks AND 600 s wall (budgets chosen, never reached in test); SECOND reveal hang
  found + fixed (option one predicate-gated to zero targets with NO option two: no chooser arms;
  fixture `w54_reveal_stall_forceclose.txt` RED on base). `assertmtgid` suite command exists; the
  flip-skip lift exposed and fixed a `getCard` regression on transformed cards.
- AA (Vita softlock): an AI-owned interrupt window with no progress for 300 ticks AND 20 s is
  declined with one stderr line (LLM seat 1,200 s; loading game 12 ticks); corpus prediction:
  0 such declines on GPT seats (a decline on a GPT seat within 1,200 s is a FAIL to investigate).
  Baka `Act` no longer holds the window forever on a non-mana extra cost.

## Set C - audit lanes (perf / memory), what to WATCH in the corpus, and the disable flags
Every output-affecting change ships a flag (silent-instrument rule: "was it me?" is one env var).
| lane | change | flag | corpus watch |
|---|---|---|---|
| G | click-path ability ownership, and!-follow-ups, if/then clones, TokenAndAbility owned by the instance | `WAGIC_W54G_LEGACY=1` | any new crash/ASAN signature; suite verdicts identical (1219/0 solo) |
| H | ability epoch gate (`mAbilityEpoch`), single `updateTargets` per tick, `testDestroy` scan gated; L13 `findMatchingAmount` sums every zone + `<=N`/`>=N` bounds | `WAGIC_W54H_LEGACY=1` | verdict drift on aslongas/lord/this cards (Isleback Spawn now pumps); a condition changed without an event is seen at the next bump |
| I | ManaCost lazy sub-costs; garbageLastTurn two-turn deferral; commander/initDeck leaks; cleanupPhase sweeps all zones; `exile=` serialises exile | `WAGIC_MANACOST_EAGER_SUBCOSTS=1`, `WAGIC_GARBAGE_ONE_TURN=1`, `WAGIC_CLEANUP_BATTLEFIELD_ONLY=1` | RSS per game; any dead-token dangle signature |
| J | render caches (HUD quads, type ids, lazy relayout), resource-miss memo, StoryFlow/keybinder fixes | `WAGIC_RENDER_NOCACHE=1` (`-DWAGIC_RENDER_NOCACHE`) | desktop/Vita visual parity (NOT eyeballed by the lane) |
| K | Baka `selectAbility` 35x fewer planner calls (ranked set identical 0 mismatches / 339 calls); hand oracle on change | `WAGIC_BAKA_LEGACY_SCAN=1`, `WAGIC_HAND_REFRESH_ALWAYS=1` | Baka-seat behaviour identical (heuristic fallbacks); hand glow / tap border staleness on desktop |
| L | narration flat with translog off; board render index; script memo; streamed translog; 24 KB trim v2; http status classes (`http_error` + `http_status`); worker shutdown wait | `WAGIC_GPT_BOARDINDEX=0`, `WAGIC_GPT_SCRIPTMEMO=0`, `WAGIC_GPT_TRANSLOG_STREAM=0`, `WAGIC_GPT_TRIM_V1=1` | prompt bytes identical to wave-53 shapes (0/42 replay dumps differed); `http_error` records 0 on the 35B |
| M | ask cache cleared per turn; situation prefill; planner once per row; combat window cache; saturating digit parser | `WAGIC_GPT_AUDIT_M_OFF=1` | cache hits per game with turn-boundary clears; A22's cached blocker helpers NEVER executed in test — first live exercise is this corpus |
| N | Vita NPOT textures + decode scratch + loader error paths (JGE; desktop untouched except loaders) | `-DWAGIC_TEX_POW2_PAD`, `-DWAGIC_NO_DECODE_SCRATCH`, `-DWAGIC_NO_USERDIR_MKDIR` | vpk13 memlog: `heap_arena` flat, `ram_free` -96 MB at boot (O1) |

## Carried into the wave-55 docket (not this corpus's to settle)
- The `chooseaname` menu annotation (from lane C); the 2-D ability-menu collapse (lane D); D8's
  residual mega-menus are D32 (log window, OWNER ITEM).
- Replay: O8 RNG split (owner YES, after this corpus); the equip auto-tap replay blocker (lane AA:
  engine auto-taps then refuses the recorded target click); lane W's two vpk12 transcripts still
  do not replay (lane I re-checked: `exile=` was not their cause).
- O4 `kMaxOptSources` measurement lane; O3 dead-module purge (KEEP networking); O7 gnu++14 on the
  Vita as its OWN build; A15(b) shared primitive + A32 per-model parse cache (projects).
- Lane B's D13 re-ask half; lane K's A37 function merge (needs a scoring pin); lane G's A14 deep
  clones are latent by construction (no route reaches them today).
- Vita open reports: 8 lag games (vpk13's `frames`/`frame ms=` lines + `avg_swp` decide O11);
  unlabeled `bug` game 1788398189 (deck1 vs deck49); Dominating Vampire livelock (D37).
