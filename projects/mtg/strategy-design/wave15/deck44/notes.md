# Notes for wagicGPT development — deck44 wave 15 (self-contained; full repros)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138/`. deck44 = deck0 in all 6 games.
Binary `/tmp/wagic-aad32bc7f`. **Record 1W-5L** (W vs135; L vs110, vs21, vs133, vs17, vs140-adj). 126
decisions, 3 fallbacks (all correct guard), name-echo 125/126. Win column is context only (doctrine).

game -> result (`results.tsv`, deck0=44):
- `1784195382` vs110 L (kill -2/1 T11) | `1784197337` vs21 L (kill -2/1 T11)
- `1784198247` vs133 L (kill -4/7 T15) | `1784200032` vs17 L (kill -4/9 T21)
- `1784202539` vs140 L-adj (15/19 T22) | `1784204084` vs135 W (20/0 T14)

## Guide change this wave: NONE (frozen byte-identical)
Live guide md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines; `diff` vs wave-13 strategy.txt empty. The 1/6
is variance + matchup + the harness combat-ask removal (below), NOT a guide or pilot-decision regression.

---

## ENGINE / HARNESS LEDGER (layer-routed here, NOT prompt text)

### #1 (NEW, HIGH, TOP ITEM THIS WAVE) — combat asks (attacker + blocker declaration) were REMOVED corpus-wide
Confirmed by phase enumeration on both corpora:
- THIS corpus (`matchups-20260716-040138`): deck44 ask phases = {Main phase 1: 110, Main phase 2: 16}; whole
  corpus phases = {Main phase 1, Main phase 2, Upkeep, Combat damage}; **0 `Blockers`-phase asks and 0
  attacker/blocker-declaration asks at ANY seat**.
- WAVE-13 corpus (`matchups-20260715-183228`): deck44 had a **`Blockers` phase (3 asks)** + `ATTACK:`-format
  attacker declarations in main phase.
So combat is now fully auto-resolved by the engine; the LLM's only combat-adjacent choice is a removal TARGET
sub-menu. **Outcome cost (deck44):** vs110 s26 and vs21 s22 both reached opp=1 / deck44=9 in MP2, then died
9 -> -2 on the T11 crackback because the auto-attacker declared ALL creatures (tapping them) and the
auto-blocker had nothing to hold back — the guide's "chump the biggest at 8 or less" survival line is
unreachable when the pilot is never asked. FIX/DECISION NEEDED FROM OWNER: is auto-combat intended?
- If YES: (a) guides' BLOCKING/LETHAL/attack-selection sections corpus-wide are inert and should be trimmed
  (core/method change, synthesis-owned, NOT single-seat); (b) revisit the auto-attacker heuristic's all-in
  policy — a "don't swing a creature you need to survive the crackback" rule belongs in the engine now, not
  the guide.
- If NO (harness regression): restore attacker/blocker asks; deck44's existing guide lines re-arm.
Owner: engine/harness combat surface + the AIPlayerBaka auto-attack/auto-block path.

