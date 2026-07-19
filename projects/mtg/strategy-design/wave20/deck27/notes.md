# deck27 notes.md — dev notes + rotation verdict (wave-20)

Seat: Zombies (UB tribal), FIRST GUIDED corpus (Step 0-ter). Binary /tmp/wagic-95e89c204.
Corpus matchups-20260719-151849. Record 3/6 (W vs135/35/49; L vs102-adj, 62, 14-adj@cap).

## ROTATION VERDICT: DO NOT ROTATE — new-work-signal PRESENT (guide revised + new engine item)

The seat produced BOTH a decision-evidenced surgical GUIDE revision (durdle late-stall off-case,
grounded in vs14 s30 + the turn-cap loss) AND a new, multi-instance ENGINE item (wither "both die"
annotation defect, item 1). Guide-mod present AND new-work-signal present -> not a rotation
candidate. Re-validate next corpus with focus on the GRIND matchups (vs14 / any deck that walls the
ground) — the revision targets exactly the long-stall the winning corpus never exercised.

## ENGINE / representation ledger items (layer-routed here, not into prompt text)

1. **[annotation, VERIFY -> FIX, HEADLINE] Wither blocker "(both die)" is WRONG when the attacker's
   toughness exceeds the blocker's power.** Oona's Gatewarden is a 2/1 WITHER wall; its 2 damage
   lands as two -1/-1 counters, so it only KILLS an attacker whose toughness is <= 2 (or that it
   deathtouches). The trade annotator prints "(both die)" whenever Oona blocks, ignoring survival:
     - vs62 s6:  Birds of Paradise 3/4  -> 2 wither -> 1/2 SURVIVES. Printed "(both die, 2 tramples)". **This mis-annotation directly CAUSED the s6 fallback** — the model correctly detected the contradiction ("if it dies it can't trample"), spiraled, and truncated.
     - vs135 s13: Abominable Treefolk 8/8 -> 2 wither -> 6/6 SURVIVES. Printed "(both die, 7 tramples)".
     - vs102 s18: Glen Elendra Liege 1/3 -> 2 wither -> 0/1 SURVIVES. Printed "(both die)".
     - CORRECT case for contrast: vs135 s9 Ice-Fang Coatl 1/1 deathtouch -> dies; deathtouch kills Oona -> "(both die)" is right.
   Fix: in the "both die" computation, a wither blocker kills the attacker only if `blockerPower >=
   attackerToughness` (post-existing-damage) OR the blocker has deathtouch; otherwise emit "your
   blocker dies, attacker lives (wither shrinks it to X/Y)". This annotator misleads the model into
   thinking Oona trades UP (kills the attacker) when it only chips toughness. Repros above.

2. **[parser, CARRYOVER — not exercised] Extended loop-salvage (blocks/attack/put) had no positive
   test at this seat.** The two combat fallbacks were not salvageable-in-principle: vs49 s16 is a
   decode degeneration with no well-formed `ATTACK:` ordinal, vs62 s6 truncated mid-confusion before
   emitting a clean answer. The wave-19 out-of-range-index case (deck14 s9) did NOT recur. options_text
   is present in every combat record; #N duplicate-name disambiguation fired correctly (vs49 s19,
   two "Lord of Atlantis" + two "Zombie Outlander", all resolved). No parser defect found — recorded
   as a clean-but-unexercised validation.

## MODEL / decode items

3. **[model-obedience, WATCH — 2nd instance, SAME opponent, does NOT promote] Free favorable-block
   DECLINED.** vs49 s5: declined `(you kill it, your blocker lives)` on Bloodmark Mentor (their
   first-strike lord) — the exact wave-19 deck49 s9 misplay recurring on the same opponent/card,
   under a guide rule that names this precise case. Mixed within the game (took it at s9; correctly
   declined chump-with-lord at vs102 s21). This is model-obedience, not representation — the annotation
   is correct and the guide is explicit. Escalate to a skill rung only on a DIFFERENT deck. Interim:
   guide block rule stays as-is (no productive re-wording).

4. **[model-decode, WATCH — 2nd instance] Ahead-position degeneration LOOP.** vs49 s16 (ahead 10/11):
   the reply opened with a correct lethal-math plan, then degenerated into "No, it's a Spirit?"
   repeated to truncation -> the corpus's one attackers fallback. Distinct from over-reasoning: this
   is a repetition-decode failure (sampling/repetition-penalty layer), NOT guide-addressable. Pairs
   with wave-19 deck110 s20. Watch for a 3rd instance before proposing a decode rung.

5. **[model-decode, KNOWN] Behind-position losing-spiral persists in the loss.** vs62 s18 (behind
   5/16, lone option "Regenerate Sanguine Guard"): ignored the single option, rebuilt a whole attack
   plan, truncated. Unchanged behavior; handled at the guide off-case layer.

## DECK-CONSTRUCTION items (route to roster owner, not the guide)

6. **[construction, INFO] The vs102 and vs14 losses are matchup/draw, not pilot error.**
   - vs102: raced by Tourach, Dread Cantor (7/6, protection from black AND green AND white) — deck27
     literally cannot block or interact with it (no removal; its black creatures can't block a
     pro-black attacker). The evasion plan (Inkfathom every turn) RAN and simply lost the race. A
     guide cannot draw an answer the deck does not contain.
   - vs14: evasion DROUGHT — no Inkfathom / Deepchannel / active islandwalk drawn; the only evasive
     threat was a single Glen Elendra Liege (2/3 flyer) vs a walled Deep Blue ground. The deck's
     go-wide plan has no reliable breakthrough when its evasion is thin and the ground is walled.
     Flagging as a construction observation; the guide revision addresses only the PILOT half
     (under-attacking in the stall), not the structural half.

7. **[construction, INFO] Deepchannel Mentor (all blue creatures unblockable) STILL never drawn**
   across two corpora (1-of). Its ceiling remains under-observed. Not an action item; noted so a
   future roster pass knows.

## HARNESS items
None. -j level / latency not implicated (0 empty_reply, 0 timeout at this seat; the two adj losses
were life-adjudications, not latency timeouts). vs14 reached the turn cap by durdle, not by timeout.
