# deck137 -- Selesnya (GW) Adventures -- wave 26 (guided ROUND 4)

Binary 77e91ef4e (wave-26 step-1 batch: commits 79e39511f + 77e91ef4e), corpus
matchups-20260725-115216, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: 1785*-ai_baka_deck137-0x*-vs-{102,131,18,22,27,133}.jsonl.

Outcomes: **3/6, 0 timeouts, 0 draws** (up from wave-25's 1/6+draw).
- WIN vs102 (clean kill, 25/-6 t15) -- deck102 collapsed 4/6->0/6 corpus-wide.
- WIN vs131 (adj-ahead, 8/1 t12) -- Izzet burn.
- WIN vs18 (adj-ahead, 8/6 t12) -- Kithkin weenie aggro; the adventure-body fix's key game.
- LOSS vs27 (KILLED -8/19 t10) -- chronic late-stabilization race loss.
- LOSS vs22 (KILLED -1/4 t11) -- ahead on board (ground Giants to 4) then blown out by a
  ~19pt ramp alpha-strike in one turn; lost the race by one turn, not a durdle.
- LOSS vs133 (adj-behind, 4/18 t14) -- ground out by the corpus 6/6 monster.

Corpus health at my seat: **0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo.** Only
fallbacks are my 15 by-design convoke defers (job 3). No priority fallback at this seat (the
corpus's 1 non-defer fallback is deck133's).

## (1) ADVENTURE-BODY FIX -- FIRST CORPUS: VALIDATED. Zero Lovestruck-class declines.

The wave-25 fix (a battlefield [adventure] creature IS its printed body; ATTACK-list inverse)
was written from the wave-25 deck27 s21 misplay (declined Lovestruck's 5/5 swing while a live
1/1 Faerie Guidemother was on board, misreading the [adventure] tag as "not a real body").

**The direct successor situation recurred four times this corpus and the model swung every
time.** vs18 game: Lovestruck Beast (5/5) [adventure] was on my battlefield alongside a 1/1
Faerie Guidemother (its unlock body) at every attack window:
- vs18 **s21 t6** (life 16/19): opts A1=Faerie(1/1), A2=Lovestruck(5/5); chose=2 (both). Reply
  reasons "My Lovestruck Beast (5/5) can easily block and kill the Cohort... deal 6 total" --
  treats Lovestruck as a real 5/5, no "not a body" misread.
- vs18 **s25 t8** (13/13), **s31 t10** (12/12), **s42 t12** (8/6): Lovestruck offered as A2 with
  the 1/1 present every time; chose=2 (swing Lovestruck) every time.
No "adventurer face, not a real 1/1" reasoning appears anywhere in the corpus. Additional
adventure-body attacks landed clean across all games (Giant Killer, Faerie Guidemother,
Shepherd of the Flock as battlefield 1/2, 1/1, 3/1 bodies): vs102 s18/s25/s29/s32/s36,
vs131 s25/s36/s49/s59/s65, vs22 s18/s28/s41/s49. The one attack-DECLINE with an adventure
body offered -- vs27 **s34 t9 life 3/19**, Faerie Guidemother(2/2) -- is a CORRECT ROLE-CHECK
stabilize hold, NOT a misread: the reply reads "My board... combined power of 7 ... 10 power vs
my 3 life... keep my creatures untapped to block and survive" -- it treats Faerie as a real 2/2
and holds it as a blocker against lethal, exactly the strengthened guide's stabilize behavior.
**Fix status: VALIDATED at first corpus.**

## (2) BLOCKERS-SEAM ROUND 2 -- floor held 4/5; ONE breach (vs18 s36), a real correctness dent.

Five blockers records. Four are short and clean:
- vs102 **s19** (1312 ch, `B1:A2` -- block-and-kill), **s30** (1176 ch, `B2:A1` chump-with-value).
- vs18 **s16** (808 ch, `B1:A1` -- Lovestruck kills a 1/1).
- vs22 **s31** (3403 ch, `B2:A1` -- Giant Killer neither-dies block). Moderate spiral, legal.

The BREACH -- **vs18 s36 t11 (life 12), 10,817 chars, chose=0 (no blockers):**
- Options: A1=Zealous Guardian(2/2 first strike), A2=Soldier(2/2 first strike),
  `B1. Lovestruck Beast (5/5) - may block A1 (you kill it, your blocker lives), A2 (you kill
  it, your blocker lives)`. The engine OFFERED a free kill-and-survive block on either attacker.
- First coded line: `BLOCKS: B1:A1, B1:A2` -- the ILLEGAL one-blocker-onto-two shape (the exact
  wave-24 pattern the floor was meant to kill), self-corrected mid-reply.
- Final line: `BLOCKS: none`. The model DECLINED the free block, took 4 (dropped to 8), on the
  belief that Lovestruck "went to graveyard" / "attacked Turn 11 ... it is tapped ... cannot
  block." A **wrong decline of free removal**, driven by re-deriving Lovestruck's tapped state
  from the game log and OVERRIDING the engine's B-offer.
- **Root cause (representation): duplicate-name instance ambiguity.** At the prior seq (vs18
  **s35 t11**) the battlefield genuinely shows TWO Lovestruck Beasts -- one
  `(5/5) [adventure] [tapped - untaps and can attack next turn]` (attacked t10) and one
  `(5/5) [adventure]` (untapped) -- with NO instance disambiguator. The B-line "B1. Lovestruck
  Beast (5/5)" gives no way to tell it points at the UNTAPPED copy, so the model resolved the
  name collision against itself and passed a free block. See notes.md R-DUPLICATE-NAME-INSTANCE.
- Impact: value leak (passed free removal + took 4), but deck137 WON this game (adj 8/6), so not
  fatal. Verdict: the blockers floor held for the LETHAL case it targets; it does NOT cover the
  non-lethal value-block seam when a same-named tapped creature muddies instance identity.
- GUIDE MITIGATION SHIPPED: the BLOCKING paragraph now states the offered B-list is authoritative
  -- a creature on a B-line is untapped and can block this turn even if the log says it attacked
  or a same-named creature is tapped; take the "you kill it, your blocker lives" block. This is
  the blockers-side analogue of the wave-25 attack-list inverse (skill.md).

## (3) CONVOKE DEFERS ROUND 4 -- 15 again, HARMLESS (brief's split confirmed exactly).

All 15 fallbacks corpus-wide are deck137's convoke tap-payment defers (kind=defer,
deferred_to_heuristic, latency=-1, empty prompt+reply). Distribution: **deck22 x9, deck27 x4,
deck131 x2** -- matches the brief's (9/4/2) exactly. No stranded attacker: on every defer turn
deck137 still attacked with a full board -- vs22 defers t6/t8/t10 and attacks landed s28 t6
(4 attackers), s41 t8 (4), s49 t10 (3). Fourth consecutive corpus of harmless defers; deck137
remains the SOLE defer seat (convoke is still a single-deck mechanic). No ledger item.

