# deck137 -- Selesnya (GW) Adventures -- wave 31 (guided ROUND 9) -- THE ARRIVAL-TRACED CONVOKE TABLE

Binary `cf7cb2e53`, corpus `matchups-20260727-025026`, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: `1785*-ai_baka_deck137-0x*-vs-{139,148,152,122,158,146}.jsonl`.
Card facts below are read off `Res/sets/primitives/borderline.txt` and the engine render in the
`prompt` field, never from Oracle memory.

================================================================================
## (0) HEADLINE
================================================================================
**The h2 prediction is FALSIFIED at this seat, and the mechanism is now source-located.**

- Divergence **A is NOT observable here and structurally CANNOT be**: `ANNOUNCE_X` fired
  **0 times** at deck137 across 6 games. The fix landed in `ManaEngine::maxAnnounceableX`,
  which **neither** MTGRules announce path calls -- and March's convoke cast rides the
  alternative-cost announce path. Corpus-wide control: `Announce the value of X` fired **28x at
  deck158** (Assault on Osgiliath, a NORMAL X cast) and **0x at deck137**. This is a
  **PATH-SCOPED FIX** in the exact shape of wave-30 method headline 2. (Section 3, F1.)
- Divergence **C CONFIRMED, exact shape**, at the seat's single defer (vs152 s34 t12) -- and it
  fired on a **PLAIN** cast, not a convoke cast, so C is the general offer-vs-`"*"`-validation
  gap, not a convoke-only defect. (Section 4.)
- **Offered-convoke resolution fell to 2/8 (25%)**, from wave-30's ~55%. **March of the
  Multitudes resolved 0/5 and produced ZERO Soldier tokens in 6 games.** Five UNLOGGED silent
  no-ops (four March, one Loxodon) -- again invisible to the fallback counter, again only
  visible by arrival-tracing. (Sections 1-2.)
- A **NEW, source-verified engine defect** explains the offer-side face: `Convoke`'s reduction
  loop terminates after the GREEN colour index, so convoke can only ever pay **green pips or
  generic** -- a white pip is never creditable. `{X}{G}{W}{W}` therefore requires **both {W}
  from LANDS** before convoke is even offerable. (Section 3, F4; repro vs146 s20/s24.)

Outcomes: **3/6, 2 KILLS, 2 adjudications, 0 timeouts, 0 draws, 0 crashes.**

================================================================================
## (1) THE ARRIVAL-TRACED CONVOKE TABLE (the centerpiece)
================================================================================
Method: for every cast decision whose option list contained a `... with its convoke cost ...`
line, record OFFERED / PICKED / ANNOUNCE_X record present / and whether the permanent or token
**arrived in the narration** of a later record. Narration turn labels run +1 vs the record
`turn` field (record t8 -> narration `=== Turn 9 ===`); both are given.

### VENERATED LOXODON -- `mana={4}{W}`, Creature 4/4 Elephant Cleric, `other={convoke}`,
### `auto=alternative ... all(creature[tapped;-attacking]|myBattlefield) counter(1/1)` (borderline.txt)

