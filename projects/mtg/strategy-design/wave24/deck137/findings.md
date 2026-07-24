# deck137 — Selesnya (GW) Adventures — wave 24 (initial-guide ROUND 2)

Binary d7ff8b6c4 (step-1 batch + pregame mulligan phase + N9 hotfix). deck137 played this corpus
WITH its wave-23 initial guide (its first real guide, now under validation) and owns the R-ADV-FACE
acceptance test. My 6 pilot logs: 1784*-ai_baka_deck137-0x*-vs-{131,59,133,102,109,27}.jsonl.

Outcomes: **2/6 + 1 draw** — WIN vs131 (adj, 15/11 t12), WIN vs102 (adj, 15/12 t10),
TIMEOUT-DRAW vs27 (12/12 t10), LOSS vs59 (adj behind 6/19 t11, raced by WB persist aggro),
LOSS vs133 (adj behind 4/14 t14), LOSS vs109 (KILLED, -5 life t8, raced by mono-R aggro).
Losing pattern UNCHANGED from wave-23: TEMPO — durdles on setup and is under-developed while an
aggressive opponent gets ahead. Both kills/near-kills (vs109 t8, vs59) are aggro RACES the deck
lost by blocking/stabilizing too late, not by bad card choice.

## (1) R-ADV-FACE ACCEPTANCE — **PASS (conclusive)**. The Usher misfire class is GONE.

The step-1 fix (c) LANDED. Every adventure-cost cast option now renders
`Cast NAME // ADV with its adventure cost {cost} {adventure spell: <full effect text> (Then exile
this card. You may cast the creature later from exile.)}` — with the ACTUAL effect text and **NO
creature (P/T)**. Verified on every adventure-cost option rendered across the 6 logs:
- deck131 s32/s40: `Lovestruck Beast // Heart's Desire with its adventure cost {g} {adventure spell:
  Create a 1/1 white Human creature token. ...}`
- deck59 s20: `Faerie Guidemother // Gift of the Fae with its adventure cost {1}{w} {adventure spell:
  Target creature gets +2/+1 and gains flying until end of turn. ...}`
- deck102 s18: `Giant Killer // Chop Down with its adventure cost {2}{w} {adventure spell: Destroy
  target creature with power 4 or greater. ...}`
- deck102 s42: `Shepherd of the Flock // Usher to Safety with its adventure cost {w} {adventure
  spell: Return target permanent you control to its owner's hand. ...}` — **THE wave-23 misfire card.**

The CREATURE-face option correctly keeps its (P/T): `Cast Shepherd of the Flock // Usher to Safety
{1}{w} (3/1)`. The two faces are now cleanly disambiguated by the ENGINE — creature face carries
(P/T), adventure face carries {adventure spell: effect}. Attackers/blockers renders still carry (P/T)
+ [adventure] tag correctly (those ARE the creature on the battlefield).

**Usher misfire recurrence: 0 (expected 0).** At deck102 s42 the Usher option now shows the bounce
effect; the model read it correctly and chose March of the Multitudes instead — no self-bounce.
Where the model DID cast an adventure SPELL this corpus it was CORRECT and effect-driven, never
P/T-driven:
- deck59 s35 (life 6, facing 4x2/2): cast Heart's Desire reasoning "creating a 1/1 Human token
  immediately ... chump blocker ... unlocks my Lovestruck Beast" — read the EFFECT (make 1/1), not
  a 5/5 body.
- deck133 s19: cast Heart's Desire "creates a 1/1 white Human token ... unlocks my Lovestruck Beast."

The mislabel-driven belief ("pay adventure cost, get the (P/T) body") that caused the wave-23
self-bounce is eliminated at the source. The FACT now rides the option surface.

**Crutch-lifecycle consequence: DEMOTE the guide's R-ADV-FACE representation crutch.** The wave-23
removal condition ("adventure-cost options carry effect text / drop the wrong P/T") is now MET on
both clauses. strategy.txt this wave drops the "(P/T) is the creature you get LATER, not now" TRAP
paragraph and the "the option line does not tell you the effect — memorize this" framing; the
per-face content is kept but recast as pure STRATEGY (when to use each face), leaning on the
now-present effect text rather than substituting for it. See notes.md (FACT surface -> demote).

## (2) The 19 deferred_to_heuristic (convoke tap-payments) — **HARMLESS (say so).**

All 19 are `kind=defer / fallback=deferred_to_heuristic`, empty prompt+reply, latency_ms=-1 (no LLM
call): the "which creatures to tap for convoke" sub-choice, routed to baka by design. Distribution:
March of the Multitudes (deck131 x7, deck102 x3, deck27 x2) + Venerated Loxodon (deck59 x4,
deck109 x3). deck137's convoke cards are again the SOLE source of the corpus's defers (19 of the
corpus's 22 fallbacks; brief's "3 TRUE model-fallbacks" excludes these).

