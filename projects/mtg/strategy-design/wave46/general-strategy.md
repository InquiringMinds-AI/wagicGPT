# Wave-46 general strategy — render / core-prompt layer routing

Corpus `matchups-20260826-155842` (THIRD fair-hand corpus), binary master `486c83189`
(wave-46 lanes F/G/H/I + wave-45 lane E). **2113 decisions / 42 seat logs / 21 games, all
21 natural, 0 timeouts, 0 draws, 0 cap adjudications.** Fallbacks **6 / 2113 = 0.28%**
(w45 0.11%, w44 0.28%). Prompt cost **+17.4% chars/decision** (8,707 -> 10,222) bought
**-17.6% post-answer reasoning overrun** and **-17.2% p99 latency** — the trade the lanes
were shipped for, and it landed.

Sources: `wave46/seats/seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`,
`seat-engine-narration.md`, and the seven `deckN/general-suggestions.md`. Every render
verdict below is adjudicated against the emitter's literal string in
`src/AIPlayerGPT.cpp` / `src/DecisionContract.cpp` (skill #44), not against a lane
nickname; synthesis independently re-read `manaAvailableLine` (:2530), `describeZoneCards`
(:3009, call sites :8642 own / :8647 opponent), `boardEffectSnippetLen` (:355),
`converterSummaryText` (:8319), `gangBlockPriceTag` (:2299), `kRepeatActivationFloor`
(:8974) and the CHOOSE_MENU header ladder (:13109).

**Owner doctrine (2026-08-26).** The core loop's purpose is the game's development —
engine mechanical correctness and the LLM's UX: legality enforced structurally without
constraining choice, information and choice presented to maximise decision quality,
efficient use of inference. Guide and skill work is a cheap side quest. Everything in
this file is core-loop work; nothing here is guide prose, and no item asks the model to
work around a surface (trust doctrine).

---

## ALREADY SHIPPED SINCE THE CORPUS — recorded, NOT docketed

Four wave-46 findings are already fixed on master (`a7310f01c` + the pending hand-count
commit) and are verified present in the tree synthesis read. They are excluded from the
docket below and carry wave-47 predictions instead.

| shipped | what | verified at | wave-47 prediction |
|---|---|---|---|
| **S1** | CHOOSE_MENU header name ladder now `resolveOwningCardName(ctx)` (3 rungs), and the E-3 rescue is gated on the NAME being unresolvable rather than on a NULL pointer — Silverquill Command's `auto=choice` carrier now names itself | `AIPlayerGPT.cpp:13109`; `DecisionContract.cpp` ~:370 (`cardNamesItself(req.contextCard)`) | `A choice is required - choose an option:` **0/N** (was 4, all deck146, 2 of them `stale_echo`); subject-less `- You chose <label>` history lines **0/N** (was 2) |
| **S2** | The ask exemplar is drawn from THIS window's option 1 (`askExemplar(options)`) instead of the hard-coded `CHOICE: 3 (Cast Example Card)` — removing the only affirmative `Cast ...` substring on cast-free screens | `AIPlayerGPT.cpp:11207-11210` | replies whose `CHOICE:` label starts with `Cast ` into a menu with **no** cast option: **0/N** (was 4/1868, 2 of them fallbacks); `stale_echo` **0** |
| **S3** | `kRepeatActivationFloor = 1` — the FIRST repeat of an activation now carries `[repeat: activated this turn N time(s) already]` | `AIPlayerGPT.cpp:8974`, gate at `:9033` | no decision exceeds 300 s on a single-option priority window (was one 900 s `empty_reply` on a byte-identical re-offer); first repeat of any loop tagged **1/1**; deck146 redundant Hive activations stay at 0-1 |
| **S4** | In-game `Your hand (N cards):` count on the pilot's own hand line, matching `pregameHandHeaderText`'s idiom | `AIPlayerGPT.cpp:8603` (pre-game shape at `:1381`) | replies stating a wrong own-hand size: **0/N** (was 3 of the 4 that stated one, once "hand size 6" against an eleven-card list); deck125's X-sizing conformance re-measured against the printed count — its guide-side flat constant (W3) is due for sunset the moment this is confirmed |

Together S1+S2 are 4 of the corpus's 6 fallbacks. **Combined prediction: total fallback
rate returns to <= 0.15% at comparable N** (w46 0.28%, w45 0.11%).

---

## RENDER / CORE — ranked, with evidence and a wave-47 prediction each

### HIGH

**R1 — There is no DRAW-PUNISHER summary line, and its absence killed two seats on their
own turns.** The board frame already carries exactly this shape of warning for one hidden
cost class: `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond. While it
is in play, life ITS CONTROLLER gains also makes the other player lose that much life...`
(`converterSummaryText`, `:8319`). The mirror class — permanents that damage a player FOR
DRAWING — has no summary anywhere. Its facts exist only as `{effect: "Whenever an opponent
draws a card, ... deals 1 damage ..."}` scattered across three separate battlefield
entries, and the cost is not on the row that causes it: a `cycling` row prints its mana
cost and says nothing about the draw.
*Evidence, two seats, both fatal.* deck130 at **3 life**, opponent battlefield carrying
Underworld Dreams + Fate Unraveler + Ob Nixilis (all rendered with their trigger text),
its own plan one sentence earlier naming all three — answered `cycling with Starstorm`,
which draws one card for exactly 3 damage. Dead at 0 in the same turn. deck125 at **6
life** against three Howling Mines and two Underworld Dreams (a thirteen-line `dealt 1
damage to you` narration, 20 -> 5 inside a single draw step) cast Sphinx's Revelation for
X=1 and drew itself to 4. Both were STRATEGY failures over a complete render — which is
precisely the condition under which the loop's answer is a summary line, not guide prose.
*Repro:* `1787777944-ai_baka_deck130-0x5634cf21aa60-vs-ai_baka_deck162.jsonl` **seq 27**;
`1787777964-ai_baka_deck125-0x55a7d1167450-vs-ai_baka_deck162.jsonl` **seq 36**.
*Ask.* A `DRAW PUNISHERS on the battlefield:` line in CURRENT SITUATION, built on
`converterSummaryText`'s scan-and-name pattern, so it reaches every decision kind including
the priority windows where cycling rows appear. `auto=@drawfoeof(player):damage:N` is
scriptable and summable, so the number is claimable; where an amount is not exactly
computable, name the permanent without a number rather than guessing one. Second, cheaper
half: append the per-draw cost to the row that causes it —
`cycling with Starstorm [cost: {3}, Cycle - drawing costs you 3 life right now]`.
*Prediction.* Draws of any kind (cycling row, draw-mode, activated draw) at a window whose
opponent battlefield names one or more draw punishers and whose printed life is at or below
the summed per-draw cost: **0/N** (w46: 2 of 2, both lethal to the pilot).

**R2 — The `Choose an option for <card>:` modal menu is the only bare menu left in the
loop, and it has cost the same seat the same card two corpora running.** Rows render as
`1. Cast Card Normally` / `2. cycling` / `3. Decline - do nothing`: no cost on either row,
no effect on either row, no draw named on the cycling row — while the CAST row one screen
earlier carries a full `{X pricing: ...}` + `{card text: ...}` and the ANNOUNCE menu one
screen later carries a pricing annotation per option.
*Evidence.* deck130 arrived at the menu with the plan "Cast Starstorm at X=0 **to cycle
it** ... per Strategy Guide Rule #1" and answered `1. Cast Card Normally`, then announced
**X=0** — `auto=damage:X all(creature)` draws nothing; only `autohand=__CYCLING__({3})`
draws, and it had four mana for a {3} cycle. It stayed on four lands with two Rorix and two
Siege-Gang stranded and lost 19 to -23. Identical failure in wave 45 at the same seat. The
guide has since gained `CASTING IT AT X=0 IS NOT CYCLING`, a by-name description of the
menu, and `NEVER announce X=0`, and the pilot cited the rule **by number** while breaking
it. Per skill amendment 85 the guide lane is declared exhausted at the next identical
failure; this is the remedy named in writing.
*Repro:* `1787777928-ai_baka_deck130-0x563ec8c07390-vs-ai_baka_deck125.jsonl` **seq 60**
(cast row) -> **61** (the bare menu) -> **62** (the X menu it should never have reached);
wave-45 control at the same seat, vs123 seq 17.
*Ask.* Annotate both rows from what the engine already parsed: the alternative cost from
`autohand=__CYCLING__({3})` and the effect from `auto=`, e.g.
`1. Cast Card Normally {2}{r}{r}{x} {casting it: Starstorm deals X damage to each creature;
draws nothing}` / `2. cycling {3} {discard Starstorm and draw 1 card}`. Lead with the
outcome (this is a cost the pilot is about to pay, not a restriction). Subsumes the carried
**#W46-15** ("the modal row does not say which path disposes of the card").
*Prediction.* `Cast Card Normally` chosen at a `Choose an option for <card>:` menu whose
carried plan contains "cycle"/"cycling": **0/N** (w45 1/1, w46 1/1). Secondary: `X = 0`
announced at any window where a cycling row was on the previous menu: **0/N**.

**R3 — A converter of THEIRS doubles every price on the combat tags, and the multiplication
is left to the reader.** The converter paragraph names blocking triggers explicitly; the
A-lines then print the per-blocker price (`(you kill it, your attacker lives (blocking
trigger: they may gain 2))`). Both facts are true, present, in the same prompt — and two
seats lost games between them in one corpus.
*Evidence.* deck152 attacked into a Perimeter Captain / Pride Guardian / Wall of Omens
board with Sanguine Bond out, three combats running: **20 -> 11 -> 5** while the opponent
went **28 -> 37**, lost at -1 from a board it was ahead on (15 life spent on its own
attacks). deck162, same opponent, one turn: three 1/1 lifelink Vampires triple-blocked its
Fate Unraveler, they gained 3, Sanguine Bond took 3 off, Exquisite Blood closed the loop —
**19 -> 0 and 15 -> 31 in a single combat**.
*Repro:* `1787777968-ai_baka_deck152-0x558324388f00-vs-ai_baka_deck126.jsonl` seq **29 /
37 / 46**; `1787777935-ai_baka_deck162-...-vs-ai_baka_deck126.jsonl` seq **17** (and the
smaller seq **14**).
*Ask.* Fold the doubling into the tag that carries the number, exactly as #W46-9's lifelink
binding did (which worked — see PASS/KEEP): `(blocking trigger: they gain 2, and their
Sanguine Bond takes 2 off you)`. The standalone paragraph is a definition; the tag is where
the decision is made. Companion, from deck162's own evidence: one sentence in the attackers
footnote stating that a parenthetical naming a benefit to THEM is a PRICE on that outcome,
not part of it — the fight result is the first clause, the price is the second, and both
happen (the wave-46 binding did this for the fatal branch only; the friendly branches carry
the tail unframed).
*Prediction.* With a converter of theirs printed, a creature whose own A-line contains
`(blocking trigger:` or `(lifelink: they gain` is sent: **0/N** (w46: 5 sends across two
seats). Secondary, seat-level: pilot life lost between an attackers record and the next
record in a converter game: **0** (w46: 9, then 6, then 3).

**R4 — A land's activated ability is surfaced almost exclusively at UPKEEP priority, which
is the one phase every guide correctly tells the model not to use it in — and the model
cannot tell "wait for main phase" from "there is no main-phase offer".** This is an
offer-TIMING item, not a wording item: it is the engine deciding when a legal action is
presented.
*Evidence.* deck146's `becomes beholder` (Hive of the Eye Tyrant) was offered **31x in
Upkeep, 11x in Blockers, 4x in Main phase 1, 0x in Main phase 2**, against 17 Main-1 and 13
Main-2 priority windows in the same games. At turn 23 with the **opponent at 1 life**, the
Hive untapped and four sources up, animating is exactly lethal; the only offer that turn was
in Upkeep, the guide's (correct, and otherwise highly effective — 27-of-78 wasted
activations down to 1-of-46) absolute said PASS, and both windows passed. The opponent went
1 -> 6 -> 11 -> 17 -> 25 and won on turn 41.
*Repro:* `1787777936-ai_baka_deck146-...-vs-ai_baka_deck125.jsonl` `priority` seq **50** and
**51**.
*Ask, in preference order.* (a) Surface the manland activation at a main-phase priority
window when one will occur this turn; or (b) annotate the row with what the model cannot
otherwise know — `{this is the only priority window you will get this turn}` /
`{you will be offered this again in your main phase}`. The option row already carries state
annotations of exactly this shape (`{right now: returns NOTHING - your graveyard has no
creature cards}`, `[repeat: activated this turn N times already]`), so the convention exists
and (b) is cheap. Neither suppresses a legal option.
*Prediction.* Activated-ability offers reaching a main-phase priority window when the seat
has one that turn: **>= 90%** (w46: 4 of 46). Or, if (b) ships: every upkeep-only offer
carries the last-offer clause, **N/N**, and lethal-in-upkeep activations are taken **100%**
(w46: 0 of 2).

