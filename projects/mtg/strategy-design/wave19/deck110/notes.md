# Deck-110 wave-19 — development notes (layer-routed engine/harness ledger + rotation verdict)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260719-092058/`, 6 deck110 seat translogs. Binary
`72b05535d` (wave-19 batch `4becc83be`: combat name tolerance / parseBlockAssignments name→label second
pass; per-pairing TRADE OUTCOME block annotations; alt-cast pitch surfacing; salvageLoopedChoice). deck21
rotated OUT (2nd converged exemplar); deck27 (Zombies) entered guideless. Wave-11 guide FROZEN (live
`deck110_strategy.txt` md5 `00849aa9`, byte-identical **12th freeze**). Answer-after-plan + name-echo;
blocks via `BLOCKS: B#:A#` OR `BLOCKS: name:name` (name-format now parses). Card facts vs
`bin/Res/sets/primitives/mtg.txt`. Every item carries a repro; engine/harness items live HERE, never in
guide text. Record: **5 WIN / 1 LOSS** (35,49,14,62,135 win; 27 loss) — up from 3/6.

## BLOCK-ECHO NAME-FORMAT — ENGINE / HARNESS: RESOLVED (wave-18's biting OPEN item, now confirmed FIXED).
The wave-19 combat name-tolerance batch resolves creature NAMES to the B#/A# roster labels — the exact
analogue of the working `CHOICE:` name-echo, as proposed in the wave-18 handoff. PROOF at this seat:
- **deck27 s23 (T14) — name-format PARSED.** Reply `BLOCKS: Ornithopter:Zombie Master` → `choice=1`,
  `chosen="Ornithopter blocks Zombie Master"`, `fallback=None`. This is precisely the shape that BIT at
  wave-18 deck21 s18 (`BLOCKS: Memnite: Boggart Ram-Gang` → unparsed → catastrophic heuristic block).
- **deck49 s12 (T5) — label-format still parses** (`BLOCKS: B2:A1` → choice=1). Both forms work.
- Block fallback count at this seat this corpus = **0** (6 block decisions, all parsed). The class the
  wave-18 finding opened is CLOSED. Status: **RESOLVED** — re-verify by absence / name-format parse next
  corpus. Owner: block-reply parsing in AIPlayerGPT harness. Cross-deck.

## TRADE OUTCOME ANNOTATIONS — ENGINE / REPRESENTATION: VALIDATED (correct + quality-positive, NEW positive).
First corpus with per-pairing block-option trade annotations. Both observed at this seat were CORRECT and
one measurably improved the decision:
- **deck49 s12 — annotation drove the OPTIMAL block.** `B2. Master of Etherium (4/4) — may block A1 (you
  kill it, your blocker lives)` vs `B1. Vault Skirge (2/2) ... (your blocker dies, attacker lives)`. Model
  chose B2:A1 (kill-and-keep) over the chump. Annotation → correct play.
- **deck27 s23 — annotation correct in a moot/lost position.** `B1. Ornithopter (0/2) [deals 0 - this block
  kills nothing]` — accurate; block was irrelevant (14 dmg vs 2 life). No mislead anywhere at this seat.
Status: KEEP; no action. Cross-deck reviewers confirm the "(both die)" / gang-block-composite forms.

## SALVAGE / GIANT-REPLY OVERTHINK — HARNESS (dormant, cross-deck): salvageLoopedChoice cannot recover a reply that ends on a PROSE COMMITMENT with no `CHOICE:` token. Both this seat's unparsed fell through this way.
The 2 unparsed (deck27 s18 land-drop @10 life; deck49 s7 cast @T2) are 13–14k-char overthinks that reached
a plain-language decision ("I will play the Mountain" / "I will choose Cast Cranial Plating") but never
emitted `CHOICE: N`. salvageLoopedChoice re-parses the last well-formed CHOICE line — there was none — so
both went choice=-1 to the heuristic. Non-biting (land-drop; early cast in a won game). This is the residual
giant-reply-tax failure mode now that block-echo is fixed. LEVER: extend salvage to resolve a terminal
prose-commitment line against the option roster (same name-echo mechanism). Owner: AIPlayerGPT reply
parsing. Priority LOW/dormant. NOT guide-fixable (reply-protocol layer).

