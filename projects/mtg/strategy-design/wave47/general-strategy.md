# Wave-47 general strategy — RENDER / CORE-PROMPT proposals, layer-routed

Corpus `matchups-20260826-182155`, binary master 5b1bf9668, 5,726 decisions across 42 seat
logs, 20/21 games natural (the deck123-vs-deck162 token-loop game DEAD-LOOPED with no gameend
and is excluded from every rate). Sources: the four seat files, seven `deckN/general-suggestions.md`,
seven `deckN/edit-texts.md`.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs —
is the purpose. Everything below is a change to what the ENGINE RENDERS or to the CORE PROMPT's
own text. Guide-side answers are named only where a rule must stay correct AFTER the render
ships. Engine BUGS and their call sites live in `engine-ledger.md`; this file is the ranked
routing view with the evidence and the wave-48 falsifier for each item. `R<n>` here and
`E<n>` in the ledger are the same item where cross-referenced.

**Governing constraint on every item below.** Legality is enforced structurally; the model's
CHOICE is never constrained. Every proposal ADDS a true token or a true row and DELETES none.
Where a fix could be written as a cap, a filter or a suppression, it is written instead as an
affordance — see R1's explicit rejection of a hard offer cap.

---

## HIGH

### R1 — an unbounded legal loop has no stopping affordance, and the `[repeat:]` tag is a counter, not a brake. (= ledger **E4**, the wave-48 HEADLINER)
Three games this corpus ran one activated ability in a closed loop (Thraben Doomsayer or Lord
of Lineage + Intruder Alarm: token enters -> Alarm untaps all -> the maker is untapped again).
`deck123 vs deck162` ran **1,868 activations in one turn** and never terminated — the
orchestrator killed it after ~9 h, 1,871 round trips, **48.2 M prompt characters** and 5.7 h of
measured latency, with **no gameend record**: an invariant-00 failure. `deck123 vs deck126` ran
**1,540** and `deck123 vs deck146` **214**; both finished only because the loop ran on the
pilot's OWN turn and the attack step consumed it.
The window shape, verbatim at seq 1921: ONE option row, `[repeat: activated this turn 1868 times
already; you control 1897 creatures]`, then `You will have priority again later this turn` — a
sentence that in a loop is a standing invitation — and pass existing **only inside the reply-
format sentence** as `0 = pass priority`. **3,818 of 4,128 priority windows in this corpus were
single-option, and in every one the only way to decline was to invent a number not in the list.**
The house rule "declines go LAST" is not honoured on any of them.
Three affordances, ranked, none of which removes a legal option:
**F1 (preferred) — an engine-level repeat-N row.** `2. Create human with Thraben Doomsayer,
repeated N times, then stop [you name N on the CHOICE line; the engine performs it N times,
re-checking the cost each iteration, and returns priority to you here]`. This is MTG 720/721's
announced shortcut expressed as an option. It ADDS a choice, is legality-safe, collapses 1,871
round trips into one, and is the only option that also fixes the two games where the loop is the
whole win condition.
**F3 — render the decline as a real LAST row** (`0. Pass priority (take no action this window)`)
on single-option priority windows. Cheapest in the set and independently correct.
**F2 — the tag states the mechanism**: `[repeat: activated this turn N times already; you
control M creatures. This turn will not advance while you keep taking this option; nothing else
on your board has changed for the last K activations.]` Restriction-first, no "you should stop".
**Explicitly REJECTED: a hard offer cap.** A cap bounds the loop but constrains the choice on
exactly the two boards where the loop is the deck's win condition, and it would have to fire at
an N nobody can justify. If a defensive bound is wanted, bound it at the ENGINE TICK level as a
per-turn activation budget that CONVERTS to an F1-style "the engine will perform the remaining N
and pass" offer — never by deleting the row.
Two structural facts the fix must respect: lane L's decline-cap clause is **unreachable from
inside a loop** (`mPassDeclineCount` increments only when `choice == 0`, so a pilot that never
passes never arms the brake), and the deadlock breaker is blind by design (its key is board
state + question; this loop is PRODUCTIVE repetition, so both halves change every window).
**Wave-48 prediction.** With F1 + F3: no game reaches 500 priority windows in one turn; the
three deck123 token games close in **< 60** priority records each (was 1,871 / 1,550 / 223);
21/21 natural; and at least one reply selects the pass row or the repeat-N row.

