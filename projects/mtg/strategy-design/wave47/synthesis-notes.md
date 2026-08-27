# Wave-47 synthesis (2026-08-27) — FOURTH FAIR-HAND CORPUS

Inputs read, in order: `wave47/review-carry.md`; `wave46/strategy-writing-skill.md` (the CURRENT
edition, amendments 1-87); `wave46/synthesis-notes.md`; the four seat files
(`seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`, `seat-engine-narration.md`); and
seven `deckN/{skill,general-suggestions,edit-texts}.md` + `strategy.txt`.

Outputs: `wave47/strategy-writing-skill.md` (wave-46 edition **verbatim** — prefix `cmp`-verified
— plus **amendments 88-102**), `wave47/general-strategy.md` (R1-R27 layer-routed with evidence and
a wave-48 falsifier each, plus the SHIPPED row, GUIDE-only routing, five measurement items and
PASS/KEEP), `wave47/engine-ledger.md` (the wave-48 docket D1-D19, both discharge tables, carried
items, fallback classes, the per-kind prompt-char delta and the lategame specimen), and the
boundary findings below.

**Ownership (owner doctrine, 2026-08-26).** The deck REVIEWER owns every change to a
`deckN/strategy.txt`; synthesis owns the skill and the general strategy; the CORE LOOP — engine
mechanics, LLM UX, bugs — is the purpose the reviews feed, and guide/skill polish is a cheap side
quest that never gates it. **No guide file was edited by this step.** Step 4 was run in full and
is reported below as findings for the reviewer lane. **No deck-content question was routed to the
owner and none is open.**

---

## The headline: the loop is the wave, and the corpus paid for it in one currency the docket can read

**(a) A LEGAL, RULES-CORRECT, UNBOUNDED LOOP KILLED A GAME AND THE SCREEN NEVER OFFERED A WAY
OUT.** One activated ability, 1,868 times in one turn, 1,871 single-option windows, **zero passes**,
no gameend — an invariant-00 failure at ~9 h wall, 48.2 M prompt characters and 5.7 h of measured
latency. Two more games ran the same loop 1,540 and 214 times and finished only because the loop
ran on the pilot's OWN turn and the attack step consumed it. Every existing brake was structurally
unreachable, and each for a different and instructive reason: lane L's decline cap arms only on a
decline (`mPassDeclineCount` increments when `choice == 0`), so a pilot that never passes never
arms it; the deadlock breaker keys on board state + question and this loop changes both, so it
detects NO-OP repetition while this is PRODUCTIVE repetition; the mana-only gate does not see a
token maker. The model was never confused — its plan named the exit and the turn was the
opponent's, so its own stated payoff was unreachable without passing, and nothing on the screen
said so. **Docketed as D1 with the three-affordance fix shape (F1 repeat-N row, F3 a real
`Pass priority` LAST row, F2 the tag stating that the turn will not advance) and with a hard
offer cap EXPLICITLY REJECTED** — a cap constrains the choice on exactly the two boards where the
loop is the deck's win condition, and the doctrine buys termination by giving a reachable stopping
act, never by deleting a legal row.