### MED

**R5 — The X-pricing row LEADS with the smallest kill and never labels the largest, so the
annotation reads as advice to underspend.** `{X pricing: max affordable X=3 (5 mana total);
smallest X that kills an OPPONENT creature: X=1 (3 mana total) - kills THEIRS: Human;
YOURS: none. At X=3 - kills THEIRS: Thraben Doomsayer, Human; YOURS: none}`. Both facts are
true; the ORDER and the LABELLING are the problem — the minimum is a named, labelled
quantity with a clause of its own and the maximum is an unlabelled trailing sentence. The
announce menu compounds it: the collapse wording `same kills as X=N, for K less mana` frames
MANA SAVED as the only axis between rows, with no marker for the row that kills most.
*Evidence.* deck130 answered X=1 where X=2 killed the token-maker as well at `YOURS: none`
and it controlled zero creatures, so the larger X was strictly free. The Doomsayer lived,
made twenty-one more tokens behind an Intruder Alarm, and the game ended 19 to -31.
*Repro:* `1787777926-ai_baka_deck130-0x5639f9b6b630-vs-ai_baka_deck123.jsonl` **seq 16**
(cast row) -> **seq 18** (announce, answered option 3 of 4).
*Ask.* Give the maximum a label in the same clause shape as the minimum — `best affordable
X: X=3 - kills THEIRS: ...; YOURS: none` — and mark the largest-kill row once on the announce
menu (`<- most kills at any affordable X`). Neither adds a claim; both promote an existing
fact to the salience of its opposite. Wave-45 lane C already established these are exactly
computable for a damage clause. Skill amendment 86 files the guide half in parallel.
*Prediction.* An announce answer whose `kills THEIRS` list is a strict subset of a larger
affordable X whose `YOURS:` reads `none`: **0/N** (w46: 1 of 4, and it decided the game).

