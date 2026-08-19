# deck116 — development notes (engine / harness / render / instruments)

Layer-routed per the brief. Nothing here is guide prose. Binary `bdb30d8e7`, corpus
`matchups-20260819-070739`.

---

## N-116g (ENGINE, **HIGH**, NEW, GAME-DECIDING) — the cheat-into-play activation is CONSUMED, NARRATED and PAID FOR, and then never reaches the stack

**Class:** silent no-op on an activated ability at a priority window. **This is a REGRESSION** —
wave-32 arrival-traced this exact class 5/5 clean on binary `7fabd9bd0`.

**Counts:** 13 `Put in Play` windows offered, 11 taken by the pilot, **8 arrived, 3 did not (27%)**.

### Repros

**(1) `1787146461-ai_baka_deck116-...-vs-ai_baka_deck105.jsonl` seq 11**, my turn 10 Upkeep,
kind=`priority`, options=1, `Mana available: 4 total`, cost `{4}, Tap`.
Chosen: `Put in Play with Quicksilver Amulet targeting Emrakul, the Aeons Torn`.
The seq-11 events narrate the action. Then, at **seq 12**:
```
Mana available: 4 total (four untapped sources, ...)
Your hand: ... Emrakul, the Aeons Torn {15} (15/15) [creature]; ...
Your battlefield (creatures: 0): Forest #1; Forest #2; Hinterland Harbor; Glimmerpost;
    Quicksilver Amulet {4} [artifact]
```
Emrakul still in hand, board still `creatures: 0`, the Amulet **not** `[tapped]`, mana **not**
spent. No `- Your Emrakul, the Aeons Torn: hand -> battlefield` line exists anywhere in the game.

**(2) same log, seq 15**, opponent's turn 11 Upkeep, options=2, `Mana available: 5 total`. Same
activation re-taken, same non-arrival. Game ends on the next record: **loss at 23 life to poison**,
holding a 15/15 annihilator-6 with the enabler online.

**(3) `1787150652-ai_baka_deck116-...-vs-ai_baka_deck36.jsonl` seq 22**, opponent's turn 13 Upkeep,
options=1, `Mana available: 1 total`, cost `{g}, Tap`, target Pelakka Wurm. Non-arrival: seq 23
shows Pelakka Wurm still in hand and `Elvish Piper {3}{g} (1/1)` **untapped**. The pilot re-took
the identical activation at **seq 23** (my turn 14 Upkeep) and that one ARRIVED (seq 24:
`- Your Pelakka Wurm: hand -> battlefield` / `- Your life +7 (now 25)`).

### Independent cross-check (opponent seat)
`1787146461-ai_baka_deck105-...-vs-ai_baka_deck116.jsonl` — a grep of its whole event stream for
`Emrakul` returns only the library reveal lines; `Opponent's Emrakul, the Aeons Torn: hand ->
battlefield` never occurs. The Amulet's own arrival IS there
(`Opponent's Quicksilver Amulet: hand -> stack` / `stack -> battlefield`), so the stream is not
lossy — the event simply did not happen.

### Engine trace — the isolation is exact
`matchups-20260819-070739/game-105v116-1787146458.stderr`, the seq-11 failure:
```
Untap Phase -------------   Turn 9
AIPlayerGPT: model chose 1 of 1
AIPlayerGPT[ph2]: take action 1/1 (cached=0)
 Ai knows exactly what mana to use for this ability.
AIPlayer:Using Activated ability
AIPlayerBaka: AI attempting to pay a mana cost.
-  Target: Quicksilver Amulet
-  Cost: {4}
AIPlayerBaka: Ai had a payment in mind.
Action added to stack: NextGamePhase.  (Current phase is: Upkeep)     <-- NO StackAbility add
Resolving Action on stack: NextGamePhase.  (Current phase is: Upkeep)
```
`game-36v116-1787150652.stderr`, the seq-22 failure (lines 1162-1172), identical shape:
```
AIPlayerGPT: model chose 0 of 2            <-- Untap-phase priority, passed
AIPlayerGPT[ph2]: pass priority (cached=0)
Action added to stack: NextGamePhase.  (Current phase is: Upkeep)
AIPlayerGPT: model chose 1 of 1            <-- Upkeep priority, activation taken
AIPlayerGPT[ph2]: take action 1/1 (cached=0)
AIPlayer:Using Activated ability
AIPlayerBaka: AI attempting to pay a mana cost.
-  Target: Elvish Piper
-  Cost: {g}
AIPlayerBaka: the Mana was already in the manapool or could be Paid with potential mana, ...
Resolving Action on stack: NextGamePhase.  (Current phase is: Upkeep)   <-- NO StackAbility add
```
Every SUCCESS carries the missing line immediately after the payment, e.g. the same file's
lines 1236-1246 (seq 23, my own turn):
```
Untap Phase -------------   Turn 13
AIPlayerGPT: model chose 1 of 1
 Ai knows exactly what mana to use for this ability.
AIPlayer:Using Activated ability
-  Cost: {g}
AIPlayerBaka: Ai had a payment in mind.
Action added to stack: StackAbility.  (Source: Elvish Piper)
Resolving Action on stack: StackAbility.  (Source: Elvish Piper)
```
**So: `MTGAbility::reactToClick`/`addToGame` succeeded through payment and then failed to push the
`StackAbility`. The failure signature to grep for, corpus-wide, is a `Target: <card>` / `Cost:` /
payment line that is NOT followed by `Action added to stack: StackAbility.  (Source: <same card>)`.**

