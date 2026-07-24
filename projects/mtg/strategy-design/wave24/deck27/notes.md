# deck27 notes.md — engine/harness/model ledger + rotation verdict (wave-24)

Seat: Zombies (UB tribal), guided round 5. Binary /tmp/wagic-d7ff8b6c4 (step-1 batch + PREGAME MULLIGAN
PHASE + N9 hotfix). Corpus matchups-20260724-125739 (PRIMARY; N9-overfire run quarantined, not read).
Record 3/6 (W vs133/59-adj/131; L vs102/109; DRAW vs137-timeout). 160 decisions, 0 fallbacks at this
seat. Guide REVISED this wave (two grounded additions — see rotation verdict + strategy.txt): bottom-
phase guidance + first-strike block caution.

## ROTATION VERDICT (round 5): DO NOT ROTATE — guide modified this wave + new-work-signal present

Standing test: rotate out only when the guide is stable-proven AND no guide-mod AND no new-work-signal.
- **Guide MODIFIED this wave** (bottom-phase guidance + first-strike caution, both from concrete harmful
  instances). A just-changed guide is by definition not "stable-proven"; it needs a validation corpus.
- **New-work-signal PRESENT (multiple):** (1) a harmful COMMIT-LOCK in the brand-new bottom phase
  (vs137 seq5, first commit-lock since wave-22) — validates the wave-23 parser proposal, still open;
  (2) a harmful free-kills-first FIRST-STRIKE misfire (vs109 seq11) just addressed in-guide, needs
  re-validation; (3) the wither-vs-first-strike annotation clause bug (representation, open).
- **Durdle off-case: 5th corpus unexercised** — still on the books, not drawn. Not stable-proven.
Re-flag next corpus: (a) does the first-strike block caution hold on the next first-striker window?
(b) does the bottom-phase guidance make the snap right (no repeat colorstuck bottom)? (c) parser last-
line-preference acceptance if it ships; (d) still hunting a walled-ground stall to exercise the durdle
off-case.

## MODEL / decode + PARSER ledger

