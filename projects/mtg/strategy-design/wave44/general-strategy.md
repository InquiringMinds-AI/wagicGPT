# Wave-44 general (core-prompt / render) proposals — layer-routed

Merged from the seven `deckN/general-suggestions.md` files and the four seat files.
Corpus `matchups-20260826-111937` (FIRST FAIR-HAND corpus), 2035 decisions / 21 games.
Each item: LAYER, seat evidence with repro, proposal, and a falsifiable wave-45 prediction.

Layer key:
- **RENDER** — the engine writes a new or changed fact into the prompt.
- **CORE** — the general prompt's standing text changes (no new facts).
- **GUIDE-only** — no prompt work; the fix belongs in a per-deck guide.
- **PASS/KEEP** — measured working, recorded so a later wave does not "fix" it.

Trust doctrine governs all of these: a guide may never hedge against a rendered fact, so
where a render is incomplete the repair is at the render, and a guide clause is at most a
stopgap that RETIRES when the render ships.

---

## RENDER / CORE — ranked

### R1 (HIGH, RENDER + CORE) — the ANNOUNCE_X rows carry no kill facts, and their ORDER is an open design question
**Evidence.** deck130 vs126 seq 44 (cast row) carries the full clause
`At X=5 - kills THEIRS: Pride Guardian x2, Wall of Omens x2, Vampire; YOURS: Rorix Bladewing`.
Two decisions later the ANNOUNCE_X menu (seq 46) prints only
`Every listed value is affordable; option 1 is the LARGEST X (X = 5).` and rows `1. X = 5 ... 6. X = 0`
— no kill facts at all. The pilot's plan at seq 45 said X=1; it answered `CHOICE: 1` and killed its
own Rorix Bladewing while two swings from winning; lost at t22. Second instance, deck130 vs162
seq 81/83 (X=5 to kill a 0/6 `[defender]` Shield Sphere, taking its own Rorix + 2 Goblins with it;
dead at t23). Corpus-wide 18 X-announce windows; 2 have a plan-stated X differing from the chosen X,
**both toward option 1** (the other is deck125 seq 83, plan X=4, chose X=11).
**ORCHESTRATOR ADJUDICATION (binding): the menu is NOT inverted.** The header states verbatim that
option 1 is the largest X and the seq-46 plan itself re-decides to X=5 — the surface was TRUE. The
defect is that the announce menu is the only decision in the game where the pilot picks a number
with no consequence preview, having seen one on the previous screen.
**Proposal (RENDER).** Annotate each `X = N` row with the X-pricing evaluator's output for that N:
`3. X = 3 {kills THEIRS: Wall of Omens x2; YOURS: none}`, with identical-outcome rows collapsed as
`(same as X=4)`. The `YOURS:` half is the half that loses games and must never be omitted.
**Proposal (CORE, separable design question — route to the owner if it is a behaviour change).**
Ordering. Today the menu is largest-first, and both observed divergences went to option 1. The
"usually-correct-first" convention argues for leading with the SMALLEST X that kills an opponent
creature, or for ordering ascending so option N = X = N where affordable. Recorded as a design
question, not a bug.
**Proposal (CORE wording, ships now either way, from deck130 G4).** Replace the fact with an
imperative: `The option number is NOT the value of X - find the row whose text reads "X = " followed
by the number you want, and reply with THAT row's number.`
**Wave-45 prediction.** With row annotations: plan-stated-X vs chosen-X divergence 0 of N
corpus-wide (was 2/18); Starstorm casts at an X whose `YOURS:` list names a non-defender creature the
seat needs as a clock, 0 (was 2/2). With the imperative wording only, the same metric measured at
deck130's windows alone.

