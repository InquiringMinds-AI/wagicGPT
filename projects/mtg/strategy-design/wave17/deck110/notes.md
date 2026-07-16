# Deck-110 wave-17 — development notes (layer-routed engine/harness ledger + rotation verdict)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-151559/`, 6 deck110 seat translogs.
Binary `/tmp/wagic-7cdcf9c73` (wave-16 batch: PLAN-caveat + legalCasts normalEntry fix +
own-target-only BAD-effect suppression). deck44/deck133 rotated OUT; deck49 (Dragons) + deck35
(Slivers) entered guideless. Wave-11 guide FROZEN (live `deck110_strategy.txt` md5 `00849aa9`,
byte-identical **10th freeze**). Answer-after-plan + name-echo. Card facts vs
`bin/Res/sets/primitives/mtg.txt`. Every item carries a repro; engine/harness items live HERE, never
in guide text. Record: **4 WIN / 2 LOSS** (21,35,62,135 win; 17,49 loss). Both losses DC1
(flood/matchup), decision-clean.

## H1 — HARNESS / DOWNGRADE HIGH→RESOLVED-WATCH: the wave-16 stale-plan spiral class (self-Galvanic-Blast + carry-vs-prompt 14k loops) DID NOT RECUR. Canary PASS.
Wave-16 H1 was the prompt re-injecting the full prior PLAN unreconciled, producing (a) a game-costing
self-blast (deck21 s6→s8) and (b) ~14k-char contradiction spirals (deck21 s13, deck133 s9). The
wave-16 batch added the PLAN-carry caveat + own-target-only suppression. This wave:
- **No self-blast corpus-wide.** All 4 Galvanic Blasts hit the opponent (deck17 s9 face, deck135 s16
  face, deck49 s15 face, **deck21 s9 → Scuzzback Scrapper**). The **deck21 game is the REMATCH** of
  the wave-16 blunder: s8 reply reasoned the Scuzzback-Scrapper kill ("Wither ... high value"),
  s9 targeted it, fb=None; deck110 WON 14/-9. Exact-spot before/after is clean.
- **Caveat fires + re-derives.** Present on 59/~181 seat decisions; 55 re-derived to a valid on-menu
  option, only 4 fell back (~93%). Clean stale-plan abandonments incl. deck17 s6/s10/s21, deck49
  s16/s17.
- **Contradiction-spiral subclass gone.** 3 unparsed this wave, none is a carry-vs-prompt loop
  (deck17 s24 = E6 puzzle; deck17 s30 = hopeless 3-life ramble; deck62 s14 = mana-sequencing
  over-think in a WON game).
RESIDUAL (bounded): 4 caveat-present fallbacks (deck17 s29, s30; deck49 s34; deck62 s14). s29/s34 are
E6-driven (unreadable sole option — the caveat can't rescue a menu bug); s30/s14 are position/
sequencing rambles. LEVER (if ever re-promoted): none needed at prompt level — the residual is E6's
to fix. Owner: `AIPlayerGPT.cpp` prompt assembly. Status: **RESOLVED-WATCH** (re-verify at any seat
that still carries a rambly plan tail next corpus; do not re-open unless a self-blast or a genuine
carry-vs-prompt loop reappears).

## E6 — ENGINE / REPRESENTATION (OPEN, priority MEDIUM→MEDIUM-HIGH): "Cast Card Normally with Mox Opal {Mox's own rules text}" is a MISLABELED mana-ability activation surfaced as a top-level (often SOLE) GPT action. Seat's #1 fallback driver.
CHARACTERIZATION (what the engine was showing): a `priority` decision, options=1, sole option =
`Cast Card Normally with Mox Opal {card text: "Metalcraft - {T}: Add one mana of any color.
Activate this ability only if you control three or more artifacts."}`. NOT a card cast — the Mox was
cast Turn 1; this is the engine offering to TAP the in-play Mox for 1 mana of any color as a
standalone action (a no-op mana float, since no spell is being paid for). Confirmed by the game log:
every time the model picks it, the log shows `- You: Cast Card Normally with Mox Opal {...}` with no
card entering and no stack object. Two label faults: (1) says "Cast Card Normally" for a mana-source
TAP; (2) dumps the Mox's OWN reminder text into the `{card text:}` (card-being-cast) slot.
FAILURE MECHANISM: when the Mox tap is the ONLY legal action AND the model's plan names a spell that
is genuinely uncastable (unpayable or already resolved), the model can't map intent onto the
unreadable sole option → mis-echo or ramble. Repros (game vs deck17 + deck49):
- deck17 s28/s29: plan "Cast Master of Etherium {2}{U}"; `Mana available: {1} from 1 source | pool
  {w}` → {2}{U} unpayable → Master correctly not offered → sole option = Mox tap → echo "Cast Master
  of Etherium" → stale_echo. s29 CARRIED the PLAN-caveat and still failed (proves the caveat can't
  rescue an unreadable sole option).
