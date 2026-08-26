# Wave-45 general-strategy: render / core-prompt proposals (layer-routed)

Sources: `seats/seat-engine-narration.md` (NEW-1..NEW-10), `seats/seat-125-126.md` (HIGH #1,
MED/LOW list), `seats/seat-123-130.md` (M1-M3, L4-L5), `seats/seat-146-152-162.md` (E-1..E-5),
and the seven `deckN/general-suggestions.md`. Engine DEFECTS with repros are ranked in
`engine-ledger.md`; this file is the layer-routed ARGUMENT for each, with the evidence and one
falsifiable wave-46 prediction per item. Nothing here asks the prompt to carry a fact a guide
should carry, and nothing here asks a guide to work around a surface being fixed (trust
doctrine).

Every emitter claim below was read out of `projects/mtg/src/AIPlayerGPT.cpp` at the working
tree, not from a lane nickname (#44).

---

## R1 (HIGH, render) — `Mana available:` prints a colour SET and never a per-colour SOURCE COUNT, while the pregame render already prints one

**Verified against the emitters, as the carry required.** `manaAvailableLine()`
(`AIPlayerGPT.cpp:2415`) emits `N total (<word> untapped sources, tapped automatically when you
cast; colours you can make: {g}{u}{r}{b}{w}; mana of ANY colour pays a generic cost like {2})`
plus `Those sources, one per untapped card: ...`. `pregameHandHeaderText()`
(`AIPlayerGPT.cpp:1318-1345`) already emits the exact count shape —
`Mana sources among those lands, counted by the engine: {W} 2, {U} 2` — and it does so *only
before the game starts*. In-game the pilot must derive a per-colour count by parsing the
sub-list, one card at a time, through `(VARIABLE output: ...)` clauses.

The seat's claim is confirmed and its ranking stands. Note the pregame function's own comment
block (`AIPlayerGPT.cpp:1300-1317`, N-139n) is the argument for this item written in advance:
per-item tags assert MEMBERSHIP, and membership is exactly what the pilot argues with — "only an
engine-computed AGGREGATE closes it". The in-game line is currently the membership form
(`colours you can make: {g}{b}{w}`) with no aggregate, which is the pre-N-139n state of the
pregame surface.

**Evidence (game-deciding).** `1787768348-ai_baka_deck126-0x55d603332df0-vs-ai_baka_deck130.jsonl`
seq 30-34, turn 14, life 6. `Mana available: 4 total (... colours you can make: {g}{u}{r}{b}{w}
...)` over a sub-list holding exactly ONE {b}-capable card (`Scrubland {w} or {b}; Overgrown
Battlement #1 {g} (VARIABLE output ...)` x3), against `Cast Sanguine Bond {3}{B}{B}`. The seat
took `Add 5 green mana with Overgrown Battlement` twice, floated ten green, could not cast, and
passed; the two tapped Battlements then could not block and life went 6 -> 1. Primitives verified:
Overgrown Battlement `auto={T}:foreach(creature[defender]|myBattlefield) add{G}` — {G} only;
Scrubland `subtype=Plains Swamp`; Sanguine Bond `mana={3}{B}{B}`.

**Proposed shape** (data is already at the call site — the sub-list is built from
`manaSources[i].colors` at `AIPlayerGPT.cpp:8222-8236`):
`colours you can make: {g}{b}{w} (sources that can make each: {G} 3, {B} 1, {W} 2)`. Count-first,
in the style `manaAvailableLine`'s own N-158g comment argues for. Two counting conventions must
be decided at ship time and stated in the render, or the number becomes a new false surface: a
dual counts toward BOTH its colours (so the counts do not sum to the total), and a
variable-output source counts as ONE source for its colour, exactly as the total already counts
it. Ship the convention as words in the clause, not as an assumption.

**Wave-46 prediction.** Plans or announced casts naming a coloured cost whose pips exceed the
per-colour count on the same window's mana line: 0/N (wave 45: 1, and it lost a game — deck126
D21). Secondary: `Add N green mana with Overgrown Battlement` taken in a window where no spell is
cast in the same decision: 0 (wave 45: 2/39).