| game  | seq (t)     | route | ANNOUNCE_X | arrival evidence in narration | verdict |
|-------|-------------|-------|------------|-------------------------------|---------|
| vs122 | 27 (t8)     | convoke direct (only cast option; `Mana available: {w} from 1 untapped source`) | n/a (no X) | narration Turn 9: **NO Loxodon event of any kind**; card still in hand at s30 | **SILENT NO-OP** |
| vs122 | 30 (t10)    | convoke direct (opt 2 of 8; 5 untapped sources) | n/a | Turn 11: `Venerated Loxodon: hand -> stack` / `stack -> battlefield` / `Counter added to Faerie Guidemother` / `Soldier (1/1): created -> battlefield` (Emmara's tapped-trigger, i.e. she was tapped to convoke) | **RESOLVED** |
| vs158 | 21 (t6)     | convoke direct (only cast option; 4 untapped sources) | n/a | Turn 7: `Venerated Loxodon: library -> hand` / `hand -> stack` / `stack -> battlefield`; blocks Orc army at s22 | **RESOLVED** |
| vs148 | 53->54 (t16)| **PLAIN** `{4}{w}` at cast seam, then `Cast Card Normally` at the mode menu (convoke was offered as opt 4 and as mode opt 2) | n/a | Turn 17: `Choose an option for Venerated Loxodon: -> Cast Card Normally` / `hand -> stack` / `stack -> battlefield`; blocks Kor Duelist s58, attacks s62 | **RESOLVED (plain -- the CONTROL case)** |
| vs152 | 25 (t10), 30 (t12) | convoke OFFERED, model chose `Cast nothing right now` | -- | -- | declined (not a failure) |
| vs146 | --          | never offered -- Loxodon bottomed at the mulligan (s3) | -- | -- | n/a |

**Loxodon: 3 convoke picks -> 2 RESOLVED, 1 SILENT NO-OP. Plus 1 plain-cast control, resolved.**

### MARCH OF THE MULTITUDES -- `mana={X}{G}{W}{W}`, Instant, `other={convoke}`,
### `auto=this(X=N) create(Soldier:creature Soldier:1/1:white:lifelink)*N` (borderline.txt)

| game  | seq chain (t)   | route | ANNOUNCE_X | arrival evidence | verdict |
|-------|-----------------|-------|------------|------------------|---------|
| vs139 | 22 -> 23 -> 24 (t8)  | convoke at cast seam (opts 2), re-offered with pool floated to `{g}{w}{w}{w}{w}` -> convoke again (opt 3 of 6) -> mode menu `1. Cast Card Normally / 2. convoke` -> **convoke** | **NONE** | narration Turn 9 contains ONLY `- Choose an option for March of the Multitudes: -> convoke`. No `March: hand -> stack`, no `Soldier ... created`, no defer record | **SILENT NO-OP** |
| vs139 | 29 -> 30 (t10)  | **PLAIN** `{g}{w}{w}{x}` at cast seam (opt 2 of 6, 6 untapped sources) -> mode menu -> **convoke** (pool `{g}{w}{w}{w}{w}{w}`) | **NONE** | Turn 11: `-> convoke` only | **SILENT NO-OP** |
| vs139 | 36 -> 37 -> 38 (t12) | convoke (opts 2) -> re-offer with pool `{g}{w}{w}{w}` -> convoke (opt 3 of 4) -> mode menu -> **convoke** | **NONE** | Turn 13: `-> convoke` only | **SILENT NO-OP** |
| vs139 | 41 -> 42 (t14)  | **PLAIN** at cast seam (8 untapped sources) -> mode menu -> **convoke** (pool `{g}{g}{w}{w}{w}{w}{w}{w}` = 8) | **NONE** | Turn 15: `-> convoke` only | **SILENT NO-OP** |
| vs152 | 33 -> 34 (t12)  | **PLAIN** `{g}{w}{w}{x}`, `Mana available: {g}{w} from 3 untapped sources`, every creature tapped from the s32 attack (so X could only be 0) | **NONE** | narration Turn 13: **no March event at all** | **DEFER (divergence C)** |
| vs152 | 25 (t10), 30 (t12) | March convoke OFFERED, declined | -- | -- | declined |
| vs122 | 19, 25, 30, 32 (t6-t12) | March convoke and/or plain OFFERED at four cast seams; model cast other cards each time | -- | -- | declined |
| vs146 | s20 (t8), s24 (t10) | March in hand from t7 with 2 white Human convokers + 3-4 Forests untapped; **NO March line offered at all** | -- | -- | **NOT OFFERED (offer-side gap, F4)** |
| vs139 | 14 (t4), 17/18 (t6) | March convoke OFFERED, declined (correctly -- X would be 0-1) | -- | -- | declined |
| vs148 | --              | March never reached hand (all copies stayed in library) | -- | -- | n/a |
| vs158 | --              | March bottomed at the mulligan (s3) | -- | -- | n/a |

**March: 5 picks -> 0 RESOLVED. 4 SILENT NO-OPS + 1 DEFER. ZERO Soldier tokens from March in
six games.**

### GRAND TALLY
- 8 convoke-route picks (3 Loxodon + 5 March; 4 of the 5 March picks come from a single game).
- **RESOLVED 2 (both Loxodon convoke). UNRESOLVED 6 = 5 silent no-ops + 1 defer.**
- **Offered-convoke resolution rate 2/8 = 25%** (wave-30, same measure: 5/9 = 56%).
- Logged fallbacks at my seat: **1 defer + 1 `truncated_abandoned`** (section 6). The defer
  counter therefore reports **1** failure where arrival-tracing finds **6**. Third consecutive
  wave in which the counter undercounts this class -- see notes.md N-137-METRIC.
- ANNOUNCE_X records at deck137: **0**. Affordability annotation `You can afford X up to N`
  occurrences at deck137: **0** (28 at deck158).

================================================================================
## (2) THE END-TO-END CONTROL PAIR (what proves the seam is not globally broken)
================================================================================
The same `Choose an option for <card>:` mode menu produces opposite outcomes depending only on
which branch is taken:

- **vs148 s53-s54 t16 (NORMAL branch, RESOLVES).** `3. Cast Venerated Loxodon {4}{w} (4/4)`
  picked; mode menu `1. Cast Card Normally / 2. convoke`; picked 1. Narration Turn 17:
  `Choose an option for Venerated Loxodon: -> Cast Card Normally`, `Your Venerated Loxodon:
  hand -> stack`, `Your Venerated Loxodon: stack -> battlefield`. The body then blocks a Kor
  Duelist (s58, 4 damage dealt) and attacks (s62) in a game deck137 **wins by kill** at 1 life.
- **vs139 s41-s42 t14 (CONVOKE branch, DIES).** Identical menu, pool 8 mana, four untapped
  creatures. Narration Turn 15 records the consumed answer `-> convoke` and **nothing else**.

Same menu, same tick shape, same turn structure: the mode-menu seam and the normal-cast branch
are sound; the failure is inside the **alternative(convoke) cast branch**.

Cost of each no-op is not zero: the engine floats the entire land base into the pool before
abandoning (vs139 t14: 8 mana floated and wasted; t10: 6; t8: 5).

**Churn is genuinely tamed** -- `AIPlayerGPT::mStuckCastLines` (AIPlayerGPT.cpp:3845-3852,
consumed at :4086) suppresses a cast line that made no board progress for the rest of that turn,
so the worst re-pick loop this corpus is 2 consecutive picks per turn (vs139 t8, t12), never the
wave-29 4-7x spiral. The livelock breaker works; the cast is still lost.

================================================================================
## (3) DEFECT DECOMPOSITION (each item routed to notes.md with repros)
================================================================================

### F1 -- divergence A's fix is on a code path the convoke cast never takes (VERIFIED IN SOURCE)
`ManaEngine::maxAnnounceableX` (ManaEngine.cpp:640) is where cf7cb2e53 added the CR 702.51
convoke creature-credit. It has exactly three references in the tree:
`AllAbilities.cpp:8209` (MenuAbility optional-cost announce), `AIPlayerBaka.cpp:2644`, and its
own declaration. **Neither MTGRules announce path calls it.** Both compute the option count from
the floated pool alone:
- normal cast, `MTGRules.cpp:433`: `int options = ... : (playerMana->getConvertedCost() - cost->getConvertedCost()) + 1;`
- **alternative cost, `MTGAlternativeCostRule::reactToClick`, MTGRules.cpp:1057**:
  `int options = alternateCost->hasSpecificX()? 20 : (playerMana->getConvertedCost() - alternateCost->getConvertedCost()) + 1;`

Per `CardPrimitive::setManaCost` (CardPrimitive.cpp:231-252) a convoke card's convoke variant is
an **alternative** `{0}`+Convoke shell that carries the `{X}`, so a "cast March with its convoke
cost" pick necessarily enters the second of those. The convoke credit is therefore unreachable on
this route. That is why the brief's falsifiable prediction ("A observable as ANNOUNCE_X firing on
mana-short convoke") produced zero observations here while firing 28x at deck158's normal X cast.

### F2 -- the alternative-cost CONVOKE branch consumes the answer and abandons the cast (4/4)
Repros (all deck137 vs139): s22-24 t8, s29-30 t10, s36-38 t12, s41-42 t14. In every one the
mode-menu answer is consumed (narration line present), the pool is floated, and then: no
ANNOUNCE_X ask, no `hand -> stack`, no token, no defer record, card still in hand next turn.
Control: vs148 s53-54 (section 2). This is the dominant residual and it is **unlogged** -- it
cannot be seen by any counter. Note also that F1 is a plausible but unproven contributor here
(the vs139 lands could make `{W}{W}` and the pool alone reached 8 at t14, so pool-only `options`
should still have been > 1) -- so **F2 is a distinct failure from F1 and needs its own repro run**,
not a "will be fixed by A" assumption.

### F3 -- divergence C confirmed (see section 4)

### F4 -- NEW: convoke can only pay GREEN pips or GENERIC; every other colour is unreachable
Both the offer probe and the real payment share one reduction loop shape, and both terminate the
colour scan on the first iteration:
- `convokeReduceByCreatures` (ExtraCost.cpp:1241-1267), used by `Convoke::offerable` (:1268)
- `Convoke::getReduction` (ExtraCost.cpp:1178-1231), used by `Convoke::doPay`

```
for (int i = Constants::MTG_COLOR_GREEN; i <= Constants::MTG_COLOR_WHITE; ++i) {
    if (next) break;
    if ((c->getManaCost()->hasColor(i) || c->hasColor(i)) && toReduce->hasColor(i)) {
        toReduce->remove(i, 1); next = true;          // only ever i == GREEN
    } else {
        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, 1); next = true;   // ALSO sets next
    }
}
```
`MTG_COLOR_ARTIFACT = 0, MTG_COLOR_GREEN = 1, ... MTG_COLOR_WHITE = 5`
(include/MTGDefinitions.h:100-105), so the loop's first index is GREEN and **both branches set
`next = true`** -- indices 2..5 are never examined. A creature reduces the GREEN pip if it is
green and the cost still has green; otherwise it reduces one GENERIC. A **white** creature can
never pay a `{W}`. This violates CR 702.51 for four of five colours.

Live repro: **vs146 s20 (t8)** -- `Mana available: {g} from 3 untapped sources`; battlefield
`Forest #1/#2/#3` untapped, `Human #1 (1/1)` + `Human #2 (1/1)` untapped (white tokens -- the
engine's own adventure render reads `{adventure spell: Create a 1/1 white Human creature
token.}`), `Lovestruck Beast (5/5)` untapped; hand holds 2x `March of the Multitudes
{g}{w}{w}{x}`. By CR the two Humans convoke `{W}{W}`, a Forest pays `{G}`, two Forests give X=2.
The offered list was `1. Cast Lovestruck Beast ... / 2. Cast nothing right now` -- **March was
not offered at all**. Same at s24 (t10, 4 Forests + 2 Humans). This is why an all-Forest board
never sees March: `{X}{G}{W}{W}` needs BOTH white pips from lands before convoke is offerable.

F4 also explains the asymmetry in section 1: `{4}{W}` Loxodon has ONE coloured pip, so a single
untapped white land makes it offerable and payable (vs158 s21, vs122 s30 both resolved), whereas
`{G}{W}{W}` March effectively demands two white lands.

F4 does **not** explain vs122 s27 (Loxodon silent no-op with `{w}` from 1 untapped source and 4+
untapped creatures -- colour-legal by F4's own rule) nor the vs139 March no-ops (Plains x4 +
Temple Garden x2 available). Those remain F2. Stated as a limit, not papered over.

================================================================================
## (4) DEFER DECOMPOSITION -- vs152 s34 t12: DIVERGENCE-C SHAPE, EXACTLY
================================================================================
Record: `{"seq":34,"turn":12,"phase":"Main phase 2","kind":"defer","choice":-1,"options":0,
"prompt":"","reply":"","chosen_text":"March of the Multitudes","latency_ms":-1,
"fallback":"deferred_to_heuristic"}`.

That is byte-for-byte the emission at **AIPlayerGPT.cpp:4135**:
```
    //The cheap menu filter let through something the real machinery rejects
    DebugTrace("AIPlayerGPT: model chose " << chosen->name << " but it fails validation; deferring to heuristic");
    writeTransLog("defer", "", "", -1, 0, chosen->name, "deferred_to_heuristic");
```
reached when `AIPlayerBaka::FindCardToPlay(pMana, "*")` returns NULL with `aiForcedCandidate`
set (:4117-4123). **Verdict: divergence C, not a new class.**

Two decomposition notes that sharpen the wave-32 item:
1. **The rejected pick was the PLAIN cast, not the convoke cast.** At s33 the offered list was
   `1. Cast March of the Multitudes {g}{w}{w}{x} / 2. Cast nothing right now` -- convoke was
   correctly absent (all three creatures tapped from the s32 attack). So divergence C is the
   general **offerable()-approves / `"*"`-validation-rejects** gap. The ledger item's fix
   direction ("accept convoke on offerable()'s payability OR loud re-ask") should be rephrased
   for the whole cast class, not scoped to convoke.
2. **In this instance the reject was arguably CORRECT.** `Mana available: {g}{w} from 3 untapped
   sources` = exactly `{G}{W}{W}`, X can only be 0, and `auto=this(X=0) donothing`. The engine
   declining a do-nothing cast is a good outcome reached by a bad mechanism (silent, no re-ask,
   the model's window is burned). The **loud re-ask** half of the named fix direction is the
   right half.

**No NEW defer class at this seat. The seat's only defer is divergence C.**

================================================================================
## (5) WIN / LOSS READ (3/6) -- against the adjudication frame
================================================================================
From `results.tsv` (deck137 is player 0 in all six rows) and the per-game `gameend` records:

| result | opponent | line | turn | read |
|--------|----------|------|------|------|
| **WIN (kill)** | 146 Orzhov Dungeons | 19 - 0 | 12 | Clean beatdown. **Zero convoke in the whole game** (Loxodon bottomed, March never offered -- F4). Won on Humans + Lovestruck Beast + Faerie Guidemother pressure alone. |
| **WIN (kill)** | 148 Kor equipment | 1 - -4 | 18 | Razor. Survived at 1 life; the t16 **plain**-cast Loxodon blocked Kor Duelist (s58) then attacked for the kill (s62). The one convoke-adjacent card that actually arrived did the closing work -- via the plain branch. |
| **WIN (adj)** | 139 Mutants | 16 - 11 | 16 | Won on board **despite four March silent no-ops** (t8/t10/t12/t14) that burned 5, 6, 4 and 8 mana. March never made a single token. |
| **LOSS** | 152 Bant midrange | -6 - 3 | 13 | The defer game. Died 3 short. March sat in hand from t5. |
| **LOSS** | 158 Orcs of Mordor | -10 - 13 | 13 | Outclassed by a 9/9 deathtouch Orc army; the convoke Loxodon DID arrive (t6) and traded into it. Deck-power loss, not a decision loss. |
| **LOSS (adj)** | 122 Eternal Life | 12 - 16 | 13 | Grind loss to lifegain; one Loxodon convoke resolved (t10), one silently no-op'd (t8). |

**2 KILLS this wave vs wave-30's 0 kills, and only 2 of 6 games adjudicated vs 6/6.** That is the
pool getting less grindy (brief: 3 cap-adj corpus-wide vs 12), not a deck137 change -- read it
per wave-30 method headline 8 and do not credit the guide for it.

The convoke residual is still costing real games in the honest direction: vs139's four dead
Marches were free wins the deck simply did not need, and vs152 lost by 3 with an uncastable
go-wide payoff in hand.

================================================================================
## (6) SEAM STEADY-STATE (everything that is NOT convoke)
================================================================================
141 ask / 32 attackers / 29 priority / 15 blockers / 12 reveal / 3 bottom / 1 defer /
6 gamestart / 4 gameend = 243 records.

- **h1 (N-139h crash gate): PASSED at this seat.** Zero assertion / abort / segfault / core-dump
  signatures in all six `game-137v*.stderr`. Housekeeping note for wave-32: the
  `in GetCost Seems ManaCost was not properly initialized` trace fires **exactly 24x in every
  one of my six games, all inside stderr lines 97-120 of ~1500** -- it is a **deck-load-time
  constant**, not a gameplay signal. Do not read it as live convoke/mutate evidence.
- **h3 (reveal chooser): outcome CLEAN, arrival-traced; option COUNT still whole-library on the
  SEARCH path.** 12 reveals, all 12 picks legal. `Flower // Flourish` fetches
  (`target(<1>*[basic;forest;plains]|reveal)`) rendered 54 / 50 / 48 / 47 options -- the whole
  library -- but carried the `ELIGIBILITY: only a basic, forest, plains card may go to "choose
  card"` header plus per-option `[eligible for "choose card"]` tags (18 of 54 tagged at vs158
  s6). The model picked a tagged-eligible Plains every time and the card **reached hand**:
  vs139 s10 -> `Your revealed Plains goes to hand`; vs148 s17 -> same. So the brief's
  "eligible-count options or no-ask at all" is not literally met on this path, but the
  annotation carries the load and there is no misread. Routed to notes.md as LOW.
- **h5 (display-toggle auto-pass): N/A at this seat.** All 29 priority windows are real
  activated-ability windows -- 27 of them offer `Tap with Giant Killer // Chop Down targeting X`
  (`auto={1}{W}{T}:target(creature) tap(noevent)`, verified borderline.txt:44884). Priority asks
  rose 18 -> 29 vs wave-30 purely because Giant Killer was drawn more; **that is not an h5
  regression.** The model passed 28/29 and its restraint is **correct**: the ability taps Giant
  Killer itself, and at vs148 s51 it reasoned "Casting Giant Killer's tap ability costs {1}{W}
  and taps my own Giant Killer, preventing it from attacking this turn. Since the opponent's
  creatures are already tapped..." -- sound. No guide-lane finding here.
- **h6 (walker magnitudes): N/A** -- deck137 runs no planeswalkers.
- **BLOCKERS FLOOR ROUND 7: HELD.** 15 records, 14 clean-parsed, zero illegal
  one-blocker-onto-multiple-attackers piles. Legal gang-blocks are correctly distinct-per-blocker:
  vs148 s58 `Venerated Loxodon blocks Kor Duelist; Giant Killer blocks Kor Duelist` (2 blockers,
  1 attacker -- legal gang), vs158 s27 `Trostani blocks Orc army; Soldier blocks Orc army;
  Soldier blocks Orc army`, vs122 s34 `Trostani blocks Ajani's Pridemate; Soldier blocks Kitchen
  Finks`.
- **The one `truncated_abandoned` (vs158 s32 t13) is the N-18e safety route firing AS DESIGNED,
  at zero cost.** Reply led with a valid `BLOCKS: B1:A3`, then reasoned itself out of the block
  and was cut off mid-reversal at 11,551 chars / 179s; `truncatedBlockCommitmentAbandoned`
  declared the safe default. The board was 4 life vs `Orc army (9/9) [deathtouch]` + `Gothmog
  (3/3)` + `Easterling Vanguard (2/1)` = 14 attacking power with a single 1/1 flyer available --
  unsurvivable either way. Safe-direction, no loss attributable.
- **Parser contradictions: NONE.** 0 unparsed, 0 empty_reply, 0 retracted_choice, 0 stale_echo.
- **g4 tapped-tag (wave-30): steady.** `[tapped - cannot attack or block this turn]` and
  `[summoning sick - cannot attack this turn]` read correctly throughout; zero misreads.
- One benign model self-inconsistency worth recording but not acting on: vs139 s30, the reply's
  first line is `CHOICE: 2 (convoke)` while its trailing `PLAN:` prose says "Cast March of the
  Multitudes normally". The answer-first protocol took the correct (first-line) answer; only the
  plan tail drifted. Not a parse defect.

================================================================================
## (7) GUIDE VERDICT: **FROZEN, NO CHANGES** (strategy.txt is a byte-identical copy)
================================================================================
`md5 413133bda07d1804a3e2a7a1c12b3718` matches `bin/Res/ai/baka/deck137_strategy.txt`.

Every guide surface fired this corpus and was read correctly: the two-face adventure teach (the
model distinguished creature vs adventure faces at vs122 s21/s23, vs139 s35), the "an adventure
creature on your battlefield is a real body" teach (Giant Killer and Faerie Guidemother attacked
and blocked without re-derivation), the Innkeeper engine, the attack-every-turn floor (32
attacker records, no spiral), the blocking floor, the role check, the one-attempt-per-card clause
(worst re-pick is 2, section 2), the Temple Garden pay-2-life teach (vs139 s28: `pay 2 life
[this permanent then enters the battlefield UNTAPPED ...]`).

