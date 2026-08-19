# deck139 -> core-prompt proposals, wave 33

## CORE (general prompt): **PASS -- 28th consecutive.**

No core-prompt change is proposed from this seat. Everything this seat found routes to the guide
layer (the non-Human falsity-fact, the shockland default, the one-blocker-one-attacker
prohibition), the render lane (N-139v creature type), or the instrument lane (N-139x). The core
prompt's completeness guarantee, its legality-is-authoritative framing and its reply protocol were
not contradicted by any surface at this seat this corpus, and the seat produced **zero fallbacks
in 178 decisions**.

One core-adjacent observation, deliberately NOT raised as a proposal: this seat's decisive defect
(vs105 s22, s24 and three siblings) is the pilot overriding the ENGINE'S OWN legality decision --
refusing an offered option as "illegal" and asserting a blocker assignment the prompt forbids two
lines above. The core prompt already tells the model the legal lists are complete and
authoritative; the failure is not that the instruction is missing, it is that the pilot supplies a
card-property premise the surface never gave it. Strengthening the core sentence would be a third
assertion of a fact already asserted twice -- the exact move wave-32 HL6 forbids. The durable fix
is the render (N-139v); the interim is the guide's offer-as-evidence rule. **Recorded, not
proposed.**

---

## OWNER DIRECTIVE 1 -- evidence contribution (no counter-proposal offered)

Per the revised directive, this seat supplies evidence only: no reordering, no brevity clause, no
terminator.

### (a) Counted fields, 178 decision records

| field | count |
|---|---|
| `post_plan_overrun > 0` | **13 / 178 (7.3%)** |
| of those, `commit_retracted = true` | **0** |
| of those, FATAL (cost the game) | **0** |
| `commit_retracted` anywhere | **0 / 178** |
| replies with NO `PLAN:` line (compliant per `f46dd58ee`) | **5 / 178** |
| fallbacks of any kind | **0 / 178** |

Overrun magnitudes: 6,993 / 4,984 / 4,943 / 4,803 / 4,437 / 4,372 / 2,942 / 2,107 / 1,247 / 630 /
484 / 434 / 159 chars. Mean post-answer tail over ALL 178 replies: **216 chars**, an order of
magnitude below the corpus mean of ~1,900 -- this seat is a LOW-overrun seat, and it still burned
its three longest replies (12,145 / 10,667 / 9,264 chars, 188 s / 178 s / 149 s) on scratch text
after a correct answer was already on the page. **This seat contributes a NO to the wave-32
promotion trigger** (zero fatal overrun+retraction) and is therefore not a source of pressure for
the terminator the ruling has already declined to ship.

### (b) Does the 4.2% post-answer self-correction carry decision VALUE that native pre-answer reasoning would need to replace?

**At this seat the rate is 6 / 178 = 3.4%, and the answer is: yes it carries value, and native
pre-answer reasoning replaces it completely -- it does not merely preserve it, it strictly
dominates.**

The six, with the executed answer and its direction:

| game/seq | kind | first answer | executed answer | direction |
|---|---|---|---|---|
| vs146 s13 | ask | `CHOICE: 2 (Cast nothing right now)` | `CHOICE: 1 (Cast Snapdax)` | **BETTER** -- reversed a cast-floor violation |
| vs152 s21 | priority | `CHOICE: 0 (pass)` | `CHOICE: 1 (Draw 1 with Clue)` | **BETTER** -- took a free card |
| vs152 s25 | blockers | `B1:A3, B2:A3, B3:A3` | `B1:A3, B2:A3, B3:A4` | **BETTER** -- spread a wall onto a 2/2, saved 2 life |
| vs152 s27 | blockers | `... blocks Elite Spellbinder` | `... blocks Tovolar's Packleader #1` | **BETTER** -- absorbed 7 instead of 3 |
| vs116 s30 | ask (sacrifice) | `CHOICE: 7 (Pollywog Symbiote)` | `CHOICE: 2 (Sacred Foundry)` | **BETTER** -- kept a creature, gave a land |
| vs158 s28 | blockers | `B1:A1, B2:A2, B3:A2, B4:A3` | `B1:A1, B2:A3, B3:A3` | neutral (re-shuffled a gang-block) |

