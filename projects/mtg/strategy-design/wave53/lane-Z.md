# Wave-53 lane Z — Vita vpk12b: five `bad blocking` games, round three

Base: master `baf36b936` (wave-53 lanes M-X merged + the reviewer-guide deploy).
Branch `w53-lane-Z`. Baseline re-measured on this worktree BEFORE any change:
suite **1216 tests (2 failed) + 47 AI (0 failed), 0 timeouts**; PARSETEST
**2258 passed / 0 failed**. The two failures are the known concurrency-only pair
`lifeline.txt` + `merrow_reejerey.txt`.

The engine in the owner's vpk12 build carries lane T (the aggregate-lethal
SURVIVAL sweep) and does **not** carry lane U (the deathtouch gang fix, merged
to master after that VPK was cut).

---

## Summary of verdicts

| # | transcript | the bad block | class |
|---|---|---|---|
| 1 | 1788388578 deck1 vs deck72 | 3 blockers piled on a 2/3, the lethal Hydra unblocked | **NEW** — pass 3 never aimed its click |
| 2 | 1788390984 deck7 vs deck2 | 2nd blocker stacked on an already-dead 1/1, 4 attackers loose | **NEW** — same defect |
| 3 | 1788392029 deck7 vs deck10 | 1 blocker on a trampler, 3 free bodies unused | **NEW** — lane T's sweep cannot top up a trampler |
| 4 | 1788394118 deck3 vs deck3 | one ground blocker on the one ground attacker | **correct / unavoidable** (arithmetic below) |
| 5 | 1788397358 deck1 vs deck36 | no blockers declared at all | **forced** — the defect is upstream in `chooseAttackers` |

Two engine fixes ship, both in `AIPlayerBaka::chooseBlockers`, each with a
fixture RED on base and GREEN after. Game 4 is discharged with arithmetic.
Game 5's cause is located, proved, and **docketed, not fixed** (it is not in
`chooseBlockers`).

---

## The instrument: what a click record can and cannot say

A `p2.battlefield[i] 1<name>` line in the blockers window is **one click**, and
a click does not choose an attacker — `MTGBlockRule::reactToClick` CYCLES the
blocker one step through the attackers it may legally block
(`getNextAttacker(current)` until `toggleDefenser` accepts), and off again at
the end of the list. So with `L` legal attackers a blocker has `L+1` states and
`N` clicks land it on state `N mod (L+1)`. That arithmetic is what lets the runs
below be read back into declarations, and it is also the defect: see fix 1.

Two things the record does **not** contain, both found here:

* **Blocks declared through `toggleDefenser` are never logged.** Only
  `GameObserver::cardClick` calls `logAction`. Lane T's survival sweep, the
  W43-1 CR 509.1c repair sweep and (now) the trample top-up all call
  `toggleDefenser` directly, so their assignments are invisible in the
  transcript **and are not replayed**. This is why fix 1 below stays on the
  clicking path rather than taking the shorter `toggleDefenser` route.
  Docket item: route the sweeps through a logged path.
* **Replay cannot reach any of these combats.** All five dumps were replayed
  (`WAGIC_REPLAY`, memory-capped, `timeout 300`) and all five diverged long
  before the reported turn:

  | transcript | diverges at | expected / got | chooser |
  |---|---|---|---|
  | 1788388578 | action 223, turn 12 | `p1.hand[2] 1swamp` / `p1.choice 0` | read the bones |
  | 1788390984 | action 72, turn 4 | `p1.zone[34] 1forest` / `p1.choice 0` | harrow |
  | 1788392029 | action 55, turn 4 | `p1.hand[4] 1forest` / `p1.next 1 3` | none |
  | 1788394118 | action 27, turn 2 | `p1.zone[0] 1stratus dancer` / `p1.choice 0` | delver of secrets |
  | 1788397358 | action 90, turn 6 | `p1.hand[2] 1swamp` / `p1.next 1 3` | none |

  Three are the chooser-window class lane U hit (a scry/search/reveal window
  resolves in a different order under replay, so the record's next click arrives
  one action early). Two are the "no click was pending" class — the replay
  reached a phase-advance where the device had a click. The player decks were
  fetched from the Vita (`ftp://10.0.0.227:1337/ux0:/data/wagic/player/deckN.txt`,
  one connection each) and installed for the attempt, then removed and the
  worktree's own `bin/User/player/deck1-3.txt` restored. Boards below are
  therefore read from the transcripts and the card definitions, exactly as in
  lanes T and U.

---

## FIX 1 — pass 3 commits with a BLIND click, so the block lands on attacker #1

