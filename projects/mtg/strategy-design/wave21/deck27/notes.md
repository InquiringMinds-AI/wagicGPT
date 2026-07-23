# deck27 notes.md — engine/harness/model ledger + rotation verdict (wave-21)

Seat: Zombies (UB tribal), wave-20 surgical revision deployed. Binary /tmp/wagic-3be7f5a67.
Corpus matchups-20260723-084938. Record 5/6 (W vs102/14/49/62/135; L vs35). 135 decisions, 0 fallbacks.

## ROTATION VERDICT: DO NOT ROTATE — deployed guide revision still UNVALIDATED + a threshold-crossing decode item

Standing test (wave-20 form): rotate out only when the guide is stable-proven AND no new-work-signal.
Neither holds here:
- The deployed **late-stall durdle revision is UNVALIDATED** — its target off-case (walled ground +
  buffed attackers held) did not recur; vs14 (the grind) was won by an evasion draw and never entered
  the stall. A guide change is on the books but unexercised -> not "stable-proven."
- **New-work-signal PRESENT**: the ahead-position degeneration loop reached its **3rd instance**
  (vs135 s18), crossing the wave-20 hold-for-3rd threshold (a decode-ledger item, below).
Re-validate next corpus with FOCUS on a walled-ground grind (evasion drought). Recommend, if the
harness allows, biasing deck27's next opponents toward ground-wall decks, or accepting that this
revision may take several corpora to draw its trigger.

## ENGINE / representation ledger

1. **[annotation, item g — VALIDATED, CLOSE] Wither "(both die)" false positive: FIXED and confirmed.**
   All 3 Oona's Gatewarden pairings this corpus annotate correctly (full verbatim + math in
   findings.md, "OWNED VALIDATION"):
     - vs102 s6: Oona 2/1 vs Elvish Doomsayer 1/1 -> "(both die)" — CORRECT (2 >= 1).
     - vs49 s4:  Oona 2/1 vs Dragonmaster Outcast 1/1 -> "(both die)" — CORRECT (2 >= 1).
     - vs35 s20: Oona 2/1 vs six 9-10/7-8 tramplers -> "(your blocker dies, attacker lives, N tramples
       (wither shrinks it to X/Y))" — CORRECT: attacker lives (2 < 7/8), shrink 9/7->7/5 & 10/8->8/6,
       trample power-1. This is the strictly-harder case the wave-20 annotator would have mislabeled
       "(both die)" for all six. Zero false positives, zero wither-induced fallbacks (wave-20's vs62 s6
       fallback was directly caused by this defect). The item's headline harm is eliminated.
   No further action on item g.

2. **[parser, item b/c — POSITIVE VALIDATION] Answer-first truncation-safety proven at scale.**
   135/135 decision replies emit a coded first line; 0 fallbacks (wave-20: 3). Two giants that would
   have been wave-20 fallbacks parsed clean via line 1: vs102 s19 (16.9k, `ATTACK: A1`) and vs14 s2
   (13.9k, truncated mid-sentence, `CHOICE: 1`). The last-well-formed-CHOICE-wins rule (item c) was
   exercised benignly once — vs35 s18 emitted `CHOICE: 0 (pass)` then a final `CHOICE: 1 (Put in Hand
   … Vodalian)`; parser took the last (the model's improved play). No committing-before-reasoning
   degradation observed. Recorded as clean validation.

3. **[parser, CARRYOVER — still not exercised] Extended combat loop-salvage / prose-intent salvage.**
   0 prose salvages fired at this seat (nothing needed one — answer-first pre-empts the truncation
   that salvage would recover from). #N duplicate-name disambiguation not stress-tested this corpus
   (no duplicate-name combat menus arose). Neither validated nor refuted; noted.

## MODEL / decode ledger

4. **[model-decode, 3rd INSTANCE — threshold crossed, but harm DEFUSED] Ahead-position repetition
   degeneration loop.** vs135 s18 (priority, ahead 19/8, 13k): correct `CHOICE: 0 (pass)` on line 1,
   then the tail degenerates to "I have 4 lands. I can play one land." repeated to truncation. Prior
   instances: wave-19 deck110 s20, wave-20 deck27 vs49 s16 (that one caused a fallback). Now 3 total
   -> the pattern is a confirmed decode-layer failure (sampling/repetition-penalty), NOT
   guide-addressable. HOWEVER answer-first now neutralizes its harm (decision locked pre-degeneration;
   0 fallbacks), so the practical priority of adding a decode rung is LOW. Item j notes
   repetition_penalty is OFF this corpus; enabling it is the natural cheap fix and would likely
   suppress the loop. Recommendation: enable/tune repetition_penalty in a future corpus and re-check
   these ahead-position priority giants; do NOT spend guide prose on it.

5. **[model-obedience, WATCH — no recurrence] Free favorable-block decline.** Did not recur (only
   favorable block offered, vs14 s13, was taken; the Bloodmark Mentor vs49 trigger did not arise).
   Stays a 2nd-instance WATCH; promote to a skill rung only on a DIFFERENT deck.

## DECK-CONSTRUCTION items (route to roster owner)

6. **[construction, INFO] The vs35 loss (-45, t10) is opponent god-draw, not pilot error.** deck35
   (Slivers) assembled six 9-10 power tramplers by t10 (~56 trample). deck27 has no removal and no
   flyers big enough to wall a 9/7; correctly declined the hopeless Oona chump (s20). A structural
   race-loss the guide cannot address. Mirror-flip of wave-20 where deck27 beat 35 on an Inkfathom
   draw — this pairing is high-variance both ways.

7. **[construction, INFO] Deepchannel Mentor (all blue creatures unblockable) STILL never drawn**
   across three corpora. Ceiling remains under-observed. Not an action item.

8. **[construction, INFO] The three L->W flips (102/62/14) are all draw/opponent variance**, not new
   pilot capability — see findings.md attribution. deck27's win-path is evasion+protection; when it
   draws that path it wins, when the opponent draws an un-interactable threat (Tourach, Sliver
   god-hand) it loses. The deck has no B-plan. Roster note only.

## HARNESS items
None. 0 empty_reply, 0 timeout, 0 fallback at this seat; latency well within budget. vs14 resolved by
t15 (no turn-cap this corpus). No -j / latency implication.
