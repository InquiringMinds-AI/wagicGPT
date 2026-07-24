# deck27 notes.md — engine/harness/model ledger + rotation verdict (wave-22)

Seat: Zombies (UB tribal), guided round 3. Binary /tmp/wagic-1d8ed8194. Corpus matchups-20260723-173843.
Record 1/6 (W vs44; L vs135/62/102/109/140), across a POOL SWAP. 135 decisions, 0 fallbacks.
Guide byte-frozen this wave except ONE surgical block-allocation sentence (findings.md → strategy.txt).

## ROTATION VERDICT: DO NOT ROTATE — durdle revision STILL unvalidated + a new answer-first-downside item

Standing test: rotate out only when the guide is stable-proven AND no new-work-signal. Neither holds:
- The deployed **late-stall durdle revision is UNVALIDATED for a 3rd corpus** — its trigger (walled
  ground stall + holding buffed attackers back) again did not occur. vs140 (the best grind window yet,
  a lifegain-control cap game) had deck27 ATTACKING EVERY TURN (never held back), so the revision was
  not exercised. A guide change is on the books but unexercised → not "stable-proven."
- **New-work-signal PRESENT**: the answer-first commit-lock interaction produced its **1st harmful
  instance** (vs109 s15 — snap line committed a lord-chump misplay, the model self-corrected in prose
  but truncated before emitting a corrected BLOCKS: line). This is a new ledger item to track (below),
  plus I made a guide edit — either alone disqualifies rotation.
Re-flag next corpus: still want a walled-ground grind with an evasion drought to exercise the durdle
off-case; watch the answer-first commit-lock for recurrence at other seats.

## ENGINE / representation ledger

1. **[annotation, wither round 2 — VALIDATED, stays CLOSED] No regression, zero false positives.**
   Six wither annotations verified against real math (full verbatim + math in findings.md
   "OWNED VALIDATION"):
     - vs44 s6 & s16: Oona 2/1 vs Faerie Miscreant 1/1 → "(both die)" — CORRECT (2 ≥ 1).
     - vs62 s6: Oona 2/1 vs Canopy Spider 1/3 → "shrinks to 0/1, attacker lives" — CORRECT; AND the
       pro-green Vodalian option → "(neither dies)" — CORRECT, the annotator accounts for protection
       negating the green attacker's damage.
     - vs62 s18: Oona 4/3 (Glen-buffed) vs a 17/21 trampler → "14 tramples … shrinks to 13/17" —
       CORRECT wither+trample+shrink math (trample = 17−3; shrink = −4/−4). Glen 2/3 → "14 tramples".
   Model chose correctly off every annotation. No further action.

2. **[representation, INFO — clean] Buffed/debuffed [X/Y] rendered correctly under a −1/−1 sweeper.**
   vs140: Black Sun's Zenith put −1/−1 counters on the Glens; the battlefield line showed
   "(2/3) [counters: 2x -1/-1]" alongside the un-countered "(4/5) (printed 2/3)". No confusion; the
   guide's "[X/Y] already includes buffs" rule held under counters too.

## MODEL / decode + PARSER ledger

3. **[model+parser, NEW — 1st harmful instance] Answer-first commit-lock: snap misplay frozen by
   truncation.** vs109 s15 (blockers, t9, 12/15): line 1 committed `BLOCKS: B1:A3, B2:A1, B3:A2`
   (chump Lord of Atlantis into a 3/3 Hellrider it cannot kill + hallucinated a nonexistent B3),
   THEN reasoned correctly that the chump is bad ("A3 survives … blocking A3 costs me a creature"),
   THEN truncated (7.6k) before emitting a corrected BLOCKS: line → parser locked the snap misplay.
   This is the HARMFUL twin of wave-21's benign vs35 s18 (there the model DID emit an improved last
   line and last-wins honored it). The floor property of answer-first (truncation-safe first draft)
   INVERTS when the snap answer is wrong and the completed reasoning would have fixed it. Two mitigations,
   both pursued at layers other than a decode campaign:
     (a) GUIDE (done this wave): front-load a free-kills-first ALLOCATION rule so the *snap* line gets
         multi-attacker blocker allocation right — the layer answer-first actually commits.
     (b) SYNTHESIS watch (cross-seat): does the commit-lock produce harmful instances at OTHER seats?
         If ≥2 seats show it, consider a parser refinement (e.g. prefer the LAST coded line only when
         the reply is non-truncated; or a "combat re-emit" nudge). Single-seat for now — do NOT open a
         parser change on N=1.

4. **[model-decode, 3rd instance — NO recurrence this corpus] Ahead-position repetition-degeneration
   loop.** Did NOT recur (deck27 rarely ahead; zero consecutive-line loops across 135 decisions). The
   sole truncation (deck135 s16, 15.7k) was a BEHIND-position over-derivation, answer-first-saved. Watch
   stays at 3 instances (wave-19 d110 s20, wave-20 d27 vs49 s16, wave-21 d27 vs135 s18), unpromoted.
   repetition_penalty knob still the cheap fix if it ever recurs harmfully; do not spend guide prose.

## DECK-CONSTRUCTION items (route to roster owner)

5. **[construction, STRUCTURAL — the durable signal] deck27 has NO answer to dedicated lifegain-control.**
   vs140 (RBW control) presented 0 creatures all game, gained to 28 via 2× Elixir of Immortality +
   Venser's Journal, and reset deck27's board with Black Sun's Zenith. deck27's evasive clock (2–4/turn)
   cannot outrace lifegain and has no disruption (no removal, no artifact hate, no reach/burst). This is
   an unwinnable matchup for the current 60 — a construction gap, not a pilot or guide problem. The
   durdle revision cannot fix it (deck27 attacked every turn and still lost the clock). Roster note.

6. **[construction, INFO] The 3 continuing-opponent flips (135/62/102 all W→L) are draw/opponent
   variance, not regression.** 135 = color/mana screw; 62 = opponent 17/21 trampler by t8; 102 = raced
   behind. deck27 played clean in all three. Same high-variance, no-B-plan surface flagged wave-21 —
   this corpus is the unfavorable tail. The pool swap ALSO added two of deck27's worst archetypes at
   once (mono-R aggro 109 + lifegain-control 140), compounding the down-swing.

7. **[construction, INFO] Still under-observed:** Deepchannel Mentor (all-blue-unblockable) drawn vs109
   but moot (defensive game); Inkfathom appeared but the deck's ceiling remains under-sampled. No action.

## HARNESS items
None. 0 empty_reply, 0 timeout, 0 fallback at this seat. Latency median ~14–16s, one 209s outlier
(deck135 s16, the 15.7k giant) still under the HTTP timeout — no empty_reply resulted. vs140 and vs102
resolved by life-adjudication at cap (legitimate; deck27 behind in both). No -j / latency implication.