### #2 (VERIFIED FIXED — wave-13 ledger #1 self-destroy/reconcile) absent-echo staleness route works
Wave-14's `stale_echo -> choice=-1` route CLOSED the wave-13 self-GFTT class at this seat. Two absent-echo
events, both correctly refused the raw index-1 cast:
- vs140 `1784202539` s22 (T17): options `[1] GFTT own-only / [2] nothing`; reply `CHOICE: 1 (Cast Counterspell)`
  (Counterspell not offered — pilot was blue-short, see #4) -> `fallback=stale_echo`, choice=-1, did NOT cast
  the own-only GFTT. Correct catch (a wave-13-shape self-destroy averted).
- vs135 `1784204084` s48 (T14): options `[1] GFTT (opp Treefolk legal) / [2] nothing`; reply echoed prior
  land decision `CHOICE: 1 (Play Island)` -> `fallback=stale_echo`, choice=-1; heuristic then cast GFTT and
  the pilot picked opp Treefolk at s49 (harmless; game already won).
Self-destroy grep: 0 hits. Target sub-menus: 4/4 -> `[opponent's battlefield]` (vs17 s10 Scion, s18 Wasp
Lancer; vs135 s24 Ohran Viper, s49 Treefolk). Mark the reconcile-arm item RESOLVED unless another seat shows a
WRONGFUL downgrade (echo that DID name an offered option yet routed to heuristic — none at deck44).

### #3 (representation, LOW/log-only) forced sub-menu framing — safety case closed, cosmetic only
Target sub-menus are still bare (no committing-spell header, no cancel), but with #2's fix + the model reading
`[opponent's battlefield]`, they no longer cause self-harm at this seat (4/4 correct). The header + own-vs-opp
annotation is a cross-deck nicety, de-prioritized below #1. Owner: option/sub-menu prompt builder.

### #4 (harness/parse) unparsed_reply guard fired once, harmless
vs133 `1784198247` s22 (T14): land-drop ask, reply rambled with no CHOICE line -> `fallback=unparsed_reply`,
choice=-1 (heuristic resolved the land drop). deck44 at 2 life, dying regardless. The "reasoning rambles,
guard working" class; no cost. Owner: reply parser (already behaving correctly).

---

## Decision-quality confirmations (verification of shipped fixes)
- **Name-echo:** 125/126 CHOICE lines carry `(name)` (the 1 miss = the s22 unparsed ramble).
- **Fallbacks:** 3, all correct guard (2 stale_echo correct catches, 1 unparsed). 0 wrongful downgrades.
- **Counter spine:** all correct/defensible. NEW: **cat #1 sweeper counter TESTED & CORRECT** (vs140 s14
  Wrath of God) — the wave-13 "sweeper untested" gap is closed. Others: Cranial Plating x2 (vs110), Staff of
  Nin (vs140), Ohran Viper + Abominable Treefolk (vs135), Control Magic + 2nd Glen Elendra Liege (vs17).
  Declines correct (Arcum's Astrolabe, Into the North, cheap goblins).
- **GFTT:** 0 self-targets; held off Bloodghast (vs133 guide anti-pattern respected).
- **Auto-tap color-stranding:** RETIRED as misdiagnosis per brief — NOT relitigated. The one blue-shortage
  (vs140 s22) is a genuine board-composition constraint (Swamp-heavy board, blue spent on own T16 creatures),
  logged to construction (#5 of findings), not to auto-tap.

## Deck-construction flags (for the user / decklist owner) — persists waves 5-15
16 Island / 10 Swamp + 6 reactive (2 Counterspell {U}{U}, 2 Arcane Denial {1}{U}, 2 GFTT {1}{B}). The
counters EARN their slots (carried the vs135 win + the vs140 Wrath counter) — "trim counters" is the WRONG
lever. Real levers: (a) blue-source density vs the double-blue counter load (vs140 s22: couldn't pay {U}{U}
for a needed Pyroclasm counter); (b) 1-2 more cheap flyers / a black-castable interaction to raise the aggro
floor and win the races it currently loses by one turn (vs21, vs110 both ended opp=1). The slow-reactive,
creature-light keep is the loss signature; the deck loses close races to faster boards (goblins 21, affinity
110) and to bigger flyers (Faeries 17) and drain engines (Gary 133).

## ROTATION VERDICT (explicit, per brief)
**Guide: FREEZE. Seat: ROTATION CANDIDATE — but SOFT-HOLD this cycle pending the #1 auto-combat triage.**
Rationale: deck44's guide took no modification (11th frozen wave), the pilot's reachable decisions are clean,
and both wave-13 open items resolved (self-destroy CLOSED via #2; forced-sub-menu downgraded to cosmetic via
#3; auto-tap RETIRED). On deck-specific grounds it is a clean rotation candidate. HOWEVER, this cycle it
surfaced a load-bearing, corpus-wide harness change (#1, combat asks removed) that (a) explains the low
win-rate without any deck44 fault and (b) determines whether the guide's ~40% block/lethal prose must be
trimmed corpus-wide. That triage is deck-AGNOSTIC (synthesis/engine-owned, not deck44 work), so it does not
by itself keep deck44 in the active pool — but deck44 should not be rotated out until #1 is answered, because
the answer directly implicates whether its frozen guide needs a (mechanical) block-prose edit. Recommendation:
**hold one cycle; rotate out once #1 is resolved and any resulting block-prose trim is applied.**

## One-line handoff
Top deck, guide FROZEN 11th wave, 1W-5L. The drop is variance (vs133 counter-light draw flip; vs131 reliable
win rotated out) + hard new matchups (17 bigger-flyer mirror, 21 goblin swarm) + a CONFIRMED harness change:
attacker/blocker asks were removed corpus-wide, so an all-in auto-attacker left deck44 with no crackback
blocker and it died at 9 -> -2 with the opponent at 1 in both vs110 and vs21. NOT a regression; 4/5 losses are
hard board kills, not adjudications. Wave-13's self-destroy class is CLOSED (absent-echo route: 2 correct
catches, 0 self-destroys, 4/4 target sub-menus correct). Sweeper counter now tested (vs140 Wrath). Engine
ledger #1 (auto-combat) is the cycle's dominant signal; guide byte-identical FROZEN.