## H1 — HARNESS (stays RESOLVED-WATCH): no stale-plan self-blast spiral. One MILD carry-vs-prompt whiff, non-biting.
- **deck27 s18** carried a faint carry-vs-prompt element: the model re-read a prior "Your plan: ... I will
  attack with Signal Pest" and looped on reconciling it against the current land-drop menu. But it is a
  land-drop ramble that fell to the heuristic (choice=-1), NOT a self-blast and NOT a genuine contradiction
  spiral. H1 stays **RESOLVED-WATCH** — re-open only on a self-blast or a genuine carry-vs-prompt loop.

## E2 — ENGINE / REPRESENTATION (UNFIXED, dormant): equip no-op marker TARGET-IDENTITY-keyed, blind to net-zero MOVES. DID NOT RECUR — validated-by-absence, 5th wave.
6 equips taken, `took_ALREADY=False` on ALL; 24 menus offered an ALREADY option, all declined. Every real
equip value-positive (raised resulting power). Identity-keyed net-zero-MOVE blind spot got no exercise. Two
unparsed this wave are the giant-reply overthinks above, NOT equip ALREADY-takes. ROOT & LEVER unchanged:
compute the marker from OUTCOME DELTA — suppress "Equip … targeting X" when moving would not raise X's power
above the current carrier (subsumes the sole-option ALREADY menus). Owner: AIPlayerGPT equip serialization.
Priority MEDIUM/dormant.

## E6 — ENGINE / REPRESENTATION: RESOLVED (confirmed by absence, 2nd wave).
`grep` count of "Cast Card Normally" across all option/prompt/chosen text = **0** (was the seat's dominant
fallback pre-wave-18; the wave-17 hand-card PUT_INTO_PLAY dead-end filter closed it). Re-verified absent.
No new dead-end shape drives a fallback. Status: RESOLVED.

## DEFERS — 0 this corpus (down from 1). No forced-land defer at this seat this wave. No item.

## DC1 — DECK-CONSTRUCTION / META (standing; the single loss).
- deck27 -9/14, T14: guideless go-wide Zombies out-raced deck110. deck110 fired both Galvanics early to
  face (s6 @opp-20, s15), reached opp-14 by T8, then had no reach/answers to a 4–5 creature Zombie board and
  was ground 20→-9. Early face-burn vs holding a Blast for reach is a judgment call the guide permits;
  single-seat, single-game variance — NOT a guide task (doctrine: win-rate is context; single-seat items
  cannot add/cut core lines). Mulligan/curve sound; deck went 5/1. Do not tighten.

## Positive validations (continuity, no action)
- Galvanic targeting 6/6 to the OPPONENT'S FACE (deck27 s7@20, s16@18; deck35 s6@20; deck14 s10@19,
  s17@14; deck135 s8@19). No self-target; batch-(c) suppression didn't need to fire. TARGET CHOICE
  sub-menu framing clean, 0 target-sub-menu fallbacks.
- Name-echo 0 wrong remaps; "(player, life N)" suffix strip visibly working.
- Attackers reconcile sane every window. Steel Overseer cast/tap parsed (deck135 s25).
- deck135 adj-WIN was decisive aggression (attacked every turn, opp→1), NOT a durdle — cap artifact.

## ROTATION VERDICT (per-deck rotation rule) — ROTATE OUT (clean candidate; self-set precondition MET).
Rule: rotate a veteran out only if (1) its guide takes NO modification AND (2) its seat surfaces NO new
work signal.
- **Condition 1 MET:** guide FROZEN (12th freeze, terminal — no decision-level strategic class in ~9 waves).
- **Condition 2 MET:** wave-18's HOLD was predicated on ONE open blocker — the block-echo name-format class,
  with deck110 as the concrete biting witness (deck21 s18). Wave-18's explicit stated precondition for a
  CLEAN rotation was: *"block-echo matcher fixed AND a subsequent corpus confirms no new class."* This
  corpus satisfies BOTH: block-echo is FIXED (deck27 s23 name-format parses) and NO new class surfaced —
  the 2 unparsed are the pre-existing giant-reply overthink pocket (cross-deck engine territory, not a
  deck110-owned signal), equip is clean (5th wave), E6 stays closed, Galvanic 6/6, annotations helping.
DECISION: **ROTATE OUT** — deck110 is the 3rd converged exemplar (after deck17, deck21). Its self-set
clean-rotation precondition is met; it teaches no new class. Retain the FROZEN guide as a regression CANARY
(Galvanic-finisher targeting, equip outcome-delta, the block-echo name-format fix, TRADE OUTCOME annotation
accuracy). If synthesis prefers to keep a live block-echo regression witness, note that the fix is now
cross-deck-observable at any combat-heavy seat (deck62/deck135 block more), so deck110's witness role is
fully dischargeable elsewhere. No reason to HOLD.
