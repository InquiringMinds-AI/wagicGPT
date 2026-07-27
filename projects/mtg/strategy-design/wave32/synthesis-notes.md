# Wave-32 synthesis notes

Eighteenth per-deck-rotation cycle. Pool = 7: holdovers **146** (2/6), **139** (2/6), **152** (3/6),
**158** (3/6, first guided) + THREE guideless Step-0 debuts **105** (6/6), **36** (4/6), **116** (1/6).
Corpus `matchups-20260727-115847`, binary `7fabd9bd0`, -T 3000, REPPENALTY=1.05.

Deliverables: `strategy-writing-skill.md` (wave-32 edition), this file, `rotation-reconciliation.md`,
`engine-ledger.md`. Deployment is the orchestrator's step, not this agent's.

---

## 0. Corpus health, stated once

| metric | wave-31 | **wave-32** |
|---|---|---|
| games | 21 | 21 |
| timeouts / draws | 0 / 0 | **0 / 0** |
| cap adjudications | 3 | **1 (campaign low)** |
| decisive gameends | 36 logs | 20 decisive, integrity clean |
| crash signatures | 0 / 21 | **0 / 21, with THREE new decks** |
| fallbacks | 6 / 1,432 (0.42%) | **3 / 1,117 (0.27%, ties the record)** |
| defers | 2 | **0** |
| truncated_abandoned | 3 | **0** |
| `validation_reject_reask` | n/a | **0 — no validation reject occurred in this pool at all** |

The three fallbacks: 1 `retracted_choice` (deck158 vs139, §5) and 2 `unparsed_reply` which are **the
same shape** — a compliant `BLOCKS:` head line written with card NAMES instead of `Bn:An` labels
(deck139 s21, deck158 s35). i5's loud re-ask produced zero silent burns but also never fired: it
remains live-validated **only** by the deck198 convoke probe. Pilot `repetition_penalty=1.05`, 10th
corpus: **KEEP**.

---

## 1. THE WAVE'S DOMINANT PATTERN — THE POISON ARC

**Seven of seven seats witnessed it. It is a corpus-integrity defect, not a seat defect, and it is
docket item #1 for wave-33 step 1.**

### 1a. The two halves

**N-105a — poison counters are rendered NOWHERE.** Not the life line (`AIPlayerGPT.cpp:2251` emits
`Your life: N | Opponent life: N` and nothing else), not any player line, not the narration, not
either combat seam. Infect combat damage prints in the **ordinary damage form** — the only tell being
the ABSENCE of the adjacent `- Opponent's life -N (now X)` line that real damage emits, which is the
weakest possible signal — and non-combat poison (`alterpoison`, Ichor Rats) narrates **nothing at
all**, four times across the corpus, never once visible. The engine holds the number
(`Player::poisonCount`) and renders it FOR THE HUMAN (`GuiStatic.cpp:138-140`): a textbook
**ishuman-lens gap**. `AIPlayerGPT.cpp` contains zero occurrences of "poison" outside a comment.

**N-105b — the blocker-seam life forecast is FALSE against infect attackers, and it cost a game.**
`AIPlayerGPT.cpp:6768-6770` builds the defender's forecast from raw attacker power with no infect
check, emitting in **12 of 12** infect blocker windows across the six games:

> `Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal: block
> only where the trade favors you; taking damage while ahead is often correct.`

Three falsehoods for an infect swing: the life will not change; "NOT lethal" is a claim about a
resource the attack does not touch and is INVERTED on any swing crossing 10 poison; and "taking damage
while ahead is often correct" is the exact opposite of correct play against a counter that never
resets and that no card in the pool removes. **Game-losing repro (deck36 s25 t12, the killing swing):**
*"Since the total damage (5) is not lethal (I would drop to 15), and blocking with the Pest achieves
nothing ... I declare no blocks"* -> `BLOCKS: none` -> poison 6 -> 11 -> deck36 loses **at 20 life**.
Four of the twelve windows ended in `no blockers`. Same-seam control: the identical emitter is CORRECT
in every non-infect window in the same corpus, so the defect is branch-scoped to damage-replacement
keywords (**check `toxic` and `wither` on the same pass** — `Damage.cpp:237` reads
`source->getToxicity()`, so toxic exists in this engine and has the same bug).