### R2 — the ranged collapse landed on the wrong side of combat. (= ledger **E2**)
Lane M's collapse works and is correct — it landed on the B-rows, whose **maximum observed width
in the whole corpus is 5**. The width is on the A-rows. Attackers screen: `deck123 vs deck126`
seq **1570** is **233,662 characters / 1,525 option rows**, 1,524 differing only in a `#N` handle,
178 s, answered with a **7,188-char** hand-enumerated `A1, A2, ... A1525`. Blockers screen:
`deck146 vs deck123` seq **26** is **74,063 characters for 2 real options**, 198 A-rows of which
196 normalise to one string, each carrying the same 120-char `[NONE of your available blockers
can block this attacker...]` tag. A-range collapse present in **0/38** blockers prompts and
**0/109** attackers prompts. The battlefield line in the SAME prompts collapses correctly, so
the mechanism exists and is pointed at the list that never gets wide.
Two halves: (a) apply the same identical-run rule to A-rows on BOTH combat screens, keeping
every label individually nameable as R8 already proves it can; (b) accept `ATTACK: all` and a
range form (`ATTACK: A2-A1525`) in the reply grammar — a pilot answering "all of them" should
not cost 7 KB.
**Wave-48 prediction.** Max attackers-prompt chars **< 30,000** and max blockers-prompt chars
**< 25,000** corpus-wide; blockers top-quartile mean **<= 6,000** chars (was 16,678 natural /
23,724 all); attackers p90 latency **< 45 s**; no reply enumerates more than 30 attacker labels.

### R3 — target menus do not collapse identical rows, and produce the corpus's largest natural prompts.
`deck126 vs deck123` seq **18 and 19**: **432 and 431 options, 116,128 and 116,148 characters**,
of which ~425 are `Deal 1 damage with Staff of Nin targeting Vampire #N [opponent's battlefield]
[cost: Tap] {card text: "..."}` differing only in the index, **each repeating the full card text
of the same Staff**. The seat's next-largest prompt is 18 KB. Correctness was unaffected (the
model answered option 431, the player, both times) — this is pure decision-value-per-token.
Two independent halves: (a) collapse an identical-except-for-index run of target rows into a
range, exactly as blockers now are; (b) print a repeated `{card text:}` **once per distinct
SOURCE**, not once per row.
**Wave-48 prediction.** No target menu exceeds **25,000** chars; no `{card text:}` string appears
more than twice in one prompt.

### R4 — render falsehood by scope: `0 of them able to attack right now` printed over 197 rows marked `[tapped - attacking]`. (= ledger **E3**)
`deck146 vs deck123` seq **26**: the opponent battlefield header reads `(220 permanents listed,
of which 215 are creatures, 0 of them able to attack right now)` on a board where 197 rows below
it carry `[tapped - attacking]`. The count is the engine's own `canAttack()` and is TRUE to that
predicate — and in a post-declaration combat window it contradicts the rows beneath it. Under the
trust doctrine a true statement in the wrong scope is a lie, and this is the only render falsehood
found this corpus. Ask: in any window where attackers are already declared, either suppress the
clause on that board or state the true fact for that scope (`N of them are attacking`). Never
delete the token silently — the {0}-cost precedent says the model confabulates rules into gaps.
**Wave-48 prediction.** **0** records in which `N of them able to attack right now` prints a
number lower than the count of `[tapped - attacking]` rows on the same board.