1. **[PARSER, HARMFUL — validated proposal, OPEN] COMMIT-LOCK in the bottom phase; prefer LAST coded
   line on non-truncated replies.** vs137 seq5 (kind=bottom, 89s, 5132 chars): coded line 1 `PUT: 3, 5,
   6` bottomed Underground Sea (the ONLY blue source), but the reply reasoned to the correct answer and
   ended "So I bottom 5, 6, and 7. PLAN: Put cards 5, 6, and 7 on the bottom." Answer-first froze the
   wrong first line. The reply was NOT truncated — the model's FINAL coded line (5,6,7) was correct and
   discarded. This is the live harmful instance the wave-23 skill.md flagged as a candidate ("possibly
   PARSER: prefer last coded line on non-truncated replies"). PROPOSAL: on a reply that terminated
   normally (not truncation-capped), when multiple coded lines of the same kind appear, prefer the LAST
   one (the model's conclusion) over the first. Scope carefully: this must NOT change truncated replies
   (where answer-first line 1 is the only safe floor) — gate on "reply reached a natural stop / did not
   hit max-tokens." Bottom/mulligan decisions are the highest-value target (arithmetic-heavy, snap
   unreliable). Guide fix shipped in parallel (make the snap right) — the two are complementary, not
   either/or.

2. **[representation, wither round 4 — NARROW REOPEN] wither-vs-first-strike annotation clause is
   WRONG.** vs109 seq11, Oona's Gatewarden block option: `A1 (your blocker dies, attacker lives (wither
   shrinks it to 0/0))`. Top-level verdict correct (Oona 2/1 dies to Ash Zealot 2/2 first strike), but
   the parenthetical "(wither shrinks it to 0/0)" is FALSE — with first strike the withering wall dies
   BEFORE dealing damage, so no wither counters land and the attacker is unshrunk. The block-annotation
   math (otherwise clean 3 corpora) does not model the first-striker-kills-the-blocker-first case for the
   wither clause. Low harm (the leading "attacker lives" was right and drives the decision; the guide now
   warns against re-deriving first strike anyway), but the clause should be suppressed when the attacker
   has first strike and lethal-to-the-blocker. Representation item; watch for other wither-vs-FS renders.

3. **[model-DECODE, verbosity NOT collapse — corpus-wide latency signal] Decode-collapse class did NOT
   recur; verbosity did.** 0 fallbacks at this seat; NO reply carried the collapse signature (biggest
   replies 86-95% unique lines, max single-line repeat = 3, vs wave-23's 1354). The answer-locked retry
   firing 0x is CORRECT — nothing to catch. BUT replies ballooned to 5-10k chars burning 60-180s each
   (vs137 bottom 89s/5132; vs59 s22 179s/10581; vs109 s19 175s/10264). This is verbose COHERENT reasoning
   (all parse), so it costs LATENCY not correctness — EXCEPT where the over-reasoning enabled the
   commit-lock (#1). Distinct from wave-23's collapse: that was unparseable garbage; this is parseable
   over-thinking. Cross-seat: if convoke/long-decision seats show the same verbosity->latency, it is a
   corpus-wide inference-cost issue, not a deck27 problem.

4. **[harness, latency/timeout] vs137 timeout = pure inference starvation; verbose bottom cost 89s.**
   Combined decision latency 2352s of the 2400s cap (98%); deck137 seat 1835s/55 dec (convoke,
   t9=627s/t7=452s) dominated, deck27 517s/23 dec. deck27 pregame consumed 158s (bottom 89s). A
   MAX-TOKENS fast-cap on any single decision (fail-fast to heuristic past a token/latency budget) would
   both reclaim timeout headroom AND cut off the runaway over-reasoning that produces commit-locks.
   Sampling/harness ledger item (not a guide item). This corpus ran at high latency (deck27 per-game
   sums 370s-1002s vs wave-23 ~12-16s p50) — likely -j saturation; two of deck27's six games (vs137,
   vs59) ran to the 2400s cap purely on latency.

## GUIDE items (owned by me, shipped in strategy.txt)

5. **[guide, NEW section — bottom-phase] Added bottom-decision guidance** (the pregame phase is new;
   the guide had none). Keep color-fixing FIRST (Underground Sea almost never a bottom; never bottom your
   only source of a needed color); three basic Swamps = colorstuck (can't cast {U} cards); keep 2-3
   lands incl. a blue source + cheapest castable creatures; bottom excess Swamps + uncastable expensive
   cards; "decide the bottom list ONCE, put it on your first line." Targets vs137 seq5 at the guide layer.

6. **[guide, block section — first strike] Added a first-strike caution.** A same-power first striker
   kills your blocker before it deals damage -> the block is a CHUMP not a trade; TRUST the "(your
   blocker dies, attacker lives)" tag, do not re-derive; never chump a lord into a first striker; also
   caveated Oona's wither (only lands if the wall survives). Targets vs109 seq11 at the guide layer.

## DECK-CONSTRUCTION items (route to roster owner)

7. **[construction, INFO] vs102 (Tourach) — now a NEAR-MISS race, not a grind-out.** 0/4 at t13 (both
   8/8 at t10, got opp to 4) vs wave-23's 0/11. Un-interactable protection threat, no removal/reach; a
   small amount of reach would flip it. Softening signal, same structural gap. Roster note.

8. **[construction, INFO] vs109 (mono-R aggro) — high-variance matchup.** Flipped from a wave-23 blowout
   WIN (18/-1) to a wave-24 blowout LOSS (0/20) — draw-dependent race with no removal. The seq11
   lord-chump (now guide-addressed) worsened it but did not cause it; deck27 was behind on tempo. Not a
   construction defect on its own, but reinforces the no-removal fragility vs a fast curve. Roster note.

## HARNESS items
9. **[harness] See #4** — max-tokens/latency fast-cap candidate; two of deck27's six games hit the 2400s
   cap on latency alone. No -j implication assertion beyond "this corpus was latency-heavy."
