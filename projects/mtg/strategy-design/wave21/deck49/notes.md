# notes.md — deck49 (Dragons) engine/harness/card ledger + rotation verdict (wave 21)

Layer-routing: engine/harness/card items here with seq repros (no prompt text). Guide edits -> none
this corpus (byte-copy). Core/decode -> general-suggestions.md. Card/roster items -> flagged to the
card/roster owner below.

## HARNESS-49a (NEW, MEDIUM) — stale_echo FALSE-POSITIVE on a valid answer-first reply
- **Repro:** deck35 game (1784820290-...-vs-ai_baka_deck35.jsonl), seq18. kind=ask, turn 8, my 20 /
  opp 8. options=[Cast Goblin War Paint (own targets), Cast nothing]. latency 83811ms, reply 5730 chars.
- The reply's FIRST line is a valid `CHOICE: 1 (Cast Goblin War Paint)` (option 1 legal). It is NOT a
  byte-repeat of seq17 (md5 differs: seq17 3b75c21c..., seq18 b963667c...). The harness routed it to
  heuristic anyway: choice=-1, fallback="stale_echo".
- **Consequence: benign only by coincidence.** The heuristic cast War Paint; seq19 targeted Thunderbreak
  Regent (the model's stated intent), buffing 4/4->6/6; seq20 swing (Regent 6/6 + Whelp 2/2, unblocked
  flyers) was lethal, deck49 won that turn (op 8 -> -1). The harness DISCARDED a correct fresh answer;
  no disavowal by the model. If the heuristic had disagreed, this would have been a real misplay.
- **Hypothesis (owner to confirm):** the seq18 reply's PLAN block re-states seq17's PLAN nearly
  verbatim ("...cast Goblin War Paint on Thunderbreak Regent ... next turn I win"); if stale_echo
  fingerprints the reply/PLAN against the prior turn, a near-duplicate PLAN from a re-deriving spiral
  trips it. RECOMMENDATION: gate stale_echo so it does NOT fire when the current reply carries a
  well-formed, LEGAL, order-agnostic CHOICE line that differs from the prior turn's CHOICE (here seq17
  CHOICE was "Play Mountain", seq18 was "Cast Goblin War Paint" — different decisions, same PLAN
  narrative). A valid distinct coded answer should always beat a PLAN-narrative echo. Low urgency (1
  benign occurrence this seat), but it is a live false-positive class under the new answer-first
  protocol and should be checked corpus-wide by the harness owner.

