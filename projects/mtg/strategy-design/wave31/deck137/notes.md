# deck137 wave-31 -- development notes (engine / harness)

Binary `cf7cb2e53`, corpus `matchups-20260727-025026`. Every item below carries a seq repro.
Layer: all ENGINE/HARNESS. No guide or core-prompt content here.

--------------------------------------------------------------------------------
## N-137a (HIGH, **NEW**) -- Convoke can only pay GREEN pips or GENERIC; four of five colours are unreachable
--------------------------------------------------------------------------------
**Code fact (verified).** Two loops share one shape, and both terminate the colour scan on the
FIRST index because BOTH branches set `next = true`:
- `convokeReduceByCreatures`, ExtraCost.cpp:1241-1267 (the OFFER probe, used by `Convoke::offerable`)
- `Convoke::getReduction`, ExtraCost.cpp:1178-1231 (the REAL payment, used by `Convoke::doPay`) --
  both the forward pass and the backward retry

```
for (int i = Constants::MTG_COLOR_GREEN; i <= Constants::MTG_COLOR_WHITE; ++i) {
    if (next) break;
    if ((c->getManaCost()->hasColor(i) || c->hasColor(i)) && toReduce->hasColor(i)) {
        toReduce->remove(i, 1); next = true;
    } else {
        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, 1); next = true;   // <-- exits the scan
    }
}
```
`MTG_COLOR_ARTIFACT = 0, MTG_COLOR_GREEN = 1, ..., MTG_COLOR_WHITE = 5`
(include/MTGDefinitions.h:100-105). Index 1 (GREEN) is the only colour ever examined; 2..5 are
dead. CR 702.51 says each convoking creature pays `{1}` **or one mana of that creature's colour**.
The existing wave-30 comment above `convokeReduceByCreatures` DESCRIBES this behaviour ("a
creature reduces the GREEN pip when it is green ... otherwise it reduces one generic") as
intentional mirroring -- it was never flagged as the rules violation it is.

**Live repro.** deck137 vs146, **s20 (t8)** and **s24 (t10)**.
- s20 board: `Mana available: {g} from 3 untapped sources`; `Forest #1/#2/#3` untapped;
  `Human #1 (1/1)` and `Human #2 (1/1)` untapped (WHITE tokens -- the engine's own adventure
  render for Lovestruck Beast reads `{adventure spell: Create a 1/1 white Human creature
  token.}`); `Lovestruck Beast (5/5)` untapped. Hand holds **2x** `March of the Multitudes
  {g}{w}{w}{x}` (`mana={X}{G}{W}{W}`, borderline.txt).
- By CR: Human + Human convoke `{W}{W}`, a Forest pays `{G}`, two Forests give X=2 -> two 1/1
  lifelink Soldiers. Legal and strong.
- Actual offered list: `1. Cast Lovestruck Beast // Heart's Desire {2}{g} (5/5) [from exile]`,
  `2. Cast nothing right now`. **No March line at all.** Identical at s24 (4 Forests + 2 Humans).

**Consequence.** A `{X}{G}{W}{W}` convoke spell is offerable only when the LANDS alone can make
both `{W}`. That is why `{4}{W}` Venerated Loxodon (one coloured pip) convokes fine (vs158 s21,
vs122 s30 both arrived) while March effectively never does.

**Fix direction.** Do not `break` the colour scan on the generic fallback: try every colour index
the creature has against the remaining cost first, and only then charge a generic. Apply the same
change to BOTH functions (they must stay in agreement -- the probe's own comment mandates it), and
add a fixture: an all-Forest board + 2 white 1/1 tokens + March in hand must OFFER convoke and
resolve at X>=1.

--------------------------------------------------------------------------------
## N-137b (HIGH, **NEW**) -- Divergence A's fix is PATH-SCOPED: `maxAnnounceableX` is not called by either MTGRules announce path
--------------------------------------------------------------------------------
**Code fact (verified).** `ManaEngine::maxAnnounceableX` (ManaEngine.cpp:640), where cf7cb2e53
added the convoke creature-credit, has exactly three references: `AllAbilities.cpp:8209`
(MenuAbility optional-cost announce), `AIPlayerBaka.cpp:2644`, and its declaration in
`include/ManaEngine.h:103`. Both MTGRules announce paths compute the X option count from the
FLOATED POOL only:
- normal cast, MTGRules.cpp:433 -- `(playerMana->getConvertedCost() - cost->getConvertedCost()) + 1`
- **alternative cost, `MTGAlternativeCostRule::reactToClick`, MTGRules.cpp:1057** --
  `alternateCost->hasSpecificX()? 20 : (playerMana->getConvertedCost() - alternateCost->getConvertedCost()) + 1`

Per `CardPrimitive::setManaCost` (CardPrimitive.cpp:231-252) a convoke card's convoke variant is
an ALTERNATIVE `{0}`+Convoke shell that carries the `{X}`, so every "cast with its convoke cost"
pick enters the second one. **The convoke X-credit is unreachable on the convoke cast route.**

**Live evidence.** `Announce the value of X` occurrences in the corpus: **28 at deck158**
(Assault on Osgiliath -- a NORMAL X cast, so the fix's own reachable path works) and **0 at
deck137** across all six games, despite 5 March picks. `You can afford X up to N` likewise 0 at
deck137, 32 at deck158.

**Fix direction.** Route both MTGRules announce blocks through
`ManaEngine::maxAnnounceableX(player, cost, anytype)` instead of the hand-rolled pool
subtraction, so the convoke credit (and any future credit) applies uniformly. Guard the
alternative path on the alternative cost object.

**Validation rule for wave-32 (do not repeat wave-30's mistake):** "A is fixed" is provable only
by an `ANNOUNCE_X` record at a CONVOKE-route pick plus the matching `Soldier (1/1): created ->
battlefield` xN. A fixture on the normal-cast path does not test this.

--------------------------------------------------------------------------------
## N-137c (HIGH, updated) -- The alternative-cost CONVOKE branch consumes the answer and abandons the cast (unlogged silent no-op)
--------------------------------------------------------------------------------
Supersedes wave-30's N-137-MARCH-XCONVOKE with a same-menu CONTROL that narrows the fault.

**Four repros, deck137 vs139** (`1785138635-ai_baka_deck137-0x5619c89d0c20-vs-ai_baka_deck139.jsonl`):
| seqs | turn | pool at the mode menu | narration for that turn |
|------|------|-----------------------|--------------------------|
| s22, s23, s24 | 8  | `{g}{w}{w}{w}{w}` (5) | ONLY `- Choose an option for March of the Multitudes: -> convoke` |
| s29, s30      | 10 | `{g}{w}{w}{w}{w}{w}` (6) | ONLY `-> convoke` |
| s36, s37, s38 | 12 | `{g}{w}{w}{w}` (4) | ONLY `-> convoke` |
| s41, s42      | 14 | `{g}{g}{w}{w}{w}{w}{w}{w}` (8) | ONLY `-> convoke` |

In all four: mode-menu answer consumed, entire land base floated into the pool, then **no
ANNOUNCE_X ask, no `March: hand -> stack`, no Soldier token, no `defer` record**, card still in
hand the following turn. **Zero Soldier tokens from March across six games.**

**CONTROL, same menu, same seam, deck137 vs148 s53-s54 (t16):** `Choose an option for Venerated
Loxodon: 1. Cast Card Normally / 2. convoke` -> picked 1 -> narration `-> Cast Card Normally`,
`hand -> stack`, `stack -> battlefield`. **The mode menu and the normal branch are sound.** The
fault is inside the alternative(convoke) cast branch downstream of the menu answer.

N-137b is a plausible but UNPROVEN contributor: at vs139 t14 the pool alone was 8, so the
pool-only `options` at MTGRules.cpp:1057 should still have exceeded 1 and armed a menu. Treat
N-137c as a SEPARATE defect requiring its own instrumented repro (env-gated `fprintf` in
`MTGAlternativeCostRule::reactToClick` around the announce block and the
`isExtraPaymentSet()` / `setExtraCostsAction` return at MTGRules.cpp:1150-1160), not as
"will be fixed by N-137b".

**Mitigation already working (do not touch):** `AIPlayerGPT::mStuckCastLines`
(AIPlayerGPT.cpp:3845-3852, consumed :4086) caps the re-pick loop at 2 per turn. Wave-29's
4-7x March spiral is gone.

--------------------------------------------------------------------------------
## N-137d (MEDIUM, CONFIRMED) -- Divergence C reproduced; it is NOT convoke-scoped
--------------------------------------------------------------------------------
**Repro:** deck137 vs152 **s34, t12, Main phase 2**. Record: `kind=defer, choice=-1, options=0,
prompt="", reply="", chosen_text="March of the Multitudes", latency_ms=-1,
fallback=deferred_to_heuristic`. This is the emission at **AIPlayerGPT.cpp:4135**, reached when
`AIPlayerBaka::FindCardToPlay(pMana, "*")` returns NULL with `aiForcedCandidate` set
(AIPlayerGPT.cpp:4117-4123).

**Two sharpenings for the wave-32 ledger item:**
1. The rejected pick at s33 was the **PLAIN** cast (`1. Cast March of the Multitudes
   {g}{w}{w}{x} / 2. Cast nothing right now`; convoke correctly absent -- all three creatures
   were tapped from the s32 attack). So divergence C is the general
   **offerable()-approves / `"*"`-validation-rejects** gap across ALL cast kinds. Re-word the
   ledger item away from "convoke casts".
2. Here the reject was substantively RIGHT: `Mana available: {g}{w} from 3 untapped sources`
   is exactly `{G}{W}{W}`, so X could only be 0, and March's own script is
   `auto=this(X=0) donothing`. The bad part is the MECHANISM (silent, no re-ask, the model's
   window is burned), not the outcome. **Prefer the "loud re-ask" half of the named fix
   direction over "accept on offerable()'s payability"** -- the latter would have let a
   guaranteed-blank cast through.

--------------------------------------------------------------------------------
## N-137-METRIC (METHOD, recurrence) -- the defer counter undercounted 6:1 at this seat
--------------------------------------------------------------------------------
Logged fallbacks at deck137: **1 defer**. Arrival-traced convoke failures: **6**
(5 silent no-ops + 1 defer). Third consecutive wave in which this class hides from the counter
(wave-27: "harmless by-design"; wave-30: 2 logged vs 5 real; wave-31: 1 logged vs 6 real).
Restated for the skill in skill.md: an exit gate on a cast/multi-step action class must be
phrased as "offered choices producing no downstream game-state effect -> 0" and verified in the
narration, per attempt, with a same-seam CONTROL that DOES resolve.

--------------------------------------------------------------------------------
## N-137e (LOW) -- Flower // Flourish search reveal is still whole-library, eligibility-tagged
--------------------------------------------------------------------------------
h3's brief bar is "eligible-count options or no-ask at all". On the `revealzone(mylibrary)` SEARCH
path used by `Flower // Flourish`
(`auto=... target(<1>*[basic;forest;plains]|reveal) ...`, borderline.txt:41113) the ask still
lists the WHOLE library: **54** options at vs158 s6, **50** at vs139 s10, **48** at vs148 s17,
**47** at vs148 s19.

**Mitigation is present and effective**, which is why this is LOW: the ask carries
`ELIGIBILITY: only a basic, forest, plains card may go to "choose card" - the rest do not
qualify and go to "shuffle" regardless. Pick ONLY from the cards marked [eligible] below.` plus
per-option `[eligible for "choose card"]` tags (18 of 54 tagged at vs158 s6). All 12 reveals at
this seat picked a legal card, and the fetch ARRIVED: vs139 s10 and vs148 s17 both narrate
`Your revealed Plains goes to hand`. Cost is prompt size, not correctness. Fold into the
N-136a family only if a seat shows a misread.

--------------------------------------------------------------------------------
## N-137f (HOUSEKEEPING) -- `in GetCost Seems ManaCost was not properly initialized` is a deck-load constant
--------------------------------------------------------------------------------
Fires **exactly 24 times in every one of the six deck137 stderr files**, always within lines
97-120 of ~1500 -- i.e. at deck load, before any gameplay, and invariant to whether convoke or
mutate ever fired in the game. The N-139h commit message cites this trace as evidence the OOB
precondition is "demonstrably live in the mutate/convoke paths"; at this seat it is **not** a
gameplay signal. Wave-32 should re-check whether the 24 are the same 24 card definitions in every
game before treating the trace as convoke/mutate evidence.

--------------------------------------------------------------------------------
## Corpus-gate confirmations at this seat (no action)
--------------------------------------------------------------------------------
- **h1 N-139h**: zero assertion / abort / segfault / core-dump signatures in all six
  `game-137v*.stderr`. **PASS.**
- **h5 N-152b**: N/A -- all 29 priority windows are real activated abilities
  (`Tap with Giant Killer // Chop Down targeting ...`, `auto={1}{W}{T}:target(creature)
  tap(noevent)`, borderline.txt:44884), not display toggles. The 18->29 rise vs wave-30 is a draw
  effect, not a regression.
- **h6 N-146g**: N/A -- deck137 runs no planeswalkers.
- **N-18e truncated-block safety route**: fired once (vs158 s32, t13, 11,551-char reply, 179s)
  and was CORRECT -- the reply committed `BLOCKS: B1:A3` then reversed itself and was cut off.
  Board was 4 life vs 14 attacking power with one 1/1 flyer; no cost. Working as designed.
