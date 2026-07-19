# deck27 notes.md — dev notes + rotation verdict (wave-19)

Seat: Zombies (UB tribal), GUIDELESS FIRST PASS. Binary /tmp/wagic-72b05535d.
Corpus matchups-20260719-092058. Record 2/6 (W vs110, vs135; L vs14, 49, 35, 62).

## ROTATION VERDICT: DO NOT ROTATE — new-work-signal PRESENT, initial guide WRITTEN

This is a guideless newcomer; wave-19 produced its FIRST guide (this deck27/strategy.txt). The
corpus surfaced concrete, guide-addressable failures (durdle/late-lords, declined free block,
recursion misallocation, evasion under-use). deck27 must run its first GUIDED corpus in wave-20 to
validate the taught classes (Step 0-ter). Not a rotation candidate.

## ENGINE / parser ledger items (layer-routed here, not into prompt text)

1. **[parser, LOW-MED] Bundled-block: salvage valid pairings when one pairing has an out-of-range
   attacker index.** deck14 s9: prompt offered only A1; model answered "BLOCKS: B1:A2" (A2 does not
   exist). Whole reply discarded -> unparsed_reply fallback. A mixed reply like "BLOCKS: B1:A1, B2:A2"
   with a valid B1:A1 would ALSO lose the good pairing under whole-reply discard. Proposal: drop the
   illegal-index pairing, keep the rest; if nothing valid remains, then fall back. Note: here nothing
   valid remained (the model hallucinated the entire premise), so this specific game would still fall
   back — the fix is for the mixed-reply case. Repro: deck14 s9.

2. **[annotation, VERIFY-PASS] TRADE OUTCOME annotations correct at this seat.** No defect. Sampled
   block prompts printed "(both die)", "(you kill it, your blocker lives)", "(your blocker dies,
   attacker lives)" and all matched the board. The wave-19 annotation batch is clean for UB combat
   (first-strike math handled: 2/3 blocker vs 3/1 first-striker correctly shown as blocker-dies).
   No action; recorded as a positive validation for the synthesis ledger.

3. **[name-tolerance, VERIFY-PASS] Combat name tolerance did not mis-fire.** The seat's only block
   fallback was a model hallucination (item 1), not a name/label parse failure. The wave-19
   name-tolerance second pass is not implicated here.

## MODEL / decode items

4. **[model-obedience, WATCH] Correct favorable-block annotation DECLINED.** deck49 s9: the model's
   own reply reasoned "block the 1/1 with my 2/2, I trade up (I kill it, I live)" then chose NO
   blockers, letting their team-wide first-strike lord (Bloodmark Mentor) live and taking 3. The
   representation was correct and the model verbalized the right read — then disobeyed it. This is a
   model-behavior gap, not a representation gap. Interim: guide block rule. Watch for a second seat
   before escalating (skill.md).

5. **[model-decode, KNOWN] Behind-position giant-reply spiral.** 5 of 6 giants in behind states; 3
   became unparsed. Unchanged from prior waves; handled at the guide off-case layer. deck35 s12
   ("I have 2 life... but the game forces me to choose") is the canonical instance.

6. **[model-decode, WATCH] Ahead-position over-deliberation giant.** deck110 s20 (won, ahead L14/2):
   15.4k ramble on a trivial Main-2 cast over hypothetical opponent removal. Rare (1 of 6 giants).
   Interim: guide "when ahead keep it simple" line. Watch whether it recurs or is noise.

## DECK-CONSTRUCTION items (route to roster owner, not the guide)

7. **[construction, INFO] The deck has NO removal and NO burn — pure creature combat.** Every loss
   was a creature-combat loss (raced by flyers vs49/vs14, swarmed vs35, out-ground vs62). A guide
   cannot draw an answer the deck does not contain. The deck's counterplay to go-wide/flyers is
   walls (Oona's Gatewarden) + regeneration + evasive racing; it is inherently soft to a faster or
   wider aggro deck. If the roster wants deck27 more competitive, that is a CONSTRUCTION lever
   (e.g. an interactive card), not a prompt lever. Flagging, not requesting.

8. **[construction, INFO] Deepchannel Mentor (makes all blue creatures unblockable) is a 1-of and
   was never drawn this corpus.** Its ceiling (whole blue team unblockable) is far above the deck's
   average card. Not an action item — noted so a future roster pass knows the card's impact is
   under-observed.

## HARNESS items
None. -j level / latency not implicated; 0 empty_reply, 0 timeout at this seat.
