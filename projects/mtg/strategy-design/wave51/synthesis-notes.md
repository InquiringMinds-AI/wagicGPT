# Wave-51 synthesis (2026-08-27) — EIGHTH FAIR-HAND CORPUS

Inputs read, in order: `wave50/synthesis-notes.md`; `wave50/strategy-writing-skill.md` (the CURRENT
edition, amendments 1-151); `wave50/general-strategy.md` (R1-R104); `wave50/engine-ledger.md` (the
wave-51 docket D1-D32: D1-D11 and D16-D18 shipped as lanes A-F; D12-D15, D19-D31 carried);
`wave51/review-carry.md`; the four seat files (`seat-engine-narration.md`, `seat-123-130.md`,
`seat-125-126.md`, `seat-146-152-162.md`); all seven `deckN/skill.md`, `deckN/general-suggestions.md`
and `deckN/edit-texts.md`; and the corpus directory
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-155545/` for every reconciliation below.

Outputs: `wave51/strategy-writing-skill.md` (wave-50 edition **verbatim** — prefix `cmp`-verified —
plus **amendments 152-167**), `wave51/general-strategy.md` (R1-R104 with a wave-51 verdict each,
R105-R135 new, GUIDE-only routing, twelve measurement items, PASS/KEEP), `wave51/engine-ledger.md`
(the wave-52 docket D1-D27, ranked, engine/UX only, each with a record citation; both discharge
tables; fallback classes; per-kind prompt chars; lategame specimens), and the boundary findings
below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the
purpose. **No guide file was edited by this step.** Step 4 was run as a report (B1-B8). Two
proposals that would have removed a legal window were rejected in that form and adopted in a
constraint-safe form; hard caps were proposed by nobody; every mulligan text quoted by the seats
is odds-form. **No deck-content question is open for the owner.** The one item the engine seat
labelled "OWNER QUESTION" (the deck146 mull-to-2) was a deck-content call under the doctrine and
was ruled by the reviewers (seat-123-130's odds ruling, seat-146's 146-M) — recorded, not routed.
No file under `projects/mtg/bin/Res/**` or `src/**` was modified.

---

## Reconciliation against the records

1. **The "deck123-vs-deck162 dead-loop, excluded" note is WRONG for the THIRD wave running and is
   dropped.** `seat-125-126.md`'s header carries it; the other three seats each caught it. Verified
   on disk by this step over all 42 JSONL files: 42 `gamestart`, 42 `gameend`, 2,129 decisions
   across seven kinds, exactly 5 `fallback` fields (`named_row_reask` 3, `unparsed_reply` 1,
   `stale_livelock` 1). Every corpus-wide rate uses 21/21; seat-125-126's per-seat numbers are
   unaffected.
2. **Fallback totals agree once summed per seat.** Engine seat 5/2,129; seat-123-130 3 (a re-ask
   recovered, the unparsed reply, the livelock); seat-125-126 1 (a re-ask recovered); seat-146 1
   (a re-ask recovered). 3 + 1 + 1 = 5. Baka-executed 2 (the livelock, the unparsed reply).
3. **Lane B: 1/421 (engine seat) vs 0/110 (seat-146) vs 0/136 (seat-125-126) — all three are
   right on their denominators.** The engine seat's exhaustive subset check over all 443
   receipts found the ONE genuine land case (`152 vs125` seq 39 `Paid {g}{w} for Katilda ... with
   Overgrown Farmland #1; Plains #2` with `Lair of the Hydra {g}; Hengegate Pathway {w}` on the
   seq 38 sources line — verified on disk); the two deck seats excluded creature/non-land sources
   and did not reach it. Recorded as 1/421 with the residual classes named. Seat-125-126's
   "Battlement over-tap" is a DIFFERENT defect (a variable source's count), not a lane-B
   regression — D4.
4. **The H1 window count.** Seat-123's script: 51 opponent-turn empty-stack priority windows / 41
   with an `[instant]` / 0 with a Cast row. This step's stricter re-count (header `It is the
   opponent's turn`, stack line empty or absent): 36 / 24 / 0. The seat's 51 is carried as the
   count (its heuristic is looser on the stack line); the ZERO is concordant and is the finding.
5. **The livelock's menu.** Seat-123 wrote that the fallback "took row 1 - Devour Flesh"; verified
   on disk: seq 48's `options_text` is exactly [`Cast Devour Flesh ...`, `Cast nothing right now
   (combat comes next this turn)`] — a pass row existed and the heuristic did not take it. That
   fact is D1's second ask (a fallback never executes a non-pass row when a pass row exists).
6. **Kaya's self-exile row.** Seat-146 could not read the row past 400 chars (`{if yo...`) and the
   engine seat counted 74 untagged rows. This step read `146 vs125` seq 284's full `chosen_text`
   on disk: no `{this hits YOUR permanent}`, no `{if you ...}` clause on that row. D9 stands as the
   engine seat filed it.
7. **`[already owned:` takes: 21 by the engine seat; seat-123 counts "1 second Alarm + 2 Tutors + 1
   engine" and seat-125-126 "2 second copies, not dead"; seat-146 "0/16 dead".** Consistent once
   the class split is applied (15 stackable, 2 legendary, 4 Tutors of which 1 was the correct
   Tutor). D10 is the class split, not a count dispute.
8. **The `[Upkeep offer:` annotation lives in `prompt` only** (appended after `{card text:}`; never
   in `options_text` / `chosen_text`) — the engine seat's note; seat-146's "34 of 35 declined"
   counted it from the prompt. Not a defect; recorded so the wave-52 reviewers grep the right
   field.
9. **PUT mis-execution mechanism is NOT pinned.** Seat-146's read of `parseAttackerSet` /
   `uniqueNameMatch` explains a miss, not row 2. Docketed with the exact reproducible input and a
   PARSETEST ask; lane G owns the trace. Verified on disk: `choice: 1, chosen_text: "Liliana's
   Caress"` against reply `PUT: 3 (Forced Fruition)`.

---

## The headline: the lanes shipped clean, and the losses moved to the seams BETWEEN answers

**(a) SIX LANES SHIPPED AND EVERY WAVE-50 HIGH WAS DISCHARGED BY STRING.** Soul Shatter: 0/15
no-sacrifice, two planeswalkers taken when highest, the `{right now: they sacrifice <name> (MV N,
their highest)}` clause matching the narrated victim 6/6 — the card-script-first rule paid on its
first outing. The planner: 1/421 plannable receipts used more multi-colour sources than the
minimum (was 69/386), 0 over-taps. The re-ask: 3/3 recovered, `stale_echo` to Baka 0/N (was 3/3).
The receipt: 53/53 activations, `with no source tapped` 0, `turn` 2,070/2,070. The tags: DIES/
SURVIVES 428/428, `{feeds:}` 78/78, `[Upkeep offer:]` 0/35 Hive takes, `{spends K}` 0/141
Hammer-class, bare back faces 0. **No render falsehood this corpus** (second wave running).

**(b) THE NEW DOCKET'S TOP FOUR ARE THE ENGINE ACTING BETWEEN THE MODEL'S ANSWERS.** A repeat-N
loop launched a casting ask per iteration, each drop counted toward a livelock breaker built for a
different failure, and the heuristic got a mid-loop decision with a pass row on the menu — and
cast an edict the model then aimed at itself (D1, a NEW fallback class, IN FLIGHT lane G). A
cleanup `PUT: 3 (Forced Fruition)` executed row 2 (D3, lane G). The pass-row echo matched the
annotation vocabulary of edict rows and executed a dead Tutor, then fell to Baka (D2, lane G). The
payment planner tapped three Overgrown Battlements for a cost one covers and floated 13 green
into a combat that took 18, with the row's `{paying this taps:}` forecast disagreeing with the
receipt (D4, IN FLIGHT lane H). None of these is a render omission; each is a decision the model
made correctly and the engine executed differently.

**(c) THE NEXT THREE ARE WINDOWS AND PRICES THE RENDER STILL OWES.** Instants are never offered on
the opponent's turn on an empty stack (0/41 windows with an `[instant]` in hand) while the core
prompt promises "priority again later this turn" (D5) — it cost a Tribute-for-15 at Emrakul. An
index/name conflict with both on the menu executes the index (D6). A Talisman-paid cast at 1 life
prints nothing lethal (D7 — wave-50's D26, promoted by three takes in one game, the last fatal).

**(d) THE GUIDE LANE'S LOSSES ARE A PRINTED COUNT OR TAG READ WITHOUT ITS EXCEPTION, AND A
MULLIGAN CLAUSE CARRIED PAST ITS LOOK.** Lane F's `converters: K` was read as the brake in four
windows with no Mine out and read past once with one out — the exemption lived three paragraphs
away (#156). `[already owned:` warned against the correct Tutor; `[from exile]` had no word in
the guide and was narrated as "illegal" four windows running (#154). A SURVIVES row was fired at
anyway (the guide's "ability rows print NO result" sentence stayed a corpus after lane E made it
false — #154). deck146's colour clause became the pilot's PLAN at look one and shipped three
hands STEP 1 said keep — a two-card keep and an 80-turn loss (#153). And #118's ceiling got its
re-derivation form: the Lantern breaks (four corpora, all wins) shared a printed fact the rule
never named (#157).

**(e) TWO PROPOSALS ARE ADOPTED IN A NARROWER FORM THAN OFFERED.** The Upkeep-animation DEFER,
re-filed with a lethal-check condition (deck152 G-1) — still removes a window; the planner half
(an animated land is a source of last resort) is adopted instead (D13). "Auto-pass other asks
during a repeat loop" (seat-123 H4) — adopted as "do not launch / do not count drops of requests
the loop will invalidate" plus "a fallback never executes a non-pass row" (D1), which removes no
window the model would have answered.

---

## What merged into the skill (152-167)

**Convergence merges (>= 2 seats):**
- **152** list-answer asks scored reply-numbers == `chosen_text` FIRST; #143 counts action words —
  deck146 S-3 + deck162 S-2 + deck125 S4.
- **153** one shippable mulligan property; #131's carry step and thirteenth hand — deck146 S-1 +
  seat-123-130's odds ruling + deck126 S4.
- **154** a lane that adds a tag re-keys the guide the same wave; #139 per corpus with a
  tag-coverage column — deck130 S1 + deck152 S-3 + deck123 S1.
- **155** a two-printed-number gate moves onto the row via the sibling family's clause; hidden
  costs keyed to the paying-source string — deck125 S1 + deck130 S3.
- **157** #118's re-derivation form; a timing rule names its catastrophe's card class; a role
  rule names the job — deck126 S1 + deck123 S3 + deck130 S4.

**Decisive single-seat merges:**
- **156** a printed count is the strongest key; exceptions in the same sentence (deck162 S-1).
- **158** at a second identical failure, read the NEXT paragraph for an outranking imperative
  (deck130 S2).
- **159** #151's sentence carries both halves: a printed row is payable (deck126 S2).
- **160** ownership picks read both lines (deck126 S3).
- **161** opponent-behaviour exceptions checked against the pool's primitives (deck125 S2).
- **162** size rules carry their life-critical branch keyed to the header (deck125 S3).
- **163** per-card exceptions written for both sides of combat (deck152 S-1).
- **164** "the only answer" gets #145's grep for a return path (deck152 S-2).
- **165** tag-keyed absolutes go INSIDE the numbered procedure (deck146 S-2).
- **166** a non-mana mana source is taught as a cost: count, window, colour (deck123 S2).
- **167** rotation on open lanes; per-loss window / no-window (deck162 S-3).

## What was REJECTED, and why
- **The Upkeep-animation DEFER with a lethal-check condition (deck152 G-1).** Removes a legal
  window and encodes a guide judgment in the engine; the planner half is adopted (D13). Same
  governing constraint as waves 49 and 50.
- **"A repeat should auto-pass other asks between iterations" (seat-123 H4, first half) as
  written.** Adopted as not-launching / not-counting the requests the loop invalidates (the row's
  own contract), never as auto-answering a window the model would see (D1).
- **A `0. Do not announce` / Decline row on the X menu shipped now (deck130 R4, seat-123 L1).**
  Second corpus, still gated on the engine ruling whether the card returns to hand after `Cast
  Card Normally` (D22). The cast-row clause form from wave 50 stands.
- **The WALL CHECK summary line as a MED render item (deck130 R2).** The guide's ordering fix
  (D51-1, #158) ships first; the row form only if it fails a third time (D26).
- **The X-menu header carrying the seat's life (deck125 G2).** The guide edit (W30, #162) ships
  first; LOW (D26).
- **deck146's 3-3 / deck162's 2-4 / deck125's 2-4 as guide effects.** n=6, real hands: a two-card
  keep, a land-destruction draw, three no-window losses. Counts only (#87, #167).
- **Any guide compensation for D1-D4 (the engine acting between answers).** Trust doctrine: a
  guide describes cards and printed strings. deck162's 162-H ("numbers only") is accepted as the
  emit-form of the ask's own instruction, not as a description of the defect, and retires when
  D3 ships.
- **A fourth restatement of deck146's Hive `[repeat:` absolute or of deck130's wall check.**
  #138 routes both to the render (D12) / the ordering fix (#158); the seats' own routing.
- **"The engine seat's OWNER QUESTION on the mull-to-2."** Not an owner question under the
  doctrine — a deck-content call; ruled by the reviewers in odds form (146-M). Recorded.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE, ranked: livelock IN FLIGHT lane G (R105) · echo vs annotation IN FLIGHT lane G
(R106) · PUT executed row 2 IN FLIGHT lane G (R107) · Battlement over-tap IN FLIGHT lane H (R108)
· no opponent-turn Cast row + the false core-prompt sentence (R109) · index/name both on menu
(R110) · Talisman lethal (R111) · MED: per-token rows uncollapsed (R112), self-exile tag (R113),
`[already owned:` classes (R114), `[from exile]` cause/tax (R115), `[repeat:` re-offer (R116),
animated land tapped (R117), conflict scope (R118), -1/-1 DIES/SURVIVES (R119), fetch makes no mana
(R120), `{spends K}` on cast rows (R121), pool-paid `{leaves N}` (R122), victim tags promoted
(R123) · LOW: payee (R124), `#N` (R125), X-menu exit (R126), pass-note noise (R127), WALL CHECK
line (R128), self-life-loss row (R129), chooser narration (R130), Caress K qualifier (R131), the
log-share decision (R132), latency (R133), X header life (R134), no deck name (R135).
Of R72-R104: **R72, R73, R74, R76, R80, R81, R82, R87, R88, R89 SHIPPED and PASSED**; R75 shipped,
untested, scope short; R77/R78/R79 shipped, rendered, each exposed a sibling; R84 and R97
carried and PROMOTED by a failing window; R70 closes on a second clean corpus.
GUIDE-only: listed per deck in `general-strategy.md`.
MEASUREMENT: twelve items, headed by the repeat-take receipt check and the reply-vs-executed list
check.
PASS/KEEP: every lane A-F string in the ledger's Set B; **no render falsehood this corpus**.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT over the seven wave-51 `deckN/edit-texts.md` proposals and the wave-50
`deckN/strategy.txt` files they amend (the reviewers' wave-51 guides are theirs to commit). Sweeps
by the seat files' own quotations and this step's reading of the edit texts: engine-behaviour
vocabulary (#99/#103/#136/#154), the hard-floor family, quoted render literals per row family with
the new tag column (#139/#154), the cross-guide KEY diff (#144), the Oracle/primitive class and
effect words (#145/#164), the mulligan single-property rule (#153), the list-answer emit form
(#143/#152). No finding below turns on a card fact this step did not see quoted from a primitive.

**TRUST DOCTRINE: CLEAN, all seven.** No guide teaches doubt of a rendered value; 162-H's "numbers
only, nothing in brackets" is the emit-form of the ask's own instruction, not a description of
D3 — PASS, and it retires when D3 ships. deck126's F22 "the engine prints only casts it can pay
for" is a fact about the row family (#159) — PASS.

| # | file : site | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck146` STEP 1/2 (146-M) — and every other guide's mulligan section | **153** | **PASS at deck146 (the colour clause is cut; the carried-plan paragraph ported). The #144 grep for the holdouts: deck125's and deck130's sections were untouched this wave — deck130's "coverage" keep of a ONE-land seven (`130 vs126` seq 1) is inside its rule and won nothing/lost nothing; but neither section has been checked for a SECOND shippable property since #153 was written.** | Each reviewer runs #153(a): list every property the section lets the pilot ship on; if more than the land count / coverage line, convert the rest to bottom-ask questions in odds form. Then #153(b)'s carry step on the one remaining clause. |
| **B2** | `deck162` 162-G (RULE 2 keyed to `{feeds:}` K) | **156** | **PASS — the exemptions are in the K sentence.** One obligation: when D26's Caress qualifier ships (`converts only past 7 cards in hand; their hand: N`), the guide's hand-size sentence becomes redundant but not false — leave it until the string is on the row, then cut (#154). | none now. |
| **B3** | `deck130` D51-2 (the "ability rows print NO result" paragraph replaced) and `deck123` A51-3 (`[from exile]` named) | **154** | **PASS — recorded so the pattern is visible: both are the same-wave re-key #154 now requires.** Two obligations for wave 52: when D11 ships, A51-3's sentence is re-keyed to the new `[from exile - your card, exiled by ...]` literal; when D10 ships, deck123 A51-6's and deck126 F23's `[already owned:` sentences are re-keyed to the `[finds: <half>]` form — both in the REVISION, before the corpus. | none now; add both to the wave-52 reviewer brief's #136 checklist. |
| **B4** | `deck152` 152-E (three PASS-on-sight Lair forms incl. the `[Upkeep offer:` literal) | **154** | **PASS with a caveat the reviewer should know: the `[Upkeep offer:` annotation is in the `prompt` field only (appended after `{card text:}`), never in `options_text`/`chosen_text`.** The pilot sees it; the reviewer's grep must read `prompt`. When D13's `- it was animated this turn` lands on the Main-1 cast row, key the "then tapped for mana" sentence to it. | none now. |
| **B5** | `deck125` W27 (Scatter counts against EVERY opponent, price 2) | **161** | **PASS — the pool check was done in the edit text ("Every deck in this pool casts creatures").** Obligation: the wave-49/50 boundary B6 (Essence Scatter's `[creature]` stack tag) is now moot — the guide no longer scopes the Scatter by opponent; do not re-file it. | none. |
| **B6** | `deck126` F21 (Lantern above the walls when a five-mana piece or Tutor is in hand at <= 4 sources) | **157** | **PASS — the re-derived condition is keyed to the hand line and the `Mana available:` line, both printed.** One check: "Idyllic Tutor" in the condition is a Tutor whose own cost is {2}{w} — the Lantern's fifth mana matters for the ENCHANTMENT it fetches, not the Tutor; the sentence reads correctly (the Tutor is on the hand line as a proxy for the enchantment coming) but a pilot may read "Tutor in hand -> Lantern" at 2 sources. | Optional: "... or Idyllic Tutor (the enchantment it finds needs five) is on your hand line ...". |
| **B7** | `deck123` A51-8 (annihilator picks: lands first, the Alarm last) and `deck146` 146-P (a land after a second walker) | **152** | **PASS — both are list-answer orders in emit form. Obligation: the wave-52 seats run #152(a) (reply numbers vs `chosen_text`) on every annihilator / cleanup ask at these seats BEFORE scoring the order — D3's parser defect can make a correct PUT look like a broken order.** | none. |
| **B8** | `deck130` D51-3 ("Talisman pays: life L -> L-1" keyed to `{leaves 0 of your 1}` + the sources line) | **155** | **PASS — the first guide text keyed to the paying-source string.** When D7's `{pays {r} with Talisman of Impulse: you take 1 - life 1 -> 0, LETHAL}` ships, re-key the dictated sentence to that literal and cut the sources-line derivation (#154). | none now. |

**Not findings — checked so nobody re-files them.** deck123 A51-13 (own-upkeep narrowed to sweeper
logs): #157 exactly, odds-form, the sweeper names are printed in the log — PASS. deck123 A51-10
(GANG-BLOCK ported by tag string): #137/#144 — PASS. deck125 W29 (Emrakul vs edict-without-
converter at their <= 15): keyed to the printed life and the `{effect:}` line — PASS. deck125 W30
(Revelation as a life spell at <= 5 with header >= 1): #162 exactly — PASS. deck126 F23 (both
ownership lines): #160 — PASS. deck126 F24 (two-land no-coverage, odds form, "12 of your 24 lands
make {W}"): #153(c) — PASS. deck130 D51-6 (Blastminer's job stated): #157/#135 — PASS. deck130
D51-1 (wall check as the floor's first case, arithmetic in emit form): #158 — PASS. deck152 152-G
(Katilda A-row): #163 — PASS. deck152 152-H (Spellbinder as a tax; Tutor as the third name): #164 —
PASS. deck146 146-N (Kaya all-`[your battlefield]` -> +1 or pass): keyed to the printed `[your
battlefield]` on every row — PASS; when D9 ships, re-key to `{this hits YOUR permanent}` (#154).
deck146 146-O (Silencer naming order): keyed to the Pelakka reveal, the opponent's line, and
`[defender]` — PASS. The 72c mood-word sweep: zero hits reported by any seat. The wave-50 B1
cleanup orders: written at 130/146/152/162 — PASS (130 untestable, 0 asks in two corpora).

---

## For the next wave

The wave-52 docket is engine correctness at the top for the third wave running, and for the first
time the top FOUR are the engine acting between the model's answers rather than the render
omitting a fact: **D1 (the livelock breaker firing inside a repeat loop, and a fallback that took
a non-pass row) and D2/D3 (the pass-row echo and the PUT parser executing a different row) are
IN FLIGHT at lane G; D4 (the planner tapping every variable source and the forecast disagreeing
with the receipt) at lane H.** D5 is the largest window class the pilot has never had (instants on
the opponent's turn) and the core-prompt sentence that promises it is false until it ships; D6 and
D7 are one-file parser/render fixes with PARSETEST cases and a suite pin each. D8-D17 are the
render's remaining silences and conflations — every one the lane-J principle (the row that asks
carries the number), and three of them (D10, D11, D13) are re-keys the guides are already waiting
on (#154). The guide lane's work this wave is the two audits the corpus proved (#153 on every
mulligan section, #154 on every row family a lane touched) and the list-answer check (#152) — run
them BEFORE the corpus, not after. deck152 is exit-eligible in wave 52 if 152-G/H and the Lair
tag hold; deck162 waits on 162-G's first corpus and D3; deck146 has two new lanes open; deck125
and deck126 re-test W26-W31 / F21-F24 before any freeze talk; deck123's Flats and `[from exile]`
lanes are first-corpus; deck130's wall check is at its second restatement and #158 names the
mechanism — the third corpus decides the row route. The log-share decision (D23) is the owner's:
the 30 KB bar broke for the first time, on one 80-turn game, and D8's row collapse is the lever
that ships first; re-measure with it in.
