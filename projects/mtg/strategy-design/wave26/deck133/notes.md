# deck133 wave-26 -- development notes (engine / harness / model / construction) + ROTATION VERDICT

Corpus PRIMARY `matchups-20260725-115216`, binary `77e91ef4e` (wave-26 step-1 batch, commits
79e39511f + 77e91ef4e), pilot repetition_penalty=1.05. Real games: 22(W), 131(W), 102(W), 27(W),
137(W-adj), 18(W) -- **6W/0L/0D** (up from 1/6). Layer-routing enforced: engine/harness/model/
construction live HERE with seq repros; the guide is BYTE-FROZEN this cycle (no edit); win/result
column is context-only.

## GUIDE -- [NO EDIT this wave] frozen, byte-identical to deployed `74efc610...`.
cmp shows wave26/deck133/strategy.txt == bin/Res/ai/baka/deck133_strategy.txt exactly. The wave-25
Arena edit HELD (findings 3: 0 low-life casts; edit's timing + per-upkeep-cost facts cited to
decline Arena at 16 life). No new guide-contradicting pattern emerged. Adding a rule for behavior
the model now performs correctly (Arena decline, Yawgmoth value-engine use with no opp creature)
would be a crutch for a non-problem -- declined per the belief-correcting-crutch discipline.

