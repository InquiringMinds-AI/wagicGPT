# Wave-48 synthesis (2026-08-27) — FIFTH FAIR-HAND CORPUS

Inputs read, in order: `wave47/synthesis-notes.md`; `wave47/strategy-writing-skill.md` (the
CURRENT edition, amendments 1-102); `wave47/general-strategy.md` (R1-R27); `wave47/engine-ledger.md`
(D1-D20); `wave48/review-carry.md`; the four seat files (`seat-engine-narration.md`,
`seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`); all seven `deckN/skill.md` and
`deckN/general-suggestions.md`; mechanical sweeps over the seven `deckN/strategy.txt` (the
reviewers' wave-48 guides) for the boundary section.

Outputs: `wave48/strategy-writing-skill.md` (wave-47 edition **verbatim** — prefix `cmp`-verified
— plus **amendments 103-119**), `wave48/general-strategy.md` (R1-R27 with a wave-48 verdict each,
R28-R47 new, the SHIPPED row, GUIDE-only routing, six measurement items, PASS/KEEP),
`wave48/engine-ledger.md` (the wave-49 docket D1-D20, ranked, engine/UX only, with the
out-of-wave SHIPPED table, both discharge tables, fallback classes, the per-kind prompt-char
delta and the lategame specimens), and the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step
owns the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is
the purpose, and guide/skill polish is the opportunistic side quest. **No guide file was edited
by this step.** Step 4 was run as a report (B1-B6 below). Hard caps on the model's legal
choices were proposed by nobody and would have been rejected; the two reviewer proposals that
edged toward a code-like floor are named in REJECTED. **No deck-content question is open for
the owner.**

---

## The headline: the loop closed by affordance, not by a cap — and the corpus's currency changed

**(a) THE WAVE-47 HEADLINER IS DISCHARGED, BY A FACTOR OF FIVE, WITH THE ROW STILL ON THE
SCREEN.** Lane N's three affordances (a real `0. Pass priority` LAST row, a repeat-N row, a tag
that states both scopes) took the three deck123 token games from 1,871 / 1,550 / 223 priority
records to **12 / 17 / 8**; the maximum priority windows in any single turn is **9**; 21/21
natural; priority prompts fell from 4,128 to 364 and their mean halved. The wave-47 OPEN
question — does qwen ever TAKE the repeat row — is answered: **12 of 14**, `x50` five times and
`x15` once, each executed exactly and receipted, then `CHOICE: 0 (pass)` on the pass row with
the guide's stop cited verbatim. The decline that was 0/1,871 is now taken on 54% of
single-option windows. The owner doctrine's bet — give a reachable stopping act, never delete a
legal row — is measured true on the exact board that killed a game last wave. Two residuals,
both parser-side: the count grammar accepts one spelling and a missing count runs once silently
(D3), and the pass row appears only where there is no other option.

**(b) THE ENGINE'S OWN REPLACEMENT RULE IS THE NEW SEAM.** E1 paid 21/21 (the model's last coded
`CHOICE:` is the decision; `retracted_choice` 0) and it is how the loop exited. It also
executed, once, a re-answer whose parenthetical named a card not on the menu (Tribute cast at 0
creatures via "Cast Sanguine Bond"), and it does not cover `ATTACK:` lines at all — the first
line was sent and the record says `answer_replaced: true`. The short name exists so the index
can be checked; D2 is the check. The same check re-classifies all four of this corpus's
`unparsed_reply` fallbacks as one countable class: an index past the menu naming a card not on
it.

**(c) DECISION-VALUE-PER-TOKEN FLIPPED FROM PROMPT SIZE TO REPLY LENGTH.** Every prompt kind's
maximum fell below 30 KB except blockers, and that one prompt (61,240 chars) is a single
unranged B-row clause with 256 handles and ONE distinct parenthetical (D1). Latency tails are now
4-6 KB replies on 9 KB prompts (347 s, 515 s, 802 s), and 9/174 deck126 replies carry two coded
answers with 2.5-4.9 KB of reasoning between them — the overrun E1 reads. It is measured (D16)
before it is changed.

