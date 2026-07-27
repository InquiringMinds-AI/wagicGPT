# deck139 -> development notes (engine / harness / card-script), wave 32

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`.
All seq references are this seat's translogs (`*ai_baka_deck139-*.jsonl`).

---

## CLOSED THIS WAVE (validated live; remove from the open ledger)

- **N-139i -- London bottoming.** CLOSED, corpus-wide not just at this seat. 15 mulligan-keeps ->
  15 bottom asks, one each; `mulligan_count == "you must put N cards" == cards named` in 15/15;
  final hand size 7-N in 15/15; zero re-asks, zero label shifts. Wave-31's signature (engine
  consumes ONE card then re-asks a shrunken hand; vs122 bottomed 4 of a required 7) has no
  instance anywhere. Seat citations: vs36 s7 (5/5, 2-card hand realised at s8), vs105 s4 (2/2),
  vs152 s4 (2/2).
- **N-139j -- 0-power attacker annotation.** CLOSED. `[deals 0 - this attack deals no damage to
  the opponent]` renders on every 0-power attacker; **26/26 attacker windows at this seat declared
  ZERO 0-power creatures**, and nonzero-attacker participation was 26/26 full. The tag is computed,
  not printed-P/T-derived: it fired correctly on `Dryad of the Ilysian Grove (0/2)` after two
  -1/-1 counters (vs105 s19/s33/s37) and the pilot excluded her by name each time.
- **N-139k -- summoning-sick CAN-block clause.** CLOSED. `[summoning sick - cannot attack this
  turn, but CAN block]` 131x at this seat / 851x corpus-wide; the old clause-free form **0x
  corpus-wide**; the paradox pair `[tapped - cannot attack or block this turn] [attacking]` **0x
  corpus-wide** (`[tapped - attacking]` 157x instead). Zero misreads, zero paradox verbalizations.
- **N-139g -- under-card statics.** ANSWERED POSITIVE and now LIVE-CONFIRMED twice: Pollywog's
  `altercost(colorless,-1)` keeps applying while Pollywog is UNDER a merged pile -- vs158 s18
  (Pollywog becomes the under card) then s26 renders `Cast Illuna, Apex of Wishes with its mutate
  cost {2}{u}{u}{r/g}` against `other={3}{RG}{U}{U}`; vs105 s26 then s27 renders
  `Cast Gemrazer {1}{g}` against `mana={3}{G}` (both Pollywogs applying, one buried).

---

## N-139m -- Pollywog + mutate cast: **DID NOT REPRODUCE LIVE. RE-RUN THE FIXTURES BEFORE FIXING.** (HIGH -> re-triage)

**Trigger predicate** (stated before reading, per wave-31 HL4.2): *a creature spell WITH MUTATE is
cast, on either route, while `Pollywog Symbiote` appears in the seat's `Your battlefield:` line.*

**Corpus count: 6 firings. Vanishes: 0. Arrivals: 6.** All six paid a DISCOUNTED cost, fired
Pollywog's `_LOOT_` (a `DISCARD ONE OF YOUR OWN CARDS` ask, twice where two Pollywogs were out),
and arrived as a `[mutated pile]`:

| seq | game | spell | rendered cost vs `other=` | loots | arrival evidence |
|---|---|---|---|---|---|
| s14 T7 | vs146 | Gemrazer | `{g}{g}` vs `{1}{G}{G}` | 1 (s15) | s18 `Gemrazer {2}{g} (4/4) ... [mutated pile - ... 1 card underneath]` |
| s27 T13 | vs146 | Snapdax | `{w}{w}{b/r}` vs `{2}{BR}{W}{W}` (-2) | 2 (s28,s29) | s32 `Snapdax ... (3/5) ... [mutated pile ...]` |
| s21 T10 | vs105 | Gemrazer | `{g}{g}` vs `{1}{G}{G}` | 2 (s23,s24) | s27 `Gemrazer {1}{u} (4/4) ... [mutated pile ...]` |
| s27 T10 | vs105 | Gemrazer #2 | `{g}{g}` | 2 (s29,s30) | s31/s32 host list `Gemrazer #1 ... [your battlefield]` |
| s15 T6 | vs158 | Gemrazer | `{g}{g}` | 1 (s16) | s19 `Gemrazer {1}{u} (4/4) ... [mutated pile ...]` |
| s26 T12 | vs158 | Illuna | `{2}{u}{u}{r/g}` vs `{3}{RG}{U}{U}` | 1 (s27) | s30 `Illuna ... (6/6) ... [mutated pile ...]` |

