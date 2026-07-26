# deck136 (Yarok of Pranks) - findings.md
# Wave-29. FIRST corpus on the MV3-FIXED guide (wave-28 revision, deployed identical).
# Binary 50eeed8be. Corpus matchups-20260726-115018. All seq refs = deck136 seat.
# Record 5/6 (UP from 2/6 wave-28). MV3 fix = the validation target this wave.

## Record: 5/6
WINS:  vs137 (WIN clean, opp 0 life, 30-0 t15 - blowout, deck137 lost every leg),
       vs146 (WIN clean, 22 to -1, t10 - lethal kill),
       vs139 (WIN adj, 8-1, t14 - ahead at cap, Step-0 debut opp),
       vs148 (WIN adj, 12-6, t14 - ahead at cap, Step-0 debut opp),
       vs122 (WIN adj, 12-6, t18 - ahead at cap, Step-0 debut opp).
LOSS:  vs93 (LOST -6, opp 19, t12 - Relentless Rats, SAME loss as w27+w28).
Fallbacks at deck136 seat: 2 (seq17 priority retracted_choice vs93; seq39 priority
unparsed_reply vs122). Both benign, both mine - decomposed below. Parser/mana clean
otherwise; f3 sick-tag renders; f2 dual-land N/A (mono-B).