### Observed correlate (stated as a correlate, not a cause)
The activation is dropped only at **Upkeep priority windows**; all three failures and no
main-phase window. Within Upkeep: opponent's-turn upkeep **0/2 arrived**, my-turn upkeep
**5/6 arrived**. Mana headroom is NOT the discriminator (a success at `Mana available: 4` with a
`{4}` cost exists at vs152 s11, and a failure at the same 4/{4} exists at vs105 s11). Both failures
whose stderr I read have a `NextGamePhase (Current phase is: Upkeep)` action pending on the stack
across the activation. Candidate interim commits to bisect against: `e4b3cc96e` (ORDER-step click
gate), `cd2983c2a` / `80d2ac217` (activation offer/payment-candidate changes),
`a46095a08`/`4dab86746` (cancelled-attacker guards).

**Fixture requirement (HL5 rung 4):** the regression fixture must ride the BROKEN branch — an
Upkeep-phase priority activation of `{4}{T}:moveTo(myBattlefield) target(creature|myhand)` with a
phase-advance pending, on **both** players' turns. A main-phase fixture passes today and proves
nothing.

**Severity: the highest at this seat.** It cost the vs105 game outright and a turn in vs36. The
offer side is correct, the model's decision is correct, the payment is correct, and the deck's
entire thesis silently fails to execute. No fallback, no defer, no counter records anything —
this is arrival-tracing catching what every instrument misses.

---

## N-116h (HARNESS/PROMPT, MEDIUM) — the PLAN field is treated as authoritative over the board snapshot in the same prompt

Downstream of N-116g, and separable from it. After the seq-11 no-op, vs105 seq 13 replies:

> *"The game state indicates that Emrakul, the Aeons Torn has already been put onto the
> battlefield via the Quicksilver Amulet activation during the Upkeep phase of Turn 10
> (**'Your Plan' confirms this sequence occurred**). Consequently, the primary objective of the
> turn -- cheating in the win condition -- is already complete."*

The same prompt contains `Your battlefield (creatures: 0)` and `Your hand: ... Emrakul, the Aeons
Torn {15} (15/15) [creature]`, and it also contains the engine's own staleness warning
(`note: the actions your plan names are no longer among the options available right now - the game
state has advanced past that plan; re-derive your choice from the current board`). Two decisions
(seq 13, seq 14) were spent on this false belief. This is the wave-32 HL10 plan-field-pollution
class at a new seat, with the aggravating detail that the **plan text outranked BOTH a correct
board render and an explicit engine warning attached to it**. Fix direction (render lane): when the
staleness note fires, the plan block is already known-stale — consider suppressing the plan text
itself rather than annotating it.

---

## N-116i (RENDER, LOW) — the `colours you can make` field uses TWO different colourless tokens, sometimes in the same line

Distribution over all deck116 prompts (126 `Mana available:` lines):
```
{g}{u}{c}  61      {g}{u}  56      {g}  12      {1}{g}{u}   9
{1}{g}{u}{c}  6    {u}  5          {g}{c}  4    {c}  3       {1}{g}  2   {u}{c}  1
```
`{1}` and `{c}` both appear for colourless, and **6 lines print BOTH** (`{1}{g}{u}{c}`). The
one measured consequence is at `vs152 s24`: `colours you can make: {1}{g}` produced
*"I cannot cast ... Kitchen Finks ({1}{G/W}{G/W} = 3 mana total, but I only have 2 green sources and
1 colorless source, so I cannot pay the hybrid costs correctly without a white source"* — a false
unpayability claim about a card in its own offer list. (The reply then self-corrected and cast it;
see findings (5).) Fix: emit `{c}` uniformly, never `{1}`, in a COLOUR list.

---

## N-116j (RENDER, LOW) — the poison aggregate line is emitted BEFORE the per-source lines that cause it, and the per-source lines show no increment

vs105 seq 11 events:
```
- Poison: you take 4 poison counters - now 8 of 10 (...; 2 more end it)
- Infect damage: 1 from Ichorclaw Myr to you - dealt as POISON COUNTERS, ...
- Poison: you now have 8 of 10 (...)
- Infect damage: 1 from Plague Stinger to you - ...
- Poison: you now have 8 of 10 (...)
- Infect damage: 2 from Ichor Rats to you - ...
```
The aggregate (+4) fires first with the final total, then each source is narrated with the same
already-final total. A reader reconstructing the combat cannot attribute counters to sources. This
is cosmetic — i1's decision-relevant job (the total and the threshold) is done correctly and the
pilot read it correctly every time — but it is a per-event ordering nit on a brand-new emitter and
is cheapest to fix now. **Do not let this dilute the i1 verdict: i1 VALIDATED, 10/10 at my seat.**