---

## R2 (HIGH, render) — a lost card and a held card are the same sentence: the copy tag drops at exactly the moment it is needed

**Verified.** `copyOfTag()` (`AIPlayerGPT.cpp:1515-1522`) returns the empty string whenever
`total < 2`. So a hand entry loses its `(copy N of M in your hand)` tag the moment one of two
copies leaves the hand — while the zone-change history line names the card with no copy
disambiguator at all. The two surfaces are individually honest and jointly unresolvable.

**Evidence.** `1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` seq 26: the
corpus's longest decision, **539.0 s on a TWO-option land drop** (`Play Swamp` / `Hold Swamp`;
corpus median 16.5 s), reply 11,084 chars of which 10,360 is post-plan overrun. The seat drew two
Pelakka Predations (seq 2) and discarded one (seq 24); the history says `- You discarded Pelakka
Predation` while `Your hand:` still lists `Pelakka Predation {2}{b} [sorcery]`. The trace,
verbatim: *"This is a contradiction. Usually, the current hand list is the ground truth... I will
trust the hand list."* The answer on line 1 was correct; the cost was entirely the arbitration.

This is the trust doctrine's own failure mode, not a guide gap: the render made the model doubt
the render. No guide prose can be written against it (that would teach doubt, which the doctrine
forbids), so it can only be fixed at the emitter.

