# Wave-49 synthesis (2026-08-27) — SIXTH FAIR-HAND CORPUS

Inputs read, in order: `wave48/synthesis-notes.md`; `wave48/strategy-writing-skill.md` (the
CURRENT edition, amendments 1-119); `wave48/general-strategy.md` (R1-R47); `wave48/engine-ledger.md`
(the wave-49 docket D1-D20: D1-D13 shipped as lanes R/S/T/U, D14-D20 carried);
`wave49/review-carry.md`; the four seat files (`seat-engine-narration.md`, `seat-123-130.md`,
`seat-125-126.md`, `seat-146-152-162.md`); all seven `deckN/skill.md` and
`deckN/general-suggestions.md`; mechanical sweeps over the seven `deckN/strategy.txt` (the
reviewers' wave-49 guides) for the boundary section; and the corpus directory itself for the
one reconciliation below.

Outputs: `wave49/strategy-writing-skill.md` (wave-48 edition **verbatim** — prefix `cmp`-verified
— plus **amendments 120-132**), `wave49/general-strategy.md` (R1-R47 with a wave-49 verdict each,
R48-R71 new, GUIDE-only routing, seven measurement items, PASS/KEEP), `wave49/engine-ledger.md`
(the wave-50 docket D1-D26, ranked, engine/UX only, each with a record citation; both discharge
tables; fallback classes; per-kind prompt chars; lategame specimens), and the boundary findings
below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step
owns the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is
the purpose. **No guide file was edited by this step.** Step 4 was run as a report (B1-B6).
Hard caps on the model's legal choices were proposed by nobody; one engine rule (the raw-count
plan expiry) is rejected below BECAUSE it would have deleted a correct plan. **No deck-content
question is open for the owner.**

---

## Reconciliation against the records

**The "excluded deck123-vs-deck162 dead-loop, no gameend" note is WRONG for this corpus and is
dropped.** `seat-146-152-162.md` (header + E-1) and, by inheritance, one line of
`seat-125-126.md` carried a brief note about a 9-hour orchestrator kill with pointer
`0x55e867ceaa40`. That pointer is not in `matchups-20260827-094106`; the deck123-vs-deck162 pair
in THIS run is `1787841709-ai_baka_deck123-0x563fceb01d00-vs-ai_baka_deck162.jsonl` and its
opposite — 31 records, natural `gameend` at turn 12 (0 to 22). Verified on disk by this step:
**42/42 seat logs carry a `gameend`**, 21/21 games natural, 21 stderr files. The engine seat and
seat-123-130 both caught this; every rate in the three output files uses 21/21. E-1's
compound-loop concern is kept as a watch (docket D25) with no record in this corpus, and this
corpus's own evidence points the other way — the 322-Vampire own-upkeep turn closed in 16
windows.

---

## The headline: the lanes paid, the losses moved into the engine, and one row lied

**(a) FOUR LANES SHIPPED AND THREE WAVE-48 LOST OR FATAL WINDOWS WERE REPLAYED CORRECTLY.** Lane
U's `Both halves of a life LOOP` clause: `152 vs126`, the exact matchup that went 20 -> 0 on its
own attack in wave 48, answered `ATTACK: none` at both both-halves windows, 0 "net 0" replies in
1,915. Lane T's `{paying this taps:}` / `{tapping Katilda now:}`: Katilda's counter ability was
taken 0 of 7 times in Upkeep/Main-1 (was 8/9, one fatal), including the exact `152 vs162` Upkeep
window; the tapped-Hive `becomes` row was rendered `[this land is TAPPED: ...]` and passed. Lane
S: `named_row_reask` 1/1 recovered, `repeat_count_reask` 1/1 recovered, `repeat_count_missing`
0, off-menu-name replacements executed 0, every ATTACK/BLOCKS replacement a correct
self-revision — the index-past-menu class that was 4/5 of wave-48's fallbacks is 0. Lane R:
blockers max 61,240 -> 17,616, 0 B-row repeats, 0 sort straddles, 39/39 headers. Lane T's
edict/sweeper rows: 0 casts at N=0 (123 rows), 0/87 on `destroys 0`, deck126's Tribute-at-1
4/4. The pool's floor text is gone at every seat (deck130 D49-1, deck126 F15 were the last).
Skill #132 names the falsifier this proved: the prior loss's own window, replayed.

