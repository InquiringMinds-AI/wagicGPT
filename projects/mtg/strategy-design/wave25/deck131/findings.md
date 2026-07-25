# deck131 — wave 25 review (UR Guttersnipe spellslinger). Guttersnipe guard round 3 + bottom-phase round 2 + turn-1 upkeep (a1).

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-035605/` (binary /tmp/wagic-62d8783a5:
RULES-FIX batch f0500ad66 + wave-25 step-1 batch, -T 3000, REPPENALTY=1.05). deck131 = 6 games.
Live guide = the wave-24 REVISED guide (10910 bytes; deployed == wave24/deck131/strategy.txt, byte-verified
via `cmp` exit 0 — the additive BOTTOM-STEP line is LIVE this corpus).

**Record: 1 W / 5 L (1/6).** The win is vs137 by adjudication (results.tsv `adj0`, 131 ahead 6-2 at the
T12 cap). Seat->game map (results.tsv; 131 is deck0 vs 137/59, deck1 vs the rest):

| result | vs (deck) | end life me/opp | turn | one-line cause |
|---|---|---|---|---|
| **WIN (adj)** | 137 (Selesnya Adventures) | 6 / 2 | 12 | mull->loaded-7 keep, curved YP T2/Gut T4/Gut2 T8, chained + swung Gut when safe; **1/6** |
| LOSS (death) | 109 (Mono-R Hellrider) | 0 / 10 | 8 | aggro out-race; dealt 10 (better than w24's zero); no early defense |
| LOSS (death) | 59 (WB Persistence, GUIDELESS) | -8 / 24 | 15 | both payoffs but Gut arrived T12; forced-lethal Gut block s27; cannot race persist+lifegain |
| LOSS (death) | 102 (Tergrid) | -1 / 1 | 14 | **got opp to 1**; swung correctly throughout; lost by ONE life (variance-narrow) |
| LOSS (death) | 27 (Undead Infiltrator) | -1 / 14 | 14 | no Guttersnipe drawn all game; single YP engine, no reach vs go-wide+lifegain |
| LOSS (death) | 133 (Phyr. Asphodel) | -2 / 20 | 14 | correct mull; post-mull hand had NO payoff; opp (drain deck) stayed 20 |

**Bottom line:** the record is **CONSTRUCTION-TERMINAL, unchanged from the standing 8+-wave diagnosis**,
now expressed at 1/6 (up from wave-24's 0/6 — the vs137 loss flipped to a win, and vs102 came within ONE
life). Zero fallbacks corpus-wide (cleanest deck131 corpus again). Every game was fully piloted. The pilot
is squeezing more out of the same deck (opponents brought to 2/10/24/1/14/20; a win and a 1-life loss), but
the four death-losses are the known limits: no early defense vs aggro (109), cannot race persist+lifegain
(59), no reach vs a lifegain/drain wall (27, 133). NONE was lost to a guide-fixable decision.

**Two GUIDE-FIXABLE findings this corpus** — both are the SAME shape: the model played CORRECTLY but its
correct play VIOLATED a too-absolute guide rule, exposing a rules-correctness gap in the guide (not a
pilot error, and NOT the sanctioned-against "shout louder"):
1. **Bottom-phase line (round 2): the "never bottom a draw spell" absolute is MISCALIBRATED.** Both bottom
   decisions correctly bottomed a draw spell (over-costed Opportunity; a duplicate Epiphany). Guide edit:
   rank the bottom by impact/redundancy; allow bottoming an over-costed or DUPLICATE draw; only "never
   bottom your ONLY draw/payoff/red-source."
2. **Never-block line: the "never Guttersnipe, even to die" absolute is Magic-WRONG when Guttersnipe is the
   ONLY blocker facing LETHAL.** vs59 s27 correctly blocked-to-survive with a lone Guttersnipe. Guide edit:
   add the lethal-and-only-blocker exception.

The Guttersnipe tapped-read guard was NOT touched (one round-3 misread recurred, vs137 s26, but it is a
stochastic model-comprehension frailty on a CORRECT representation, non-decisive — sanctioned-against to
shout louder; it goes to notes WATCH). See below.

---

## MANDATE (1) — GUTTERSNIPE TAPPED-READ GUARD, ROUND 3: ONE MISREAD RECURRENCE (non-decisive)

Every Guttersnipe attack/hold window this corpus, diffing the model's claimed tapped-set vs the prompt's
actual opponent [tapped] tags:

| game | seq | T | board (relevant) | model read | decision | verdict |
|---|---|---|---|---|---|---|
| vs109 | s12 | 5 | Stromkirk[tapped], Goblin[tapped] | held Gut, swung YP | HOLD | CORRECT (conservative) |
| vs109 | s20 | 7 | all 4 (Stromkirk/2 Goblin/Hellrider) [tapped] | "all tapped, no blockers" | SWING Gut+YP | **CORRECT** |
| vs137 | s10 | 4 | Flaxen[tapped] (fresh Gut, held) | held Gut | HOLD | CORRECT |
| vs137 | s14 | 6 | Flaxen (1/2) [tapped], only creature | correct | SWING Gut | **CORRECT** |
| vs137 | s17 | 8 | Flaxen (2/3) UNTAPPED present | untapped read | HOLD both Gut | **CORRECT** |
| vs137 | s22 | 10 | Flaxen[tapped], Edgewall[tapped] | "both tapped" | SWING Gut,Gut | **CORRECT** |
| vs137 | **s26** | 12 | Flaxen[tapped], Edgewall[tapped], **Trostani(1/4) + 2 Soldiers UNTAPPED** | **"no untapped creatures to block (all attackers tapped)"** | SWING Gut,Gut | **MISREAD** |
| vs102 | s13 | 7 | opp 0 creatures | safe | SWING Gut+YP | CORRECT |
| vs102 | s30 | 13 | Tergrid[tapped], Sheoldred[tapped] | "all tapped" | SWING Gut+YP | **CORRECT** |

**Round-3 result: the guard works 8/9; ONE fresh tapped-misread (vs137 s26).** The model claimed the
opponent had "no untapped creatures to block (all their attackers are tapped from last turn)" — but
Trostani Discordant (1/4) and its two 1/1->2/2 Soldier tokens, all just deployed, carried NO [tapped]
tag and could block. **This is the SAME class as wave-23 vs27 (Lord of the Undead) and wave-24 vs59 s23
(Restless Apparition), now at 3 corpora / this seat.** A new sub-shape appeared in the model's rationale:
it reasoned "the ATTACKERS are tapped, so nothing can block" — forgetting FRESHLY-DEPLOYED (non-attacking)
untapped blockers. **Non-decisive:** s26 was the T12 adjudication-cap turn with opp at 2 and 131 at 6 —
the swing was EV-robust regardless of the read (unblocked = win now; blocked = trade but 131 still wins the
6-2 life adjudication that ends the game that turn). So the ACTION was fine; only the STATED REASON rested
on a false board read. Recorded as rung-2 (decision-input) misread, non-decisive.

**Guide UNTOUCHED on the guard** (sanctioned-against to shout louder at an already-explicit, already-8/9
line). The guard prose already covers a just-created untapped token ("a creature printed WITHOUT [tapped]
is UNTAPPED and CAN block... check every creature individually"). The residual is stochastic model
comprehension -> notes I-1 WATCH (now 3 corpora at this seat, still single-SEAT).

---

## MANDATE (2) — BOTTOM-PHASE GUIDE LINE, ROUND 2 + NATURAL-STOP (b2) AT BOTTOM RECORDS

**Two bottom records this corpus (vs137 s3, vs133 s3) — both after a correct STEP-1 mulligan, both a
mull-to-6 keep needing to bottom exactly 1.** BOTH bottom decisions are STRONG play, and BOTH VIOLATE the
wave-24 guide's literal "never bottom ... a draw spell (Artificer's Epiphany/Opportunity)" absolute:

- **vs137 s3:** hand = 2x Guttersnipe, Young Pyromancer, **Opportunity ({4}{u}{u})**, Artificer's Epiphany,
  Mountain, Island. Model bottomed **Opportunity** — the correct bottom: a loaded keep (two payoffs + the
  cheap dig), only 2 lands (keep both), no Prism/Elixir/surplus. The 6-mana Opportunity is genuinely the
  worst card; keeping the cheaper Artificer's Epiphany as the dig is right. Reasoning was impeccable.
- **vs133 s3:** hand = **2x Artificer's Epiphany**, Essence Scatter, Island, Hydrolash, Mountain, Cyclonic
  Rift. Model bottomed the **duplicate Artificer's Epiphany** — keep one dig, bottom the redundant second.
  Also correct: 2 lands (keep both), no Prism/Elixir/surplus, no payoff to protect.

**Round-2 verdict: the bottom LINE is MISCALIBRATED.** The wave-24 addition (added to fix vs27 s3 keeping
Prism Ring) contains an absolute "never bottom a draw spell" that is Magic-WRONG for exactly these hands —
an over-costed draw (Opportunity when a cheaper dig is in hand) and a DUPLICATE draw ARE the correct bottom.
Neither hand this corpus contained Prism Ring or Elixir, so the "bottom Prism Ring FIRST" part (the piece
the line was actually written to fix) went UNTESTED — the part that DID get exercised was falsified twice.
The model correctly IGNORED the bad absolute both times; had it obeyed literally, it would have bottomed a
Guttersnipe or a needed land instead. **-> JUSTIFIED guide edit** (see strategy.txt): keep the
Prism-Ring/Elixir-first ranking (still correct, still untested this corpus but sound), and replace the
"never bottom a draw spell" absolute with an impact/redundancy ranking that endorses bottoming an
over-costed or duplicate draw while protecting your ONLY red source / ONLY payoff / ONLY dig. This is a
rules-correctness fix, NOT "louder at a covered line" (the covered behavior was WRONG, not under-emphasized).

**Natural-stop (b2) at the bottom records: no first-vs-last conflict, parse correct both times.** Both
replies are answer-first ("PUT: 4 (Opportunity)" / "PUT: 7 (Artificer's Epiphany)") and their concluding
PLAN restates the SAME bottom — no natural-stop/answer-first divergence to resolve. Parser recorded
chosen_text='Opportunity' / "Artificer's Epiphany" correctly (the numeric `choice` field is an internal
option index and differed from the "PUT: N" label, but chosen_text is the reliable field and matched the
model's intent both times). Natural-stop precedence was not stressed here (answer-first and final line
agreed); no defect observed.

---

## MANDATE (3) — TURN-1 UPKEEP (a1) + NO-UNTAP-CASTING (a2): CLEAN, NO CARD-LEVEL SURFACE AT THIS SEAT

**(a1) turn-1 upkeep:** deck131 runs ZERO upkeep-triggered cards (Guttersnipe/Young Pyromancer trigger on
CAST; Prism Ring on lifegain; Elixir is an activated ability). So NO card at this seat "fires a turn
earlier" under the a1 change — there is nothing to fire at upkeep. The a1 change surfaces only as the model
now RECEIVING priority at its upkeep (7 upkeep-phase priority records this corpus, turns 5-13). It handled
them correctly: mostly `pass`, and twice activated Elixir of Immortality at upkeep for 5 life (vs109 s14
T7, at a life it deemed low). No narration confusion, no phantom triggers, no expectation of an old redirect
menu. Clean.

**(a2) no untap-window casting:** confirmed NO in-game untap-phase priority/cast/activation this corpus.
Every cast and activation is at a main phase (or a legitimate response window). The seat's historical
untap-window activations (old fixtures) do NOT recur under a2 — behavior change confirmed and correct.
(The "Phase: Untap" strings that appear in prompts are pregame mulligan/bottom markers at turn 0, and lazy
narration phase-markers — not in-game untap decisions.)

**(a4) pw-redirect removal / (a5) battles / (a6) day-night:** N/A at this seat (no planeswalkers, battles,
or day/night cards in deck131 and none observed opponent-side driving a 131 decision).

---

## MANDATE (4) — 1/6 DECISIONS-READ (construction-terminal flag: CONFIRMED, STANDS)

Zero fallbacks at this seat this corpus (no empty_reply/unparsed/retracted_choice/stale_echo). Every loss
is a real, fully-piloted game. Decisions-based read:

**The 1 WIN (vs137) was cleanly piloted, best-case for the deck:** correct STEP-1 mulligan of a no-Mountain
hand -> loaded-7 keep -> correct bottom (Opportunity) -> curve YP T2 / Gut T4 / Gut2 T8 -> chained spells,
held Guttersnipe on untapped boards (s10/s17) and swung it on tapped boards (s14/s22), closed at the T12
adjudication ahead 6-2. Exactly how the deck is meant to play. The one blemish (s26 tapped-misread) was
non-decisive on the winning turn.

**The 5 LOSSES are construction-terminal, unchanged (owner's ledger, restated not relitigated):**
- vs109 (mono-R): killed T8; dealt 10 (opp 20->10) — improved over wave-24's zero, but no early defense.
- vs59 (WB persist): both payoffs but Gut landed T12 and immediately had to block-to-survive; opp LIFEGAINED
  to 24. Cannot race persist+lifegain. **No tapped-misread SWING this game** (contrast wave-24 vs59 s23).
- vs102 (Tergrid): **1-life loss** — swung Guttersnipe correctly every window (opp all-tapped reads right),
  got opp to 1. Variance-narrow; the closest construction-loss on record for this seat.
- vs27 (Undead): NO Guttersnipe drawn all game; single-YP engine, no reach; opp go-wide+lifegain to 14.
- vs133 (Asphodel): correct mulligan, but post-mull hand held NO payoff creature; drain deck stayed 20.

**Construction-terminal flag: CONFIRMED, STANDS.** Threat density 6/60, no reach vs lifegain/drain, no
early defense vs aggro, clock too slow to race the field. No guide edit changes this record; the lever is
DECK CONSTRUCTION (owner's call — sideboard already carries Spellheart Chimera reach / Aetherling finisher).
The two guide edits this wave fix rules-correctness gaps in EDGE decisions (a mulligan bottom, a
forced-lethal block); neither flips a game, and both were already played correctly by the model.

---

## MANDATE (5) — REPPENALTY=1.05 round 4 + retry: SAME, no degradation

- Reply length p50 ~1200 / p95 ranged 3377 (vs27) to 13305 (vs137) / max 15217 (vs137, within the corpus
  p95 7997 / max 15217 norms cited in the brief). No decode-collapse, no token-loop; the longer vs137 tail
  is coherent deliberation on the winning game.
- Retry fired 0x — CORRECT for this seat (no garbage/decode-collapse replies to catch; every reply examined
  opens with a clean in-range first line). Zero fallbacks confirms the parser stack converged here too.
- Latency: within corpus norms under -T 3000; no timeout-draw at this seat (the corpus's 1 timeout-draw is
  elsewhere).

**Verdict:** rep_penalty=1.05 at deck131 — SAME; prose coherent, self-correction is the normal N9 family,
zero decode-collapse, retry correctly 0.