**Proposed shape.** Either keep the copy tag on the hand line whenever a same-named card left the
hand this game, or disambiguate at emit time on the zone-change line ("discarded Pelakka
Predation (1 of the 2 you held)"). Generalises to every discard, mill, exile and sacrifice of a
duplicate.

**Wave-46 prediction.** Prompts where a bare-name loss line coexists with a same-named, untagged
hand entry: 0/N. Secondary: no reply trace containing "contradiction" about the hand list; the
p99 decision latency on 2-option asks falls below 120 s.

---

## R3 (HIGH, render) — the opponent's decisive permanents are four bare words, and the one line that explains them fires only on a window a control deck never reaches (#W45-6, carried and now decisive)

Carried from the wave-44 MED docket, re-affirmed with a specimen that decided a game and is
worse than anything wave 44 held. `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl`
seq 67-70 (t26-t28) renders `Exquisite Blood #1 {4}{b} [enchantment]; ...; Exquisite Blood #2
{4}{b} [enchantment]; ...; Staff of Nin {6} [artifact] [tapped]; Sanguine Bond {3}{b}{b}
[enchantment]` — an ASSEMBLED Sanguine Bond + Exquisite Blood loop plus a repeating damage
engine, given to the reader as four names, four costs and four type words. Primitives: Sanguine
Bond `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`; Exquisite Blood
`auto=@lifelostfoeof(player):life:thatmuch controller` — together an unbounded loop. deck125
died on turn 29 from 30 life; deck152 went 22 -> 0 in one turn off a single Staff ping to the same
board.

The channel exists and is SELECTIVE, which is what makes this a gap rather than a design: the
same corpus renders `Talisman of Impulse ... [tapping for mana deals 1 damage to its controller]`
on the seat's OWN battlefield line and `[defender, doesn't untap during its controller's untap
step]` on opponent walls. The gloss layer is per-property; triggered abilities of opponent
enchantments and artifacts simply have no glosser. Target rows meanwhile carry `{target text:}`
and `{card text:}` freely.

Second half, same item: the `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond`
summary — the one place the mechanism is explained — rendered 5 times this corpus and **only on
attackers windows**. A creatureless control seat never reaches one. It explained the board to the
deck that was winning and never to the deck that was dying.

**Proposed shape.** Give opponent non-creature permanents the same one-line `{card text: ...}`
snippet their own option rows already get; at minimum, hoist the converter/engine summary lines
out of the attackers window into the shared CURRENT SITUATION frame.

**Wave-46 prediction.** Opponent non-creature permanents rendered with no text on any decision
surface: 0/N. Secondary: the converter line appears on at least one non-attackers window in every
game where a converter is on either battlefield (wave 45: 0 of 3 such games at the seat that
lost to it).

---

## R4 (MED, render) — the ANNOUNCE_X menu has no preview for any non-damage X spell, and X=0 is never called out as a null cast

**Not a regression, and the lane C scope claim is confirmed** — `xAnnounceRowKills` is gated on
`sv.priceable` and a damage evaluator has nothing to price on `auto=life:X && draw:X`. But 12 of
the corpus's 14 ANNOUNCE_X windows are Sphinx's Revelation and render bare `1. X = 2` / `2. X = 1`
/ `3. X = 0`, and **3 of those 12 were answered X = 0** with 4, 5 and 6 affordable — a Sphinx's
Revelation cast for zero life and zero cards. The other 21 X windows in the corpus (deck146's
Agadeem's Awakening) are the same class one screen earlier: the CAST row carries `{X pricing: max
affordable X=N (M mana total)}` and the announce menu would carry nothing.

Repro: `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` seq 58 (cap 5, chose
X=0) and seq 64 (cap 6, chose X=0); `...deck125-0x562f799014b0-vs-ai_baka_deck123.jsonl` seq 30
(cap 4, chose X=0). Control with pricing:
`1787768330-ai_baka_deck130-0x559cc3d27570-vs-ai_baka_deck123.jsonl` seq 21.

**Proposed shape.** A second priceable class for `life:X` / `draw:X` scripts — `X = 5 {X pricing:
you gain 5 life and draw 5 cards - your library: N cards}` (the library count matters; a wave-40
game was lost to a Revelation off an empty library) — or, as the cheapest honest floor, annotate
only the X=0 row: `{X pricing: X=0 does nothing; the spell resolves for no effect}`. Do NOT remove
the X=0 option: it is legal and occasionally correct.

**Wave-46 prediction.** ANNOUNCE_X rows with no annotation on a spell whose X has any quantified
effect: 0/N (wave 45: 36 of 42 rows). X=0 chosen with a positive-effect X affordable: 0/N (wave
45: 3 of 12).

---

## R5 (MED, render) — the BLOCKERS screen prices only 1-on-1, on boards where the seat must gang-block to survive

The wave-45 lane-A `GANG BLOCK:` clause is the attackers-side answer to exactly this question and
it works (15/15 priceable A-lines priced, 0 silently skipped). Its mirror does not exist. At
`1787768352-ai_baka_deck123-0x56177edb6e40-vs-ai_baka_deck152.jsonl` seq 47 (t14, life 13) one
attacker — Luminarch Aspirant (7/7) deals 7 — faced thirteen blocker rows, every one reading
`(your blocker dies, attacker lives)`, with no group price anywhere on the screen. The pilot wrote
~250 words deriving "I need exactly 7 blockers to kill it", assigned all thirteen, and the engine
took seven. It got there, and the core prompt's own doctrine ("Do not re-derive these outcomes;
use them") was violated by the screen not carrying the outcome.

**Proposed shape** (the clause already exists on the other side): for each attacker, the cheapest
group of the pilot's listed blockers that kills it —
`GANG BLOCK: any 7 of your listed blockers together deal 7, enough to kill this attacker; each
result above is a LONE blocker only`.

**Wave-46 prediction.** Blockers windows whose only listed outcomes are `(your blocker dies,
attacker lives)` while a legal group of the listed blockers kills the attacker, carrying no group
price: 0/N (wave 45: 1 of 1). Secondary: blockers replies containing hand-derived group
arithmetic ("I need N blockers"): 0/N.

---

## R6 (MED, engine) — the CHOOSE_MENU subject is missing on TargetChooser SIDE sub-asks (E-3 residual, `AIPlayerGPT.cpp:12743`)

The wave-45 E-3 fix is a clean PASS on the class it targeted: all 57 `Choose an option for
<spell>:` and all 5 `Choose one mode for <spell>:` headers name their spell, and there are 0
`stale_echo` fallbacks in the corpus. All **9** surviving `A choice is required - choose an
option:` headers are one path: **Teferi, Who Slows the Sunset's +1 side-selection sub-ask** —
6 x `1. choose your land / 2. choose opponent land`, 3 x `1. choose your creature / 2. choose
opponent creature` — all on deck152 (`...deck152-0x55bef9967010-vs-deck126` seq 21, 24, 38, 41;
`...deck152-0x55b6b7adf0d0-vs-deck125` seq 24, 42, 55, 76, 87).

Build path, read not inferred: the CHOOSE_MENU handler (`AIPlayerGPT.cpp:12716-12743`) recovers
`ctxName` from `ctx->getDisplayName()`, then `ctx->model->data->getName()`, then the ETB-land
recovery, and falls to the generic string at 12743 when all three are empty. Teferi's +1 spawns
its side chooser with no card `ctx`; the SAME planeswalker's loyalty menu, which has one, renders
`Choose an option for Teferi, Who Slows the Sunset:` 15 times in the same corpus.

**All 9 answered cleanly, 0 fallbacks** — this is now a legibility item, not a parse risk: the
model cannot tell from the sub-ask which permanent it is resolving or what the choice does.

**Proposed shape.** Thread the parent's source name and effect summary into the sub-ask header,
the way the E-3 fix carried it into the modal root.

**Wave-46 prediction.** Subject-less `A choice is required` headers: 0/N (wave 45: 9). No new
fallback class appears on the sub-ask path (wave 45: 0).

---

## R7 (MED, core prompt / engine) — #W44-9 mana-only priority windows: the wave-44 "game composition" verdict does not survive a second corpus

deck126: **39 of 188 decisions (20.7%)** and **39 of 46 priority windows (85%)** offered nothing
but `Add N green mana with Overgrown Battlement`, spread across THREE games, not one —
`...deck126-...-vs-deck125` seq 12,16,19,20,22,24,26,27,30,32,33,35,36,38 (14);
`...vs-deck162` seq 5,8,9,10,14,15,16,19 (8);
`...vs-deck130` seq 8,13,16,17,19,20,21,25,26,27,28,32,33,34,36,37,38 (17).
37 were passed; the 2 taken are the R1 losing incident. Wave 43 measured 21.6%, wave 44 4.1% in
one game, wave 45 20.7% across three: the rate tracks whether an Overgrown Battlement is on the
board, which is most games this deck plays. Each of those windows is a full prompt assembly and a
model round trip to answer "pass".

**Constraint on the fix, from the same seat's evidence:** it must NOT auto-pass when a pending
cost exists — the guide legitimately keeps "cast Tribute in this same window" — so the gate is
"no legal action in this window other than mana production, and no cost is pending", not "the
only options are mana".

**Wave-46 prediction.** Priority windows whose entire option set is mana production with no
pending cost, reaching the model: 0/N (wave 45: ~37 at one seat). Secondary: deck126's decision
count per game falls without any change in its take-rate on the windows that do reach it.

---

## R8 (MED, render) — `{X pricing: same as X=N}` points UP, so the cheaper equal X reads as the derivative one

The collapse row is correct and follows the largest-first menu order, but the fact the reader
needs is that the SMALLER X is strictly cheaper for an identical board. The corpus's ONE
plan-vs-chosen-X divergence is exactly this: at
`1787768342-ai_baka_deck130-0x55f269ac9490-vs-ai_baka_deck146.jsonl` seq 17->18 the plan said
"Use Starstorm at X=1 to kill the Goblin and Triumphant Adventurer", the menu read
`1. X = 2 {kills THEIRS: Goblin, Triumphant Adventurer; YOURS: none}` / `2. X = 1 {same as X=2}`,
and the answer was option 1. Both X values kill the same two creatures and `YOURS:` is `none` at
both, so it cost one mana and nothing else — the opposite of wave 44's two divergences, which
cost the seat its clock. It is still a violation of the guide's "pick the smallest X" and it is
the residual this lane should be measured on.

**Proposed shape.** `{X pricing: same as X=2 - this X costs 1 less mana for the same result}` on
any collapsed row whose reference X is larger.

**Wave-46 prediction.** Casts at an X above a collapsed row naming an equal-result smaller X:
0/N (wave 45: 1 of 4 windows that named an X).

---

## R9 (MED, render) — a benefit inside a fatal parenthesis needs its branch named

`(your attacker dies, their blocker lives (lifelink: you gain 3))` produced, at
`...deck152-...-vs-deck162` seq 28, verbatim: "The Intrepid Adversaries gain 3 life each via
lifelink (total +6 life) **and deal 6 damage to the opponent** (reducing them to 4)." Both cannot
happen. Every other clause in that parenthesis family describes the blocked outcome, so the tail
is internally consistent — but on a line whose head says the attacker dies, a bare benefit reads
as an addition. 3 records, 1 dead lord, and one of the three doors through which the corpus's
0-for-7 stop was lost (skill amendment 55 carries the guide half; this is the render half).

**Proposed shape.** Bind the number to its branch: `(your attacker dies, their blocker lives
(lifelink: you gain 3 from the block, and this attacker deals nothing to their life))`.

**Wave-46 prediction.** Replies that add a parenthesised keyword benefit to face damage on the
same attacker: 0/N (wave 45: 1 of 3 records carrying the tail, and it was decisive).

---

## R10 (MED, render) — put activated-ability state on the option row

`becomes beholder with Hive of the Eye Tyrant [cost: {3}{b}]` is byte-identical whether the Hive
is a land or already a 3/3 Beholder. 78 offers, 27 taken, **10 taken while the battlefield line
already read `Hive of the Eye Tyrant (3/3) [menace]`** and the header said "of which 1 is a
creature" (repro: `...deck146-...-vs-deck125` seq 98 and 100, turn 30, same Hive, same phase), 18
taken in UPKEEP against a guide that says pass in upkeep, and 6 of the 27 a SECOND activation
inside one upkeep priority chain (seq 87+88, 124+125, 133+134, 147+148, 154+155, 160+161). Two
Silverquill Commands sat uncast in hand throughout. The render is honest — the animated Hive
prints (3/3) and is counted — but the state is on the board line and not where the decision is
made. This is the item skill amendment 65 routes here: the wave-44 guide edit moved the counts
12 -> 10 and 19 -> 18, which is the signal to stop rewording.

**Proposed shape** (the engine's own convention, applied to an activated-ability row):
`{already a 3/3 Beholder until end of turn - paying again adds nothing}`, beside the existing
`{target text:}`, `{right now: returns NOTHING}`, `{X pricing:}` family.

**Wave-46 prediction.** `becomes beholder` taken while the battlefield line already reads
`(3/3)`: 0/N (wave 45: 10). Second activations inside one priority chain: 0/N (wave 45: 6). Note
which of the guide edit and the annotation is present when adjudicating.

---

## R11 (MED, prompt gate) — the stale-plan note is now a nag on standing-strategy plans (a REFINEMENT of a lane that passed; do not reopen #W45-4)

Lane D shipped clean and every leg passes: 261/1909 = 13.7% fire rate (bar was <25%), **land-drop
fires 0/374** (was 327/419), **0 false fires on all 261** tested against the note's literal claim,
precision 45/45 on a seeded hand-read sample, and every `castFreeAskHeader` family at zero. The
residual is usefulness, not truth: **129 of 261 fires (49%) repeat against an UNCHANGED plan text
at the same seat**, and deck125's guide-mandated standing plan ("Answer their threats, resolve
Staff of Nin, ping their face every turn") alone accounts for 136 fires — deck125 eats 36.8% of
its own prompts, deck162 3.4%. A plan naming a card the seat does not yet hold is a strategy, not
stale intent, and re-nudging it every decision spends tokens and invites the model to rewrite a
correct plan. Repro: `...deck125-...-vs-deck126` seq 28, 30, 31 — three consecutive fires,
identical plan text, identical claim.

**Proposed shape** (gate, not wording): suppress a repeat fire while the plan text is
byte-identical to the one already nudged, or require the plan's named card to be in a zone the
seat could act from now. A second, cheaper cut named by two seats: suppress when the plan's named
actions are all of a different decision KIND than the current ask (~14 of 49 audited fires are
"plan names a combat action, menu is a cast menu").

**Wave-46 prediction.** Repeat fires on byte-identical plan text: 0/N (wave 45: 129/261). Overall
fire rate stays under 15% and false fires stay at 0.

---

## R12 (MED, prompt) — the plan-vs-affordability contradiction the note cannot catch

The plan-note fires when the plan names an action the menu does NOT contain. The failure it
cannot see is a plan naming an action the menu DOES contain and the seat cannot afford: "Cast
Intruder Alarm {2}{u} using Tundra and Underground Sea" survived thirteen consecutive windows at
`Mana available: 2 total` (repro: `...deck123-...-vs-deck126` seq 12-24), and deck130 carried
"next turn, untap to 8 mana" for four turns on four Mountains (`...deck130-...-vs-deck123` seq
27). Both are strictly-decidable arithmetic contradictions between the plan text and a number
printed two lines above it.

**Proposed shape**, same channel and same conservatism as the existing note:
`(note: your plan names Intruder Alarm {2}{u}, which costs 3 mana; this window shows 2.)` — fire
only on an exact card-name match against the plan text with a computed mana value strictly above
the printed total. Never on a judgment.

**Wave-46 prediction.** PLAN lines naming a spell whose mana value exceeds the window's printed
`Mana available: N total`, surviving into the next window unchanged: 0/N (wave 45: 13
consecutive at one seat, in the game it lost 42-0). Note: skill amendment 47 ships the guide half
this wave, so wave 46 measures the guide alone unless this lands too — adjudicate which is
present.

---

## R13 (LOW, render) — name the cost of the land-drop decline, once, on the row

The `Land drop:` ask is already the best-explained question in the prompt, and declining is legal
and occasionally right (a Forgotten Cave held for a cycling turn). What it never says is what
`Play no land right now` costs, and it is the only option in the game strictly dominated whenever
another option exists. One clause on the option row —
`4. Play no land right now  [you have 3 lands in hand and 2 on the battlefield; skipping this
leaves your mana total unchanged until your next turn]` — would have ended a thirteen-window run;
both numbers are already computed elsewhere in the same prompt. It must state the cost, never
forbid the choice.

**Wave-46 prediction.** `Play no land right now` answered while a `Play <land>` line is listed:
0/N corpus-wide (wave 45: 13 at deck123, 14 turn-level at deck125, 2 at deck126). Same
adjudication caveat as R12 — amendment 46 ships the guide half this wave.

---

## R14 (LOW, render) — the gang price ignores the declaration-set minimum

`gangBlockPriceTag` is suppressed whenever `need < 2`, including when the attacker has MENACE and
a legal block therefore requires two bodies regardless. At
`1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` seq 27, A1 (Hive of the Eye
Tyrant, menace, 3/3) shows `Fate Unraveler #2 (3/4) (your attacker dies, their blocker lives)` and
no gang clause, because one blocker's power already reaches 3 — but one blocker cannot legally
block it, so the cheapest LEGAL lethal block (Fate Unraveler + either Shield Sphere) is never
priced. Fix shape: floor `need` at `max(2, minBlockersRequired())`.