### 1b. Why it is a CORPUS-INTEGRITY item, not a deck105 item (deck158 P5's framing, adopted)

All six of deck105's wins are poison alt-wins. **Not one opponent was reduced to 0 life** — they ended
at 20, 21, 15, 23, 23 and 14, three of them having GAINED life above their starting total. Every other
seat's loss pricing in this corpus is therefore contaminated:

| seat | loss shape | own life at gameend |
|---|---|---|
| deck158 | ahead on life AND board, opponent's only creature tapped | **15** |
| deck152 | opponent on 2, three points from dead | **23** |
| deck146 | lost while GAINING life | **21** |
| deck116 | chump-blocked its own engine twice to stop poison it could not count | **23** |
| deck139 | knew the rule, could not see the count | **14** |
| deck36 | the corpus's ONLY cap adjudication, at 20-20 | **20** |

Each of those seats' own instruments reported a HEALTHY seat: deck152 had 0 fallbacks / 211, 4/4 full
attacker declarations and a deploy floor that held; deck36 had 4 clean kills and 14/14 defensible
blocks. **A loss can be invisible to every counter the loop owns when the WIN CONDITION itself is
unrendered.** That is why the intake check (enumerate the pool's alt-win conditions and grep for their
state variables, once per pool-composition change) is now P0 and annotates every record in the corpus.

### 1c. deck105's 6/6 is CONFOUNDED, and its own seat review says so

The seat's preserve ledger is flawless — 29/29 attacker declarations, 1/90 cast-nothing (and that one
correct), 2/2 targets, 0/148 fallbacks, 0 crash signatures — and the record is still **not a clean
measurement**, because N-105b told all six opponents at the decisive seam that the swings were
survivable, including on the exact swing that killed deck36. deck105's review reports it as CONFOUNDED
and routes the ledger item rather than reporting "deck105 is strong". Two secondary contributors are
worth carrying into the re-run: poison ignores the pool's main defensive currency entirely (every other
deck competes on the life axis; deck105 finished at <=8 life in four of six games and won all four),
and the deck runs 35 lands in 65 cards with a curve topping at 5 MV, i.e. a lot of slack for a deck the
pool cannot answer.

**MANDATE for wave-33: fix N-105a + N-105b, then RE-RUN deck105 against the same pool before pricing it
for the difficulty ladder.** If it still goes 5-6/6, that is a construction/roster question; if it
drops, the perfecta was substantially a render artifact. **The 6/6 carries an asterisk until then**,
and the wave-33 brief must say so in the corpus-data section rather than repeating it as a headline
result.

### 1d. What the pilot does and does not need

Understanding is NOT the problem. deck105's plans are correct infect strategy from turn 0 (*"aiming to
reach 10 poison counters before the opponent stabilizes"*, *"5 damage = 5 poison counters"*) and it
priced Ichor Rats' symmetric cost correctly at 3/3 casts — the wave-20 symmetric-card face did not fire.
i13's card text is the load-bearing item here: **the infect reminder text is the only place the word
"poison" exists in the entire prompt**, and it is what let a guideless pilot play infect at all. What
fails is arithmetic on an unrendered number, every time it matters: *"I am at 20 life and need 10 poison
counters to win; I currently have 0"* (actual 6, and it won that game while writing *"bringing the
opponent to 5/10"* on the swing that reached 11); *"I need 20 poison. I have 11"* (the threshold
conflated with their LIFE); and the diagnostic 8,827-char / 143.6 s reply ending *"If the game is still
going, then my count is wrong."* **The model does not need to be taught what poison is; it needs to be
SHOWN the count.**

