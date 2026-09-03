# Wave 56 — lane Y (hotfix): the AI proliferated the human's +1/+1 counters

Base: `d61dad89c` (binary-identical to the gated `d0515fae3` — the only diff between them is
a docs file, `strategy-design/wave56/review-carry.md`).

## What the owner saw

Vita play report 2026-09-03, vpk15, tag "bad targeting": *"One match where the opponent
proliferated +1/+1 counters on my creature."*

Transcript `psp-work/logs/20260903-vpk15/transcript-1788458030-player_deck6-vs-ai_baka_deck133.txt`,
lines ~273-281: the AI (deck133, Phyrexian Asphodel) activated Yawgmoth, Thran Physician's
`{B}{B}, Discard a card: Proliferate` **five times in one turn** (`p2.battlefield[5] 1yawgmoth`
+ `p2.choice 0`, x5). None of its own permanents carried a counter; the human's Stonebrow,
Toski and Boartusk Liege all carried +1/+1 counters. Every activation therefore paid two black
mana and a card from hand to make the opponent's board bigger.

Yawgmoth's script is faithful to Oracle (`auto={B}{B}{discard(*|myhand)}:_PROLIFERATE_`,
`_macros.txt`: `name(Proliferate) notatarget(proliferation) proliferate`) — this is an ENGINE
targeting-quality defect, not a card-script defect. No card script was touched.

## Mechanism (read, then reproduced)

* `notatarget(proliferation)` builds a `ProliferateChooser` (`TargetChooser.cpp:265`,
  a `TypeTargetChooser("*")` with `maxtargets = UNLIMITED`).
* `ProliferateChooser::canTarget` had **AI guidance in its PLAYER branch only** (never worsen
  our own poison, never add to an opponent with no poison). Its CARD branch accepted **any**
  in-play card whose counters list was non-empty — either player's.
* `AIPlayerBaka::chooseTarget` sets `playerTargetedZone = 2` for a `ProliferateChooser`, so the
  candidate walk covers BOTH boards, and `AbilityFactory::abilityEfficiency` scores
  `AAProliferate` at 90 whenever it is not player-aimed (AIPlayerBaka.cpp ~851 / ~1541).
  Nothing anywhere asked whether a counter on the board was worth proliferating *for the AI*.

## The rule implemented (in words)

In `ProliferateChooser::canTarget`, card branch only, and only when
`source->controller()->isAI()`:

1. Score each **kind** of counter on the candidate permanent. A counter that carries power or
   toughness is scored by `power + toughness` (so +1/+1 is +2, -1/-1 is -2). A 0/0 named
   counter is scored from two short explicit lists: `loyalty, charge, level, quest, lore,
   shield, oil, experience, energy, verse, study, ki, growth, spore, hoofprint, fade, time,
   page, fate, gold` = +1 (helps its controller); `age, stun, doom, finality` = -1 (hurts its
   controller). Anything unlisted scores 0.
2. Sum those per-KIND scores (not per counter count) — proliferate adds one counter of every
   kind already there, so the marginal effect of the choice is one of each kind. The parser
   builds `AAProliferate` with `allcounters = true` for both `proliferate` and `propagate`, so
   this is exactly what resolves.
3. The permanent is a legal choice for the AI iff: it is **ours** and the sum is `>= 0`, or it
   is an **opponent's** and the sum is `< 0`.

Unknown-counter policy, stated explicitly in the code comment: net 0 on our own permanent is
proliferated anyway (the upside is free, we already paid the cost); net 0 on an opponent's
permanent is declined (when we cannot tell what a counter does, we do not hand it to the other
seat).

`fade` and `time` are in the **good** list on purpose: they are the fading/vanishing counters
(`MTGAbility.cpp` `AVanishing`), and the permanent is sacrificed when one can no longer be
removed, so more of them is more turns. Suspend also names its counters `time`, but a suspended
card sits in exile and `canTarget` already requires `isInPlay`, so an in-play `time` counter is
always the vanishing kind. **This was caught by the suite, not by reading**: the first draft put
`fade` in the harmful list and `generic/proliferate_shroud_i1130.txt` (Blastoderm, fading 3) went
red. Corrected, and `time`/`petal`/`despair`/`wish`/`infection`/`poison`/`paralyzation` were
dropped from that list at the same time as unfounded or already covered by the player branch.

## Why no engine gate beyond the chooser was needed