**(d) THE CROSS-DECK KILLER IS THE CONVERTER, IN THREE SHAPES, AND ONE OF THEM IS THE ITEM
CARRIED UN-SHIPPED FROM WAVE 47.** deck152 held every priced ground attacker and sent the
unpriced flier into a board holding BOTH loop halves: 20 -> 0 in one combat (R12, now D5, HIGH).
deck146 sent 7 of 7 priced rows into the single converter and died on its own attack at 4 life
with the price printed on the row — a THIRD corpus, and the replies all perform the same wrong
arithmetic ("net 0"). deck130 lost to the same pair with no out in the sixty. The guide lane at
146 is at #102's routing with a belief-text metric; the render half (a DIRECTION clause and the
loop clause) is on the docket.

**(e) THE MULLIGAN DOCTRINE DIVERGED, UNDER THE OWNER'S RULING, AT THREE SEATS — AND THE POOL
NOW CARRIES TWO.** Three chains (146 to a three-card all-land hand after bottoming its own plan;
125 to a four-card keep and then a bottoming ask whose three NEVERs could not all be satisfied;
162's one-land six under two colliding absolutes) all traced to hard floors that the executor
argued past or that contradicted each other. All three reviewers replaced them with odds
reasoning satisfiable at any bottom count (amendment 119). deck123, deck126 and deck152 still
carry the floor text that failed 1/1 at deck146 — boundary B2.

---

## What merged into the skill (103-119)

**Convergence merges (>= 2 seats):**
- **106** diagnose the wrong arithmetic before the #102 rewrite — deck146 S-2 ("net 0"),
  deck125 S3 (a cast that draws is not a "draw"), deck123 S4 (a printed row does not bind).
- **107** a rendered count is not a rendered predicate — deck126 S1 ("1" never became "only,
  now") + deck162 S-3 (the exception's name survived without its condition).
- **108** a conditioned exception is a numbered entry at its winning rank; a modal card's
  entry carries mode conditions — deck123 S3 (Damnation at entry 6 behind the makers) +
  deck146 S-4 ("always castable" enumerated false by the pilot).
- **109** read the violating menus and split by cost before scoring a gate — deck126 S2 (the
  rule was the defect) + deck125 S2/S4 (free violations teach optionality; the consequence trace).
- **112** lethal is the first check of the TURN where creatures pay; the guide owns the
  payment — deck152 S-2/S-3 + deck130 S1 (#71's authoring step).
- **113** repeatable-activation checklists read tapped state and the sources list — deck146 S-3
  + deck152 S-3 (one lesson at two surfaces).
- **114** name the escalated form of a matchup mechanic first; write the mechanism, not the
  safe board — deck152 S-1 + deck126 S4.
- **115** the dictated plan sentence is the guide's guaranteed surface; a plan names an action
  — deck125 S1 + deck146/162's #102 plan metrics + deck130 S4/M3.
- **116** E1-era adjudication by the last line; `answer_replaced` as the specimen set —
  deck126 S3 + deck146 S-5.
- **117** offers by phase, windows by kind — deck152 S-5 + deck146 G-6 + deck162 S-5.
- **119** mulligan sections are odds satisfiable at every bottom count; NEVER-sets jointly
  satisfiable — deck146 D1 + deck162 D2 + deck125 W13.

**Decisive single-seat merges:**
- **103** a guide's WHY must survive the render it names (deck123 S1: a ceiling justified by a
  233,662-char prompt that lane O made 11,274; four crossings, no cost).
- **104** one-decision form of a repeated action: the guide owns the arithmetic and the
  emit-grammar, stated positively (deck123 S2; 4 of 12 takes lost their N).
- **105** a worked example is a template with blanks, never a finished sentence (deck146 S-1:
  parroted on menus without the Command).
- **110** when the render marks the answer, "answer the mark" is line one (deck130 S3).
- **111** a tag with several PRESENT forms: the rule names them all and keys on the fact they
  share (deck130 S2; the enumerated form silenced the collapsed-form rule and cost the Commander).
- **118** violated-unpunished has a ceiling at the third CORPUS (deck162 S-2; 3, 3, 6 breaks,
  0 losses, the rule is in the plan and the answer breaks it).

## What was REJECTED, and why
- **deck162 S-1's definition-of-done: "every mulligan section states exactly ONE stop look,
  the same look as the rest of the pool."** A fixed stop look IS a floor — the shape the owner
  ruled out and the shape that failed 1/1 at deck146 this corpus. Replaced by amendment 119
  (odds at every look, satisfiable at every N) with the pool-convergence obligation kept.
- **deck126 G2's bounded `THINK:` block before the CHOICE line.** A core-prompt change to a
  channel E1 is currently rescuing 20 of 21 times; measured first (D16's counter with the
  rescued/harmful split), changed only if the count says drift.
- **deck162 S-4 as an amendment** (a "biggest" removal-target rule names engine makers first).
  It is #17 applied to targets — adopted at the seat (162-B), recorded as a supporting instance.
- **deck123 S5** (Baka sacrificed its 6/6 to an edict at N=4). One sample; WATCH, not a rule.
- **Any guide compensation for the E1 name/index hole, the repeat-count grammar, the vigilance
  double-count, the `taps you out` wording, the bare Tribute/sweeper rows, or the missing loop
  clause.** Trust doctrine: every one prints a TRUE string or omits one; each is docketed
  (D2, D3, D9, D11, D10, D5) with a prediction, and the only guide content that belongs is a rule
  that stays correct after the render ships. The reviewers wrote it that way in every case but
  one (B1).
- **Reading deck126's 1-5 -> 4-2, deck162's 5-1 or deck123's 1-5 as guide effects.** n=6 and
  real hands; two of deck126's wins were the opponent's mull-to-4 and a loop the guide does not
  touch. Counts only (amendment 87 and the new-baseline rule).
- **Loosening deck152's counter-spreading rule (2/13, third corpus, in wins).** #45/#64 hold;
  the seat recorded the temptation and did not act on it — correct.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE, ranked: the unranged B-row `may block` list (R28) · the E1 name/index and
ATTACK-line holes (R29) · the repeat-count grammar and receipt (R30) · the converter loop-pair
clause, carried and now a lost game (R12) · draw-price coverage (draw:X, activated draws,
opposing cast-triggers) and the DRAW FORECAST line (R31) · the auto-tap colour choice (R32) ·
`{right now:}` on the edict and sweeper rows after four and five waves of prose (R33) · the
immortal action-less plan (R34) · one re-ask for an index past the menu + `(moves on to
combat)` (R35) · the unmarked `//` back-face drop (R36) · the vigilance double-count (R37) · what
paying an action TAPS, on the row (R38) · the draw-engine row's feeds count, filed early (R39) ·
the overrun counter (R40) · LOW: the `#n` sort straddle (R41), the 1-based turn field (R42), the
menace re-ask (R43), the ATTACK-range receipt (R44), the X marker echo on the top row (R45),
small residuals (R46), Ob Nixilis target text (R47).
Of R1-R27: **R1, R2, R4, R5, R6, R7, R8, R10, R13, R14 SHIPPED and PASSED**; R3 shipped and
untested at scale; R9 partial; R11 holds and is extended by R34; R12 promoted to HIGH; R15-R27
carried with the new evidence noted on each.
GUIDE-only: deck123's repeat arithmetic + emit spelling, deck152's loop-pair verdict and
Spellbinder priority, deck146's direction sentence, deck125's composed Fruition price and the
gate inside the plan line, deck152's lethal-first turn check, deck146's tapped-state checks,
deck130's "answer the mark", deck162's one-land branch by look.
MEASUREMENT: six items, headed by `answer_replaced` read-and-split and offers-by-phase.
PASS/KEEP: lanes N/O/P/Q's shipped strings and behaviours, E1's replacement, the mana-only gate,
the GANG BLOCK first live hold, the affirmative tag 44/44, land drops 0 declines everywhere, the
own-hand count, the pregame no-plan ask. **Zero render falsehoods this corpus**; R37 is a
double-count in a brand-new header.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT over the seven wave-48 `deckN/strategy.txt` files. Sweeps:
render-hedging vocabulary (`invert|backwards|upside down|do not trust|misleading|is a bug|is
wrong|the prompt says|ignore the|unreliable|does not show|template error|off-by-one|may be
wrong|not accurate`); negative-existence vocabulary (`do NOT print|does NOT print|is not
printed|no field|prints nothing|is read|not read|only spelling` — amendment 99, now also 103/104);
mood-word vocabulary (72c) — **zero hits**; the hard-floor family (`NEVER bottom|NEVER keep|hard
stop|(keeping 5)|LAST look|never mulligan more than`); the wave-47 repeat-row spelling; and a
coverage diff for the loop pair (`Exquisite Blood` by file). Card facts behind the wave-48 edits
were verified by the seats against the primitives (each seat file lists its set); this step
re-verified none against Scryfall and no finding below turns on a card fact.

**TRUST DOCTRINE: CLEAN except B1, all seven guides.** Every hedging hit is a quotation of the
render's own true string (`deck123:412`, `deck130:330`, `deck126:327`, `deck162:13`, `deck152:59`
quoting the LETHAL / land-drop / bottoming lines) or a naming of the MODEL's own fabricated
argument, which #28 requires (`deck126:202` "You have got this wrong: offered A1 ...",
`deck152:192/219` "THE PARENTHESIS WINS", `deck146:478` and `deck162:381` refuting the plan
sentence per #102, `deck130:230` refuting the guide's OWN deleted sentence). **No guide teaches
doubt of a rendered value.** The wave-47 B3 residual (deck125's "do not print [defender]") is
GONE — deleted by W11 when lane P shipped, exactly as predicted.

| # | file : line(s) | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck123/strategy.txt` : 87 (RULE 1, the repeat-row spelling) | **99**, **104** | **The one trust-doctrine residual in the pool, and it is a dated bet against the top of the wave-49 docket.** The line reads *"No other spelling is read. 'N=17' and a bare 'repeated N times, then stop' with no number were each carried out as ONE token ..."*. TRUE today; **D3 widens the grammar to `N=17` / `17 times` / bare integers** and the sentence goes false the day it lands — the same shape as deck125's "does NOT print a count" (falsified 413/413) and its "[defender]" line (falsified this corpus). The positive instruction above it (write `x17` exactly as the row's example does) is right and sufficient. | Cut *"No other spelling is read."* and re-phrase the cost sentence as history, not grammar: *"Write it exactly that way: in this corpus 'N=17' and the bare row label were each carried out as ONE token, and the pilot came back for another window each time."* Keep the `x<N>` instruction at line 86 and line 521 unchanged. |
| **B2** | `deck123/strategy.txt` : 14-17, 437-440, 480-484, 514 · `deck152/strategy.txt` : 5-8, 384-386, 428-429 · `deck126/strategy.txt` : 321-322, 337 · `deck130/strategy.txt` : 361-362 | **119** (owner ruling: no code-like floors; NEVER-sets jointly satisfiable), **97**, **81** | **The pool now runs two mulligan doctrines.** deck146 (146-F), deck162 (162-C) and deck125 (W13) replaced their floors with odds reasoning after three chains this corpus. deck123 and deck152 still carry the IDENTICAL STEP 1 text that **failed 1/1 at deck146** (*"(keeping 5) or ANY lower number: KEEP, if the hand holds at least one land — you are DONE, do not read another rule"*), hoisted to line 14/5 as the file's first rule; deck126 carries *"Never mulligan more than once ... this is your last look: answer 1"* — the exact absolute that collided with a one-land branch at deck162 (#97). And four guides carry stacked NEVER-bottom absolutes (deck123: three lands + never your only maker + never your last castable creature; deck130: only Starstorm + three lands; deck126: Bond + Blood + Tutor; deck152: four lands + only early play) that can exceed the cards in hand at a small keep — deck125's seq-5 shape, where the executor "chose freely". No new loss at these four seats this corpus (deck123 kept two one-land sixes correctly), so this is PREVENTATIVE, but the failing text is verbatim-shared. | Adopt 146-F's shape at each seat: price each look as a fresh seven shaved by one more card at the deck's own land odds (name the land count and the colour-source count); at the bottom ask give a preference ORDER (lands that cover the cheapest spells, those spells, then the rest by cost) that is satisfiable at any N; delete "you are DONE" / "last look: answer 1"; where two branches can both apply (one land at the second look), state which wins and why in the odds. |
| **B3** | `deck146/strategy.txt` (0 mentions of `Exquisite Blood`) · `deck162/strategy.txt` (0) | **114**, **81** (coverage diff) | **deck146 faces deck126, paid the most to the SINGLE converter (7/7 rows, a death), and has no sentence about the PAIR** — the escalated form that took deck152 from 20 to 0 in one combat and that R12/D5 will render. 146-A fixes the direction belief; it does not say what both names on the battlefield line mean. deck162 also faces deck126 (0 converter windows this corpus; its hold rule exists) — lower priority, same gap. | One line in each DECIDING list, keyed to the render: *"Their battlefield line shows BOTH 'Sanguine Bond' and 'Exquisite Blood': any damage you deal them and any life they gain repeats without limit. ATTACK: none, whatever the tags say, until one is gone."* (deck152's 152-A wording is the model.) |
| **B4** | `deck123/strategy.txt` : 194-199 (RULE 5) · `deck126/strategy.txt` : 148, 355 (Tribute zero stop) | **102** (routing), **106** | **Both guides restate the edict count rule again this wave** (deck123 in the plan-sentence form, deck126 with the zero stop in two places) while the count is now DOCKETED on the row (D10: `{edict: they control N creatures ...}`). Correct per #102 — the plan-sentence check IS the third-failure move — but the seats should predict the plan-text metric only (deck123 P7 does) and NOT add prose at a fifth corpus; deck126's second restatement at :355 is #96's fork risk (rule section + DECIDING list saying the same procedure twice). | No edit now; record that if D10 ships, both seats delete the derivation and keep one pointer to the row's `{edict:}` clause (amendment 89's standing sweep). deck126: keep :355 (the DECIDING line) and turn :148 into a pointer. |
| **B5** | `deck130/strategy.txt` : the X procedure (D48-5) | **110** | PASS as edited — "answer the mark" is line one and the steps are the unmarked fallback. Recorded so the seat does not re-derive it; **note D14** (the mark may also be echoed on row 1 next wave, which changes nothing in the rule). | none |
| **B6** | `deck152/strategy.txt` : 492+ (the new DECIDING SITUATIONS list) | **24**, **98** | **New this wave and correct in shape**; one check for the reviewer: the loop-pair line (152-A) and the "race with evasion" posture must both appear in the list with the pair line FIRST (amendment 114's ordering), and every prohibition restated in the list must have its positive branch beside it (#98). The seat says it restated "every existing prohibition"; the positive halves were not verified by this step. | Reviewer self-check next wave; no wording proposed. |

**Not findings — checked so nobody re-files them.** deck123's `x<N>` instruction (lines 86, 521)
is the correct positive form of amendment 104. deck125's W11 deletion is amendment 99 applied
(the wave-47 B3 is closed). deck146's 146-E template-with-blanks is amendment 105 exactly, and
146-A names the belief in the fold's words (106). deck152's 152-C is 112 with the `Those sources`
key. deck162's 162-C is 119's odds-by-look shape and deletes both colliding absolutes. deck130's
D48-1/2/3/4 key on printed numbers throughout (72c sweep: zero mood words pool-wide). The
attractive-nuisance sweep returns only quotations of the render's own list headers. deck125
still has no single ordered cast list (wave 46's C5) — not re-filed: no cast-order misplay this
corpus, and #45 forbids re-opening on no new evidence.

---

## For the next wave

The core loop is where wave 49's weight belongs, and the docket's shape says what kind of wave it
is: **D1-D3 are one-file parser/emitter fixes each with a lost creature or a wasted turn behind
them, D4 is engine correctness in the payment code that cost a game by one turn, and D5 is the
carried item that finally cost a game.** D6-D11 are the render's remaining silences on the
decisions the guides have been trying to write in prose for four and five waves (the edict
count, the sweeper preview, the draw price's three missing classes, what paying an action taps)
— every one is the same principle lane J stated: where the frame carries a summary of a cost,
the ROW that incurs it carries the number. D20 is the owner's frontend item and stands on its
own. The guide lane is in a different place than it was: three seats rewrote their mulligan
sections under the owner's ruling and three still carry the text that failed (B2), the
converter pair needs one line at deck146 (B3), and one negative sentence about the parser needs
cutting before D3 makes it false (B1). deck162 remains the pool's exit candidate — 5-1, its one
loss a T10 kill with the new tiebreak obeyed, 0 blockers windows in six games; put its exit on
the wave-49 agenda if 162-A's plan metric holds. deck146 moved on every wave-47 edit and fell on
a third failure with the belief now named; **re-check it after D5's direction clause ships**
rather than spending another edit budget on the guide.