**R6 — `{effect:}` truncates a planeswalker mid-loyalty-ability, dropping the ultimate.**
`boardEffectSnippetLen` (`:355`) is width-aware but not SHAPE-aware, and a planeswalker's
rules text is a list of loyalty abilities. Observed (count = windows rendered that way):
Sorin, Lord of Innistrad **x81** cut at `-2: You get an emblem with "Creatures you
control get...` — the emblem's effect and the entire **-6 ultimate** gone; Lolth, Spider
Queen **x83** cut inside `-3: Create...`, the -8 gone; Ob Nixilis **x18**; Teferi **x32**;
Kaya **x8**; Ranger Class **x34**; Lightning Greaves **x72**. It truncates VISIBLY (`...`),
so this is not a falsehood and not a doctrine breach — but the opponent's planeswalker
ultimate is the class of fact that decides late games and it is the half being dropped.
Budget context, measured: `{effect:}` costs ~70 chars/decision, ~4.6% of the wave's growth;
max distinct effect names on any line was **4**, so the 55-char tier was never reached and
the budget is not the cost driver (board width is).
*Repro:* `1787777938-ai_baka_deck126-0x5618ce439fc0-vs-ai_baka_deck123.jsonl` seq **14** on.
*Ask.* Split the text on ` -- ` into loyalty-ability clauses and budget PER CLAUSE, keeping
the last (the ultimate); or exempt planeswalkers from the width tiers.
*Prediction.* `{effect:}` clauses on a planeswalker whose `...` falls after a `+N:`/`-N:`
marker with no closing sentence: **0/N**.