## (4) SPIRAL-RELOCATION CEILING CHECK -- ceiling CONFIRMED with data; one correctness dent.

Reply-length profile at my seat (245 decision replies): **p50=1107, p95=9919, max=14603.**
Wave-25: five 12k-15k replies, max 14,987. The ceiling is STABLE (max 14,603 vs 14,987).
Nine replies exceeded 10k this wave (vs 5 last wave) -- MORE long spirals, because deck137
played longer/wider games (more turns, more generic seams hit), not because any floored seam
regressed. Seam classification of the 9 (>10k):
- convoke casts: vs131 s41(13587), s55(11889), s63(11418); vs22 s51(14350).
- land drops: vs22 s46(14024), s33(10907, >6k tier); vs131 s19; vs133 s24.
- casts/reveals/priority: vs18 s23/s28/s35; vs22 s17(reveal, 46 opts); vs133 s11.
- shockland pay-2: vs131 s52(12090) -- spirals EVEN WITH the c2 annotation present.
- blockers: vs18 s36(10817) -- the one dent.
All but s36 are ubiquitous unfloorable seams (every land drop, every convoke cast, every
priority window) exactly as PROPOSAL 1 predicted; all landed CORRECT. **s36 is the exception
that refines the ceiling clause: the residual spiral is latency-only UNLESS it rides a
representation ambiguity (duplicate-name instance), in which case it can also produce a wrong
commit.** Ceiling doctrine "floor high-stakes seams, the rest is latency" holds; the amendment
is that a representation gap under a generic seam converts latency back into a correctness risk
-- fix the representation, not the seam. See skill.md.

## (5) c1-c5 BEHAVIOR SHIFTS.

**c2 (ETB pay-or-tap annotations NOW FIRING) -- VALIDATED at my shockland surface; RESOLVES the
wave-25 R-SHOCKLAND-ETB-UNLABELED residual.** All 7 Temple Garden pay/tap menus now render the
per-option consequence tag: `pay 2 life [this permanent then enters the battlefield UNTAPPED -
usable (tap for mana / attack) this turn]` / `tap [... enters the battlefield TAPPED - unusable
until your next untap step]`. Decisions: vs131 s3(t1)/s52(t11), vs18 s3(t0)/s18(t6), vs22 s3(t0),
vs27 s13(t5), vs133 s16(t9) -- **ALL chose=1 (pay 2 life), all parsed, no stale_echo, no
double-annotation.** The wave-25 nit (bare `1. pay 2 life / 2. tap`, no consequence tag) is gone;
the option text is now self-describing. RESIDUAL (pure cosmetic): the standalone menu subject is
still `Choose an option for :` (empty) in the s3-type cases -- but with self-describing options
this no longer matters. No double-annotation confusion observed. Castle Ardenvale's
"enters tapped unless you control a Plains" rides its card text (vs102 s6 hand list, s26
create-human activation) -- correct; Castle has no player pay-choice so no c2 menu applies.