### Games 1 and 2

**Game 1** — `transcript-1788388578-player_deck1-vs-ai_baka_deck72.txt`,
`#result winner=p1 turn=16 life=26/0`, `#classification=bad blocking`.
Human p1 = deck1 "Fangbearer Grind". Final combat, p1 declares **two**
attackers: `p1.battlefield[5] 1winding constrictor` (2/3) and
`p1.battlefield[7] 1hungering hydra` (`abilities=oneblocker`, `auto=counter(1/1,X)`
plus enrage counters — the seat's actual killer). The AI seat has exactly three
untapped bodies and clicks:

```
odious trow  (bf[5])  x3   then x1
rendclaw trow(bf[6])  x3   then x1
rendclaw trow(bf[8])  x3   then x1
```

Two attackers = three states. The **x3** runs are pass 1 walking Constrictor →
Hydra → unassigned and refusing both (`AIStats::isInTop` is the gate, and pass 2
never sees them). The trailing **single** click on each is pass 3. One click
from unassigned lands on the FIRST attacker in the attacking player's
battlefield order — the Constrictor at index 5, not the Hydra at index 7. So
**all three Trolls blocked a 2/3 and the Hydra was unblocked**, and the seat
died at 0.

Note what that also costs: lane T's survival sweep runs next, correctly computes
`unblockedDamageTo == the Hydra's power >= life`, picks the Hydra as its chump
target — and then finds **no free body**, because pass 3 had already spent all
three on the wrong attacker. The sweep was starved, not wrong.

**Game 2** — `transcript-1788390984-player_deck7-vs-ai_baka_deck2.txt`,
note `turn=14 life=9/-28`, `#classification=bad blocking`. Final combat, p1
attacks with five: Experiment One (bf[0]), Go-Shintai of Boundless Vigor (bf[5],
trample), Predator Ooze (bf[6], indestructible), Renata (bf[7], `power=*`),
Rubblebelt Raiders (bf[8], 3/3). The AI clicks
`earth elemental (bf[7]) x1`, then `elvish archers (bf[10]) x7`.

Five legal attackers = six states. **x1** = pass 1 accepting Experiment One
(4/5 into a 1/1: tracked toughness goes negative, so pass 2 keeps it). **x7** =
six clicks of a full pass-1 reject cycle (the Archers cannot take Experiment
One any more — its tracked toughness is already ≤ 0) **plus one pass-3 click**,
which lands right back on Experiment One. Two blockers on a 1/1 that one of them
had already killed, and Raiders / Go-Shintai / Ooze / Renata all unblocked.

The pass-3 guard that should have stopped this — `currentBlockers >= maxBlockers`
— *did* run; it just ran against `bestAttacker`, which is not where the body
went.

### The mechanism

`chooseBlockers` pass 3 scores a `bestAttacker` carefully (kill / survive-and-
reduce / prevent-lethal branches, `isInTop` bonuses) and then commits with

```cpp
observer->cardClick(card, MTGAbility::MTG_BLOCK_RULE);
opponentsToughness[bestAttacker] -= card->power;
```

one blind click. It never checks where the blocker landed, and it decrements the
tracked toughness of an attacker that may have received nothing. Every
per-attacker cap in the pass is therefore evaluated on a different card than the
one that gets the blocker, which is why the pile-up has no ceiling. The same
blind commit is used for the menace `extraBlockers`.

### The change (`src/AIPlayerBaka.cpp`)

* new file-static `aimBlockerAt(observer, blocker, target)` — clicks until
  `blocker->defenser == target`, capped by the attacker count; if it never
  lands (an evasion the scorer missed) it clicks the body back to unassigned
  and reports failure, so nothing is left on an attacker nobody chose. It stays
  on `cardClick` deliberately: that is the logged, replayable path.
* pass 3's scoring loop now skips attackers this body cannot legally block
  (`card->canBlock(attacker)` — the engine's own gate, incl. evasion, ONEBLOCKER
  and the CR 509.1c feasibility half). The blind click made that filter
  unnecessary before; an aimed one needs it, and it is the right question anyway.
* both commits (the scored blocker and each `extraBlocker`) go through
  `aimBlockerAt`; the `opponentsToughness` bookkeeping only happens when the aim
  succeeded. `extraBlockers` candidates are filtered by `canBlock(bestAttacker)`
  too, so a menace set is legal per member.

### Fixture — `bin/Res/test/ai_block_aim_scored_attacker_w53z.txt`

Mons's Goblin Raiders (1/1) and Craw Wurm (6/4) attack a defender at **6**
holding three Grizzly Bears (2/2). The Raiders are FIRST on the battlefield.
The Wurm alone is exactly lethal (6 ≥ 6) and no Bear kills it (2 < 4), so pass 3
scores the Wurm 6 and the Raiders 3 — the Wurm is the pick for every Bear.

* **RED on base** (measured by reverting `src/AIPlayerBaka.cpp`, rebuilding and
  running scoped): `phase problem. Expected [Main phase 1](4), got [Combat
  damage](8)`, `life problem for player 1. Expected 6, got 0`, defender graveyard
  `expected 1, got 0`, defender battlefield `expected 2, got 3` — all three
  Bears on the Raiders, the Wurm unblocked, the defender dead at 0.
* **GREEN after**: Bear 1 aimed at the Wurm (Bear dies, Wurm lives), Bear 2 sees
  the Wurm capped and takes the Raiders (Raiders die, Bear lives), Bear 3 finds
  every attacker capped and stays home. Defender ends at **6**.

---

## FIX 2 — the survival sweep cannot top up a TRAMPLER

### Game 3

`transcript-1788392029-player_deck7-vs-ai_baka_deck10.txt`, note
`turn=12 life=15/0`, `#classification=bad blocking`. Final combat: p1 attacks
with **Mossborn Hydra alone** (`abilities=trample`, 0/0 + counters; deck7 is a
counters deck running Karn's Bastion and Pridemalkin). The AI seat clicks

```
hearthfire hobgoblin (bf[3]) x2  ... then x1
goblin mountaineer   (bf[4]) x2
goblin mountaineer   (bf[6]) x2
hearthfire hobgoblin (bf[8]) x2
```

One attacker = two states, so every **x2** is pass 1 assigning and taking back.
The single trailing click on bf[3] is pass 3. Declared blocks: **one** Hearthfire
Hobgoblin (2/2 **double strike**). Three untapped bodies — a second 2/2 double
striker and two 1/1 Goblin Mountaineers, **4 more toughness** — stood and
watched, and the trample overflow took the seat to exactly 0.

Nothing about the outcome makes those three bodies worth keeping: the seat lost
the game that combat. And the two Hobgoblins are double strikers — two of them
blocking deal 2×(2+2) = 8 to the Hydra on top of absorbing 4.

### The mechanism

Lane T's sweep computes the right number — `unblockedDamageTo` is trample-aware
and adds `power - absorbed` for a blocked trampler — but its **target search**
opens with

```cpp
if (atk->power <= 0 || atk->blockers.size())
    continue;
```

so the moment pass 3 puts one body in front of a trampler, the sweep can see the
damage and cannot reach the attacker carrying it. Target becomes NULL, the loop
breaks, and the seat dies with bodies to spare. Lane T named this gap in its own
"what I did NOT verify" section and docketed it; this is it closing.

### The change (`src/AIPlayerBaka.cpp`)

The sweep's target search now ranks by *damage this attacker currently gets
through*, and admits an already-blocked attacker when — and only when — it has
TRAMPLE and its blockers' combined toughness does not cover its power. Unblocked
attackers keep the menace / feasibility skip they had (a partial set declaration
there is pass 3's job and would only be deleted by the rules layer); a trample
top-up needs no such guard because the minimum is already met. Legality of the
added body still goes through `canBlock()` + `canBlockPairwise(target)`, which is
where ONEBLOCKER is enforced (`addBlocker` sets `blocked`), so a
"can't be blocked by more than one creature" attacker is never topped up.

### Fixture — `bin/Res/test/ai_block_trample_topup_w53z.txt`

Colossal Dreadmaw (6/6 trample) attacks a defender at **4** holding three Mons's
Goblin Raiders (1/1). Pass 3 blocks with one Raider (prevent-lethal branch) and
then caps out at `maxBlockers` 1: absorbed 1, overflow 5 ≥ 4.

* **RED on base**: `life problem for player 1. Expected 1, got -1`; defender
  graveyard `expected 3, got 1`; defender battlefield `expected 0, got 2` — one
  blocker, 5 trample through, dead at -1.
* **GREEN after**: the sweep adds the other two Raiders (absorbed 3, overflow 3,
  which 4 survives) and stops; 3 tramples through, defender at **1**, all three
  Raiders in the graveyard.

---

## GAME 4 — the block was correct and the game was already lost

`transcript-1788394118-player_deck3-vs-ai_baka_deck3.txt`,
`#result winner=p1 turn=10 life=19/-3`, `#classification=bad blocking`.
Human p1 = deck3 "Merkfolk" (mono-U).

Final combat, p1 attacks with **Insectile Aberration 3/2 flying**, **Stratus
Dancer 2/1 flying** and **Talrand, Sky Summoner 2/2** (no evasion). The AI seat
has exactly one untapped body, `jukai messenger (bf[7])` — 1/1 forestwalk,
ground. It clicks three times. Jukai Messenger has exactly **one** legal
attacker (both Aberration and Dancer fly), so its cycle is two states: two
clicks are pass 1 assigning and refusing, the third is pass 3 putting it back on
Talrand. **It blocked the only attacker it was allowed to block.**

The arithmetic that makes this unavoidable, read forward off the transcript's
combats (p2 starts at 20):

| combat | attackers | blocked? | p2 life after |
|---|---|---|---|
| turn 4 | Delver of Secrets 1/1 | no blocker declared | 19 |
| turn 6 | Delver of Secrets 1/1 | no blocker declared | 18 |
| turn 8 | Aberration 3/2 F, Aberration 3/2 F, Stratus Dancer 2/1 F | none | 10 |
| turn 10 | same three fliers | 1 click each on Jukai Messenger and Grizzly Bears = **no legal attacker**, no block | 2 |
| turn 12 | Aberration 3 F + Dancer 2 F + Talrand 2 | Talrand blocked | **-3** |

At 2 life facing **5 power in the air** and holding only ground blockers, every
declaration loses. Blocking Talrand was strictly the best of them: it saved 2
(the seat would have been at -5 otherwise) and traded a 1/1 into a 2/2 that dies
to 1 damage... in fact Jukai Messenger's 1 power does *not* kill Talrand, so the
block is a clean chump, which is exactly right when you are dead either way.
**No fix; no hole.** The `bad blocking` tag on this game is, on the evidence, a
misattribution of a game lost in the air three turns earlier.

---

## GAME 5 — the block was FORCED empty; the defect is in `chooseAttackers`

`transcript-1788397358-player_deck1-vs-ai_baka_deck36.txt`,
`#result winner=p1 turn=14 life=43/-12`, `#classification=bad blocking`.
Human p1 = deck1 (B/G deathtouch + lifelink, hence 43 life); AI p2 = deck36,
artifacts.

Final combat, p1 attacks with five (Vampire Nighthawk 2/3 flying deathtouch
lifelink, Gifted Aetherborn 2/3 deathtouch lifelink ×2, Mire Triton 2/1
deathtouch, Wildwood Scourge). The AI seat issues **zero** blocker clicks. That
is not a decision — pass 1 clicks every untapped creature with power > 0 at
least once, so zero clicks proves the seat had **no untapped body with power**.
What it did have:

* **Steel Wall 0/4 defender** — cast that same turn cycle and killed by p1's
  Murder in first main (`p1.hand[2] 0murder` → `p2.battlefield[11] 1steel wall`).
* **Ornithopter 0/2 flying ×2** — the only bodies left, and both had **attacked
  on p2's immediately preceding turn** (`p2.battlefield[1] 1ornithopter`,
  `p2.battlefield[9] 1ornithopter`), so both were tapped. They deal **0 damage**;
  the transcript shows p2 attacking with them on four separate turns.

An Ornithopter is the seat's only answer to a 2/3 flier. Attacking with it
converts a chump block into nothing at all, four times over.

`AIPlayerBaka::chooseAttackers` has no power gate: its `attack` branch is
literally "attack with all creatures", and `shouldAIForceAttack` even returns
**true** for any flier when the opponent shows no untapped flier or reacher —
which is how a 0/2 gets sent in. This is a real hole and it is the cause of the
owner's fifth `bad blocking` tag, but it is **not in `chooseBlockers`**, so this
lane docketed rather than fixed it: a `power >= 1` guard (or an
"unless it has an attack trigger" form of one) in `chooseAttackers` touches every
AI game in the suite and belongs in a lane that can gate it properly.
**Docket item W53-Z/D-a: Baka attacks with 0-power creatures and loses the block.**

---

## Files

* `projects/mtg/src/AIPlayerBaka.cpp` — `aimBlockerAt` + pass-3 legality filter +
  aimed commits (fix 1); survival-sweep trample top-up (fix 2).
* `projects/mtg/bin/Res/test/ai_block_aim_scored_attacker_w53z.txt` (new)
* `projects/mtg/bin/Res/test/ai_block_trample_topup_w53z.txt` (new)
* `projects/mtg/bin/Res/test/_tests.txt` — both registered.
* `projects/mtg/bin/Res/test/ai/perimeter_captain_may_batch_w41.txt` —
  **re-baselined**, see below.

### The one existing pin that moved

`ai/perimeter_captain_may_batch_w41` expected the defending AI to gang-block one
of three Raging Goblins with all three of its walls and take 2 from the other
two (life 24). With aimed blocks it blocks **all three** Goblins one apiece and
takes 0 (life 26). Verified at the mechanism, not inferred: the `Defenser
Toggle` trace shows three DISTINCT attacker addresses receiving the three
defenders. The property the fixture exists to pin — three identical `may`
triggers in one window, all accepted, +2 each — is unchanged and still
discriminating (one trigger 22, two 24, three 26); only the combat damage
changed, and it changed in the defender's favour. Expected life updated to 26
with the reason recorded in the fixture's own header.

## Gate (this worktree, memory-capped, `-p MemoryMax=4G -p MemorySwapMax=0`)

| | baseline (base source) | after |
|---|---|---|
| suite | 1216 tests, 2 failed; 47 AI, 0 failed; 0 timeouts | **1216 tests, 2 failed; 49 AI, 0 failed; 0 timeouts** |
| PARSETEST | 2258 passed / 0 failed | **2258 passed / 0 failed** |

The two failures are the known concurrency-only pair `lifeline.txt` +
`merrow_reejerey.txt`. AI 47 → 49 is this lane's two fixtures. Every named
blocking pin re-run individually and green in one scoped batch (15/15):
`ai_block_lethal_set_wave53` (T), `ai_block_deathtouch_gang_w53u` (U),
`blocker_one_attacker_only_w53m` (M), `argothian_enchantress` (deck62 N8),
`menace_defender_{one_blocker,two_blockers,ai_control_vanilla}`,
`plague_wight_blockers_shrink`, `trample_vs_{ordinary,protection}_blocker`,
`nontrample_vs_protection_blocker`, `combat_block_after_attack_trigger`,
`ai/perimeter_captain_may_batch_w41`, plus both new fixtures.
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. No PARSETEST cases added:
nothing here touches a parsed or rendered string.

## Falsifiable predictions

1. In the next Vita session, no transcript shows two or more of the AI seat's
   blockers ending on the same non-menace attacker while another attacker is
   unblocked. (The remaining legitimate multi-block is pass 1's kill gang, which
   lane U capped only for deathtouch.)
2. No transcript shows the AI seat holding untapped bodies while a trampler's
   overflow kills it.
3. If the owner still reports a bad block after this, it is either (a) pass 1's
   general missing per-attacker cap — a cost-blind gang on a NON-deathtouch
   attacker, live only in long games where `AIStats::isInTop` has three entries —
   or (b) the 0-power attacker item above emptying the block before it happens.

## What I did NOT verify

* **No replay reached any reported combat** (five divergences, table above), so
  every board here is read from the transcript click record plus the primitives,
  not reconstructed live. The x3/x1 click decomposition into pass 1 / pass 3 is
  an inference from the code's clicking pattern and the legal-attacker count; it
  is consistent with all five records and with lane U's independent reading, but
  no run of any kind observed those decisions (`chooseBlockers` does not run
  during a replay — the AI's clicks are in the record).
* **Game 3's exact life-before is not recoverable** from the dump, so I claim
  only that three free bodies worth 4 more absorbed damage were left unused in a
  combat the seat lost; I do not claim a specific surviving life total for that
  game. The fixture proves the mechanism and the direction, not that game's
  counterfactual.
* **Game 2's counterfactual is not claimed either.** The fix gets a real attacker
  blocked instead of a second body on a corpse; whether that changes a game the
  seat lost by 28 points is not something the record can answer.
* **No Vita rebuild, no device run, no corpus or selfplay shakedown.** Desktop,
  headless, suite-level only. No win-rate measurement of either change.
* I did **not** fix the 0-power attacker hole (game 5), and I did not measure
  what a `power >= 1` guard in `chooseAttackers` would do to the AI suite.
* I did **not** route the `toggleDefenser` sweeps onto a logged path, so the
  survival sweep, the W43-1 repair sweep and the new trample top-up still write
  no blocker clicks into the transcript and are not replayed. Fix 1 stays on the
  clicking path for exactly this reason, but the sweeps' invisibility is a
  standing hole in the owner's own diagnostic instrument.
* Nothing here touches `AIPlayerGPT`, `MTGRules.cpp`, the render, or any card
  script; the LLM seat changes only in that its heuristic fallback blocks better.
* The Vita player decks fetched over FTP were used only for the replay attempt
  and were removed afterwards; `bin/User/player/deck1-3.txt` are byte-restored
  from the pre-lane copies.