### R5 — the collapsed blocker tag picks its representative by BODY SIZE, and the decision it feeds is about PRICE. (= ledger **E-1**)
`deck146 vs deck126` seq **50**: every A-row read `they have 5 untapped creatures that could
block this one, biggest Vampire (1/1) (you kill it, your attacker lives (lifelink: they gain 1,
and their converter takes 1 off you))`. The four hidden blockers were `Perimeter Captain (0/4)`
(2 life for EVERY defender they block with — `auto=@combat(blocking)
source(creature[defender]|mybattlefield):may life:2 controller`, verified), `Pride Guardian
(0/3)` (3 life) and two `Wall of Omens (0/4)`. The printed representative carried the SMALLEST
price on the board. The seat priced the swing at 1, paid **11 life from 18**, and lost the game;
at seq **59** it did the same thing again at 6 life. Lane K's converter fold has made the PRICE
the load-bearing half of the tag, so "biggest" is now the wrong selector.
Restriction-first, nothing deleted: keep the biggest-body row and ADD a price-selected one
(`most expensive to attack into, Perimeter Captain (0/4) (neither dies (blocking trigger: ...))`),
or append the summed worst case (`[if they block with every creature on that list, they gain up
to N and your converter loss is N]`).
**Second-order effect, and it is why the guide could not compensate:** any rule keyed to a COUNT
of printed clauses is unsatisfiable under this collapse (skill amendment 91).
**Wave-48 prediction.** No reply on any seat prices a collapsed-tag swing using only the printed
representative's number while a larger price sits on the same battlefield line.

### R6 — `{card text:}` on option rows truncates mid-word and drops whole abilities. (= ledger **E5**)
7,291 occurrences, 28 distinct tails. At least four drop a REAL ability, not reminder text:
Thraben Doomsayer `"...other creatures you..."` (2,015x — the Fateful-hour lord clause gone),
Lord of Lineage `"...onto the battlefield. //"` (1,751x — the entire back face gone),
Siege-Gang Commander `"...{1}{R}, Sacrifice a Goblin: Siege-Gang"` (165x), Sorin `"-2: Destroy
target creature. Its"` (32x). R6's wave-46 per-clause budget was applied to `{effect:}` only.
Ask: extend the same clause-aware budget to the option row's `{card text:}` — never cut
mid-word, never cut inside a `--` clause, never cut before a `//` face separator.
**Wave-48 prediction.** Occurrences of `{card text: "..."}` cutting mid-word **0/N**; Thraben
Doomsayer's Fateful-hour clause and Lord of Lineage's back face present on every row that names
them.

---

## MEDIUM

### R7 — the menu header still cannot name an `auto=choice name(...)` carrier, and that family's option labels are raw script tokens. (= ledger **E6** + **E7**)
`A choice is required - choose an option:` is still **4/4** — three Silverquill Command mode
choosers (`deck146 vs130` seq 30, `vs125` seq 20, `vs126` seq 20) and one Peer into the Abyss
target chooser (`deck162 vs152` seq 21). Both cards are `auto=choice name(...)` alternative
lists (verified `borderline.txt:102762` / `:82589`); the wave-46 three-rung `resolveOwningCardName`
ladder does not reach that family. The harm is now zero (all four answered correctly, **0
fallbacks**, against 2 of 4 last corpus — the exemplar change did that), so this is a naming
residual, not a decision defect.
The same family leaks raw script tokens into its option labels: Peer into the Abyss renders
`1. target opponent` / `2. target controller` — the literal `name(...)` strings. **"controller"
means YOU**, which no reader can derive. Ask: map the self/opponent branch names to `you` /
`the opponent` at the menu emitter.
**Wave-48 prediction.** `A choice is required` **0/N**, every `auto=choice` menu header names its
card, fallbacks on those menus stay **0**; and `target controller` **0** occurrences in any
rendered option label.

### R8 — the stack line and the counter-target clause carry no keyword tags while the battlefield line does. (= `deck125` G1)
`deck125 vs deck126` seq **66**: the stack reads `1 (top): opponent's Perimeter Captain {w}
(creature 0/4) [spell]` and the counter row reads `- can target on the stack: Perimeter Captain
{w} (creature 0/4) {target text: "Defender -- ..."}`. Neither carries `[defender]`. The SAME card
on the battlefield line in the same corpus reads `Perimeter Captain #1 {w} (0/4) [defender]`.
Every counter and removal rule in the pool is written in battlefield vocabulary, so a rule keyed
to a tag is **unsatisfiable exactly where the decision is made** — and this seat spent a Cancel
accordingly. Same defect class as the own-hand count: the fact is at the call site. The tag
string the emitter already builds for the battlefield line is the whole cost.
**Guide dependency to retire, not to keep:** the deck125 guide is currently re-keyed onto
`{target text:}` as a workaround AND carries a negative-existence sentence about the render
(boundary finding B3 in `synthesis-notes.md`). Both retire the moment tags ship.
**Wave-48 prediction.** Every stack line and every `can target on the stack:` clause carries the
same keyword set as that card's battlefield entry; a counterspell spent on a stack object whose
printed power is 0 or whose text begins "Defender": **0** (was 1/28).