**(b) THE NEW DOCKET IS ENGINE CORRECTNESS FIRST, RENDER SECOND — AND ITS TOP ITEM IS A
REGRESSION.** Twelve of 234 casts paid twice (eleven of them answered on the stack), 7/7 in wave
48, 0 in wave 47 — it began with the wave-48 binary, not lane T, and it empties the seat's
response mana in the exact window a counterspell is on the stack (D1, IN FLIGHT at lane V).
Tribute to Hunger resolved against two creatures and sacrificed nothing because a stale reveal
zone from the opponent's Pelakka Predation was consulted as the choice set — the opposite seat's
log says "revealed 2 cards but none was a legal target" (D2). The cleanup-step discard threw away
deck123's only Intruder Alarm with no decision record (D4). Five Spellbomb target rows became one
self-target row between main 1 and main 2 with no board change, and the pilot shot its own
Blastminer (D6). None of these is a guide item; each has a file and a seq.

**(c) THE CORPUS'S ONE RENDER FALSEHOOD IS A PRONOUN.** Lane T's edict row prints Tribute's
wording on Devour Flesh: `... Master of the Feast is sacrificed, you gain 5` — the TARGET gains.
Believed twice at deck123; the guide states the truth and, correctly, does not teach doubt of the
row (D3). Everything else open is an omission, an enumeration (the 34,333-char `legal targets
right now:` row, D5), a counter, or a parser edge.

