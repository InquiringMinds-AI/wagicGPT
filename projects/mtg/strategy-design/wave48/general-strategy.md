# Wave-48 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R27 revised, R28-R47 new)

Corpus `matchups-20260827-072004` (fifth fair-hand corpus), binary master **eccf8284c** (E1
5ce8f8a5a + lanes P 9b19fbfe6 / O 96a07e864 / N 7a858904d / Q 7246c4a95). 42 seat logs,
**1,729 decisions** (ask 1,240 / priority 364 / attackers 71 / blockers 26 / reveal 20 / bottom
8), **21/21 games natural** — every seat carries a `gameend`; longest seat 133 records. Wave 47
was 5,726 decisions with one seat at 1,921 records and no gameend. Sources: the four seat files,
seven `deckN/general-suggestions.md`, seven `deckN/skill.md`.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs —
is the purpose. Everything below is a change to what the ENGINE RENDERS or to the CORE PROMPT's
own text. Guide-side answers are named only where a rule must stay correct AFTER the render
ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-49 docket); this file is
the ranked routing view with the evidence and the wave-49 falsifier for each item.

**Governing constraint on every item.** Legality is enforced structurally; the model's CHOICE
is never constrained. Every proposal ADDS a true token, a true row, or one re-ask, and DELETES
nothing. Hard caps are rejected (R1's rejection stands and was vindicated: the loop closed by
affordance, 1,871 -> 12 windows, with the row still on the screen).

---

## STATUS OF R1-R27 (wave-47 items), by the emitter's actual string

