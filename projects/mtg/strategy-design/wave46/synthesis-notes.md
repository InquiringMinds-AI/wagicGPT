# Wave-46 synthesis (2026-08-26) — THIRD FAIR-HAND CORPUS

Inputs read, in order: `wave46/review-carry.md`; `wave45/strategy-writing-skill.md` (the
CURRENT edition, amendments 1-67); `wave45/synthesis-notes.md` and `wave45/engine-ledger.md`;
the four seat files (`seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`,
`seat-engine-narration.md`); and seven `deckN/{skill,general-suggestions,edit-texts}.md` +
`strategy.txt`.

Outputs: `wave46/strategy-writing-skill.md` (wave-45 edition verbatim + **amendments 68-87**),
`wave46/general-strategy.md` (R1-R21 layer-routed + the SHIPPED table + measurement items +
PASS/KEEP), `wave46/engine-ledger.md` (the wave-47 docket, both discharge tables, fallback
classes, carried items), and the boundary findings below.

**Ownership (owner doctrine, 2026-08-26).** The deck REVIEWER owns every change to a
`deckN/strategy.txt`; synthesis owns the skill and the general strategy; the CORE LOOP —
engine mechanics, LLM UX, bugs — is the purpose the reviews feed, and guide/skill polish is a
cheap side quest that never gates it. **No guide file was edited by this step.** Step 4 was run
in full and is reported below as findings for the reviewer lane. **No deck-content question was
routed to the owner and none is open** — every one the seats raised was decided in the reviewer
lane from the corpus and is recorded in `engine-ledger.md`.

---

## The headline: four docket items shipped before this file was written, and the wave's two worst losses were decided before turn one

**(a) THE LOOP OUTRAN ITS OWN LEDGER.** Four wave-46 findings are already fixed on master
(`a7310f01c` + the pending hand-count commit): the CHOOSE_MENU header name ladder and its
name-gated rescue, the ask exemplar drawn from option 1, the repeat-activation tag from the
FIRST repeat, and the in-game `Your hand (N cards):` count. Between them they close **4 of the
corpus's 6 fallbacks**, the corpus's single 900-second decision, and the root cause of
deck125's worst-conforming rule (X sizing, 4/17 — the pilot was being asked for a number the
prompt did not print, and 3 of the 4 replies that stated it stated it wrong). They are recorded
as SHIPPED with wave-47 predictions and are **not** on the docket.

**(b) A CORRECT, KEYED, IMPERATIVE MULLIGAN STOP LOST TWO GAMES ON PLACEMENT.** The wave-45
boundary pass gave deck123 exactly the text amendment 67 asks for — and put it at STEP 1 of the
MULLIGAN section, line ~340 of a 424-line file, for the ask that fires before turn one. Two of
six games ran the chain to **ZERO cards** (seven mulligans, entire hand bottomed, 42-0) and to
**ONE card** (six mulligans, one decision made all game, dead turn 13); nine looks at
`(keeping 5)` or lower were answered Mulligan holding at least one land, three of them holding
three lands and a coverage line naming castable spells. Nothing about the wording would have
improved with another pass. Two placement facts caused it, and both are now skill amendments:
the rule for the first ask belongs at the top of the FILE (69), and the pre-game chain is the
one place in the loop where an identical ask repeats with the pilot's own carried plan re-shown
as an answer to it — deck123 wrote "Mulligan to find a hand with 4+ lands" at look 2 and
re-emitted that exact sentence as its reply at looks 3, 4, 5, 6 and 7. The render half is
docketed as R9 (price the row; re-scope or omit the carried plan on pre-game asks). **The
reviewer has already hoisted it to RULE -1 at line 9 and named the carried-plan trap by its
rendered string** — verified in this pass.

**(c) THE THIRD STRUCTURAL FINDING IS AN ORDERING ONE, FOR THE SECOND WAVE RUNNING.** Six of
this wave's twenty amendments are about where a sentence sits, not what it says: an exception
printed below its own blanket imperative (70), an unconditional ladder rung deleting every
conditioned rung beneath it (70), an absolute with no lethal carve-out above it (71), a
governing number written at the bottom of the ladder it governs (79), a first-ask rule buried in
its own named section (69), and a first-match list that is not total (76). Every one cost a game
this corpus. None is a knowledge problem, and all six are cheap.