### R9 — lane J's `[DRAW PRICE:]` row tag is not reaching the rows that draw.
The `DRAW PUNISHERS` summary line shipped and works (**2,043 prompts**; 0 voluntary draws at any
punisher window, 0 of 19 at deck130, 0 of 13 Revelations at deck125). The per-row half fired
**twice in the entire corpus**, both in one deck146 seat — effectively UNTESTED. At `deck130 vs
deck162` seq 34/35/42/43 the summary line and a `cycling with Forgotten Cave [cost: {r}, Cycle]`
row are on the SAME screen and the row carries no price. The design principle behind the lane is
sound and general (`deck130` G47-5): **where the frame carries a summary of a hidden cost, the
ROWS that incur it carry the number.** Both existing summary lines — `DRAW PUNISHERS` and
`LIFE-TO-DAMAGE CONVERTER` — are of that shape and are read at the top of a 15-25 KB prompt and
acted on at the bottom.
**Wave-48 prediction.** `[DRAW PRICE:` emits on **every** row that causes its controller to draw
while a punisher line is printed (report N); draws at or below the summed cost stay **0/N**.

### R10 — the ANNOUNCE_X screen re-shows a carried plan that already names an X, and nothing marks the maximal row. (= `deck130` M1 + G47-4)
`deck130 vs deck126` seq **37**: `YOUR PLAN (as you last stated it): Cast Starstorm at X=3 to
kill both Pride Guardians`, reply `CHOICE: 3 (X = 3)` restating that sentence verbatim, over an
`X = 4 {kills THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: none}` row one line above.
**The X was fixed at the cast row one screen earlier, before the per-row kill lists existed.**
This is the wave-46 mulligan-chain shape (fixed for pregame asks in this same binary) relocated
onto the X menu. Second half: every comparative phrase on that screen is about MANA SAVED
(`same kills as X=N, for K less mana`); nothing marks the row that kills the most, although the
header already states `option 1 is the LARGEST X`.
Ask, either or both: suppress the carried-plan block on ANNOUNCE_X (no plan written before the
kill lists is evidence about them), and mark the row the procedure selects once per menu —
`<- most kills at any affordable X that costs you nothing`. That is a summary of rows already
printed, not a recommendation.
**Wave-48 prediction.** Announce answers restating a carried plan's X verbatim while a larger
free-kill row is on the screen: **0**; announce answers landing on the largest free-kill row:
N of N corpus-wide.