---

## N-116a (KNOWN-OPEN, routed) — whole-library tutor reveals: COUNTED, not re-diagnosed

Per the brief. **9 reveals; 439 options rendered for 40 eligible cards (11.0 : 1).** Per-window
options / eligible: Fabricate 50/3, 49/4, 50/4; Time of Need 46/5, 51/5, 51/5, 47/4, 46/5, 49/5.
Prompt sizes 12,535-24,984 chars.

**New datum worth carrying to the fix's cost/benefit:** the reveal REPLY tax is no longer the
seat's problem. All nine replies are 622-1,054 chars at 11-17 s (wave-32: up to 13,796 chars /
220.6 s). The remaining cost is entirely PROMPT-side, so the fix is now a context-budget item
rather than a latency item — which changes where it sits against the cap-adjudication work.

## N-116d (KNOWN-OPEN, routed) — subset framing: **0 occurrences, UNEXERCISED**

Zero un-tokened (`no <N>`) reveals occurred. The only such effect in this deck is Farseek, and the
pilot cast Farseek **0 times in 36 offered windows** because the guide demoted it. The defect is
untested this corpus, not fixed. Any wave-34 validation of the subset-framing fix must NOT use
deck116 as its witness while this guide is deployed.

---

## Steady-state / known-open items, checked and reported

- **N-152g (legend suppression):** NOT EXERCISED. Every legend in deck116 is a 1-of; no window in
  six games offered a legend cast while a same-name copy was in play. 0 suppressed windows.
- **N-158e (flash windows):** confirmed unexercisable at this seat — the only flash card is
  Jin-Gitaxias {8}{U}{U} and it never became payable. No hold-for-flash teach exists in the guide.
- **i7 (`(printed X/Y)` on the board snapshot):** **0 false tags.** A regex over all 131 prompts for
  `(P/T) (printed P/T)` with a zero delta returns nothing; the only occurrences carry a real delta
  (`Kitchen Finks (2/1) (printed 3/2) [persist] [counters: 1x -1/-1]`) and the pilot read it
  correctly at vs146 s13. Negative witness for N-152d layer 2 at a second seat.
- **i8 (combat tags) and N-152j/N-158n (`BLOCKS:` parsing):** 9 blocker windows, 9 parsed, 0
  `unparsed_reply`, 0 `all_assignments_illegal`. Both reply forms appear and both parse:
  index (`BLOCKS: B1:A1, B2:A2`) and the engine's name form in `chosen_text`. The
  `[summoning sick - cannot attack this turn, but CAN block]` tag renders and is read correctly
  (vs146 s12 declines an attack having correctly excluded the sick Piper from its eligible set).
- **i3, i4, i5, i6, i10:** not exercised at this seat (no multicolour targeting, no all-flexible
  manabase, no re-asks, no amass, no attachments).
- **`all_assignments_illegal`:** 0 occurrences. Closest shape is `vs36 s17`, where the reply wrote
  `BLOCKS: B1:A1, B1:A2` — one blocker assigned to two attackers. The engine took the first legal
  assignment (`chosen_text: Emrakul, the Aeons Torn blocks Silver Myr`) and dropped the second
  rather than failing the set. Recorded as a lenience datum, not a defect: the outcome was the
  correct block.

---

## Instrument notes for the counter work

- **`pruned_pairs` is never emitted at this seat.** Record keys across all 140 records:
  `commit_retracted` 131, `post_plan_overrun` 131, and no `pruned_pairs` key at all. If the field is
  meant to be always-present, that is an instrument gap; if it is conditional, say so in the spec so
  seats do not report its absence as a zero.
- **`post_plan_overrun` measures post-PLAN, and the waste at this seat is post-ANSWER.** Seat totals:
  post-ANSWER 263,247 chars vs post-PLAN 14,025 chars — the field sees **5.3%**. It is also format-
  sensitive: `vs152 s3` writes its plan line as lowercase `Plan:` and scores 262 against 14,206
  actual post-answer chars; `vs139 s3` omits the PLAN line entirely (legal under `f46dd58ee`) and
  scores 0 against 13,326. **Proposed instrument change (measurement only, no behaviour): add
  `post_answer_overrun` = chars after the first committed answer line, alongside the existing
  field.** Without it the wave-34 A/B cannot measure what the protocol change is aimed at.
- **A late second `CHOICE:` line is HONOURED, not retracted.** `vs152 s24` emits
  `CHOICE: 4 (Cast nothing right now)` on line 1 and `CHOICE: 3 (Cast Kitchen Finks ...)` near the
  end; the record shows `choice: 3`, `commit_retracted: false`. Worth stating explicitly in the
  protocol docket: today the model CAN re-commit and the parser accepts it, which is why the
  "self-correction has value" question is not hypothetical — it is just unreliable (1 of 5 divergent
  replies at this seat actually emitted the second line).