**c5 (may-ask renders): UNEXERCISED at this seat.** No may-triggers surfaced in deck137's 6 games
(no "Triggered ability of X - you MAY..." options). deck137 has no convoke/adventure may-ask
surface. No new confusion, nothing to validate.

**c1 (target-preview) / c3 (PT-pump delta):** no targeted-removal-with-hidden-target or activated-
pump seat at deck137 this corpus that exercises these; Giant Killer's Chop Down tapper activations
(vs22 s38/s44 priority) rendered targets fully (`Tap ... targeting Skyraker Giant` /
`targeting Calamity Bearer`), no truncation. Nothing anomalous.

## (6) c4 PARSER-HARDENING CORPUS GATE -- PASS. 11 reversals, all parsed to the model's final line.

Eleven replies opened with a first coded line that differed from the final coded line
(natural-stop / b2 reversals). In EVERY one the parsed `choice` equals the LAST coded line --
zero drops, zero mis-picks, zero fallback-where-parseable:
- vs102 s6 (3->1), vs131 s19 (2->1), s32 (3->1), **s63 (2->4)**, vs18 s23 (2->1),
  **s36 (B1:A1,B1:A2 -> none)**, vs22 s21 (1->4), s38 (0->1), s44 (0->2), vs133 s14 (3->1),
  s26 (1->2).
The example-echo exclusion and line-anchored selection did NOT drop any legitimate answer at
this seat. s36 is NOT a hardening misfire: the parser faithfully took the model's stated final
`BLOCKS: none` -- the error there is the model's reasoning (job 2), not the parser. b2 remains a
spiral safety net (the reversals are the model revising to its considered final answer, which the
parser honors); the one reversal that landed on a WORSE-but-legal answer (s36) is a model belief,
not a parse failure. Gate: PASS.

## (7) RECORD DECOMPOSITION 1/6 -> 3/6, AND ROTATION.

The +2 wins are ~HALF pool-shift, ~HALF genuine:
- POOL (external): deck59 + deck109 -- wave-25's two aggro KILLERS of deck137 -- rotated OUT.
  deck102 COLLAPSED 4/6->0/6 corpus-wide, converting a wave-25 deck137 loss into a wave-26 win
  (clean 25/-6). Neither is a deck137 play improvement.
- GENUINE (play): the adventure-body fix held and materially helped the vs18 win (Lovestruck
  swung 4x for real damage); the ROLE-CHECK stabilize executed correctly (vs27 s34); zero
  comprehension misplays in the three LOSSES.
- CAVEAT: 2 of 3 wins were ADJUDICATED-AHEAD at the turn cap (vs131 8/1, vs18 8/6), not decisive
  kills. Only vs102 closed the game. The beatdown still does not reliably CLOSE.
- LOSSES UNCHANGED in shape: vs27 = chronic late-stabilization (20->15->10->3->dead, opp never
  below 19); vs22 = ahead on board (opp to 4) then one-turn ~19pt ramp burst; vs133 = ground out.

**Play-quality convergence -- NAMED PATH.** The COMPREHENSION surface HAS converged: 0
true-fallbacks, adventure-body fixed, c2/c4 clean, blockers-lethal floored. What has NOT
converged is CLOSING SPEED / early board pressure -- the deck wins by being ahead at the cap and
loses the same tempo races one turn too late. That is a DECK-LAYER play problem (curve/sequencing/
when-to-commit), not a parser or representation problem. This is the live new-work signal.

**ROTATION VERDICT: does NOT rotate.** Guide WAS modified this wave (the blockers B-list-
authoritative clause, from the s36 breach). Fresh new-work signal EXISTS: (a) the s36 blockers
value-decline + R-DUPLICATE-NAME-INSTANCE representation candidate (new); (b) the closing-speed
play gap named above (unresolved deck-layer work). Keep in pool. See notes.md.

## repetition_penalty=1.05 -- fourth-corpus verdict at this seat: SAME. No prose harm, no spiral cure.
Reasoning stays coherent across all nine 10k-14k replies (adventure, convoke, wither, lethal math
all sound). The penalty neither degraded prose nor tamed the generic-seam spiral. Reconfirms: a
guide/representation commit-rule cures a spiral where the sampler cannot; the residual moves to
seams a guide cannot reach and becomes latency -- except where a representation gap (s36) turns it
back into a correctness risk.