**Wave-46 prediction.** Menace A-lines whose eligible set contains a lethal legal gang and carry
no gang clause: 0/N.

---

## R15 (LOW, render) — the modal row does not say which path disposes of the card

`Choose an option for Starstorm:` lists `1. Cast Card Normally` / `2. cycling ... [cost: {3},
Cycle]` / `3. Decline - do nothing`. The cycling row carries its cost and its card text; `Cast
Card Normally` carries neither an outcome nor a pointer to the X menu that follows it — and the
seat took row 1 believing it spent the card the same way, then announced X=0 (repro:
`...deck130-...-vs-deck123` seq 20). One trailing clause: `1. Cast Card Normally [you will be
asked for X next]`. Same class as the ANNOUNCE_X preview: a menu step whose consequence is fully
known and not shown. Skill amendment 57 ships the guide half.

**Wave-46 prediction.** `Cast Card Normally` answered on a cycling-capable modal menu at a board
where the guide's own release names cycling: 0/N (wave 45: 1 of 1).

---

## R16 (LOW, render) — `Transform:backside` and `teferieffect` reach live decision surfaces

`Transform:backside with Bloodline Keeper` appears in 141 prompts — 80 in the seat's own history
voice, 58 opponent-voice, and **3 on a numbered OPTION ROW the model must choose from** (repro:
`...deck123-...-vs-deck130` seq 37; `...deck123-...-vs-deck146` seq 15;
`...deck123-...-vs-deck125` seq 55). `ability: teferieffect Counter Removed [triggered/activated
ability]` appears on the STACK listing in 31 prompts. Raw card-script identifiers on a decision
surface are exactly the silent-omission shape the trust doctrine warns about from the other side:
the model confabulates a meaning for a token it cannot parse. Fix shape: a display-name map at the
ability-label emitter (`Transform Bloodline Keeper into Lord of Lineage`; `Teferi's untap/tap
effect`).

