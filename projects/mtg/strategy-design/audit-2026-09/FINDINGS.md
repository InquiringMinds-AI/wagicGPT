# Engine audit — findings (2026-09-12..14). Repo untouched; all runs read-only in scratch worktrees.

## 1. Outcome A/B, heuristic-only (no LLM), 1,050 games/side, base 2d09ea75c (08-24) vs HEAD 4a4f17a25
Per-deck wins of 300: 126 +43 (z 3.5), 123 +34 (z 2.8), 130 −36 (z −3.2), 125 −26 (z −2.1); 146/152/162 flat. 0 crashes, 0 hangs, game lengths equal.
=> The engine changes DID move heuristic play. None of it is in the audited 09-03..HEAD range (flat at every step). All of it is in the 82 engine commits of waves 44-57 (08-24..09-03), which were never audited.

## 2. Localisation (150 games per pairing per point; ±12 at 95%)
- 126v152: 26 (base) -> 32 -> 41 -> 48 (commit 49, 09-02) | 57, 51, 58 | **71** (commit 53 = 3329c40f1, wave-54 lane F, 09-02) | 61, 65, 73, 73, 76, 74, 67 (commit 60) -> 64 (anchor). Pooled pre-53 55/150 vs post-53 70/150, 3.6σ. STEP = 3329c40f1.
  What that commit changes for a Baka seat: reveal driver stall-guard + a fix for reveals whose option one is predicate-gated to zero targets with no option two (previously built neither ability). The AATurnSide/AAFlip setMTGId change is suite-only (real games already set the id) and is NOT the cause. The reveal-path hunk (AllAbilities.cpp ~689-833, +144) is the candidate and needs a targeted read.
- 123v125 / 123v130: a DRIFT from 08-24 to 09-02 (21->37 / 81->95 of 150) with no single step; the window holds wave-51/52 Baka payment-planner, opponent-turn instant-cast, and blocking changes (deliberate play changes) plus owner Vita fixes.
- 130 and 125 losses are the mirror of 123/126's gains within the pool.

## 3. Categorical defects found by measurement
- HANG: d7526c7bf (wave-62 lane Z, 09-05, designation-marker D16) hangs ~10% of 126v152 heuristic games (spinning 3 MB stderr); persisted 14 commits; fixed by 0026a9d87 (daybound marker = descriptor chooser, 09-05). At least one corpus binary shipped with it. Both reviewers had flagged this change.
- CRASHES: 3 in 4,950 games at mid-range commits, 0 in 2,100 at the endpoints (too few to localise): SIGSEGV resolving StackAbility (Brutal Cathar) @c15106f4d; SIGSEGV resolving StackAbility (Marsh Flats) @462c63982; SIGABRT `GuiPlay::receiveEventPlus: Assertion false` after a free Bloodline Keeper activation at untap @7be32f8f0 (that assert line has since moved; class status at HEAD unknown). OPEN.

## 4. Reviewer audit of 09-03..HEAD rules-engine delta (comparison.md)
Agreed, act on: edict auto-replacement picks the HUMAN's sacrifice (Oracle "of their choice"); parked-reveal draw rescue hides a stalled reveal; designation-marker guard = universal name list (the hang above); declined-land fingerprint in Baka's per-tick path; Act() pass floor (3rd rewrite); expired-menu completion path; Idyllic Tutor forced find (CR 701.23b). Disagreed: ward sweep (12 cost-increase cards modelled as pay-or-counter), Baka own-turn instant arm, parser-suppression bit. Neither supports a wholesale revert.

## 5. What this says about the loop (owner's concern)
The nine days after the loop definition added ~1,900 engine lines that did NOT change heuristic outcomes but DID ship a 10% hang for 14 commits and carry the seven agreed defects above, most of them LLM-seat symptoms fixed in shared paths. The ten days BEFORE it (waves 44-57) changed heuristic play measurably and were never measured. No gate in either period played a heuristic-only game.

## 6. Instruments left behind (~/.gatelogs/engine-audit)
baka-ab.sh (pool A/B), bisect-step.sh + sweep-driver.sh (per-commit screen, REPS/SUFFIX env), ab-compare.py, trajectory.md (all readings), cr.txt (official CR), astra.out / fable.out / comparison.md.