## CARD-49-ashenmoor (ESCALATED to GAME-MATERIAL; ROUTE TO CARD/ROSTER OWNER) — Ashenmoor Liege 4/1 vs real 4/4
- **Primitive:** projects/mtg/bin/Res/sets/primitives/mtg.txt — `name=Ashenmoor Liege / power=4 /
  toughness=1`. Real Oracle (Shadowmoor): Ashenmoor Liege is a **4/4** ("Other black creatures you
  control get +1/+1. Other red creatures you control get +1/+1. Whenever a source deals damage to
  Ashenmoor Liege, that source's controller loses that much life."). The primitive also drifts the
  triggered ability to a targeting-based "that player loses 4 life" — a separate, lower-priority text
  drift.
- **Game-material impact (repro):** deck27 game, seq15 blocks. Menu: "B1. Ashenmoor Liege (4/1) - may
  block A1 (both die)" vs A1 Zombie Outlander (2/2). deck49 at 18 life, no lethal pressure. deck49
  blocked and LOST the Liege (both die, per the 4/1). With the TRUE 4/4, the Liege kills the 2/2 and
  SURVIVES, keeping deck49's only anthem/threat. The block reads as correct given the shown 4/1 (this
  is the G-49c hazard — the "(both die)" annotation is accurate to the wrong stat), so the misplay is
  invisible at the decision layer; the fault is the card script. This aggravated the deck27 loss
  (deck49's board never recovered a threat).
- **FAITHFUL fix (per wagic-cardscript rule — data, no recompile):** set `toughness=4` in the mtg.txt
  Ashenmoor Liege entry (and, separately/later, correct the damage-punisher text). I did NOT make this
  edit — a shared primitive change affects deck power level (deck49's anthem becomes a real 4/4 body)
  and other seats' card renderings; that is a roster/card-owner decision, not a per-deck-review action.
  Flagged for the card/roster owner with the repro above. Escalation rationale: this was "queued/
  cosmetic" through wave-20; wave-21 shows it costing a game.

## E-batch21 confirmations at this seat (confirm, keep)
- **ANSWER-FIRST protocol: WORKING.** Every long reply this seat carries a coded first line — deck27
  seq22 (12939c) `CHOICE: 2`, seq23 (13042c) `BLOCKS: B1:A5`, deck102 seq14 (9541c) `CHOICE: 1`,
  deck35 seq18 (5730c) `CHOICE: 1`. 0 unparsed_reply, 0 empty_reply, 0 truncation-unparse (wave-20 had
  3 here). The wave-20 develop-seam truncation class (E-49d GUIDE component) is DEAD (guide dissolved
  the trigger; see findings). The DECODE component (repetition/progressing spiral) persists but now
  parses (general-suggestions G-49a).
- **retracted_choice / prose-salvage: N/A at this seat** (0 of each). The seat's only fallback is the
  stale_echo false-positive above.
- **E-49e (loop-salvage scope) — no test this corpus.** No ask/target deliberation loop reached a
  salvage decision (answer-first put the coded line up top before any loop could mangle it). The
  wave-20 recommendation (keep loop-salvage scoped to format-mangled single intents, do NOT extend to
  oscillating two-aim asks) stands, untested-but-unthreatened.
- **E-49f (stolen-threat / Persuasion) — did not recur** (deck14 curved out clean this corpus, no
  theft). The 0-attacker disqualifier list ("is the creature still yours?") remains a standing check.
- **Attacker-sparing payment:** N/A at this seat (deck49 pays off Mountains, no creature-mana); no
  tapped-attacker seam to stress. Nothing to add.

**Engine items this corpus: 1 NEW harness (HARNESS-49a stale_echo false-positive, MEDIUM) + 1 ESCALATED
card (CARD-49-ashenmoor, now game-material, to card owner) + batch21 answer-first confirmations +
standing Oracle queue (Bloodmark 1/1 vs 2/1, War Paint {1}{R}/+2+2 vs {R}/+1+1, Crucible {3}{R} vs
{2}{R}{R} — unchanged, lower priority than Ashenmoor).**

## Rotation verdict — GUIDE CONVERGED; recommend deck49 rotate to FROZEN-CANARY (all residual work is layer-routed OFF the guide)

**All three wave-20 exit questions answered (findings):** (1) develop-seam fallbacks DEAD — YES,
emphatically (6/6 clean sub-1200-char commits; the guide edit dissolved the trigger, not merely
parse-absorbed it — proven by reply LENGTH). (2) burn-face rate HELD (13/13 face, 0 creature-targeted)
without over-correction harm; the exception's narrow true-fire is UNDER-TESTED (no must-kill-blocker
spot arose). (3) "either is fine, COMMIT" blind-cast risk UNTESTED (no lethal-burn fork arose) — no
harm observed.

**Standing rotation test (from wave-20 notes): no-guide-mod AND no-new-work-signal = rotation
candidate.**
- **no-guide-mod: TRUE.** strategy.txt is a BYTE-COPY (md5 479f907edbbc57e76c02bfaab8ba12d9, identical
  to the deployed wave-20 revision). No decision-evidenced change is warranted — the edits validated,
  and nothing new in the guide's own scope fired wrong.
- **no-new-work-signal: FALSE** — but ALL of it is routed OFF the deck49 guide: HARNESS-49a (harness
  owner), CARD-49-ashenmoor (card/roster owner), G-49a residual tax (core/decode). NONE is deck49-
  guide work; the deck49 guide itself is done.

**Verdict:** the deck49 GUIDE is CONVERGED (two waves of validated edits, all taught classes surviving,
byte-frozen). Because every residual item lives at another layer, deck49 does not need another active-
revision cycle. **Recommend rotating deck49 to FROZEN-CANARY status (like decks 135/35)** — keep the
guide deployed byte-identical as a control while the cross-layer items are handled by their owners. It
is NOT a hard rotation-OUT-of-pool candidate only in the literal "no-new-work" sense; the honest read
is that its GUIDE work is complete and it should freeze, not keep taking active review bandwidth.
Re-open the deck49 guide only if a FROZEN corpus surfaces a taught-class regression or the Ashenmoor
4/4 fix (if the card owner ships it) changes deck49's play pattern enough to need a guide note.
