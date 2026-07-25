# deck133 wave-25 — development notes (engine / harness / model / construction) + ROTATION VERDICT

Corpus PRIMARY `matchups-20260725-035605`, binary `62d8783a5` (wave-25 step-1 on rules-fixed
`f0500ad66`), pilot repetition_penalty=1.05. Real games: 131(W), 137(DRAW/timeout), 27(L-adj),
59(L), 102(L), 109(L) — **1W/4L/1D.** Layer-routing enforced: engine/harness/model/construction
live HERE with seq repros; the guide has ONE surgical edit (findings.md finding 1); win/result
column is context-only.

## GUIDE — [ONE EDIT this wave] Phyrexian Arena line sharpened (was too passive; model overrode it).
Only change to strategy.txt this cycle. The existing "PHYREXIAN ARENA: 1 life every upkeep for a
card - skip casting it when defending" was quoted-then-violated by the model in BOTH aggro/tempo
losses (vs109 s19 @5 life, vs102 s28 @7 life), rationalizing Arena as "stabilizing" / "immediate
card advantage" / "find an answer." Rewritten to name (a) the TIMING fact — Arena draws at your
NEXT upkeep, not on cast (engine `auto=@each my upkeep:draw:1`), so under lethal-next-turn the
draw never arrives; (b) that it is "not a play, it is a countdown"; (c) cast NOTHING at low life
with no other play. Grounded in oracle + script. Both instances NON-DECIDING (already lost) but a
2-window convergent latent-deciding pattern. Repro: grep any deck133-seat log for `Cast Phyrexian
Arena` chosen_text and cross-check `my_life` <= 10.

## HARNESS/PARSER — [OPEN, low-priority — de-fang deployed, but keep the hardening] retracted_choice example-leak (b1).
The b1 de-fang (protocol example -> `"CHOICE: 3 (Cast Example Card)"`, a non-live name) is DEPLOYED.
Acceptance at this seat (the corpus's sole prior retracted_choice producer): **0 retracted_choice
corpus-wide** (all 15 fallbacks are deck137 convoke); AND the quoting shape VANISHED (0/227 replies
quote the new example; 0 `"CHOICE:` in-prose; 0 format-spirals). This is the WEAK-evidence rung and
weak is the CEILING: the fix works by removing the aggravator that produced the quote-latch, so the
STRONG confirmation (model still quotes the de-fanged example AND it parses clean) can no longer
occur — I cannot distinguish "de-fang killed it" from "model didn't quote CHOICE this corpus." No
counter-evidence; result consistent with the fix. RECOMMEND the wave-24 parser hardening STILL ship
as belt-and-suspenders: (1) count only LINE-ANCHORED coded CHOICE indices for the retraction/second-
index scan; (2) EXCLUDE `CHOICE:` tokens inside quotation marks or that echo the protocol `e.g.`
example. The de-fang is a trigger-removal, not a parser guarantee; a future example text or model
behavior shift could re-trigger.

## MODEL/RULES-KNOWLEDGE — [note-only, folded into the guide edit] model believes Phyrexian Arena draws "immediately" on cast.
vs102 s28 reply: *"it provides immediate card advantage (draw 1, lose 1)."* Arena draws at the
BEGINNING OF YOUR UPKEEP (`@each my upkeep:draw:1`), not on resolution. This mis-timing is what
makes the low-life Arena cast strictly wasted (you die before the draw). Addressed in the guide
edit (states the timing); no engine/core action.

## RULES-SHIFT (batch A) — [no observable effect at this seat this corpus]
- **a1 (turn-1 upkeep fires):** deck133's only upkeep-trigger card is Phyrexian Arena; Bitterblossom
  NOT in the deck. Arena cast at t7/t11 only, never t1 — no turn-1 upkeep trigger surfaced. Mulligan
  now renders at `t0 Untap` (one Keep per game, all 6, no double-ask); starting player skips turn-1
  draw. Clean.
- **a2/a3/a4/a5/a6:** no deck133 decision touched a redirect menu, Battle, or day/night surface. No
  shift, no new confusion observed. (a4 pw-redirect-removal: deck133 has no direct-damage-to-face
  spell, so untestable here.)

## MODEL/LATENCY — [note-only] the 133v137 TIMEOUT-DRAW is a SLOW GRIND (mirror of wave-24's pattern).
133 vs137 adjudicated DRAW at the 2400s cap, turn 14, even life 10/10. deck133 side ran long
Collective-Brutality-escalate deliberations (s34 202s, s47/s50 ~170-190s each). Two attrition decks,
neither with a fast clock; deck133 never assembled a lethal Gray Merchant (GM offered 0x — not
drawn). Even life at the cap -> fair draw. NOT an engine/phase stall (normal phase progression, 0
empty-reply). Same face as wave-24's 102v133 timeout; the long-tail LLM latencies (p95 8.4KB / 200s+
replies) still bind the wall-clock cap at ~turn 14. Note-only.

## CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
- **Threat-drought is the dominant loss driver this corpus.** In all 3 flipped losses (vs109/102/59)
  Gray Merchant AND Phyrexian Obliterator were offered **0 times** — the deck drew none of its
  payoffs and floods on discard + Bloodghast (which cannot block). The ONE win (vs131) is the ONE
  game GM arrived. This is variance on a top-heavy payoff base, not a decision error. Deck-tuning
  lever (upstream, if ever pursued): more early interaction / a castable early blocker would smooth
  the drought-loss variance; out of scope for the guide.
- **No reach / no flyer answer except Gray Merchant + Liliana -2.** Carried. vs27 improved this
  corpus (dealt 6 dmg, competitive adj loss) but still the structural no-reach shape.
- **No early blocker; Bloodghast cannot block.** Carried — central to the vs109 aggro loss.
- **Self-damage suite (Thoughtseize 2, Arena 1/upkeep, fetch 1 each).** The Arena instances are now
  guide-addressed (finding 1); Thoughtseize-at-16 vs aggro (vs109 s12) and vs control (vs102 s8) is
  quieter but the same family — the guide already caps Thoughtseize to turns 1-3, and both were t3.
  Carried, quiet.

## GUIDE EDIT LEDGER (this wave)
- **strategy.txt REVISED** — line 126 (Phyrexian Arena) sharpened per finding 1. All other bytes
  identical to deployed `6ed73f00...39ce`. cmp shows exactly the Arena-line region differs.
- **KEPT / validated:** Liliana of the Veil +1 caution — CLEAN round 3 (4 activations, 0 anti-pattern,
  findings 3). Two-walker distinction applied correctly.
- **KEPT / unexercised:** Bloodghast forced-sac fodder (no edict at this seat; findings 4); Yawgmoth
  lines (Thran Physician never cast this corpus — Yawgmoth acceptance round 2 could not re-run).
- **NO new rule added:** self-FP, Gray Merchant (1 correct win-mode cast), mulligan all clean by
  existing guide + engine surfaces.

## ROTATION VERDICT — STAY (NOT a rotation candidate this cycle).
The standing test: "no-guide-mod AND no-new-work-signal = candidate" (for rotation OUT). Applied
UNSENTIMENTALLY per the wave-25 brief (the loop's endgame is real; converged seats rotate).
- **Prong 1 (no-guide-mod): FALSE** — the guide was REVISED this cycle (Phyrexian Arena sharpening,
  a genuine 2-window convergent guide-contradicting reasoning pattern, findings 1).
- **Prong 2 (no-new-work-signal): FALSE** — live new work exists: (a) the Arena edit needs round-2
  validation; (b) the example-leak closure is only WEAK-confirmed (shape vanished, ceiling reached)
  and the parser hardening has not shipped; (c) Yawgmoth acceptance round 2 is UNEXERCISED (Thran
  Physician not drawn) and (d) Bloodghast-fodder round 2 remains UNEXERCISED.
Both prongs fail candidacy -> **STAY.** Honest caveat (unsentimental): deck133 is CLOSE to a clean
rotation candidate. Its unique retracted_choice signal is effectively resolved (0 corpus-wide, the
de-fang looks effective — only the strong-confirmation is structurally unavailable). If NEXT cycle
the Arena edit validates (or no more low-life Arena casts occur), the parser hardening ships, and
Yawgmoth/Bloodghast-fodder stay unexercised with the guide otherwise frozen, deck133 becomes a
straightforward ROTATE-OUT. This cycle it stays on the strength of the fresh guide edit alone.
