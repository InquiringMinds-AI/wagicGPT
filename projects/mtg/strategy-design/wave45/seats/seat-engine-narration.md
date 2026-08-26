# Wave-45 engine + narration validation seat (harvested 2026-08-26)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-131833/` — 42 seat logs, 21 games,
**1909 records / 1825 decisions** (1237 ask, 427 priority, 104 attackers, 33 blockers, 16
reveal, 8 bottom; + 42 gamestart / 42 gameend). Fallbacks **2 / 1825 = 0.11%** (w44 0.28%,
w43 0.17%). Total inference 11.25 h. Games ended turn 10–54, mean 20.0.

**Emitter authority.** Every render verdict below was adjudicated against the emitter's
literal string read out of `src/AIPlayerGPT.cpp` / `include/GptPlanCaveat.h` at the working
tree, which is byte-identical to master `38450dc62` for both files (verified). The corpus
binary is `fd69fadc2`; the only emitter delta between them is **lane E (#W45-20, owner
prefix on damage history lines)**, so every unprefixed damage line in this corpus is
expected, not a finding — it is the wave-46 baseline instead (see NEW-6).

No `bin/Res`, `src/` or git-state edits were made; nothing was built or run.

---

## PREDICTION SET B — adjudication

### #W45-1 (lane A, menace block side) — **PASS on all three legs** (low N: one window)
Emitter strings checked: `menaceBlockPairingTag` →
`" (no 1-on-1 result exists - only a block by TWO OR MORE of your creatures counts; this one
alone does not block it at all)"`; `zeroPowerBlockerTag(anyMenace)` → the
`"...so it stops NOTHING there..."` branch; `blockCountRequirementTag` → `" [menace - only a
block by TWO OR MORE ...]"`.