**(b) THE WAVE'S SECOND STORY IS THE COLLAPSE, AND IT IS ON THE WRONG SIDE OF EVERY SCREEN.**
R8's ranged collapse works and landed on the B-rows — whose maximum width in the entire corpus is
**five**. The width is on the A-rows (**233,662 chars / 1,525 rows** on an attackers screen;
**74,063 chars for two real options** on a blockers screen) and on target menus (**116,148 chars /
431 rows**, the same card's full text repeated 425 times). A-range collapse fired 0/38 and 0/109.
Priority prompts grew **+24.4%** wave-over-wave and are 72% of all decisions. By weight, the
wave-48 docket is a decision-value-per-token docket: D2, D3, D11 and D13 pay that back.

**(c) THE THIRD STRUCTURAL FINDING IS THE CARRIED PLAN, FOR THE SECOND WAVE RUNNING — AND THE
ENGINE'S OWN FIX PROVED THE MECHANISM.** Removing the carried plan from PREGAME asks produced the
cleanest pregame result the project has had: **8 mulligans corpus-wide, every one a single look,
zero chains, no seat below keeping 6**, against wave 46's nine third-look chains. The identical
mechanism then cost three decisions one layer down, at three different seats: an attackers window
(`152 vs146` seq 18, a plan restated over two `your attacker dies` tags), an ANNOUNCE_X menu
(`deck130 vs126` seq 37, an X fixed one screen before the kill lists existed), and deck123's edict
plans. Amendment 94 makes the refusal a per-STOP obligation; R10 files the render half.

**(d) FOUR OF THIS WAVE'S FIFTEEN AMENDMENTS ARE A REVIEWER CORRECTING THEIR OWN WAVE-46 EDIT.**
deck123's token stop was written on a quantity the action cannot move (88). deck146's CHECK 0 was
an exception with no observable and became the default answer within one corpus, 10 non-lethal
animations of 12 takes (93). deck146's lifegain rule was keyed to a string the render had changed
and to a count the render collapses (90, 91). deck125's X section asserted the absence of a field
that then shipped (99). That is the loop working — every one of the four is a falsifiable
prediction that came back FALSE and was traced, and none of them is a knowledge problem.

---

## What merged into the skill (88-102)

**Convergence merges (>= 2 seats):**
- **89** take a rule's quantity from the surface nearest the choice, the OPTION ROW first —
  deck125 S1 (a four-wave lane) + deck130 G47-5 + three lanes that reached ceiling with no guide
  change once a per-row field shipped.
- **93** an emitted token belongs to a RULE, never to a prediction's score — deck152 S-3 and
  deck146 S-2 arriving from opposite directions, with deck152 naming the collision itself.
- **94** a fresh-computed STOP outranks the carried plan, in one sentence, at the STOP — deck152
  S-4 + deck130's recorded-but-not-proposed observation + the seat-123-130 ANNOUNCE_X item.
- **88** a stop condition is testable only against a number the action moves — deck123 S47-1
  decisively, with deck125's and deck146's surface arguments pointing the same way.

**Decisive-falsification merges (one seat, the evidence closes it):**
- **90** quote the string the emitter actually prints, and check it against the corpus — a rule
  keyed to `(blocking trigger:` while the row printed `(lifelink:`; 11 life, one loss.
- **91** never key a rule to a count of items the render may collapse — same loss, second and
  independent defect; the threshold was unsatisfiable by construction on every wide board.
- **92** a total ordered first-match list is the only executable ordering; every conditional
  stated elsewhere needs a rank in it — Tribute to Hunger at exactly one creature, **0 of 5**,
  one loss, with the condition written, correct and unreachable.
- **95** write the tiebreak for two members of an elevated SET on one menu — deck162's only loss
  was scored INSIDE a rule at 100% compliance, and the compliance count cannot see it.
- **96** one source of truth per procedure; the card section points and never restates — deck130,
  second consecutive corpus, second card-section sentence deleted.
- **97** two absolutes whose conditions can co-occur need their precedence written at the
  collision; a hoist is the wrong repair for a rule losing a conflict — Emrakul 1/4 after being
  hoisted, with three bare replies as the distinguishing symptom.
- **98** a rule stated only as its exception fires only as its exception — Path to Exile 1/13 in a
  lost game, with Lightmine Field as the exact mirror at the same seat.
- **99** a guide states what the render prints and never what it does not — a bold sentence
  asserting the render's absence, falsified on 413/413 prompts.
- **101** a distribution heuristic must name the marked cases it does not cover — five blockers
  onto one 3/2 twice while two `[menace]` attackers walked in; no guide in the pool had a menace
  rule.
- **102** third-failure routing: shrink the rule and move the check onto the pilot's own PLAN
  sentence — the edict fact stated three times, third corpus, with the fact on the option row.
- **100** rendering a fact does not stop the model restating it (measurement rule) — the own-side
  `{effect:}` render shipped and the re-derivation rate went 15% -> 17%.

## What was REJECTED, and why
- **"Delete the `[repeat:]` tag's credibility / teach the pilot the tag is misleading."** Trust-
  doctrine violation. The tag is TRUE to its own words ("this turn"); measurement showed it
  RESETS at the turn boundary (seq 1099: N=650 at turn 10; seq 1565: N=14 at turn 11 on the same
  loop). Docketed as **D13** — a scope defect in an affordance, with the guide-side consequence
  reported to deck123 below, never as a hedge.
- **"Docket the `[repeat:]` 15-vs-1,540 as a counting bug."** Docketed, but with the mechanism
  CORRECTED by direct measurement rather than as reported: the seats read it as the tag counting
  one ability among several; it is that AND a per-turn reset, and the reset is what makes it
  useless as the loop's only brake. Recording the correction so nobody re-derives it.
- **"deck126's 5-1 -> 1-5 is a guide regression."** Three of its five losses have no guide
  content in them at all (a token loop, a countered control matchup, a 66-life flier board), and
  every decision class the seat can measure IMPROVED. n=6 cannot separate a guide effect from
  hands. Counts only, per amendment 87 and the new-baseline rule.
- **"deck126's `Cast nothing` discipline slipped, 24/24 -> 4/47."** The opposite: 38 of the 47
  had nothing on the menu but a duplicate Lantern, which the guide forbids, and only 4 had a
  matched entry. Reading the raw rate alone would have condemned a working edit. Restated as
  counts (amendment 87).
- **"Loosen deck162's Rule 2 brake (39/42) and deck152's counter-spreading rule (2/13)."** Both
  violated-and-unpunished in games that were WON, at n<=3. #45 forbids closing on a low-power
  sample and #64 gives the observation a home that is not "evidence for loosening". Both stand;
  both seats recorded the temptation explicitly, which is the right handling.
- **"Teach the pilot to compensate for the collapsed blocker tag / the untagged stack line / the
  missing `[DRAW PRICE:]`."** Trust-doctrine violation in each case: all three surfaces print TRUE
  strings. They are D4, D7 and D8 with predictions, and the only guide content that belongs is a
  rule that stays correct AFTER the render ships. The reviewers wrote none — the sweep below
  confirms it, with one exception noted as **B3**.
- **"Amend the skill to say guides should teach the pilot to pass priority."** Not written. The
  pilot demonstrably knows how (`CHOICE: 0 (pass)` twice in the dead-looped file, and twice more
  in prose that the parser then discarded). The affordance half is D1; the general lesson is
  amendment 88's.
- **deck162 S-6 ("a section unreachable for N corpora at a seat whose ARCHETYPE explains the
  unreachability should be moved behind a pointer").** PARKED, at the seat's own request and
  correctly: it needs a measurement of whether guide length affects compliance, which no wave has
  run. Recorded here so it is not lost — three of deck162's six sections are now UNTESTED for
  three consecutive corpora at a **winning** seat, and the seat is the pool's exit candidate.
- **The deck125-vs-deck123 matchup ceiling** (89 of 187 decisions answered `Cast nothing right
  now` under a Staff of Nin drain, fourth consecutive wave) and **deck152 vs deck125** (a 39-turn
  grind in which the deploy floor was satisfied at every window). Construction notes by their
  seats; no prose fixes a card the deck does not contain, and neither produced an adjudicable
  misplay.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE, ranked: the unbounded loop's stopping affordance (R1) · the ranged collapse on the
wrong side of combat (R2) · uncollapsed target menus (R3) · `0 of them able to attack right now`
over 197 attacking rows (R4, **the corpus's only render falsehood**) · the collapsed blocker
tag's body-sized representative (R5) · `{card text:}` mid-word truncation (R6) · the
`auto=choice` header and its raw `target controller` labels (R7) · untagged stack/target clauses
(R8) · the missing `[DRAW PRICE:]` row tag (R9) · ANNOUNCE_X's carried plan and unmarked maximum
(R10) · "no PLAN where the plan cannot change" as a builder rule (R11) · the converter PAIR loop
(R12) · uncollapsed GAME LOG narration (R13) · run-length-only battlefield collapse (R14) ·
`{leaves N}` on activated rows (R15) · cast-ask then priority-ask answered as one menu (R16) ·
the bare `Cast nothing right now` row (R17) · the DFC Pathway land route (R18) · LOW: the
attacker row's silent cost (R19), the degenerate-decode counter (R20), two fuzzy counters (R21,
R22), the cycling row's missing bracket (R23), the mana-line number word (R24), generalising
`{right now:}` (R25), castability on a LETHAL blockers window (R26), one keyword rendered three
ways (R27).
GUIDE-only, staying in guides after the render ships: deck130's summary-line-keyed draw-punisher
rule, deck126's per-colour Lantern test and its new menace rule, deck152's converter-vs-price
precedence, deck162's converter/lifegain hold, deck123's re-pointed edict teach.
MEASUREMENT: five items, including the like-for-like restatement every suppressed rate needs, the
per-kind prompt-char delta, and the re-derivation rate amendment 100 now requires.
PASS/KEEP: sixteen surfaces measured working — the own-hand count (5,644/5,644, 0 wrong-size
assertions), the option-1 exemplar, the priced mulligan row and pregame plan suppression, the
land-drop consequence sentence, `{c}` in colour sets, the `{effect:}` clause budget, the
affirmative no-blocker tag (60/60, and it has ABOLISHED the untagged A-line), the `THIS COMBAT`
scope, the X=0 callout (0 of 29), `{X pricing:}` per row, the two attacker-count headers, the
`{leaves N}` cast field, the mana-only gate (0 leaks, was 3).

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT, not as edits. Sweeps: render-hedging vocabulary
(`invert|backwards|upside down|do not trust|misleading|is a bug|is wrong|the prompt says|ignore
the|unreliable|does not show|template error|off-by-one|may be wrong|not accurate`) **plus, new
this wave, negative-existence vocabulary** (`do NOT print|does NOT print|is not printed|no field`,
per amendment 99); attractive-nuisance substrings (`can attack|can block`); mood-word vocabulary
(amendment 72c); a **new mechanical check for amendment 90** — every double-quoted render literal
of 12+ characters containing a render marker was extracted from all seven guides and counted
against the whole 125 MB corpus, digit-normalised, with schematic placeholders (`N`, `M`, `...`)
excluded by hand; ordered-cast-list totality and rank-coverage against 62/76/92; and every card,
mana and colour claim behind a wave-47 edit. Card facts were verified with `/usr/bin/grep` against
`projects/mtg/bin/Res/sets/primitives/*.txt` only — no Scryfall fetch, and no finding turns on a
rules subtlety where the two could differ.

**TRUST DOCTRINE: CLEAN except B3, all seven guides.** Every hedging hit is either a quotation of
the render's own true string (`deck130:75`, `deck152:59`, `deck162:13` quoting the land-drop line;
`deck123:387`, `deck130:298` quoting the LETHAL header; `deck123:446`, `deck126:313`,
`deck162:307` quoting the pregame strings) or a naming of the MODEL's own fabricated argument,
which #28 requires (`deck126:189` "You have got this wrong: offered ... you blocked the Wolf and
took 3"; `deck152:173`, `deck152:194`, `deck152:200` "THE PARENTHESIS WINS"; `deck123:111`;
`deck130:212` refuting the guide's OWN deleted sentence). **No guide teaches doubt of a rendered
value.**

**CARD FACTS: CLEAN.** Verified this wave, by name, in the primitives: `Thraben Doomsayer`
(`auto={T}:token(Human,Creature Human,1/1,white)`), `Intruder Alarm`
(`auto=@movedTo(creature|myBattlefield):untap all(creature)` + `lord(creature) doesnotuntap`) and
`Lord of Lineage` (`auto={T}:token(Vampire,Creature Vampire,2/2,black,flying)`, `backside=Bloodline
Keeper`) — deck123's whole loop analysis is **exact**; `Tribute to Hunger` (`target=opponent ...
notaTarget(creature|mybattlefield) sacrifice`) — deck123's and deck126's "you never pick the body"
and the exactly-one-creature condition are **exact**; `Perimeter Captain`
(`auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller` — it
fires for EVERY defender they block with, not only itself), `Pride Guardian` (`:life:3
controller`), `Sanguine Bond` (`auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
opponent`) and `Exquisite Blood` (`auto=@lifelostfoeof(player):life:thatmuch controller`) —
deck146's converter arithmetic and deck162's loop-pair proposal are **exact**; `Dwarven
Blastminer` (`auto={2}{R}{T}:destroy target(land[-basic])`) — deck130's attack floor is **exact**
and its 13 declines are correct; `Fate Unraveler` (`{3}{B}`, **Enchantment Creature**, 3/4,
`auto=@drawfoeof(player):damage:1 opponent`) and `Underworld Dreams` (`{B}{B}{B}`, `type=
Enchantment`, no body) — deck162's new tiebreak's two facts are **exact**; `Starstorm`
(`mana={X}{R}{R}`, `auto=damage:X all(creature)`) — deck130's X procedure is **exact**;
`Lair of the Hydra` (`auto={1}{G}:name(Becomes a 1/1 hydra)`) and `Katilda, Dawnhart Prime` —
deck152's two non-token CARDS are **exact**. **One card-fact defect found — see B6.**

**RENDER-LITERAL CHECK (amendment 90), NEW: 0 hard misses across all seven guides.** Every
literal a wave-47 edit keys on exists in the corpus at volume: `of which 1 is a creature`
(**796**, and it is the SINGULAR form deck126's F1 keys on — the plural `of which 1 are creatures`
has zero occurrences, so the reviewer got the tricky half right), `would not cover any spell in
it` (12) and `would cover the cost of` (46) for deck152's 152-A, `of them without a restriction
against attacking` / `of them able to attack right now` (5,586 each) for deck125's W2 and
deck162's 162-A, `casting this taps you out` (765) for W4, `repeat: activated this turn` (14,703)
for A47-1 and 146-D, `(lifelink: they gain` (84) and `(blocking trigger:` (104) for 146-A,
`DRAW PUNISHERS on the battlefield` (2,043) for D47-3, `menace - only a block by TWO OR MORE`
(10) for F3. Three quoted EXAMPLES are non-literal and are reported below at low severity (B4,
B7); nothing a rule branches on is affected.

**CLOSED from wave 46's boundary pass — do not re-file:** **C1/C2** (the RULE -1 hoist and the
carried-plan clause) are moot at five of the six seats because lane L removed the mechanism —
8 mulligans, zero chains, zero seats at keeping-4 — and where a guide DID hoist a RULE -1, its
M=6 branch is now the finding (B1, reviewer-owned and already edited as 152-A). **C3** (deck126's
defender-allocation line): PASS, 0 replies argue from a raw body count. **C4** (the deck146/152
"(neither dies)" ordering): PASS, 0. **C5** (deck126's ordered list): shipped, and it is the
wave's cleanest guide result — but it produced amendment 92's failure as a side effect, which is
B2 below and is already fixed at that seat.

**Findings, by file. Each names the amendment it breaks and a suggested wording; applying them is
the reviewer lane's call, and a reviewer who disagrees on the evidence should say so rather than
apply it. Confidence is stated where the finding is preventative.**

| # | file : line(s) | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck162/strategy.txt` : 44-52 (RULE 1's new tiebreak) vs 99 (`THE WHOLE CAST ORDER, ONE LIST` entry 1) | **92** (a conditional stated elsewhere needs a rank in the total list), **95** (a set tiebreak must reach the executable surface) | **HIGH confidence, and it is the same mechanism that went 0/5 at deck126 this corpus.** The seat's entire wave-47 edit — take the punisher that is also a body when your own line reads `of which 0 are creatures` and theirs shows an unrestricted attacker — lives only in RULE 1's prose. Entry **1** of the numbered first-match list the pilot executes still reads, undivided, `1. Underworld Dreams, Fate Unraveler or Ob Nixilis.` A pilot running the list top-to-bottom satisfies entry 1 with either card and never reaches the tiebreak, which is exactly how a correct, legible condition scored 0 of 5 at deck126. | Split entry 1 in the list itself: *"1a. Fate Unraveler — whenever your own battlefield line reads 'of which 0 are creatures' and theirs shows 1 or more without a restriction against attacking. 1b. Underworld Dreams or Ob Nixilis — otherwise, or when 1a's condition does not hold."* Keep RULE 1's paragraph as the WHY. |
| **B2** | `deck162/strategy.txt` : 45-46 | **75** (a by-name property list is a static claim), card fact | **HIGH confidence, and it is a card-fact slip inside the new edit.** The tiebreak reads *"Fate Unraveler (3/4) and Master of the Feast (5/5 flying) are Enchantment CREATURES - they punish AND they block."* `Master of the Feast` is `auto=@each my upkeep:draw:1 opponent` — it makes the OPPONENT DRAW. It is a draw ENGINE with a body, not a punisher: it deals no damage on a draw, and RULE 2's brake exists specifically to stop draw engines going down without a punisher out. It also sits at entry **5** of the cast list, behind that brake. As written, a pilot reading the tiebreak can cast Master of the Feast ahead of Underworld Dreams and satisfy neither rule. | Name it for what it is: *"Fate Unraveler (3/4) is the only PUNISHER in the three that is also a body — it punishes AND it blocks. Master of the Feast (5/5 flying) is a draw ENGINE with a body and is still governed by RULE 2's brake: it never goes ahead of a punisher."* |
| **B3** | `deck125/strategy.txt` : 92 | **99** (a guide states what the render prints and never what it does not) | **The one trust-doctrine residual in the pool, and it is the seat's own proposed amendment applied to its own file.** The W6 workaround reads *"...the stack line and the counter row do not print [defender] - that tag only..."*. It is TRUE today and is a dated bet against a lane that is actively shipping: **D7/R8 is docketed to put the battlefield line's keyword set on exactly those two surfaces**, and the sentence goes false the day it lands — the same way this guide's "the ask does NOT print a count of your own hand" went false on 413/413 prompts and had to be deleted this wave. | Cut the negative clause and keep only the positive test the rule already has: *"RECOGNISE A WALL ON THE STACK BY WHAT THE ROW DOES CARRY: a spell whose {target text: ...} BEGINS with 'Defender', or whose printed power is 0, is a wall. LET IT RESOLVE."* Add a one-line sunset: *"when the stack row starts printing [defender], key this rule to the tag instead."* |
| **B4** | `deck123/strategy.txt` : 84 (RULE 1's `[repeat:] >= 20` tripwire) | **88** (a stop is testable only against a number the action moves) | **HIGH confidence, measured directly, and it does not affect the rule's primary half.** The tripwire is keyed to the `[repeat:]` tag. Measured on `1787786558-...deck123-...-vs-deck126.jsonl`: the tag reads **650** at seq 1099 (turn 10) and **14** at seq 1565 (turn 11) on the SAME loop with **1,539 creatures** on the board — the counter is scoped to the TURN and resets at the turn boundary, and it is scoped per ABILITY while this deck feeds the loop from two makers. On that board the tripwire cannot fire. The edit's PRIMARY stop (M = `of which M are creatures` against L + C + 3) is monotone across the turn boundary and is unaffected — this is a note about the backstop only. Docketed engine-side as **D13**. | Two words: *"...If the [repeat:] N on the row is 20 OR MORE, pass. That number counts only THIS turn and only THIS ability, so it can read small on a board you have already looped hundreds of times - the M count above is the stop that always works; the tripwire only ever adds a stop, never removes one."* |
| **B5** | `deck123/strategy.txt` : 221-238 (`WHAT TO CAST WHEN SEVERAL THINGS ARE LISTED`) | **76** (a first-match list is total or it is not a list), **92** | **PREVENTATIVE — no demonstrated cost this corpus** (deck123's losses were the loop and two edict casts, neither an ordering failure). The list runs to 7 entries and has **no `Cast nothing right now` closing entry and no roll-call**, while entry 5's own text instructs the pilot to answer `"Cast nothing right now"` — an answer the list does not rank. deck146's and deck162's lists close with it, deck126's roll-call took unpaid declines from 24/24 to 4/47, and deck130's closes at entry 7. This is the cheapest form of the edit in the pool. | Append: *"8. 'Cast nothing right now' - the LAST entry, and correct only when no entry above appeared in the menu. EVERY CARD YOU CAN CAST HAS A LINE ABOVE."* and, if the reviewer agrees the roll-call is worth the bytes, one line naming the deck's castable cards that currently have no entry. |
| **B6** | `deck126/strategy.txt` : 32 · `deck123/strategy.txt` : 408 · `deck130/strategy.txt` : 295 | **72c** (every rule input is a printed number or tag — no mood words) | **Three mood-word rule inputs, and all three have a printed number one line away.** deck126:32 gates a cast on *"when the part of a cost that is a plain number ... looks one or two out of reach and you have three or four walls out, it is probably affordable"* — in a guide whose own preceding sentence gives the exact formula (the `Mana available:` number plus one per defender past the first). deck123:408 and deck130:295 both decline a trade *"while your life is comfortable"*; deck130's very next line supplies the number it should have used (*"when the damage otherwise puts you at 5 life or less"*). | deck126: *"...take it when the plain-number part of the cost is at most your 'Mana available:' number plus one for every defender past the first on your battlefield line; below that, do not."* deck123/deck130: replace *"while your life is comfortable"* with *"while the combat header's 'you would be at N' is above 5"*, borrowing each guide's own existing number. |
| **B7** | `deck146/strategy.txt` : 381 (CHECK 2's example battlefield line) · `deck152/strategy.txt` : 143 (the lifelink paragraph's quoted tag) | **90** (quote the string the emitter prints) | **LOW severity — examples, not rule keys; neither rule branches on the quoted form.** deck146 shows an animated Hive as `"Hive of the Eye Tyrant #1 (3/3) [menace]"`; the bare `[menace]` form has **0** occurrences — the battlefield line prints `[menace (can't be blocked except by two or more creatures)]` (64), an attacker row prints a third wording and a blockers A-line a fourth (docketed as **D19**). deck152 quotes `"(your attacker dies, their blocker lives (lifelink: you gain 3))"`; that composite has **0** occurrences — the real long form is `(lifelink: you gain N from this block only, and this attacker deals nothing to their life)` (88), which now states the mutual exclusivity the guide's whole paragraph was written to teach. | deck146: quote the real battlefield form, or drop the tag from the example and keep the load-bearing half (`(3/3)`). deck152: replace the quoted composite with the printed clause and cut the paragraph to a pointer — *"the tag now says it itself: 'from this block only, and this attacker deals nothing to their life'. Exactly one of those two things happens."* |

**Not findings — checked so nobody re-files them.** deck126's F1/F2 key on the SINGULAR
`of which 1 is a creature` and on `of which 0 are creatures`, both real and both at volume; its
new menace rule keys on the blockers-screen wording, which is the correct surface for a blocking
rule. deck125's W2 correctly names BOTH attacker-count wordings and says why they differ
(`liveScope`); its W4 keys on `leaves N` and on the `casting this taps you out` variant, both
real; its W5 deletion is exactly amendment 99 applied. deck123's A47-2 is textbook 102 (one
duplicate deleted, the check moved onto the PLAN sentence, the plan-text metric predicted) and its
A47-1's primary stop is textbook 88. deck130's D47-1 is textbook 96 (a card-section sentence
DELETED, replaced by a pointer to the rule that owns the procedure) and D47-2 is textbook 94.
deck146's 146-A carries both the string-family fix (90) and the collapse fix (91); 146-C is
textbook 93 (the exception now demands its two numbers). deck152's 152-A is deliberately narrow
and keys on the coverage line, and 152-B is textbook 94. The mood-word sweep returns **zero
further hits**; the attractive-nuisance sweep returns only quotations of the render's own
`"Your creatures that can attack:"` list header. deck125 and deck152 still have no single ordered
cast list (wave 46's C5, reported there at low confidence): **not re-filed** — neither produced an
adjudicable cast-order misplay this corpus, deck152's deploy floor ran 45/45, and #45 forbids
re-opening on no new evidence.

---

## For the next wave

The core loop is where wave 48's weight belongs and the docket says so in one number: priority
prompts, 72% of all decisions, grew 24.4% while the collapse that was meant to shrink them landed
on a list five rows wide. **D1 is the headliner and is not optional** — it cost a completed game,
which is invariant 00. D2, D3 and D4 each have a lost game or a killed game behind them. D13 is
small but it is the item that decides whether deck123's new guide-side backstop can fire at all,
so it should ship in the same wave as D1's F2. The guide lane is in the best shape it has been:
every wave-46 rung that could be adjudicated was, four reviewers corrected their own prior edits
against falsified predictions, and the seven boundary findings above are the smallest set yet —
two of them preventative, two of them examples. deck162 is the pool's exit candidate on the
record; deck146 is the seat that moved backwards, and its two guide-attributable losses are both
things its guide contains and cannot execute, so **re-check it after D4 ships** rather than
spending another wave's edit budget on the guide.