### R2 (HIGH, CORE predicate) — the plan-mismatch note over-fires, and on land drops it contradicts the prompt three lines above it
**Evidence (four seats converge).** Overall fire rate 870/2035 = 42.8% (wave-43: 34%). The predicted
half PASSED: attackers 0/91, blockers 0/37, reveal 0/6, bottom 0/4, X-announce 0/14.
The residue: **land drops 327/419** corpus-wide — deck125 137/140, deck126 43/52, deck146+152+162
86/125, of which at least 35 have a plan that literally says "play a land"/"play lands".
Cross-tab against a 27,691-name primitive vocabulary: precision 705/870 = 81%, recall 705/974 = 72%.
**Five strictly-verified false fires** (an option row literally begins `Cast <the exact name the plan
says to cast>`): `deck146-0x5559288b224` seq 84 (Acererak), `deck126-0x5603808fbfc` seq 9,
`deck126-0x563b923035e` seq 39, `deck146-0x563b90fca49` seq 24 and seq 72; a sixth by inspection,
`deck125-0x5653db89c3b` seq 25 (plan "Clear the board with Supreme Verdict now", option 2 is
`Cast Supreme Verdict {1}{u}{w}{w}`). A further 52 of 193 fires at deck123's seat name a card that IS
in the menu under the engine's own verb (plan "Tap Bloodline Keeper", option `Create vampire with
Bloodline Keeper`); repro deck123 vs162 seq 48.
On a land drop the note asserts the list does not contain the plan's actions, three lines below the
prompt's own `its absence from the choices below does not mean it is gone`. It also contributed to the
679s decision (R10 below), which was a land drop.
**Proposal.** (a) Suppress the note on windows whose option set structurally excludes casts —
land drop, X-announce, mode/sub-ask menus. (b) Match plan CARD NAMES against menu card names before
firing, ignoring the verb. (c) Suppress when the matched plan clause begins "Next turn" / "Then" — a
plan is a multi-turn object and 141 of deck123's 193 fires are the note treating it as a this-menu one.
**Wave-45 prediction.** Fires on land-drop windows 0/~400; strict false fires 0; overall fire rate
below 25% with precision >= 95%; attackers/blockers stay 0.

### R3 (HIGH, RENDER) — the collapsed blocker summary prints only the favourable outcome
**Evidence (two seats).** deck162 vs123 `attackers` seq 13:
`[their untapped blockers: they have 6 untapped creatures that could block this one, biggest
Bloodline Keeper #1 (3/3) (you kill it, your attacker lives)]`. The model swung; three 2/2 Vampires
gang-blocked; the 5/5 Master of the Feast died; game lost at -21. Every word is true and the standing
footnote does say "before gang-blocks" — but the only OUTCOME shown is favourable, on a board where
the unfavourable one is trivially available. Related and adjacent: the same collapse form is what all
seven guides are NOT written against (they teach the enumerated form only — skill amendment 31).
**Proposal.** Price the collapse rather than the biggest single blocker: `they have 6 untapped
creatures that could block this one; any 2 of them together deal 4, enough to kill it`, or minimally
`biggest X (1-on-1: you kill it, your attacker lives) - but 6 blockers can gang this attacker`.
Record the collapse THRESHOLD somewhere reviewers can see it, since guide rules are written against
the enumerated form.
**Wave-45 prediction.** Attacks taken on a collapsed tag with N >= 2 that end with the attacker dying:
0 (was 1/1, and it was a game). deck162 attack rate rises above 2-of-6 windows without a new loss of
this shape.

### R3b (HIGH, RENDER — same field, block side) — menace attackers get a bare 1-on-1 verdict on blocker rows
**Evidence.** 3 blockers windows contained a menace attacker; **8 of 8 blocker-row entries offering
that attacker render a bare 1-on-1 trade with no menace caveat**, e.g.
`B2. Bloodline Keeper (5/5) [flying] - may block ... A4 (you kill it, your blocker lives)` where A4 is
`Hive of the Eye Tyrant (4/4) [menace]`. The attacker line above it DOES carry the caveat, so the same
prompt contradicts itself. The model declared `B2:A4` alone, the engine silently dropped it
(`dropped_assignments: 1`) and 4 damage went through. Worse on a Pride Guardian defender row, whose
`[deals 0 - this block kills nothing, but it STOPS all of the damage from whichever attacker it
blocks]` is flatly false for a solo block on a menace attacker.
Repro: `1787761186-ai_baka_deck123-0x5557c3baf5d0-vs-ai_baka_deck146.jsonl` seq 17; render half also
at `deck126-0x563b923035e` seq 47 and 57. deck123's seat independently recorded a lone 5/5 spent on a
menace attacker at vs146 seq 17.
**This discharges the "corpus-unreachable" status of the defender-side menace lane** — the probe deck
is still owed for the Baka block-side behaviour, but the RENDER half has live specimens.
**Proposal.** Block-side rows either suppress the 1-on-1 verdict for a menace attacker or prefix it
exactly as the attack side does.
**Wave-45 prediction.** Blocker rows offering a menace attacker with a bare 1-on-1 verdict: 0/N.
Solo blocks declared on a menace attacker: 0. `dropped_assignments` from menace: 0.

### R4 (HIGH, RENDER) — combat previews model DEATHS and never model LIFE
**Evidence (three seats, two decided games).** `Perimeter Captain (0/4) (neither dies)` is true and
lethally incomplete: primitives confirm `auto=@combat(blocking) source(creature[defender]|
mybattlefield):may life:2 controller` — 2 life for EVERY blocking defender they control, per Captain —
and `Pride Guardian` `auto=@combat(blocking) source(this):life:3 controller`. Sanguine Bond
(`@lifeof(player) ...: life:-thatmuch opponent`) converts that into damage on the attacker.
- deck146 vs126 attackers seq 37/44/51/56/65/73: opponent life 13 -> 19 -> 31 -> **42** on blocks whose
  every tag read "(neither dies)"; single turns gained 12 and 13. deck146 lost at T26 a game it led at T16.
- deck123 vs126 seq 40 and seq 59: 12 attackers into 2 Captains + 3 Walls + 2 Bonds; life 20 -> 8 -> 0.
The convention already exists one class down: `Vampire (1/1) (both die (lifelink: they gain 1))`, and
wave-43's E-3 lifelink omission is now FIXED and verified (`(both die (lifelink: you gain 1))`,
`deck126-0x563b923035e` seq 20 B2). This is the same annotation one step larger — the trigger is on a
third permanent, not a keyword on the blocker.
**Proposal.** Extend the existing lifelink parenthesis to any `@combat(blocking)` life trigger:
`Pride Guardian #1 (0/3) (neither dies (they gain 3))`, and where a `@lifeof(player)` converter is on
either battlefield, one summary line on the attackers prompt:
`[if they block with N defenders: they gain 2N (Captains), you lose that much (Bonds)]`.
**CORE companion (deck123 G1, ships regardless, trust-preserving).** The standing attackers paragraph's
exclusion list is "gang-blocks, pump or combat tricks". Add one clause:
`... - before gang-blocks, pump, combat tricks, or any life gained or lost by the block itself.`
This keeps the tag authoritative for what it measures and tells the pilot what it measures; it is NOT
a hedge against the render.
**Wave-45 prediction.** With the render fix: opponent life gained from BLOCK triggers in any single
deck146 game < 6 (was 12 and 13 in single turns); `(neither dies)` bodies sent into a 2+-defender
board 0 (was 6/6 turns all-in). With the CORE clause only, measured at deck123 and deck146 alike.

