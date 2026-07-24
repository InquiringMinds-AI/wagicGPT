# deck27 notes.md — engine/harness/model ledger + rotation verdict (wave-23)

Seat: Zombies (UB tribal), guided round 4. Binary /tmp/wagic-0e6361732. Corpus matchups-20260724-013710.
Record 4/6 (W vs109/133/131/137-adj; L vs102, vs140-adj), across another POOL SWAP. 136 decisions,
3 fallbacks (all vs137, all unparsed decode-collapse). Guide BYTE-FROZEN this wave (= the deployed
wave-22 free-kills-first revision; strategy.txt is a cmp-verified byte-copy).

## ROTATION VERDICT (round 4): DO NOT ROTATE — new-work-signal present (harmful ahead-degeneration + pilot item)

Standing test: rotate out only when the guide is stable-proven AND no new-work-signal.
- **Free-kills-first revision: VALIDATED (positive), but on THIN evidence.** Its target scenario
  (multi-attacker lord-chump) arose once (vs109 s13) and resolved CORRECTLY at the snap layer; every
  single-attacker snap was also clean; NO 2nd harmful commit-lock. This is the right sign — the guide is
  trending toward "stable-proven" on the block axis. But N=1 for the exact multi-attacker allocation.
- **New-work-signal PRESENT (disqualifies rotation):** the ahead-position degeneration watch RECURRED
  and turned HARMFUL for the first time — 3 consecutive unparsed decode-collapses (vs137 s12-14), a NEW
  manifestation (token collapse -> no coded line -> unparsed; answer-first cannot save it) AND the first
  fallback cluster ever at this seat AND correlated with the rep_penalty=1.05 pilot (ledger item 3). Any
  one of {new harmful class, pilot-correlation, thin-validation-needs-another-corpus} disqualifies.
- **Durdle off-case: 4th corpus unexercised** — a guide change on the books, still not drawn. Not
  "stable-proven."
Re-flag next corpus: want (a) more multi-attacker blocker cases to thicken the free-kills-first
validation; (b) watch for the ahead-degeneration collapse at other seats / under any pilot change;
(c) a walled-ground evasion-drought to finally exercise the durdle off-case.

## ENGINE / representation ledger

1. **[annotation, wither round 3 — VALIDATED, stays CLOSED]** All annotations correct, zero false
   positives (full math in findings.md focus 3): vs102 s7/s10 Oona 2/1 vs Tourach 4/3 -> "shrinks to
   2/1" (power-2 -> 2 counters); vs109 s9 Oona 2/1 vs Chainwalker 3/2 -> "(both die)" (3/2 - 2/2 = 1/0
   dies). Third clean corpus. One benign wrinkle: the model's OWN prose at vs102 s7 miscounted "four
   -1/-1 counters" while the ENGINE annotation was correct and the choice was correct — the annotation
   protected the decision from the model's arithmetic slip. No action.

## MODEL / decode + PARSER ledger

2. **[model+parser, commit-lock — NO 2nd harmful instance]** No snap-misplay-frozen-by-truncation this
   corpus. Stays at 1 harmful instance (wave-22 vs109 s15), single-seat. The >=2-seat promotion
   threshold is NOT met by deck27. Do NOT open a parser change on N=1. Synthesis cross-seat watch
   continues (did the commit-lock produce harmful instances at OTHER seats this corpus?).

3. **[model-DECODE, 3 HARMFUL instances this corpus — ahead-degeneration RECURRED, escalated] Decode
   repetition-collapse -> unparsed_reply, clustered, while AHEAD.** vs137 s12 (ask, 122714ms), s13
   (attackers, 98693ms, 1354x verbatim-line repeat), s14 (blockers, 82229ms), all at 19/10 -> 19/1.
   Reply shapes: endless `**`/`(U+FFFD)**` / `### 0.` / `C.`, ZERO coded lines. CLASSIFICATION:
   **decode-owner, NOT parser-owner** — the parser correctly returns unparsed (INDEX-WINS/echo recovery
   need a well-formed choice index or sibling CHOICE line; a token-collapse has neither, nothing to
   recover). Do NOT route to a parser change. Board impact NEGLIGIBLE (heuristic covered all 3, deck27
   won cap-adjudication 14/1). This is the ahead-degeneration watch (was 3 verbatim-line instances,
   always answer-first-saved) now manifesting as an UNSAVEABLE token collapse. Two facts sharpen it:
     (a) answer-first is a truncation-safe floor ONLY when a coded line 1 is emitted; a full decode
         collapse defeats it (distinct failure class from the commit-lock — collapse = no answer at all;
         commit-lock = a WRONG answer frozen). Reviewers/synthesis should classify these separately.
     (b) PILOT TIE: rep_penalty=1.05 (this corpus's pilot) is the ahead-loop's proposed "cheap fix" —
         yet the loop recurred WITH it active and HARDER. **rep_penalty=1.05 is INSUFFICIENT for this
         collapse.** If a decode-stability lever is pursued: a HIGHER rep_penalty and/or a max-tokens cap
         that fails fast to the heuristic (rather than burning 82-122s of garbage) are the candidates —
         but that is an engine/sampling ledger item for the between-wave work, not a guide change.
   Cross-seat: the corpus had 4 unparsed total; if the OTHER unparsed owners also show decode-collapse
   while ahead/on-trivial-decisions, this promotes from a deck27 watch to a corpus-wide pilot finding.

4. **[model-decode, ahead-loop history]** Prior verbatim-line ahead-loop instances (wave-19 d110 s20,
   wave-20 d27 vs49 s16, wave-21 d27 vs135 s18) did NOT recur in the OLD benign form this corpus; the
   new form is item 3. The watch is no longer "unpromoted/benign" — it produced harm. Roll the watch
   into item 3 going forward.

## DECK-CONSTRUCTION items (route to roster owner)

5. **[construction, STRUCTURAL — durable signal, softening] deck27 vs dedicated lifegain-control:
   CLOSER but still short.** vs140 (RBW lifegain-control) went to a ONE-life cap adjudication (6/7, t27)
   this corpus vs wave-22's blowout (17/28, t24). deck27 attacked every turn and got the opponent to 7;
   the clock is nearly there but still loses the race with no removal/disruption/reach. Same construction
   gap (no answer to lifegain, no B-plan), now demonstrably a narrow miss rather than a structural
   blowout — a small amount of reach or disruption would likely flip this matchup. Roster note.

6. **[construction, INFO] vs102 (Tourach) remains the other bad matchup** — un-interactable protection
   threat, ground deck27 out 0/11. Flagged since wave-21; unchanged; variance/matchup, not a pilot or
   guide problem.

## HARNESS items

7. **[harness, latency] The 3 vs137 collapses burned 82-122s each** (at/over the HTTP timeout) producing
   pure garbage before falling back. A max-tokens or repetition-tripwire fast-fail would reclaim ~5
   minutes of wall time across those 3 decisions and route to the heuristic sooner. Sampling/harness
   ledger item, not a guide item. No -j implication (this was a decode collapse, not server saturation —
   the p50 latencies this seat were a healthy 11.8-16.4s).