## HARNESS/PARSER -- [OPEN, LOW-priority -- c4 corpus-gate finding] retracted_choice false-fallback on a NEW sub-shape (premature-answer-then-non-line-leading-correction).
The ONE non-defer fallback corpus-wide (kind=priority, type=retracted_choice) at deck133 vs102 s22,
turn 9, my15/opp16, latency 84.7s, choice=-1. Full decomposition + verbatim reply in findings.md
finding 2. Summary for the ledger:
- The reply LEADS with `CHOICE: 0 (pass)` (line-leading), then reasons, EXPLICITLY retracts it
  ("Why did I think 'pass'? Because I assumed I needed mana. I don't."), and recodes to option 3 --
  corroborated 3 ways: mid-prose `So, CHOICE: 3 (...)`, natural-language `So the choice is 3.`, AND
  the `PLAN:` action (Sacrifice Bloodghast to Yawgmoth = option 3's action, not pass).
- `choiceRetractedNoReplacement` (AIPlayerGPT.cpp ~2510) fires condition (a): the recode `So, CHOICE:
  3` is prefixed "So, ", so it is NOT line-leading, so it can only ever register as a contradictory
  mid-prose second coded index (n=3 != chosenNum 0) -> retract -> heuristic. This is the c4
  line-anchoring's DESIGNED behavior (refuse to salvage a non-line-leading coded index as the
  answer), and the SAFE direction (false-fallback to heuristic, not a wrong take). The detector's own
  comment ("A false fallback here only routes to the (safe) heuristic; a false take ... is the
  harmful case we still catch") documents this tradeoff.
- It is structurally INDISTINGUISHABLE from the harmful "answer-then-drift" shape it is designed to
  catch (wave-24 vs140 s9: line-1 `CHOICE: 2 (Decline)` then buried `So CHOICE: 1`). Both = a
  line-leading answer + a contradictory mid-prose recode.
- **Candidate discriminator (LOW value, offered not urged):** the two shapes differ in that HERE the
  `PLAN:` action corroborates the RECODE (option 3), not the line-1 index. When a natural-stop reply's
  line-leading coded index contradicts its own PLAN action while a non-line-leading recode matches the
  PLAN, prefer the recode (or at least do not count it as a foreign retraction). Requires matching
  PLAN prose to an option action -- the fragile name-matching the parser deliberately avoids -- so
  LOW priority. Rarity: 1/1364 decisions corpus-wide; non-deciding (game won 14/-5).
- **Model/protocol contributor (note-only):** the answer-first protocol can induce a PREMATURE commit
  (`CHOICE: 0`) that the model then overturns in reasoning, producing exactly this shape. Not
  actionable at the guide layer; noted as the generative cause. Repro: vs102 log seq22.
- Refines the wave-25 conclusion: retracted_choice is NOT extinct (0 corpus-wide was wave-25's read).
  It recurred once in wave-26 in a DIFFERENT trigger (genuine self-correction), independent of the
  b1 example-echo de-fang. The de-fang/example-echo exclusion are not implicated here.

## MODEL/RULES-KNOWLEDGE -- [RESOLVED -- Arena timing fact internalized] the wave-25 "Arena draws immediately" misbelief is corrected.
vs131 s11 reply: "Phyrexian Arena (which costs life and draws slowly)"; s13: "the 1 life per turn
cost is unnecessary." The model now cites the slow-draw timing and per-upkeep cost that the guide
edit added. Weak-by-construction (deciding low-life trigger absent this corpus) but no counter-
evidence; the misbelief did not recur. No engine/core action.

## LEDGER ITEM DISCHARGED -- Yawgmoth SacrificeCost acceptance round 2.
Wave-25 carried "Yawgmoth/Bloodghast-fodder unexercised at 133." Yawgmoth is now EXERCISED: 5
activations (vs131 s37, vs102 s13, vs137 s29/s45/s46), all payable (fallback=None), cost
`[cost: Life, Sacrifice]` correct, engine used correctly (removal on opp creatures when present,
sac+draw value when opp has none). The wave-24 unpayable-cost class (offer "Sacrifice another
creature" with no other creature) did NOT recur. Item DISCHARGED. (Bloodghast forced-sac FODDER
line still UNEXERCISED -- no edict at this seat; carried, cheap.)

## RULES-SHIFT / CONTRACT (c1-c5) -- observed at this seat:
- **c1 (target-preview cap removed):** 0 "(+N more)" markers in 6 logs; 7 removal decisions clean.
  No regression, no prompt-length side effect. Not a stress test (single-target removal vs small
  boards). No action.
- **c2/c3/c5:** UNTESTABLE at this seat -- deck133 runs fetches only (no shockland/pathway ETB, c2=0),
  has no activated pump (c3=0), and faced no resolvable may-triggers (c5=0). Carried as
  "deck-structurally-untestable at 133."
- **c4:** the retracted_choice finding above.

## CONSTRUCTION (upstream decklist flags -- RE-CONFIRMED, do NOT fix with guide length)
- **Top-heavy payoff base / threat-drought variance (the deck's dominant swing factor).** Wave-25
  lost 3 games with GM+Obliterator offered 0x; wave-26 WON with Obliterator deployed 5/6. Same deck,
  opposite variance. The 6/6 is substantially this variance reversing PLUS the pool losing deck133's
  two hardest matchups (109 aggro + 59 grind rotated out). Deck-tuning lever (upstream, if ever
  pursued): more early interaction / a castable early blocker would smooth the drought-loss variance.
  Out of scope for the guide.
- **No early blocker; Bloodghast cannot block.** Carried -- was central to the wave-25 vs109 aggro
  loss; simply not punished this corpus (109 gone, incoming Step-0 decks did not exploit it).
- **No reach / no flyer answer except Gray Merchant + Liliana -2.** Carried. Not tested this corpus.
- **Self-damage suite (Thoughtseize 2, Arena 1/upkeep, fetch 1 each).** Arena now guide-addressed and
  behaving (findings 3). Quiet this corpus.

## ROTATION VERDICT -- ROTATE OUT.
The standing test: "no-guide-mod AND no-new-work-signal = candidate." Applied UNSENTIMENTALLY per the
brief (the loop's endgame is real; termination gates on NEW-WORK EXHAUSTION at the guide/deck-review
layer). The wave-25 notes explicitly teed this up: "If NEXT cycle the Arena edit validates (or no
more low-life Arena casts occur), the parser hardening ships, and Yawgmoth/Bloodghast-fodder stay
unexercised with the guide otherwise frozen, deck133 becomes a straightforward ROTATE-OUT."

Scoring that prediction this cycle:
- **Prong 1 (no-guide-mod): TRUE.** Guide byte-FROZEN this wave (cmp-verified). The Arena edit HELD
  (0 low-life casts, facts cited); no new guide-contradicting pattern; no new edit warranted.
- **Prong 2 (no-new-work-signal at the guide/deck layer): TRUE.**
  - Arena edit: validated (weak-by-construction, but no more low-life casts) -- condition met.
  - Yawgmoth acceptance round 2: EXERCISED and CLEAN -- open item DISCHARGED (better than the wave-25
    prediction of "stays unexercised").
  - Bloodghast-fodder: stays unexercised -- carried, cheap, no work.
  - Liliana round 4, Gray Merchant, self-FP: all CLEAN, no work.
  - The ONE new signal (the retracted_choice c4 finding) is a HARNESS/PARSER-lane item, NOT a
    deck133-specific guide item. Per layer-routing, engine/harness items are worked from the ledger
    between waves regardless of pool membership. Critically, the sub-shape (premature-answer-then-
    correct) is GENERAL model x parser behavior -- ANY deck can produce it, any future corpus can
    re-witness it. Keeping deck133 in the pool is NOT the mechanism to work it. deck133 was the
    historical unique producer of the OLD example-echo retracted_choice; that trigger is de-fanged
    and this NEW trigger is not deck133-bound. So no deck133-specific new-work signal exists.

Both prongs of the rotation candidacy test are TRUE -> **ROTATE OUT.**

Honest caveats (unsentimental, on the record):
1. The 6/6 is PARTLY POOL-INFLATED -- deck133's two worst matchups (109 aggro, 59 grind) both left
   the pool, replaced by two weaker Step-0 decks (18/22). The structural construction flags (no early
   blocker, no reach, Bloodghast can't block, top-heavy payoffs) are UNCHANGED; they simply were not
   punished. But those are CONSTRUCTION-lane/upstream items, already ledgered, not guide-fixable and
   not deck-review work -- they do not gate rotation.
2. The Arena edit is only WEAK-confirmed (deciding low-life trigger absent). This is the same
   trigger-absent ceiling as the b1 de-fang, which was accepted as sufficient for deck59/deck109
   rotations. Consistent precedent: weak-but-clean + no counter-evidence does not block rotation.
3. deck133 has been a productive seat (retracted_choice class, Arena misplay, Liliana discipline,
   Yawgmoth acceptance) but every one of those is now CLOSED or discharged at the guide/deck layer.
   Its guide is proven and frozen. It has exhausted its deck-review new-work. ROTATE OUT.