**(d) THE CROSS-DECK STOP MOVED, 0/7 -> 2 held of 5.** The per-creature stop on
`(your attacker dies, their blocker lives)` was re-formed three different ways in wave 45,
per amendment 53's requirement to diagnose per guide. Both of the clauses named hardest —
deck152's vigilance clause and its lifelink clause — **held on their first live windows**, and
deck152's lifelink-bound row was the row that was HELD. The three residual loopholes are again
three DIFFERENT arguments, which is amendment 53 confirmed rather than refuted: a
friendly-sibling theory ("Triumphant Adventurer blocks Fate Unraveler"), a stop never consulted
at all, and a collapsed-tag rule losing to "she flies and tramples". Amendment 77 turns the
first into a principle-form refutation, which deck146's reviewer has already shipped.

---

## What merged into the skill (68-87)

**Convergence merges (>= 2 seats):**
- **70** an exception lives above the imperative it excepts; an unconditional rung deletes the
  conditioned rungs beneath it — deck126 S5 + deck162 S3 + deck146 S2, three losses.
- **72** every rule input is a printed number or tag — no derived counts, no running tallies, no
  mood words — deck125 S1 (a six-row conformance table) + deck152 S3 + deck146 S3.
- **78** a posture rule selects among what the prices allow — deck152 S1 (the deciding loss) +
  deck162's mirror (19 -> 0 in one combat, having neither rule).
- **82** a rule keyed to a rendered token must branch on its absence — deck162 S2 + deck126 G-E.
- **87** an engine-side suppression changes a denominator, not a rate — deck126 S7 + the engine
  seat's own falsified take-rate prediction.
- **69** the first-ask rule goes at the top of the file — deck123 S46-B, and the placement family
  it belongs to (70/79) is three-seat convergent.

**Decisive-falsification merges (one seat, evidence closes it):**
- **68** an unreachable release condition is an unconditional rule — a release naming two
  enchantments the deck cannot remove; 42-0.
- **71** a lethal carve-out goes ABOVE the absolute — opponent at 1 life, lethal animation, PASS.
- **73** collapse a multi-threshold branch to one constant — 6/11, 3-4/7, 4/7 across three waves.
- **74** a class rule must name members — 0/39 named vs 2/39 unnamed, eight lines apart.
- **75** a by-name property list is falsified by the board — the Urborg board, 5/5 wrong casts.
- **76** a first-match list is total or it is not a list — 9 of 27 breaks are unrankable cards.
- **77** refute the principle, not the instance — the friendly-sibling shape.
- **79** the governing number is written first, as an instruction about what to emit — a bare
  `BLOCKS: none` at N=8 against a floor of 9, second corpus running, the margin it died by.
- **80** every floor names its regime — the four-card-hand land floor.
- **81** run a coverage diff before tuning — three losses, three absent sections, three siblings
  that have them.
- **83** a role rule is defined by a count, not a card name — Master of the Feast.
- **84** a threshold's sunset is two-sided and its WHY is re-measured — 6 crossings AND 6
  declines; the WHY re-measured TRUE and that is what killed the rule.
- **85** the second identical failure ends the guide lane — Starstorm cycling, twice, cited by
  rule number while broken.
- **86** when guide and render lead clause push the same way, state the other direction and file
  the render item — X=1 over a strictly free X=2; 19 to -31.

---

## What was REJECTED, and why

- **"Loosen or delete deck162's Rule 2 engine brake (5/7) and deck123's blocking STEP-1 letter
  (1 divergence)."** Both are VIOLATED-UNPUNISHED at n<=2 with the odds arguable. #45 forbids
  closing on a low-power sample and #64 gives the observation a home that is not "evidence for
  loosening". Both stand; the deck162 seat recorded the temptation explicitly, which is the
  right handling.
- **"The 2-of-4 X-collapse failure means `#W46-8` FAILED."** It did not. `#W46-8`'s own metric —
  did the model take the priced reference row above a collapsed cheaper one — is **4 of 4
  correct**, and the bare wave-45 `same as X=` shape is at zero. The 2-of-4 is a DIFFERENT
  metric (answering the TOP of a collapsed equal-kill run) and is routed to R5, whose real
  defect is that the annotation labels only the minimum.
- **"deck126's `Add N green` discipline regressed, 2/39 -> 1/16."** Different populations: the
  auto-pass removed 180 windows before they reached the model, all at that seat. Restated as
  counts, docketed as a measurement rule (amendment 87), not as a finding.