**(d) THE PARSER'S NEW EDGES ARE THE SHAPE OF THE MODEL'S OWN REASONING.** The "last clean coded
line" executed `CHOICE: 1 (Cast Lightning Greaves) is wrong because ...` — the option the reply
rejected — and skipped `x20).` for `x19` (D7). The prompt's own worked example `CHOICE: 1 (Goblin
(1/1))` does not parse (D8), and `CHOICE: 0` on a no-pass ask escaped the re-ask (D9). Each is a
one-file fix with a PARSETEST case.

**(e) THE PLAN-CARRY LESSON CUTS BOTH WAYS.** "The game is lost" carries fell 48 -> 5, and a
plan re-said every ~6 windows never expires (28 identical at `125 vs126`) — but two of the long
echoes were CORRECT plans (the opening plan; deck152's loop-lockout "ATTACK: none"), and a
raw-count trigger would have expired them. The expiry keys on content — a verdict opener, or no
action on the current menu — and the count becomes a report field (D10).

**(f) THE GUIDE LANE'S NEW FAILURES ARE ALL SENTENCE-SHAPE, AND THE SKILL NOW SAYS SO.** Six
corpora of deck125's Staff gate share one shape: the dictated plan sentence's action fires and
the qualifier after it is dropped; deck123's repeat stop held exactly while the number was in the
plan (#120). Four seats derived a quantity a lane now prints on the row (#121). deck130's
survival override compared the wrong two numbers (#122); deck125's Emrakul absolute had no
counter-shape and lost at 48 life (#123); deck126 cast a second Exquisite Blood as "both
enchantments" (#125) and repeated its own anecdote (#126).

---

## What merged into the skill (120-132)

**Convergence merges (>= 2 seats):**
- **120** gate-first plan sentence, action with "only", three blanks for a repeated action —
  deck125 S1 (W15, sixth wave) + deck123 S1 (A49-1).
- **121** re-key to the row's count in the same wave; lists led by the own-turn menu row; soften
  absolutes with the printed count — deck123 S2/S5 + deck125 S3 + deck126 S1.
- **132** the replayed prior loss as the preferred falsifier; UNTESTABLE-AT-THIS-SEAT after two
  no-window corpora — deck152 + deck146.

**Decisive single-seat merges:**
- **122** a survival carve-out states the post-action quantity (deck130 S1; a loss).
- **123** a finisher absolute carries its counter-shape, keyed to a visible surface, after the
  absolute (deck125 S2; a loss at 48 life).
- **124** one-row self-target windows: the pass is the answer; owner tag + pass rule for every
  self-targetable ability (deck130 S2; two self-hits) — pairs with docket D6.
- **125** a named pair is two different names; a duplicate's collapsed form is not the pair
  (deck126 S2).
- **126** an anecdote matching the new failure = a rule keyed to the story (deck126 S3).
- **127** narrated actions with no decision record are a review surface (deck123 S4) — pairs
  with docket D4; the mitigation sentence is written about cards, never about the engine.
- **128** the quoted-literal grep (deck123 S3) — the mechanical step of #103.
- **129** a "YOUR UPKEEP" block for upkeep mana sinks (deck130 S3).
- **130** cast-order rank is also order in time for X spells (deck130 S5).
- **131** the four-hands-by-three-keeps self-consistency check for mulligan sections (deck130 S4).

## What was REJECTED, and why
- **Any raw-echo-count plan expiry (the shipped "5 identical echoes" clause as a TRIGGER).**
  Seat-146-152-162 E-3 has the counter-instance: `152 vs126` s31-41's loop-lockout plan is the
  wave-48 fix working, and a count trigger would have deleted it. Retired to a report field; the
  content keys stay (D10). Multiple seats asked for windows-since-stated; that is still a count.
- **Seat-146-152-162 E-1 as a docket item** (the "excluded dead-loop game"). Not in this corpus
  (reconciliation above). Kept as a watch with no record (D25).
- **deck146's UNTESTABLE flag as a WATCH only.** Adopted instead (in #132) — the flag costs
  nothing and #21's structural-null rule already wants it; the row-annotation move is deferred to
  a third corpus exactly as the seat asked.
- **Any guide compensation for D1 (double-tap), D2 (the stale reveal zone), D3 (Devour's
  pronoun), D4 (the engine discard), D6 (the vanishing rows).** Trust doctrine: a guide describes
  cards and printed strings, never engine behaviour; each has a docket entry with a prediction.
  Two guide sentences already drift toward describing the defect — boundary B1.
- **deck125 W38's Cancel at 2 life under Forced Fruition as a rule change.** The seat was dead
  to the next draw step regardless; a break with no game in it (the seat itself declined).
- **deck126's own-turn holds at `they control 3 creatures` (seq 16-18) as a new rule.** Three
  windows, no cost, first corpus; F10's re-key covers the shape (#45).
- **Reading deck146's 2-4 -> 4-2 or deck162's second 5-1 as guide effects.** n=6, real hands;
  deck146's converter lanes had zero windows. Counts only (#87).
- **deck125 G3's late land-drop declines as a guide edit.** 4/88 at 12+ lands, first corpus;
  counted against 88 (R11).
- **deck123 G4 as guide text** ("this ask has no pass"). It is a core-prompt line — routed to D9.
- **Loosening deck152's counter-spreading rule (2/13, fourth corpus, in wins).** #45/#64 hold;
  the remedy is a row annotation (R70).

---

## Layer routing (`general-strategy.md`)

RENDER/CORE, ranked: the double-tap regression, IN FLIGHT (R48) · the `legal targets right now:`
enumeration (R49) · Tribute's stale reveal zone (R50) · Devour Flesh's gain recipient (R51) · the
unrouted cleanup discard (R52) · vanishing target rows + self-only windows (R53) · the clean-line
rejection (R54) · the unparseable worked example (R55) · content-keyed plan expiry (R56) ·
`CHOICE: 0` on a no-pass ask (R57) · the mana-only leak (R58) · the sweeper's K (R59) · the
unrendered chosen name (R60) · the repeat row's count + placeholder (R61) · LOW: the FORECAST
mirror (R62), the MDFC marker (R63), the payment receipt (R64), the decode counter (R65), the
owner-side loop wording (R66), the second-copy count (R67), `{paying this taps:}` on defenders
(R68), reading aids (R69), the counter-target size (R70), the log-share watch (R71).
Of R1-R47: **R12, R28, R29, R30, R33, R35, R37, R38, R41 SHIPPED and PASSED**; R31 shipped and
passed on the classes it met; R34 shipped and partial; R32 shipped and unadjudicable by string;
R39 and R20 promoted; the rest carried with the new evidence on each.
GUIDE-only: deck123's three-number plan shape and own-upkeep rule; deck125's gate-first plan
sentence and Emrakul counter-shape; deck126's Tribute-first button, pair-as-two-names, STEP 1
re-key; deck130's post-sweep subtraction, self-target pass, upkeep Hammer rule,
Starstorm-first; deck123's edict/sweeper re-keys.
MEASUREMENT: seven items, headed by the double-tap diff and the `- Paid` receipt.
PASS/KEEP: every lane R/S/T/U string listed in the ledger's Set B; **one render falsehood this
corpus (R51)**.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT over the seven wave-49 `deckN/strategy.txt` files. Sweeps:
render-hedging vocabulary (the wave-48 list + `engine bug|engine will|the engine discards|double
tap|tapped twice`); negative-existence vocabulary (#99/#103/#128); the hard-floor family
(`NEVER bottom|NEVER keep|hard stop|(keeping 5)|LAST look|never mulligan more than|NEVER go
below|you are DONE`); quoted render literals (`Put in Play with` vs `Put a card onto the
battlefield with`); the repeat-row example spelling (`x50`); Devour Flesh's gain sentence; the
`[your battlefield]` / `YOUR OWN` self-target coverage; `Exquisite Blood` coverage; "the game is
lost". Card facts behind the wave-49 edits were verified by the seats against the primitives
(each seat file lists its set); this step re-verified none against Scryfall and no finding
below turns on a card fact.

**TRUST DOCTRINE: CLEAN except B1, all seven guides.** Every hedging hit is a quotation of the
render's own true string (`deck123:459`, `deck130:361` "WHEN THE PROMPT SAYS LETHAL";
`deck152:62`, `deck162:13` "the prompt says so on its own line") or a naming of the MODEL's own
fabricated argument, which #28 requires (`deck126:218` "You have got this wrong: offered A1 ...",
`deck152:222` "THE PARENTHESIS WINS", `deck146:481` / `deck162:384` refuting the plan sentence per
#102, `deck130:255` refuting the guide's OWN deleted sentence). Devour Flesh: `deck123:199-204`
states the true recipient WITHOUT mentioning the row — exactly right while D3 is open. **No guide
teaches doubt of a rendered value.**

| # | file : line(s) | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck123/strategy.txt` : 569 (Skeins bullet: *"the engine discards for you at end of turn"*) · `deck126/strategy.txt` : 22-23 (RULE #1: *"At your Upkeep the engine will offer you a single action: tapping Overgrown Battlement for green mana"*) | **127**, **103**, **99** | **Two guide sentences describe ENGINE BEHAVIOUR that is on the wave-50 docket, and both go false the day their item ships.** deck123:569 is TRUE today and describes D4 (the unrouted cleanup discard) — when the discard becomes an ask, "the engine discards for you" is a false sentence in the exact spot the pilot reads before a draw spell. deck126:22 describes the mana-only gate's LEAK (D12: six single-Battlement windows, three with an empty stack) as if it were the screen's design; when the gate holds, the "single action at your Upkeep" never appears and RULE #1's premise is gone (the instruction — pass — stays right). Same shape as wave-48 B1 (deck123's parser sentence, cut this wave — PASS). | deck123:569 -> state the card fact, not the mechanism: *"not this window unless you can cast what it draws — cards above seven leave your hand at end of turn, and you do not choose which."* deck126:22 -> key on the row, not the phase: *"Whenever a window's only action row is `Add N green mana with Overgrown Battlement` (Upkeep, Draw, the opponent's turn — wherever it appears), answer CHOICE: 0 (pass)."* Keep every other sentence of RULE #1. |
| **B2** | `deck162/strategy.txt` : 334-335 (*"Never bottom SHIELD SPHERE ... and never bottom your only punisher"*) · `deck152/strategy.txt` : 26-29 (*"a flat violation of this rule"* about a ship at (keeping 5) holding a land) | **119**, **131** | **Residual NEVER-sets and absolute wording in two mulligan sections after the pool converged on the odds form.** deck162's two NEVERs are jointly satisfiable at bottom-1 and bottom-2 but at a 5-card keep with Sphere + one punisher + 3 lands and a bottom-2 ask they collide with the LANDS rule two lines down ("bottom one ONLY when the keep would still hold FOUR OR MORE lands") — the deck125 W13 shape of wave 48. deck152:29 calls a ship at (keeping 5) "a flat violation" — the odds-by-look sentence beside it (line 389) is correct; the absolute label is the part #119 retired. No loss at either seat this corpus (deck162's four bottoms were all correct, 0 chains past keep-6), so this is PREVENTATIVE; deck130 (D49-1) and deck126 (F15) closed theirs this wave and the pool's floor text is otherwise gone. | deck162: turn the two NEVERs into the head of the preference ORDER (*"keep in this order: Shield Sphere, your only punisher, lands to four, then spells by cost"*) so any N is satisfiable; delete "Never". deck152:29: replace "a flat violation of this rule" with the odds ("a fresh six is no likelier to hold two lands than this five and is one card smaller"). Run #131's twelve-hand check on both sections. |
| **B3** | `deck123/strategy.txt` : 84 (the repeat row's example quoted verbatim: *`e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x50)"`*) | **103**, **128** | **A quoted render literal that D11 will change.** The guide quotes the row's example exactly (correct today, and #128's grep finds it on every repeat row); D11 asks the engine to print `x<N>` and the creature count instead. When it ships the quotation is a string no prompt contains — the wave-48 B1 shape again. Line 98's history ("x50, x120 and x100 ... with M already at 24 ...") is fine as history. | Quote the ROW's shape, not its number: *`e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x<N>)" — the row's example shows the format; the number is YOURS, from the PLAN line's subtraction.`* Re-check after D11 ships. |
| **B4** | `deck123/strategy.txt` : 199-204 (Devour Flesh: *"Naming the opponent does NOT gain YOU anything ... You never gain life from Devour Flesh"*) | **trust doctrine**, **99** | **PASS — recorded so nobody "fixes" it.** The row currently prints the FALSE `you gain N` (D3); the guide states the true recipient without quoting or contradicting the row. This is the correct posture while D3 is open: a card fact, not a render hedge. Do NOT add "the row says you gain but ..." — and after D3 ships, the sentence stays true. | none |
| **B5** | `deck125/strategy.txt` : 339 (*"it is a hard stop: do not cast Lightmine Field while ..."*) · `deck123/strategy.txt` : 168 (*"THE HARD STOP, because this rule was broken at 14 against 6"*) | **doctrine** (hard caps rejected), **121** | **Two "hard stop" labels on rules that are correctly odds-keyed underneath.** deck125:339 gates Lightmine on the sweeper's second number — a printed count, fine — but "hard stop" is the floor vocabulary the owner ruled out, and the row's own string (`deals 0 until they have an attacker`, W17) now carries the fact. deck123:168 labels the loop stop, which A49-1 re-based on the PLAN line's subtraction; the label predates the re-key. Cosmetic-to-doctrine: the mechanism is right, the word invites the executor to treat it as a cap it may argue past (#119's lesson). | Replace "hard stop" with the reason in both: *"do not cast Lightmine Field off a row reading `deals 0 until they have an attacker`: it costs the turn and deals nothing until they attack."* / *"the stop: past it every further token is a window you pay for and a body a sweeper takes for free."* |
| **B6** | `deck146/strategy.txt`, `deck152/strategy.txt`, `deck162/strategy.txt` (UNCHANGED this wave) | **132**, **117** | **Correct to leave unchanged, with one obligation for the wave-50 seat: mark the UNTESTABLE lanes.** deck146's 146-A/146-B and deck152's 152-B/152-D produced zero windows for one or two corpora; deck162's 162-B had no maker/token split. #132 asks the seat file (not the guide) to flag UNTESTABLE-AT-THIS-SEAT at the second no-window corpus, and #117's offers-by-phase table must show the zero. Nothing to change in the guides; the deck162 guide's Rule 2 keeps its current text and gains nothing until D15 ships (#102's exhaustion clause — a fifth restatement is the one edit forbidden). | Seat-file obligation only; no wording. |

