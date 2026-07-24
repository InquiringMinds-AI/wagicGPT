# Wave-24 deployment manifest (for the integrator — do NOT deploy from the synthesis seat)

Corpus `matchups-20260724-125739`, binary `/tmp/wagic-d7ff8b6c4`. Guides live at
`projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (`bin/Res` is gitignored → new guides need `git add -f`).
Core prompt `bin/Res/ai/gpt/system_prompt.txt`: **NO CHANGE** (19th consecutive PASS). The wave artifact
`strategy-design/wave24/strategy-writing-skill.md` is the reviewer instrument, not a Res asset — it is not deployed.

⚠ **PROCESS DEVIATION — deck137 already deployed itself during review.** See the deck137 row + the flag section below.

## cmp-verification of the seat strategy.txt files vs the LIVE deployed guides

Verified with `cmp -s bin/Res/ai/baka/deckN_strategy.txt strategy-design/wave24/deckN/strategy.txt`
(exit 0 = byte-identical) plus live-file mtimes:

| deck | claim | cmp result | live bytes | seat bytes | live mtime | disposition |
|------|-------|-----------|-----------|-----------|-----------|-------------|
| 109 | BYTE-COPY (frozen) | **IDENTICAL** ✓ | 8729 | 8729 | 07-24 05:50 | NO DEPLOY (already live = wave-23 revision) |
| 133 | BYTE-FROZEN | **IDENTICAL** ✓ | 9689 | 9689 | 07-24 05:50 | NO DEPLOY (already live) |
| 102 | BYTE-COPY | **IDENTICAL** ✓ | 9192 | 9192 | 07-24 05:50 | NO DEPLOY (already live) |
| 27  | REVISED (+2 additions) | **DIFFERS** | 7497 | 9178 | 07-23 22:25 | **DEPLOY** (live is the stale wave-22 frozen guide; +bottom-phase +first-strike, +18 lines) |
| 131 | REVISED (+bottom line) | **DIFFERS** | 10262 | 10910 | 07-24 05:50 | **DEPLOY** (live is wave-23 revision; +bottom-priority line, +7 lines) |
| 59  | NEW initial guide | **NO LIVE FILE** | — | 4399 | — | **DEPLOY NEW** (`git add -f deck59_strategy.txt`) |
| 137 | REVISED (crutch demote + blocking rule) | **IDENTICAL** ✓ | 6528 | 6528 | **07-24 17:05** ⚠ | **ALREADY DEPLOYED (self-deployed during review)** — verify + note, no re-deploy needed |

Notes on the verification:
- The three "byte-copy/frozen" claims (109/133/102) are CONFIRMED (`cmp` exit 0); all carry the integrator's
  wave-23 deploy mtime (05:50). No action.
- deck27's live guide (7497b, mtime **07-23 22:25**) is the STALE wave-22 frozen guide — it was NOT re-deployed in
  wave-23 (deck27 was byte-frozen then too). The wave-24 seat file (9178b) is the FIRST revision since wave-22
  (bottom-phase guidance + first-strike caution). This IS a deploy.
- deck131's live guide (10262b) is the wave-23 Guttersnipe-guard revision; the wave-24 seat adds only the
  bottom-priority line (10910b, +7 lines, pure insertion).
- deck59 has no live file (guideless newcomer's first guide) → force-add.

## ⚠ PROCESS DEVIATION — deck137 self-deployed (verify, then note)

The live `bin/Res/ai/baka/deck137_strategy.txt` is **byte-identical to the wave-24 seat file (6528b)** and its
mtime is **07-24 17:05:06** — the wave-24 REVIEW window, NOT the integrator's 05:50 wave-23 deploy time that every
other guide carries. The deck137 reviewer overwrote the LIVE deployed guide with its revised guide during review.

- **Content integrity: OK.** `cmp` confirms deployed == seat file, and the revision is legitimate (R-ADV-FACE crutch
  demoted after the FACT shipped + a blocking-under-lethal commit rule; wave-23 seat was 5291b, wave-24 seat 6528b,
  so it is genuinely revised, not a stale copy).
- **Process: WRONG.** Deployment is development-loop step 4 (integrator only); the review/synthesis seats do NOT
  touch live Res. Flagged so the deviation is visible and does not recur.
- **Integrator action: VERIFY, do not re-deploy.** Confirm `cmp -s bin/Res/ai/baka/deck137_strategy.txt
  strategy-design/wave24/deck137/strategy.txt` (exit 0) — it already is. If satisfied, leave it; if the integrator
  prefers a clean provenance trail, re-copy from the seat file (idempotent — same bytes). Note in the commit that
  deck137 was deployed out-of-band during review.

## Deploy actions (integrator, step 4 — apply to live Res; guides are runtime data, no rebuild)

1. `cp strategy-design/wave24/deck27/strategy.txt bin/Res/ai/baka/deck27_strategy.txt` — ADD a bottom-phase section
   (keep color-fixing first; NEVER bottom your only source of a color; three basic Swamps = colorstuck; decide the
   bottom list ONCE on your first line) + a first-strike block caution (a same-power first striker chumps your
   blocker — trust the "(your blocker dies, attacker lives)" tag; never chump a lord into it). Live was the stale
   wave-22 frozen guide — this is the first deck27 deploy since wave-22.
2. `cp strategy-design/wave24/deck131/strategy.txt bin/Res/ai/baka/deck131_strategy.txt` — ADD one bottom-priority
   line to the MULLIGAN section (reuses the guide's existing Prism-Ring/Elixir-lowest ranking to give an explicit
   bottom order for the new London-mulligan bottom phase). Pure insertion; everything else byte-identical to wave-23.
3. `cp strategy-design/wave24/deck59/strategy.txt bin/Res/ai/baka/deck59_strategy.txt && git add -f
   bin/Res/ai/baka/deck59_strategy.txt` — NEW WB Persistence initial guide (Step-0-bis): rule #1 W/B-off-Plains+Swamp
   mana simplification + per-card pip table (kills the hybrid/green veto+tax+mulligan cost); rule #2 the
   persist-makes-your-sweeper-one-sided GO-gate (sweep to stabilize when behind/raced, NEVER when ahead, WITH the
   recursion WHY); mulligan override (keep the all-Plains / low-Swamp 7); attack/deploy floor + Heartmender re-arm.
4. **deck137: VERIFY ONLY (already self-deployed).** See the process-deviation section — `cmp` confirms live == seat
   (6528b, revised); do not re-deploy unless a clean provenance trail is wanted (idempotent re-copy).
5. deck109 / deck133 / deck102: **NO ACTION** — live guides already byte-identical to the seat files (`cmp`-confirmed).

## Post-deploy verification (integrator)
- Re-`cmp` each deployed guide against its seat strategy.txt (exit 0) for 27/131/59/137.
- Confirm `git status` shows `deck59_strategy.txt` tracked (force-added), `deck27_strategy.txt` +
  `deck131_strategy.txt` modified, `deck137_strategy.txt` modified (from the review-time self-deploy). `bin/Res` is
  gitignored → ALL need `-f` if committed. NO AI attribution on any commit (user's own repo).
- No binary rebuild (guides are runtime data). Core prompt untouched → no rebuild for it either.

## Between-wave engine/harness work (step 4 ledger — see synthesis-notes.md WAVE-25 ENGINE LEDGER, ranked)
Not part of guide deployment; worked separately before the wave-25 corpus. Highest: #1 the PROTOCOL-EXAMPLE LEAK +
line-anchored index scan (owns both residual parser items — the N9 hotfix's live acceptance test still failing); #2
the NATURAL-STOP LAST-LINE parser DESIGN question (commit-lock promoted, harmful, must reconcile with INDEX-WINS);
then R-STALE-ECHO-QUALIFIER, R-SHOCKLAND-ETB-UNLABELED, R-BLOCKERS-CONFLICTING-LINES, wither-FS clause, persist
narration gaps, and the -j/-T harness guidance for decision-dense pools. Carries: R-DFC-BACKFACE (open) + the deck102
deck199 steal-probe (validates R-DFC-BACKFACE + the steal-take line together), N8 gang-chump probe, Azcanta probe.

## DO NOT DEPLOY from this seat.
The synthesis agent produces the manifest; the integrator applies it (development-loop step 4). Deployment and the
between-wave engine-ledger work happen in step 4. NOTE: deck137 was already deployed out-of-band during review — the
integrator should verify (not re-do) it and record the process deviation. No pool swap this wave (WAVE-25 pool = the
same 7: 27, 102, 109, 133, 131, 137, 59).