- **Bare 1-on-1 verdicts on menace-attacker B-rows: 0 / 3 rows.** Exactly ONE blockers window
  in the whole corpus offered a menace attacker:
  `1787768324-ai_baka_deck162-0x55ab5151ec30-vs-ai_baka_deck146.jsonl` **seq 19** (t14,
  14 vs 10). A1 Hive of the Eye Tyrant (3/3) carries the A-line requirement tag, and **all
  three** B-rows (Shield Sphere #1, Shield Sphere #2, Fate Unraveler #2) render
  `A1 (no 1-on-1 result exists - ...)` in place of the trade verdict, with A2/A3 still
  carrying real verdicts on the same rows. Wave-44's 8/8 defect is gone.
- **`zeroPowerBlockerTag`'s menace branch: 2 / 2 eligible rows.** Both Shield Spheres render
  `[deals 0 - this block kills nothing, and against any attacker whose A-line states a
  two-or-more requirement this creature alone does not block it at all, so it stops NOTHING
  there; against the others it stops all of the damage from whichever attacker it blocks -
  each A-line above says how much]`. The wave-44 false "it STOPS all of the damage" on a
  menace board is not producible here.
- **Solo menace blocks: 0.** The seat declared `B3:A2, B2:A3, B1:none` — no solo block on A1.
- **Menace-caused `dropped_assignments`: 0.** See the classification below.

Attack side (unchanged lane, re-verified): **16 menace A-lines across 13 attackers windows**,
all carrying `menaceAttackRestrictionTag`. 13 of 16 report `they have 0 untapped creatures
... so it cannot be blocked at all this combat` and correctly carry **no** `their untapped
blockers` tag (the caller cleared `entries`). The 3 above-minimum cases keep the forecast
under the once-per-prompt menace scope line. No over-claim found.

#### ALL 7 `dropped_assignments`, classified
All 7 come from **one record**:
`1787768326-ai_baka_deck162-0x560dd1e6c400-vs-ai_baka_deck152.jsonl` **seq 14** (t10, 10 vs 13).
The window offered A1–A3 and **exactly one** blocker row, B1 (Fate Unraveler 3/4). The reply
was `BLOCKS: B1:A2, B2:none, B3:none, B4:none, B5:none, B6:none, B7:none, B8:none`. B2–B8 name
blockers that do not exist; the parser counted each as a dropped assignment.

| # | cause | count |
|---|---|---|
| `Bn:none` for a blocker index that was never rendered | 7 | 
| menace / declaration-set restriction | **0** |
| any other legality refusal | 0 |

So the carried docket item "`dropped_assignments` conflates `Bn:none`" is **confirmed at 7/7**:
the counter has zero signal about illegal blocks in this corpus, and the seat's actual block
(B1:A2) was legal and executed. The metric needs splitting before it can be a lane gate again
(see NEW-1).

### #W45-2 (lane A, collapsed tag + gang price) — **render PASS; behaviour split verdict**
Emitter strings: `gangBlockPriceTag` → `"GANG BLOCK: any N of them together deal D, enough to
kill this attacker; each result above is a LONE blocker only"` / `"their N biggest together
deal D, ..."`; collapse selector `kPotentialBlockersEnumerateMax = 4` (now a named constant,
`AIPlayerGPT.cpp:2195`).

- **231 attacker A-lines; 97 carry a `their untapped blockers` tag; 4 are COLLAPSED; 17 carry
  a `GANG BLOCK:` clause.**
- **Gang price present vs silently skipped — 15 / 15 priceable, 0 silently skipped.** Measured
  over the 52 enumerated multi-candidate A-lines by re-running the caller's own arithmetic
  (largest-first `need`, `need >= 2`, combined power >= attacker toughness) off the rendered
  `(P/T)`s: 15 lines were arithmetically priceable and **all 15** carried the clause; the other
  37 were correctly silent (a lone blocker already kills it, or no gang reaches the toughness).
  **No line carried a clause the arithmetic did not support.** The `gangOk` suppressors
  (deathtouch/wither/infect/prevention among the candidates) never fired in this corpus, so
  that leg is UNTESTED.
- Of the 4 collapsed lines, **2 are priced and 2 are correctly silent**: Nadaar (3/3) vs a
  4/4 biggest → `need = 1`, gang clause suppressed by design; Dwarven Blastminer (1/1) vs a
  0-power wall board → no gang reaches 1. Both silences are honest.
- **Behaviour — "attacks taken on a collapsed tag with N>=2 that end with the attacker dying".**
  Read as *"a collapsed tag carrying a priced gang"*: **0 / 2** → PASS. Both priced attacks
  went in and the attacker LIVED (`...deck162-...-vs-deck123` seq 23: Fate Unraveler 3/4 into
  14 blockers, lone-blocked by Bloodline Keeper, killed it and lived; `...deck152-...-vs-deck123`
  seq 19: Luminarch Aspirant 7/7 into 12 blockers, triple-blocked for 4, lived).
  Read as *"any collapsed tag"*: **1 / 4** → FAIL on the letter.
  The one death is
  `1787768346-ai_baka_deck146-0x56532a72c410-vs-ai_baka_deck123.jsonl` **seq 23**: Nadaar
  attacked on a collapsed tag whose single shown outcome was the **unfavourable** one
  (`biggest Vampire #1 (4/4) (your attacker dies, their blocker lives)`), was lone-blocked by
  Lord of Lineage (5/5) and died. **The collapse mechanism is not implicated** — no gang was
  involved, and the row truthfully predicted the death. This is STRATEGY, and the literal
  metric is a bad proxy for the lane; restate it for wave 46 (see NEW-5).

### #W45-3 (lane B, block-triggered life + converter) — **render PASS, one leg UNTESTED-as-stated**
Emitter: `combatTradePreviewStats` → `"(blocking trigger: you gain 3)"` /
`"(blocking trigger: they gain 3)"` / `"(blocking trigger: they may gain 2)"`; summary
`"LIFE-TO-DAMAGE CONVERTER on the battlefield: ... theirs - <name>. While it is in play, ..."`.

- **The tag renders, in all three voices, 3 rows / 3 windows.** Blockers voice:
  `deck126-...-vs-deck146` seq 9, Pride Guardian `(your blocker dies, attacker lives (blocking
  trigger: you gain 3))`. Attackers voice: `deck146-...-vs-deck126` seq 7, Pride Guardian
  `(you kill it, your attacker lives (blocking trigger: they gain 3))`.
- **FIRST LIVE OBSERVATION of the TargetChooser leg — PASS.**
  `1787768348-ai_baka_deck130-0x55d604650990-vs-ai_baka_deck126.jsonl` **seq 19** renders
  `biggest Wall of Omens (0/4) (neither dies (blocking trigger: they may gain 2))` on a board
  whose `Opponent battlefield` line shows **Perimeter Captain** alongside the Wall. Verified
  against the primitives: Wall of Omens has NO blocking trigger (`mtg.txt:131250`, `auto=draw:1
  controller`); Perimeter Captain has `auto=@combat(blocking)
  source(creature[defender]|mybattlefield):may life:2 controller` (`mtg.txt:85397`). So the
  source-filter leg correctly attributed a THIRD permanent's trigger to the wall's pairing, and
  correctly voiced the scripted `may` as optional. Pride Guardian's certain 3 verified likewise
  (`mtg.txt:88355`, `source(this):life:3 controller`).
- **Block-trigger life gained in any single deck146 game: max 3 — PASS (< 6).** Exactly one
  block-trigger life event in any deck146 game across the corpus (Pride Guardian, +3, in
  146 v 126). Wave-44's 13 → 42 climb did not recur at n=6.
- **Converter line: 5 prompts**, all attackers windows, all `theirs - Sanguine Bond`. Correct
  where it fires — but see NEW-2: the converter is named ONLY on the attackers window, and the
  worst corpus board (Sanguine Bond + 2 Exquisite Blood) never reached one.
- **"`(neither dies)` bodies sent into a 2+-defender board 0/N" — FAIL on the letter (6/7),
  but NOT a misplay.** All 6 sends are Silverquill Silencer / Goblin / Nadaar into **Shield
  Sphere** or **Wall of Omens** boards. Verified from the primitives: Shield Sphere's trigger is
  `counter(0/-1) all(this)` (`mtg.txt:103831`) and Wall of Omens has none — **neither gains
  life**. Attacking into a wall that neither dies nor gains is free: it is blocked for 0 or it
  connects. The metric conflates "defender board" with "life-gain board"; the mechanism the lane
  cares about (life fed to the blocker) fired **0 times**. Restate for wave 46.

### #W45-4 (lane D, plan-mismatch note) — **PASS on every leg**
Emitter: `kStalePlanNote` (`AIPlayerGPT.cpp:265`), gated by
`gptcaveat::planActionsStale` at `AIPlayerGPT.cpp:6667`.

- **Fires: 261 / 1909 records = 13.7%; 261 / 1824 prompts = 14.3%.** Prediction "< 25%" **PASS**
  (lane D's replay estimate was ~12.5%; wave 44 was 42.8%). Against the only two ask families
  the gate can arm on it is 261/1031 = 25.3%, which is the honest denominator to quote.
- **Land-drop fires: 0 / 374 — PASS** (was 327/419). Classified every prompt by its ask header
  line: fires are **188 on `Casting decision`** (of 605) and **73 on `Your legal actions`**
  (of 426). **Every** `castFreeAskHeader` family is at **zero**: land drop 0/374, target choice
  0/60, `Choose an option for X` 0/57, pre-game mulligan 0/54, sacrifice 0/18, dungeon room
  0/14, ANNOUNCE_X 0/14, `A choice is required` 0/9, venture 0/8, exile 0/7, `Choose one mode
  for X` 0/5. Attackers/blockers windows produce no numbered rows and fire 0.
- **Strict false fires: 0 / 261 — PASS.** Tested the note's literal claim on **all 261 fires**
  (not a sample): for each, extracted the card each option row OFFERS (verb head, `with <card>`,
  `Flip Side with <card>`, target enumerations stripped as the emitter strips them) and matched
  the pre-comma head against the carried plan. **Zero fires had a plan-named card on an option
  row.** A 22-record shortlist thrown up by a looser substring scan was hand-read and is 100%
  generic-word noise (`Defender`, `Destroy`, `Counter`, `Choose`, `Return` inside `{card text:}`).
- **Precision, hand-verified sample n = 45 (seeded random over the 261): 45/45 claim-true**
  → 100%, clears the >= 95% bar. Reading each one: in every case the plan's named cast /
  activation / equip genuinely was not offered by that menu.
- **Residual (usefulness, not truth):** **129 of 261 fires (49%) repeat on an UNCHANGED plan
  text at the same seat**, and 136 fires carry the deck125 standing-strategy plan "Answer their
  threats, resolve Staff of Nin, ping their face every turn" — a strategy, not a stale intent.
  deck125 eats 124/337 = 36.8% of its own prompts; deck162 only 5/145 = 3.4%. The note is
  truthful and is now a nag on the seats whose guides teach a standing plan. See NEW-4.

### #W45-5 (lane C, X pricing) — **render PASS on the sweep class; TARGETED class UNTESTED**
Emitter: `xKillRowCore` → `" {X pricing: kills THEIRS: ...; YOURS: ...}"` (with
`"(one target only) "` inserted for the targeted class), collapse
`" {X pricing: same as X=N}"`, `xAnnounceRowKills` gated on `sv.priceable`.

**All 14 ANNOUNCE_X windows, classified by spell:**

| # | file (tail) / seq | spell | rows annotated? | why |
|---|---|---|---|---|
| 1–4, 7–14 (12) | deck125 seats ×4 files | **Sphinx's Revelation** | **NO** | `xSurveyBoard` → `!priceable`: the spell's X buys life + cards, not damage. Rows render bare `X = N`. Lane C's stated behaviour, and the honest one for a damage pricer. |
| 5 | `...deck130-0x559cc3d27570-vs-deck123` seq 21 | **Starstorm** (sweep) | **YES** | `X = 2 {X pricing: kills THEIRS: none; YOURS: none}` / `X = 1 {same as X=2}` / `X = 0 {same as X=2}` |
| 6 | `...deck130-0x55f269ac9490-vs-deck146` seq 18 | **Starstorm** (sweep) | **YES** | `X = 2 {kills THEIRS: Goblin, Triumphant Adventurer; YOURS: none}` / `X = 1 {same as X=2}` / `X = 0 {kills THEIRS: none; YOURS: none}` |

So: **2 / 14 annotated, and both are the only two priceable windows.** The orchestrator's
pre-count ("`X pricing: kills` in only 2 records") is fully explained: 12/14 are Sphinx's
Revelation. **Not a regression** — but it is a real gap, because the bare `X = N` rows say
nothing at all about what X buys, and three of those twelve were answered **X = 0** (windows 2,
3, 8 — a Sphinx's Revelation cast for zero life and zero cards, with 5, 6 and 4 X affordable).
See NEW-3.
- **Casts at an X whose `YOURS:` names the seat's clock: 0 / 2 — PASS** (was 2/2). Both
  annotated windows read `YOURS: none`.
- **`X = N` row coverage: 6 / 6 rows on the two priceable windows carry either the full
  `{X pricing: kills ...}` or `{X pricing: same as X=N}` — PASS.**
- **Plan-vs-chosen X divergence: 1 / 4 windows that named an X — FAIL on the letter.**
  Window 5 planned 0 / chose 0; window 7 planned 3 / chose 3; window 14 planned 6 / chose 6;
  **window 6 planned X=1 and chose X=2**. The `{same as X=2}` row on X=1 was accurate (X=1
  kills both 1/1s), so the seat paid one extra mana for an identical board. Mechanism is a
  collapse-wording asymmetry, not a false surface — see NEW-5b.
- **A TARGETED X spell never reached ANNOUNCE_X** in this corpus, so the
  `"(one target only)"` phrasing remains **UNTESTED** for the second wave running.

### E-3 (subject-less modal menus, dd8bdb503) — **PASS on the modal class; the 9 residual headers are the Teferi sub-ask, and I can name the build path**
- **Modal-spell menus: 0 subject-less headers.** All 57 `Choose an option for <spell>:` and all
  5 `Choose one mode for <spell>:` asks name their spell. **0 `stale_echo` fallbacks** anywhere
  in the corpus.
- **The 9 `A choice is required - choose an option:` headers are ALL Teferi, Who Slows the
  Sunset's +1 side-selection sub-asks** — 6 × `1. choose your land / 2. choose opponent land`
  and 3 × `1. choose your creature / 2. choose opponent creature`, at two seats
  (`...deck152-0x55bef9967010-vs-deck126` seq 21, 24, 38, 41; `...deck152-0x55b6b7adf0d0-vs-deck125`
  seq 24, 42, 55, 76, 87). All 9 answered cleanly, 0 fallbacks.
- **Build path (read, not inferred):** `AIPlayerGPT.cpp:12716–12743`. The CHOOSE_MENU handler
  recovers `ctxName` from `ctx->getDisplayName()`, then `ctx->model->data->getName()`, then the
  ETB-land recovery; when all three come back empty it falls to
  `string("A choice is required - choose an option:")` (line 12743). The Teferi +1 spawns its
  side-selection menu with no card `ctx` to recover, so it lands on the generic branch — while
  the SAME planeswalker's loyalty-ability menu, which does have a ctx, renders
  `Choose an option for Teferi, Who Slows the Sunset:` 15 times in this corpus. This is the
  carried **G-6/G-7** class the carry-list predicted would persist; it is now localized to one
  emitter branch and one missing ctx thread. → NEW-1 on the docket.

### Fallbacks (2 / 1825 = 0.11%) — both at ONE seat, both mechanisms named
`1787768352-ai_baka_deck123-0x56177edb6e40-vs-ai_baka_deck152.jsonl`.
1. **`retracted_choice`, seq 22** (t11, casting decision, 4 options, latency 237.5 s,
   `post_answer_overrun` 3172). The reply's FIRST line is a well-formed `CHOICE: 3 (Cast Idyllic
   Tutor)`; the PLAN: line then argues the seat OUT of it verbatim — *"We HAVE Intruder Alarm on
   battlefield. So Idyllic Tutor is NOT the right play... So we should NOT cast Idyllic Tutor"*
   — and enumerates the options again. `commit_retracted: true`, `choice: -1`, Baka took over.
   **Mechanism: post-answer PLAN prose retracting a committed first-line answer.** Not a render
   defect; it is the reply-protocol seam, and it is the same overrun shape as the 539 s decision
   below. Owner reply-protocol ruling territory — routed, not patched.
2. **`unparsed_reply`, seq 61** (t17, priority, 6 equip rows, latency 154.5 s). The reply is
   degenerate multilingual token soup with no `CHOICE:` anywhere (`coded_answers: 0`,
   `commit_retracted: false`). The prompt is unremarkable — a clean 6-row Equip menu with the
   `Lightning Greaves is ALREADY attached to Human #3` re-attach annotation on every row.
   **Mechanism: model/serving-side generation collapse, not a prompt defect.** One instance;
   do not spend a lane on it, but pin it — a second occurrence would make it a serving item.

### The 539 s decision — a RENDER ambiguity, and it is the corpus's longest decision
`1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` **seq 26** (t15, ask,
**2 options** — `Play Swamp` / `Hold Swamp`). 539.0 s, reply 11,084 chars, of which
`post_answer_overrun` 11,061 and `post_plan_overrun` 10,360. Answered correctly on line 1.
The trace's terminal ~1,500 chars are the model adjudicating a **contradiction it found in the
render**: the history log says `- You discarded Pelakka Predation` (turn 13) while
`Your hand:` still lists `Pelakka Predation {2}{b} [sorcery]`. Verbatim: *"This is a
contradiction. Usually, the current hand list is the ground truth... I will trust the hand
list."*

**Both surfaces are TRUE.** The seat drew two copies (seq 2: `- You drew Pelakka Predation`
twice), discarded one at t13 and one at t16 (seq 28). But the discard history line is a **bare
card name with no copy disambiguator**, and the hand line **drops the `(copy N of M in your
hand)` tag the moment the count falls to 1** — so the two surfaces are individually honest and
jointly unresolvable. Under the trust doctrine this is the exact failure the doctrine exists to
prevent: the render made the model doubt the render, and it cost the single most expensive
decision in the corpus (539 s on a 2-option land drop; corpus median is 16.5 s). → **NEW-7, HIGH.**

### The 8 mulligans — new-baseline record (NO patch, per the carry)
8 of 42 seats mulliganed (34 kept at 7). 8 `bottom` records, all bottomed real cards.

| seat file (tail) | mulls | kept-hand lands (pre-bottom) | bottomed | FINAL hand |
|---|---|---|---|---|
| `deck125-0x55ed9b6ba460-vs-deck126` | 1 | 5 / 7 | Island | 6 cards, **4 lands** |
| `deck146-0x55ab501e54a0-vs-deck162` | 1 | 2 / 7 | Soul Shatter | 6 cards, **2 lands** |
| `deck162-0x560dd1e6c400-vs-deck152` | 1 | 5 / 7 | Drowned Catacomb | 6 cards, **4 lands** |
| `deck123-0x564b59dd4a40-vs-deck126` | **3** | 3 / 7 | Damnation, Vision Skeins, Idyllic Tutor | **4 cards, 3 lands** |
| `deck152-0x55bef9967010-vs-deck126` | 1 | 4 / 7 | Tovolar's Huntmaster | 6 cards, **4 lands** |
| `deck146-0x56532a72c410-vs-deck123` | **3** | 3 / 7 | Acererak, Vanishing Verse, Silverquill Command | **4 cards, 3 lands** |
| `deck152-0x56177da76060-vs-deck123` | 1 | 2 / 7 | Sigarda, Champion of Light | 6 cards, **2 lands** |
| `deck125-0x562f799014b0-vs-deck123` | 1 | 4 / 7 | Plains | 6 cards, **3 lands** |

**=> OWNER QUESTION (not patched).** Two seats **mulliganed to four**: deck123 (vs126) and
deck146 (vs123). Both lost their games. deck123's third mulligan discarded a hand of
Arcane Sanctum + 3× Tundra + 2× Scrubland + Devour Flesh — a genuine 6-land flood, so the
mulligan itself is defensible by odds; the question is whether a guide should ever permit a
**third** mulligan rather than keeping and bottoming lands, since the London mulligan lets a
7-card look be shaved to 6 without going to 4. deck146's chain is the same shape. No mull-to-zero.
Per amendment #46/#47 these grade by ODDS, not conformance, and floors are owner-routed — I am
recording the windows, not moving anything. Both post-launch guide edits (deck125 carve-out
removal, deck146 one-land rule) are excluded from this reading per the carry.

### Carried MED/LOW docket — recurrence check (all still live)
- **Opponent non-creature permanents carry no text: CONFIRMED, and worse than wave 44.**
  `...deck125-0x55ed9b6ba460-vs-deck126` seq 68 (t28) renders
  `Opponent battlefield (13 permanents ...): ...; Exquisite Blood #1 {4}{b} [enchantment]; ...;
  Exquisite Blood #2 {4}{b} [enchantment]; ...; Staff of Nin {6} [artifact] [tapped]; Sanguine
  Bond {3}{b}{b} [enchantment]` — an assembled **Sanguine Bond + Exquisite Blood** loop plus a
  repeating damage engine, and the render gives the reader four bare names, four mana costs and
  four type words. deck125 lost that game.
- **`teferieffect` lexicon leak: 31 prompts**, rendering on the STACK listing as
  `3: ability: teferieffect Counter Removed [triggered/activated ability]`.
- **`Transform:backside` lexicon leak: 141 prompts** — 80 `- You used: Transform:backside with
  Bloodline Keeper`, 58 opponent-voice, and **3 on a live OPTION ROW**
  (`1. Transform:backside with Bloodline Keeper [cost: {b}] ...`). A raw card-script token on a
  decision surface, not just in history.
- **`(neither dies)` / `[defender]` gloss, singular loyalty `(now N)`, no lethal headline on the
  attack side:** all unchanged; no new evidence this wave beyond the above.
- **#W44-4 Day/Night — now PASSES.** 75 prompts carry `It is currently Day.` / `It is currently
  Night.` in CURRENT SITUATION (wave 44: 0/2035). Recording the flip; I did not trace which
  step-1 lane shipped it.

---

## LATEGAME SPECIMEN CANDIDATE
`wave45/lategame-specimen.txt` is already filled (deck146 vs deck125 seq 186, t54, 26,326 chars).
**Second candidate, offered because it exhibits a docket item rather than length:**
`1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` **seq 68**, t28, priority,
13,484 chars — the Sanguine Bond + 2× Exquisite Blood + Staff of Nin board above, rendered
entirely textless to the player who is about to lose to it.

---

## NEW -> wave-46 docket (ranked)

### HIGH

**NEW-7 (HIGH, render) — a lost card and a held card are the same sentence.**
The zone-change history line names a card with **no copy disambiguator**, while the hand line
**drops** its `(copy N of M in your hand)` tag once one copy remains. When a seat held 2 copies
and lost 1, the two surfaces read as a contradiction and the model must arbitrate which of its
own surfaces to disbelieve — the precise thing the trust doctrine forbids. Cost: the corpus's
longest decision, **539 s on a 2-option land drop**, 10,360 chars of post-plan overrun.
Repro: `1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` **seq 26** (the
reasoning), **seq 24** (the discard line), **seq 2** (both draws).
Fix shape (render, not prompt prose): either keep the copy tag on the hand line whenever a
same-named card left the hand this game, or disambiguate the zone-change line at emit time
("...discarded Pelakka Predation (1 of the 2 you held)"). Generalizes to every
discard/mill/exile of a duplicate.
Prediction for wave 46: prompts in which a bare-name loss line coexists with a same-named,
untagged hand entry: 0/N; and no reply trace containing the string "contradiction" about the
hand list.

**NEW-2 (HIGH, render) — the board's decisive permanents are four bare words.**
The wave-44 item "opponent non-creature permanents carry no text" is not a cosmetic gap: this
corpus contains an assembled Sanguine Bond + Exquisite Blood loop rendered as two names, and the
`LIFE-TO-DAMAGE CONVERTER` summary line — the ONE place the converter is explained — fires only
on the **attackers** window (5 prompts), which a control seat with no creatures never reaches.
Repro: `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` **seq 67–70**
(t26–t28, the whole loop textless on a priority/casting window).
Fix shape: give the opponent's non-creature permanents the same one-line `{card text: ...}`
snippet their own option rows already get, or at minimum hoist the converter/engine summary
lines out of the attackers window into the shared situation frame.
Prediction: opponent non-creature permanents rendered with no text on any decision surface: 0/N.

### MED

**NEW-1 (MED, engine) — `dropped_assignments` is not measuring what a lane gate needs, and the
CHOOSE_MENU ctx is missing on TargetChooser sub-asks.** Two separable halves at the same rung:
(a) 7/7 dropped assignments this wave were `Bn:none` for blocker indices that were never
rendered; the counter should split *illegal-pairing* from *phantom-index* before any lane gates
on it again. Repro: `1787768326-ai_baka_deck162-0x560dd1e6c400-vs-ai_baka_deck152.jsonl` seq 14.
(b) All 9 subject-less `A choice is required` headers are Teferi's +1 side-selection sub-ask
falling to `AIPlayerGPT.cpp:12743` because no card `ctx` is threaded; the same planeswalker's
loyalty menu, which has a ctx, renders its name 15 times. Repro:
`1787768338-ai_baka_deck152-0x55bef9967010-vs-ai_baka_deck126.jsonl` **seq 21** (and 24, 38, 41).
Prediction: subject-less headers 0/N; `Bn:none` counted as a dropped assignment 0/N.

**NEW-3 (MED, render) — ANNOUNCE_X is unannotated for every non-damage X spell, and X=0 is
never called out as a null cast.** 12 of 14 ANNOUNCE_X windows (all Sphinx's Revelation) render
bare `X = N` rows, and **3 of those 12 were answered X = 0** with 4, 5 and 6 affordable —
a Sphinx's Revelation cast for zero life and zero cards.
Repro: `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` **seq 58** (cap 5,
chose X=0) and **seq 64** (cap 6, chose X=0); `...deck125-0x562f799014b0-vs-deck123` **seq 30**
(cap 4, chose X=0).
Fix shape: a second priceable class for `life:X` / `draw:X` scripts (`X = 5 {X pricing: you gain
5 life and draw 5 cards}`), or — cheapest honest floor — annotate only the X=0 row
(`{X pricing: X=0 does nothing; the spell resolves for no effect}`). Do NOT remove the X=0
option: it is legal and occasionally correct (cycling-shaped plays, e.g. window 5's deliberate
Starstorm at X=0).
Prediction: ANNOUNCE_X rows with no annotation on a spell whose X has ANY quantified effect: 0/N;
X=0 chosen with a positive-effect X affordable: 0/N.

**NEW-4 (MED, prompt-gate) — the stale-plan note is now a nag on standing-strategy plans.**
0 false fires and 0 cast-free fires (lane D shipped cleanly), but **129 / 261 fires (49%) repeat
against an UNCHANGED plan text at the same seat**, and deck125's standing plan "Answer their
threats, resolve Staff of Nin, ping their face every turn" alone accounts for 136 fires
(deck125 eats 36.8% of its own prompts vs deck162's 3.4%). A plan that names a card the seat
does not yet hold is a strategy, not stale intent, and re-nudging it every decision spends
tokens and invites the model to rewrite a correct plan.
Repro: `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` seq 28, 30, 31 —
three consecutive fires, identical plan text, identical claim.
Fix shape (gate, not wording): suppress a repeat fire while the plan text is byte-identical to
the one already nudged, or require the plan's named card to be in a zone from which the seat
could act on it now. This is a REFINEMENT of a lane that passed — do not reopen #W45-4.
Prediction: repeat fires on unchanged plan text 0/N; overall fire rate stays < 15%.

**NEW-5 (MED, measurement) — two set-B metrics are bad proxies and should be restated before
wave 46 gates on them.** (a) "#W45-2 attacks on a collapsed tag that end with the attacker
dying" catches deaths the collapse did not cause — restate as *"attacks taken on a collapsed
tag whose gang price was rendered, that end with the attacker dying to a gang block"*.
(b) "#W45-3 `(neither dies)` bodies sent into a 2+-defender board" counts free attacks into
trigger-less walls as failures — restate as *"...into a board carrying a `@combat(blocking)`
life trigger"*. Evidence for both is in the set-B sections above.

**NEW-5b (MED, render) — `{X pricing: same as X=N}` points UP, so the cheaper equal X reads as
the derivative one.** The collapse row is correct and the wording follows the largest-first menu
order, but the fact the reader needs is that the SMALLER X is strictly cheaper for an identical
board. The one plan-vs-chosen divergence in the corpus is exactly this: the seat planned X=1,
read `X = 1 {X pricing: same as X=2}`, and cast at X=2.
Repro: `1787768342-ai_baka_deck130-0x55f269ac9490-vs-ai_baka_deck146.jsonl` **seq 18**
(plan at seq 17 says X=1; the cast at seq 19 announced X=2).
Fix shape: `{X pricing: same as X=2 - this X costs 1 less mana for the same result}` on any
collapsed row whose reference X is larger.
Prediction: casts at an X above a collapsed row that names an equal-result smaller X: 0/N.

**NEW-6 (MED, verification baseline for lane E) — 586 bare-subject history-line instances.**
This corpus's binary predates lane E, so this is the number wave 46 must drive down, not a
finding: **1,897 unique history lines / 8,386 instances**, of which **355 unique / 586 instances**
open on neither a player subject nor a possessive. Every one is either a damage line (lane E's
target) or `It became Day` / `It became Night` (10 + 4 instances, subject-less by nature and
correctly untagged). Mirror-ambiguous specimens present in this pool:
`Silverquill Silencer dealt 3 damage to Shield Sphere` (×10) and
`Nadaar, Selfless Paladin dealt 4 damage to Wall of Omens` (×4) — both names are run by two
decks in the pool.
Prediction (restating the carry's): unprefixed card-SUBJECT history lines 0/N from wave 46, with
`It became Day`/`Night` explicitly exempted from the count.

### LOW

**NEW-8 (LOW, lexicon) — `Transform:backside` and `teferieffect` reach live surfaces.**
`Transform:backside with Bloodline Keeper` appears in 141 prompts including **3 numbered OPTION
ROWS** the model must choose from; `teferieffect Counter Removed` appears on the STACK listing in
31 prompts. Raw card-script identifiers on a decision surface.
Repro (option rows, 3): `1787768330-ai_baka_deck123-0x559cc2a07250-vs-ai_baka_deck130.jsonl`
**seq 37**; `...deck123-0x56532ba655e0-vs-deck146` **seq 15**; `...deck123-0x562f7ac1d250-vs-deck125`
**seq 55**. Repro (stack): grep `ability: teferieffect`.
Fix shape: a display-name map at the ability-label emitter (`Transform Bloodline Keeper into
Lord of Lineage`; `Teferi's untap/tap effect`).

**NEW-9 (LOW, gate) — the gang price ignores the declaration-set minimum.**
`gangBlockPriceTag` is suppressed whenever `need < 2`, including when the attacker has **menace**
and a legal block therefore requires 2 bodies regardless. On
`1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` **seq 27**, A1 (Hive of the
Eye Tyrant, menace, 3/3) shows `Fate Unraveler #2 (3/4) (your attacker dies, their blocker
lives)` and no gang clause, because one blocker's power already reaches 3 — but one blocker
cannot legally block it. The cheapest LEGAL lethal block (Fate Unraveler + either Shield Sphere)
is never priced. Fix shape: floor `need` at `max(2, minBlockersRequired())`.
Prediction: menace A-lines whose eligible set contains a lethal legal gang and carry no gang
clause: 0/N.

**NEW-10 (LOW, serving) — one degenerate generation.** `unparsed_reply` at
`1787768352-ai_baka_deck123-0x56177edb6e40-vs-ai_baka_deck152.jsonl` **seq 61** produced
multilingual token soup after 154 s on an ordinary 6-row equip menu. One instance, no prompt
defect found. Pin it; a second occurrence makes it a serving/sampling item, not a prompt item.

---

## WHAT I DID NOT CHECK
- Set A (the 7 guide edits) — not this seat; no `deckN/` outputs were written, per the brief.
- Per-game causal traces of who won and why. Win rates are reported nowhere in this file, per
  the new-baseline rule.
- `gangOk`'s deathtouch/wither/infect/prevention suppressors — no candidate in this corpus had
  any of them, so that branch is UNTESTED, not passing.
- The `"(one target only)"` targeted-X phrasing — no targeted X spell reached ANNOUNCE_X.
- Whether wave-44's `#W44-4` Day/Night fix came from a step-1 lane or elsewhere; I verified the
  render, not the provenance.
- The Baka-side menace gang-block behaviour (the red pin / probe deck) — still owed, and this
  corpus's single menace blockers window was a GPT seat, so it says nothing about Baka.
- Scryfall cross-checks: the four card facts I relied on (Perimeter Captain, Pride Guardian,
  Wall of Omens, Shield Sphere) were verified against `bin/Res/sets/primitives/mtg.txt` only;
  no Scryfall fetch was made, so a primitive-vs-Oracle divergence on those four would not have
  been caught here.