## JOB 1 - MV3-FIX VALIDATION (vs93 game): the fix WORKS at the decision layer; the
## Rats win was BLOCKED by a SEPARATE engine bug. Verdict: MV3 fix VALIDATED; verify-
## oracle incident arc CLOSED. The 5-turn impossible-Legion's-End-plan class is GONE.
The wave-28 exit criteria named two success conditions ("Ritual actually swept the Rats,
OR the pilot correctly benches Legion's End vs Rats"). The SECOND is met cleanly and the
Ritual-VALUATION half of the first is met; only the physical sweep was blocked (engine).

(a) LEGION'S-END-ON-RATS IMPOSSIBLE PLAN = GONE. In wave-28 the pilot chased "Legion's
    End the Rats" for FIVE turns and relapsed after self-correcting. This wave it NEVER
    once plans to Legion's End a Rat. It rejects Legion's End as useless vs MV3 Rats at
    EVERY decision it holds them (seq 4,5,7,8,9,10,12,13,15,16,17,18,19,21,22,23,25).
    Verbatim, the correct valuation is now stable:
      seq 8:  "two copies of Legion's End, but its text explicitly states it can only
               exile creatures with mana value 2 or less. Therefore, Legion's End cannot
               target the Rats."
      seq 21: "Relentless Rats is MV 3. Legion's End only works on MV 2 or less. My
               previous analysis was correct: Legion's End CANNOT kill Relentless Rats."
    The pilot no longer trusts a guide fact over the board - because the guide fact is now
    correct. This is the direct, verbatim removal of the poisoned class.

(b) RITUAL VALUED CORRECTLY AS THE ANSWER. The pilot repeatedly, correctly identifies
    Ritual of Soot as THE Rats answer and plans around it:
      seq 11 (reply): "Ritual of Soot destroys all creatures with mana value 3 or less.
               Since Relentless Rats has a mana value of 3, this spell will destroy it
               regardless of how many pump effects are active. This is the perfect answer
               to the specific threat they are building."
      seq 13: "Since they are MV 3, Legion's End cannot remove them. I will cast Ritual
               of Soot {2}{B}{B} to destroy all creatures with mana value 3 or less."
      seq 21: "You must rely on Ritual of Soot (which kills MV 3 creatures)."
    The guide's MV3 fix converted the pilot's understanding exactly as designed.

(c) WHY THE RATS GAME WAS STILL LOST - a NEW, SEPARATE engine bug (routed to notes.md as
    N-136a). At T5 the pilot cast Mausoleum Secrets (seq 10) and TUTORED Ritual of Soot
    (seq 11, kind=reveal, chose "Ritual of Soot", PLAN "Put Ritual of Soot into hand.
    Next turn ... cast Ritual of Soot"). But Ritual NEVER reached hand. Mausoleum Secrets'
    card-script gate is target(<1>*[black;manacost<=type:creature:mygraveyard]|reveal)
    - black card, MV <= CREATURE CARDS in graveyard. At T5 the pilot had 0 creatures in
    its graveyard (Knight still on battlefield), so the ONLY legal find was MV<=0 =
    nothing. Ritual is MV4. The reveal SEAM offered the ENTIRE 43-card library as PUT
    options (the prompt tail lists all 43, incl. Ugin/Liliana/Cavalier), NOT filtered by
    the option's target predicate. The pilot picked the illegal Ritual; the engine
    silently rejected it and returned it to library. stderr: "AIPlayerGPT: reveal put 1
    of 43 to option one in one reply" then no move-to-hand. The pilot spent the rest of
    the game believing it held its sweeper and hunting to re-find it, discovering only
    late (seq 16 "Ritual of Soot is NOT in my hand"; seq 19 "It seems you found Ritual of
    Soot but didn't take it? ... goes to library") that the tutor had failed. By then it
    was too far behind; it stabilized with Dread Presence (seq 20) and cast Liliana at 2
    life (seq 24) for a -4 (seq 25) that came a turn too late. Life 20->12->2->-6.
    THIS BUG IS NOT THE GUIDE'S. The guide steered correctly; an illegal-option seam
    defect blocked execution. It RECURS: same bug fired vs139 (seq16 tutored Rankle MV4,
    never cast) and vs148 (seq22 tutored Rankle MV4, never cast) - both games won anyway.

## JOB 2 - 2/6 -> 5/6 DECOMPOSITION (honest per-matchup, first-guided-attribution
## discipline). The jump is POOL SHIFT + VARIANCE, NOT proven MV3-guide efficacy.
Stable across waves: vs137 WIN (w27/w28/w29), vs93 LOSS (w27/w28/w29). The +3 net is:
  - POOL SHIFT (dominant): deck18 (6/6 Kithkin), deck22 (Giants), deck134 rotated OUT;
    three Step-0 guideless DEBUT decks rotated IN - deck122 (2/6), deck139 (2/6),
    deck148 (1/6), the three WEAKEST decks in the pool. deck136 beat all three, but ALL
    THREE by ADJUDICATION (ahead at the turn cap: vs139 8-1 t14, vs148 12-6 t14, vs122
    12-6 t18) - grind-to-cap wins vs weak unguided decks, not lethal conversions. In w28
    deck136's losses were to deck18/deck22 (both now gone); removing its two hardest
    opponents and adding three of the field's weakest is a pure roster gift of ~3 wins.
  - GENUINE FLIP: vs146 was an adj LOSS in w28 (8-16, 22-turn grind) -> a clean WIN this
    wave (22 to -1, t10). Real, but vs146's own guide + variance move this too; not
    attributable to deck136's guide.
  - THE MV3-FIX MATCHUP WAS LOST AGAIN. vs93 - the exact matchup the fix targets - is a
    LOSS for the THIRD straight wave. The guide fix improved the DECISIONS (Job 1) but a
    separate engine bug (N-136a) blocked the win. So NONE of the 5 wins is traceable to
    the MV3 guide fix; the fix's value showed up as decision quality, invisible in W-L.

## JOB 3 - 5/6-HIDES-TAXES AUDIT. The 5/6 record masks:
  1. The flagship guide-target matchup (vs93 Rats) is STILL a loss - and lost to a
     recurring engine bug the record cannot see (N-136a).
  2. 3 of 5 wins are ADJUDICATED grinds vs the pool's three weakest (Step-0) decks.
  3. N-136a fires in HALF the corpus (vs93, vs139, vs148) - three phantom-answer tutors;
     it only cost the game where the pilot was already under pressure (vs93).
  4. A transient MANA-MISCOUNT wobble (seq16 vs93): "I cannot cast Ritual of Soot yet.
     It costs {2}{B}{B} (four mana total, three of which must be black)" - miscounts the
     black pips as 3 (it is 2) while holding 4-5 black sources that trivially pay. It
     self-corrected in the same reply ("I have enough mana ... Why did I think I couldn't?")
     then landed on the truth (Ritual was never in hand). Benign, self-healed; noted.

## JOB 4 - WALKER/RANKLE SURFACES
  - RANKLE MAY-ASK WATCH -> RESOLVED (surfaced this corpus). vs122: Rankle connected and
    the "choose any number of: discard / lose1-draw / sacrifice" trigger DID reach the
    interactive-AI seat - as a SEQUENCE of per-mode opt-in asks: seq15 "Choose an option
    for Rankle: 1. sacrifice / 2. Decline" -> sacrifice; seq18 "1. discard / 2. Decline"
    -> discard (plus a life-and-draw mode ask between). The pilot picked the beneficial
    modes correctly across the sequence. The wave-28 WATCH ("may-ask may not reach the
    interactive seat") is DISPROVEN - it reaches the seat and the pilot navigates it. ONE
    side-effect: seq39 unparsed_reply (see Job 5) is the pilot bleeding Rankle-mode
    context into the NEXT priority window - a mild cost of the sequential decomposition.
  - LILIANA -4 -> NOW EXERCISED (wave-28 carry-forward RESOLVED). vs93 seq25 activated
    "-4: each player sacrifices two creatures" - render clean, correct anti-wide pick vs
    4 Rats (though cast at 2 life in a lost cause - too late, not a mispick). Both Liliana
    modes are now confirmed live (+1 w28, -4 w29).
  - UGIN -> STILL NEVER CAST (across all 6 games). Fully unexercised for a 3rd wave. Guide
    nudge already present. Carry forward.

## JOB 5 - f-BATCH / FALLBACK DECOMPOSITION (both fallbacks are mine)
  - f3 SICK-TAG: renders correctly at this seat, e.g. "Soul Warden {w} (1/1) [summoning
    sick - cannot attack this turn]" (vs122 seq5). Confirmed.
  - f2 DUAL-LAND: N/A - mono-B seat, every land taps {B}, no color reach to verify.
  - LAND TAGS: clean; all mulligans "Keep this hand"; no color confusion.
  - FALLBACK 1 (seq17 vs93, priority T7 MP2, retracted_choice): pilot chose "0 (pass)"
    (declining a Castle Locthwain activation); the parsed choice was RETRACTED as stale
    (async answer arrived after the priority window advanced) -> heuristic answered
    (also a pass). This is the ~2% self-healing stale-drop; the pilot WANTED to pass, so
    zero play cost. Benign.
  - FALLBACK 2 (seq39 vs122, priority T12, unparsed_reply): the pilot answered a PRIORITY
    window with a RANKLE-MODE answer - CHOICE: 2 (Choose "discard") - hallucinating it was
    still inside a Rankle mode sequence ("The prompt presents a sequence of Rankle's
    triggered abilities ... the second is life and draw ... the third is discard"). The
    actual prompt offered pass / Castle Locthwain. No legal option matched -> unparsed_reply
    -> Baka answered. Root cause = Rankle's sequential per-mode asks (Job 4) bleeding
    context into the immediately-following priority window. Low severity (Baka answered,
    game won), but a real cost of the sequential-mode design -> notes.md.

## JOB 6 - ROTATION PROJECTION: NOT near-terminal; KEEP. Not a wave-30 candidate.
The brief's terminal test is "MV3 fix validated AND no new work." The first half is met
(Job 1). The second is NOT: this wave surfaced (i) N-136a, a recurring engine seam bug
that actively costs the Rats game and needs a fix + re-validation wave; (ii) a real,
verify-oracle-grounded GUIDE modification this wave (Mausoleum Secrets constraint
sharpening - the pilot over-valued Secrets as a bomb-tutor and lost tempo). The Rats
matchup is STILL unwon (blocked by N-136a). Projected exit: after a wave where N-136a is
fixed AND the pilot's now-correct Ritual valuation actually converts (Ritual reaches hand
and sweeps the Rats, or the pilot correctly declines to over-tutor and draws Ritual
naturally to a win) AND Ugin either fires or is ruled seat-structural. One more validation
wave minimum.

## Layer routing summary
- Engine: N-136a (Mausoleum Secrets / reveal seam offers illegal PUT options - the whole
  library instead of only cards matching the option's target predicate) -> notes.md, with
  repro + 3-game recurrence. seq39-style priority/Rankle-mode context bleed -> notes.md
  light watch.
- Guide: Mausoleum Secrets constraint sharpened (dead-creature gate concrete, bombs it
  cannot reach named, "do not over-tutor for Ritual early" in the Rats matchup) ->
  strategy.txt. Verify-oracle grounded in borderline.txt auto=.
- Skill: MV3-fix VALIDATED-at-decision-but-blocked-at-execution = a decision-quality-vs-
  record decoupling lesson; the reveal-seam illegal-option class as a guide-can't-fix-it
  boundary -> skill.md.
- Core: no core-prompt defect at this seat -> PASS #24 (general-suggestions.md).
