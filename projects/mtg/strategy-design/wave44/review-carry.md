# Wave-44 review carry-list (assembled 2026-08-26 at corpus launch)

Corpus: FIRST FAIR-HAND CORPUS (owner ruling 2026-08-24: real hands, no OptimizedHand,
no aggressivity pin). Binary = 2d09ea75c objects relinked against boost 1.92 (ec4f92838
harness commit; engine unchanged). Serve: FP8-35B qwen36-35b-a3b @ spark:8084, -j 21,
no cap, timeout 900, maxtokens 6000. Outdir: the matchups-20260826-* run.

## Discipline: THIS IS A NEW BASELINE
- Waves <=43 dealt rigged 3-land/1-creature/3-spell AI hands. Do NOT attribute win-rate
  or curve variance vs wave-43 to guide edits or engine lanes — hand distribution moved.
- Rotation (freeze/exit) verdicts from waves <=43 reset: a seat needs clean corpora
  under FAIR hands before exit; carry the wave-43 rung state as history, not standing.
- Mulligan seams are LIVE for the first time. Every seat reviewer records: mulligan
  offered? taken? kept-hand land count; any mull-to-zero or repeated mull WITH a guide
  floor => route the seam-floor question to the OWNER (do not patch the floor in prose).
- Fallback rate baseline for the 35B tier: 0.17-0.18% (waves 42-43). Same threshold.

## Prediction set A — wave-43 GUIDE EDITS (20 edits incl. 2 deletions)
Per wave43/seats/*.md "Predictions" lines (seat-123-130 P1 Starstorm>=60% at kills>=2;
seat-125-126 W6-W9; seat-146-152-162 S1/S2 open on 146). Each: PASS / FAIL / UNTESTED
(no window arose) — UNTESTED is a legitimate verdict, never inferred.

## Prediction set B — wave-44 ENGINE LANES (wave43/engine-ledger.md ranked items)
- #W44-2 X-pricing headline opponent-first: Starstorm cast rate at kills>=2 windows
  33% -> >=60% (shared with P1 above; this is the render half).
- #W44-3 removal {target text:} — removal casts on unseen-text targets -> 0 blind
  choices; count removal decisions where the chosen target's text was rendered.
- #W44-4 Day/Night line present in CURRENT SITUATION whenever a daybound card is on
  board (was 0/2190).
- #W44-5 teferieffect stack label fixed; dungeon rows carry no zone tag.
- #W44-6 reveal false-frame: no library-count/hand-size-0 mid-reveal frames.
- #W44-7 plan-note gate resurrected: fire rate on attackers ~0 (was 34% all-false);
  fires only on option rows with "resolve" verb / narrow windows — cross-tab
  fire-vs-actual-mismatch.
- Lethal basis (2d09ea75c): trade/lethal clauses use remaining toughness after marked
  damage; forecast bRemaining matches outcome (who-dies preview lie on marked damage is
  a KNOWN deferred item — log instances, don't re-docket).
- Loyalty "lost (now N)"; lifelink/DS clauses in trade verdicts present.
- Fair hands (lane A): opening-hand land counts distributed (not 3/3/3...); no
  aggressivity pin artifacts (Baka-style forced ability use absent on GPT seats).

## Wave-45 seeds already known (do not rediscover)
Baka menace gang-block (red pin menace_defender_one_blocker.txt — expected 1 ASSERT per
gate); #W44-9 mana-window auto-pass economics; counter-preview false affordance (repro
locus needed); CS-024a; manarestriction sweep; observers' LOW items; who-dies preview
on marked damage.

## Owner prompt review (invariant 0)
Pull ONE verbatim lategame prompt from this corpus into wave44/lategame-specimen.txt.
