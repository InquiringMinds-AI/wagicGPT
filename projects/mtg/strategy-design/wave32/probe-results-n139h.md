# Wave-32 probe results — N-139h (Pollywog discounted mutate)

Probe run 2026-07-27 on the merged wave-32 step-1 binary (commit 6ca1f8411), debug build,
all runs under `systemd-run --scope -p MemoryMax=4G -p MemorySwapMax=0` (post-OOM-incident
discipline; a runaway allocation now dies as a contained failure).

## Verdict

- **Crash half of N-139h: HOLDS.** No crash/abort/assert in any run (21-game corpus at
  wave-31 + every capped fixture run here). The wave-31 bounds guards stand.
- **Resolution half: NEW DEFECT FOUND — N-139m.** A creature spell with mutate cast while
  Pollywog Symbiote is on the battlefield PAYS its (discounted) cost, fires Pollywog's
  cast-trigger loot (library count drops), and then the card VANISHES — it ends in no
  enumerable zone (battlefield/graveyard/hand all asserted). Reproduces on BOTH the mutate
  route and the NORMAL cast route of a mutate card. The discount mechanism itself is
  exonerated: an Emerald Medallion (altercost, no trigger) discounted cast lands cleanly.
  Mechanism hypothesis: the loot's discard chooser arming during cast/resolution derails
  the spell off the stack (kin to the menu-arming landmine class, but this loses a
  RESOLVING SPELL, not a menu answer).

## The differential (all fixtures in bin/Res/test/ai/, UNREGISTERED except the control)

| fixture | setup | result |
|---|---|---|
| `_control_gemrazer_undiscounted.txt` | mutate, no Pollywog, full {1}{G}{G} | GREEN — merge lands, pool 0 |
| `_control3_gemrazer_medallion.txt` | NORMAL cast, Emerald Medallion discount {2}{G} | GREEN — lands, pool 0 |
| `pollywog_discount_gemrazer_n139h.txt` | mutate route, Pollywog discount {G}{G} | FAILS — paid, loot fired (library 3->2), card in NO zone |
| `pollywog_discount_dirgebat_n139h.txt` | mutate route, {1}{B}{B} multi-pip | FAILS — same |
| `_control2_gemrazer_normal_discounted.txt` | NORMAL cast, Pollywog out, {2}{G} | FAILS — same (not mutate-specific) |

Notes for the fix agent:
- The two GREEN controls double as regression fixtures for the fix; the three FAILING ones
  flip to registered once green. Zone-assert semantics: unlisted zones in an [ASSERT]
  player section are enforced ZERO — list library/hand leftovers explicitly (the loot
  draw leaves `hand:Mind Stone`, `library:` minus one).
- Harness cannot express comma card names (`Nethroi, Apex of Death` parses as two cards)
  — that is why the probe uses Gemrazer/Dirge Bat; Gemrazer is also the exact card from
  the live wave-30 crash offer.
- Scope of the live risk: any deck139-style seat with Pollywog on the battlefield loses
  every subsequently cast mutate-card spell (the corpus never showed it only because
  Pollywog survived to cast-time in 1 of 6 games and that offer was declined).
- RELEASE-build leg of the original N-139h validation is still owed once N-139m is fixed
  (debug-only pass is not a validation for the arithmetic half).

## Consequences

- deck139 rotation stays GATED (exit gate "N-139h revalidated" now reads "N-139m fixed +
  discounted mutate arrival-traced live").
- N-139m goes to the wave-33 step-1 docket at HIGH (silently loses a paid spell; the
  guide-side workaround — do not teach around it — is NOT wanted per the no-stopgap rule
  for engine-lane defects).

---

# Addendum: deck198 convoke probe FINAL VERDICT (4 games, binaries 6ca1f8411 + 7fabd9bd0)

- **Crash: FIXED** (7fabd9bd0) — game 1 SIGSEGV root-caused (doPay/isPaymentSet keying
  drift left stale payment targets; see commit); games 2-4 clean, 0 cores.
- **F4 (offer): PASS** — `Cast March of the Multitudes with its convoke cost {x}`
  offered at ONE untapped source with untapped creatures (game 2 seq18 t7), impossible
  pre-fix. N-137a/N-152c offer-side validated live.
- **Divergence-C re-ask: OBSERVED LIVE** — 2x `validation_reject_reask` markers
  (game 4), no silent burns at that seat. First live firing of the i5 machinery.
- **F1/F2 (announce + arrival): FAIL** — across all games, ZERO
  `March: hand -> stack`, ZERO `Announce the value of X` on any route, despite
  convoke-route PICKS at three seats (each picked twice = stuck-line cap engaging,
  later plain-route retries also unarrived in capped games). Confirms the engine
  agent's own caveats: N-137b announce routing shows no observed behaviour change on
  the alternative path, and **N-137c has a second layer downstream of the FindCardToPlay
  scoring fix** — the alternative-cost convoke branch still abandons the cast after
  consuming the pick.
- Probe games 3-4 hit the 3000s cap running CONCURRENTLY (-j2 halves each game's
  decision budget vs one Spark instance) — run probe one-shots SEQUENTIALLY.

**Residual for wave-33 step-1: N-137c layer 2** (instrument
`MTGAlternativeCostRule::reactToClick` MTGRules.cpp:1150-1160 through the announce/
`setExtraCostsAction` seam on a convoke pick; the deck198 probe pair spec (recreate
from this file + wave31/deck137/findings.md §8) is the validation instrument. Convoke
arc remains OPEN; deck137 stays rotated out.

---

# Addendum 2 (post-corpus adjudication): N-139m RE-SCOPED to the fixture-harness path

The wave-32 corpus (binary 7fabd9bd0) live-arrival-traced **6/6 Pollywog-present mutate
casts**: discounted cost paid, loot fired, `[mutated pile]` arrived (deck139 seat review,
incl. Snapdax +4 life and Illuna reveal resolving). The three failing fixtures re-run on
7fabd9bd0 STILL FAIL identically. Therefore the card-loss reproduces ONLY under the
scripted test-harness click path, not in live AI/GPT games — the live-severity HIGH is
withdrawn; the item re-scopes to the HARNESS lane (hypothesis: the script pump's
handling of the loot's mid-cast discard chooser diverges from live menu flow — kin to
the known scripted-fixture menu-arming landmines). deck139's exit is NO LONGER gated on
it (see the wave-32 seat review's gate table: discounted-mutate-arrival-traced-live is
MET). The five fixtures stay preserved with their STATUS headers as the harness-lane
reproducer.