---

## 2. SECOND PATTERN — PATH-SCOPED RECURRENCES CONTINUE, AND HL5 NEEDED THE FIXTURE-SIDE RULE

Wave-31 promoted path-scoping from caution to sweep. It **held the line** — every seat ran the sweep and
three of the four wave-32 recurrences were caught BY it — but two of them were structurally outside its
reach, and both are now written in.

| item | what survived | why HL5 as written missed it |
|---|---|---|
| **N-152d layer 2** | `(printed X/Y)` fixed on the attacker/target enumerators, **still false on the BOARD SNAPSHOT** — `Moonrage Brute (3/3) (printed 2/2)` x22 where the true delta is 0 | nothing; the sweep FOUND it. HL5 working. |
| **N-152h** | the retired MDFC wording survives on a THIRD card class (MDFC *spells*: Emeria's Call still promises the back face "appears there as an alternative-cost cast") | nothing; the sweep found it, cross-seat |
| **N-158l** | an empty `[from ]` on i14's OWN new surface, 38 prompts | nothing; the sweep found it |
| **N-158m (HIGH)** | i14's amass preview reaches **3 of 52 offers (5.8%)** | **INPUT-side scope.** ONE emitter, it works, old string gone, new string greps clean. `amassCountersFromScript` scans `CardPrimitive::magicText` for a literal `counter(1/1`; macro expansion runs on a LOCAL copy in the ability factory (`MTGAbility.cpp:6650`) and is never written back, so every `_AMASSORCn_` card presents an opaque token. |
| **N-152f (MED-HIGH)** | the all-flexible-land-base mutual-deferral deadlock in `deferFlexibleSource` | **FIXTURE-side scope.** The shipped `dual_land_multipip_cast_n152c.txt` is one dual **plus a MONO source of the contested colour** — the branch that already worked. |

Two rungs added to HL5 as a result. **(1) Enumerate the DATA's indirection layers before claiming
coverage for any card-data scan** (macros, `transforms(...)` payloads, `ability$!...!$` wrappers,
alternative-cost faces) and state which representation the scan sees — a one-line design-time check.
**(2) A fix's FIXTURE must ride the BROKEN branch.** N-152f is the textbook case and the reviewer named
it as such: the fixture rode the passing sibling, so the class survived with no test anywhere near it.
A fixture on the passing path is a decoration, not a regression test.

Also folded in: **report an annotation as a COVERAGE FRACTION, never as "it renders"** (49 blank offers
names the defect; "Foray renders correctly" hides it), and **the deciding number belongs at the seat
that COMMITS** — Feed the Swarm's per-target life costs work on the cast line, changed a decision and
won a game (deck158 vsk36 s17/s18), while the subsequent TARGET CHOICE menu, where the target is
actually committed, carries no cost at all (answered blind at 11 and 6 life).

**Guide-side twin (deck152 P3), same shape one layer up.** The wave-31 revision correctly deleted the
false Pathway-flip teach and left a prohibition scoped to *"A Pathway's blue back face"*. Zero Pathway
flips followed — and the seat then spent two priority windows taking the identical option on a DIFFERENT
card (`Flip Side with Tovolar's Huntmaster`, deck105 s28/s31), against an annotation whose last two
words are literally `Ignore this option`. The prohibition named a CARD; the render names a MENU LABEL
riding four card classes. Same failure as a path-scoped render fix: neither author enumerated the
surface.

---

## 3. THIRD PATTERN — A PARTIALLY-FIXED MISREAD MUTATES; REPRESENTATION BEATS WORDING ESCALATION

deck139's two-cycle instance is the cleanest evidence the campaign has produced for the fix-layer
routing, because the guide layer did everything right and still lost.

- **Cycle 1 (wave 31).** Belief: *"this hand has zero lands"* with three dual lands in it. Fix applied at
  the GUIDE layer per wave-31 HL10 — re-aim the exemplar: enumerate the five green lands by printed name
  and quote the render substring (*"A card printed `(land: taps for {R}{G})` IS a land AND IS a green
  source; read the braces, not the name"*).
- **Cycle 2 (wave 32).** The mulligan rate HALVED (2.50 -> 1.50/game, mean kept hand 5.0 -> 5.8, three
  games at zero mulligans) — and the belief did not die, it **MUTATED** to *"zero GREEN mana sources"*,
  with the pilot now **reciting the guide's own list while holding a member of it**: vs105 s1, *"3 lands
  but zero green mana sources (Forest, Stomping Ground, Temple Garden, Overgrown Tomb, Breeding Pool)"* —
  with a literal **Forest** in the rendered hand. **Six false mulligans of nine**, each against a hand
  line that printed `(land: taps for {R}{G})`.

The invented disqualifier is NEW each time (*"Stomping Ground is red/green but you have no way to pay
the life"* — the shockland's ETB choice treated as a barrier to PRODUCING mana). That is why a third
sentence cannot win: **both the guide sentence and the per-card tag are the same move — they assert
MEMBERSHIP, and membership is exactly what the pilot argues with.** The controlled comparison sits in
the same seat, same wave: i11's per-item creature markers produced **ZERO** misreads on the creature
axis while the land/colour axis produced six. A per-item tag answers "is this one an X"; only a COUNT
answers "how many X do I have", which is the question the decision turns on — and a count is the one
form with nothing per-card left to disqualify. Hence **N-139n**: a count header on the mulligan/bottom
hand block, `Your hand (7 cards): lands 3 (green sources: 1) | creatures 4`.

Two corollaries adopted. **Re-aiming an exemplar is a ONE-SHOT remedy** — if the re-aimed exemplar fires
and the belief survives in a NEW phrasing, the lever is SPENT; record it and route upward. And the
**diagnostic** that distinguishes this case from ordinary disobedience: *the reply REPRODUCES your list
and then EXCLUDES a member of it.*

---

## 4. THE MULLIGAN CLUSTER — five of seven seats, three distinct failure modes of one rung

Worth calling out because it is the wave's largest guide-lane cost and it is fully curable in prose.

| seat | mulligans / 6 games | worst outcome |
|---|---|---|
| deck146 | **19** | **two literal 0-CARD opening hands**, 7 mulligans each, both games lost |
| deck116 | 9 | a 0-card hand (vs158), never played a land, dead t8 |
| deck139 | 9 (from 15) | 5 mulligans, 2-card keep, dead t7 having cast nothing |
| deck105 | 2 | mulliganed a 4-land 3-creature two-colour opener for a **1-drop the deck does not contain** |
| deck158 | 2 | the guide's leading COUNT clause called a colour-screwed hand a KEEP; the pilot overrode it |

Three failure modes, all now written into Step-5 rule 6 and Step 4's skeleton:
1. **No floor -> the preference becomes a filter.** deck146's heuristics are sound in isolation and it
   quoted them as the LICENCE for both 0-card hands. The render is faultless — it prints `having already
   taken 4 mulligans you will bottom 4 cards on a keep (keeping 3)` on every ask — so this is a STRATEGY
   failure by the routing, and the guide is the only channel.
2. **A floor keyed to a HELD count -> DEFERRED, not rejected.** deck139's floor was never argued with; it
   was re-promised at every step (*"I will keep whatever I draw next regardless of quality"*), at a seam
   that gets exactly one chance and cannot self-correct, while the prompt printed the authoritative count
   in front of it the whole time. **Key the terminator to a string the prompt PRINTS.**
3. **A criterion the deck cannot satisfy** — the floor caps the spiral but never stops the FIRST
   mulligan, which was the error. Check satisfiability against the decklist at authoring time.

Note in deck146's favour and against reading its 19 as pilot sloppiness: **i7's bottoming machinery is
correct throughout** (15/15 corpus-wide, one bottom ask per game, count exact, labels constant, including
the 7-mulligan case) and a contributing render defect is real — **N-146k**, the pre-game mulligan prompt
renders `Mana available: 0 total (no untapped sources)`, a battlefield fact before a battlefield exists,
and the pilot's phrasing (*"produce mana right now"*) echoes it almost word for word in 5 of 14 mulligan
replies.

---

## 5. CORE PROMPT — PASS, 27th consecutive (7 of 7 seats)

Two items are RECORDED rather than adopted, and one convergence question was CHECKED and answered.

**deck105 G1 — HELD (the reviewer's own preferred disposition).** `AIPlayerGPT.cpp:63` asserts *"You are
playing a duel of Magic: The Gathering. You win by reducing the opponent's life from 20 to 0."* For
deck105 that is flatly wrong — all six wins came at opponent life totals of 20, 21, 15, 23, 23 and 14 —
and it is the ONLY win-condition statement the pilot ever receives, asserted flatly, surviving every
prompt of every game. The proposed edit is one clause (*"— or by any alternative win or loss condition a
card in play states; when a card says a player loses the game, that is as real as life reaching 0"*),
which withdraws an absolute rather than adding a rule. **Held because adopting a core edit in the same
wave as the N-105a render fix would make the two uncorrelatable.** Re-check at deck105's next corpus
after the render lands; adopt only if the conflation PERSISTS with poison rendered. The seat counts as a
PASS and the streak is unbroken.

**deck158's COMMIT terminator — FLAGGED, not proposed.** Evidence: vs139 s16, 12,180 chars / 206 s, at 2
life. The reply OPENS with a complete, correct, protocol-compliant answer — `CHOICE: 1 (Cast Mordor
Muster {1}{b})`, full reasoning, and a `PLAN:` line — then continues for ~10,000 more characters
(*"Wait, looking at the opponent's board..."*, *"Actually, let's re-evaluate..."*) without ever
re-committing. The safety route correctly refused a retracted answer and **a correct decision was
discarded; Baka answered at 2 life.** Not proposed at n=1, and because the trigger is a hopeless board
where a terminator that stops the model re-examining a bad position may cost more elsewhere than it
saves. **The counter is installed instead:** every wave-33 seat reports, as a counted field, `replies
whose text continues past their own PLAN: line`, and of those how many retracted and how many were
fatal. Two or more seats with fatal post-PLAN retractions promotes it to a core proposal with its
measurement already in hand. A new taxonomy entry — the **COMMIT-FAILURE** fallback class — is added
alongside, since neither the wave-26 parser-safety entry nor the wave-20 two-imperative entry describes
the shape (no second imperative in tension, guide not quoted, turn not low-stakes, answer already
produced).

**deck36's phase-header contradiction — CHECKED, NOT adopted.** deck36 asked synthesis to check
convergence on *"the prompt says 'Phase: Main phase 1 | It is the opponent's turn.' This is a direct
contradiction"* (deck152 s15/s16, 3 verbalizations at 2 opponent-turn priority windows). **Grepped all
seven seats' findings: ONE seat, no second witness in the corpus.** Not adopted; recorded as a watch. If
a second seat verbalizes it, the reviewer's own preference stands — a REPRESENTATION change to the phase
header (name the turn owner before the phase: `Opponent's turn - their Main phase 1; you hold priority`)
rather than new core doctrine.

**What core got RIGHT, recorded against future thinning:** the interface-contract anchor ("if a spell is
not in the cast list you CANNOT cast it right now") was quoted back verbatim and OBEYED at three seats,
including deck158 vs139 s16 where it overruled the model's own wrong arithmetic; the plan-staleness
notice fired correctly at four seats; anti-deferral held (deck105: 89 of 90 ask windows took an action);
and the attacker/blocker floors were sufficient without per-deck reinforcement at both strong Step-0
seats (deck105 29/29, deck36 20/21 + 14/14). **The cleanest confirmation yet of the perception-vs-strategy
routing** comes from deck158: core already carried "Never decline a listed action because you believe you
cannot pay for it" through wave-31 and it LOST to a bad surface anyway; the i12 RENDER fix took the false
declines to 0/33 with no core change in either direction.

---

## 6. THE STEP-1 BATCH — per-item verdicts

| item | verdict | evidence |
|---|---|---|
| **i1** N-152c/N-122b generic-fill colour assignment | **PARTIAL PASS** | Mechanism proven at its own discriminator (deck158 s19: Sigarda offered off Overgrown Farmland's SECOND ability, with s14/s16/s17 as the negative control; Teferi path repeats at 4 windows). **Residual A = N-152f** (all-flexible base, code-grounded); **Residual B = N-152g** (LOW, needs a probe, hypothesis NOT acted on). Both contradiction windows are among the seat's largest replies. |
| **i2** convoke | **UNEXERCISED in pool**; probe-validated offer-side only. N-137c layer 2 remains open. |
| **i3** N-146h Agadeem | **UNEXERCISED** — 0 cast offers, 0 casts, 0 menus at deck146. Every appearance is pre-game. Needs a PROBE, not another corpus. |
| **i4** lifelink-ueot | no witness in pool |
| **i5** loud re-ask | **UNEXERCISED corpus-wide** — zero markers, and zero validation rejects occurred. Live-validated only by the deck198 probe. |
| **i6** truncation salvage | **UNEXERCISED** — 0 truncations |
| **i7** London bottoming | **VALIDATED, CLASS CLOSED corpus-wide** — 15 games with mulligans, 15 bottom records, one per game, `says_mulligans == must_put == named` in 15/15, hand size 7-N in 15/15, including the 7-mulligan case. N-139i CLOSED. |
| **i8** combat tags | **VALIDATED** — `[summoning sick ... but CAN block]` 851x corpus-wide, old form **0x**, paradox pair `[tapped - cannot attack or block] [attacking]` **0x**. deck146 notes two surviving stock-phrase verbalizations with the tag present, neither costly -> a WORDING observation, folded into HL6's closing clause, not a defect. |
| **i9** zero-power attacker tags | **VALIDATED** — deck139 26/26 attacker windows declared zero 0-power creatures; the tag is DYNAMIC and read dynamically (a printed 2/4 Dryad reduced to 0/2 by infect counters was excluded by name three times). deck36 calls it the single reason its attacker seam scores 20/21. |
| **i10** attachment handles | **UNEXERCISED, corpus-wide** — the natural witness seat (deck36) swept all 42 translogs: 0 `attached to:`, 0 `{attached:`, 0 `[equipment]`. The pool that was to validate it (deck148) rotated OUT in wave-31. **Deliverable is a PROBE SPEC (N-36h).** The instance-handle HALF is exercised heavily and PASSES (0 unhandled duplicates across 144 prompts). |
| **i11** creature markers | **VALIDATED** — zero "zero creatures" mulligans at every seat that mulliganed. Isolates the still-open LAND/COLOUR axis by contrast. |
| **i12** count-first mana line | **VALIDATED for correctness, class DEAD** — deck158 0/33 false declines against wave-31's 4/55 fatal instances; old string 0 occurrences. **Does NOT reduce tax** (deck105 NM4). ONE counterexample, outcome-neutral, in a won game (deck139 vs146 s21) — logged, not re-opened. **Successor belief found**: cost-notation misparse (`{2}{R}` read as 2 mana). |
| **i13** broad card text on Cast lines | **VALIDATED, adopt permanently** — 100% coverage at every seat. **The flagged length risk did NOT materialise**: deck146 latency p90 115s -> 51s and >60s calls 37 -> 13; deck139 reply p50 925 / p95 6,393 with every latency statistic down; deck158 p95 reply length FELL 46% in the same wave the prompt grew. Only deck152 shows tail growth (prompt p90 +11.8%, max +36%) with no degradation. **At deck105 it is load-bearing**: the infect reminder text is the only "poison" string in the prompt. |
| **i14** magnitude cluster | **PARTIAL.** Foray damage post-amass **PASS 3/3 exact, both branches**, `damage 0` gone. Feed the Swarm per-target life costs **PASS on the cast line** (changed a decision, won a game) / **ABSENT on the target menu** (N-158k). Counter narration **PASS on count**, path-scoped on source (N-158l empty `[from ]`). Amass preview **3/52 = 5.8%** (N-158m, HIGH). |
| **i15** transform-DFC honesty | **half PASS** — literal `backside` is 0 across all 42 logs and both emitters resolve real face names. `(printed X/Y)` **still false on the battlefield line** (N-152d layer 2, 22 occurrences); a THIRD path (MDFC spells) still carries the retired wording (N-152h). |
| **i16** AIStats FATAL | **PASS** — 0 crash signatures across 21 stderr files with three new decks. |

---

## 7. FIXES THAT LANDED — worth recording, because the loop's failures are easier to see than its wins

- **The Acererak class is DEAD, decisively** (deck146 exit-gate item 1). Casts 15 -> **1**; windows where
  offered removal with a live target was declined for Acererak **4 -> 0**; reply passages planning to
  attack/block/count his 5 power **59 -> 0**. The single cast is the guide's exact sanctioned condition
  and the pilot RESTATES the corrected teach while making it. Two further replies refuse to plan around
  him unprompted. Arrival-traced end to end (`hand -> stack` / `stack -> battlefield` / `battlefield ->
  hand`, then the venture chooser). This was the item the whole hold was called for.
- **deck158's kill CLOCK collapsed four turns** — winning turns 11/13/19/19/13 (median 13) unguided ->
  **9/8/10 (median 9)** guided, three clean kills, zero cap adjudications at the seat. The win column
  hides it.
- **deck139's latency tail collapsed** — >195 s decisions **6 -> 1**, median 16,867 -> 15,601 ms, p90
  77,085 -> 58,896. The DECIDE-FAST teach is PROVEN and demotes. The one survivor is diagnostic: the
  corpus's longest reply is a BOTTOM ask (213 s / 12,845 chars on a 7-option pick-5), and it is the same
  belief as §3 — the seat's largest time cost and its largest strategic leak are one belief.
- **deck152's deploy floor cured its target pathology** — declines against the floor FLAT at 2, lowest
  life at any decline **1 -> 11**, and the wave-31 fatal-hold shape gone. Its two survivors sit on the
  SAFETY axis, which is headline 8's new clause.
- **deck139's mutate seam went 6/6 correct** with the mislabel class dead (wave-31: 2 of 2 casts took a
  hardcast line while the reply asserted a mutate; wave-32: 6 of 6 took the `with its mutate cost` line),
  and **N-139g is discharged POSITIVE** — the under-card static discount is live-confirmed twice, so the
  PROMISE-OWED line becomes a validated teach.

---

## 8. Cross-seat items the synthesis is routing rather than adopting

- **N-152j / N-158n — the blockers name-form parser gap.** Both corpus `unparsed_reply` fallbacks are one
  shape: a compliant `BLOCKS:` head line on line 1 with card NAMES instead of `Bn:An`. Two seats
  independently classify it as a **PARSER COVERAGE gap, not a compliance gap**, and both explicitly
  DECLINE to route it to the core prompt — the core already carries a correct positive example three lines
  above the failure, so the model is not missing the instruction, it is substituting an equivalent
  notation, and a negative clause cannot cover the next equivalent notation. **Two-seat convergence on the
  routing is recorded as a standing ledger priority.** Note the honest caveat from deck158: its instance
  ALSO asserted a tapped creature as an available blocker and an illegal flying block, so a name-tolerant
  parser recovers a PARTLY-legal assignment there, not a clean one.
- **N-139s — an ALL-ILLEGAL blocker assignment set exits as a silent `unparsed_reply`**, where the i5
  machinery just installed a loud re-ask elsewhere. MED, instrument-adjacent, ledger.
- **N-116a / N-136a — whole-library tutor reveals.** deck116 renders 44-51 options for eligible sets of
  4/5/7. Mitigated by labelling (an eligibility header plus per-line `[does NOT qualify]` tags) and the
  picks are 9/9 correct, so it is **tax-only** — but the corpus's single largest reply is a 48-option
  Farseek reveal at 13,796 chars / 220.6 s. deck116 verified BOTH emitter framings are unfiltered, so the
  finding is **not** path-scoped and the fix must not be either. Related: **N-116d**, an un-tokened
  `target(...)` renders SUBSET framing on a pick-exactly-one effect -> over-selection in 5/5 Farseeks
  (outcome-neutral here only because the eligible cards were interchangeable).
- **N-36f — "coloured mana cannot pay a generic cost", with the core line PRESENT and quoted.** deck36
  routes it to REPRESENTATION, correctly: the pilot quotes core's own sentence and reaches the opposite
  conclusion, so repetition is not a mechanism. Concrete surface proposal recorded (state what the mana
  BUYS on the mana line). One instance, recovered same-turn.
- **N-139q — Nethroi card-script divergence.** The primitive returns ANY NUMBER of creatures each with
  power<=10; Oracle is "total power 10 or less". Per the standing rule the GUIDE softens to an
  oracle-true description and the deviation routes to notes as a card-script item.

---

## 9. Method-headline map (what the skill gained, and from whom)

| HL | rung | seats |
|---|---|---|
| 1 | unrendered alt-win = P0 corpus integrity (intake check / per-loss visibility / winning-seat cross-seat audit / name-the-resource + count-independent stopgap) | **7 seats** |
| 2 | card-fact audit is a LEDGER, on the SHIPPED guide, with a CLASS sweep | 146 |
| 3 | quoted render strings are a versioned dependency; two-step stopgap retirement; the SUPERSEDED verdict | 158 |
| 4 | HL6 is BIDIRECTIONAL (the adjective is the defect); + the positive unexercised-card form | 152, 105 |
| 5 | path-scope on the INPUT side; coverage FRACTION; the COMMIT seat; the fixture rides the BROKEN branch; guide-side surface-string scoping | 158, 152 |
| 6 | escalate to an engine-computed COUNT; the exemplar lever is one-shot; negate the false SENTENCE by name | 139, 146 |
| 7 | the mulligan rung, three failure modes | 146, 139, 105 (+158, 116) |
| 8 | a floor owes closed exceptions, the SAFETY counter-argument, a lethality-keyed off-ramp, and a RANKING of offers | 152, 116, 105, 139 |
| 9 | Step-0: preserve ledger unconditional; ABSENCE table; #1-slot third case; ABSENT-FIELD audit; self-exclusive magnitude; the two-halves deck | 116, 36 |
| 10 | five instruments: distinct-belief count, instant count before the split, plan-field length, binary hash on both sides, retraction position | 105, 36, 139, 158 |

**Adoption honesty.** Every proposal was adjudicated; three were NOT adopted as their reviewers wrote
them — deck146 P1's non-reviser clause (ADAPTED to a separate terminal pass + the ledger form as the
mechanical substitute, with the second-agent version noted as an orchestrator process option rather than
a rule a seat can enforce on itself), deck158 P6's protocol change (FIELD adopted, TERMINATOR flagged
pending the counter), and deck105 G1 (HELD). deck116 P1(b) is adopted but TAGGED single-seat with the
reviewer's own promotion condition. deck36 P6 is recorded as housekeeping with no rung.
