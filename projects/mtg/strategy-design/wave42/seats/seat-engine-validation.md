# Wave-42 engine fix-validation seat — harvested 2026-08-24

PROVENANCE GATE: corpus binary = 6788e6171 (step-1 only) — the same-day owner narration
fixes are NOT in it; their artifacts (card-text tails, damage double-entry, Mountain-"R")
are PRE-FIX, predictions carry to wave-43's corpus.

## Verdicts (details + cites in task transcript)
- Lane A CONFIRMED across the board: searches 10/10 one-line sourced; D1 share 0.439%
  (was 15.2%); bottoming clean (N=1); reveal-N control intact; scry honest null.
- Lane B CONFIRMED: tag coverage 148/148 of legally-blockable attacker lines (90
  untagged = correct omissions); 0 real reasoning contradictions (models QUOTE the tag);
  cap fires 6/154.
- Lane C CONFIRMED on 3 of 4: sourceless 0/611, effect-fallback 0, duplicates 0;
  symmetry PARTIAL — 95.1% matched, 0 zero-vs-nonzero splits, but the "residual gaps
  all mana labels" clause FALSIFIED (10/15 gaps = end-of-game delta truncation
  [logging artifact], 5 genuine, only 1 mana).
- Lane D CONFIRMED: all six lexicon classes 0; reveal grammar well-formed; D5 50/50
  named with P/T.
- Lane E CONFIRMED + live evidence: #N handles offered as separate lines (the exact
  two-Hive shape); repeat-keying honest null (precondition 0); loud-fail guard FIRED
  LIVE once (stale_livelock on a byte-identical two-candidate reveal ask) and the game
  continued legally — first field firing.
- Lane F: werewolf timing CONFIRMED 2/2; Ludevic's null (not in pool); hand-flip
  prediction FALSIFIED — Brutal Cathar x5 + Tovolar's Huntmaster x2 still offer Flip
  Side from HAND (the borderline.txt residual lane F flagged as F3) -> wave-43 HIGH.
- Lane G CONFIRMED: 0/4144 uncollapsed runs; 0 "your zone"; 1820/1820 line-boundary
  (trim-exercised); savings peak 14.3% deck123.
- Fallbacks 4/2202 = 0.18% (best yet; empty_reply/stale_echo/retracted all 0).
  stale_livelock = lane-E guard on a degenerate candidate set (NEW class, benign-ish);
  deferred_to_heuristic x2 = MDFC flip-then-play re-enters a committed ask with
  ZERO-OPTION menus (fix the re-entry); unparsed x1 = plan-name leak into a sole-option
  answer slot.

## NEW defects (wave-43 docket; full cites in transcript)
HIGH: 1 hand-reveal events attribute cards to the EFFECT'S CONTROLLER not the owner
(both seats mirrored wrong); 2 transform-DFC hand-flips (borderline.txt); 3 transforms
NEVER narrated (board silently swaps name/P/T between records).
MED: 4 Day/Night rendered as a player's battlefield permanent (66 board entries);
5 observer used-lines drop targeting clauses (0/302 vs 64/309 actor — public info);
6 observer used-lines drop #N handles (lane-E fix not applied to that path); 7 event-log
run-collapse missing (52 runs >=3 identical lines, loyalty ticks worst); 8 MDFC
zero-option re-asks.
LOW: 9 teferieffect counter label; 10 mana ability self-named lowercase; 11 doubled
verb "put a +1/+1 put counter"; 12 actor/observer verb-case divergence (two render
paths where lane C claims one); 13 dungeons render as [your zone]; 14 multi-select
ask/reply protocol mismatch (echo_index_conflict).
Instrument caveats recorded (end-of-game gaps = logging artifact; B check covers
flagged sentences; A/E clauses rest on N=1/N=0 preconditions).