**No harm found across all 19.** The heuristic's tap-selection never stranded a wanted attacker or
blocker: (a) in deck131 the model convoked March on t5/t7/t9 and still attacked with its Faerie
Guidemother every one of those turns (s19/s27/s35) — the heuristic did not tap the intended
attacker; (b) in deck59 the t8 Loxodon convoke tapped creatures that untapped before the t11 block,
so its Human+Beast blockers were available (the block problem there was comprehension, not a
convoke-tapped blocker — see (6b)). Convoke inherently taps the convokers so they can't attack THAT
turn, but that is a property of the model CHOOSING to convoke (a proactive board-widening play), not
a defer error. Verdict: mechanically harmless, no ledger item; flagged only so synthesis knows
deck137 is the defer source.

## (3) s4 stale_echo — deck102 s4, **stale_echo FALSE POSITIVE; harm NIL (arguably negative-harm).**

Not an attack/cast seam — it is the Temple Garden shockland ETB choice. s3 the model played Temple
Garden; s4 the engine offered `Choose an option for :` (empty subject) with `1. pay 2 life` /
`2. tap` (= the "enters tapped unless you pay 2 life" decision). Reply first line:
`CHOICE: 2 (Tap Temple Garden)` — a WELL-FORMED, IN-RANGE index (2) whose label ("tap") matches
opt2. Under INDEX-WINS rule (c) ("a well-formed in-range CHOICE index is trusted unless the echoed
label is genuinely out-of-context") this should have been TRUSTED. It instead downgraded to
stale_echo (choice=-1, heuristic answered).

- **Classification: FALSE POSITIVE, the FOURTH-downgrade shape recurring on a non-target menu.** The
  model decorated the bare option word "tap" with the card name — `(Tap Temple Garden)` — and
  "Temple Garden" collides with the STALE sibling option from s3 (`Play Temple Garden`). This is the
  same qualifier-name-prefix-breaks-option-N-match shape wave-23 documented as stale_echo's fourth
  downgrade cause (deck14/deck62 spell-name-prefix on target menus), here on a 2-option shockland
  ETB menu. INDEX-WINS (c) did NOT recover it: an in-range CHOICE index whose parenthetical carries a
  card-name qualifier that matches a recently-seen option still routes to stale_echo. Ledger item for
  the parser owner (notes.md) — a residual gap in INDEX-WINS's coverage.
- **Harm: NIL / negative.** The heuristic resolved the shock to `pay 2 life` (my_life 20->18 s4->s5),
  keeping Temple Garden untapped, which ENABLED the model's own turn-1 Flower cast at s5. Entering
  tapped (the model's "tap" answer) would have wasted turn 1 for this aggressive deck. The fallback
  choice was at least as good as — arguably better than — the model's answer. No board harm.
- **Underlying representation gap (ledger, sibling of wave-23 R-PAINLAND):** the shockland ETB
  renders as an UNLABELED `Choose an option for :` (empty subject) with bare `pay 2 life` / `tap` and
  no signal it is the Temple Garden "enters tapped unless you pay 2 life" decision. The model burned
  ~1,500 chars unsure whether the prompt was "malformed." Same unlabeled-life-cost-menu family as the
  painland item. Route: notes.md.

## (4) Initial-guide ROUND 2 — did the wave-23 rules land?

- **ATTACK FLOOR — LANDED STRONGLY (the headline win).** ZERO "ATTACK: none" spirals this corpus.
  Every one of the 15 attackers decisions across the 6 games SWINGS with everything available, incl.
  1/1s and 1/2s into the face when the board allows (deck131 s9/s19/s27/s35 Faerie to face;
  deck102/deck27 Giant Killers + Innkeeper alpha strikes). The wave-23 passivity that produced the
  15,901-char "ATTACK: none" spiral is GONE. Note the flip side: available attackers are usually
  only 1-3 — the deck's problem is now BOARD DEVELOPMENT / tempo (it doesn't have enough bodies),
  not attack-declaration. Attack floor did its job; the tempo leak is separate (and matches the
  loss pattern).
- **INNKEEPER ENGINE — PARTIAL.** When Innkeeper is castable early the model deploys it on curve
  (deck27: cast t1, then the draw engine fired all game — 33 Innkeeper log mentions). But vs131 and
  vs109 cast it t7 (too late to matter), and vs59/vs133/vs102 didn't cast it early (draw variance).
  The rule lands when the opportunity exists; it can't manufacture the draw. Keep the guide line.
- **ROLE CHECK / stabilize — the WEAK SPOT (see (6b)).** The deck's two aggro losses (vs109 kill t8,
  vs59) are where it needed to switch to LIFELINK STABILIZE and block, and its BLOCKING under lethal
  pressure spiraled hard (13.6k chars). The role-check exists in the guide but has no concrete
  BLOCKING mechanics; strategy.txt adds a blocking-under-lethal line this wave.

## (5) Pregame mulligan phase — **CLEAN at this seat.**

- Mulligan ask rendered in all 6 games (s1: `Keep this hand` / `Mulligan`). All 6 keep/mull
  decisions SENSIBLE: 5 keeps on 3-land + fixer + threat hands (all correct); 1 mulligan (vs59, on a
  3-lands-but-triple-Flower clunky hand — reasonable) followed by a keep of the redrawn 7 (s2) and a
  bottom-1 (s3).