`AIPlayerBaka::createAbilityTargets` (AIPlayerBaka.cpp ~2234) is the AI's only route to this
activation. For a `maxtargets != 1` chooser it collects candidates with `canTarget`, and
`if(!realTargets.size() ... ) return 0` — returning 0 adds **nothing** to the ranking, so
`rankActivations`/`chooseOrderedAction` never see the ability and it is never activated. The
cost is therefore never paid. Verified empirically by fixture 1: on the pre-fix binary the AI
discards a card and the opponent's creature grows; on the fixed binary the hand is intact and
the ability is never put on the stack. **No efficiency gating at AIPlayerBaka.cpp ~851/~1541
was needed and none was added.**

`MenuAbility` in `AAProliferate::resolve` (the one-kind pick when several counter kinds are
present) is **unreachable from card script today** — `MTGAbility.cpp:5964` and `:5977` set
`allcounters = true` for both `proliferate` and `propagate`, and nothing else constructs an
`AAProliferate`. Mixed counters therefore resolve by the all-counters rule, which is exactly
what the net-sum rule above scores. No change made there; noted rather than over-reached.

## Fixtures

New test primitives in `Res/test/lexicon/test_primitives.txt`:
`2100000600 Lexicon Plus Counter Seed` ({0} sorcery, +1/+1 counter on target creature on either
battlefield), `2100000601 Lexicon Minus Counter Seed` (same with -1/-1), and
`2100000602 Lexicon Proliferate Physician` — a comma-free stand-in carrying **only** Yawgmoth's
proliferate ability. The suite's zone lists are comma-separated, so "Yawgmoth, Thran Physician"
cannot be named by a fixture; the seeds are sorceries (not the existing `{0}` artifact witness)
so nothing lingers on the battlefield for the AI to re-activate after the `ai` switch.

| fixture | on base `d0515fae3` binary | on lane binary |
|---|---|---|
| `w56y_ai_wont_proliferate_opponent_counters.txt` | **RED** — hand 2 -> 1 (a card was discarded), graveyard 1 -> 2, opponent's Grizzly Bears **3/3 -> 4/4** | **GREEN** — no activation, Bears stay 3/3, hand intact |
| `w56y_ai_proliferates_own_good_counters.txt` | GREEN | GREEN |
| `w56y_ai_proliferates_opponent_bad_counters.txt` | GREEN | GREEN |

Fixture 1 is the defect reproduction. Fixtures 2 and 3 are **over-blocking guards** and pass on
both binaries by construction — 2 pins that the AI still proliferates its own +1/+1 counter
(3/3 -> 4/4, one discard), 3 pins that it still proliferates an *opponent's* -1/-1 counter
(2/2 with one -1/-1 becomes 0/0 and dies). They prove the rule is directional, not a ban.

## Gate (worktree, THREADS=1, `systemd-run --user --unit=w56Y-gate2`, MemoryMax=4G)

| leg | result | baseline |
|---|---|---|
| suite | **1234 tests (0 failed), 54 AI tests (0 failed), 0 timeouts** | 1234/0, 51 AI/0, 0 timeouts |
| PARSETEST | **2736 passed, 0 failed** | 2736/0 |

AI count 51 -> 54 = the three new fixtures. `git diff | grep -c $'\357\277\275'` = 0.

## Not verified

* **No live model run.** No GPT/Spark game, no self-play corpus, no translog. The claim that
  this ends the reported behaviour rests on the fixture counterfactual on the Baka path only.
* **No Vita/PSP build.** Nothing was built for the console; the owner's next VPK is the real
  confirmation, and port builds are owner-initiated.
* **No human-seat fixture.** `TestSuiteAI` derives from `AIPlayerBaka`, so `isAI()` is 1 for
  BOTH seats in every suite test — a real human seat cannot be expressed in a fixture. That the
  guidance is AI-only is argued from the code (`Player::isAI()` returns 0, `AIPlayer::isAI()`
  returns 1; the whole block is inside `if(source && source->controller()->isAI())`), not
  demonstrated by a test. The existing scripted-proliferate tests
  (`Contagion_Clasp.txt`, `generic/proliferate_shroud_i1130.txt`) do run through the new rule and
  are green.
* **The GPT/oracle path is untouched and unmeasured.** `LegalActionsOracle` only drops a
  target-chooser ability when `maxtargets == 1`, and proliferate's is UNLIMITED, so an
  `AIPlayerGPT` seat can still be OFFERED the activation with no legal choice on the board (it
  would resolve to nothing). Baka — the seat in the owner's report — cannot. Not changed here;
  docketed as a possible follow-up.
* **The 0/0 counter lists are judgment, not enumeration.** They cover the common cases; an
  unlisted counter kind falls to the net-0 policy above. `lore` (sagas) is classified helpful,
  which is arguable — but for an opponent's permanent helpful and unknown produce the same
  answer (decline), and for our own permanent both produce "proliferate", so the classification
  is only load-bearing inside a mixed-counter sum.