**Not findings — checked so nobody re-files them.** deck123's "(keeping 5) or smaller and the
hand holds one land: keep" (552/574), deck146:441, deck152:514 are the #119 odds-by-look shape
(162-C's), not the wave-48 B2 floor — each sits beside its odds sentence. deck130:419 "There is
no 'the game is lost'. A PLAN line names the NEXT ACTION" is #115 applied (PASS: 1 carry, was
48). deck125's W15 plan sentence is gate-first at both sites (30, 434) — #120 exactly. deck123's
`PLAN: stop at M = ...` (95) is #120's three-blank shape. deck130's `[your battlefield]` pass rule
(178, 250, 448) is #124 at every self-targetable ability. deck146's and deck152's `YOUR OWN`
rules (40/40, 11/11) are the same rule already at ceiling. `Exquisite Blood` appears in deck146
(1) and deck162 (1) — the wave-48 B3 line is present. The 72c mood-word sweep: zero hits
pool-wide. deck123's Marsh Flats literal now matches the emitter (`Put a card onto the
battlefield with`, lines 126/592 — A49-3, #128 PASS).

---

## For the next wave

The wave-50 docket is engine correctness at the top for the first time since the fair-hand
corpora began: **D1 is a regression (in flight at lane V), D2-D4 are rules/decision faults with
a file and a seq each, D5 is the last enumeration, D6 is a vanished menu.** D7-D9 are one-file
parser fixes with PARSETEST cases; D10-D15 are the render's remaining silences, every one the
same principle lane J stated (the row that incurs a cost carries the number) — and D15 is the
first item routed to the engine by the skill's own exhaustion clause after four corpora of a
guide rule that would not take. D18's receipt is cheap and should ship with D1 so the fix is
visible by string. The guide lane is quieter than it has been: three guides unchanged, the pool's
floor text gone, and the new failures all sentence-shape (#120-#123) with the seat's own metric
on each. deck162 is the pool's exit candidate for a second wave — put its exit on the wave-50
agenda if D15 ships and Rule 2 stops being a guide concern; deck152 is exit-eligible if 152-A
and 152-C hold a second corpus. deck146's converter lanes have had no window for two corpora:
flag UNTESTABLE-AT-THIS-SEAT next wave rather than spend an edit. Two guide sentences (B1)
describe engine behaviour that D4 and D12 will change — cut them before the lanes land, not
after.