| item | wave-48 verdict | evidence | residual |
|---|---|---|---|
| **R1** loop stopping affordance (lane N: F1 repeat-N row, F3 real `0. Pass priority` LAST row, F2 tag states both scopes) | **SHIPPED, PASS on every prediction** | max priority windows in one turn **9** (was 1,871); the three deck123 token games closed in **17 / 12 / 8** priority records (was 1,550 / 1,871 / 223); 21/21 natural; pass row on **139/139** single-option windows, chosen 0 in **75** (a decline that was 0/1,871 is now 54%); the repeat row was offered 14 times and **TAKEN 12** (8 with a count the engine executed exactly: `x50` x5, `x15`); tag max 203 = 3 singles + 4x50 (reconcilable) | the count grammar: `N=45` / omitted N -> silent single activation, 4 windows (**R30**, wave-49 D3) |
| **R2** A-row ranged collapse on both combat screens + range grammar | **SHIPPED, PASS bar one prompt** | max attackers prompt **24,153** (was 233,662); `A3-A257. Human #1-#255 (1/1) ... x255` answered `ATTACK: A1, A2, A3-A257` in 3 s; attackers p90 **50.6 s**; blockers p90 56.1 s | the B-row `may block A1 (...), A3 (...) ... A257 (...)` list is unranged and unfactored: **61,240 chars** (**R28**, wave-49 D1); same-name rows straddling a `#n` sort split (2/71, **R41**) |
| **R3** target-menu collapse + card text once per source | **SHIPPED (numbered-option ranges, 50 prompts), scale UNTESTED** | largest Staff target menu this corpus 11 rows; a choice landed inside a collapsed range 2 times, both accepted | none docketed; re-measure when a 100+ row target menu recurs |
| **R4** `0 of them able to attack right now` scope falsehood | **SHIPPED, PASS 26/26** | `257 of them are attacking right now` over 257 `[tapped - attacking]` rows | the SECOND bucket double-counts a `[vigilance] [attacking]` body: `2 attacking, 3 more able` on a line with two non-attackers (**R37**, wave-49 D9) |
| **R5** collapsed blocker tag adds the PRICE-selected representative | **SHIPPED, PASS (thin: 2 collapsed tags, 1 `most expensive to attack into:`)** | `146 vs126` s72: the clause named Pride Guardian and the reply named Pride Guardian — then attacked anyway (guide, 146-A) | none; keep |
| **R6** `{card text:}` clause-aware budget | **SHIPPED, PASS 0/2,077 mid-word cuts** | 13 tails end `...` after a full stop | **118** blocks end UNMARKED on a bare back-face name (`// Agadeem, the Undercrypt` 44, `// Pelakka Caverns` 30, `// Lord of Lineage` 12 ...) — an unmarked truncation (**R36**, wave-49 D12) |
| **R7** `auto=choice` header names its card; `target controller` -> `you` | **SHIPPED, PASS 0 / 0** | `Choose an option for Barkchannel Pathway:` 57 renders; `{right now: if you choose "the opponent": life -6 ...}` | none |
| **R8** stack line / counter-target clause keyword tags | **SHIPPED, PASS 0/33** | 33 windows offered a counter on a `{target text: "Defender ...` object; chosen `Cast nothing` or a non-wall 33/33; `[defender]` on 5 stack rows at deck125 | deck125's negative-existence sentence retired (W11) — boundary B3 closed |
| **R9** `[DRAW PRICE:]` reaching draw rows | **PARTIAL** | 11 tagged prompts (cycling, Wall of Omens, Dream Fracture); 0 draws at any tagged row | three uncovered classes: `draw:X` rows (Sphinx's Revelation, `deck125 vs162` seq 38/41), activated-ability draws (`Draw 1 with Clue`, `152 vs162` s28/29, cracked at 8 life), and opposing cast-triggers (Forced Fruition: seven draws per spell, `deck125 vs162` seq 44/51, the game) (**R31**, wave-49 D6) |
| **R10** ANNOUNCE_X: no carried plan, maximal-row marker | **SHIPPED, PASS 0/28 plan blocks, marker on 4** | 3 of 4 marker windows chose the marked row | 1 of 4 took row 1 (X=6) with the marker on row 5 (X=2, same kills) — the marker sits where largest-first puts it LAST (**R45**, wave-49 D14) |
| **R11** "no PLAN where the plan cannot change" as a builder rule | **HOLDS at both shipped sites, extended by R34** | pregame 0 plan blocks; land asks 392/392 with 0 declines on three seats, 84/84, 168/168, 131/131, 93/93 | the in-game carried plan is IMMORTAL when it names no action: 48 windows (**R34**, wave-49 D7) |
| **R12** converter PAIR loop clause | **NOT SHIPPED — NOW A LOST GAME** | `152 vs126` s25: both `{effect:}` texts on the battlefield line, block names Sanguine Bond alone, `ATTACK: A1` (7-power flier) -> **20 -> 0 in one combat**, opponent 14 -> 32 | promoted to HIGH (**R12**, wave-49 D5) |
| **R13** GAME LOG repetition collapse | **SHIPPED** | `created 50 1/1 Human tokens`; `Your Goblin dealt 1 damage to Fate Unraveler (x3)` (11 prompts); priority mean **23,546 -> 11,331** chars | none |
| **R14** battlefield collapse by identical text | **SHIPPED** | `Perimeter Captain #1-#3 ... x3`, `Human #1-#255` | the option/A-row/B-row lists still sort lexicographically on `name #n`, so `#1`/`#2` of one name straddle a different card (**R41**) |
| **R15** `{leaves N ...}` on activated-ability rows | **CARRIED, now with a consequence** | `Life with Elixir of Immortality [cost: {2}, Tap]` took deck125 to 0 open before an uncontested Underworld Dreams (`vs162` seq 29); 4 Elixir activations to 0-2 open | LOW, D18 |
| **R16** cast ask then priority ask answered as one menu; explicit range | **CARRIED, evidence grew** | 4 of 5 fallbacks this corpus are an index past the menu with a name NOT on it (Acererak on a 2-row priority window right after a cast ask; Rorix; Fate Unraveler; `Attack with all creatures`) | (**R35**, wave-49 D8: one re-ask + the range tail) |
| **R17** bare `Cast nothing right now` | **CARRIED, one new shape** | `deck123 vs126` seq 29: `CHOICE: 5 (Attack with all creatures)` on a 4-row Main-1 cast menu with 257 creatures — the exit row says nothing about leading to combat | fold `(moves on to combat)` into R35 |
| **R18** DFC Pathway land route | **HEADER SHIPPED (lane Q), tail UNTESTED** | `Choose an option for Barkchannel Pathway:` 57 renders; no seat measured the PLAN-line/consequence tail | LOW, carried |
| **R19** attacker rows state the upside, never the cost | **CARRIED, family widened** | the cost of an action includes what paying it TAPS: `becomes beholder` on a `[tapped]` Hive whose payment tapped the other Hive (`146 vs125` s64), `casting this taps you out` when the sources were the attackers (`152 vs162` s24, lethal stayed home) | (**R38**, wave-49 D11) |
| **R20** degenerate-decode counter | **CARRIED, 0 events this corpus** | the 800 s ceiling recurred twice (801.9 s, 590.7 s) with COMPLETE replies — not decodes | LOW |
| **R21** fuzzy counters | **CARRIED** | `mana_only_windows_skipped` max **835** (was 8,610), 0 leaks | LOW |
| **R22** `dropped_assignments` pads | **CARRIED, 0 this corpus** | | LOW |
| **R23** cycling row `[cost:]` bracket | **UNTESTED** | 12 `Choose an option for` menus at deck130, 12/12 correct | LOW |
| **R24** `manaAvailableLine` number word above ten | **CARRIED, third wave: 85 emissions** | `Mana available: 11 total (untapped sources, ...` at deck130 vs125 turns 20+ | LOW |
| **R25** `{right now:}` per-branch preview generalised | **CARRIED, three new callers** | the sweeper row (deck125's 4/8 casts into empty/all-defender boards, FIVE waves of prose), the edict row (deck123's RULE 5, FOURTH corpus), the draw-engine row (deck162's Rule 2, third corpus) | promoted to MED as a family (**R33**, **R39**, **R47**) |
| **R26** LETHAL blockers window castability fact | **CARRIED, one adjacent instance** | `deck123 vs152` seq 26 at LETHAL: "I have no way to block" under a printed `B1. ... may block A1` row (STRATEGY, A48-4) — not castability | LOW |
| **R27** one keyword, three wordings | **CARRIED, now with a fallback behind it** | `deck126 vs146` seq 24 single-blocked two menace Spiders under the explicit A-line -> `all_assignments_illegal` -> Baka with no re-ask (**R43**, wave-49 D15) | LOW/MED |

---

## HIGH

### R28 — the B-row `may block` list is the last uncollapsed enumeration on the combat screens. (= ledger **D1**; seat-engine **D-1**)
`deck126 vs deck123` seq **15**, blockers, **61,240 chars**, three B-rows. The A side collapsed
perfectly (`A3-A257. Human #1-#255 (1/1) deals 1 [...] x255`); each B-row's clause enumerates
`may block A1 (neither dies (blocking trigger: you gain 3, and your converter takes 3 off
them)), A3 (neither dies (...)), ... A257 (...)` — **256 handles, ONE distinct parenthetical**,
22,926 chars for B1 alone, repeated on B2 and B3. Every other prompt in the corpus is <= 27,987.
Two independent collapses, either sufficient: (a) emit ranges over consecutive handles with an
identical parenthetical (`may block A1 (...), A3-A257 (neither dies (...))`) — takes this prompt
to ~5 KB; (b) factor the parenthetical when every entry's is identical (`may block A1, A3-A257
(all: neither dies (...))`). Secondary: `B1. Pride Guardian #1` and `B3. Pride Guardian #2` are
identical rows split by `B2. Overgrown Battlement` (R41's sort straddle).
**Wave-49 prediction.** No prompt of any kind **> 30,000** chars (was 61,240; next-largest
27,987); blockers max **< 15,000**.

### R29 — the E1 replacement executes a re-answer whose parenthetical NAME does not match the option at that index, and does not cover `ATTACK:`/`BLOCKS:` lines at all. (= ledger **D2**; seat-125-126 **HIGH #1**, seat-146-152-162 **E-1**)
E1 paid: 21 `answer_replaced` records, 21/21 resolved to the model's last coded `CHOICE:`,
`retracted_choice` 0 (was 2), and `deck123 vs126` seq 25 exited the loop by exactly the shape
E1 was built for (line 1 the repeat row, last line `CHOICE: 0 (pass)`). Two holes:
(a) `deck126 vs130` seq **25**: first line `CHOICE: 2 (Cast nothing right now)` citing the
guide's zero stop; re-answer `CHOICE: 1 (Cast Sanguine Bond)` — a card nowhere on the 2-row
menu; index 1 = Tribute to Hunger, executed at a `0 are creatures` header: nothing sacrificed,
tapped out. The short name is requested precisely so the index can be checked.
(b) `deck146 vs126` attackers seq **48**: reply opens `ATTACK: A1, A2`, deliberates 3,000 chars,
concludes "So ATTACK: A1 is correct. Do not sacrifice Spider", ends `ATTACK: A1`. The engine
sent BOTH (`chosen_text: "Silverquill Silencer, Spider"`); the Spider died and the seat paid 4.
The record reads `answer_replaced: true` — the field says the opposite of what happened.
Ask: (a) on a name/index mismatch refuse the replacement — keep the earlier coded answer whose
name matches, else fallback — and COUNT it (`named_row_not_offered`); the same check
re-classifies this corpus's four `unparsed_reply` fallbacks (Acererak, Rorix, Fate Unraveler,
"Attack with all creatures" — every one an out-of-range index naming a card not on the menu) as
a model-belief failure distinct from a parse miss. (b) Apply the last-coded-line rule to
`ATTACK:` and `BLOCKS:`, or state in the attackers/blockers tail which line is taken, and never
set `answer_replaced` on a record whose executed answer is the first line.
**Wave-49 prediction.** Replacements executing an index whose parenthetical names a row not on
the menu: **0**; `answer_replaced` records whose executed `ATTACK:`/`BLOCKS:` matches the FIRST
coded line: **0** (was 1); `named_row_not_offered` counted separately from `unparsed_reply`.

### R30 — the repeat-N row's count is parsed from one spelling, and a missing count runs once, silently. (= ledger **D3**; seat-engine **D-2**, seat-123-130 **H1**)
`scanRepeatCountInLine` accepts `x50` / `x 50` / `50 times`. The row's own text says "you name
N on the CHOICE line". `deck123 vs162` seq 28 `..., repeated N times, then stop, N=17)` and
seq 29/31 + `deck123 vs126` seq 22 (the row label echoed with no number) each ran ONE
activation with no receipt — the narration showed a single `created a 1/1 Human token` and the
pilot, which had computed 17 correctly, re-echoed. Three of four wrong spellings reproduced the
ROW's wording rather than its example. 4 of 12 takes; ~262 s of latency in one turn.
Ask: accept any integer after the short name (`x17`, `N=17`, `N = 17`, `17 times`, `, 17)`); on
no integer, re-ask once ("you chose the repeat row but named no N") or run once WITH a receipt
(`ran 1 time (you named no count)`) so the next window is informed. Deletes nothing.
**Wave-49 prediction.** `repeat_count_missing` **0/N**; every repeat-row take carries a receipt
with N >= 2 or a narrated "no count named".

### R12 — the converter PAIR is an unbounded loop and the block names one direction of it. (= ledger **D5**; seat-146-152-162 **E-5**; carried from wave 47, NOW A LOST GAME)
`152 vs126` seq **25**: battlefield line carries `Exquisite Blood {4}{b} [enchantment] {effect:
"Whenever an opponent loses life, you gain that much life."}` AND `Sanguine Bond {3}{b}{b}
[enchantment] {effect: "Whenever you gain life, target opponent loses that much life."}`; the
block reads `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond. While it is
in play, life ITS CONTROLLER gains also makes the other player lose that much life ...`. The
seat held every priced ground attacker (s20, s23 `ATTACK: none`), then sent the unpriced flier:
7 damage -> +7 -> -7 -> +7 ... **20 -> 0**. Its own PLAN named both cards and computed a finite
race. Primitives verified: `@lifelostfoeof(player):life:thatmuch controller` +
`@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`.
Ask, wave-47 text unchanged: when both halves are on one battlefield, the block says `both
halves of a life LOOP are on their battlefield (Sanguine Bond + Exquisite Blood): any life they
gain repeats without limit, so ANY nonzero payment on a tag is fatal, not merely expensive`,
and every A-row on that board carries the fold `(... and their loop makes any damage you deal
fatal)`. Companion, from deck146 (G-4, MED): the single-converter block gains one clause of
DIRECTION — `this is not a trade: their total goes UP by N and yours goes DOWN by N in the same
event` — because the pilot read the fold as a wash three times ("net 0"; skill #106).
**Wave-49 prediction.** Every prompt whose battlefield lines carry both halves prints the loop
clause; **0** `ATTACK:` answers other than `none` on such a board from any seat; **0** replies
compute "net 0" on a board with the direction clause printed.

### R31 — the draw price is a summary line, and three classes of row that incur it carry no number; nothing on the screen forecasts the NEXT draw step. (= ledger **D6**; deck123 R1, deck130 R1, deck125 G1, seat-146-152-162 **E-4**, seat-engine **D-6**)
Lane J's `DRAW PUNISHERS ... Every card YOU draw costs you N life` line is on 2,000+ prompts and
0 voluntary draws happened at a TAGGED row. Uncovered: (a) `draw:X` rows — `Cast Sphinx's
Revelation ... {X pricing: ... draws you 1 card}` under Fate Unraveler, untagged
(`scriptSelfDrawCount` returns 0 for a non-literal); (b) activated-ability draws — `1. Draw 1
with Clue [cost: {2}, Sacrifice]` under `... 2 life`, cracked at 8 life (`152 vs162` s29);
(c) opposing cast-triggers — `Forced Fruition {effect: "Whenever an opponent casts a spell,
that player draws seven cards."}` on their line: `Cast Elixir of Immortality {1}` at 15 life
drew 7 and cost 7 (`deck125 vs162` seq 44), `Cast Cancel` at 11 life drew 14 and killed the
seat (seq 51); deck130 read the same text as "causing THEM to draw 7" and cast Rorix at 1 life.
And the count of forced draws in the coming draw step is nowhere: `deck123 vs162` seq 35 (9
life) died to a 9-card draw step (Puzzle Box: hand size 7, + Howling Mine, + Dictate) at 2 per
card, with nothing on the screen stating K.
Ask: (a)/(b) the same emitter, two more sources (the `{X pricing:}` clause is the natural
carrier for `draw:X`; the Clue/Draw ability class is the same shape as cycling); (c) when an
opposing permanent's `auto=` matches `@movedTo(...|opponentstack):draw:N`, every `Cast ...` row
gets `[DRAW PRICE: casting this draws you N (their Forced Fruition), and their Underworld
Dreams deals you N]`; (d) one line beside the punisher line, computed from the primitives the
line already scans: `DRAW FORECAST: your next draw step draws K cards (1 + Howling Mine 1 +
Dictate of Kruphix 1 + Teferi's Puzzle Box: your hand size 7) = K x N life`.
**Wave-49 prediction.** `[DRAW PRICE:` on every `draw:X` row, every `Draw N with <token>` row
and every cast row under an opposing cast-draw trigger while a punisher line is printed (report
N); draws/casts at or below the summed cost **0/N**; 0 deaths in a draw step whose printed
K x N was at or above the pilot's life when a life-preserving action was on the previous menu.

### R32 — the automatic mana payment spends the scarce colour on a generic cost. (= ledger **D4**; seat-123-130 **H2**)
`deck123 vs162` seq 17 -> 19: `Idyllic Tutor {2}{w}` paid from `Underground Sea #1, Underground
Sea #2, Scrubland #3` — the deck's only two blue sources — when `Swamp, Scrubland, Scrubland`
paid it exactly. `{U} 0` after the cast; the just-tutored `Intruder Alarm {2}{u}` was absent
from the menu for a full turn cycle (`"cannot be cast this turn due to lack of blue mana"`), the
loop started on turn 13 instead of 11, and the game was lost by one turn. The pilot has no
tap-payment decision and cannot route around it. This is engine correctness in the payment
code, not a render item, and it sits in the same family as the three human-seat mana fixes
shipped 2026-08-27 (see the ledger's SHIPPED OUT-OF-WAVE row).
Ask: pay generic costs from sources whose colour set is a subset of the others' (mono-colour
first, most-flexible last). Companion render (deck123 R4, LOW): `{leaves 3 of your 6 untapped
mana sources untapped - {w}{b} only}` so the pilot can at least sequence around the result.
**Wave-49 prediction.** 0 casts whose payment taps a source producing colour C while a source
not producing C could have paid the same pip; report the count of `{U} 0`-after-cast windows
where a blue spell was in hand.

---

## MEDIUM

### R33 — the edict row needs the count it depends on, and the sweeper row needs what it would destroy: `{right now:}` for the two decisions five and four waves of prose have not closed. (= ledger **D10**; seat-123-130 **M2**, deck126 G3, deck125 G2)
deck123's RULE 5 fails a FOURTH corpus on the opponent's creature count: `Cast Tribute to
Hunger ... - legal targets right now: the opponent` was cast at `of which 0 are creatures` with
the creature ON THE STACK (`vs130` seq 22 -> resolved, nothing sacrificed, no life gained) and at
`of which 4` (`vs152` seq 18). deck126 cast the same spell at 0 creatures via the E1 hole. deck125's
sweeper rule fails 4/8 casts into empty or all-defender boards for the FIFTH wave (`vs130` seq
45/72 at `0 permanents`, `vs126` seq 46 into five walls, `vs162` seq 21). Both facts are one
header line away from the row; the guide lane is exhausted at both (skill #102 applied twice).
Ask, the R25 family: `Cast Tribute to Hunger ... {right now: they control 1 creature - Thraben
Doomsayer (2/2) is the one they must sacrifice; you would gain 2}` / `{right now: they control
0 creatures - this sacrifices nothing and gains you 0}` (the 0-branch is the zero stop the
guide keeps restating); `Cast Supreme Verdict ... {right now: destroys 0 of their creatures
(0 able to attack), 0 of yours}`; Lightmine Field `{right now: 0 creatures able to attack -
deals 0}`. Same emitter as `{X pricing:}` / `{leaves N}`.
**Wave-49 prediction.** Edict casts at N != 1 (deck123) and at N = 0 (any seat): **0/N**;
sweeper casts at a row reading `destroys 0`: **0/N** (was 4/8 + 2 edicts + 1).

### R34 — the carried plan is immortal when it names no action. (= ledger **D7**; seat-123-130 **M3**, deck130 R2, deck123 R3)
`deck130 vs125` seq **84-131**: `YOUR PLAN (as you last stated it): The game is lost. I have no
creatures, no removal ... Passing is the only legal action with no impact.` — 48 consecutive
windows. The reply protocol asks for a new PLAN "only if part of yours is now done or false"; a
verdict is never done and never false. Under it: seq 97 `Hammer of Bogardan` target menu
`1. The opponent (life 26) / 2. Yourself (life 9)` answered **`2 (Yourself)`**; seq 111-112
Starstorm at X=12 with the row reading `there is no creature on the battlefield for it to
damage`. This is R11's principle and the wave-46 pregame fix generalised to the in-game plan.
Ask: drop the plan block (and re-ask for a PLAN) when the carried plan contains no card name and
no option verb from the current menu, or after K identical verbatim echoes (K = 5). Guide side
(skill #115): a PLAN names the next action; a verdict is not a plan.
**Wave-49 prediction.** No plan block echoed verbatim more than 5 consecutive windows; damage
target answers of `Yourself`: **0**; PLAN lines containing "the game is lost": **0** (was 48).

### R35 — an index past the menu with a name not on it goes straight to Baka; the exit row does not say it leads to combat. (= ledger **D8**; seat-engine **D-3**, **E-6**; supersedes **R16**/**R17**)
4 of 5 fallbacks this corpus: `146 vs125` s80 `CHOICE: 3 (Cast Acererak the Archlich)` on a 2-row
priority window immediately after a cast ask; `130 vs126` s30 `CHOICE: 6 (Cast Rorix Bladewing)`
on 5 rows; `162 vs123` s10 `CHOICE: 3 (Cast Fate Unraveler)` on 2 rows (4-drop with 3 mana —
the engine correctly did not offer it); `123 vs126` s29 `CHOICE: 5 (Attack with all creatures)`
on a 4-row Main-1 cast menu with 257 creatures — the model wanted combat and the exit row
`4. Cast nothing right now` does not say it leads there. The parser was right to refuse every
one; the re-ask machinery exists for validation rejects and this class is the same.
Ask: (a) one re-ask on index > N with no name match — `"<name>" is not on this list; answer
with a number from 1 to N, or 0`; (b) `Cast nothing right now (moves on to combat)` when the
attack step is still ahead this turn; (c) R16's "these are ACTIVATED ABILITIES only; no card in
your hand appears below" on a priority ask that directly follows a cast ask.
**Wave-49 prediction.** Fallbacks from an index past the menu: **0/N** (was 4/5); corpus
fallback rate stays <= 0.31%.

### R36 — the `//` face budget drops the whole back face with no marker. (= ledger **D12**; seat-engine **D-4**)
118 `{card text:}` blocks end on a bare face name — `... a value X or less. // Agadeem, the
Undercrypt` (44), `... discards that card. // Pelakka Caverns` (30), `... five or more
Vampires. // Lord of Lineage` (12), `// Moonrage Brute` (11), pathways (16). The reader cannot
tell "the back face is a land with no text" from "the back face's text was cut". Lane Q's
mid-word prediction held (0/2,077); this is the unmarked half.
Ask: `// <face> (text omitted)` or at minimum `...` after the face name.
**Wave-49 prediction.** `{card text:}` blocks ending on a bare `// <name>`: **0/N** (was 118).

### R37 — lane P's blockers header double-counts a vigilance attacker. (= ledger **D9**; seat-123-130 **M1**, deck130 R3)
`deck123 vs152` seq **26**: `of which 4 are creatures, 2 of them are attacking right now, 3 more
able to attack right now` on a line holding Briarbridge Tracker `[vigilance] [attacking]`
(untapped), Brutal Cathar `[tapped - attacking]`, and two non-attackers. "More able" should be
2: an untapped vigilance attacker is `isAttacker()` AND passes the able-to-attack test. The
first bucket is right 26/26; the second must exclude attackers.
**Wave-49 prediction.** `N attacking, M more able` with N + M exceeding the creature count, or M
counting a `[attacking]` body: **0**.

### R38 — the cost of an action includes what paying it TAPS, and neither the activated-ability row nor the cast row says so. (= ledger **D11**; seat-146-152-162 **E-2**, **E-3**; deck152 G-2, G-4; deck146 G-2; widens **R19**)
Three shapes, one emitter family: (a) `becomes beholder with Hive of the Eye Tyrant #2 [cost:
{3}{b}]` was bare while the battlefield line read `Hive of the Eye Tyrant #2 [tapped]` and the
only black source was the OTHER, already-animated Hive; taken; both Hives ended `[tapped -
cannot attack or block this turn]`, eight mana, no attackers record (`146 vs125` s63-65).
(b) `Cast Briarbridge Tracker {2}{g} {leaves 0 of your 3 untapped mana sources untapped -
casting this taps you out}` when `Those sources` were `Intrepid Adversary {w}; Brutal Cathar
{w}` — the attackers; 11 lethal power stayed home (`152 vs162` s24). (c) `put 1/1 counters with
Katilda [cost: {4}{g}{w}, Tap]` taken in Upkeep 8 of 9 times; the row prints the cost and not
that Katilda will not attack. (d) R19's original: Dwarven Blastminer's `{T}` ability forfeited
by attacking, silent on the A-row.
Ask, restriction-first: on any row whose payment must include a creature, `{paying this taps:
Intrepid Adversary, Brutal Cathar - they cannot attack this turn}`; on a `becomes <creature>`
row for a tapped source, `[this land is TAPPED: animated, it still cannot attack this turn]`;
on a `Tap`-cost activation of a creature offered before combat, `{tapping Katilda now: she
cannot attack this turn}`; on an A-row for a creature with an untapped-only ability, `[attacking
taps it: you lose {2}{R},{T}: ... this turn]`. The mana line already holds the facts.
**Wave-49 prediction.** Creature-tapping casts in a window where the seat's untapped power was
lethal: **0**; `becomes` taken for a `[tapped]` source: **0**; report N for each clause.

### R39 — a draw-engine cast row could carry the punisher count the way `[DRAW PRICE:]` carries the draw cost. (= ledger **D18** carried; deck162 G-1, filed early per #86)
deck162's Rule 2 (no engine behind a punisher) is at its third corpus of breaks (3, 3, 6; all in
wins) and is now under #102/#118 with a plan-text metric. If that fails in wave 49 the remedy
is `{feeds: the opponent draws N extra per turn; punishers on your battlefield: 0}` on `Cast
Howling Mine` / `Cast Master of the Feast` rows — a count the engine already computes for the
other side's `DRAW PUNISHERS` line. Not asked for yet; recorded so it is not re-derived.

### R40 — post-answer overrun is now the reasoning channel, and it is unmeasured. (= ledger **D16**; deck126 G2)
9/174 deck126 replies carry two or more coded `CHOICE:` lines with 2.5-4.9 KB of reasoning
between them (`post_answer_overrun` 4,648 / 4,889 / 4,779 / 2,563); latencies 235 s and 515 s.
deck125: 0/406. The prompt's "Write nothing else" is not holding and E1 has made the overrun
load-bearing (it rescued 20 of 21 replacements). Ask: a first-class counter
(`multi_choice_replies`, with the rescued/harmful split of skill #116) before any prompt-side
change; a bounded pre-CHOICE reasoning block is the candidate if the count says the overrun is
amplifying drift rather than rescuing answers.
**Wave-49 prediction.** The counter exists; report per seat with the split.

---

## LOW

### R41 — the A-row / B-row / option lists sort lexicographically on `name #n`, so `#1`/`#2` of one name straddle another card and defeat the identical-text grouping. (seat-engine **D-1** secondary, watch item; = ledger **D13**)
`146 vs162` seq 15, 20: `A1. Triumphant Adventurer #1 ... A3. Triumphant Adventurer #2` split by
`A2. Silverquill Silencer`; `B1. Pride Guardian #1` / `B3. Pride Guardian #2` split by `B2.
Overgrown Battlement`. Cosmetic at 4 rows; the same mechanism doubled the 61 KB prompt. Fix
together with R28's grouping if that route is taken.

### R42 — the 1-based `turn` field. (seat-engine **D-8**; = ledger **D17**)
The JSONL `turn` is the log's turn minus one (`turn: 1, phase: Main phase 1` under `=== Turn 2
===`). Every citation mixing the field with the narration is one off; wave 47's "turn 10" was
the field. Emit the same number in both.

### R43 — a menace single-block goes to the heuristic with no re-ask. (seat-engine **D-7**; = ledger **D15**; closes the wave-47 red pin)
`deck126 vs146` seq 24 `BLOCKS: B1:A2, B2:A3` single-blocking two Spiders whose A-lines read
`[menace - only a block by TWO OR MORE of your creatures counts; ...]` -> `all_assignments_illegal`
-> Baka. `multiblock_reask` gives gang conflicts one re-ask; menace is the same class. The
corpus now has the record the pin wanted; R27's canonical-substring ask stands beside it.

### R44 — `ATTACK:` ranges naming non-existent labels are silently trimmed. (deck130 R4)
`ATTACK: A1, A2-A4, A5-A7` on a 4-attacker screen: the engine took the four real labels
(correct) and the pilot's next plan counted eight attackers. A one-line receipt (`you named
A5-A7, which are not on the list - ignored`) is the R30 receipt's sibling.

### R45 — the X free-kill marker sits on the row largest-first puts LAST, and the option-1 bias took row 1. (seat-engine **D-5**; = ledger **D14**)
`deck130 vs146` seq 47: rows `1. X = 6 {kills THEIRS: Silverquill Silencer; YOURS: none}` ...
`5. X = 2 {X pricing: same kills as X=6, for 4 less mana} [<- most kills at any affordable X
that costs you nothing]`; reply `CHOICE: 1 (X = 6)`, no reasoning; 4 mana overpaid on a turn it
then cast Siege-Gang with 3 floating. The owner's largest-first ordering stands; within it,
echo the marker on the top row (`[X=2 kills the same for 4 less mana - see row 5]`). Guide side
(skill #110) makes "answer the mark" the rule's first line.

### R46 — small render residuals, recorded together.
`[DRAW PRICE:]` verb agreement with one name (`Fate Unraveler punish` -> `punishes`; deck123 R5)
· `Keep this hand` could print the current keep size the way the Mulligan row prints the next
(`Keep this hand (keeping 6 cards)`; deck125 G4) · `manaAvailableLine` number word above ten
(85 emissions, R24) · `{leaves N}` on activated-ability rows (R15) · the cycling `[cost:]`
bracket (R23, untested) · the DFC Pathway PLAN-line tail (R18, untested).

### R47 — the Ob Nixilis `-2` rows print the target's name and the source's text, not the target's. (deck162 G-4; R25 family)
`vs123` s21 chose `Human (1/1)` over `Thraben Doomsayer (2/2)` — the maker then made twenty
more. Guide-side the seat names the token makers (162-B); the render's general form is R25's
`{right now:}` carrying the target's `{card text:}`.

---

## SHIPPED SINCE THE CORPUS — recorded with predictions, NOT docketed

| when | what | wave-49 prediction |
|---|---|---|
| **2026-08-27, owner live play on the Vita (out of wave), 19e2b04f7 and its siblings** | Three HUMAN-SEAT oracle fixes in the cast/usable gate: (1) the human cast gate counted only a dual land's FIRST colour (19e2b04f7); (2) a Delve alternative cost was priced as `{0}`, drawing a false castable border on Gurmag Angler; (3) an ability-tapping source counted its OWN mana toward its cost, drawing a false usable border on Westvale Abbey. All three are the human player's affordability oracle, not the LLM seat's render. | Human-seat affordability borders agree with the engine's actual payability on dual lands, Delve spells and self-tapping abilities (owner live-play verification); no LLM-seat rate is claimed to move. R32 is the LLM-seat sibling in the same payment family. |

---

## GUIDE-ONLY — stays in guides after the render ships
- deck123's repeat-row arithmetic (N = (L + C + 3) - M) and the ONE emit spelling stated
  positively (A48-1; skill #104) — the stop stays correct after R30 widens the grammar.
- deck152's loop-pair verdict keyed to both names on the battlefield line (152-A) and the
  Spellbinder hand-look priority (152-B) — both stay correct after R12 ships.
- deck146's converter DIRECTION sentence in the fold's own words (146-A; skill #106).
- deck125's composed Forced-Fruition price (W8) and the tap-out gate inside the dictated plan
  line (W10; skill #115).
- deck152's lethal-first-check-of-the-turn and `Those sources` payment rule (152-C; skill #112).
- deck146's tapped-state / payment checks on the Hive (146-C; skill #113).
- deck130's "answer the mark" first line (D48-5; skill #110) and planeswalker-first (D48-1).
- deck162's odds-based one-land branch by look (162-C) and the punisher tiebreak (PASS 1/1).

## MEASUREMENT items for the wave-49 reviewer briefs
1. **`answer_replaced` per seat, READ and split** (rescued vs executed-not-on-menu; skill #116).
2. **Offers by phase and windows by kind per seat** for every tap-cost or repeatable activation,
   and the per-kind window count so unreachable sections are visible (skill #117).
3. **Plan-text / belief-text metrics** for every #102/#106 edit: deck146 "net 0" (was 3/4
   windows), deck162 "stack the draw engines" (was 6), deck123 plan-names-the-edict-body (was 2).
4. **Prompt chars per decision kind, wave 48 -> 49**, all games (no exclusions this time).
5. **Hold-gate fractions split by spend and by consequence trace** (skill #109): deck125's Staff
   sub-lane (2/6) and Elixir casts (exempted).
6. **The repeat receipt count vs the tag** per token game (R30), and `repeat_count_missing`.

## PASS / KEEP — measured working, do not re-open
The lane-N pass row (139/139) and repeat row (12/14 taken, 8/8 counted takes executed exactly)
· lane O's A-row/battlefield/option collapses and the range grammar (2/2 accepted) · lane P's
attacking header (26/26 correct on the first bucket), stack keyword tags (0/33 counters on
walls), the priced representative (1/1) · lane Q's `{card text:}` clause budget (0/2,077
mid-word), `Choose an option for <card>:` (57), `target controller` gone (0), ANNOUNCE_X with
no plan (0/28) and the marker (3/4) · E1's replacement (21/21 to the last coded line, 0
`retracted_choice`) · the mana-only gate (0 leaks, max 835 skips) · `{right now:}` on Peer into
the Abyss · the GANG BLOCK clause (first live hold, `162 vs123` s17) · the affirmative
no-blocker tag (44/44 attacked at three seats, 19/19 and 24/24) · land drops (392/392 + 84 +
168 + 131 + 93, 0 declines) · the own-hand count (406/406 at deck125) · the pregame no-plan ask
(every keep's PLAN was about the hand in front of it). **Zero render falsehoods found this
corpus**; the one wrong number (R37) is a double-count in a new header, and everything else open
is an omission, an unranged list, or a missing affordance.