Resolved-effect check one rung deeper: Snapdax's `auto=@mutated(this):name(Gain 4 life) life:4
controller` RESOLVED (`Your life: 12` at s31 -> `16` at s32); Illuna's reveal trigger RESOLVED
(s30 `kind=reveal`, s31 answered `put in play`); Gemrazer's destroy trigger had no legal target on
any of the four boards (`Opponent battlefield (creatures: 0)` / no artifacts or enchantments).

**Version fact that explains the divergence and dictates the next step.** The probe fixtures ran
on `6ca1f8411`. This corpus ran on `7fabd9bd0`, which is LATER and lands the convoke crash fix
whose commit message is *"doPay/isPaymentSet keying drift left stale payment targets"*. N-139m's
described mechanism (spell pays, cast-trigger fires, spell leaves the stack into no enumerable
zone) is in the stale-payment-record family.

**ACTION, in this order:**
1. Re-run on `7fabd9bd0` (or current HEAD), sequentially, under the probe's
   `systemd-run --scope -p MemoryMax=4G -p MemorySwapMax=0` discipline:
   `bin/Res/test/ai/pollywog_discount_gemrazer_n139h.txt`,
   `pollywog_discount_dirgebat_n139h.txt`, `_control2_gemrazer_normal_discounted.txt`
   (via `WAGIC_TESTSUITE_FILE=test/_probe.txt` -- Res-RELATIVE path; a `Res/test/...` form silently
   runs zero tests).
2. **If green:** N-139m was a fixture-vs-binary staleness artifact. Register all five probe
   fixtures (including the two green controls) into `_tests.txt` as regression tests and CLOSE the
   docket item. Cost: minutes.
3. **If still red:** the finding is the DIVERGENCE, and it localises the defect to the `[AI]`
   fixture harness cast path (scripted menu-pump) rather than the production
   `DecisionManager::planCastSpell` one-click-per-tick path that the live corpus rides 6/6. That
   is a different, cheaper investigation than the one currently docketed at HIGH.
4. Either way, the **normal-cast route of a mutate card with Pollywog out (`_control2`'s shape)
   remains UNEXERCISED LIVE** -- it was offered at this seat (`Cast Gemrazer {1}{g}` / `{2}{g}`)
   and never chosen, because the mutate line is strictly better. If step 3 applies, that route
   needs a probe of its own; the corpus will not produce it.

Do NOT teach around any of this in the guide (engine lane, no-stopgap rule for engine defects).

---

## N-139n (NEW, HIGH) -- the mulligan / bottom hand block has no LAND or GREEN-SOURCE **COUNT**

**Symptom.** Six of this seat's nine mulligans were false against the deck's own keep criterion,
every one of them asserting a colour-source shortage that the rendered hand contradicts:

- vs36 s1, hand contains `Stomping Ground (land: taps for {R}{G})`. Reply: *"critically **zero
  sources of Green mana** ... Stomping Ground ... require a payment of life to produce Green ...
  the hand lacks a single card that can *produce* Green mana (like a Forest, Breeding Pool, or
  Overgrown Tomb)"*.
- vs36 s2, hand contains `Overgrown Tomb (land: taps for {B}{G})`. Reply: *"**zero untapped green
  mana sources** ... there is no Forest, Stomping Ground, Breeding Pool, or any other land that
  taps for Green in the hand"*.
- vs36 s4, hand contains `Stomping Ground`. Reply, self-contradictory in one sentence: *"**zero
  lands that tap for green mana** (Swamp is black only, **Stomping Ground is red/green** but you
  have no way to pay the life ...)"*.
- **vs105 s1**, hand contains a literal `Forest (land: taps for {G})`. Reply: *"3 lands but
  **zero** green mana sources (Forest, Stomping Ground, Temple Garden, Overgrown Tomb, Breeding
  Pool)"* -- it recites the guide's own list while holding item #1 of it.
- vs152 s1, hand contains `Overgrown Tomb`. Reply: *"critically **zero green mana sources** ...
  Overgrown Tomb enters tapped unless I pay 2 life, and even if I pay, it provides {B} or {G}, but
  I have no way to pay the 2 life *and* have mana left"*.
- vs36 s5, hand contains a literal `Forest`. Reply: *"This hand has **zero lands**"* (the mulligan
  itself is correct at 1 land; the stated reason is false).

**Why this is representation and not a guide edit.** The deployed guide already names all five
green lands, already quotes the render substring, and already states the falsity-fact. The pilot
reproduces that list and then excludes a member of it, inventing a disqualifier (the shockland's
"pay 2 life or enter tapped" ETB choice, read as a barrier to PRODUCING mana). Per
representation-beats-instruction a third assertion is not the fix. Note the contrast within the
same prompt: **i11's `[creature]` markers produced ZERO creature-axis misreads at this seat** --
because the creature question the pilot asks ("do I have an early play") is answered by
membership, whereas the land question ("do I have enough green") is answered by a COUNT that no
surface supplies.

**PROPOSAL.** Print an engine-computed count header above the hand in the mulligan ask, the bottom
ask, and (optionally) `CURRENT SITUATION`:

```
Your hand (7 cards): lands 3 (green sources: 1) | creatures 4 | other 0
```

Colour breakdown should be the set the hand's lands can actually produce, computed the same way
the i12/N-158g `Mana available: N total (... colours you can make: {g}{u} ...)` line is computed --
reuse that code path rather than minting a second one, and reuse its wording so the pilot meets
one idiom, not two. Precedent for the shape working: i12's line killed the affordability-denial
class at every seat but one (see N-139u below).

**Expected observable / falsifiable prediction for wave 33:** zero mulligans whose reply asserts
"zero lands" or "zero green sources" while the printed count is >= 1, and this seat's mulligan rate
at or below the field mean.

---

## N-139o (NEW, HIGH, CROSS-SEAT) -- poison counters are **never rendered**; this seat lost a game at 14 life

**Symptom.** deck139 lost vs105 (`matchups-20260727-115847`, game `139v105`) with
`gameend: my_life 14, opp_life 8, turn 13, won false`. It died to poison. The board snapshot
prints only life:

```
--- CURRENT SITUATION ---
Phase: Attackers | It is your turn.
Your life: 14 | Opponent life: 13
Mana available: 2 total (...)
```

There is **no poison field anywhere in the 39 records** of that seat log -- not in CURRENT
SITUATION, not in the narration. The `[infect]` keyword DOES render on the opposing attacker lines
(31 prompts: `A1. Plague Stinger (1/1) deals 1 [flying, infect]`, `A2. Ichor Rats (2/1) deals 2
[infect]`), and the pilot knows the rule -- vs105 s19, verbatim: *"the opponent has two infect
creatures (Plague Stinger and Ichor Rats) that threaten to kill me quickly (**I'm at 14 life, and
10 poison counters lose the game**)"*. It simply has no way to know how close it is, so it made
survival decisions off a life total that was not the clock killing it (vs105 s20 and s34 both
DECLINE blocks on the explicit ground that *"taking 3 damage drops me to 11 life (still safe)"* --
both of those unblocked hits were poison counters, not damage).

**Why it is not a guide item.** An invisible deciding fact is a platform defect wearing a guide
costume. The guide carries an interim tell keyed to the visible `[infect]` tag (block infect
attackers first regardless of life), tagged as a stopgap -- see the stopgap register below -- but
that tell is strictly weaker than the number.

**PROPOSAL.** Render poison counters wherever life is rendered, on both sides, and only when
nonzero (zero-suppressed so no deck pays for a mechanic it never meets):

```
Your life: 14 (poison: 7 of 10) | Opponent life: 13
```

Also narrate each accrual as a first-class event (`You get 2 poison counters (7 of 10)`), the same
way counters are narrated with count+source under i14.

**Cross-seat urgency.** deck105 is the corpus's **6/6 Step-0 perfecta** and it is the pool's infect
deck. Every seat that faces it decides against an unrendered lose-condition. Any seat review of
deck105's opponents this wave is a second witness; recommend the synthesis agent check
deck116/deck146/deck152/deck158 for the same shape before sizing the fix.

---

## N-139s (NEW, MED) -- an ALL-ILLEGAL blocker assignment set exits as a silent `unparsed_reply`, not an i5-style loud re-ask

**Repro:** vs152 s21, T11, `kind=blockers`, `fallback=unparsed_reply`, `choice=0`, reply 8,171
chars. The offered lines:

```
A5. Elite Spellbinder (3/1) deals 3 [flying]
B1. Pollywog Symbiote (1/3) - may block A1 (you kill it, your blocker lives), A2 (neither dies),
    A3 (both die), A4 (both die)
