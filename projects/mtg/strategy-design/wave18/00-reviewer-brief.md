# Wave-18 reviewer brief (shared context for all 7 deck agents + synthesis)

Fourth per-deck-rotation cycle. deck17 rotated out (first converged exemplar); deck14 (Deep
Blue) entered GUIDELESS. decks 49/35 play their FIRST GUIDED corpus; decks 21/62 play their
REVISED guides round 2 (21 = power floor on race-math; 62 = Blanchwood recount-ban +
A-list-only attacks). Binary f664539a3 adds the wave-17 engine batch: (a) TARGET CHOICE
sub-menu framing (names the pending effect, "not a cast or phase step"); (b) hand-card
PUT_INTO_PLAY dead-ends filtered from the priority menu (the Mox E6 fix); (c) beneficial
opponent-only-target cast suppression (E-49c); (d) explicit "BLOCKS: none" example +
paraphrase catch; (e) "(player, life N)" suffix strip in the echo matcher.

⚠ **OWNER RULING (post-corpus, already implemented for wave-19)**: hard-suppressing legal
plays on a strategy judgment is WRONG ("you cant decide that a play is bad and therefore
never offer it") — both effectBadOrGood suppressions (wave-16 own-only BAD, wave-17
opponent-only GOOD) are being REPLACED with warning ANNOTATIONS on the option line. So: do
NOT write guide text assuming those options never appear, and do NOT celebrate the
suppressions as final behavior. If your seat shows a spot where the suppression HID a
correct play this corpus, flag it with seq — it strengthens the annotation design.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-185930` — 21 games, round-robin
  over 110,135,21,62,49,35,14. Binary /tmp/wagic-f664539a3.
- Win table: 35:4/6, 135:3/6, 21:3/6, 110:3/6, 14:3/6, 49:2/6, 62:2/6 — flattest table yet.
  1 draw/timeout, 2 life-adjudicated, 0 segfaults (5th clean corpus).
- Fallbacks 2.2% (25/1146) — NEW BEST: 22 unparsed, 2 stale_echo, 1 defer, 0 empty.

## KNOWN CORPUS-WIDE (do not re-diagnose)

- **TARGET CHOICE framing VALIDATED at scale**: shown on 647 decisions; stale_echo collapsed
  13 -> 2 corpus-wide; deck49 (last wave's owner with 7) has ZERO fallbacks this corpus.
  Reviewers: spot-check your seat's target decisions for quality (right targets picked), and
  flag any spot where the framing itself confused (new failure shapes only).
- **E6 CLOSED**: "Cast Card Normally with X" appeared 0 times corpus-wide (was 5 fallbacks at
  deck110 last wave). deck110's reviewer confirms no NEW dead-end shapes in priority menus.
- **PLAN-caveat**: 382 injections, steady state. unparsed 22 (up 4 from wave-17's 18 with a
  bigger ask count) — reviewers at unparsed-heavy seats (110: 6, 62: 5, 14: 4) characterize
  what rambled; the caveat class (stale-plan spirals) should NOT be among them.
- **Azcanta / optionone reveal auto-decline is KNOWN-OPEN (HIGH ledger)**: the engine
  auto-declines every optional reveal-to-hand chooser for AI seats before the model can
  answer (root-caused wave-17; fix pending a reveal repro harness). deck135's reviewer: note
  fingerprints (reveal happened, nothing to hand) but do NOT re-diagnose; the model never had
  the choice.

## Seat assignments

- **deck110 (3/6):** E6-closed confirmation; 6 unparsed + 1 defer — characterize; equip-churn
  standing watch (4th wave); record dropped 4/6 -> 3/6 — decisions or matchups?
- **deck135 (3/6):** best record since entering (1/6 -> 3/6) — did the sub-menu framing
  unlock its control game? Force quality watch (face-burn-Force class, 4th corpus); Azcanta
  fingerprint notes only; stale [attacking] tag standing watch.
- **deck21 (3/6) — REVISED-GUIDE ROUND 2:** power-floor rule validation (did the 0-power
  misfire class die? any race-math firings this corpus with correct power math?);
  kill-on-sight-engine sharpening effect; Step 0-ter table.
- **deck62 (2/6) — REVISED-GUIDE ROUND 2:** Blanchwood recount-ban + A-list-only attacks —
  died/persisted/mutated; 5 unparsed at this seat — the old giant-reply tax pocket
  (computed-P/T distrust) or something new? Record 4/6 -> 2/6: structural or decisions?
- **deck49 (2/6) — FIRST-GUIDED VALIDATION (Dragons):** guide rules #1/#2 (mana-is-trivial,
  target-sub-menu) now partially superseded by the engine framing — which taught classes
  fired/died? ZERO fallbacks at this seat (was 9) — quantify the tax collapse. Record 3/6 ->
  2/6 guided: construction (top-heavy curve) was pre-flagged; confirm or revise.
- **deck35 (4/6, pool leader) — FIRST-GUIDED VALIDATION (Slivers):** the anti-chump-block
  countermand — did FC1 (defensive spiral) die? 0-attacker durdle signature check (the
  skill's new instrument); multi-lord stacking finally exercised?
- **deck14 — GUIDELESS FIRST PASS (Deep Blue, 3/6):** Step 0-bis doctrine. Mono-blue
  tempo/control read; write the initial guide (format-match a live guide).

## Doctrine (BINDING)

Win-rate is context; the unit of analysis is the DECISION with seq repros. Layer-routing
enforced. Single-seat items cannot add/cut core lines. Per-deck rotation: state your seat's
verdict explicitly in notes.md (no-guide-mod AND no-new-work-signal = rotation candidate).

## Output contract (per deck agent, sequential, in strategy-design/wave18/deckN/)

findings.md -> strategy.txt (byte-identical if frozen, REVISED if warranted; newcomer 14:
the NEW initial guide) -> skill.md -> general-suggestions.md -> notes.md (engine items +
rotation verdict). CURRENT skill = strategy-design/wave17/strategy-writing-skill.md. Guided
seats read their wave-17 deckN continuity files.
