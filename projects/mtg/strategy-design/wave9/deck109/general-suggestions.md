# General-layer / engine / reply-protocol suggestions — deck109, wave 9

These route ABOVE the per-deck guide (reply-protocol layer, engine ledger, or core-prompt watch). Ranked
by leverage. All are self-contained for the project pause.

## 1. (Reply-protocol) A NON-NUMERIC reply head defaults to option 0 — worst-case at the attacker seam
**Repro:** corpus `matchups-20260714-094942`, file `1784040589-ai_baka_deck109-0x55c31fc62f20.jsonl`,
seq7, `kind=attackers`, T3, me 19 / opp 20. Reply head token = the literal string **`none`** (full reply:
`"none\nPLAN: Attack with Stromkirk Noble and Gore-House Chainwalker this turn..."`). The engine parsed
the non-numeric head to `choice=0` = `no attackers`. The PLAN wanted the attack; both creatures were
attack-eligible (not summoning-sick). Result: a skipped attack of 1+3=4 (non-outcome-changing; won T11).
**Why it matters:** at the attackers seam, option 0 is the passive/no-op option, so a garbled head silently
breaks the aggro deck's frozen spine. This is the wave-8 intent-collapse class (deck135) in a new shape —
a WORD head rather than a stale number.
**Fix (reply-protocol layer, not the guide):** when the head token is non-numeric, do NOT default to 0 —
either (a) scan the PLAN for the intended option, or (b) re-prompt for a bare number, or (c) at minimum log
it as a parse failure rather than silently selecting option 0. Feeds the standing reply-protocol A/B
(accept the LAST bare number / move CHOICE after the PLAN); add "reject/repair non-numeric heads" to its
scope. **Symmetric across all decks — do not write it into any single guide.**

## 2. (Engine ledger #3) Phyrexian Obliterator punisher-rider still OWED — verify at the deck133 seat
No opponent damage-punisher (Obliterator, or an opposing Boros Reckoner) entered any deck109 blocker/attack
window this corpus, so the broadened punisher rider could not be observed rendering OR being used at this
seat. The wave-8 gap (A2 showed only "Phyrexian Obliterator (5/5) [trample]" with no sacrifice-text rider)
remains unconfirmed-resolved from here. **The verification belongs at the deck133 seat** (the seat that
pilots Obliterator into opponents' blocker prompts) and in deck133's OPPONENTS' seat logs. Route: confirm
the broadened rider now renders the `auto=@damaged(this)... sacrifice` text on Obliterator when it is an
attacker, and that a pilot facing it declines to block. Until confirmed, the per-deck NEVER-block-Obliterator
line stays in every guide that could face it (deck109 keeps it).

## 3. (Representation — present-and-winning, informational) The blocker-seam lethal line (#5) is holding
The `Unblocked, these attackers deal up to N - you would be at M - NOT lethal / LETHAL` line fired in all 3
deck109 blocker windows and was OBEYED 3/3 (deck110 seq4 NOT-lethal -> no blockers; deck110 seq14 LETHAL ->
chump; deck131 seq8 NOT-lethal + exception (a) token-maker). The wave-8 "present-and-losing" wall
(read-and-disobey) did NOT reappear on the windows that occurred. No action — this is confirmation the
shipped representation, once the per-deck guide re-anchored to it (threshold-reuse), is now a working
present-and-winning surface at this seat. Keep watching at higher-volume blocker seats before any core
escalation of its wording (still the model-experiments frontier, not a core sentence).

## 4. (No core edit from this seat) Zero core-prompt candidates
deck109 produced no core-prompt candidate this wave. The one leak is reply-protocol; the blocker/Pillar
fixes are validated at the representation/per-deck layers. Consistent with 6 consecutive waves of core PASS
at this seat.