- **"Teach the model that the collapsed tag / the `STOPS all N` clause / the `same as X=N` row
  is misleading."** Trust-doctrine violation. All three are TRUE strings; each is a render item
  with a prediction (R12, R5) and the guide-side answer is a rule keyed to the string as
  printed (52/82), never a hedge against it. The reviewers wrote none — the boundary sweep
  below confirms it.
- **"Add general skill prose teaching pilots to work around the missing draw-punisher summary,
  the bare modal menu, or the untagged A-line."** Same rule. Those are R1, R2 and R11; the only
  guide content that belongs is a rule that stays correct AFTER the render ships (deck130's
  `{effect:}`-keyed #0 RULE is exactly that shape and is correct as written).
- **"Route the mulligan floors / the token cap / the bottoming question to the owner."** Not
  raised this wave — the seats decided their own, which is the doctrine working. Recorded so it
  stays decided.
- **The deck123 vs125 matchup ceiling** (48 of 103 records "Cast nothing right now"; two Staff
  of Nin drained it from 44) and **deck130 vs126** (mono-red reach against a board that gains
  faster than it burns). Construction notes by their seats; no guide prose fixes a card the deck
  does not contain, and neither produced an adjudicable misplay.
- **deck126's mana-pool disappearance observation** (a floating `{g}{g}{g}` gone between two
  consecutive decisions in one phase, nothing spent). The seat is explicit that this is an
  OBSERVATION, not a diagnosis — it read four `Empty()` call sites, did not trace the
  phase/stack machinery and did not reproduce it. **Not docketed as a bug**; it is recorded here
  with its repro (`1787777938-...deck126-...-vs-...deck123.jsonl` turn 10, Main 2, seq 23 -> 24)
  so a wave-47 lane with an engine build can decide it. It matters because the guide layer
  teaches pilots to trust the pool line.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE, ranked: the draw-punisher summary line (R1) · the bare modal menu (R2) · the
converter's doubling folded into the price it doubles (R3) · manland offer-TIMING (R4) · the
X-pricing row's unlabelled maximum (R5) · planeswalker `{effect:}` truncation (R6) · own-side
`{effect:}` (R7) · the blockers-screen ranged collapse (R8) · the pre-game carried plan and the
unpriced Mulligan row (R9) · `{1}`/`{x}` vs `{c}` (R10) · the affirmative no-blocker tag (R11) ·
`STOPS all N ... THIS COMBAT` (R12) · the land-drop latency tail (R13) · two engine-side counts
(R14) · non-damage ANNOUNCE_X and the null X=0 (R15) · LOW: the number-word above ten (R16),
textless tokens (R17), the bare damage OBJECT (R18), the mana-only gate's 3 leaks (R19), the
target list's current-size clause (R20), the generic-mana clause (R21).
GUIDE-only, staying in guides after the render ships: deck130's `{effect:}`-keyed draw-punisher
rule, deck126's per-colour-count Lantern test, deck152's converter-vs-price precedence,
deck162's role-defined keep-it-home rule, and the collapsed-tag rules now in six of seven guides.
MEASUREMENT: five items, including the like-for-like restatement every auto-pass rate now needs
and the pre-registered blockers-width gate.
PASS/KEEP: 13 surfaces measured working, with **zero new render falsehoods found** — every open
item is an omission, a scope or an ordering.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT, not as edits. Sweeps: render-hedging vocabulary
(`invert|backwards|upside down|do not trust|misleading|is a bug|is wrong|the prompt says|ignore
the|unreliable|does not show|template error|off-by-one|may be wrong|not accurate`),
attractive-nuisance substrings (`can attack|can block`), mood-word vocabulary (amendment 72c),
mulligan/bottoming structure against 66/67/69/80, collapsed-tag and `GANG BLOCK` coverage
against 52, ordered-cast-list coverage against 62/76, defender-allocation coverage against
54/77, threshold-WHY coverage against 61, and every card, mana and colour claim behind a
wave-46 edit. Card facts were verified with `/usr/bin/grep` against
`projects/mtg/bin/Res/sets/primitives/*.txt` only — no Scryfall fetch, and no finding turns on a
rules subtlety where the two could differ.