**R7 — `{effect:}` is opponent-only, so the model's OWN combo pieces stay textless.**
`describeZoneCards(..., effectText)` is passed `true` at exactly one call site — the
opponent's `inPlay` (`:8647`) — while the own-battlefield call (`:8642`) takes the default
`false`. Defensible for creatures and for anything with an option row; not for the pieces
the pilot must reason about. deck126's own Sanguine Bond, Exquisite Blood and Staff of Nin
render bare on `Your battlefield:` in the same prompt where the opponent's Intruder Alarm
carries a full `{effect: ...}`, and the model was re-deriving its own combo from memory in
the PLAN field every window of that game. Note the whole R-chain of wave 45's #W46-3
(deck125 dying to an assembled Bond + 2x Blood) is a combo the OWNER of the pieces also has
to reason about.
*Repro:* `1787777938-ai_baka_deck126-0x5618ce439fc0-vs-ai_baka_deck123.jsonl` **seq 41**.
*Ask.* Extend `effectText` to own non-creature, non-land permanents that carry **no option
row this window** (a permanent whose ability is already offered as a choice needs no gloss).
*Prediction.* Own-side non-creature permanents with a `text=` line and no option row this
window rendering bare: **0/N**. Cost is unestimated (the filter's hit rate is not measurable
from the corpus) — measure prompt-char delta at ship.

**R8 — The blockers screen has no ranged collapse, and it is the one decision kind that got
both bigger and slower this wave.** The battlefield line five lines above collapses
identical bodies (`Vampire #1-#19 (4/4) ... x19`); the B-rows do not. deck123 vs130 seq 55
is **16,887 chars, of which 2,887 are 22 B-rows**, 21 of them differing only in a `#N`
handle and all reading the identical trade. Corpus-level movement (n=30 vs 33, low power,
NOT called a regression): blockers situation-tail mean 4,022 -> **4,431** chars (+10.2%),
p90 5,381 -> **6,567** (+22.0%); blockers latency mean 35.8 s -> **46.0 s** (+28.4%), p90
55.8 s -> **87.2 s** (+56.2%), max 257.7 s. Attackers moved the same way (25.4 -> 32.0 s).
*Repro:* `1787777926-ai_baka_deck123-0x5639f884b1d0-vs-ai_baka_deck130.jsonl` **seq 55**.
*Ask.* Apply the existing ranged collapse to consecutive B-rows identical in name, P/T,
keyword list and every per-attacker trade. The reply grammar accepts a single `B#:A#`, so
the collapse must still let the pilot name one member — whether the range itself is
nameable is the lane's design question, not a blocker on the saving.
*Prediction (pre-registered by the engine seat so it cannot be read post hoc).* Blockers
latency p90 **<= 60,000 ms** and blockers situation-tail mean **<= 4,600 chars**. N will
again be ~30: treat a single-corpus result as a signal to size a longer run, not as proof.