### R5 (MED, RENDER) — a bare `[defender]` tag with no gloss seeded 14 false beliefs
**Evidence.** The battlefield row renders `Fog Bank {1}{u} (0/2) [flying, defender]`; the option row for
the same card renders `Defender (This creature can't attack.)`; the blockers window renders a third,
fuller gloss. From the bare tag the model concluded **"Fog Bank cannot block"** and attacked into it on
that basis — 7 replies in one seat (`deck152-0x557785f52f40` seq 7, 8, 9, 10, 25, 26 + the seq-26 plan),
14 replies corpus-wide asserting `<X> cannot block` about a card that can block. Repro:
`1787761180-ai_baka_deck152-0x557785f52f40-vs-ai_baka_deck162.jsonl` seq 7.
The surface is honest but under-glossed, and the honest gloss already exists two renderers away.
Cheapest item in this docket.
**Proposal.** Gloss the battlefield-line tag the way the option row already does: `[defender - can
block, cannot attack]`.
**Wave-45 prediction.** Replies asserting `<X> cannot block` for a card that can block: <= 1 (from 14).

### R6 (MED, RENDER) — opponent non-creature permanents carry no text anywhere on the prompt
**Evidence.** The opponent battlefield line prints `Sanguine Bond {3}{b}{b} [enchantment]`,
`Staff of Nin {6} [artifact]`, `Howling Mine {2} [artifact]`, `Lightmine Field {2}{w}{w} [enchantment]`
— name and type only. Target rows DO carry `{target text:}` (lane #W44-3, 53/53 at deck123's seat,
96/103 at the 146/152/162 seat, 0 blind removal choices corpus-wide), so the channel exists and is
simply not applied to the board line. Every card in that list changed or ended a game at these seats
(two Staffs of Nin ended both deck123's and deck130's vs125 games from an empty board; Lightmine Field
made attacking impossible; the Bonds killed deck123). The pilot currently recalls them from training —
exactly the failure mode #W44-3 was filed to remove on the removal lane.
Repro: deck123 vs125 seq 50; deck123 vs126 seq 40.
**Proposal.** An `{effect:}` clause on opponent permanents whose primitive has an `auto=` line — at
minimum `@combat`, `@lifeof`, `@each ... upkeep` and damage triggers.
**Wave-45 prediction.** Opponent permanents with an `auto=` line and no text on the battlefield row: 0.
Secondary, behavioural: replies that mis-state a rendered opponent permanent's effect: 0 (spot-count).