**TRUST DOCTRINE: CLEAN, all seven guides.** Every hedging-vocabulary hit is either a quotation
of the render's own true string (`deck130:71` "the prompt says so on the [land drop] line",
`deck123:363` / `deck130:284` quoting the LETHAL header, `deck162:296` quoting the mulligan
price line) or a naming of the MODEL's own fabricated argument, which #28 requires
(`deck126:164` "You have got this wrong: offered ... you blocked the Wolf and took 3";
`deck152:138` "...and it is backwards" against the model's own quiet-board excuse;
`deck152:165` "THE PARENTHESIS WINS"; `deck123:91`). **No guide anywhere teaches doubt of a
rendered value.** Wave 44's one violation stays fixed.

**CARD FACTS: CLEAN.** Verified this wave against the primitives, by name:
`Underworld Dreams` (`{B}{B}{B}`, Enchantment, `auto=@drawfoeof(player):damage:1 opponent`),
`Fate Unraveler` (`{3}{B}`, **Enchantment Creature**, same auto line — so it is correctly both a
draw punisher AND a creature for deck130's cycling gate), `Ob Nixilis, the Hate-Twisted`
(`{3}{B}{B}` Legendary Planeswalker, same auto line) — deck130's three-name draw-punisher list
and its summed "3 damage per draw" are **exact**;
`Damnation` (`{2}{B}{B}` Sorcery, `auto=bury all(creature)`) and `Sanguine Bond` / `Exquisite
Blood` (both `type=Enchantment`) — deck123's "Damnation kills creatures and nothing else" and
"nothing in this deck removes an enchantment" are **exact**;
`Tribute to Hunger` (`{2}{B}` Instant, `ability$!name(sacrifice) notaTarget(creature|
mybattlefield) ... !$ targetedplayer`) — it targets a PLAYER and takes a creature of their
choice, so deck123's "it cannot remove a planeswalker" and its N-check are **exact**;
`Urborg, Tomb of Yawgmoth` (`auto=lord(land) transforms((swamp))`) and `Chromatic Lantern`
(`lord(land|mybattlefield) transforms((... add{B} ...))`) — deck126's by-name black-source list
is **correctly deleted** and its replacement, keyed to the render's own per-colour clause
(`{B} 1 or no {B}`), is right on an Urborg board where the old list was false;
`Starstorm` (`mana={X}{R}{R}`, `auto=damage:X all(creature)`, `autohand=__CYCLING__({3})`) —
deck130's "casting it at X=0 is not cycling" is **exact**;
`Silverquill Silencer` (`{W}{B}`, **3/2**) and `Emeria's Call` (`{4}{W}{W}{W}` Sorcery with an
`Emeria, Shattered Skyclave` land back face) — deck146's two new cast-order entries are
**exact**, including "count them as SPELLS ONLY: a hand with two lands and an Emeria's Call is a
TWO-land hand";
`Master of the Feast` (`{1}{B}{B}`, **5/5**, flying, `@each my upkeep:draw:1 opponent`) —
deck162's role rule and its "5/5 flier" line are **exact**;
`Shield Sphere` (`{0}`, `@combat(blocking) source(this):counter(0/-1) all(this)`),
`Fall of the Gavel` (`{3}{W}{U}`, `fizzle` + `life:5`), `Silverquill Command` (`{2}{W}{B}`
Sorcery), `Hive of the Eye Tyrant` (`type=Land` + `{3}{B}` creature), `Lolth, Spider Queen`
(`{3}{B}{B}`). **No card-fact defect found.**

**CLOSED from wave 45's boundary pass** — do not re-file: **B1/B3** (mulligan card-count stops
at deck125 and deck152) present; **B2/B4/B5/B6** (bottoming priority orders with land carve-outs
at deck125, deck152, deck126, deck130) present; **B7** (deck130's coverage-line keep) present
and **PASS 6/6 live, including two one-land sevens**; **B8-B11** (collapsed-tag / `GANG BLOCK`
rules at deck123, deck126, deck130, deck146) present, and deck126's got its **first live
adjudication at 2/2**; **B12/B13** (deck146's single ordered cast list) present and **7/8 on its
headline metric, up from 0/1**; **B14** (deck123's recorded falsifier) recorded, still
unreachable-in-pool, and a DIFFERENT falsifier landed instead (amendment 68); **B15** (threshold
WHYs, amendment 61) present in **all seven** `edit-texts.md`, two of them under an explicit
`THRESHOLD WHYS (skill amendment 61)` heading. That is the whole wave-45 boundary list closed in
one wave.

**Findings, by file. Each names the amendment it breaks and a suggested wording; applying them
is the reviewer lane's call, and a reviewer who disagrees on the evidence should say so rather
than apply it. Two of the five carry no demonstrated failure at their own seat this corpus and
say so.**

| # | file : line(s) | amendment broken | finding | suggested wording |
|---|---|---|---|---|
| **C1** | `deck125/strategy.txt` : 298 (of 395) · `deck126/strategy.txt` : 267 (of 330) · `deck152/strategy.txt` : 322 (of 419) · `deck162/strategy.txt` : 278 (of 344) · `deck130/strategy.txt` : 299 (of 384) | **69** (the rule for the FIRST ask goes at the top of the file) | Five guides still carry the mulligan section deep in the file, for an ask that fires before turn one. **PREVENTATIVE, not demonstrated at these seats** — deck125 and deck126 took one mulligan each and both keeps were guide-legal; deck130 kept 6/6; deck152's one STEP-1 violation won its game; deck162 took zero. But the demonstrated failure at deck123 was a *correct text in the wrong place*, its cost was two games out of six, and the two guides whose sections sit lowest (deck152 at 77% of the file, deck162 at 81%) are exactly the two whose mulligan sections are least tested. deck123, deck146 and deck130's own `#0 RULE` show the fix costs nothing. | Do not move the section. Hoist a short **RULE -1** to the top of the file, above the deck thesis, in deck123's shipped form: *"RULE -1 - THE FIRST ASK OF THE GAME. The ask reads 'Pre-game mulligan decision (London mulligan)' and from the second look on it prints its own price: '(keeping M)'. READ THE M. M is 5 or less: answer 'Keep this hand' the moment the hand holds ONE LAND - you are DONE, do not read another rule anywhere in this file before answering. M is 6: keep unless the hand is zero lands or all lands. M is 7: the comparison in the MULLIGAN section applies."* Then add the carried-plan clause below it (C2). |
| **C2** | all five guides in C1 | **69** (a rule for a repeated identical ask must survive its own carried plan) | None of the five names the trap that ran deck123's chain to zero: the pre-game ask re-shows `YOUR PLAN (as you last stated it)`, and a plan sentence about a hand already shipped answers the question again at every look. This is the mechanism, not the wording, and it is invisible until a chain starts. The render half is docketed (R9) but the guide branch is owed either way. | Append to the hoisted RULE -1: *"THE CARRIED PLAN IS NOT EVIDENCE ABOUT THIS HAND. From the second look on, the ask shows you 'YOUR PLAN (as you last stated it)', and that plan is the one you wrote about the hand you already threw away. Each look is a NEW seven with its own 'counted by the engine: N lands, M spells' header. Decide from that header. If your plan tells you to mulligan and the header says two or more lands, the plan is out of date - say so and keep."* deck123's line 20-27 is the tested text to copy. |
| **C3** | `deck126/strategy.txt` : 314 (DECIDING SITUATIONS, "Your Vampires on the attackers list outnumber the 'they have N untapped creatures' on the tag: attack with all of them - the surplus connects.") | **54** (the defender chooses which attacker to block), **29** (a counting rule names the class that defeats it) | A pure body-count rule in the closing list — the clause nearest the decision (#24) — with **no statement anywhere in the guide that THEY allocate the block**: a sweep for `they (choose\|pick\|decide) which` / `the defender (chooses\|picks)` returns **0 hits in deck126**, against 3 in deck146, 1 in deck152 and 1 in deck123. deck146's wave-46 break was exactly this falsehood in a new shape and cost a card; deck126 attacks with 1/1 Vampires, which any single block eats. The list line is also stricter-looking than the body rule it summarises, so #51 applies too. | Append to the line, in the vocabulary the guide already uses: *"- but the surplus is a NUMBER of bodies, not a choice of which ones live. THEY pick which attacker each blocker stops, and they pick the one their block wins. Send when the surplus is worth more than the biggest single Vampire you can lose, and never send the Vampire that is your only lifelink body on a board with a Sanguine Bond of theirs."* |
| **C4** | `deck146/strategy.txt` : 169-172 · `deck152/strategy.txt` : 52-55 (the "(neither dies)" outnumber clause) | **70** (an exception lives above the imperative it excepts) | Both guides carry the correct refutation — `OUTNUMBERING THEM DOES NOT PROTECT A CARD` at deck146:223 and deck152:123 — **fifty to seventy lines BELOW** the clause that tells the pilot to count ("send it only when the attackers you are sending OUTNUMBER their untapped blockers"). That is the exact geometry amendment 70 was written from, and the reviewers fixed it in their BLOCK sections this wave while leaving it in the ATTACK sections. It has not cost a game in this shape yet (deck146's break came through the sibling clause, not this one), so this is ordering hygiene on a rule that is one wave from being tested. | Move the refutation ABOVE the count in both files, and make the count conditional on it: *"THEY choose which attacker each blocker stops, and they choose the one their block wins - so a surplus of attackers protects the SWING, never a particular card. With that settled: send an all-'(neither dies)' creature only when the attackers you are sending outnumber their untapped blockers, or when it has flying or menace they cannot answer."* Keep the one-name corollary and the Shield Sphere exception where they are. |
| **C5** | `deck126/strategy.txt` (whole file) — 0 ordering markers; also `deck123`, `deck152`, `deck125` | **62** / **76** (one ordered first-match cast list per guide, total over the deck's castable cards) | Wave 45's **B13** asked deck126 and deck130 for an ordered cast list. deck130 shipped one (`THE WHOLE CAST ORDER, ONE LIST`, 2 markers) and deck126 did not — second wave carried. The evidence for the ask got stronger this corpus, not weaker: deck146 shipped one and its headline metric went **0/1 -> 7/8** with overall first-match at 46/73, deck162's single numbered list runs **20/21** (the pool's best per-deck metric), and amendment 76 explains the residual (a list that omits a castable card teaches the executor to stop reading it). deck126 lost a wave-45 game casting a combo half over the removal that answered the only creature on the board. **Reported at lower confidence for deck123, deck152 and deck125**, which produced no adjudicable cast-order misplay this corpus and whose casting rules are already keyed to option rows; deck152's deploy floor (50/50) may be its equivalent, and the reviewer is better placed than this pass to judge that. | For deck126: one numbered list covering its main-phase cast asks, with every castable card matched and `Cast nothing right now` as the LAST entry — removal-with-a-live-target > the missing combo half > Idyllic Tutor when neither piece is out > Chromatic Lantern at `{B} 1` > a wall > cast nothing. Keep the existing card sections as detail hanging off its entries, per 76. |

**Not findings — checked so nobody re-files them.** deck123's RULE -1 hoist, carried-plan
clause, enchantment fact, power-vs-printed-life replacement stop and its lethal exception
(amendment 71) are all present and correctly ordered. deck146's Hive `CHECK 0` is a numbered
lethal check ABOVE the upkeep absolute, in the guide's own lethal wording, and names the
offer-timing fact — textbook 71. deck146's principle-form sibling refutation ("THEY choose which
attacker their blocker stops ... never write a plan in which one of your ATTACKERS 'blocks'")
is textbook 77. deck146's cast order is now total and closes with an explicit "EVERY CARD YOU
CAN CAST HAS A LINE ABOVE" plus `Cast nothing right now` as entry 9 — textbook 76. deck152's
block section opens with *"FIRST LINE OF EVERY BLOCK ANSWER, BEFORE ANYTHING ELSE: find the
header's 'you would be at N'"* — textbook 79 — and its converter section now states the
precedence outright ("the converter line NEVER licenses a swing the tags price against you") —
textbook 78. deck126's Lantern rule is keyed to the rendered per-colour count with the by-name
list deleted — textbook 75. deck130's `#0 RULE` scans the rendered `{effect:}` text for the
draw-punish class BEFORE naming the three cards as examples, and sits above the per-card cycling
table — textbook 72 and the right side of 75. deck125 hoisted its Emrakul absolute into DECIDING
SITUATIONS (line 363). The mood-word sweep returns **zero hits in all seven guides**. deck162's
mulligan stop is keyed to `"having already taken 1 mulligan"` rather than the `(keeping M)`
price line — a different string, correct content (it stops at the second look, which is what the
evidence supports), and untested at 0 mulligans; recorded as an alternate key, not a defect.
deck125 remains **NOT APPLICABLE** for amendment 52 (its only attacker is an Emrakul that was
never blocked).

---

## For the next wave

The reviewer lane closed the entire wave-45 boundary list in one wave and the five findings
above are smaller than any wave-45 set. The core loop is where wave 47's weight belongs: R2, R4
and R15 are now the oldest unshipped items with a game-losing repro each, R1 and R3 are new
HIGHs with two-seat evidence, and S1-S4's predictions must be adjudicated alongside the wave-47
lanes so the four shipped fixes are not assumed.