**Wave-46 prediction.** Card-script identifiers (`Transform:backside`, `teferieffect`, any
`lowercase:lowercase` token) on any option row or stack line: 0/N.

---

## R17 (LOW, metric) — `dropped_assignments` conflates `Bn:none` padding

The corpus's ONLY non-zero value is 7, all from one record:
`1787768326-ai_baka_deck162-0x560dd1e6c400-vs-ai_baka_deck152.jsonl` seq 14, a window that offered
exactly ONE blocker, answered `B1:A2, B2:none, B3:none, B4:none, B5:none, B6:none, B7:none,
B8:none`. Nothing was dropped; seven no-ops were counted. The true corpus count is 0, so the
metric currently has zero signal about illegal blocks and will mask the first real menace-caused
drop. Split *illegal-pairing* from *phantom-index* before any lane gates on it again.

**Wave-46 prediction.** `Bn:none` counted as a dropped assignment: 0/N; corpus
`dropped_assignments` reads 0 unless a genuinely illegal pairing occurs.

---

## R18 (LOW, render) — the "held back" tag is one-sided

`[held back, it CANNOT block ANY of their 1 creatures: Master of the Feast (flying)]` tells the
seat what keeping a body home does NOT buy. There is no converse for the board where the answer
is the whole game: at `...deck130-...-vs-deck146` seq 21/25 the attack list was Rorix alone, the
opponent's only creature a 4/4 with vigilance, and nothing on the attackers screen said Rorix was
also the only blocker or that the blockers screen's trade for that pair is "you kill it, your
blocker lives". Symmetric with the existing tag and computable from the same data: on an attack
row for the seat's ONLY untapped creature, append `[if you keep it home it can block: Nadaar (4/4)
(you kill it, your blocker lives)]`. Skill amendment 56 ships the guide half (the race gate), so
this is the cheaper pool-wide form, not a duplicate.

**Wave-46 prediction.** Sole-attacker windows where the seat sends its only untapped body into a
race it loses by rendered arithmetic: 0/N (wave 45: 2 of 2, and it was the loss).

---

## MEASUREMENT ITEMS (no code, but they gate wave-46 verdicts)

- **M-a. Two set-B metrics are bad proxies and are restated before wave 46 gates on them.**
  (a) "#W45-2 attacks on a collapsed tag that end with the attacker dying" catches deaths the
  collapse did not cause — the one death (`...deck146-...-vs-deck123` seq 23) was a collapsed tag
  whose single shown outcome was already `(your attacker dies, their blocker lives)`, truthfully
  predicting the death, with no gang involved. Restate as *"attacks taken on a collapsed tag
  whose gang price was rendered, that end with the attacker dying to a gang block"* (wave 45:
  0/2). (b) "#W45-3 `(neither dies)` bodies sent into a 2+-defender board" counts free attacks
  into trigger-less walls as failures — all 6 of the corpus's 6 sends were into Shield Spheres
  (`counter(0/-1) all(this)`, no life) or Walls of Omens (`auto=draw:1 controller`, no trigger),
  both primitive-verified. Restate as *"...into a board carrying a `@combat(blocking)` life
  trigger"* (wave 45: 0).
- **M-b. Lane E baseline.** This corpus predates lane E (#W45-20), so its 355 unique / 586
  instance bare-subject history lines are the number wave 46 drives down, not a finding.
  `It became Day` / `It became Night` (14 instances) are subject-less by nature and are exempted
  from the count. Mirror-ambiguous specimens already present: `Silverquill Silencer dealt 3 damage
  to Shield Sphere` (x10), `Nadaar, Selfless Paladin dealt 4 damage to Wall of Omens` (x4) — both
  names run by two decks in the pool, which is the OBJECT-side residual the carry flagged.

---

## PASS / KEEP — surfaces measured working this corpus; do not "fix" these

| surface | measurement |
|---|---|
| menace block side (`menaceBlockPairingTag`, `zeroPowerBlockerTag` menace branch, `blockCountRequirementTag`) | bare 1-on-1 verdicts on menace B-rows 0/3; `stops NOTHING there` 2/2 eligible; solo menace blocks 0; menace-caused dropped assignments 0. One window (`...deck162-...-vs-deck146` seq 19) — a render PASS, not a rate |
| menace attack side | 16 menace A-lines across 13 windows, all carrying the restriction tag; 13 correctly carry no blockers tag; no over-claim |
| `GANG BLOCK:` pricing (attackers) | 15/15 arithmetically priceable A-lines priced, 0 silently skipped, 0 clauses the arithmetic did not support; 2 of 4 collapsed lines priced and 2 correctly silent |
| block-trigger tags (`combatTradePreviewStats`) | renders in all three voices incl. the scripted `may` (`(blocking trigger: they may gain 2)`); TargetChooser leg's FIRST live observation, correctly attributing a third permanent's trigger; block-trigger life in any deck146 game max 3 (wave 44: 12-13 in single turns) |
| `LIFE-TO-DAMAGE CONVERTER` line | 5 prompts, correctly scoped (absent while the Bond was in hand at seq 35, present at seq 42 after it resolved) — see R3 for its window-scope gap |
| plan-mismatch note (lane D) | every leg PASS; see R11 for the usefulness refinement |
| X pricing on the priceable class | 6/6 rows on the 2 priceable windows carry a full or collapsed `{X pricing:}`; casts at an X whose `YOURS:` names the seat's clock 0/2 (wave 44: 2/2) |
| E-3 modal root | 57 `Choose an option for <spell>:` + 5 `Choose one mode for <spell>:`, 0 subject-less, 0 stale_echo |
| `{target text:}` on removal/counter rows | 58/58 counter rows and every Path row — third consecutive wave at 100% |
| fair hands | 42 openers, land counts 0-6, 8 mulligans, real London chains with correct running "having already taken N mulligans" text, real `bottom` asks citing CR 103.5, no aggressivity-pin artifact |
| Day/Night | 75 prompts carry `It is currently Day.` / `It is currently Night.` — the wave-44 FAIL was a shorthand grep (#44), now recorded PASS on the emitter's own string |
| `(VARIABLE output: ...)` annotation | present on every Overgrown Battlement mana line; it is what let the R1 diagnosis separate a colour problem from a count problem |
| land-drop ask explanation, the not-your-last-chance paragraph, the general combat hint | no reply quoted the hint against a guide rule in 22 blocker windows; wave-41 amendment #1 concern stays discharged |