**No teach MISLED.** The one guide-teach VIOLATION this corpus is the pilot's, not the guide's:
vs152 s33 cast March with exactly `{G}{W}{W}` and every creature tapped -- X could only be 0 --
against the guide's explicit line 51-52 *"at X=0 it makes ZERO Soldiers and is a wasted card.
Only cast March when you can pay X of at least 2-3"*. The teach is already correct, explicit and
in the right words; no prose edit makes it more so, and rewriting it would burn the freeze for a
single lapse the engine then declined anyway (section 4). Guide stays byte-frozen.

The guide's line 55 (*"Convoke lets tapped creatures pay part of X too, so a wide board makes a
BIG March"*) is rules-correct per CR 702.51 and matches the engine's INTENDED behaviour, but the
engine has never once delivered it at this seat. It is not a misteach -- it is a promise the
engine owes. If the wave-32 F1/F2/F4 work does not land, a future reviewer should revisit whether
this line has become a stale-render stopgap in the wave-30 method-headline-3 sense.

================================================================================
## (8) ROTATION RECOMMENDATION: **ROTATE OUT -- with a mandatory convoke PROBE hand-off.**
================================================================================
**The h2 gate as written is NOT met** and I will not pretend otherwise: offered convokes did not
all resolve (2/8), and the failures are not exclusively divergence-C shape (1 defer vs 5 silent
no-ops). By the letter of the sharpened gate this reads "stay."