**R9 — The pre-game mulligan chain re-shows the pilot's own carried PLAN into an identical
repeated ask, and the plan answers it; and the `Mulligan` row is the one unpriced row in the
loop.** The chain is the only place where the SAME question is asked up to seven times with
no state change except a counter, and `YOUR PLAN (as you last stated it)` is re-emitted each
time carrying a sentence that is itself a mulligan instruction.
*Evidence.* deck123 wrote "Mulligan to find a hand with 4+ lands ..." at look 2 and
re-emitted that exact string as its reply at looks 3, 4, 5, 6 and 7 — **seven mulligans,
kept ZERO cards**, lost 42-0 on turn 20. Second game: **six mulligans, kept ONE card**, one
decision made all game, dead turn 13. Nine of those looks were at `(keeping 5)` or lower
holding at least one land; three held THREE lands and a coverage line naming castable
spells. 2 of 6 games. The price lives only in the header sentence ("having already taken 5
mulligans ... (keeping 2)"), never on the row being chosen — while casts carry
`{X pricing:}`, targets carry `{target text:}` and blockers carry the computed trade.
*Repro:* `1787777938-ai_baka_deck123-0x5618cf757b20-vs-ai_baka_deck126.jsonl` **seq 1-8**;
`1787777971-ai_baka_deck123-0x56503bcdf9e0-vs-ai_baka_deck162.jsonl` **seq 1-8**.
*Ask, both halves, neither suppressing a legal option nor deleting a fact.* (a) Price the
row: `2. Mulligan (a keep after this one would keep 4 cards)`, with the terminal case made
readable — `2. Mulligan (a keep after this one would keep 0 cards - your entire hand goes to
the bottom)`. (b) On pre-game mulligan asks specifically, either omit the carried-plan block
(no plan stated about a hand that no longer exists is evidence about the hand on screen) or
re-scope it truthfully: `PLAN YOU STATED ABOUT A HAND YOU HAVE ALREADY SHIPPED (it is not
about this hand): ...`. The second is the more faithful to the trust doctrine — nothing is
deleted, the scope is stated (a true statement in the wrong scope is a lie).
*Prediction.* Mulligan chains reaching a third look (`(keeping 5)`) with one or more lands
in hand: **0/N** (w46: 9 windows across 2 games). No seat reaches `(keeping 4)` (w46: 2, and
both lost, one 42-0 from a zero-card keep).

**R10 — A colourless mana source renders as `{1}` inside a list the prompt calls "colours",
and the SAME card renders `{x}` in one window and `{c}` in another in the same game.**
`colours you can make: {1}{g}{r} ... Talisman of Impulse {x} or {r} or {g}` (deck130 vs123
seq 8) against `colours you can make: {g}{r}{c} ... Talisman of Impulse {c} or {r} or {g}`
(same game, seq 14). `{1}` is a generic AMOUNT and `{x}` is a variable marker; neither is a
colour, and the pilot is being asked to read a colour set. 18 emissions at one seat pair.
Traced: the aggregate comes from `potential->toString()` (`ManaCost::toString`,
`ManaCost.cpp:1248` renders the `MTG_COLOR_ARTIFACT` slot as `{<count>}`), the per-source
string from `Constants::MTGColorChars`, whose index 0 (`MTG_COLOR_ARTIFACT`) is `'x'` while
index 6 (`MTG_COLOR_WASTE`) is `'c'` (`MTGDefinitions.cpp:7`). Talisman's `auto={T}:Add{1}`
lands in one slot or the other depending on path; **which path flips it is NOT determined**
and is the lane's job.
*Repro:* `1787777926-ai_baka_deck130-0x5639f9b6b630-vs-ai_baka_deck123.jsonl` seq **8** vs
seq **14**.
*Ask.* Render a colourless mana SOURCE as `{c}` in both places, and never a bare count
inside a colour set. General rule the mana frame should hold: one symbol per concept. This
surface has the longest history of misread arithmetic in the project; a vocabulary that
drifts between three spellings for one thing costs more than the character it saves.
*Prediction.* Non-colour tokens (`{1}`, `{x}`) inside a `colours you can make:` set or its
per-source sub-list: **0/N** (w46: 18 at one seat pair).

**R11 — An A-line with no `their untapped blockers` tag says nothing, and absence is the
least legible state a surface can be in.** Six of deck162's eight attack windows carried
A-lines with no blockers tag, and the render is honest in every case (the opponent's line
reads `(0 permanents listed)`, or their only creature is a flier the attacker flies past).
The model declined twice — at opponent life 13 and at opponent life 4 — reasoning "preserve
Fate Unraveler as a blocker" against a player with no creatures at all; deck126 threw away a
free lifelink point at an untagged A-line the same way. The model cannot distinguish
"nothing can block this" from "the tag was not computed", and the trust doctrine cuts the
same way: teaching a pilot to read meaning into a MISSING clause is teaching it to doubt the
render.
*Repro:* `1787777935-...deck162-...-vs-...deck123.jsonl` seq **15** and **19**;
`1787777938-ai_baka_deck126-...-vs-...deck123.jsonl` seq **18**.
*Ask.* Print the affirmative form — `[no creature they control can block this attacker]` —
alongside the existing menace and held-back tags, which are already written in exactly this
positive style. Subsumes the carried **#W46-18** (the `[held back, it CANNOT block ...]`
tag's missing converse).
*Prediction.* `ATTACK: none` chosen for an A-line carrying the new affirmative no-blocker
tag: **0/N** (w46: 3 across two seats). Attack rate on untagged A-lines back to **>= 90%**.

**R12 — The `STOPS all N damage` clause is unscoped about DURATION and is read as a standing
property of the wall.** `B1. Perimeter Captain (0/4) [defender] [deals 0 - this block kills
nothing, but it STOPS all 1 damage from reaching you] - may block A1 (your blocker dies,
attacker lives (blocking trigger: you may gain 2))` carries a QUANTITY and a VERDICT with no
precedence between them. deck126's only loss is two windows where the quantity beat the
verdict: a 0/4 Perimeter Captain traded itself for ONE point of stopped damage against a
1/1 first-strike deathtoucher, and a 0/3 Pride Guardian took the 3-damage attacker that
kills it over the 1-damage attacker it survives. The board was empty two turns later and the
seat took 10 and died on turn 10. The control is in the same corpus: the guide's carve-out
fired correctly on a Vampire B-line — which carries no STOPS tag.
*Repro:* `1787777959-ai_baka_deck126-0x55e4f7ca6610-vs-ai_baka_deck146.jsonl` seq **5** and
**8**.
*Ask.* Scope the existing quantity truthfully, four characters: `[deals 0 - this block kills
nothing, but it STOPS all 1 damage from reaching you THIS COMBAT]`. The render must NOT
carry the verdict ("do not block") — that is strategy and belongs in the guide (skill
amendment 70 ships that half). This asks only that the quantity stop overstating its reach.
*Prediction.* A wall assigned to a `your blocker dies` row while a `neither dies` /
`your blocker lives` row is offered on the same B-line: **0/N** (w46: 2 of 2, and they lost
the game).

**R13 — The land-drop binary is the loop's latency tail, and it is the one decision with
exactly one right answer.** The corpus's two slowest 2-option asks are both land drops:
**453 s** on `['Play Underground Sea', 'Hold Underground Sea - do not play it now']` and
**367 s** on the Brightclimb Pathway equivalent (`answer_replaced` fired on the second).
Both replies are full-turn strategic monologues in the PLAN field. p99 across all 755
two-option asks is 117.8 s, so the #W46-2 bar is met — the tail is one identifiable menu
shape. Separately, three seats declined land drops this corpus and every decline's reply
reasons the drop against the spell it intends to cast ("Cast Intrepid Adversary. Next turn
play land"), which the ask's own explanation ("The land drop is its OWN decision ... its
absence from the choices below does not mean it is gone") does not answer: it explains the
question's independence, never the thing the model gets wrong.
*Repro:* `1787777956-ai_baka_deck146-...-vs-ai_baka_deck130.jsonl` seq **27**.
*Ask, either or both.* (a) Suppress the PLAN request on land-drop asks — the reply rules
already make PLAN conditional, and a land drop cannot falsify a plan. Token/latency only;
removes no information. (b) State the consequence rather than the mechanism: `playing this
land does not reduce what you can cast this turn`.
*Prediction.* Max 2-option latency **< 200 s** (w46: 453 s); land-drop declines corpus-wide
**0/N** (w46: 9 across deck152 5, deck162 4 — deck146, which has a land rule, is 43/43).

**R14 — Two counts the header could carry that every control guide currently instructs the
pilot to derive.** The `counted by the engine` idiom now has three instances (pre-game hand
breakdown, pre-game mana sources, in-game per-colour source counts) and every one replaced a
place where a guide told a pilot to count and every one moved a measured rate. Stating the
design rule for the core loop: **anything a guide currently instructs the pilot to COUNT off
a rendered list is a candidate for an engine-side count, and the engine has the data at the
call site.** Two ranked candidates visible this corpus, both same shape and same call site:
(a) **opponent creatures able to ATTACK** — the header `Opponent battlefield (6 permanents
listed, of which 2 are creatures)` says 2 on a board of two walls, and deck125 spent a
Supreme Verdict on exactly that twice this wave; proposed `(of which 2 are creatures, 0 of
them able to attack)`, which would retire a whole guide branch in every control deck in the
pool. (b) **untapped sources remaining after the option row's spell is paid for** — every
tap-out rule in every guide is a subtraction the pilot performs, and deck125's has read 4/7,
3-4/7 and 6/11 across three waves; proposed `{leaves 3 untapped}` on the cast row, from the
affordability check the engine already runs.
*Explicitly NOT proposed:* a "your hand is full" style threshold WARNING. A boolean proxy is
what the guide then has to trust in place of a number, and proxies-for-numbers are the defect
class S4 just fixed. If only one thing lands, land the count.
*Prediction.* (a) Sweeper/removal casts into a board whose creatures are all `[defender]` or
printed power 0: **0/N** (w46: 2 of 10 at deck125). (b) deck125's Staff tap-out conformance
re-measured against the printed remainder; do not close the lane below n=10.

**R15 — Carried MED, re-armed with new evidence: non-damage ANNOUNCE_X has no preview, and
X=0 is never called out as a null cast (`#W46-4`).** Unchanged mechanism (`sv.priceable`
has nothing to price on `auto=life:X && draw:X`), but wave 46 adds the second half's
evidence: deck130 announced **X=0** on a damage spell it meant to cycle, at a menu where
the X=0 row is unmarked, and lost the game (R2's specimen). deck125 ran 17 Sphinx's
Revelation menus, all bare, all correctly ordered largest-first.
*Ask.* Unchanged: a preview for the non-damage X class, and a callout on the X=0 row when
X=0 makes the spell do nothing (`{X=0: this spell does nothing}`), which is a fact about
the card's own script, not advice.
*Prediction.* X=0 answered at a window where the seat could afford X>=1 and X=0 resolves to
no effect: **0/N** (w46: 1 of 4 at deck130, decisive; w45: 3 of 12 at deck125).

### LOW

| id | item | locus | repro | prediction |
|---|---|---|---|---|
| **R16** | `manaAvailableLine` drops the spelled-out number word above ten (`if (sources <= 10)`), so wide boards read `Mana available: 11 total (untapped sources,` — and the word exists precisely so "a digit cannot be misbound to a colour", per the function's own comment. 42 emissions at one seat pair; a 25-source specimen exists | `AIPlayerGPT.cpp:2530-2537` | `1787777948-ai_baka_deck123-0x55fba47b0300-vs-ai_baka_deck125.jsonl` seq **48**; `1787777970-...deck125-...-vs-...deck152` seq **170** | the count-word is present at every source count: **N/N** |
| **R17** | TOKEN permanents are textless: `Clue [artifact]` renders bare on both battlefields (23 opponent-board entries corpus-wide; 14 on one seat, 7 on another, 2 on a third) while every non-token artifact beside it carries `{effect:}`. A Clue reads "{2}, Sacrifice this artifact: Draw a card" — real text, and decision-relevant against a draw-punish or draw-matters seat. Tokens have no primitive `text=`; the fix reads the instance's GRANTED abilities (`token(Clue,...)` + `transforms((,newability[...]))`) | `describeZoneCards` effect gloss / `isEngineTokenText` path | `1787777970-ai_baka_deck125-0x557f248fbfe0-vs-ai_baka_deck152.jsonl` seq **32-38**; `152 vs162 attackers` seq **19** | tokens with granted abilities rendering bare: **0/N** |
| **R18** | Damage OBJECT still bare in history lines — lane E's declared residual, now counted: **128 corpus-wide** (`Human` 24, `Wall of Omens` 16, `Perimeter Captain` 14, `Pride Guardian` 12, `Goblin` 10, ...). Checked against both battlefield lines in the same prompt: **0 cases where the bare name was on BOTH battlefields at that moment**, so the mirrored-pool hazard is structural, not realised | history-line emitter (subject half shipped in lane E) | any seat; `- Your Staff of Nin dealt 1 damage to Human` | unprefixed damage OBJECTS: **0/N**. Close when a cheaper occasion arises — LOW, not MED, in this pool |
| **R19** | The mana-only auto-pass leaks 3 windows via the FLOATING-POOL half of the gate (`!getManaPool()->getConvertedCost()`), all after a declined CAST menu in the same phase, none showing an `Already in pool:` clause. 3 of 183 = 1.6%; all three were passed, so no play was lost, and the gate is behaving as its comment is written | `AIPlayerGPT.cpp:10945-10952` | `...deck126-...-vs-...deck123` seq **24**; `...deck126-...-vs-...deck125` seq **28**, **30** | all-mana windows reaching the model **<= 2%** of the mana-only class, **0** non-mana options lost |
| **R20** | The target list prints each creature's CURRENT size and nothing says so; the battlefield line has a separate convention for it (`(6/5) (printed 2/3)`). One clause in the target-list preamble makes a whole class of guide rules keyable to a rendered observable instead of a tally the executor cannot keep (skill 72(b)) | target-list preamble | deck152 counter placements, `Sigarda, Champion of Light (9/9) [trample, flying] [your battlefield]` | counter/buff placements keyed to a printed size in guides that adopt it; measured as a guide rate, not a render one |
| **R21** | The mana line already says `mana of ANY colour pays a generic cost like {2}`; a 350-second reply still reasons about whether it "has generic mana". Strengthening it to state that generic is never a separate resource you can lack would retire a recurring latency sink | `manaAvailableLine` tail | `...deck162-...-vs-...deck152` seq **10** | replies reasoning about possessing "generic mana": **0/N** |

### Carried unchanged from the wave-45 docket (no new evidence beyond re-observation)

`#W46-10` activated-ability STATE on the option row (`becomes beholder` is byte-identical
whether the Hive is a land or already a 3/3 — distinct from the repeat tag, which shipped);
`#W46-11` stale-plan note repeats on byte-identical plan text (49% of fires; **do not
reopen #W45-4**, which PASSED); `#W46-12` the plan-vs-AFFORDABILITY clause; `#W46-14`
`gangBlockPriceTag` ignores the declaration-set minimum (floor `need` at
`max(2, minBlockersRequired())` so a menace-mandated 2-body gang is priced); `#W46-16`
lexicon leaks on live rows (`Transform:backside` in 141 prompts incl. 3 numbered OPTION
ROWS; `ability: teferieffect Counter Removed` on the STACK in 31); `#W46-19` singular
loyalty `(now N)` and the bare `[defender]` gloss; `#W46-20` no LETHAL headline on the
ATTACK side.

---

## MEASUREMENT ITEMS (not render changes; restatements a wave-47 gate needs)

1. **Restate every rate the mana-only auto-pass touches on a like-for-like basis.** The
   wave-46 prediction "take-rate on surviving windows unchanged" was scored FAIL against a
   RAW priority take-rate that rose 47.8% -> 62.3% — which is the denominator effect the
   gate exists to produce (180 guaranteed-`pass` windows removed, all at deck126). Restate
   as: priority take-rate EXCLUDING the mana-only class is within +/-5 pt of wave 45's
   same-basis figure. Same discipline for deck126's `Add N green` rate (1/16 vs 2/39 are
   different populations). Skill amendment 87.
2. **`mana_only_windows_skipped` stays in the `gameend` record permanently**, not just for
   the validating wave — it is the only way a reviewer can tell a rate change from a
   denominator change, and this wave it was exactly that difference.
3. **Count, corpus-wide, replies whose PLAN line is byte-identical to the shown plan**, split
   by whether the reply protocol's own condition ("no plan shown yet, or part of yours is now
   done or false") was met. This is `#W46-11` from the other side and the deck123 mulligan
   chain from a third. No wording change is proposed until the split exists; if most
   identical re-emissions are protocol violations the clause is not being read, and if most
   are genuine falsifications restated unchanged that is a different fix.
4. **Blockers/attackers width and latency, pre-registered** (R8): p90 <= 60,000 ms and tail
   mean <= 4,600 chars, N ~30, single-corpus result sizes a longer run rather than proving.
5. **A summed worst-case attackers line is an OPEN QUESTION, not a spec.** deck123 offers
   `IF EVERY ONE OF THEIR UNTAPPED CREATURES BLOCKS: they gain up to N life from blocking
   triggers and lifelink, and the converter above turns that into N off your life` on one
   window of evidence. The per-block gains are already computed and printed on the tags; the
   SUM is claimable only if "every one blocks" is stated as the BOUND rather than as a
   forecast. If it cannot be claimed exactly it must not be claimed at all — the tags are
   true and R3's per-tag binding is the cheaper answer. Recorded so it is neither lost nor
   built on one window.

---

## PASS / KEEP — surfaces measured working this wave, do NOT "fix" them

| surface | measurement |
|---|---|
| **#W46-1 per-colour source counts** | **1708/1708 exact**, recounted from each window's own per-source sub-list, paren-aware: 0 colour-count mismatches, 0 source-total mismatches, and the stated convention holds on the specimens that test it (a five-colour dual counted under all five; `Overgrown Battlement {g} (VARIABLE output...)` counted as ONE source). Behaviour: coloured-pip overcommits **0** at every seat — deck126's wave-45 game-losing overcommit did not recur in 211 decisions, and every combo-piece cast was at `{B} 3` or better |
| **#W46-2 copies-left-behind** | Clause on **5,904 lines / 1,154 windows**. Correctness: 107 hand-zone clauses cross-checked against the same record's hand list, 96 exact, all 11 apparent mismatches explained as snapshot lag inside the same `events` block, **0 falsehoods**. Behaviour: replies containing contradiction-arbitration language **5 -> 0** (the word does not appear in any reply in the corpus). p99 on 2-option asks **117.8 s** (bar 120 s; w45 141.3 s) |
| **#W46-3 `{effect:}` on opponent permanents** | **1,320 name-groups covered, 0 uncovered**; clause fidelity **21/21** exact prefixes of the primitives' `text=`, no invented text. Every "textless" instance is a numbered LATER copy under the deliberate `{effect (each copy of this card does this): ...}` collapse — correct by design. Wave-45's MED item (Staff of Nin, Sanguine Bond, Exquisite Blood, Intruder Alarm, Talisman all textless) is **CLOSED**. Residual is tokens only (R17) |
| **#W46-3 converter scope** | **10/10 seat-logs** with Sanguine Bond on a battlefield carried the converter line on a non-attackers window (**147 non-attackers windows** vs 5 total in w45). The one apparent gap has Exquisite Blood, not Sanguine Bond — loss->gain, correctly outside `lifeToDamageConverterScript`'s class. **Not a defect** |
| **#W46-5 GANG BLOCK blockers mirror** | Render **PASS**: the one blockers emission is arithmetically correct (1+3 = 4 >= toughness 4). Behaviour **PASS 0/30**: no reply re-derives a group's power by hand (w45: 1 of 1 wide window, ~250 words). Silent cases audited against the emitter's own gate rather than by eye — every silence is `need = 1` or a `gangOk` exclusion (first strike). **0/13** all-`your blocker dies` windows had an unpriced lethal group |
| **#W46-9 lifelink tail binding** | **82 tails, 4 exact shapes, 0 loose tails anywhere in any prompt**; all on `A<n>.`/`B<n>.` trade lines, 56 attackers / 26 blockers. Scoping correct: unbound `(lifelink: you/they gain N)` sits only under `both die` / `you kill it` / `your blocker dies` heads. Behaviour **0 replies add keyword benefit to face damage** (w45 had the "gain 3 life each AND deal 6 damage" reply), and on one seat the bound row was the row that was HELD |
| **#W46-7 mana-only auto-pass** | Counter present in **all 42 gameend records**; **180 windows skipped**, ~85% of the one affected seat's priority churn. "No skipped window had a non-mana option" proven two ways — by the emitter (`isManaOnlyAction` is type-exact and the gate breaks on the first non-matching option) and by reconstruction (**13/13** windows where a Sorin loyalty or a Staff ping coexisted with the Battlement survived and were asked; **0 lost non-mana options**). Residual is R19 |
| **#W46-8 X collapse wording** | **21/21** ANNOUNCE_X windows carry `{X pricing: same kills as X=N, for M less mana}`; the bare wave-45 `same as X=` shape appears **0 times**. Behaviour: of the 4 windows offering a collapsed cheaper row the model took the cheaper row **4 of 4** — the wave-45 failure did not recur. (The 2-of-4 FAIL one seat reported is a DIFFERENT metric — answering the top of a collapsed equal-kill RUN — and is routed to R5, not to this lane) |
| **Lane E (#W45-20) owner-prefixed history** | **586 unprefixed lines -> 4**, and **unprefixed card-SUBJECT lines 586 -> 0**, over a full replay of 9,302 history lines. The 4 survivors are `- It became Day`, a CR 730 designation with no card subject — correctly untagged |
| **#W46-6 side-ask subject** | Teferi's side chooser — all 9 of wave 45's bare headers — is **gone**; `Choose an option for Teferi, Who Slows the Sunset:` renders 10 times. The 4 residual headers were one different emitter, now **SHIPPED** (S1) |
| **`dropped_assignments`** | **0 on every record corpus-wide**, including a reply reading `B1:A2, B2:none`. The wave-45 `Bn:none` padding artefact did not recur; **#W46-17 is DISCHARGED**, no split needed |
| **Fair hands (3rd corpus)** | 42 openers, engine land counts 0-7, 8 mulligan chains, correct running "having already taken N mulligans ... (keeping M)" text at every step including a full seven-mulligan London chain, and a CR 103.5 `bottom` ask correctly reading "Your ENTIRE hand goes to the bottom" at M=0. 8 `bottom` records, 7 options each, 8-42 s, **0 fallbacks** |
| **New render FALSEHOODS this wave** | **ZERO.** 1708/1708 mana counts, 107/107 copies clauses, 21/21 effect clauses vs primitives, 82/82 lifelink tails on their branch, 10/10 converter games, 21/21 X-pricing rows, 0 unprefixed card-subject lines. Every open item is an OMISSION, a SCOPE, or an ORDERING — not a lie |

**Explicitly asked not to be changed:** the `(VARIABLE output:)` annotation, the land-drop
ask's own explanation of its independence (R13 asks to ADD a consequence clause, not to
replace it), the general combat hint, and the `[defender]` legality gloss's truthfulness
(its brevity is `#W46-19`, its content is correct). Three seats' "misleading surface"
candidates — the collapsed tag, the `same as X=N` row, the `STOPS all N` clause — are all
TRUE strings; each is routed as a render item with a prediction (R12, R5) and NEVER as a
guide teach to doubt them.