### R7 (MED, RENDER) — the LETHAL block header states the goal but not the method; the attack side has no headline at all
**Evidence.** Block side: `Your life: 6. Unblocked, these attackers deal up to 11 - you would be at -5 -
LETHAL if it all connects (at 0 life you LOSE - 0 is not survival): block enough to survive` is 37/37
arithmetically correct (L-D==R, verdict polarity, the R==0 case). It still produced the losing answer at
deck152 vs146 seq 20, because nothing connects each attacker's own `deals N` to the surplus that must be
removed. Attack side: **0 of 91** attacker windows carry any lethal headline — the model hand-derives its
own swing in prose ("deal 12 damage to win"), on the side that wins games, while the prompt tells it
"Do not re-derive these outcomes; use them" for the block side. Repro:
`1787761180-ai_baka_deck152-0x557785f52f40-vs-ai_baka_deck162.jsonl` seq 26 (opponent at 19, A1 8/8 + A3 3/1).
**Proposal.** On LETHAL headers only, print the surplus and the sorted damages:
`You must stop at least 6 of the 11: A2 deals 7, A1 deals 2, A3 deals 2.` On attackers, print the mirror:
`Opponent life: L. If every attacker you send connects, they are at R - LETHAL / NOT lethal.`
Both are arithmetic the engine already has and both stay statements about the board, not advice.
**Wave-45 prediction.** LETHAL blockers windows where the chosen assignment leaves surviving damage below
printed life: 100% (was 0/1, and it was a loss). Attack windows: 91/91 carry the headline.

### R8 (MED, CORE) — the standing blockers paragraph never says the `may block` line IS the legality list
**Evidence.** deck130 vs146 seq 32 (life 3, LETHAL, one blocker whose `may block` line lists BOTH
attackers) replied that "Nadaar is unblockable by Blastminer", blocked the 2-damage Goblin, took 4 and
died at -1. Blocking Nadaar takes 2 and survives at 1. The standing text explains the parenthetical trade
results and says "Trust it over your own arithmetic; never re-derive it" — it does not say what the list
itself means.
**Proposal.** One sentence: `Every attacker named on a blocker's "may block" line is an attacker that
blocker CAN legally block; the parenthetical says how the fight ends, not whether it is legal.`
**Wave-45 prediction.** Replies asserting a legality claim contradicted by a rendered `may block` line: 0
(was 1, decisive).

### R9 (MED, RENDER) — trend teaches have no prompt-side support: no previous value of any counter
**Evidence.** deck130's rule #2 CROSS-CHECK asks the pilot to compare the opponent's life with what it was
"when you last looked". The prompt prints only the current scalar; the previous value exists only in the
cumulative game log, tens of lines up and interleaved. deck130 fired 9 face-damage spells in one game at
opponent life 25 / 22 / 18 / 20 / 17 / 19 / 24 / 27 / 24 and never noticed the reversals.
**Proposal.** One optional clause on the life line when it has moved the wrong way since this seat's
previous decision: `Opponent life: 24 (was 19 at your last decision, +5)`. The harness already has the delta.
**Alternative if out of scope:** retire trend-based teaches from every guide — they cannot be executed
against a scalar. (This is the only item in this file whose rejection has a mandatory GUIDE consequence.)
**Wave-45 prediction.** With the delta clause: deck130 face-damage casts above its 6-life floor drop to 0
(was 9/9) and at least one reply cites the delta. Without it: the guides carry no trend clause at all, and
the metric is that no guide sentence asks for a comparison the prompt cannot supply.

