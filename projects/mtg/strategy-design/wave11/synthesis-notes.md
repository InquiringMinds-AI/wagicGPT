# Wave-11 synthesis notes — the SECOND verification corpus (cold-resume record)

**Corpus:** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254` — 21 games round-robin over decks
44, 109, 110, 131, 133, 135, 140. Binary `/tmp/wagic-54a5a9d01` = the wave-10 engine batch (max_tokens 4096 +
parse ramble guards + stack-target annotation fix + fetch pre-targeting keying), **answer-after-plan** reply
protocol, wave-10 guides (deck109 +1 hybrid line). Core prompt unchanged. Baselines: wave-10
`matchups-20260715-031825` (2048 cap, pre-guards), wave-9 `matchups-20260714-094942` (head-first control).
Win table: 109 5/6, 133 5/6, 44 3/6, 140 3/6, 110 3/6, 135 1/6, 131 1/6; 0 raw timeouts, 6 life-adjudicated.

This is the SECOND corpus after the 2026-07-14 pause. Its job was to VERIFY the wave-10 batch end-to-end AT
SCALE, not to open new authoring diagnoses. **Dominant mode: FREEZE — all seven guides are byte-identical to
the live baka guides** (verified by diffing each `wave11/deckN/strategy.txt` against
`bin/Res/ai/baka/deckN_strategy.txt`; zero bytes), and all seven seats voted core PASS. Every method adoption
is a verification refinement (how to SCORE a shipped / still-unshipped fix), not a new guide headline.

---

## 1. Corpus verdict — validation table (which shipped fixes were confirmed)

| Shipped fix | Verified? | Evidence |
|---|---|---|
| **Ramble / truncation guards (prose-hijack + scavenge)** | ✅ CLOSED corpus-wide | Zero prose-hijack casts; every fallback -> heuristic (choice 0/-1), no scavenged body-digit cast. deck140 machine-checked over all 6 seats; deck44 own-targets 29/29 obeyed, 0 recorded self-casts (wave-10 was 36/1); deck133 out-of-range `CHOICE:3` REJECTED not int-grabbed. Wave-10 classes closed at deck133 (self-Fatal-Push number/parse), deck44 (truncation-scavenge s68), deck140 (formless s34). |
| **Stack-target annotation fix (counter options)** | ✅ 0 contradiction corpus-wide | `grep 'NO legal target right now'` = 0 at every seat. Counter/removal options render cleanly (`- can target on the stack: X`). Force reasoning-tax COLLAPSED ~470 -> ~100 words (deck135/deck140 s30-s32); Force target discipline recovered (15 windows / 0 casts, wave-10's soft-target mis-Forces gone). |
| **deck109 hybrid line ({B/R} Rakdos Cackler)** | ✅ class -> 0, fact echoed | 0 "lacking black mana" declines (was 3), 0 spiral markers (was 1), 2/2 offered `{b/r}` casts taken; reply echoed the guide fact verbatim. Interim carrier; the option-line hybrid annotation is still UNSHIPPED (0 `- hybrid:` strings), so the note stays at full prominence. |
| **Artifact-tag completeness** | ✅ 211/211 agree, zero offset | deck110 re-derived per-line `[artifact]` count agrees with `Artifacts in play: you N` in 211/211 (up from 177/177). Residual hand-fold-in persists but is OUTCOME-neutral (all 7 Galvanic Blasts at surf>=3). |
| **Thin-hand / empty-hand Thoughtseize annotation** | ✅ 100% obeyed, 2nd wave | deck133: 48 windows, 0 casts into EMPTY hands (10+9), 0 into THIN hands; all real casts on full early hands. |
| **Gray Merchant bare-N + `resolved` narration** | ✅ 3/3 shown==resolved | deck133: drains8 -> +8/-8, drains7 -> +7/-7 (lethal), drains4 -> +4/-4. The "+2" workaround stays DELETED, no regression. |
| **Fetch pre-targeting keying** | ✅ no re-ask CLASS | Fetch windows 115 corpus-wide (vs 96 wave-10, 216 wave-9) — the small rise is trajectory, not a re-ask class. Residual: a CHOSEN-but-unresolved crack re-offered for ONE window (down from wave-10's ~2), decision-count inflation only (ledger #7). Minor cosmetic dup-line render (deck135 ENGINE #3). |
| **adjudicate-by-life-at-cap** | ✅ 2nd corpus sane | deck140 reached the cap 4/6, all adj by life, rows exact vs seat life; one life-snapshot timing curiosity (deck140 vs44), not a correctness bug. |
| **Answer-after-plan (head-first intent-collapse)** | ✅ still DEAD all 7 seats | Every reply non-numeric-headed; deck110 0 true head-vs-choice mismatches over 223; deck131 mulligan close held a 2nd corpus (7/7 both directions); deck109 attacker seam 26/26. |
| **Bare `BLOCKS: none` parser bug (deck131 found)** | ⏳ SHIPPED-PENDING-CORPUS | Fixed in the working tree (AIPlayerGPT.cpp: explicit all-decline now declares no blockers; suite running). Not yet corpus-verified. See ledger #5. |
| **4096 max_tokens (2048-truncation class)** | ✅ but DISPLACED to latency | unparsed_reply fell 60 -> 11; but empty_reply rose 3 -> 72 (4.7%) = the 120s HTTP-timeout on longer tails, all self-healing, none game-deciding. 240s `WAGIC_GPT_TIMEOUT` default committed (95cf9f5f9) for the next corpus. |

Net: the entire wave-10 batch VALIDATED; no shipped fix was inert or regressed. The only interface cost is the
truncation->latency DISPLACEMENT (empty_reply 72), already addressed for the next corpus.

---

## 2. Ranked engine / harness ledger (merged from all 7 notes.md — repros in the per-deck files)

Layer-routing: these live HERE, never in prompt/guide text. Ranked by severity × evidence.

1. **Force of Negation's FREE ALTERNATIVE COST never counters — the targeted noncreature spell resolves anyway
   and the pilot bleeds blue cards (HIGHEST, CARRIED, next cycle's top fix).** deck135 ENGINE #1. The
   annotation half shipped (see #b below); the RESOLUTION bug stands — `planCastSpell` fences the free
   alternative (`viaAlternative`) to the legacy path where the stack-target binding is lost, so Force pays its
   exile cost and resolves as a pure cost-payment. BLEED is now SMALL but the MECHANISM is intact: deck135 this
   corpus = 1 Force cast (on Pyroclasm, a correct board wipe), adjudicated mid-resolution, so NO confirmed
   counter AND no confirmed bleed this game (down from wave-10's 4 casts / 0 counters / ~7-8 exiles — partly
   trajectory, partly the restored discipline from #b). FIX: bind the stack noncreature spell as Force's target
   (the `can target on the stack: X` hint already knows it exists — the two code paths disagree). DETECT
   REGRESSION: a Force cast where the targeted stack spell's EVENTS show `stack -> graveyard/battlefield`
   (resolved) instead of a counter. VALIDATION TODO: force a NON-adjudicated Force-on-board-wipe window and
   confirm the wipe is COUNTERED and the blue card is spent for real effect.
   - **#1b (FIXED/VALIDATED):** the `NO legal target right now` annotation no longer applies to counter-type
     (stack-targeting) options. `grep` = 0 corpus-wide; reasoning-tax collapse + Force target discipline
     recovered (deck135). The interface rung is closed; the resolution rung (#1) is the still-pending item.

2. **Auto-tap greedily taps COLORED sources for GENERIC costs, stranding held counter mana (GAME-CHANGING;
   RECURRED outcome-neutral = clean regression fixture).** deck44 #2. Cost the vs140 Blightsteel game in
   wave-10; wave-11 RECURRED mechanically but harmless: vs131 s30 (Archmage {4}{u} tapped all 3 Islands with 3
   Swamps free -> single Swamp left) and s21 (Sprite {1}{u} stranded a blue), with NO counter window before
   untap (game won). The loss-decoupled vs131 s30 repro is a BETTER regression fixture than the wave-10
   loss-tangled one. FIX: auto-tap generic costs from colorless/off-color sources FIRST, preserving colored
   sources (esp. when the player holds an instant of that color). The guide's "leave two blue sources untapped"
   is UNEXECUTABLE without this. Owner: the mana/auto-tap payment code. Standing clearest game-changing item.

3. **Unlisted/unaffordable-card fixation reconcile-arm (reply-protocol A/B; 2+3 seats, bidirectional,
   sharpened spec).** The answer-after-plan trailing PLAN/CHOICE mismatch has ONE root: the plan's NAMED action
   is a card ABSENT from the options because it is unaffordable this turn, and the trailing index mis-maps.
   BIDIRECTIONAL — OVER-cast (unlisted finisher on an empty board -> listed sweeper over Cast-nothing: deck140
   vs135 s10, vs131 s27, Pyroclasm into `creatures:0`) and UNDER-cast (unlisted sweeper/removal at C>=1 ->
   Cast-nothing over an offered payable removal: deck140 vs109 s9). One flavor emits an OUT-OF-RANGE index
   (deck109 vs135 seq12: unlisted Hellrider, `CHOICE:5` for 4 options -> -1 -> heuristic; deck131 H3 vs110
   seq17: unlisted Young Pyromancer, `CHOICE:5`). 3 witnessing seats this wave (deck140, deck135 Q5, deck109) +
   wave-10 deck110-P3 lineage. SHARPENED SPEC: the reconcile-named-action arm must, when the named action is
   ABSENT from options, route to the BEST AVAILABLE LISTED option (payable removal if the plan wanted removal;
   pass/Cast-nothing if it wanted to hold or deploy an unaffordable finisher) — never an arbitrary cast — and
   reject-and-repair an out-of-range index. Dominates "accept the last bare number." Route to the code-appended
   reply-protocol layer; NEVER the guide/core (2nd-seat item cannot touch core). NOTE the phantom-board
   look-alike (deck140 P2, deck110 s68/s82): a PLAN-vs-BOARD misread (plan believes a creature present) is a
   different LAYER (representation) — read the plan before routing a "sweep into creatures:0" symptom.

4. **Multi-copy equip churn no-op marker keyed on TARGET-IDENTITY, blind to a net-zero/low-value move between
   EQUIVALENT instances (MEDIUM; RECURRED milder, one costly instance).** deck110 E2. Wave-10 was a net-zero
   A->B->A bounce (a loss); wave-11 the CONSOLIDATION shape — deck109 T8 both Cranial Platings consolidated onto
   Master while the plan believed the Ornithopter kept one, stripping the flyer clock at 4 life in a race it
   lost (2 wasted `{1}`). Plus a minor same-turn re-pick (deck135). The single OUTCOME-DELTA predicate ("moving
   the equipment to X would not increase X's resulting power above the current carrier's") catches both the
   two-Platings and equivalent-carriers shapes. Owner: `AIPlayerGPT.cpp` equip-option serialization. NO guide
   line (the guide already forbids it; the pilot has no surfaced gain signal). SCORING NOTE (deck110): classify
   each equip run correct-stack / correct-upgrade / churn against the outcome delta before quantifying — the
   sanctioned two-Plating stacks (deck140 s32/33, s46/47) are NOT churn.

5. **Bare `BLOCKS: none` unparsed -> baka blocker heuristic runs (GAME-RELEVANT; SHIPPED-PENDING-CORPUS).**
   deck131 H1. Corpus-wide `BLOCKS: none` -> unparsed 4/4, `BLOCKS: B<n>:none` -> parsed 3/3. The baka fallback
   is NOT a no-op: at vs109 it blocked Guttersnipe (the deck's only reach, which the guide most protects, and
   which the pilot reasoned to NOT block) into a fatal trade, in a loss. FIX ALREADY IN THE WORKING TREE
   (explicit all-decline now declares no blockers; AIPlayerGPT.cpp; suite running) — record SHIPPED-PENDING-
   CORPUS, verify next corpus. Reviewer method: for a `kind==blockers` record `chosen_text='None'` records the
   PARSE, not executed combat — cross-check the next record's combat `events`.

6. **Hybrid-cost option-line annotation (representation; cross-deck backstop; STILL UNSHIPPED).** deck109
   Ledger A. Append `- hybrid: pay with {R} or {B}` to any option whose cost carries `{X/Y}`. 0 `- hybrid:`
   strings in the corpus -> the deck109 guide card-note is the SOLE carrier and must NOT be demoted until this
   ships. Owner: the option-text builder (same site as `{right now: ...}` / `- legal targets right now: ...`).
   Fold in the colored-pays-generic affordability breakdown on the `{N}{color}` option line (the durable lever
   for that family; core stays PASS — §5).

7. **Fetch chosen-but-unresolved re-offer residual (minor; decision-count inflation only).** deck135 ENGINE #2.
   Improved wave-10's 3-windows-per-crack to a consistent 2 (`choose -> re-offered once -> resolve`); the crack
   still sits on the stack for one window. Low-impact. Cosmetic sibling (ENGINE #3): duplicate-identical
   targeting option lines when the deck holds multiple copies of the same snow basic — dedupe.

8. **Hand-card artifact type-tag sub-gap (low).** deck110 E3. The `[artifact]` tag lives on the BATTLEFIELD
   line only; artifact CREATURES in HAND show only P/T, so forward projections ("cast X to reach N artifacts")
   guess the type. The wave-10 under-count did not recur clearly. Candidate micro-lever: tag artifact cards in
   the HAND serialization. Non-outcome-affecting.

**Reviewer-facing / minor (game-neutral):** `resolved:` field still not emitted (deck133 #5 — `events` carries
the deltas; an explicit `resolved: battlefield|countered|fizzled` would remove the narration-scrape);
`tools/intent-collapse-metric.py` still uncalibrated for answer-last (deck140/135 HARNESS #1 — its reversal
flags are POINTERS to read, never a defect count; recalibrate before any cross-arm count).

**Model-comprehension watches (harness eye, NOT guide lines):** reasoning-disobey of the own-targets
annotation via a stack-vs-battlefield / destroy-vs-counter misconception (deck133, self-Fatal-Push onto a
creature SPELL on the stack — 2 instances, both won; representation refinement = name WHY the on-stack target
is illegal); first-strike combat-math error at the block-when-not-lethal seam (deck131 H4, single-seat);
Azcanta-transform card-fact hallucination (deck135, single window); confusion-spiral ROOT = mana-availability
distrust ("no untapped sources") / generic-mana-payment looping (deck131 H2 — the 4096 cap SPLIT it: most
conclude correctly, the deepest run past 120s to the HTTP timeout; the 240s fix is the lever, the model
comprehension is the root).

---

## 3. Guide dispositions (VERIFIED by diff against the live baka guide)

`diff bin/Res/ai/baka/deckN_strategy.txt strategy-design/wave11/deckN/strategy.txt` — all seven IDENTICAL:

| Deck | Disposition |
|---|---|
| deck44 | FROZEN — byte-identical (8th wave). |
| deck109 | FROZEN — byte-identical (the wave-10 hybrid line is VERIFIED end-to-end; KEPT at full prominence — its representation backstop is unshipped, sole carrier, thin sample). |
| deck110 | FROZEN — byte-identical. |
| deck131 | FROZEN — byte-identical (7th consecutive freeze). |
| deck133 | FROZEN — byte-identical (zero-edit diff). |
| deck135 | FROZEN — byte-identical (13357 bytes). |
| deck140 | FROZEN — byte-identical (7364 bytes). |

`general-strategy.txt` = byte-identical copy of the live core prompt (`cmp` clean). See §5.

---

## 4. Deck-construction flags (restated for the user; user owns the decklists — NOT relitigated)

- **deck44 (UB tempo/control):** the all-reactive/creature-light slow keep is the loss signature — all three
  losses this wave (vs109, vs110, vs140); the win rate dipped 4/6 -> 3/6 purely on draw variance (same spine,
  same obedience). Counters EARN their slots (protected all 3 wins). Levers: black-source density vs the black
  spell load; 1-2 more cheap flyers over a 3rd/4th reactive card. "Trim the counters" is the WRONG lever.
- **deck109 (RDW):** no answer to evasion (flyers/shroud/pro-red); no card advantage / mana sink — structural
  and racy (beat vs44 untouchable-flyers on the race this wave, lost it last). 5/6 — do not change.
- **deck110 (affinity):** few creatures drawn into a flood deck owns the 4-loss column. vs135 flipped L->W on
  identical guides = pure draw variance. If tuned: trim 0-power filler (Signal Pest 0/1, Ornithopter 0/2)
  and/or Mox/land count toward more mid bodies + reach/evasion-block redundancy.
- **deck131 (Guttersnipe spellslinger):** threat density 6/60; the 1/6 WIN was the YP-swarm second win-path
  (Guttersnipe never cast, drew+held two Young Pyromancers) = variance in the redundant-payoff direction, not
  a play change. No non-incremental finisher / no reach vs lifegain (vs140 lost adjudication by THREE life — a
  single reach finisher likely flips it); sideboard carries Spellheart Chimera + Aetherling. 1/6, obeyed-but-
  losing 7th wave — wins come from CONSTRUCTION, not guide prose.
- **deck133 (Golgari devotion/drain):** best corpus yet (5/6, all clean kills). The one LOSS (vs44 Faeries):
  opp sat static at 20, deck133 dealt ZERO damage and bled out on its own fetch/discard/Yawgmoth self-damage —
  no reach/flyer answer except Gray Merchant (density 4/60, no selection); no early blocker (Bloodghast can't
  block). Consider +1 GM / cheap selection / a flyer-defense source / a 1-copy Thoughtseize->Inquisition shift.
- **deck135 (Modern Snow):** the manabase strands the deck's own 2-color win creatures (Coatl {G}{U}, Viper
  {1}{G}{G}, Treefolk {2}{G}{U}, Diamond Faerie {2}{G}{W}{U} off only 2 Snow-Island + 2 Snow-Plains + fetches);
  slow clock is structural (Treefolk lands T6-T11). Raise effective double-pip sources or shave a pip. The
  pilot-side develop is fixed. vs110 marginal keep reverted W->L this wave — single point, variance-bound;
  leave the mulligan rule FROZEN.
- **deck140 (BR/grixis control):** reach starvation + mana-screw is the ENTIRE loss column (vs109 had no red
  for Pyroclasm at the pivotal C=5; vs133 out-raced). 14 sweepers is the right density — it needs TEETH, not
  text: (a) a cheaper/second recurring clock that survives a counter; (b) a 1-2 mana early lifegain/blocker vs
  aggro; (c) more untapped RED so Pyroclasm {1}{r} is a reliable turn-2/3 sweeper.

---

## 5. Core adjudication — PASS (no prompt-body edit; 8th consecutive wave)

**No 2+-seat core candidate this wave.** The wave-10 offered=payable / colored-pays-generic false-belief family
did NOT harden further: deck135 saw NO recurrence (the model self-corrected the "generic" inference in every
window), and the deck109 hybrid-misread class went fully to ZERO under its validated card-note. The single
fresh witness (deck140 vs109 s9: the plan asserted "I have exactly the mana for Pyroclasm {1}{R}" against
`Mana available {b}{w}{w}{w}` with no red) is SINGLE-SEAT and is really an instance of the fixation event
(ledger #3, headline 1), not an independent core candidate. The core body already carries the governing facts
verbatim (line 4: *"Hybrid symbols like {U/B} can be paid with EITHER of the two colors, and any color can pay
a generic cost"*); a louder/third copy of a present fact is the sanctioned-against escalation. The durable fix
is REPRESENTATION (ledger #6: option-line hybrid annotation + a colored-pays-generic affordability breakdown).
All seven seats independently voted core PASS. The reply-protocol residuals (headline 1) own the code-appended
reply-protocol A/B, never the core body.

`general-strategy.txt` is a byte-identical copy of the live core prompt (`cmp` clean).

---

## 6. POOL-ROTATION verdict — the loop CONTINUES on this pool (rotation NOT triggered)

Per the user's standing rotation directive, three gates:
1. **Did THIS cycle surface no non-context (engine/harness) changes?** — **NO.** The bare-`BLOCKS: none` parser
   fix SHIPPED mid-cycle (working tree, SHIPPED-PENDING-CORPUS), and a substantial engine batch remains
   OUTSTANDING (Force free-alt-cost resolution #1; auto-tap color-stranding #2; the unlisted-fixation
   reconcile-arm #3; multi-copy churn outcome-delta marker #4; hybrid option-line annotation #6). So the pool
   STAYS — the loop continues on THIS set for at least one more cycle after the engine work lands and is
   verified (rotating before then would expose unverified fixes to a fresh pool).
2. **≥2 loop passes on this set?** — **YES**, many (this is wave 11).
3. **General strategy unchanged in the last loop?** — **YES** (core PASS, byte-identical; §5).

Gates 2 and 3 are met, but **gate 1 is NOT** (a fix shipped this cycle + engine items outstanding), so rotation
does not fire. **Verdict: keep the current 7-deck pool; verify the BLOCKS:none fix next corpus and ship the
engine batch (priority: #1 Force resolution, #2 auto-tap, #3 unlisted-fixation reconcile-arm, #6 hybrid
annotation), then re-evaluate rotation next cycle.**

### Resume ordering for the next reviewer
1. Confirm the BLOCKS:none fix landed: grep the next corpus for a `kind==blockers` record whose reply is a
   clean decline and whose next-record combat `events` show NO block of the protected creature (deck131
   Guttersnipe is the regression fixture).
2. Confirm the 240s `WAGIC_GPT_TIMEOUT` landed (empty_reply down from 72; distinguish latency from
   truncation).
3. Confirm the engine batch as it ships: a Force cast where the targeted stack spell EVENTS show a COUNTER (not
   `stack -> graveyard/battlefield`); auto-tap preserving colored sources (vs131 s30 fixture); no scavenged-
   digit mis-casts; the unlisted-fixation reconcile-arm routing to best-available/pass.
4. Re-derive `intent-collapse-metric.py` for answer-last BEFORE any cross-arm count.
5. Watches to hold (do NOT promote without a 2nd instance): reasoning-disobey-via-stack-misconception
   (deck133); first-strike block-math (deck131 H4); Azcanta-transform card-fact (deck135); Obliterator
   punisher-rider USE (still owed at the deck133 seat; guardrail KEPT at full prominence on a 3rd zero-window
   wave).