**5 of 6 improved, 0 of 6 degraded.** The value is real. But read WHERE it comes from: in every
one of the six, the FIRST answer was emitted before the deliberation that corrected it. vs152 s27
is the cleanest specimen -- it opens `BLOCKS: Arboreal Grazer blocks Elite Spellbinder`, then does
the arithmetic in scratch (*"Blocking the 7/7 saves 7 life. Blocking the 3/1 saves 3 life ...
Maximizing saved life = Block the 7/7"*), then re-emits. **The value is in the DELIBERATION, not
in its POSITION after the answer.** Native pre-answer reasoning puts exactly that deliberation
before the commit, where it produces the better answer on the FIRST line -- so the ruling does not
have to replace this 3.4%, it converts it from a lucky recovery into the normal path.

Three points that sharpen the case rather than complicate it:

1. **The current composition makes the recovery a coin flip on the parser.** These six were
   rescued only because the answer-selection happened to take the LATER coded line. The same
   shape with the parser taking the earlier line would have executed the WORSE answer six times
   out of six, silently. The 3.4% is not a benefit of answer-first; it is answer-first's damage
   being partially undone by a parser convention.
2. **The seat's largest historical cost was exactly this pattern.** The wave-32 213 s / 12,845-char
   specimen (vs36 s7) was a reply that answered, then argued with itself for 12,000 characters,
   and its `PUT:` line and its prose named DIFFERENT five cards. Pre-answer reasoning is the only
   arrangement in which a model's deliberation and its commit cannot disagree.
3. **The overrun is not disobedience.** 13/13 overrunning replies still committed, and the five
   no-PLAN replies were compliant. The model is doing what the protocol composition invites. This
   seat's data does not support "the model ignores the terminator"; it supports the owner's
   diagnosis.

### (c) Was any discarded scratch text load-bearing for THIS review?

**Yes, and it does not argue against the change.** Three review conclusions were read out of
post-answer scratch: vs152 s25/s27's damage arithmetic (which is how I established the pilot
understood the board and that the loss was environmental, not a misread); vs116 s30/s31's
annihilator-6 accounting (how I established the sacrifice ordering was deliberate); and the
self-refuting *"Arboreal Grazers and Pollywog are the only ones"* / *"Arboreal Grazer (Human)"*
pair across vs158 s23-s24 that pinned N-139v as a fabrication rather than a misread.

**The requirement this places on the change is already in the ruling: capture native reasoning to
the translog (`reasoning_content`) in the dev loop.** With that, seat review loses nothing -- it
gains, because the reasoning will be attached to the decision it actually produced instead of
trailing the one it did not. If native reasoning were enabled WITHOUT translog capture, this seat
would lose its primary diagnostic instrument; that is the one dependency I would ask the wave-34
implementation to treat as non-optional, and the ruling already states it.

**Requested A/B measurements at this seat**, so the wave-34 validation is falsifiable here:
mutate take-rate on offered windows; count of replies asserting a deck creature is Human; shockland
pay-rate; `coded_answers > 1` count (should go to zero); p95 reply length; >100 s decision count.

---

## OWNER DIRECTIVE 2 (dev-vs-user verbosity split) -- evidence

Supported, with a number from this seat. 178 decisions, mean reply **1,583** chars, total **275 KB of
reply text for six games** -- roughly 46 KB / ~11k output tokens per game of which the executed
answer is under 2%. The three most expensive replies (12,145 / 10,667 / 9,264 chars) are all
wide-board combinatorial decisions and all three were answered CORRECTLY, so this is not a quality
cost to recover -- it is pure user-facing waste under the shipped default, exactly as the owner
frames it. No proposal; evidence only.

## OWNER DIRECTIVE 3 (strip decoration from consumed-decision narration) -- evidence

Measured at this seat: narration carrying `{card text: ...}` appears in **20 of 178** prompts,
against a median prompt size of 5,423 chars. The largest single instance is
vs116's turn-11 sequence, where six consecutive sacrifice decisions each re-narrate the previous
one's full option label. Small, agrees with the ~320-char estimate on a per-line basis, and I found
**no decision at this seat that turned on it**. Supported as a correctness-of-architecture item,
not as a play-quality item.

---

## Cross-seat items this seat can corroborate for synthesis

1. **i1/i2 (the poison pair) is VALIDATED from the OPPONENT'S chair, which is where wave-32 said
   the damage was done.** 4/4 infect blocker windows carry the poison count and the
   poison-denominated forecast; **0/4** carry a `NOT lethal` life-forecast; every infect attacker
   line carries the damage-type clause; non-combat poison narrates. The wave-32 finding that all
   six of deck105's opponents were told a three-way falsehood at the decisive seam is **fully
   remediated at this seat**. And the record confirms it: **deck105 went 6/6 -> 2/6.** The
   asterisk was right; the perfecta was substantially a render artifact.
2. **deck105's G1 (the core prompt's `AIPlayerGPT.cpp:63` "You win by reducing the opponent's life
   from 20 to 0"), HELD pending this corpus -- my evidence says the conflation did NOT persist at
   this seat and I recommend the HOLD be resolved as DO NOT ADOPT for now.** With poison rendered,
   this seat's pilot priced the alt-win correctly and unprompted in every infect window
   (*"Unblocked, they would grant you 6 poison counters, bringing your total to 8 out of 10"*;
   *"sacrificing them is the only way to prevent the lethal accumulation of poison counters"*). It
   lost that game to a blocker-assignment rules error, not to a win-condition misconception. The
   render alone discharged it here. **One seat, one alt-win deck faced; if another seat shows the
   conflation surviving the render, that outranks this.**
3. **The commit-terminator promotion trigger: this seat votes NO** (0 fatal overrun+retractions,
   0 `commit_retracted`), and the ruling has superseded the question anyway.
4. **A pool-composition note synthesis will want when reading every seat's record:** deck158
   3/6 -> **6/6** and deck146 2/6 -> **4/6** in the same corpus where deck105 went 6/6 -> 2/6.
   Three seats moved by 2 or more. Any seat reading its own delta this wave is reading a pool that
   moved under it.