### R10 (MED, CORE + harness) — anonymous option rows generate the fallbacks; and the reply channel has no decode stop
**Evidence — fallbacks.** 6 fallbacks / 2035 = 0.29% (baseline 0.17-0.18%). **NOT a demonstrable
regression**: Poisson P(X>=6) ~ 0.16 at the base rate, and 3 of the 6 sit in one seat produced by one card
(Teferi, Who Slows the Sunset). Mechanism is unchanged and now has a precise trigger: **all 5 `stale_echo`
are on option rows carrying no card name** (`choose a land`, `Decline - do nothing`, `creature gains 3/3
and you draw`, three byte-identical `Vampire (2/2)` rows). The protocol demands `CHOICE: N (SHORT NAME)`;
with no name in the row the model back-fills from its PLAN line and the echo pass rejects it. Two further
`echo_index_conflict` recoveries have the same cause. Subclass rate **4 fallbacks / 57 mode-label asks =
7.0%** vs 0.29% overall. Repro:
`1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl` seq 52 / 53 / 62;
`deck146-0x563b90fca490-vs-deck126` seq 35 (a FALSE stale — the parenthetical names the ask's OWN source
card, `Silverquill Command`).
**Evidence — decode stop.** The corpus's single `retracted_choice`
(`deck162-0x5589480413b0` seq 37): a valid `CHOICE: 3 (Cast Howling Mine)` followed by ~2400 characters of
open deliberation ending "So the first choice is Underworld Dreams... CHOICE: 4" — the harness correctly
refused a contradictory commit. Same root, second symptom: the **679s** decision,
`1787761215-ai_baka_deck126-0x563b923035e0-vs-ai_baka_deck146.jsonl` seq 14 — an 11,372-char reply on a
three-option LAND DROP (prompt 7,232 chars, below the 8,512 median), re-deriving Sanguine Bond's cost five
times, then re-emitting the same CHOICE. 27 records have `post_plan_overrun > 0` (top 5065 / 3606 / 2667 /
2341); the five next-slowest decisions (485s/462s/414s/352s/277s) are all replies of 6k-26k chars. The
plan-note (R2) fired on that land-drop window, which is what set it re-litigating its plan.
**Proposals.** (a) ENGINE: `AIPlayerGPT.cpp` already strips a `pendingSource` prefix for the
`"<X> targeting <Y>"` family — add the symmetric case, so an echo matching the ask's OWN source card name is
read as a source prefix, not a stale echo. (b) RENDER: name the source on anonymous rows
(`choose a land for Teferi, Who Slows the Sunset`, `Vampire (2/2) [blocker #3 of 6]`), or accept a
numeric-only echo where the row has no proper name. (c) CORE: on asks whose options carry no card name,
swap the protocol example for one that copies the option's own words —
`e.g. "CHOICE: 2 (you draw and sacrifice creature)" - copy the option's own words`. (d) HARNESS: a decode
stop after the PLAN/CHOICE line converts both the 679s and the retraction into normal decisions.
**Wave-45 prediction.** `stale_echo` 0; fallback rate <= 0.15%; mode-label subclass rate back at baseline;
`post_plan_overrun > 0` records <= 3 (from 27); max latency < 120s; `retracted_choice` 0.

### R11 (MED, RENDER) — a sub-ask never states what the parent ability does
**Evidence.** `deck152 vs deck125` seq 52: "Choose an option for Teferi, Who Slows the Sunset: 1. choose a
land 2. Decline - do nothing" — nothing says that Teferi's +1 untaps one of yours or taps one of theirs.
The model answered about a different card entirely and the reply was discarded. Same at seq 53 and 62;
these are 3 of the corpus's 6 fallbacks. Compounding: the land rows in those menus carry no mana text (R12).
**Proposal.** Sub-asks generated by an ability carry that ability's rendered effect text in the header line,
the way cast options carry `{card text: "..."}`.
**Wave-45 prediction.** Ability sub-asks with no effect text in the header: 0. Fallbacks on Teferi sub-asks: 0.