- deck17 s24: rambles 13.6k chars, diagnoses the mislabel ("actually 'Activate Mox Opal' but ...
  mislabeling it? ... This is a puzzle") → unparsed_reply.
- deck17 s25: echoes "Cast Thoughtcast" (off-menu) → stale_echo.
- deck49 s34: at 1 life, sole option = Mox tap; echoes "Cast Cranial Plating" (cast at s33) →
  stale_echo.
Chosen-harmlessly (contrast, no fallback): deck17 s7/s12/s14/s17/s18/s23/s32; deck49 s9/s21/s22/s25/
s26/s29/s31/s32. OUTCOME COST this wave = none (all in already-lost deck17 line or dead deck49
endgame; staleness route caught every one, no lethal miss) — but 5 wasted round-trips and the seat's
dominant fallback source, exactly the profile E6 was promoted on.
LEVERS (owner `AIPlayerGPT.cpp` option serialization): (1) RELABEL as `Tap Mox Opal: add one mana of
any color`, never "Cast Card Normally", never the source's reminder text in the card slot; (2)
STRONGER — SUPPRESS bare mana-ability activations from the GPT priority menu when no cast is in
progress (behavior-neutral; auto-tap already covers payment; would delete all 5 fallbacks). Never as
the SOLE option. Cross-deck (any mana rock, any seat). NOT guide-fixable (guide already describes the
line correctly; the model still can't parse the sole-option menu — representation bug).

## E2 — ENGINE / REPRESENTATION (UNFIXED, dormant): equip no-op marker is TARGET-IDENTITY-keyed, blind to net-zero MOVES. DID NOT RECUR — validated-by-absence, 3rd wave.
10 equip decisions, all value-positive by OUTCOME DELTA. Every `(ALREADY — change NOTHING)` re-offer
was PASSED: deck21 s17 (equipped 2nd Plating→Memnite, passed the ALREADY), deck35 s13 (2nd
Plating→Ornithopter, passed 2 ALREADY), deck35 s28 (passed the ALREADY). First-equips of separate
Platings: deck135 s10/s25, deck62 s11/s19, deck35 s7/s13. ZERO ALREADY options taken corpus-wide
(Rule #1 100%). The identity-keyed net-zero-DOWNGRADE-move blind spot got no exercise (all moves
raised resulting power). ROOT & LEVER unchanged: compute the marker from OUTCOME DELTA — suppress
"Equip ... targeting X" whenever moving would not raise X's resulting power above the current
carrier. Owner: `AIPlayerGPT.cpp` equip-option serialization. Priority MEDIUM/dormant.

## DEFERS — 3 deferred_to_heuristic (the corpus's only 3, all at this seat): benign, CORRECT.
deck17 s3, deck49 s3, deck49 s4 — all `prompt=""`, `latency=-1`, `choice=-1`,
`chosen="Glimmervoid"`: the harness deferred a forced land-drop to the heuristic (no LLM call) and it
played a Glimmervoid. Playing Glimmervoid is never wrong for this deck (free untapped any-color land +
artifact fodder). All 3 correct; saved 3 round-trips. Harness working as intended. No item.

## DC1 — DECK-CONSTRUCTION / META (standing, reaffirmed — owns the loss column).
- deck17 -8/18: Faeries. Control Magic STOLE the Ornithopter; deck110 flooded (mid/late hand stuck
  on Mox/Thoughtcast/Master, Master repeatedly unpayable) vs a grown flying board. No legal damage
  path by mid-game.
- deck49 -4/16: Dragons/fast-red. Pure flood — 1-creature keep (Ornithopter), then lands + one
  Lightning-Bolted 0/2; hand line was "Mox Opal" every turn. Raced 20→1.
Mulligan NOTE (reaffirmed): all opening keeps were again 1-creature and the deck went 4/2 — do NOT
tighten the mulligan rule (would ship winners). Not guide tasks.

## Positive validations (continuity, no action)
- Galvanic targeting 4/4 opponent (no self-target). Own-target suppression (batch c) largely didn't
  need to fire (opponent targets legal alongside own; model chose opponent, e.g. deck135 s15).
- legalCasts unpayable-hardcast fix (batch b) working: deck17 s28/s29 Master non-offers were the fix
  correctly withholding an unpayable {2}{U}.
- Name-echo 0 wrong remaps. Attackers reconcile sane. E3 metalcraft count not re-litigated into a
  wrong pick.

## ROTATION VERDICT (per-deck rotation rule) — HOLD (marginal): do NOT rotate deck110 out this cycle, but it is now a NEAR-ROTATION candidate.
Rule: rotate a veteran out only if (1) its guide takes NO modification AND (2) its seat surfaces NO
new work signal.
- Condition 1 MET: guide FROZEN (10th freeze, terminal — no decision-level class has earned a change
  in ~7 waves).
- Condition 2 PARTIALLY met: the seat's headline wave-16 item (H1) is now RESOLVED (canary pass), and
  E2 is validated-by-absence a 3rd time. The ONLY live new-work signal is **E6**, and it is an ENGINE
  item this seat has now fully characterized with repros — the diagnostic work is DONE; what remains
  is the fix in `AIPlayerGPT.cpp`, not further seat analysis. deck110 also remains the canonical
  VALIDATOR for the Galvanic finisher and the equip outcome-delta behavior (exercised at scale only
  here), which is a reason to keep it in the pool as a regression canary even after E6 lands.
DECISION: **HOLD this cycle** (E6 is still OPEN and this is the seat that owns/reproduces it — losing
the seat before the fix ships would drop the regression witness). It becomes a CLEAN rotation
candidate once **E6 is fixed AND a subsequent corpus confirms the degenerate Mox menu is gone with no
new class** — at that point the guide is terminal and every owned item is closed. Flag for synthesis:
if E6 is scheduled to land before wave-18, plan to rotate deck110 out after the post-fix validation
corpus.