- **Bottom record (vs59 s3): N=1 (CORRECT for one mulligan, London).** It bottomed one of a
  duplicate 5-drop (Venerated Loxodon, with a 2nd Loxodon still in hand) from a top-heavy redraw —
  a defensible "worst card" pick (redundant top-end over keeping developers). No double-ask, no
  wrong N.
- **Game-start integrity clean:** turn ownership reads correctly (vs131 opens "Turn 1 - opponent's
  turn", model on the draw); first draw lands on the non-starting player's turn. No double asks.
- **Leyline 103.6 window: N/A (correctly did not fire)** — deck137 has no leyline-keyword cards and
  none appeared in any opening hand. Nothing to validate; correct that the window stayed silent.
- **No mulligan-phase anomalies** (no double asks, no wrong N, no narration confusion observed at
  this seat). The known "opening hand" narration cosmetic wasn't disruptive.

## (6) 27vs137 timeout-draw — DECOMPOSE: **SLOW GRIND / latency-bound. NOT a phase/engine stall.**

Both seats reached turn 10 at 12/12 life, no gameend record either side (cap-adjudicated draw).
Per-seat inference:
- **deck137 seat: 52 LLM decisions, sum latency 1,835s** (median 18.5s, one 197.2s spike),
- **deck27 seat: 22 LLM decisions, sum latency 517s** (median 16.4s, max 89s).
- **Combined pure-inference wait = 2,352s ~ the 2,400s cap.** The game timed out because the two
  seats' summed decision latency consumed essentially the entire cap — the classic timeout-economics
  failure (skill corpus gotcha #3), here NOT from a stall but from decision COUNT x inflated -j3
  latency. Pregame consumed normal time (~17s mull decisions); no unusual pregame cost.

The dominant contributor is deck137's DECISION DENSITY: 52 LLM calls by turn 10 — ~2.4x deck27's 22.
deck137 (adventures + convoke + go-wide) is the most decision-dense deck in the pool (adventures
double every card's cast options, convoke adds defers, go-wide adds many creature/land/attacker
choices). 52x18.5s alone ~ 30 min. This makes deck137 the deck MOST likely to hit the cap at
saturated latency. Both at even life = a genuine grind, correctly adjudicated a draw. Harness/ledger
note (notes.md): deck137 games warrant a higher WAGIC_GPT_TIMEOUT or lower -j; not a play failure.

## (6b) NEW finding — the worst spiral MOVED to the BLOCKERS seam (deck59 s39, 13,642 chars).

With the attack-floor rule killing the wave-23 attackers spiral, deck137's single worst reply this
corpus is now a BLOCKING decision under lethal pressure. deck59 t11, life 6, facing four 2/2 persist
attackers (8 lethal). Two blockers offered: B1 Human (1/1), B2 Lovestruck Beast (5/5). The model's
FIRST line was `BLOCKS: B2:A1, B2:A2, B2:A3, B2:A4` — an ILLEGAL one-blocker-onto-four-attackers
assignment (the INVERSE of the wave-23 gang-block trap), despite the prompt's explicit line
"Assign each blocker to AT MOST ONE attacker (a creature cannot block two attackers)." It then spent
13.6k chars re-deriving that constraint from scratch, arriving at the CORRECT plan (`B2:A4` Beast on
the Fear creature + `B1:A1` Human chump, survive at 2). Recorded outcome chosen_text =
"Human blocks Safehold Elite" (B1:A1 only) — matches neither the illegal first line (B2:A1) nor the
model's full corrected 2-block plan, i.e. a PARTIAL/AMBIGUOUS application of a reply carrying
CONFLICTING BLOCKS: lines. Two routes:
- GUIDE (deck-relevant — deck137 is the stabilize deck): add a blocking-under-lethal line so the
  model doesn't re-derive "at most one attacker per blocker" mid-spiral. Shipped in strategy.txt.
- LEDGER (parser owner): the blockers parser's handling of a reply with an illegal first BLOCKS
  line + corrected later lines is worth a look — the recorded single-chump outcome doesn't match the
  model's coherent final plan. Repro: deck59 s39. See notes.md (don't over-claim mechanism; flagged).

## repetition_penalty=1.05 — second-corpus verdict at this seat: SAME (no prose harm, no spiral cure)

Consistent with wave-23. The penalty did NOT tame this seat's worst spiral (13,642-char lethal-block,
BIGGER than any wave-23 attackers spiral). No prose degradation — reasoning stays coherent and
lands correct (adventure mechanics, Fear, wither, persist, convoke all reasoned soundly). The KEY
observation: the worst spiral RELOCATED from the attackers seam (fixed by the guide's attack floor)
to the blockers seam — evidence that a GUIDE/representation fix eliminates a spiral where the 1.05
penalty cannot. Recommend the guide's new blocking line + keep the decode-side guard; the penalty is
neither helping nor hurting at this seat.