### R12 (LOW-MED, RENDER) — land rows carry no mana text, and text-less duals render as bare `(land)`
**Evidence.** `Tundra [land] [opponent's battlefield]` and `Forest [land] [your library]` render with no
production text while every non-land target row carries its rules text (deck152 vs125 seq 64, 71, 85, 102).
In a land-destruction menu, `Scrubland (land)` sits beside
`Woodland Cemetery (land) {target text: "...{T}: Add {B} or {G}."}` — verified in the primitive
(`mtg.txt:100808`: `type=Land`, `subtype=Plains Swamp`, **no** `text=` line), so the renderer is honest and
the engine nonetheless knows the subtypes the model needs. 32 entries corpus-wide.
Repro: `1787761212-ai_baka_deck130-0x55df6ecbf9d0-vs-ai_baka_deck126.jsonl` seq 12.
**Proposal.** Synthesize a mana line from `subtype=` when `text=` is absent, in the shape the land-drop menu
already uses: `Tundra [land] (taps for {W} or {U}) [opponent's battlefield]`.
**Wave-45 prediction.** Land target rows with no colour information: 0/N.

### R13 (LOW, RENDER) — duplicate copies in a target clause carry no `{target text:}`, and the dedup is unmarked
**Evidence.** `Brutal Cathar #1 ... {target text: "..."}, Elite Spellbinder #1 ... {target text: "..."},
Brutal Cathar #2 {2}{w} (creature 2/2) [daybound], Elite Spellbinder #2 {2}{w} (creature 3/1) [flying]`
(deck125 vs152 seq 21); Perimeter Captain #1 with text, #2 and #3 without (deck125 vs126 seq 76). The
corpus-wide accounting confirms this is a deliberate dedup (313 same-name repeats) and not suppression —
but a deduped row is shape-identical to a genuinely textless card (149 tokens, 32 duals).
**No decision this corpus chose a duplicate copy**, so this is speculative.
**Proposal.** Print `{target text: as #1}` on later copies.
**Wave-45 prediction.** N/A behaviourally; the render metric is that 0 rows are ambiguous between "deduped"
and "textless".

### R14 (LOW, RENDER) — the X-announce window still shows the spell in hand with no ON THE STACK line
**Evidence.** 14/14 Sphinx's Revelation announce windows: the card is still on `Your hand:`, the hand count
is identical to the preceding casting window, and there is no `ON THE STACK` section. Repro:
`1787761192-ai_baka_deck125-0x555929beb550-vs-ai_baka_deck146.jsonl` seq 86 (hand 8, cast) vs seq 87
(hand 8, X menu). Arguably correct Wagic state — X is announced during casting — but any guide sizing X
against the hand is then off by one, and the seat had to ship a `SUBTRACT 1` workaround, which is a guide
hedging against the render.
**Proposal.** Either `Sphinx's Revelation {u}{u}{w}{x} [instant] [being cast now]` on the row, or a
one-line `ON THE STACK: your Sphinx's Revelation (X not yet chosen)` header in the announce window.
**Wave-45 prediction.** With either: the deck125 guide's `SUBTRACT 1` clause is retired and hand-cap
compliance does not fall (>= 12/14).

### R15 (LOW, RENDER) — singular loyalty counters carry no `(now N)`; history sources carry no owner prefix
**Evidence.** The split is not loss-vs-gain (as wave-43 recorded) but **plural vs singular**:
`lost 3 loyalty counters (now 1)` 4/4 OK, `got 4 loyalty counters (now 4)` 12/12 OK,
`lost a loyalty counter` **0/6**, `got a loyalty counter` **0/4**. 10 lines. Repro:
`deck146-0x563ae3950310` seq 20; `deck126-0x55df6d9a1f20` seq 23.
Separately, 402 history lines identify a source by bare card name with no `Your`/`Opponent's` prefix
(`- Luminarch Aspirant dealt 1 damage to the opponent (now 19)`); harmless where checked, genuinely
ambiguous in the mirrors this pool contains (both deck123 and deck146 run Nadaar and Hive of the Eye Tyrant).
**Proposal.** Route the singular path through the plural formatter. The owner-prefix item is a LOG-SHAPE
ruling (parity vs owner tags), not a patch — carried to the OWNER-CALL list.
**Wave-45 prediction.** `lost a loyalty counter` / `got a loyalty counter` lines without `(now N)`: 0/10.

---

## GUIDE-only (no prompt work; recorded here so the layer stays clean)

- **The X-menu stopgap clause in deck130's guide** is a guide guard for R1. When the announce rows carry
  kill facts it is RETIRED, not generalised — recording doubt of a render that is about to become
  trustworthy is a trust-doctrine violation (deck130 seat, "NOT PROPOSED").