```
The reply's head line is FORM-compliant -- `BLOCKS: Pollywog Symbiote blocks Elite Spellbinder` --
but names the one attacker B1 is not offered against (Pollywog has neither flying nor reach). The
parser dropped the illegal assignment, which left nothing, and exited to the heuristic.

**The parser is correct.** The observation is about the EXIT. i5 shipped a loud re-ask for
validation rejects at the cast seam (`[RE-ASK n]`, cap 2, then Baka); the blockers seam has no
equivalent, so a reply whose every assignment is illegal is indistinguishable in the log from a
garbled reply, and the pilot never learns which attacker was out of reach. Suggested shape,
mirroring i5: on a `BLOCKS:` line that PARSED structurally but whose assignments were ALL dropped
as illegal, re-ask once with a one-line reason (`<blocker> cannot block <attacker> - it has no
flying or reach`), then fall through to the heuristic.

**Severity vs repro quality (recorded separately, per wave-31 HL4.3):** severity MED (the heuristic
answered and the game was already lost by the previous decision -- see findings 6c); repro quality
**HIGH** -- one record, fully determined by the printed option set, no timing dependence.

---

## N-139q (NEW, MED, CARD-SCRIPT) -- Nethroi's mutate trigger is far more permissive than Oracle

Primitive (`borderline.txt`):
```
name=Nethroi, Apex of Death
auto=@mutated(this):name(Return creatures) target(<anyamount>creature[power<=10]|mygraveyard) moveTo(myBattlefield)
```
This returns **any number of creatures, each with power <= 10**. Real Oracle is *"return any
number of target creature cards with **total** power 10 or less from your graveyard to the
battlefield."* The engine has no total-power budget at all, so Nethroi is strictly stronger than
the card. Card-data lane (`Res/sets/primitives/borderline.txt`), not engine code. The deployed
guide asserted the Oracle number and has been softened to a generic, oracle-true description
pending the data fix (per the wave-23 oracle-vs-engine-behaviour fork). Unexercised this corpus --
Nethroi's mutate line was never offered.

---

## N-139r (NEW, LOW, DATA QUESTION) -- `@mutated(mytgt)` hook present on five mutants, absent on two

Gemrazer, Migratory Greathorn, Nethroi, Vadrok and Everquill Phoenix each carry BOTH
`auto=@mutated(this):...` and `auto=@mutated(mytgt):...`. **Snapdax and Illuna carry only
`@mutated(this)`** (verified by grep over `borderline.txt`). If `mytgt` is the hook that fires when
the card mutates UNDER an existing creature, then Snapdax's and Illuna's triggers are silently
dead on the under route. I did not read the engine, so this is a question, not a claim: what does
`@mutated(mytgt)` resolve to, and is the two-card asymmetry intentional? The guide has been changed
to mandate `mutate over` as an absolute, which is correct under either answer, so nothing is
blocked on this.

---

## N-139p (NEW, LOW) -- the bottom ask drops its hand-size sentence at N=7

At 7 mulligans the prompt's *"You will be left with a N-card hand, so keep your best N and bottom
your worst M"* clause is absent (deck146 vs158 s8, deck146 vs139 s8, deck116 vs158 s8 -- all
7-mulligan keeps, i.e. a 0-card hand). The count instruction itself is correct in all three and
all three were answered correctly, so this is cosmetic. Presumably a zero-case guard in the
sentence builder.

---

## N-139t (NEW, LOW) -- the loot's DISCARD header attributes its source inconsistently

`DISCARD ONE OF YOUR OWN CARDS: your own effect (**Pollywog Symbiote**) makes you discard ...`
(vs146 s15) vs `... your own effect (**Gemrazer**) makes you discard ...` (vs105 s29). The ability
belongs to Pollywog; Gemrazer is the mutate SPELL that triggered it. Both were answered correctly,
so decision-neutral, but the second form invites the false belief actually observed in the replies
(*"casting it triggers its own ability (since it has mutate)"*, vs158 s9; same at vs146 s12 --
Pollywog's `abilities=` field is empty and it has no mutate). Naming the ability's SOURCE
permanent consistently would remove the invitation.

---

## N-139l (RECURS, LOW, unchanged) -- merged-pile cost pip drift

After merging, the pile renders the cost of whatever card ended up carrying it rather than the top
card's own cost: `Gemrazer {2}{g} (4/4)` (vs146 s18) and `Gemrazer {1}{u} (4/4)` (vs105 s27,
vs158 s19 -- `{1}{u}` is Pollywog's cost, the under card) where the pre-merge cast line read
`Cast Gemrazer {3}{g}`; `Illuna, Apex of Wishes {1}{u} (6/6)` (vs158 s30). Third corpus running.
No decision affected in any of the six merges. LOW.

---

## N-139u (OBSERVATION against i12, not a new item) -- one count-first mana-line counterexample

i12's prediction was zero "I cannot afford" declines of offered payable casts. One occurred at
this seat, in a game it WON, outcome-neutral. vs146 s21, Main phase 2, with
`Mana available: 3 total (three untapped sources, tapped automatically when you cast; colours you
can make: {g}{u}{r}{b}{w})` printed and two payable creature casts offered, the reply declined:
*"I have no untapped lands left to pay for any of the cards in my hand ... my only available mana
is generic or colored but insufficient"*. Reported for the synthesis agent's cross-seat tally
only; a single non-outcome-affecting instance is not grounds to re-open the representation.

---

## STOPGAP REGISTER (guide lines, their defect, their removal condition)

| guide line | defect it patches | removal condition |
|---|---|---|
| *"An attacker tagged [infect] does NOT reduce your life - it gives you poison counters, and 10 poison counters lose you the game at any life total. Your poison count is not printed anywhere, so ... block the infect attackers FIRST"* | **N-139o** -- poison counters absent from the board snapshot | DELETE once `Your life: N (poison: M of 10)` renders; then re-grep the next corpus for the "still safe at 11 life" reasoning shape to confirm it stays at zero |
| *"The thought 'Stomping Ground / Temple Garden / Overgrown Tomb / Breeding Pool cannot make green because I must pay 2 life' is ALWAYS FALSE ..."* | **N-139n** -- no green-source count; the shockland ETB clause read as a mana-production gate | DELETE the shockland clause (keep the two falsity-facts above it) once the hand-count header ships and one corpus shows zero false-colour-shortage mulligans |
| *"THE FLOOR IS THE NUMBER PRINTED IN FRONT OF YOU ... 'having already taken N mulligans' ... this is your LAST look"* | not an engine defect -- a wording fix for a deferred internal counter (skill.md P2) | none; permanent guide content |

## HARNESS / CORPUS HYGIENE (no action needed)

- Crash gate: **0** signatures (`segmentation|SIGSEGV|assert|abort|FATAL|core dumped|terminate
  called`) across all six deck139-seat `.stderr` files. Third consecutive clean corpus at this seat.
- `fails validation; deferring to heuristic`: **0** occurrences in all six seat stderr files.
- `validation_reject_reask` / `validation_reject_exhausted`: **0** at this seat and **0
  corpus-wide** -- i5 is still live-validated only by the deck198 convoke probe, as the brief
  states.
- Latency at this seat improved on every statistic (median 15,601 ms, p90 58,896, >195 s count
  1 vs wave-31's 6). i13's prompt-growth cost risk did not materialise here: reply p95 fell from
  the ~13 k tail cluster to 6,393 chars.
