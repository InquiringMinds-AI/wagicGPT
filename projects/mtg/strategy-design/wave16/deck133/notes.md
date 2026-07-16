# deck133 wave-16 — development notes (engine / harness / model / deck-construction) + rotation verdict

Corpus: PRIMARY `matchups-20260716-084532`, binary `/tmp/wagic-69793930a` (wave-15 engine batch:
zero-mana free alternative casts + stale_echo option-subset-of-echo matcher + hybrid-pip annotation),
Spark @0.75. Real games: 135(W), 110(W), 21(W), 62(W), 17(L-adj), 44(L) — **4W/2L (up from 2W/4L)**.
Layer-routing enforced: engine/harness/model/construction items live HERE with seq repros; the guide
is a ZERO-edit frozen diff.

## ITEM A (own-target-only removal cast) — VALIDATED CLEAN; DOWNGRADE the suppression lever to parked WATCH.
Wave-15's finding 2 (self-Fatal-Push via card-presence hallucination, 1/11 own-target windows) did
NOT recur. The `Cast Fatal Push - the only legal targets are YOUR OWN` option was offered ~20x
(vs135 s14/15/16; vs21 s10/12/14/17/19/22/25/27/29/32/34/35/37/39/40/41) and declined 20/20. Combined
annotation-obeyed rate across wave-15+16 = 30/31.
- **Engine/representation lever (wave-15 #A — "suppress a removal cast whose only legal targets are
  friendly"):** still architecturally sound but now 0/20 recurrence -> its empirical support is one
  non-deciding wave-15 firing. **DOWNGRADE from build-candidate to parked WATCH.** Re-escalate only
  if a self-target removal cast RESOLVES again (deciding or not) at any seat.

## ITEM B (stale_echo superstring false-positive) — VALIDATED by absence; SIGNATURE SHAPE UN-EXERCISED.
The wave-15 batch's option-subset-of-echo matcher shipped. This corpus: 0 wrongful superstring
downgrades at this seat. BUT the exact Yawgmoth-attack shape (`Attack with Yawgmoth, Thran Physician`
superstring of `Attack with Yawgmoth`) had NO window this corpus — deck133 never had an
attack-with-multiword-named-creature ask; the only Yawgmoth strings were exact-echoed `Play/Hold
Urborg, Tomb of Yawgmoth` land drops (vs62 s36, vs44 s6). So item B is "no regression, positive match
still awaiting a live witness." Carry the positive-match confirmation forward to any future corpus
with a Yawgmoth attacker.

## HARNESS / SERIALIZATION

### B. [NEW, LOW, non-deciding] stale_echo via DEGENERATE OPTION LABEL `Becomes ` (distinct from the superstring class).
**vs62 seq29 T8 my13/opp20** (deck133-vs-deck62 file). Options serialized `['Becomes ', 'Decline - do
nothing']` — a Liliana, the Last Hope `+1` target sub-menu whose entries render as a bare `Becomes `
prefix with NO target creature and NO resulting stats. The model's echo `CHOICE: 1 (Activate
Liliana's +1 ability on Canopy Spider)` is a coherent, correct, present intent but cannot match the
stub label -> stale_echo -> heuristic (which added the loyalty counter anyway; non-deciding).
- **Candidate fix (HARNESS/SERIALIZATION, LOW):** emit the full target-menu option label
  (creature name + resulting P/T, e.g. `Canopy Spider becomes -1/4`) instead of the truncated
  `Becomes ` stub. This is upstream of the matcher — loosening the matcher cannot help (there is no
  name to match). Affects any deck running Liliana the Last Hope's +1 or similar stat-change target
  menus. Single seat; carry as a WATCH ledger item, not urgent.

### C. Fallbacks: 5 (2 stale_echo + 3 unparsed_reply), all choice=-1, heuristic played, NONE deciding.
- stale_echo: vs17 s11 (TRUE — Thoughtseize discard-step confusion; echo names the cast step while
  engine advanced to the discard-pick step); vs62 s29 (TRUE but degenerate-label, item B above).
- unparsed_reply: vs21 s30 (ramble, no CHOICE line); vs62 s32 (GM devotion-ramble, model #A below);
  vs62 s39 priority (2nd-main plan-ramble after combat already resolved, harmless).
- **empty_reply = 0** (Spark-death class stays CLOSED, 5th clean corpus). Verified no self-Fatal-Push
  / no self-destroy resolved at any fallback window.

### E. `resolved:` field still not emitted (reviewer-facing, carried wave-9..15). Low priority.
The `events` narration again carried the deltas used to verify FP kills (x3), GM-as-body combat
(vs62), and Liliana counter adds. An explicit `resolved: battlefield|graveyard|fizzled|countered`
would remove the manual scrape. ALSO carried: `options` is intermittently serialized as an INT COUNT
rather than the option list (e.g. this seat's early asks show `options` = 3 while `options_text`
holds the list) — reviewers use `options_text`; if cheap, make `options` consistently the list.

## MODEL / REPRESENTATION

### A. [carried, LOW, non-deciding] GM "don't hand-count devotion" line disobeyed under devotion-math temptation.
vs62 seq32 (unparsed_reply): GM offered `drains 8`; the guide says "do NOT count devotion by hand ...
use the shown number directly." The model wrote a long manual devotion count instead of reading the
shown N and never emitted a CHOICE -> ramble guard -> heuristic. Non-deciding (WAIT at N8<opp18/my12
was guide-correct). Present-and-disobeyed -> NO new guide text. Carry as a MODEL watch; re-escalate
only if a hand-count flips a DECIDING GM win/stabilize call.

### D. self-Fatal-Push card-presence hallucination (wave-15 notes #A) — did NOT recur. Keep parked.
No self-FP this corpus; all 3 real FP casts hit an enemy creature over an offered friendly target
(Signal Pest, Ice-Fang Coatl, Raging Goblin — all `battlefield -> graveyard`). The Bloodghast-is-
always-a-hand-card confusion had no repro. Keep parked; do not escalate.

## ENGINE / REPRESENTATION — inherited items CONFIRMED STEADY
- **own-target-only Fatal Push annotation** obeyed 20/20 (item A).
- **GM bare-N shown==resolved** — the "+2" workaround stays DELETED; the one GM cast-option window
  (vs62 s32) showed a bare `drains 8` with no N->N+2 inflation.
- **Yawgmoth / Liliana / fetch / Collective Brutality coexistence** clean; no re-ask inflation, no
  no-legal-target loops.

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
- **Self-damage suite (Thoughtseize 2, Phyrexian Arena 1/turn, fetch 1 each) disproportionately
  costly under life-adjudication.** vs44 LOSS (my20->8 on 3x Thoughtseize + fetch vs a control deck
  that only reached 18, no closer drawn — finding 4); two of the three Thoughtseize casts were
  post-T3 against non-discard-answerable threats. A 1-copy Thoughtseize->Inquisition shift and
  cutting Phyrexian Arena would reduce self-inflicted bleed in life-capped games. Carried
  wave-10 #11 / wave-11 #12 / wave-13 / wave-15.
- **No reach / no flyer answer except Gray Merchant.** vs17 (Faeries): dealt ZERO damage all game
  (opp flat at 20), GM never drawn (density 4/60). Carried wave-9..15.
- **No early blocker; Bloodghast cannot block.** Did NOT bite this wave (vs110 and vs21 flipped to
  wins on drawing removal / winning the race), but the structural hole is unchanged. Consider a cheap
  early blocker. Carried wave-10 #10 / wave-11 #11 / wave-13 / wave-15.

## ROTATION VERDICT — ROTATION-OUT CANDIDATE (lean), with one minor cross-seat nit flagged.
Guide is a ZERO-edit frozen diff (11th+ consecutive freeze) — first rotation condition met. On the
second condition (new work signal), deck133 is now MUCH cleaner than wave-15:
- Item A (its signature open lever) recurred 0/20 -> the suppression lever is DOWNGRADED to parked
  WATCH, not fresh build signal.
- Item B validated by absence; residual is awaiting a positive Yawgmoth-attack witness, not new work.
- self-Fatal-Push watch CLEAN; GM devotion-ramble and card-presence hallucination both parked.
- The ONLY genuinely new item is the degenerate-label stale_echo (notes #B) — and it is
  HARNESS/SERIALIZATION and CROSS-SEAT (any Liliana-the-Last-Hope +1 menu), not deck133-specific;
  deck133 does not need to stay in rotation to keep watching it.
**Recommendation: deck133 is a rotation-OUT candidate this cycle** — the guide is stable and its two
signature open items resolved clean/parked, leaving no deck-specific new work signal. If synthesis
prefers one more cycle, the only justification is to catch a positive Yawgmoth-attack superstring
match (item B's un-exercised shape); that is a harness-validation reason, not a guide reason. Flag
the degenerate-label nit to whichever seat next hits a stat-change target menu.
