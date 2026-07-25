# deck131 wave-25 — dev notes (engine / harness / model / deck-construction; self-contained).

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-035605/`, 6 deck131 games, **1W/5L**.
Binary /tmp/wagic-62d8783a5 (RULES-FIX f0500ad66 + wave-25 step-1 batch, -T 3000). Pilot: rep_penalty=1.05.
Live guide = wave-24 REVISED (10910 bytes, deployed==wave24 byte-verified via cmp exit 0). This wave: TWO
guide edits (bottom-step recalibration + never-block lethal-only-blocker exception) -> 12161 bytes.
deck131 is deck0 vs 137/59, deck1 vs 109/102/27/133 (see results.tsv). Zero fallbacks at this seat.

Seat->game map (results.tsv): vs137 adj0 WIN 6/2 T12 | vs109 L 0/10 T8 | vs59 L -8/24 T15 |
vs102 L -1/1 T14 | vs27 L -1/14 T14 | vs133 L -2/20 T14.

Layer-routing: engine/harness/model items live HERE, never in the guide. Deck-construction flags are
RESTATED for the user, not relitigated.

## Reproducible engine/harness/model ledger

### I-1 — [MODEL-COMPREHENSION, WATCH, guide-unfixable] tapped-read misread RECURRED, round 3 (vs137 s26)
- vs137 s26 (attackers, T12): reply `ATTACK: A1, A2` (both Guttersnipe) claiming *"the opponent has no
  untapped creatures to block them (all their attackers are tapped from last turn)."*
- The s26 prompt opponent battlefield: `Flaxen Intruder (3/4) [tapped]; Edgewall Innkeeper (3/3) [tapped];
  Trostani Discordant {3}{g}{w} (1/4); Soldier (2/2); Soldier (2/2)`. **Trostani + both Soldiers carry NO
  [tapped] tag — they were UNTAPPED and could block.** The model reasoned from "the ATTACKERS are tapped"
  to "nothing can block," IGNORING three freshly-deployed non-attacking untapped blockers. SAME class as
  wave-23 I-1 (vs27 Lord of the Undead) and wave-24 I-1 (vs59 Restless Apparition); now 3 corpora at THIS
  seat. NEW sub-shape: the false generalization is "attackers-tapped => all-tapped," not just "one-tapped
  => all-tapped."
- **NON-DECISIVE:** s26 was the T12 adjudication-cap turn (opp 2, 131 6). The swing is EV-robust
  regardless of the read — unblocked = immediate win; blocked = Guttersnipe trades but 131 still wins the
  6-2 life adjudication that ends the game that turn. The ACTION was fine; only the STATED REASON rested on
  a false board read. Rung-2 (decision-input) misread.
- **The per-creature [tapped]-read guard held 8/9 this corpus** (see Interface validations) and the prose
  already covers a just-created untapped token; shouting louder is the sanctioned-against move -> guide
  UNTOUCHED on the guard. Cross-seat WATCH: tapped-read misread is now confirmed at 3 corpora at deck131
  ONLY; promote to a cross-seat combat-safety item ONLY if a 2nd SEAT shows it. Repro: for
  kind=='attackers' with Guttersnipe among options, diff the reply's claimed tapped-set vs the prompt's
  opponent-battlefield [tapped] tags; a reply that says "all/no untapped" while a creature lacks [tapped]
  is this class. See I-6 for a below-bar core-prompt mitigation candidate (affirmative untapped tagging).

### I-2 — [GUIDE-DEFECT via CORRECT DEVIATION, FIXED] the "never bottom a draw spell" absolute was Magic-wrong
- vs137 s3 (bottom, T0): model bottomed **Opportunity** ({4}{u}{u}) from a loaded mull-to-6 keep (2x
  Guttersnipe + YP + cheap Artificer's Epiphany + 2 lands). CORRECT bottom (over-costed 6-drop, cheaper dig
  kept). vs133 s3 (bottom, T0): model bottomed the **duplicate Artificer's Epiphany** from a keep holding
  TWO copies. CORRECT bottom (keep one dig, bottom the redundant).
- BOTH violate the wave-24 guide's literal *"never bottom ... a draw spell (Artificer's Epiphany/
  Opportunity)."* The model correctly IGNORED the absolute both times. Neither hand held Prism Ring/Elixir,
  so the "bottom Prism Ring FIRST" piece (what the line was written to fix) went UNTESTED; the part that
  WAS exercised was falsified 2/2. -> guide FIXED this wave (impact/redundancy ranking; over-costed and
  duplicate draws are valid bottoms; only "never bottom your ONLY draw/payoff/red-source"). Rules-
  correctness fix, NOT louder. Repro: for kind=='bottom', check that the bottomed card is genuinely the
  lowest-impact of the hand (Prism/Elixir > surplus land > over-costed-with-cheaper-in-hand > duplicate),
  and that no ONLY-copy of a payoff/dig/red-source was bottomed.

### I-3 — [GUIDE-DEFECT via CORRECT DEVIATION, FIXED] the "never block with Guttersnipe" absolute was wrong when it is the ONLY blocker facing lethal
- vs59 s27 (blockers, T13): 131 at 7 facing 8 attacking power (Safehold 1 + Restless Apparition 2 +
  Lingering Tormentor 2[fear] + Kitchen Finks 3), battlefield creatures: ONLY Guttersnipe. Model reply
  `BLOCKS: Guttersnipe: Kitchen Finks` with correct math: fear blocks Tormentor unblockable, blocking the
  biggest (Finks 3) with the lone Guttersnipe drops incoming to 5 -> life 2, survives; not blocking = dead
  (8>=7). This is CORRECT forced-lethal-only-blocker play, but the wave-24 guide said *"it stays alive in
  EVERY case... if you are truly about to die, chump with an Elemental or Young Pyromancer, never
  Guttersnipe"* — a rule that assumes a chump exists; here none did. -> guide FIXED this wave (added the
  lethal-AND-only-blocker exception). NOT the wave-24 I-5 non-lethal-block slip (that was life 1, chumps
  available); this is genuinely forced. Repro: for kind=='blockers' where the model blocks with
  Guttersnipe, confirm (incoming power >= my_life) AND (Guttersnipe is the only creature I control) before
  scoring it a discipline slip — if both hold, it is CORRECT.

### I-4 — [RULES-FIX a1, CLEAN] upkeep priority now granted; deck131 has no upkeep triggers
- 7 upkeep-phase priority records (turns 5-13): model mostly `pass`, twice activated Elixir of Immortality
  at upkeep for 5 life (vs109 s14 T7). deck131 runs ZERO "at the beginning of your upkeep" cards, so the a1
  change surfaces NO earlier-firing trigger at this seat — only the model correctly handling new upkeep
  priority. No narration confusion, no expectation of the removed pw-redirect menu (a4). CLOSED-clean.

### I-5 — [RULES-FIX a2, CLEAN] no untap-window casting
- Confirmed NO in-game untap-phase priority/cast/activation this corpus (every cast/activation is at a main
  phase or a legitimate response window). The seat's historical untap-window activations (old fixtures) do
  NOT recur under a2 (untap grants no priority). Behavior change confirmed correct. ("Phase: Untap" strings
  in prompts are pregame mulligan/bottom markers at turn 0 + lazy narration markers, not in-game decisions.)

### I-6 — [CORE-PROMPT REPRESENTATION, BELOW-BAR CANDIDATE] affirmative untapped-blocker tagging
- The tapped-read misread class (I-1) is the model failing to enumerate untapped blockers from the ABSENCE
  of a [tapped] tag. A candidate core-prompt mitigation is to tag untapped opponent creatures AFFIRMATIVELY
  in the battlefield representation (e.g. append "[untapped - can block]" to any opponent creature without
  [tapped]), turning a silent absence into a present token the model must read. This would help EVERY seat's
  combat reads, not just deck131. BELOW BAR to propose now: the evidence is single-SEAT (deck131 only, 3
  corpora), and the current representation is CORRECT (the failure is comprehension, not a defect) — routed
  to general-suggestions as a below-threshold candidate for the synthesis agent, NOT a proposal. Promote to
  a real core-prompt proposal ONLY on cross-SEAT convergence (a 2nd seat showing a tapped-read misplay).

## Interface validations (clean this corpus)
- **Mulligan 6/6 correct by STEP-1** — 4 KEEP with 1-2 Mountains + YP (vs109/59/102/27), and the STEP-1
  NO-RED mulligan FIRED TWICE (vs137, vs133 — both no-Mountain/no-dig hands correctly shipped; wave-24
  fired it once). Both post-mull keeps sensible; both bottoms correct (see I-2). Strongest mulligan corpus.
- **Guttersnipe attack discipline 8/9** (held all untapped boards, swung all all-tapped/no-creature boards);
  1 misread swing (I-1), non-decisive.
- **No-block discipline correct** — "no blockers" on non-lethal (vs109 s8/s13, vs137 s7/s11/s18, vs102 s31,
  vs27 s16); the ONE Guttersnipe block (vs59 s27) was correct forced-lethal-only-blocker (I-3).
- **Attacker/blocker declarations pure-index/name, all parsed** (no name-leak fallback).
- **Zero fallbacks** (no empty_reply/retracted_choice/stale_echo/unparsed) — cleanest deck131 corpus again;
  natural-stop (b2) parse correct at both bottom records (answer-first agreed with final line).

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 8+-wave diagnosis holds; the 1/6 is its clean expression (see findings MANDATE 4):
- **Threat density 6/60** (3 Guttersnipe + 3 YP). vs27 ran on a SINGLE engine (no Guttersnipe drawn all
  game); vs133's post-mull hand had ZERO payoff creatures. Variance the deck cannot absorb.
- **No reach vs lifegain/drain** — vs59 (opp to 24), vs27 (opp to 14), vs133 (opp stayed 20). 131 could not
  push face damage past a lifegain/drain wall.
- **No early defense vs aggro** — vs109 (mono-red) killed 131 by T8 (dealt 10 this time, up from wave-24's
  zero, but still out-raced).
- **Clock too slow to race the field** — vs102 (opp to 1, lost by ONE life) is the variance-narrow ceiling;
  a single extra threat or one point of reach flips it. Draw-variance, not a play regression.
- Manabase 14 Island / 8 Mountain: no screw loss this corpus (mulligan gate held; 4/6 kept 1-2 Mountains,
  2/6 mulliganed no-red hands correctly).
- Sideboard candidates unchanged (Spellheart Chimera reach, Aetherling finisher) for the no-reach axis.
- These are the user's to decide; recorded, not argued.

## For a future deck131 reviewer — gotchas
- n=6, single run: win-rate is noise; 1/6 here vs 0/6 wave-24 vs 1/6 wave-23 are all the SAME construction-
  terminal deck under draw variance — do not read a trend. vs137 flipped loss->win on draw variance.
- Positions vary per row: deck131 is deck0 in vs137/vs59 rows and deck1 in the rest THIS corpus — read the
  my_life field / results.tsv columns per record, do not assume a fixed column.
- vs137 wrote NO gameend for this seat (adjudicated win); read final life + adj0 winner from results.tsv.
- **Score Guttersnipe attack windows against ACTUAL [tapped] tags** (I-1) — the misread now generalizes as
  "attackers-tapped => all-tapped," so specifically check FRESHLY-DEPLOYED (non-attacking) opponent
  creatures/tokens for a missing [tapped] tag.
- **The `bottom` record (kind=='bottom')**: score against the RECALIBRATED bottom ladder (I-2) —
  Prism/Elixir > surplus land > over-costed-with-cheaper-in-hand > duplicate; an over-costed or duplicate
  draw IS a valid bottom now; only bottoming an ONLY-copy of a payoff/dig/red-source is an error.
- **Guttersnipe BLOCK is not automatically a slip** (I-3): it is CORRECT when incoming >= my_life AND
  Guttersnipe is the only creature to block with.

## ROTATION VERDICT

**DO NOT ROTATE OUT this wave — guide MODIFIED (Gate 1 fails). But this seat is APPROACHING terminal.**
- Gate 1 (guide-mod?): FAILED to be a candidate — the guide WAS modified (TWO rules-correctness edits:
  bottom-step recalibration + never-block lethal-only-blocker exception). A revised guide is never a
  rotation candidate; it must re-validate next corpus.
- Gate 2 (new-work-signal?): thin but present. The two guide edits need round-1 re-validation. The tapped-
  read guard is only PARTIALLY validated (8/9; one recurrence) and is a STOCHASTIC frailty that is
  guide-unfixable (sanctioned-against to shout louder) — this is NOT a work item, it is a standing WATCH.
  I-6 (affirmative untapped tagging) is a below-bar core-prompt candidate, not this seat's work.

**UNSENTIMENTAL read for the roster owner:** deck131 is CONSTRUCTION-TERMINAL (owner's call, 8+ waves) and
NEAR guide-exhaustion. The two edits this wave are the last obvious rules-correctness gaps in edge decisions
(a mulligan bottom, a forced block) — both were already PLAYED correctly by the model; the guide simply
lagged the play. After they re-validate, this seat likely has NO more guide-fixable work: the tapped-read
misread is stochastic-unfixable, the record is construction-capped, and there are no new decision classes
surfacing. **Projected: deck131 becomes a genuine ROTATION CANDIDATE at wave-26** if (a) the bottom ladder
and the block exception validate (or simply are not contradicted), and (b) no new guide-addressable
decision class appears. It is not eligible THIS wave only because the guide changed. The construction lever
(threat density / reach / early interaction) remains the owner's, off the loop's guide track.
