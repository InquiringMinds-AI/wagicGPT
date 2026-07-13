# Deck-135 wave-5 notes — platform, harness, deck-construction

Citations: run matchups-20260713-145947, files `<epoch>-ai_baka_deck135-*.jsonl`.

## FOR THE ENGINE OWNER

### E1. Fetch re-offer loop (wave-3 F2 / wave-4 E1) — PERSISTS, and it is the top inference-budget drain
A chosen fetch/crack is re-offered and re-answered across subsequent phases before it
executes. Fresh repros:
- vs133 `1783979342`: Prismatic Vista seq2→3→4 (Main1→Main2→Upkeep, 3 calls for one
  crack); Flooded Strand seq10→11→12→13→15 — **5 calls**, including seq11 and seq12
  offered DURING the Attackers step; Misty Rainforest seq20.
- vs131 `1783977943`: pervasive; contributes directly to the timeout (105 decisions to
  reach only T16, final 9v23). The life payment / land arrival lands only on the LAST
  answer of each run, so earlier answers did not execute.
Same suggested fix as prior waves: trace the crack's click choreography at the first
offer, and/or suppress a re-ask when an identical option set was answered within the
same turn and only the phase changed.

### E2. Turn-ownership / seam stamping (wave-4 E3) — still worth doing
Not a big offender this corpus (the "it's the opponent's turn" delusion was rare), but
the phase line still sits far above the option list. Cheap: stamp ownership on the
choice-list header ("Your legal actions (YOUR Main Phase 1):"). One narration change.

### E3. Translog schema — two residual asks unchanged from wave-4 E6
- `options` is still a count; the actual option STRINGS would make offered-vs-taken
  tallies trivial and kill the option-line regex work (I still had to re-parse prompts).
- A `reask_of: <seq>` marker when the same activation repeats would make E1 measurable
  without regexing prompts.

## RESOLVED / DOWNGRADE CANDIDATES (verify before deleting from the ledger)
- **Plan-echo splice (wave-4 E4): appears FIXED.** 0 replies opened with a stray
  fragment of the previous plan across vs131/vs140/vs110 (scanned). If confirmed on the
  full corpus, close E4.
- **Reply parser desync: clean.** 0 head-integer-vs-choice mismatches across all 6
  games — the head-first parser is holding.
- **Degenerate blocker reply / illegal assignment (wave-4 model residuals): not observed
  this corpus.** Keep the canned litmus probes; selfplay didn't reproduce them.

## FOR THE DECK BUILDER (user)
### D1. The manabase is the standing tax — flag persists from wave 4
Mainboard runs exactly **2 Snow-Covered Islands** and **2 Snow-Covered Plains** while
{U} touches Coatl/Treefolk/Search/Force and Diamond Faerie needs {W} AND {U} on top of
{G}. This wave it bit as: Diamond Faerie stuck uncastable in hand for the whole back
half of vs110 (no white). Arcum's Astrolabe fixes colors but costs a mana to use
({1}{T}: add W/U/B/R), so it is a slower fixer than its reputation. A cheaper structural
fix than any prompt text: a 3rd Snow-Covered Island (over a 2nd Plains — white is only
Gelid Shackles {W} plus half of Diamond Faerie) would defuse both the wave-4 Island-hunt
and this wave's uncastable-Faerie. This is the user's call — flagging, not prescribing.

### D2. Diamond Faerie is arguably mis-slotted for the GPT pilot
A {2}{G}{W}{U} 3/3 with a TEMPORARY (UEOT) pump is hard for the pilot to cast and hard
for it to use well (the pump is a combat trick, not an anthem). If the deck is being
tuned for the GPT track, a cheaper on-color threat in that slot would deploy more
reliably. Recorded as an option, not a recommendation.

### D3. Sideboard is dead weight in selfplay (unchanged from wave 4)
`#SB:` cards never appear in any translog — no sideboarding step exists. Main-deck slots
are the only real estate that matters for the GPT track.

## FOR THE WAVE COORDINATOR
- **The over-correction audit found NO overshoot this wave for deck135** — the attack
  floor is obeyed and healthy. The lesson for the loop: an audit that only hunts for
  overshoot-of-the-loud-rule will miss a leak that DISPLACED to the adjacent seam
  (attack seam fixed → cast seam is now the leak). See skill.md P1.
- **Outcome accounting:** the vs131 timeout is a behind-on-life durdle (9v23) — under
  life-adjudication it is a loss, making deck135's real record 1W-5L this run. deck131's
  Elixir/Prism Ring lifegain and the fetch re-offer loop both inflate its games' length.