I recommend **rotate out anyway**, on the campaign's own unsentimental criterion --
**new-work exhaustion in the GUIDE lane** -- and I state the evidence so the synthesis agent can
overrule me on the record:

1. **The guide track is exhausted.** Nine guided rounds; byte-frozen two consecutive waves; every
   teach exercised and correct this corpus (section 7); zero parser events; zero perception
   misreads. The single teach violation is a pilot lapse against already-optimal prose. There is
   no guide edit this corpus justifies, and none is foreseeable -- which is precisely the
   wave-30 deck136 shape ("every guide surface exercised + correct; the only live item is an
   engine seam better validated by a probe than a rotation slot").
2. **The convoke arc is ENGINE-LANE ONLY and, as of this corpus, fully characterized.** wave-30
   kept the seat because the residual was *uncharacterized* and the metric had just been shown to
   lie. That premise is discharged: this review hands wave-32 three **source-located** defects
   with file:line and repro seqs (F1 `ManaEngine.cpp:640` vs `MTGRules.cpp:433/1057`; F2 four
   repros with a same-menu control; F4 `ExtraCost.cpp:1241-1267` + `1178-1231` with a live
   repro), plus a confirmed divergence-C shape. None of that needs a random-draw seat to advance.
3. **A random-draw seat is now the WORSE instrument.** Six games produced only 8 convoke picks,
   and **half of the corpus's March evidence came from one game (vs139)**; two games contributed
   zero convoke events at all (vs146 by F4, vs148 by draw). A stacked **probe deck** discharges
   convoke an order of magnitude more densely and can be pointed at the exact colour case F4
   predicts.
4. **Rotating does NOT close the arc, and this write-up must not be read as closing it.** The
   arc closes when the wave-32 fix is validated against the falsifiable observable below.

### MANDATORY HAND-OFF -- the convoke probe + the falsifiable observable for wave-32
Build a probe pair in the numbering gap (verify the contiguous roster ends below 198 first):
- `deck198.txt`: 4x March of the Multitudes, 4x Venerated Loxodon, 4x Emmara, 12x **Forest**,
  4x Plains, 4x Edgewall Innkeeper, 4x Lovestruck Beast (its Heart's Desire makes the WHITE Human
  tokens F4 needs), 4x Flower // Flourish. The deliberately Forest-heavy mana base is the F4
  discriminator: pre-fix, March must NEVER be offered on an all-Forest board with white
  convokers; post-fix it must be.
- Copy `deck137_strategy.txt` to `deck198_strategy.txt`. Run pinned one-shot GPT games with
  `timeout 3000`. **DELETE both probe decks afterwards** and verify the highest `deckN` is back
  to the real roster.

Post-fix PASS conditions, all three arrival-traced (never counter-read):
- **F1/F2**: an `ANNOUNCE_X` record with `Announce the value of X for March of the Multitudes`
  fires on at least one convoke-route pick, AND the narration shows `March: hand -> stack` plus
  `Soldier (1/1): created -> battlefield` xN with N == the announced X.
- **F4**: on an all-Forest board with >= 2 untapped white Human tokens, a
  `Cast March of the Multitudes with its convoke cost {x}` line **is offered**, and after
  payment the pool balances (no negative component).
- **F2 negative control**: `Cast Card Normally` on the same mode menu still resolves (it does
  today -- vs148 s53-54; a fix must not break it).
- **Whole class**: offered-convoke picks producing no downstream game-state effect == 0.

repetition_penalty=1.05 -- ninth-corpus verdict at this seat: **SAME, KEEP.** Coherent across all
replies; no prose degradation; the one 11.5k-char blockers deliberation is a length event the
N-18e route already handles safely, not a sampler event.