### R11 — make "no PLAN where the plan cannot change" a rule of the prompt builder, not two special cases.
Two shipped instances both worked and are the strongest engine results of the corpus: pregame
asks carry **no `YOUR PLAN` block** (50 priced mulligan asks, 0 with a plan; **8 mulligans, every
one a single look, zero chains, zero seats at keeping-4**, against wave 46's 9 third-look chains
and two six-chains), and land-drop asks end `this decision needs no PLAN line` with a consequence
sentence (**386 land asks, max latency 50.8 s** against wave 46's 453 s, declines 2/386).
The generalisation is doing real work and is stated nowhere as a principle: **a decision whose
outcome cannot invalidate the standing plan should neither request a PLAN line nor re-show one.**
The next candidate visible from the corpus is ANNOUNCE_X (R10).
**Wave-48 prediction.** Pregame and land-drop results hold (chains 0, land declines <= 2/N); if
ANNOUNCE_X is added, R10's prediction is its falsifier.

### R12 — a converter PAIR is an unbounded loop and the converter block describes only one direction of it. (= `deck162` G-8)
The block reads `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond. While it
is in play, life ITS CONTROLLER gains also makes the other player lose that much life...` — exact
for `Sanguine Bond` alone (`auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`).
deck126 also runs `Exquisite Blood` (`auto=@lifelostfoeof(player):life:thatmuch controller`,
verified). With BOTH on one battlefield any single point they gain is a closed loop. The block
names only the card it found, and a model reading it computes a bounded price for an unbounded
event. Ask: when both directions are present on the same battlefield, say so in the same block —
`both halves of a life LOOP are on their battlefield (Sanguine Bond + Exquisite Blood): any life
they gain repeats without limit, so ANY nonzero payment on a tag is fatal, not merely expensive`.
True token, restriction-first, deletes nothing.
**Wave-48 prediction.** Every prompt whose battlefield lines carry both halves prints the loop
clause; **0** replies price a tag payment as a finite number on such a board.

### R13 — the GAME LOG narration does not collapse repetition. (= ledger **E12**)
~180 identical `- You used: Create human with Thraben Doomsayer` / `- Your Thraben Doomsayer
created a 1/1 Human token` pairs per prompt, held flat only by the log's `(...earlier events
trimmed...)` cap — so ~90% of a 26 KB prompt is one repeated sentence and the model is never
given a count. The battlefield line in the same prompt collapses 1,893 tokens into one row; the
narration is the reason those prompts are 26 KB rather than 3 KB. Ask: collapse consecutive
identical narration events to `- You used: Create human with Thraben Doomsayer (x180, creating
180 1/1 Human tokens)`. This is the token-cost half of the same failure R1 fixes the decision-cost
half of.
**Wave-48 prediction.** Mean priority prompt chars in a token-loop game **< 8,000** (was 25,775).

### R14 — the battlefield ranged collapse is run-length-only, so interleaving defeats it. (= ledger **E8**)
On the same 220-permanent board a run of alternating `Vampire`/`Human` tokens rendered one row
each (`Vampire #200 ...; Human #4 ...; Vampire #202 ...`) while the adjacent run above collapsed
to `x174`; two collapse runs `#1-#22` and `#23-#196` were split only by a single interposed
Doomsayer row. Ask: group by identical rendered TEXT rather than by adjacency (or sort the entry
list before collapsing). R8's label-nameability note already covers the non-contiguous case.
**Wave-48 prediction.** No prompt contains more than 5 uncollapsed rows whose normalised text is
identical to another row on the same board line.

### R15 — `{leaves N of your M untapped mana sources untapped}` is on cast rows only. (= `deck125` G2)
The field is the single most useful new surface at the deck125 seat — it made a lane measurable
for the first time in four waves. It is absent from mana-costing ACTIVATED-ability rows:
`Life with Elixir of Immortality [cost: {2}, Tap]` prints its cost and not what it leaves, while
that seat's tap-out gate explicitly covers the Elixir "cast OR activated", so the rule can be
read off the row in one half of its scope and must be computed in the other. 15 such windows.
**Wave-48 prediction.** Every mana-costing activated-ability row carries `{leaves N ...}` or the
tap-out variant; report the count.

### R16 — a cast ask and the priority ask that follows it are answered as one menu. (= ledger **E5-adjacent**, `deck146` G-2)
`deck146 vs deck125` seq **35** (cast menu, 5 rows, answered `Cast nothing right now`) is
immediately followed by seq **36** (priority, 2 rows: `becomes beholder`, `Flip Side`) answered
`CHOICE: 8 (Cast Acererak the Archlich)` — out of range, naming a card from the PREVIOUS menu.
It is one of the corpus's two off-menu hallucinations. **The parser was right to refuse**; the
prompt composition invited it. Two candidate prompt-side fixes, both restriction-first: (a) when
a priority ask directly follows a cast ask in the same phase, say so — "you have already answered
this turn's casting question; these are ACTIVATED ABILITIES only, and no card in your hand
appears below"; (b) state the range explicitly — "answer with a number from 1 to 2, or 0".
**Wave-48 prediction.** Off-menu replies naming a card from the immediately preceding menu: **0**
(was 2 corpus-wide).

