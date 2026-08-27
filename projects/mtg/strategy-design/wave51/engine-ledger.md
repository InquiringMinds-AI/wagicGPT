# Wave-51 engine ledger — THE WAVE-52 DOCKET (engine / UX only)

Corpus `matchups-20260827-155545` (eighth fair-hand corpus), binary master **c010ddb47** (lanes A
42f2eff2b, B df8ff7641, C 4cf3745a7, D ac7c5f5be, E 9c772cd3d, F 7c0a04d1d + Vita fixes
ca8a71e14 / dd8663146; PARSETEST 2022/0; suite expected 1154/0 + 41 AI/0). 42 seat logs, **2,213
records**, **2,129 decisions** (ask 1,493 / priority 445 / attackers 110 / blockers 31 / discard 31
/ reveal 13 / bottom 6), **21/21 games natural — 42/42 seats carry `gameend`** (verified on disk by
this step over every JSONL: 42 `gamestart`, 42 `gameend`, 5 `fallback` fields). The
`seat-125-126.md` header's "deck123-vs-deck162 dead-loop game is excluded" note is a stale-note
artifact for the THIRD wave running (this run's pair `1787864154-...deck123-0x563f06e15f80-vs-
...deck162.jsonl`, 34 records, natural `gameend` turn 12) and is **DROPPED**; its numbers are
per-seat and unaffected. Corpus root: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-155545/`.
Seat names `deckA-vs-deckB` = the deckA seat's JSONL; `seq` is the record's own field; the JSONL
`turn` field now equals the narration's `=== Turn N` (lane D, 2,070/2,070).

**Health.** Fallbacks by the `fallback` field **5 / 2,129 = 0.23%**: `named_row_reask` 3 (all
recovered next record — `[RE-ASK]` rendered 3 times, its first live firings), `unparsed_reply` 1
(D2's second window), `stale_livelock` 1 (**NEW class**, D1). **Baka-executed decisions 2** (the
livelock and the unparsed reply). `answer_replaced` 18 (all audited by the engine seat: last-clean-
line self-revisions, all correct; one `deck123-vs-deck125` pattern ×5 `CHOICE: 5 (Cast Idyllic
Tutor)` -> `CHOICE: 6 (Cast nothing right now)`). `commit_retracted` 0. `parse_note` 12:
`named_row_reask_recovered` 3, `attack_last_line_taken` 2, `named_row_not_offered` 2 (+1 compound),
`stale_echo_in_range` 2 (+1 compound), `echo_index_conflict` 1, `echo_index_conflict_ambiguous` 1.
`mana_only_windows_skipped` max **161** (was 203). Max priority windows in one turn **10** (=
wave 50; `deck123-vs-deck126` turn 14, the repeat/equip turn). Max prompt **33,536** (`deck146-vs-
deck125` seq 279, priority, turn 78, 71% log) — **3 prompts > 30,000**, all that game (seq 235
30,533 / 237 30,898 / 279 33,536; verified on disk); largest non-log-driven prompt **29,469**
(`deck123-vs-deck126` seq 44, 23% log, 35 rows, 26 of them equip rows — D8). `- Paid` receipts
**452** actual (the prompt-side count 16,779 is log carry). stderr: `with no source tapped` 0,
`auto-passing without a model call` 1,387, `repeat plan iteration` 44, `dropping stale async
answer` 39 (9 games; ONE game reached the 6-streak). Latency max 849 s (a 10,969-char correct
self-revision on a 10.7 KB prompt — deliberation, not size).

**Mulligans (real hands).** 36 seats kept 7; 6 mulliganed: four to 6, `deck152-vs-deck125` to 5,
and `deck146-vs-deck125` FIVE times to a **two-card keep** (seq 1-6; the colour clause that seeded
it is a guide item, cut by the reviewer — 146-M; skill #153). No mull-to-zero.

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the reviewers';
guide-side halves are named only where a docket item retires one. Hard caps on the model's legal
choices are rejected; every item adds a true token, a true row, a receipt or one re-ask, and
deletes nothing. Card-behaviour defects go CARD-SCRIPT FIRST. Two window-removing proposals were
rejected this wave (the Upkeep DEFER re-file, D13; auto-passing asks inside a repeat loop, D1's
first form) and adopted in their constraint-safe forms.

---

# THE DOCKET, RANKED

## D1 — HIGH — `stale_livelock`: the repeat-N loop launches a casting ask per iteration, each dropped answer counts toward the 6-streak breaker, and Baka gets the mid-loop decision — it CAST an edict the model then aimed at itself — **IN FLIGHT (lane G, `~/Projects/wagicGPT-w52-g`)**
(= engine seat **D-1 HIGH** / seat-123-130 **H4** / `general-strategy.md` **R105**)
**Repro.** `1787864189-ai_baka_deck123-0x5593ba17b190-vs-ai_baka_deck126.jsonl` seq **47** `CHOICE:
29 (Create vampire with Lord of Lineage x17)` on the row `... [repeat: you control 28 creatures;
activated this turn 2 times already. This turn will not advance while you keep taking this
option.] ... x17`; seq **48** `kind: ask, fallback: stale_livelock, latency_ms: -1, reply: "",
choice: -1` with `options_text` = [`Cast Devour Flesh {1}{b} {right now: they control 7 creatures
- they choose which one} {leaves 0 of your 2 ...}`, `Cast nothing right now (combat comes next this
turn)`] — the heuristic took row 1 (all verified on disk); seq **49** the model, asked for a target
on a spell it never cast, `CHOICE: 2 (Yourself (player, life 17))`; seq **50** `CHOICE: 1 (Lord of
Lineage (5/5) ...)`; seq **51** events `- Your Lord of Lineage died / - You gained 5 life (now 22)
/ - Your repeated activation ran 7 of the 17 times you named (the cost could no longer be paid)`.
stderr `game-126v123-1787864185.stderr` lines 2145-2231: each `AIPlayerGPT[ph4]: repeat plan
iteration k/17 (no model call)` is preceded by `land-drop ask NOT issued` and `dropping stale
async answer` — an async Casting decision (Devour Flesh castable with 2 open) launched between
iterations, the board changed, the answer dropped, ×6 -> line 2231 `6 consecutive stale drops -
giving this decision to the heuristic` -> `chose -1 of 2` -> `ai is doing a combo:Devour Flesh`.
**Mechanism.** `kStaleLivelockLimit = 6` (AIPlayerGPT.cpp ~8117, `pollCompletion` ~8095-8125) was
built for an unstable prompt on an UNCHANGED state; the repeat loop changes the state legitimately
every iteration. Precondition wave 50's four >= 17 repeats lacked: a castable spell in hand during
the loop.
**Ask.** (a) While `repeat plan iteration` is in progress, do not launch a model request for a
window the next iteration will invalidate (or do not count its drop toward the streak) — the
row's own contract says "then returns priority to you here", so no window the model would have
answered is removed; (b) a fallback of ANY class never executes a non-pass row when a pass row
exists on the menu (seat-123 H4's second half; the wave-50 D3 principle — Baka's pick is not what
the model would have chosen). PARSETEST/suite: a 17-repeat with a castable instant in hand -> 17
tokens, no ask record between the take and its receipt.
**Wave-52 prediction.** `stale_livelock` **0/N** (was 1); every repeat take runs `N of the N times
you named` unless the cost fails for a board reason narrated in the same block; Baka-executed
non-pass rows 0.

## D2 — HIGH — the `Cast nothing right now` echo is matched against the FULL row text including `{...}` annotations; `{right now: ... at 0 this does nothing}` on edict rows makes the pass row ambiguous — 1 dead Tutor cast + 1 `unparsed_reply` to Baka — **IN FLIGHT (lane G)**
(= engine seat **D-2 HIGH** / **R106**)
**Repro.** `1787864180-ai_baka_deck123-0x5611df0fea40-vs-ai_baka_deck125.jsonl` seq **48**: reply
`CHOICE: 5 (Cast nothing right now)`; rows `1. Cast Tribute to Hunger {2}{b} {right now: they
control 0 creatures - at 0 this does nothing} ... 2. Cast Devour Flesh ... {at 0 this does nothing}
... 5. Cast Idyllic Tutor {2}{w} [already owned: Intruder Alarm on your battlefield - this finds only
an enchantment card] ... 6. Cast nothing right now`; `parse_note: echo_index_conflict_ambiguous`,
`choice: 5`, `chosen_text: Cast Idyllic Tutor ...` (verified on disk) — Cancelled at seq 50. seq
**49**: `CHOICE: 4 (Cast nothing right now)` on a 3-row menu -> `fallback: unparsed_reply`, Baka.
**Mechanism.** AIPlayerGPT.cpp ~13620-13660: the echo's significant words (`nothing`, `right`) are
matched against the full option text; every 0-creature edict row matches -> `echoConflict` ->
index-wins (48) / no remap for an out-of-range index (49).
**Ask.** Match the echo against the row's SHORT LABEL (text before the first ` {` / ` [`), or treat
the pass-row label as a reserved exact echo. PARSETEST: the seq 48 reply on its menu (expect row
6), the seq 49 reply (expect row 3). Audit siblings: any annotation vocabulary overlapping a row
label (`nothing`, `pass`, `decline`).
**Wave-52 prediction.** `echo_index_conflict_ambiguous` / `unparsed_reply` on a reply whose
parenthetical equals a row's short label: **0/N** (was 2).

## D3 — HIGH — the cleanup `discard` PUT parser executed a DIFFERENT card than the numbered row when the reply carried a bracketed name — **IN FLIGHT (lane G)**
(= seat-146-152-162 **E-1 HIGH** / deck162 G-2 / **R107**; skill #152)
**Repro.** `1787864179-ai_baka_deck162-0x55a817091b20-vs-ai_baka_deck146.jsonl` seq **17** (T12
cleanup, hand 8): `options_text` = [Peer into the Abyss, Liliana's Caress, Forced Fruition,
Dictate of Kruphix, Teferi's Puzzle Box, Underworld Dreams, Forced Fruition, Swamp]; reply first
line `PUT: 3 (Forced Fruition)`; record `choice: 1, chosen_text: "Liliana's Caress"` (verified on
disk); narration seq 18 `Cleanup discard (hand 8, limit 7): you chose Liliana's Caress`. The 11
other PUT lines this corpus (no bracket) mapped to their printed rows.
**Mechanism (NOT pinned).** `AIPlayerGPT::cleanupDiscard` (src/AIPlayerGPT.cpp:22651) ->
`parseAttackerSet(decisionPart, hand.size(), send, &names)` (:20288): the index pass should mark
row 3; the name pass splits on commas and matches "forced fruition" against TWO identical names
(rows 3 and 7) -> `uniqueNameMatch` -1 — explains a miss, not row 2. The reproducible input is the
string above against that 8-name list; the `WAGIC_GPT_PARSETEST` self-test (:33483) can take it.
**Ask.** The PUT parser honours a bracketed name the way `parseChoice`'s named-row family does:
index first; a name matching the indexed row confirms; a name matching a DIFFERENT row -> one
re-ask; a name matching nothing -> the index. PARSETEST: the exact string; `PUT: 3 (Forced
Fruition)` with duplicates at 3 and 7 -> row 3. The ask text's "number only" and deck162's 162-H
are interim.
**Wave-52 prediction.** `PUT:` replies whose executed `chosen_text` differs from the printed row(s)
named: **0/N** (was 1/12).

## D4 — HIGH — the payment planner taps EVERY Overgrown Battlement (a variable source worth {G} x defenders) for a cost ONE covers, floats the rest, and `{paying this taps:}` disagrees with the receipt — three walls tapped into a combat that took 18 — **IN FLIGHT (lane H, `~/Projects/wagicGPT-w52-h`)**
(= seat-125-126 **HIGH #1** / deck126 G1 / **R108**; wave-50 D32 "Hive over-tap under the planner" carried into a wall deck)
**Repro.** `1787864171-ai_baka_deck126-0x564fc96be4a0-vs-ai_baka_deck152.jsonl` seq **18** row `Cast
Tribute to Hunger {2}{b} {right now: they control 7 creatures - they choose which one} {leaves 1
of your 4 untapped mana sources untapped} {paying this taps: Overgrown Battlement, Overgrown
Battlement - they cannot attack this turn}` (TWO named) -> seq **19** events `- Paid {2}{b} for
Tribute to Hunger with Overgrown Battlement #1; Overgrown Battlement #2; Overgrown Battlement #3;
Woodland Cemetery` (THREE) and the seq 19 prompt `Already in pool: {g}x13 (13 mana ALREADY
produced and floating right now ...)` (all verified on disk). seq **26** -> 27: Exquisite Blood,
`Paid {4}{b} ... with Overgrown Battlement #1; #2; #3; Woodland Cemetery`; the same record's
blockers window offered only `B1. Pride Guardian`, `B2. Wall of Omens` against Tovolar's
Huntmaster, two Wolves, two Intrepid Adversaries and the fliers; 32 -> 14 (`you would be at 3`).
seq **30/31**: `Paid {5}{b}{b} for Sanguine Bond with Overgrown Battlement #1; #2; #3; Chromatic
Lantern; Savannah` — five sources for seven mana when one Battlement made five.
**Ask.** In `ManaEngine::planPayment` (lane B), when a variable source's output alone covers the
remaining generic cost the plan stops there (variable sources are last, then at most one); the
`{paying this taps:}` clause is computed from the plan the payment will actually execute so the
forecast and the receipt agree. Suite pin: Tribute {2}{b} with three 5-defender Battlements + one
{b} land taps ONE Battlement; `{paying this taps:}` count == receipt count.
**Locus.** lane B's `planPayment` / `deferFlexibleSource` (df8ff7641) as exercised by
`AIPlayerBaka::payTheManaCost`; the `{paying this taps:}` emitter (lane T).
**Wave-52 prediction.** `{paying this taps:}` count == receipt count on variable sources: N/N (was
1/2); Battlements tapped for a payment one covers: **0** (was 3/3); floating mana after a planned
cast: 0 (was 13).

## D5 — HIGH — NO empty-stack priority window on the opponent's turn carries a `Cast` row: instants are never offered in their turn; the core prompt's "priority again later this turn" sentence is false on every such window
(= seat-123-130 **H1 HIGH** / deck123 R4 / **R109**)
**Repro.** Seat-123's script over all 42 logs: 51 opponent-turn empty-stack `priority` windows, 41
with an `[instant]` in hand, **0 with a Cast row**; this step's stricter re-count (header `It is
the opponent's turn`, stack line empty or absent): 36 / 24 / **0** — concordant on the zero.
`1787864180-ai_baka_deck123-0x5611df0fea40-vs-ai_baka_deck125.jsonl` seq **111** -> **112**: Emrakul
on the stack in their turn-60 main 1 (Tribute at N 0); the extra turn 61 ran upkeep -> draw ->
main 1 -> attack with no window, so `Cast Tribute to Hunger {right now: they control 1 creature -
Emrakul ... you gain 15}` (which resolved for +15 at seq 65 in the FIRST Emrakul's main 2) was
never offered before annihilator 6 took the Alarm and 15 damage ended the game.
`1787864168-ai_baka_deck123-0x55ebcae15190-vs-ai_baka_deck130.jsonl` seq **8** (their upkeep, Tragic
Slip in hand, Blastminer on their line): five Flats rows, no Cast row; that prompt reads "You will
have priority again later this turn, so instants and activated abilities you hold stay castable
this turn" (verified on disk).
**Ask.** (engine/orchestrator) Offer the pilot's castable instants on the opponent's turn at the
stops the activated abilities already get (upkeep, main phases, beginning of combat), or at
minimum one stop after a creature resolves on their side. (core prompt, interim) Drop the
"priority again later this turn" sentence on opponent-turn windows or restrict it to the pilot's
own turn.
**Wave-52 prediction.** Opponent-turn empty-stack windows with an `[instant]` in hand carrying a
Cast row: > 0 (was 0/41); Tribute/Devour/Slip cast on the opponent's turn on an empty stack: >= 1
at deck123; the false sentence on opponent-turn windows: 0.

## D6 — HIGH — an index/name conflict where BOTH are on the menu executes the INDEX
(= seat-123-130 **H2 HIGH** / **R110**; sibling of lane C's `off_menu_name_in_range`)
**Repro.** D2's seq 48: the parenthetical `Cast nothing right now` IS on the menu at row 6, the
index says 5 — row 5 executed. Contrast `1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_
deck125.jsonl` seq **295** `CHOICE: 3 (Plains #3 - "W")` on rows `1. Plains #2 / 2. Plains #3 / 3.
Plains #4` -> `echo_index_conflict`, executed 2 (name wins) — correct.
**Ask.** When the parenthesised short name matches a menu row EXACTLY and the index points
elsewhere, the NAME wins (or one `[RE-ASK]` quoting both lines, `index_name_conflict`). D2's
short-label match removes the ambiguity that produced this window; the rule stands for a genuine
two-row conflict. PARSETEST: `CHOICE: 5 (Cast nothing right now)` on D2's menu -> row 6.
**Wave-52 prediction.** Replies whose parenthetical exactly names a row other than the indexed
one, executed at the index: **0/N** (was 1).

## D7 — HIGH — a cast whose payment plan taps a self-damaging source for the pilot's LAST life point prints nothing lethal
(= seat-123-130 **H3 HIGH** / deck130 R1 / **R111**; wave-50 D26 PROMOTED; skill #155)
**Repro.** `1787864160-ai_baka_deck130-0x556e1bc31180-vs-ai_baka_deck126.jsonl` seq **85** (`Your
life: 1`): `Cast Spark Spray {r} {leaves 0 of your 1 untapped mana source untapped - casting this
taps you out}`; the only source was Talisman of Impulse (`{T}:Add{R} and!( damage:1 controller
)!`); events `Paid {r} for Spark Spray with Talisman of Impulse`, `Your Talisman of Impulse dealt 1
damage to you (now 0)`; seq 86 `gameend` 0/24, the opponent never attacked (verified on disk). seq
82-83 the same at 2 -> 1; seq 49 at 3 -> 2. The frame prints `CAUTION - some usable mana sources
DAMAGE YOU`; the row that kills is silent. Lane B's planner used Talismans for GENERIC pips without
damage (seq 49 `Paid {1} ... Talisman #2`, seq 79) — the clause is needed only when a coloured pip
is planned onto a pain source.
**Ask.** On cast rows whose plan taps a pain source for a coloured pip: `{pays {r} with Talisman of
Impulse: you take 1 - life 1 -> 0, LETHAL}`; same emitter for pain lands and phyrexian mana.
Suite pin: Spark Spray at 1 life with a Talisman as the only {r} source -> the clause with LETHAL.
**Wave-52 prediction.** Casts paid by a pain source's coloured pip at life <= the damage with the
clause rendered: **0/N** (was 1 lethal + 2).

## D8 — MED — per-token ability target rows are not collapsed: 26-34 `Equip with Lightning Greaves targeting Vampire #N` rows (~600 chars each) = the only non-log prompt near 30 KB; own-Goblin sacrifice menus of 9-20 rows = the three longest replies at deck130
(= engine seat **D-4** / seat-123-130 **M2** / deck123 R2 / deck130 R3 / **R112**)
**Repro.** `1787864189-ai_baka_deck123-0x5593ba17b190-vs-ai_baka_deck126.jsonl` seq **44** (29,469
chars at 23% log, 35 rows, 26 equip rows — verified on disk), **42** (36), **47**, **51** (34); each
row repeats `(Lightning Greaves is ALREADY attached to Lord of Lineage - this MOVES it to Vampire
#N, and Lord of Lineage loses what it grants)`, the `[repeat: activated this turn 1 times already
...]` tag and the card text. `1787864168-ai_baka_deck130-0x55ebcc135bf0-vs-ai_baka_deck123.jsonl`
seq **26**: 20 sacrifice rows; `130 vs126` seq 15/19/23/27/33/35: 11 rows each; latency 592 / 652 /
602 s on those windows (seat-123 L2). The target CLAUSE already collapses `Vampire #1-#27 x27`.
**Ask.** One option row per identical-token group with `xN` (or the target as a sub-ask), as the
X-menu rows and the target clause already do; collapse own-permanent self-target rows the same
way (deck126 G6: five `{this hits YOUR permanent}` Staff rows of 33 at `126 vs123` seq 23).
**Wave-52 prediction.** No priority window with > 12 rows of the same verb+source over `#N`
targets; no non-log-driven prompt > 20 KB (was 29,469).

## D9 — MED — `{this hits YOUR permanent}` is on damage/destroy rows only; exile / loyalty rows targeting the pilot's own permanent carry nothing — Kaya -3 exiled HERSELF at 1 life
(= engine seat **D-3** / seat-146-152-162 **E-5** / deck146 G-2 / **R113**)
**Repro.** `1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_deck125.jsonl` seq **284** (`Your
life: 1`): `-3: exile non-land permanent with Kaya the Inexorable targeting Kaya the Inexorable
[your battlefield] [cost: Counters] {card text: "+1: ... -- -3: Exile target nonland permanent. --
-7: ..."}` — the full `chosen_text` on disk carries NO self-hit clause (seat-146's unread `{if
yo...` tail is not on this row); taken. Rendered with the tag: `Deal N damage ... [your
battlefield] {this hits YOUR permanent}` 229, `Destroy ...` 30. Untagged: `Equip ... [your
battlefield]` 118 (legitimate), Kaya `-3` self-exile 74 (1 taken), `+1 ghostform` 47 (benign),
`-3: emblem` 9, `-3: destroy target creature` 2.
**Ask.** The lane-W tag on every exile / destroy / sacrifice-class ability row whose target is the
pilot's own permanent; equip and buff rows stay untagged. Suite pin: Kaya -3 with only own
permanents -> every row carries the tag.
**Wave-52 prediction.** Self-exile / self-destroy loyalty rows carry the tag N/N; takes 0 (was 1).

## D10 — MED — `[already owned: <name> on your battlefield]` marks SAME-NAME, not dead: 15/21 takes were stackable duplicates that were the plan; the two legendary takes bought a legend-rule bin with no clause; the Tutor form names the half the pilot HAS and reads as a warning against the correct Tutor
(= engine seat **D-5** / seat-125-126 **MED #3** / deck126 G3 / **R114**; lane E's D8 second draft)
**Repro.** 139 rows, 21 takes. Stackable: Howling Mine ×4, Talisman of Impulse ×5, Underworld
Dreams ×3, Staff of Nin ×2, Elixir ×1 (`162 vs126` seq 9/13; `125 vs130` seq 20). Legendary:
`1787864185-ai_baka_deck152-0x55a529a8cd70-vs-ai_baka_deck146.jsonl` seq **41** `Cast Teferi, Who
Slows the Sunset {2}{u}{w} [already owned: Teferi ... on your battlefield]` -> seq 43 `- Your
Teferi ... was put into your graveyard from the battlefield (that Teferi ... was 1 of 2 copies)`;
`146 vs126` seq 20 Lolth (a loyalty refresh — legitimate). Tutor: `1787864148-ai_baka_deck126-
0x563799bf88c0-vs-ai_baka_deck125.jsonl` seq **21** `Cast Idyllic Tutor {2}{w} [already owned:
Exquisite Blood on your battlefield - this finds only an enchantment card]` — Sanguine Bond was
the card to find, the Tutor was right (taken, won); `126 vs152` seq 31 both out (the warning
right).
**Ask.** Three forms: stackables `[a second copy stacks]` (or untagged); legendaries `[legendary:
the legend rule will put one into your graveyard]`; Tutor rows name the half NOT owned —
`[finds: Sanguine Bond - not on your battlefield or hand]` / `[both halves already on your
battlefield - a copy adds nothing]`.
**Wave-52 prediction.** Legendary second-copy casts with the legend clause rendered: **0/N** (was
2); Tutor rows naming the missing half N/N; dead-Tutor takes with that form 0 (was 4).

## D11 — MED — `[from exile]` cast rows carry no cause and no tax: "distractor / illegal" for four windows at deck123, 248 s at deck126
(= seat-125-126 **MED #2** / deck126 G2 / deck152 G-2 / deck123 A51-3 / **R115**; skill #154)
**Repro.** `1787864171-ai_baka_deck126-0x564fc96be4a0-vs-ai_baka_deck152.jsonl` seq **26** (248 s):
`2. Cast Idyllic Tutor {4}{w} [from exile]` -> "Option 2 is likely a distractor ... So Option 2 is
illegal"; seq 19 `1. Cast Staff of Nin {8} [from exile]` (base {6}); seq 30 `Cast Sanguine Bond
{5}{b}{b} [from exile]` taken, won. `1787864150-ai_baka_deck123-0x55db5c1994a0-vs-ai_baka_deck152.
jsonl` seq **20/24/26/27**: `Cast Intruder Alarm {4}{u} [from exile]` with the PLAN "Intruder Alarm
is exiled, I cannot cast it" four windows running. Primitive: Elite Spellbinder `moveto(
opponentexile) ... newability[canplayfromexile forever], newability[changecost(colorless:2)]`.
**Ask.** `[from exile - your card, exiled by their Elite Spellbinder; you may cast it from there at
{2} more than printed]` once per emitter path (Spellbinder, Kaya, the Cathar's blink); on the
Spellbinder's own hand-reveal ask `{exiled this way it stays castable for {2} more}` on the header.
**Wave-52 prediction.** PLAN lines reading "exiled ... cannot cast" / "distractor" beside a `[from
exile]` row: **0** (was 5).

## D12 — MED — the `becomes beholder` row RETURNS in the same window after a successful animation, tagged `[repeat: activated this turn 1 times already ...]`, taken 3 of 9 — twelve mana for three 3/3s that already existed
(= seat-146-152-162 **E-2** / **R116**; wave-50 D32 Hive carry; skill #165)
**Repro.** `1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_deck125.jsonl` seq **89 -> 91** (T47
Main 1: animate, then the repeat row, `CHOICE: 4`), **110 -> 112** (T51), **260 -> 262** (T76, the
row also carrying `{spends 4 of your 7 ...; Silverquill Command {2}{b}{w} in your hand needs 4}`).
0/3 at deck152's Lair. The guide's absolute exists and was skipped in sequence (#138 — no fourth
restatement).
**Ask.** After a `becomes` animation resolves, the identical row is not re-offered until the phase
advances, OR is re-offered with `{already animated this turn - a second activation adds nothing}`
(the D4 `x0`-is-decline family). The annotation form is the constraint-safe minimum.
**Wave-52 prediction.** `[repeat:` `becomes` rows taken after a same-window animation: **0/N** (was
3/12).

## D13 — MED — an `[Upkeep offer:]` Lair animation was taken and the animated land was then TAPPED FOR MANA by the Main-1 cast; the D7 annotation was not enough — the DEFER re-file is REJECTED again, the planner half adopted
(= seat-146-152-162 **E-3** / deck152 G-1 / 152-E / **R117**)
**Repro.** `1787864167-ai_baka_deck152-0x56112d739ff0-vs-ai_baka_deck125.jsonl` seq **36** (T17
Upkeep) `becomes a 1/1 hydra ... [Upkeep offer: this animation lasts only until end of turn, and
the same row is offered again in your main phase - declining here counts toward this turn's two
declines]` taken; seq **39** `Cast Elite Spellbinder ... {paying this taps: Lair of the Hydra - it
cannot attack this turn}` taken; seq 41 attackers = Sigarda, Adversary. 34/35 tagged Upkeep offers
at the seat declined; 0/32 Hive Upkeep takes at deck146 (lane E's annotation PASS).
**Ask.** (a) lane B's planner treats an animated creature-land as a source of last resort when
another source can pay the pip (as Katilda-granted creature mana already is); (b) the Main-1 cast
row's `{paying this taps: Lair of the Hydra}` gains `- it was animated this turn`. **The DEFER
form (skip the Upkeep offer unless a lethal check holds) is REJECTED**: it removes a legal window
and encodes a guide judgment in the engine.
**Wave-52 prediction.** Animated lands tapped by the planner while another source could pay:
**0/N** (was 1/1).

## D14 — MED — `plan_choice_conflict` misses the single-row take, the no-PLAN reply, and the natural-language "we must pass"
(= seat-123-130 **M3** / deck146 G-3 / seat-146 **E-4** / **R118**; lane C scope; wave-50 D20 counter ships with it)
**Repro.** Single-row `CHOICE: 1 (Create vampire ...)` under `PLAN: ... this window: pass`: `123
vs162` seq 29, `123 vs126` seq 37/44, `123 vs125` seq 15 (one token each). `123 vs126` seq **47**
(x17 at M 27 vs stop 24): NO PLAN line — nothing to compare. `146 vs125` seq **282** (2 life): PLAN
"Lolth's 0 ability is suicidal ... We must pass and hope", CHOICE `1 (Lolth 0)`, executed, 2 -> 1.
**Ask.** (a) the conflict covers the single `[cost: Tap]` row when the PLAN says pass; (b) a
repeat-row reply with no PLAN line -> `plan_missing` re-ask; (c) the PLAN-side pattern gains
`\bmust pass\b|\bwe pass\b|answer is pass`; (d) the `decision_reversed_in_prose` counter (wave-50
D20) ships so the class is measured. PARSETEST: the seq 282 reply (expect re-ask), the seq 47 reply
(expect `plan_missing`).
**Wave-52 prediction.** Executed takes whose PLAN says pass in any of the three forms without a
re-ask: **0/N** (was 5).

## D15 — MED — -N/-N spell target rows print `(P/T)` but no `{right now: -1/-1 - SURVIVES (toughness T)}`
(= seat-123-130 **M1** / deck123 R1 / **R119**) `123 vs152` seq 11 (`Elite Spellbinder (4/2)`, "it is
3/1"), `123 vs130` seq 21 (`Siege-Gang Commander #2 (2/2)`) — both lived. Ask: lane E's clause from
the printed toughness on Tragic Slip-class rows.
**Wave-52 prediction.** -N/-N takes at a survivor with the clause rendered: 0 (was 2).

## D16 — MED — fetch rows do not say the Flats makes no mana
(= seat-123-130 **M4** / deck123 R3 / **R120**; skill #166) `1787864168-ai_baka_deck123-0x55ebcae15190-
vs-ai_baka_deck130.jsonl` seq **6-13**: six passes with "when I have 3 mana" in the plan and `Mana
available: 2`. Ask: `{this land makes no mana until sacrificed; taking this now raises your
untapped sources from N to N+1}`.
**Wave-52 prediction.** Fetch rows passed while a hand spell needs the source: 0 (was 6).

## D17 — MED — the `{spends K of your M ...; <card> in your hand needs K}` clause on own-turn CAST rows whose `leaves N` is below the cheapest instant in hand
(= deck125 G1 / **R121**; skill #155) `1787864167-ai_baka_deck125-...-vs-ai_baka_deck152.jsonl` seq
**24** `Cast Staff of Nin {6} {leaves 1 of your 7}` with Cancel in hand; `125 vs162` seq 24 the same
with Fracture + Gavel + Scatter — the next opponent turn resolved three creatures / Ob Nixilis +
Mine #2 with no window. Emitter path: lane E's ability-row clause (`125 vs130` seq 8).
**Wave-52 prediction.** Staff / Elixir / Revelation taken below the cheapest counter's price with
the clause rendered: 0 (was 2).

## D18 — LOW — cast rows paid entirely from a floating pool print no `{leaves N}` clause
(= seat-125-126 **LOW #5** / deck126 G4 / **R122**) `126 vs152` seq 12, 19. Ask: `{leaves 0 untapped
sources and N floating}`.

## D19 — LOW — receipts without a payee: 7 `- Paid {1}{w} with ...` with no `for <card>`
(= engine seat **D-6** / **R124**) `152 vs123` seq 35; `152 vs125` seq 36 ×2, 48; `152 vs162` seq 10;
`152 vs146` seq 20 — the Intrepid Adversary ETB pay-N-times cost. Ask: `for Intrepid Adversary's
ability`.

## D20 — LOW — duplicate identical rows without `#N` on assignment / hand-reveal / library asks
(= engine seat **D-7** / **R125**) `146 vs126` seq 52 (two `Perimeter Captain (0/4) [defender]`);
`152 vs126` seq 25-28, 10; `146 vs125` seq 61 (`Plains [land] [your library]` ×4).

## D21 — LOW — pass replies stamped with name notes
(= engine seat **D-8** / **R127**) `146 vs125` seq 160, `123 vs146` seq 15: `CHOICE: 0 (pass)` ->
`named_row_not_offered`. Executed pass; corpus-count noise.

## D22 — LOW-MED — an exit on the X menu (second corpus; engine ruling on cancel semantics still needed)
(= wave-50 D25 carried / seat-123-130 **L1** / deck130 R4 / **R126**) `1787864168-ai_baka_deck130-
0x55ebcc135bf0-vs-ai_baka_deck123.jsonl` seq **38-40**: `Cast Card Normally` on `even at X=4 NOTHING
the OPPONENT controls dies`, then `X = 0` — the card spent for 0 damage. Ask: `Decline - do nothing`
on the X menu, gated on whether the card can return to hand after `Cast Card Normally`.

## D23 — MEASUREMENT / DECISION — the game-log window is now the WHOLE 30 KB overrun: 3 prompts > 30,000, 102 above wave 50's max, 96 of them in one 80-turn game at 71-82% log
(= engine seat watch / seat-146 E-6 / deck146 G-4 / **R132**; wave-50 D30 continued)
`1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_deck125.jsonl` seq **279** (33,536; 23.7 KB
log), **235-240** (30.5-30.9 KB, 77-82% log). Per-kind means up 2-19% on that game alone; discard
p90 latency 54 -> 123 s. Latency did not fall with wave 50's smaller prompts and did not rise in
step with these; the three longest replies at deck130 are ROW-driven (D8). Decision for the
owner: a turn-bounded or size-bounded older-turn compaction is the only lever left on the log
side; D8 ships first and the log share is re-measured with it in. No change asked this wave.

## D24 — LOW-MED — CARRIED with a failing window: the edict row's named victim carries its tags (wave-50 D12)
(= deck126 F22 / **R123**) `1787864176-ai_baka_deck126-0x563202380060-vs-ai_baka_deck162.jsonl` seq
**17**: `Fog Bank is sacrificed, you gain 2` taken at 13 life over a printed Bond row — the
`[defender]` the guide's carve-out keys on was not on the row. Ask unchanged: `Fog Bank
[defender] (0/2) is sacrificed`.

## D25 — LOW — CARRIED, re-measured not re-derived: wave-50 D13 (creature converter tag; 0 windows), D14 (opponent-draws pricing; 0 windows), D15 (discard list cost/type — 31 asks, bare names, D3 rode on it), D19 (`rejected_line_skipped`; no window), D21 (repeat row inputs: `123 vs162` seq 15 counted Shield Sphere as 0), D22 (enters-tapped; 0 breaks), D23 (Lightmine header; 0 windows), D24 (GANG BLOCK punisher: `123 vs146` seq 14 declined, the guide now ported), D27 (stale state block: `146 vs125` seq 162-167), D28 (damage-order ask: `146 vs126` seq 52 answered "Wall of Omens", lane C recovered it), D29 (singular), D32 residuals (R66 owner-side LOOP wording; R70 counter-target size — 0 spreading second corpus -> CLOSE; cycling tag wording R101; `REVEAL_DBG` compile-time gate; `parseBlockAssignments` re-scan count)

## D26 — LOW — new LOW renders, one line each: a per-window `WALL CHECK: N of your attackers meet a lifegain blocker; blocking all of them costs you 2N life` above the A-rows (deck130 R2; guide first) · a self-life-loss ability row's `{right now: you go to N}` (deck146 G-1; `146 vs125` seq 269/282) · chooser-attributed narration `You chose the sacrifice for their Tribute to Hunger: Emrakul` (deck125 G3; `125 vs123` seq 53) · the Caress-only `{feeds:}` K carries `(converts only past 7 cards in hand; their hand: N)` (deck162 G-1) · the off-menu `[RE-ASK]` prints the unpayable pip (deck126 G5; wave-50 D3's WHY, still open) · the X-menu header carries the seat's life when at or below the largest attacking power (deck125 G2) · lane C's genuine-echo exemption noise (`stale_echo_in_range` on a `CHOICE: 0 (pass)`).

## D27 — MED (frontend / human seat) — CARRIED wave-50 D31 / wave-49 D26: Arena-style grouping + kill the O(n^2) RenderSpell scan — OWNER ITEM, unchanged
This corpus produced a 28-Vampire board (`123 vs126`), 26-row equip menus (D8) and a 33-row
Upkeep Staff-ping window (`126 vs123` seq 23); the human-seat pile problem remains reachable.

---

# SHIPPED OUT-OF-WAVE — recorded, NOT docketed
| change | commit | LLM-visible? | verify |
|---|---|---|---|
| Ability borders priced by strict potential + planner | ca8a71e14 | no | suite only; no window |
| Morph Face-Up ability reaped after the flip; oracle skips superseded `->next` | dd8663146 | narration only if a morph is in the pool (none) | by string if a morph deck rotates in |
| Lanes G (D1-D3) and H (D4) | in flight | yes | the docket's predictions |

---

# DISCHARGED — SET B: wave-51 engine lanes, by the emitter's actual string

| lane / commit | prediction | verdict | counts |
|---|---|---|---|
| **A 42f2eff2b** (D1 card script) edict resolutions with no sacrifice 0/N | **PASS 0/15** | 15 `You cast <Soul Shatter / Tribute / Devour>` blocks, every one followed by a victim line; `146 vs162` seq 41 `Opponent used: sacrifice a creature or planeswalker with Soul Shatter targeting Ob Nixilis`; `146 vs125` seq 171 Emrakul (non-targeting edict vs protection — correct CR) | Flare of Malice / Riveteers Charm never cast |
| **A** walkers sacrificed when highest | **PASS 2/2** | `146 vs162` seq 40 -> 41 Ob Nixilis (MV 5); `146 vs126` seq 43 -> 44 Sorin (MV 4) | |
| **B df8ff7641** (D2) multi-colour source spent while a mono of an unneeded colour sat untapped 0/N | **PASS 1/421 plannable** (was 69/386) | exhaustive subset check over 443 receipts; 14 `{X}`, 8 creature/foreach sources excluded; **0 over-taps**; the one land case `152 vs125` seq 39 (`Lair {g}; Hengegate {w}` idle); 4 deck152 "suboptimal" = the planner correctly preferring lands over Katilda-granted creature mana; seat-146's 0/110 and seat-125-126's 0/136 concur | **D4** is a different defect (variable-source count) |
| **C 4cf3745a7** (D3) in-range index + off-menu name -> `named_row_reask`, never `stale_echo` to Baka | **PASS 2/2 re-asked + recovered; `stale_echo` 0** | `123 vs130` seq 25 -> 26; `126 vs146` seq 22 -> 23; a third (index past menu, `146 vs126` seq 52) also recovered | **D6** is the sibling (name ON the menu) |
| **C** (D4) `x0` = pass; `ran 1 time (you named 0)` 0 | **UNTESTED / PASS 0** | 0 `x0` written; repeat offered 6, taken 3 | **D1** interrupted one |
| **C** CHOICE-vs-PLAN conflict re-ask | **UNTESTED; scope short** | 0 renders; the over-take had no PLAN line; single-row and prose forms uncaught | **D14** |
| **D ac7c5f5be** (D5) `- Paid` on every activation N/N | **PASS 53/53 observable** | 55 priority takes with `[cost: {`; 2 followed directly by `gameend`; Hive receipts 4 sources | |
| **D** `with no source tapped` count | **0** | events, prompts, stderr | |
| **D** (D18) `turn` == narration | **PASS 2,070/2,070** | | |
| **D** (D10) edict clause victim == narration | **PASS 6/6** | tie / 0-board forms 0 renders — UNTESTED | |
| **D** (D17) `; ` separator | **PASS** | | |
| **E 9c772cd3d** (D6) ability-damage takes at toughness/loyalty > N: 0 | **rendered 428/428; FAIL 4/12 takes (2 deliberate two-steps, 2 Spark Spray sub-menus)** | `130 vs126` seq 37 (second shot killed), 49 (wasted at 3 life) | deck130 D51-2 |
| **E** (D7) Upkeep animations followed by `ATTACK: none`: 0 | **PASS 0/35 Hive (by avoidance); Lair 1/35 taken then tapped for mana** | annotation lives in `prompt` only (appended after `{card text:}`) | **D12**, **D13** |
| **E** (D8) dead Tutor / second-copy casts with the tag: 0 | **rendered 139; 21 takes — 15 stackable (the plan), 2 legendary (binned), 4 Tutors** | the tag conflates | **D10** |
| **E** (D9) Hammer-class returns with `{spends K ...}`: 0 | **PASS 0/141; 0/2 Hammer** | | |
| **F 7c0a04d1d** (D11) `{feeds:` on every draw-engine row | **PASS 78/78** | four shapes | |
| **F** deck162 engine casts at `converters 0` with opp 0 creatures: 0 | **FAIL 3/42 by the literal (2 first-Mine exemptions, 1 break)** | 162-G | |
| **F** (D16) bare `// <Name>"}` 0 | **PASS 0** | `(text omitted)` 278 | |
| carry: max priority windows/turn <= 10 | **PASS 10** | | |
| carry: Baka fallbacks <= 0.31% | **PASS 0.23%** (Baka-executed 2) | | |
| carry: `mana_only_windows_skipped` <= 1,000 | **PASS 161** | | |
| carry: no prompt > 30,000 | **FAIL 3** — all one 80-turn game, 71-78% log | **D23** | |

# DISCHARGED — SET A: wave-50 guide edits + boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (2-4) | A50-1 answer-first: conflicts 0/4, one take at M >= stop with NO PLAN line; A50-4 entry-0 **0/2 FAIL** (third corpus -> dictated plan sentence); A50-5 Tutor **FAIL 2 + 1 engine**; second Alarm 1; A50-9 cleanup 4/4 sane; Flats passed 6 windows (A51-1); `[from exile]` narrated falsely 0/4 (A51-3); Slip at toughness 2 twice; GANG-BLOCK kill declined (no rule -> ported); own-turn chains 2/2 won (A51-13) | D1, D2, D5, D6, D8, D11, D14, D15, D16 |
| **125** (2-4) | W52 converter half PASS 0, edict half FAIL 1 (the failing cast WON -> W29); W53 sweeper-at-0 **38/38**; W54 tap-out gate **FAIL 2 + 1 W22-legal that cost a game** (W27 reverts the exception); W55 PASS; W56 cleanup 0/15 counters sent wrongly, #143 3/15 letter / 1/15 intent; W57 land 47/47, X=3 FAIL 2/5 (X=1/2 casts -> W26); Path took Cathar over Sigarda (W28); Revelation at 1 life not cast (W30); Caress cost 8 life in cleanup (W31) | D17 |
| **126** (4-2) | D51 Tribute first window **1/1**; D52 defender victim **FAIL 1/1** (F22); D53 UNTESTED (0 discards); D54 re-ask 1/1 recovered; D55 Lantern-over-wall FAIL 2 (fourth corpus, all wins -> F21 re-derived); D56 blocks PASS; D57 PASS 0 + 0/12; Battlement over-tap cost 18 (engine); `[from exile]` 248 s; Tutor with the missing half in hand (F23) | D4, D10, D11, D18, D24 |
| **130** (5-1) | D50-2 UNTESTED; D50-4 X-menu FAIL 1 (`X = 0` after `Cast Card Normally`); D50-5 silent-family sentence FALSE all corpus (D51-2); D50-7 wall attacks **FAIL 2/2** (#132 replay FAIL; the floor outranked the check -> D51-1); D50-8 FAIL 1+2; D50-1 Hammer **PASS 0/2** with lane E's clause; D50-12 Talisman floor **FAIL 3** (the last lethal); Blastminer held 7 windows vs an empty board (D51-6); own-Commander Hammer pick (D51-5) | D7, D8, D15, D22 |
| **146** (3-3) | 146-H PUT half A PASS, half B 1/4; 146-K Bond target **1/1**; Hive Upkeep 0/32 (146-C holds) but `[repeat:` re-offer taken 3/9; Soul Shatter 6/6 incl. two walkers (B2 confirmed); mull-to-2 chain on the colour clause (146-M cut); Kaya self-exile at 1 life (146-N); Silencer named a card already on their line (146-O); Lolth 0 at 3 and 2 with "we must pass" (E-4) | D1-adjacent, D9, D12, D14, D20 |
| **152** (3-3) | 152-A both-halves `ATTACK: none` **1/1** (first real window, 37 life); 152-E Lair TAPPED/repeat 0/0, Upkeep tag **1/35** then tapped for mana; 152-F Katilda B-row PASS 0/1; Katilda A-row FAIL 1 (152-G); deploy floor 0 breaks; Spellbinder "only answer" card-fact overstated (152-H); lane B 1 land case | D13, D19, D20 |
| **162** (2-4) | RULE 1 **15/15**; RULE 2 vs `{feeds:` K: 1 break + 4 over-brake windows (162-G re-keys to the string); 162-D 0 windows; 162-E Caress 1/1; PUT mis-executed by the engine (E-1 -> 162-H interim); three no-window losses | D3 |
| **pool** | wave-50 boundary B1 (cleanup orders at 130/146/152/162): written; 130 had 0 asks (UNTESTABLE second corpus), 146 10 in order but one (146-P), 152/162 in order — PASS; B2 (deck146 Soul Shatter keyed to Oracle): confirmed 6/6 — PASS; B3 (D50-5 cut the wave D6 ships): NOT done at revision time — the sentence stayed false a corpus (skill #154); B4 (Caress count words vs `{feeds:}`): re-keyed 162-G — PASS; B5 (Lair/Hive re-key to the annotation string): NOT done at revision time, first Upkeep take under the tag happened (skill #154); B6 (Scatter `[creature]` stack tag): overtaken — W27 makes the Scatter count against every opponent; B7 (A50-1 `x0` line): left, not false — PASS | |

---

# CARRIED — open, re-measured, not re-derived
See D22-D27. The wave-50 OPEN questions: "does the repeat row get taken" — 3/6, every counted take
executed exactly except the one the ENGINE interrupted (D1); "the ~800 s ceiling" — max 849 s, a
complete correct self-revision; the floating-mana class — RETURNED on a different path (13 {g}
floating after a planned Battlement payment, D4); the double-tap — 0 over-taps in 421 plannable
receipts, the receipt now the standing regression detector for both.

---

# FALLBACK CLASSES — 5 / 2,129 = 0.23%; Baka-executed 2

| n | class | records | disposition |
|---|---|---|---|
| 3 | `named_row_reask` (off-menu name in range / index past menu) — **all recovered** | `deck123-vs-deck130` seq 25; `deck126-vs-deck146` seq 22; `deck146-vs-deck126` seq 52 | lane C working; the `[RE-ASK]`'s WHY still LOW (D26) |
| 1 | **`stale_livelock`** (NEW) — Baka cast an edict mid-repeat | `deck123-vs-deck126` seq 48 | **D1**, lane G |
| 1 | `unparsed_reply` — the pass-row echo ambiguous against annotations | `deck123-vs-deck125` seq 49 | **D2**, lane G |
| 0 | `stale_echo` executed (was 3) | | lane C paid |
| 0 | garbage decode / exemplar / `CHOICE: 0` on a no-pass ask | | lane Y holds, untested |
| — | **not a fallback but executed wrong**: `echo_index_conflict_ambiguous` (`deck123-vs-deck125` seq 48, dead Tutor); PUT executed row 2 (`deck162-vs-deck146` seq 17, no fallback field) | | **D2/D6**, **D3** |

---

# PROMPT CHARS PER DECISION KIND — wave 50 -> wave 51 (ALL games, 21/21)

| kind | n50 | mean50 | max50 | n51 | mean51 | max51 | p90 latency 50 -> 51 (s) | max lat 51 |
|---|---|---|---|---|---|---|---|---|
| ask | 1,214 | 9,102 | 25,018 | 1,493 | 10,418 | 29,355 | 56.2 -> 56.8 | 849.5 |
| priority | 376 | 12,038 | 25,793 | 445 | 14,302 | 33,536 | 54.8 -> 59.3 | 602.0 |
| attackers | 102 | 11,227 | 25,087 | 110 | 11,324 | 26,911 | 52.4 -> 62.9 | 308.7 |
| blockers | 37 | 11,268 | 25,450 | 31 | 10,374 | 20,779 | 67.5 -> 79.1 | 171.4 |
| discard | 28 | 9,534 | 25,402 | 31 | 12,506 | 27,450 | 54.4 -> 123.0 | 477.6 |
| reveal | 15 | 14,049 | 20,689 | 13 | 14,544 | 19,789 | 42.5 -> 43.2 | 59.7 |
| bottom | 8 | 2,200 | 2,542 | 6 | 2,266 | 2,469 | 78.3 -> 70.2 | 81.2 |

Means up 2-19% (priority +19%): the 80-turn game alone contributes 291 asks at 57-82% log; 102
prompts exceed wave 50's max, 96 in `deck146-vs-deck125`. Mean log share ask 57% / priority 61% /
attackers 61% / discard 61% (wave 50: 54/58/62). The largest non-log prompt (29,469) is ROW-driven
(D8). Latency is reasoning-bound; the deck130 outliers are row-count-bound (D8).

---

# LATEGAME SPECIMEN — invariant 0 (for the owner's per-wave prompt review)

**Primary (the long game):** `1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_deck125.jsonl`
seq **279** (33,536-char priority window at turn 78 of 80: 23.7 KB log, 11 loyalty rows incl. three
Kaya self-exile rows with no self-hit tag — D9, the stack with Path over Silverquill Command, a
hand of 4 Vanishing Verse + 2 Soul Shatter with no targets) paired with seq **235-240** (30.5-30.9
KB, 77-82% log — D23) and seq **160-181** (Emrakul's arrival, the six annihilator asks with the
stale state block — D25/D27, the seq 170 `{right now: they sacrifice Emrakul (MV 15, their
highest)}` cast, Kaya's emblem recasting her every upkeep).
**Repeat machinery + livelock:** `1787864189-ai_baka_deck123-0x5593ba17b190-vs-ai_baka_deck126.jsonl`
seq **44-51** (the 35-row equip fan-out — D8; the x17 take; the `stale_livelock` record with its
2-row menu; the self-targeted Devour — D1).
**Echo/index conflict:** `1787864180-ai_baka_deck123-0x5611df0fea40-vs-ai_baka_deck125.jsonl` seq
**48-50** (D2/D6) and seq **111-112** (the Emrakul turn with no opponent-turn window — D5).
**Planner over-tap:** `1787864171-ai_baka_deck126-0x564fc96be4a0-vs-ai_baka_deck152.jsonl` seq
**18-19**, **26-27**, **30-31** (D4) and seq **26**'s 248-s `[from exile]` deliberation (D11).
**The lethal Talisman:** `1787864160-ai_baka_deck130-0x556e1bc31180-vs-ai_baka_deck126.jsonl` seq
**82-86** (D7), with seq **20** (the four-attacker wall window, every tag correct and overridden —
the render at its most complete, read and ignored; the guide item).
**The mis-parsed PUT:** `1787864179-ai_baka_deck162-0x55a817091b20-vs-ai_baka_deck146.jsonl` seq
**17-18** (D3).