- **Lifegain-wall attack gates** (deck123 EDIT A, deck146 146-A, deck152 152-E): guides must gate on the
  rendered battlefield line's 0/N defenders TODAY, because R4 has not shipped. These clauses stay even
  after R4 ships — R4 makes them checkable, it does not make them unnecessary.
- **Hive already-on check, Fog Bank connection test, Fateful Absence own-targets stop, Sanguine Bond as a
  standalone clock, Savannah's colours**: pure guide gaps against honest renders. Verified: the Hive
  animation state IS rendered (`(3/3) [menace]` + the creature count), so the 23 redundant animations are
  not a render defect.
- **`[doesn't untap during its controller's untap step]` on deck123's board is CORRECT** — deck123 runs
  Intruder Alarm; Master of the Feast's own primitive is `abilities=flying` + `@each my upkeep:draw:1
  opponent` with no such clause. Checked so it is never re-docketed.

---

## PASS / KEEP — measured working this corpus, do not "fix"

| surface | measurement |
|---|---|
| Day/Night state line | **PASS** — `It is currently Day.` / `It is currently Night.` in 236 records vs 229 records rendering a daybound/nightbound card; 13/13 and 31/31 and 10/10 at the three seats that had windows; 0 in games with no daybound card. (The 0/2035 FAIL first reported was a grep for the ledger's shorthand, not the emitter's string — skill amendment 44.) |
| removal `{target text:}` | **PASS** — 862 card-target entries, 368 explicit texts + 313 same-name dedups + 181 legitimately textless (149 tokens carrying inline keyword tags, 32 duals with no `text=`); **0 suppressed, 0 blind removal choices** corpus-wide. |
| reveal frames | **PASS 6/6** — `Your library: 28 cards (3 of them are the cards listed in the search/reveal below - they are still in your library until this decision resolves)`; opponent hand size a real number in all 6; whole-library searches correctly say "This is a search, not a look at the top of your library". The wave-43 E-2 *opponent-hand* shape is UNTESTED, not resolved. |
| lethal basis / forecast | **PASS 37/37** — L-D==R, L==`my_life`, correct verdict polarity including `at 0 life you LOSE - 0 is not survival`. |
| lifelink / deathtouch in trade verdicts | **PASS** — wave-43 E-3 fixed: `(both die (lifelink: you gain 1))`, mirrored `(they gain 1)`; 37 deathtouch/first-strike rows with correct paired verdicts. |
| plural loyalty `(now N)` | **PASS** — 4/4 and 12/12. |
| dungeon zone tags | **PASS** — `[your zone]` 0 occurrences (was 84); 13 unique dungeon lines clean; 16/16 dungeon-choice windows. |
| battlefield frame integrity + run collapse | **PASS** — 3839/3839 `permanents listed` counts correct once runs expand; 1173/1173 collapsed runs have span == the `xN` multiplier. |
| DFC / Flip Side labels | **PASS 123/123** (122 land wording + 1 honest hand-card wording). |
| observer parity | **PASS** — 0 defects across 21 games at `gameend` (life mirrored, turn, complementary win flags). |
| rules-text / mana-hint tails, `put a put`, token text leak | **PASS 0** each. |
| `Land drop:` phrasing | **KEEP verbatim** — both variants load-bearing; the "no land you could play right now" form is what let deck123 correctly answer "Play no land right now" in a mana-screw game (vs130 seq 7). Do not shorten. |
| variable-mana annotation | **KEEP verbatim as the pattern for all variable sources** — `Overgrown Battlement {g} (VARIABLE output: this ONE source adds more than one mana - its amount is a count, read its own rules text; the total above counts it as one source, not as its mana)`. It states the render's own limitation inside the render, which is what let a reviewer correct a guide instead of filing a bug. |
| "This is not your last chance to act this turn" | **KEEP** — deck123 passed in both main phases 25 times across six games with options on the menu; the paragraph is doing its job. Recorded so a future wave does not read pass-pass as a defect. |
| fair hands (lane A) | **PASS** — 41 keeps at 2x13 / 3x17 / 4x9 / 5x2 lands; 4 mulligans (2 from 0-land hands), each exactly once, 4 matching `bottom` records; no mull-to-zero, no repeated mulligan, no aggressivity-pin artifact. |