### R17 — `Cast nothing right now` is the most-taken option on some seats and the render says nothing about what it costs. (= `deck146` G-3)
20 of 71 cast menus at deck146; 47 of 107 at deck126. Every other row on the screen carries a
consequence annotation (`{leaves 4 of your 7 untapped mana sources untapped}`, `{right now:
returns NOTHING - your graveyard has no creature cards}`), and the one row meaning "spend nothing
and change nothing" is BARE. Ask, in the shape that made the land drop work: `Cast nothing right
now {you keep N untapped sources you cannot carry into your next turn}`, or on a main-2 window
`{this is your last casting window this turn}`. The token is true (mana empties at end of turn)
and the render currently makes the free-looking answer look free.
**Wave-48 prediction.** `Cast nothing right now` taken with a live higher-ranked entry on the
same menu falls at deck146 and deck126; report both rates against this wave's 20/71 and 4/47.

### R18 — the modal-DFC Pathway land route is not covered by the land-drop treatment. (= ledger **E10**)
28 records: the `Choose an option for <Pathway>:` menu still REQUESTS a PLAN line and omits the
`(playing a land costs no mana and uses up no cast: it does not reduce what you can cast this
turn)` consequence sentence that the plain land ask now carries. Repro: `deck152 vs deck126`,
Barkchannel Pathway, first of 28.
**Wave-48 prediction.** DFC land menus carry the same tail as the land-drop ask; land-drop
declines across BOTH routes stay <= 2/N.

---

## LOW

### R19 — attacker rows state the upside of attacking and never its cost. (= ledger **E9**)
The affirmative `[no creature they control can block this attacker]` fired on **60 rows and 60
were attacked** at three seats (42/42) — a clean win. The 11 corpus-wide declines are **10 of one
creature in one seat**: `Dwarven Blastminer (1/1)` (`auto={2}{R}{T}:destroy target(land[-basic])`,
verified `mtg.txt:33251`), whose land destruction is FORFEITED by attacking. The decline is
defensible and guide-mandated; the row simply cannot say so. Ask: where an attacker has an
untapped-only activated ability, name it on the A-row — `[attacking taps it: you lose
{2}{R},{T}: destroy target nonbasic land this turn]`. Restriction-first, no recommendation.
**Wave-48 prediction.** Every A-row for a creature with an untapped-only activated ability names
it; the affirmative tag's attack rate on rows WITHOUT such an ability stays >= 95%.

### R20 — a degenerate decode deserves a counter distinct from `unparsed_reply`. (= `deck125` G4)
`deck125 vs deck123` seq **39**: a **16,271-character** reply of one repeated token (`ai茧ai...`),
no CHOICE line, `latency_ms` **798,216**, recorded only as `unparsed_reply`. deck126's max is
800,769 ms, which suggests an ~800 s ceiling somewhere in the client. 3 such events corpus-wide.
An operator reading the ledger cannot currently tell a 16 KB token-loop from an ordinary parse
miss. Inference-layer, not a prompt defect — but it is a cheap instrument (`degenerate_decode`
when the reply exceeds N chars with no CHOICE line and low token entropy).
**Wave-48 prediction.** The class is separately counted; report both counts.

### R21 — two counters are fuzzy instruments. (= ledger **E11** + `deck126` G7)
`mana_only_windows_skipped` is built from `shown` AFTER the two-decline retirement filter, so a
window holding "mana ability + an option already retired this turn" is classified mana-only and
counted. It skips nothing live; it makes the counter imprecise. Second half, and it is a
reporting fix rather than a code one: **8,673 skips corpus-wide, all at one seat, 8,610 of them
in ONE game** — the gate is the reason that game finished at all, and **0 all-mana menus leaked
to the model** (wave 46 leaked 3). The gameend record should carry the count of windows that DID
reach the model alongside the skip count, so a pathological game announces itself in one field
instead of a join.
**Wave-48 prediction.** The counter is computed pre-filter or renamed; gameend carries both
fields; leaks stay **0**.

### R22 — `dropped_assignments` counts `Bn:none` pads. (= ledger **E-6**)
`deck152 vs deck123` blockers seq **26** answered `BLOCKS: B1:A1, B2:none, B3:none` on a
two-blocker screen and the record reads `dropped_assignments: 1`. Nothing was lost. The counter
is the engine seat's instrument for REAL drops and a pad inflates it. Ask: exclude `:none`
assignments, or state the B-range on the ask the way the A-range is stated.
**Wave-48 prediction.** `dropped_assignments` counts only assignments that named a real blocker
and a real attacker.

### R23 — one row in a two-row modal menu carries a `[cost:]` bracket and its sibling does not.
`1. cycling {card text: "Cycling {3} (...)"} ` beside `2. Cast Card Normally [cost: {r}{r}{x}]
{card text: ...}`. The alternative-cost row's cost is buried inside a parenthetical. **0 wrong
choices in 4 windows this corpus** (the lane's annotation and the guide edit shipped together),
so this is parity, not a defect — but it is the last unannotated row shape at that seat and the
menu it lives in cost that deck a card in two previous corpora. Ask: build the bracket from the
`autohand=__CYCLING__({3})` value the parser already has — `1. cycling [cost: {3}, Cycle]
{discard this card and draw 1}`.
**Wave-48 prediction.** Every row in a `Choose an option for <card>:` menu carries a `[cost:]`
bracket or explicitly reads free.

### R24 — `manaAvailableLine` still drops the spelled-out number above ten. (carried from wave 46)
`Mana available: 11 total (untapped sources, tapped automatically...` — 41 emissions at deck130
(all vs152, turns 20+) and 4 at deck123. Cosmetic; carried a second wave so it is not re-derived.

### R25 — the `{right now: ...}` per-branch preview is the corpus's best decision surface and is not yet general. (= `deck162` G-9)
`deck162 vs deck152` seq **20**: `1. Cast Peer into the Abyss {4}{b}{b}{b} {right now: if you
choose "target opponent": life -5, draws 19; if you choose "target controller": ...}`. The seat
cast it, answered the follow-up chooser correctly, and won 21-9 -> **21-0** that turn. The row
named both branches and priced both, on the row, before the commit. Contrast the same seat's
`-2: destroy target creature and draw two cards with Ob Nixilis ... targeting <name>` rows, which
print the target and not the consequence, and which the seat answered inconsistently. Ask: extend
the per-branch preview from X/modal casts to activated abilities whose effect quantity depends on
the chosen target — same emitter, same shape. Nothing here is a bug.
**Wave-48 prediction.** Target-dependent activated-ability rows carry `{right now: ...}`; report
the count and the seat's consistency on Ob Nixilis-shaped rows.

### R26 — on a LETHAL-flagged blockers window, put the affordability fact next to the priority promise. (= `deck123` G47-3)
`deck123 vs deck152` seq **27**: a 5-attacker blockers screen produced a ~1,800-word reply at
**522 s** in which the pilot re-derived every per-attacker trade the rows already printed and
then built its survival line on `Cast Tribute to Hunger` — with `Mana available: 1 total` on the
same screen and Tribute costing `{2}{b}`. The screen already says "Trust it over your own
arithmetic; never re-derive it", and the instruction lost on the one window where the header said
LETHAL. The invitation is the priority promise, so the fix goes next to it: `You keep priority
through the rest of this combat ... of the cards in your hand, N are castable with your current
Mana available: <list or "none">.` The engine already computes castability for every `ask`
window. Hides nothing, forbids nothing, removes a whole branch of reasoning that was fiction.
**Wave-48 prediction.** Combat replies building a line on a spell whose mana value exceeds the
same screen's `Mana available: N total`: **0** (was 1 of 1 such window); blockers p99 latency
falls.


### R27 — one keyword, three different rendered wordings across three surfaces. (= ledger **D19**; found in the boundary pass)
`menace` renders as `[menace (can't be blocked except by two or more creatures)]` on the
battlefield line (**64**), `[menace - cannot be blocked by fewer than two creatures; they have N
untapped creatures that could join such a block, ...]` on an attacker A-row (**36**), and
`[menace - only a block by TWO OR MORE of your creatures counts; one creature alone does not
block it at all]` on a blockers-screen A-line (**10**). No one is a substring of another, so a
rule keyed to one is unsatisfiable on the other two — the same defect class as R5 and R8, in a
third place. Ask: one canonical keyword substring per keyword, with the surface-specific clause
appended after it.
**Wave-48 prediction.** Every `[menace ...]` tag opens with the same canonical substring; report
the distinct-form count for `menace`, `defender`, `flying` and `lifelink`.


---

## SHIPPED SINCE THE CORPUS — recorded with its prediction, NOT docketed

| commit | what | wave-48 prediction |
|---|---|---|
| **5ce8f8a5a** | **E1 prose re-answer replacement.** `choiceRetractedNoReplacement` scanned for the replacing `CHOICE:` **line-leading only**, so a mid-line `So, CHOICE: 0 (pass).` after the reasoning was not found, `choice` became -1 and the heuristic decided — and because `choice != 0` the model's own explicit stop did not even count toward the decline cap. Both corpus occurrences (`deck123 vs deck146` seq **34** and **221**, the two most expensive deliberations in the corpus at 216 s and 190 s) were the model correctly reasoning ITSELF out of a token loop. Now a mid-line `CHOICE: N` after the retraction marker is the decision, and a resolved `0` counts as a decline. | `retracted_choice` records carrying a resolvable mid-line replacement: **0/N**; any remaining `retracted_choice` has no parseable `CHOICE:` anywhere after the retraction. Secondary: at least one loop window is exited by a resolved mid-line pass, and `mPassDeclineCount` arms lane L's clause on it. |

---

## GUIDE-ONLY — stays in guides after the render ships
- deck130's `{effect:}`-keyed / summary-line-keyed draw-punisher rule (**0 of 19**, and it runs in
  the negative direction too: at 3 life with no punisher line it cycled and said so).
- deck126's per-colour-count Lantern test (5/5 wrong -> 1/3, positive half discharged).
- deck152's converter-vs-price precedence and deck162's converter/lifegain hold (**2/2** in the
  exact matchup that produced a 19-to-0 turn last corpus).
- deck123's edict identity teach (RULE 5), now re-pointed at the pilot's own PLAN sentence.
- deck126's menace all-or-nothing rule (new; the render is complete, no guide in the pool had it).
- deck162's punisher tiebreak (new; see the boundary findings — it needs a rank in the cast list).

## MEASUREMENT items for the wave-48 reviewer briefs
1. **Restate every rate touched by a suppressing lane on a like-for-like basis** (skill 87). The
   mana-only gate, the two-decline retirement and any A-row collapse all move denominators.
2. **Prompt chars per decision kind, wave 47 -> 48, loop-family games excluded on both sides.**
   Wave 46 -> 47 moved: ask +6.2%, **priority +24.4%**, attackers +9.5%, blockers +1.9%, reveal
   -13.9%, bottom -13.6%. Priority is 72% of all decisions; R2/R3/R13 are the items that pay it back.
3. **Re-derivation rate** (skill 100): a regex over replies for restated own-board mechanics,
   reported against wave 47's 34/199 (17%) at deck126.
4. **The `[repeat:]` tag's own arithmetic** — see ledger E13; report tag value vs the seat's own
   activation count per game.
5. **Blockers/attackers width gate, pre-registered**: max A-rows, max B-rows, and the count of
   collapsed ranges on each screen.

## PASS / KEEP — measured working, do not re-open
`Your hand (N cards):` (5,644/5,644 prompts, **0** replies stating a wrong own-hand size, was
3/4) · the menu exemplar drawn from option 1 (0 `Cast *` replies into a cast-free menu in 4,128
priority prompts; the priority tail's placeholder is NOT latched at 4,127/4,128 — leave it alone)
· the priced mulligan row + pregame plan suppression (R11) · the land-drop consequence sentence
(R11) · `{c}` in colour sets (`{1}`/`{x}` inside a colour set: **0**, was 18) · the per-clause
`{effect:}` budget (61 planeswalker strings, 0 truncated, 61/61 keep a negative-loyalty clause)
· own-side `{effect:}` minus option-row permanents (0 unexplained bare own permanents in 612
decisions) · the affirmative no-blocker tag (60/60 attacked; it has ABOLISHED the untagged A-line)
· the zero-power `STOPS ... THIS COMBAT` scope (0/12, and it is not being over-read into a general
licence) · the converter fold in both voices · the X=0 null-cast callout (**0 of 29** X
announcements chose X=0) · `{X pricing:}` per-row (took one seat's X lane 4/17 -> 12/12 with no
guide change) · `N of them able to attack right now` / `N of them without a restriction against
attacking` (a perfect complement by turn; discriminated one seat's sweeper casts **13/13**) ·
`{leaves N of your M untapped mana sources untapped}` on cast rows (made a four-wave lane
measurable) · the mana-only auto-pass gate (**0 leaks**, was 3; it is why the pathological game
finished).
**Zero new render falsehoods found this corpus except R4**, which is a scope error rather than a
wrong value. Every other open item is an omission, a collapse pointed at the wrong list, or a
missing affordance.
