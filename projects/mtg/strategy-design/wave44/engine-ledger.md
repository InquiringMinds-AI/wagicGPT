# Wave-44 engine ledger — THE WAVE-45 DOCKET (assembled 2026-08-26)

Corpus `matchups-20260826-111937` — FIRST FAIR-HAND corpus, binary `2d09ea75c`.
21 games / 2119 records / **2035 decisions** (1442 ask, 455 priority, 91 attackers,
37 blockers, 6 reveal, 4 bottom). Fallbacks **6 / 2035 = 0.29%**.
Repro paths are relative to `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-111937/`.

**New-baseline rule still binding:** nothing in this ledger attributes win-rate or
game-length movement to guide edits or engine lanes. Hand distribution moved this wave.

---

## OPEN ITEMS — RANKED

### HIGH

**#W45-1 (HIGH, render) — menace BLOCK side is a live false affordance.**
8 of 8 blocker-row entries offering a menace attacker render a bare 1-on-1 verdict with no
menace caveat, in the same prompt whose attacker line carries the caveat. The model declared a
solo block, the engine dropped it (`dropped_assignments: 1`) and 4 damage went through. The
Pride Guardian defender row makes it worse: `[deals 0 - this block kills nothing, but it STOPS
all of the damage from whichever attacker it blocks]` is flatly false for a solo block on a
menace attacker.
Repro: `1787761186-ai_baka_deck123-0x5557c3baf5d0-vs-ai_baka_deck146.jsonl` **seq 17**
(committed solo block); render half at `deck126-0x563b923035e` **seq 47** and **seq 57**;
independent instance at deck123's seat, vs146 seq 17 (a lone 5/5 on a `[menace]` attacker).
**This supersedes wave-43 #W44-8's "corpus-unreachable" status for the RENDER half.** The Baka
probe deck is still owed for the block-side BEHAVIOUR fix (red pin
`menace_defender_one_blocker.txt` — Baka never gang-blocks menace).
Fix shape: suppress or prefix the 1-on-1 verdict for menace attackers on block rows, exactly as
the attack side does. Trust doctrine: a false surface ranks HIGH.
Prediction: bare 1-on-1 verdicts on menace attacker rows 0/N; solo menace blocks 0;
menace-caused `dropped_assignments` 0.

**#W45-2 (HIGH, render) — the collapsed blocker summary prints only the favourable outcome.**
`[their untapped blockers: they have 6 untapped creatures that could block this one, biggest
Bloodline Keeper #1 (3/3) (you kill it, your attacker lives)]` — three 2/2 Vampires then
gang-blocked and killed the 5/5; game lost at -21. True, and the only outcome shown is the
favourable one on a board where the unfavourable one is trivial. All seven guides are written
against the ENUMERATED form and none mentions the collapsed one.
Repro: `1787761186-...deck162-0x5652b65f6280-vs-...deck123` **attackers seq 13**.
Fix shape: price the collapse (`any 2 of them together deal 4, enough to kill it`) or minimally
tag it `1-on-1 only — N blockers can gang this attacker`. **Record the collapse threshold where
reviewers can see it.**
Prediction: attacks taken on a collapsed tag with N >= 2 that end in the attacker dying: 0.

**#W45-3 (HIGH, render) — combat previews model deaths and never model LIFE.**
`Perimeter Captain (0/4) (neither dies)` hides 2 life per blocking defender per Captain
(`auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller`);
`Pride Guardian` hides 3 (`source(this):life:3 controller`); Sanguine Bond converts it to damage
on the attacker. Two seats, two decided games: deck146 vs126 attackers **seq 37/44/51/56/65/73**
(opponent 13 -> 42, single turns of +12 and +13, lost at T26 from a T16 lead);
`1787761184-ai_baka_deck123-0x55e881efea80-vs-ai_baka_deck126.jsonl` **seq 40 and seq 59**
(life 20 -> 8 -> 0). The lifelink convention already exists and now renders correctly
(`(both die (lifelink: they gain 1))`), so this is the same annotation one step larger — the
trigger sits on a third permanent, not on the blocker.
Fix shape: `(neither dies (they gain 3))` for any `@combat(blocking)` life trigger, plus one
attackers-prompt summary line where a `@lifeof(player)` converter is on either battlefield.
Prediction: block-trigger life gained in any single deck146 game < 6; `(neither dies)` bodies
sent into a 2+-defender board 0/N.

**#W45-4 (HIGH, core predicate) — the plan-mismatch note asserts a falsehood about the menu on
screen, and over-fires 42.8%.**
Precision 81% / recall 72% against a 27,691-name vocabulary; **5 strictly-verified false fires**
(an option row literally begins with the plan-named `Cast <name>`):
`deck146-0x5559288b224` **seq 84** (Acererak), `deck126-0x5603808fbfc` **seq 9**,
`deck126-0x563b923035e` **seq 39**, `deck146-0x563b90fca49` **seq 24** and **seq 72**; a sixth by
inspection `deck125-0x5653db89c3b` **seq 25** (Supreme Verdict). Verb-mismatch subclass: 52 of
193 fires at deck123's seat (repro deck123 vs162 **seq 48**). **Land drops: 327/419 fire**,
contradicting the same prompt's `its absence from the choices below does not mean it is gone`
three lines above; it also contributed to the 679s decision (#W45-8).
The combat half of wave-43's ask is **DISCHARGED**: attackers 0/91, blockers 0/37, reveal 0/6,
bottom 0/4, X-announce 0/14.
Fix shape: suppress on structurally cast-free menus (land drop, X-announce, mode sub-asks); match
CARD NAMES not verbs; suppress on plan clauses beginning "Next turn"/"Then".
Prediction: land-drop fires 0/~400; strict false fires 0; overall rate < 25%, precision >= 95%.

**#W45-5 (HIGH, render) — ANNOUNCE_X rows carry no kill facts.**
The `{X pricing:}` clause with `kills THEIRS:` / `YOURS:` rides the CAST row and is **absent at
the moment X is chosen**. deck130 killed its own Rorix Bladewing twice from that menu, in games
it was winning (vs126 seq 44/46, opponent at 9 and Rorix two swings from lethal, lost t22;
vs162 seq 81/83, X=5 for a 0/6 `[defender]` Shield Sphere, dead t23). 18 X-announce windows
corpus-wide, 2 with plan-stated X != chosen X, both toward option 1 (the other is deck125 seq 83,
plan X=4, chose X=11).
**ADJUDICATED NOT A BUG: the menu is not inverted.** The header states verbatim "option 1 is the
LARGEST X (X = 5)" and the seq-46 plan itself re-decides to X=5. The surface was TRUE.
Fix shape: annotate each `X = N` row with `{kills THEIRS: ...; YOURS: ...}` (reuse the X-pricing
evaluator per N; identical rows collapse to `(same as X=N-1)`); the `YOURS:` half is mandatory.
**Ordering is a separate DESIGN question routed to the owner** — see OWNER-CALL #4.
Prediction: plan-vs-chosen X divergence 0/N; casts at an X whose `YOURS:` list names the seat's
clock 0 (was 2/2).

### MED

**#W45-6 (MED, render) — opponent non-creature permanents carry no text anywhere.**
`Sanguine Bond {3}{b}{b} [enchantment]`, `Staff of Nin {6} [artifact]`, `Howling Mine {2}
[artifact]`, `Lightmine Field {2}{w}{w} [enchantment]` — name and type only, while target rows
carry `{target text:}` at 100% (the channel exists, it is just not applied to the board line).
Every card in that list changed or ended a game at these seats. Repro: deck123 vs125 **seq 50**;
deck123 vs126 **seq 40**.
Fix shape: an `{effect:}` clause on opponent permanents whose primitive has an `auto=` line — at
minimum `@combat`, `@lifeof`, `@each ... upkeep` and damage triggers.
Prediction: opponent permanents with an `auto=` line and no text on the board row: 0.

**#W45-7 (MED, lexicon) — raw `Transform:backside` leaks in option rows AND history; bundle with
`teferieffect`.**
NEW this wave, not in wave 43: **57 prompt-instances / 13 unique lines**, e.g.
`4. Transform:backside with Bloodline Keeper [cost: {b}]` and
`- You used: Transform:backside with Bloodline Keeper #1`. Verified: `mtg.txt:12582` Bloodline
Keeper has `backside=Lord of Lineage` + `auto={B}:flip(backside)`, and the card's real `text=`
says "{B}: Transform Bloodline Keeper." The engine already resolves the placeholder on ONE path
(`AIPlayerGPT.cpp:8494`, `if (otherName == "backside")`) — which is why the Flip Side rows read
correctly — but the ability-name / stack-label path does not.
Repro: `1787761184-ai_baka_deck123-0x55e881efea80-vs-ai_baka_deck126.jsonl` **seq 36** (option row),
**seq 43** (history).
**Carried, same locus:** `teferieffect` stack label, **29 prompt-instances / 3 unique lines**, all
`N: ability: teferieffect Counter Removed [triggered/activated ability]` — identical count to wave
43; the wave-43 one-liner at `stackAbilityName` did NOT ship. Repro:
`1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl` **seq 52 / 55 / 62**.
(The LOG-side Teferi counter IS fixed: "a bookkeeping (Teferi's +1) counter". Dungeon zone tags:
PASS, 0 occurrences, was 84.)
Prediction: `Transform:backside` 0 prompt-instances; `teferieffect` 0.

**#W45-8 (MED, harness) — the reply channel has no decode stop.**
Two symptoms, one root. (a) The **679s** decision,
`1787761215-ai_baka_deck126-0x563b923035e0-vs-ai_baka_deck146.jsonl` **seq 14**: an 11,372-char
reply on a three-option LAND DROP (prompt 7,232 chars, BELOW the 8,512 median — size is not the
cause), re-deriving Sanguine Bond's cost five times, then re-emitting the same CHOICE; ~11 minutes
of a 21-way-parallel run; the plan-note (#W45-4) fired on that window. (b) The corpus's only
`retracted_choice`, `deck162-0x5589480413b0` **seq 37**: a valid `CHOICE: 3 (Cast Howling Mine)`
followed by ~2400 chars of deliberation ending on a contradictory `CHOICE: 4` — correctly refused.
27 records carry `post_plan_overrun > 0` (5065 / 3606 / 2667 / 2341 top); the next five slowest
decisions (485/462/414/352/277s) are all 6k-26k-char replies. Latency median 13.5s, p95 47.7s.
Fix shape: a decode stop after the PLAN/CHOICE line.
Prediction: `post_plan_overrun > 0` <= 3; max latency < 120s; `retracted_choice` 0.

**#W45-9 (MED, render + core) — anonymous option rows are the fallback generator (`stale_echo`).**
All 5 `stale_echo` fallbacks and both `echo_index_conflict` recoveries are on rows with no card
name to copy (`choose a land`, `Decline - do nothing`, `creature gains 3/3 and you draw`, three
byte-identical `Vampire (2/2)` rows). **Subclass rate 4 / 57 mode-label asks = 7.0%** vs 0.29%
overall. One is a **FALSE stale**: `deck146-0x563b90fca490-vs-deck126` **seq 35**, reply
`CHOICE: 6 (Silverquill Command)` — the parenthetical names the ask's OWN source card.
Repro (genuine): `deck152-0x5580323c50b0-vs-deck125` **seq 52 / 53 / 62**;
`deck162-0x5652b65f6280-vs-deck123` **seq 17** (three identical Vampires).
Fix shapes, any one sufficient: (a) ENGINE — `AIPlayerGPT.cpp` already strips a `pendingSource`
prefix for the `"<X> targeting <Y>"` family; add the symmetric case so an echo matching the ask's
own SOURCE card name is a source prefix, not a stale echo (recovers seq 35 alone); (b) RENDER —
name the source on anonymous rows (`choose a land for Teferi, Who Slows the Sunset`,
`Vampire (2/2) [blocker #3 of 6]`); (c) CORE — protocol example that copies the option's own words.
Prediction: `stale_echo` 0; total fallback rate <= 0.15%; subclass back at baseline.

**#W45-10 (MED, render) — a sub-ask never states what the parent ability does.**
`Choose an option for Teferi, Who Slows the Sunset: 1. choose a land 2. Decline - do nothing` —
nothing says the +1 untaps one of yours or taps one of theirs; the model answered about a
different card and the reply was discarded (3 of the 6 corpus fallbacks). Repro:
`deck152-0x5580323c50b0-vs-deck125` **seq 52 / 53 / 62**. Compounded by #W45-13 (land rows).
Fix shape: carry the ability's rendered effect text in the sub-ask header, as cast options do.

**#W45-11 (MED, render) — no lethal headline on the ATTACK side (0/91), and the LETHAL block
header states the goal without the method.**
Block side is 37/37 arithmetically correct and still produced the losing answer at deck152 vs146
**seq 20**, because nothing connects each attacker's `deals N` to the surplus to be removed.
Attack side has no equivalent at all, so the model hand-derives its own lethal in prose, on the
side that wins games, in a prompt that elsewhere says "Do not re-derive these outcomes; use them".
Repro: `1787761180-ai_baka_deck152-0x557785f52f40-vs-ai_baka_deck162.jsonl` **seq 26**.
Fix shape: on LETHAL headers, `You must stop at least 6 of the 11: A2 deals 7, A1 deals 2, A3
deals 2.` On attackers, the mirror lethal line. Both are arithmetic the engine already has.

**#W45-12 (MED, render) — `[defender]` on the battlefield line has no gloss.**
14 replies corpus-wide assert `<X> cannot block` about a card that can block; 7 of them in one
seat off `Fog Bank {1}{u} (0/2) [flying, defender]`. The option row already renders
`Defender (This creature can't attack.)` and the blockers window a fuller gloss — the honest gloss
exists two renderers away. Repro:
`1787761180-ai_baka_deck152-0x557785f52f40-vs-ai_baka_deck162.jsonl` **seq 7** (also seq 8, 9, 10,
25, 26). **Cheapest item in this docket.**

**#W45-13 (MED, render) — land target rows carry no mana text; text-less duals render bare.**
`Tundra [land] [opponent's battlefield]` (deck152 vs125 **seq 64, 71, 85, 102**) and
`Scrubland (land)` beside `Woodland Cemetery (land) {target text: "...{T}: Add {B} or {G}."}` in
one Molten Rain list (repro `1787761212-ai_baka_deck130-0x55df6ecbf9d0-vs-ai_baka_deck126.jsonl`
**seq 12**). Verified `mtg.txt:100808`: Scrubland is `type=Land`, `subtype=Plains Swamp`, no
`text=` — the renderer is honest and the engine knows the subtypes. 32 entries.
Fix shape: synthesize the mana line from `subtype=` when `text=` is absent.

**#W45-14 (MED, carried) — mana-ability-only window auto-pass (was #W44-9).**
Recounted this wave: **9 windows** whose only options are mana activations with no pending cost,
all in one game — `deck126-0x55bd628d9340-vs-deck125` seq 17, 21, 23, 24, 25, 26, 30, 33, 35 —
= 4.1% of that seat's inference but **41% of its priority windows**; deck125 0/125. All nine were
correctly passed. Wave-43's 21.6% headline was **game composition, not a stable rate** — the
auto-pass is still worth shipping (9 deterministic model calls in one 38-turn game), at MED.

**#W45-15 (MED, instrument) — `dropped_assignments` conflates a declined blocker with a refused
one.** 4 blockers records carry a non-zero value; only ONE is a real drop
(`deck123-0x5557c3baf5d` seq 17, the solo menace block). The other three count the model's explicit
`Bn:none` declarations — which the prompt itself teaches — as drops
(`deck162-0x557787294490` seq 17; `deck130-0x55882f2454d0` seq 32; `deck162-0x5589480413b0` seq 17).
**Wave-43's "dropped-assignment accounting: corpus total 1, non-menace, correctly accounted" was
computed on this conflated metric and must be re-read as UNVERIFIED.** Costs review accuracy, not
games — but it is an instrument defect and instruments come first.

**#W45-16 (MED, render) — no previous value of any counter is available for trend teaches.**
The prompt prints scalars; the previous value lives only in the interleaved cumulative log.
deck130 fired 9 face-damage spells in one game at opponent life 25/22/18/20/17/19/24/27/24 without
noticing the reversals. Fix shape: `Opponent life: 24 (was 19 at your last decision, +5)` when it
moved the wrong way since this seat's previous decision — a delta the harness already has.
**If this is ruled out of scope, the consequence is mandatory: trend-based teaches are retired from
every guide**, because they cannot be executed against a scalar.

### LOW

**#W45-17 (LOW) — singular loyalty path drops `(now N)`, both directions.** Not the loss/gain split
wave-43 recorded: `lost 3 loyalty counters (now 1)` 4/4 OK and `got 4 ... (now 4)` 12/12 OK, while
`lost a loyalty counter` **0/6** and `got a loyalty counter` **0/4**. 10 lines. Repro:
`deck146-0x563ae3950310` **seq 20**; `deck126-0x55df6d9a1f20` **seq 23**.

**#W45-18 (LOW) — X-announce window shows the spell still in hand, no ON THE STACK line.** 14/14.
Repro: `1787761192-ai_baka_deck125-0x555929beb550-vs-ai_baka_deck146.jsonl` **seq 86** (cast, hand 8)
vs **seq 87** (X menu, hand 8). A guide currently carries a `SUBTRACT 1` workaround — i.e. a guide
hedging against the render, which the fix retires.

**#W45-19 (LOW) — duplicate copies in a target clause carry no `{target text:}`, and the dedup is
unmarked.** Repro: `deck125-0x558033705f60-vs-deck152` **seq 21** (Brutal Cathar #2, Elite Spellbinder
#2 bare); `deck125...-vs-deck126` **seq 76** (Perimeter Captain #2, #3). Corpus accounting shows the
dedup is deliberate (313 same-name repeats), but a deduped row is shape-identical to a genuinely
textless one. `{target text: as #1}` removes the guess. No decision this corpus chose a duplicate.

**#W45-20 (LOW) — history lines identify a source by bare card name with no owner prefix.** 402
lines, e.g. `- Luminarch Aspirant dealt 1 damage to the opponent (now 19)`. Genuinely ambiguous in
this pool's mirrors (deck123 and deck146 both run Nadaar and Hive of the Eye Tyrant). Needs the
owner's LOG-SHAPE ruling (narration parity vs owner tags) — see OWNER-CALL #5 — not a patch.

**#W45-21 (LOW) — a malformed double assignment is accepted silently.** deck130 vs146 **seq 32**
replied `BLOCKS: B1:A2, B1:none` — the same blocker assigned twice; the parser took the first with no
`fallback` flag. Correct in the safe direction, but self-contradicting block replies are invisible in
the corpus. Worth a counter, not a reject.

### CARRIED FROM WAVE 43 — NOT DISCHARGED, NOT RE-DERIVED
- **Baka menace gang-block** (red pin `menace_defender_one_blocker.txt`): Baka never gang-blocks a
  menace attacker; expect exactly 1 ASSERT per gate — the safety net, not a regression. The probe
  deck is still owed for the block-side BEHAVIOUR; #W45-1 covers the render half only.
- **who-dies preview on marked damage** — known deferred. No isolable instance in 31 real blocks this
  corpus (Shield Sphere's 0/6 -> 0/5 -> 0/4 -> 0/3 tags were consistent). UNTESTED, not re-docketed.
- **counter-preview single-object false affordance** (1/57 in wave 43): **0** single-object instances
  in 45 counter windows this corpus — consistent with a fix OR an absent window. No verdict claimed;
  still needs a repro locus.
- **opponent-hand reveal false frame (E-2 shape)**: no opponent-hand reveal arose. UNTESTED, NOT
  resolved — the #W44-6 PASS covers own-library reveals only (6/6).
- **trample carve-out / trample parenthesis**: fifth consecutive structural null. UNREACHABLE-IN-POOL
  per skill amendment 21 — discharge by probe, never by another corpus wait.
- **CS-024a Boggart Trawler backside** (likely adjacent to #W45-7's placeholder path).
- **manarestriction blast-radius sweep** (85 cards, needs `alsoabilities` flavour).
- **counter-clause keyword set**; **WSrcUnlockedCards bounds guard**; **Baka X=0**;
  **mana-tap asymmetry #W42-D2r** (parked at the owner).
- **EDIT C maker-just-landed clause** (n=2, still under-sampled).

---

## DISCHARGED THIS WAVE — ALL SET A/B ADJUDICATIONS

### Set A — wave-43 GUIDE EDITS (20 edits incl. 2 deletions)
| edit | verdict | count |
|---|---|---|
| deck123 EDIT J (wall-board ATTACK: none) | **FAIL** | 1/3 (vs126 seq 40, 59 both decisive; held seq 33) |
| deck123 EDIT K (Greaves gate) | **PASS** | 35/35 declines — single-branch, all at 0 creatures |
| deck123 EDIT L (edict N-check) | **PASS-with-1** | 83/84 (violation vs152 seq 11 at N=2) |
| deck123 EDIT M (spend every blocker) | **PASS** | 2/2 |
| deck123 12-creature stop-rule | **PASS** | stopped at exactly 12 (vs126 seq 31) |
| deck130 EDIT 1 (Starstorm cycling release) | **PASS** | 2/2 legal of 33 offers |
| deck130 EDIT 3 (Blastminer attack floor) | **PASS** | 7/7 |
| deck130 EDIT 4/9 (Cave land-drop gate) | **PASS in effect** | 2/2 in effect, 1/2 by the letter (vs162 seq 59) |
| deck130 EDIT 6 (read the WHOLE X annotation) | **PASS on reading, OVERSHOT on acting** | reads the tail, then sweeps its own clock (vs126 seq 44, vs162 seq 81) |
| deck130 EDIT 7 (lethal-chain exception) | **UNTESTED** | no lethal-chain window |
| deck130 EDIT 8 (backstop retire) | **PASS** | 21/21 owner-tagged, 0 self-hits |
| deck125 W6 (Path the engine creature) | **PASS** | 6/7 within 2 windows (the miss is a counter-hold) |
| deck125 W7 (zero decking losses) | **PASS, by luck** | 0/6; library minima 20,12,13,20,39,14 |
| deck125 W8 (X <= library-2-Staffs) | **FAIL** | 13/14 (violation vs146 seq 87: library 13, X=12) |
| deck125 W9 (hand + X <= 7) | **FAIL** | 5/14 — root cause: largest offered X taken **14/14** |
| deck125 W-carry (Elixir at library <= 12) | **FAIL (never fired)** | 17 offers, 0 taken; last offer at library 13 |
| deck126 D9 (zero BLOCKS: none at <= 8) | **PASS, stronger than predicted** | 15/15 windows, 100% of offered blockers assigned |
| 146/152 S1 (per-creature attacker STOP) | **FAIL, split** | 3 windows: right about CARDS, wrong about TOKENS |
| 146/152 S2 (sole attacker all-"(neither dies)") | **UNTESTED** | no such window (adjacent 162 confirmation only) |
| 146/152 S3 (Shield Sphere named exception) | **UNTESTED** | adjacent proof from the other side only |
| 146 S4 (DELETE the hold-back gate) | **PASS** | 40 attack decisions, 0 where its absence caused the error |
| 162 S5 (Fate Unraveler "(both die)" read) | **UNTESTED** | tag never appeared |
| 162 S6 (**DELETE** the D3 gate + cut to stops) | **FAIL, twice** | vs123 seq 13 (lost a card and the game); vs152 seq 19 (free attack declined) |
| P1 (Starstorm cast rate at kills>=2 >= 60%) | **directional PASS, UNDERPOWERED** | 2/3 = 67%, n=3; loose metric 3/9 = 33%, identical to wave 43 |
**Set A tally: PASS 11 (2 qualified), FAIL 6, UNTESTED 4, underpowered 1.**

### Set B — wave-44 ENGINE LANES
| lane | verdict | counts |
|---|---|---|
| Fair hands (lane A) | **PASS** | 41 keeps at 2x13/3x17/4x9/5x2; 4 mulligans (2 from 0-land), 4 `bottom` records; no mull-to-zero, no repeat, no aggressivity artifact |
| #W44-2 X-pricing opponent-first | **render PASS / behaviour underpowered** | 9/9 headline windows opponent-first; 42 Starstorm decisions with 0 old-form headlines; behaviour n=3 |
| #W44-3 removal `{target text:}` | **PASS** | 862 card-target entries: 368 explicit + 313 dedups + 181 legitimately textless; **0 blind removal choices** (53/53, 45/45 counters, 96/103, 10/10 Paths) |
| #W44-4 Day/Night line | **PASS** | 236 records carry `It is currently Day./Night.` vs 229 rendering a daybound card; 13/13, 31/31, 10/10 per seat; 0 in games with no daybound card |
| #W44-5 teferieffect + dungeon tags | **SPLIT** | dungeon **PASS** (0 occurrences, was 84; 16/16 windows); teferieffect **FAIL** (29 instances, unchanged) |
| #W44-6 reveal false frame | **PASS** | 6/6 records; own-library attribution correct; opponent-hand shape UNTESTED |
| #W44-7 plan-note gate | **SPLIT** | combat **PASS** (0/91, 0/37, 0/6, 0/4, 0/14); precision **FAIL** (42.8% fire, 81% precision, 5 strict false fires, 327/419 land drops) |
| Lethal basis / forecast bRemaining | **PASS** | 37/37 on L-D==R, `my_life`, verdict polarity incl. the R==0 case |
| Lifelink / deathtouch in trade verdicts | **PASS** | wave-43 E-3 fixed; 37 deathtouch rows with correct paired verdicts |
| Loyalty `(now N)` | **PARTIAL** | plural 4/4 and 12/12 PASS; singular 0/6 and 0/4 FAIL |
| Menace attack side | **PASS** | caveat + untapped-joiner count present |
| Menace defender side (#W44-8) | **FAIL — and no longer corpus-unreachable** | 8/8 bare 1-on-1 verdicts; 1 committed solo block |
| Run collapse / battlefield frame | **PASS** | 3839/3839 counts, 1173/1173 spans |
| Observer parity | **PASS** | 0 defects / 21 games |
| Rules-text tails, mana-hint tails, `put a put`, token text | **PASS** | 0 each |
| DFC / Flip Side labels | **PASS** | 123/123 |
**Set B tally: PASS 11, SPLIT 2, PARTIAL 1, FAIL 2 (teferieffect, menace defender side).**

### Fallback classes this wave (6 / 2035 = 0.29%)
- **`stale_echo` on nameless option rows — 5 of 6.** Subclass rate **7.0%** (4 of 57 mode-label asks)
  vs 0.29% overall. One is a false stale (source-card parenthetical). -> #W45-9.
- **`retracted_choice` / no decode stop — 1 of 6**, plus 27 `post_plan_overrun` records and the 679s
  decision. -> #W45-8.
- **Rate verdict: NOT a regression.** 6 events on n=2035 against a 0.17% base has expectation 3.5,
  Poisson P(X>=6) ~ 0.16; and 3 of 6 sit in one seat produced by one card (Teferi). Per-card
  concentration, not rate drift — per skill amendment 45, no verdict is claimed from this sample.

---

## OWNER-CALL

### 1. deck146 — a written floor's first live window says the floor is wrong (verbatim from the seat)
> **OWNER QUESTION (do not patch): deck146 mulliganed a ONE-LAND seven against its own written
> floor.** deck146's guide STEP 2 lists the complete mulligan set as "ZERO lands; an all-land hand;
> or NO source of one of your colors", and states in terms "A one-land hand is NOT on it and is a
> KEEP". The model mulliganed a 1-land 7 (Plains + 6 spells, two of them the Emeria's Call /
> Pelakka Predation "not lands" cases the guide warns about) — **and won that game.** So on the
> first corpus where mulligan floors were ever testable, the floor's very first live window says the
> floor is wrong, in the model's favour. That is a floor question, not a prose bug: it belongs to the
> owner. Every mulligan teach in these three guides is otherwise still at **1 window each** — nothing
> here supports rewriting a floor in either direction.

### 2. deck125 — a colour-screwed 2-land keep (verbatim from the seat)
> **OWNER QUESTION (mulligan floor, deck125):** the single mulligan violated the guide's explicit
> floor. The hand was `2 lands, 5 spells`, `Mana sources among those lands: {W} 2`, `Playing every
> land in this hand would cover the cost of: Path to Exile {w}` — two Plains, Emrakul, Dream Fracture
> {1}{U}{U}, two Fall of the Gavel {3}{U}{W}, one Path. The guide says KEEP at 2 lands and says a hand
> is keepable if that coverage line names even one card; the model shipped it and won the 52-turn
> game. n=1, and the win is not evidence. I added a narrowly-shaped carve-out (single-colour source
> line AND coverage line naming exactly one card) rather than loosen the floor, and I am routing the
> floor itself to the owner: **should a keepable-by-count hand whose lands make one colour while five
> of seven cards need the other be a keep?**

### 3. deck123 / deck130 — the permissive 2-land floors, outcomes split 1-1 (verbatim from the seat)
> => OWNER QUESTION (not patched, per the carry): both floors are permissive - deck123 "KEEP any
> 7-card hand with two or more lands", deck130 "Mulligan only a hand with zero lands, or a hand
> that is all lands". The four 2-land keeps split 1-1 per seat on outcome, and the one
> catastrophic case is legible: deck123 vs deck130 kept Scrubland + Isolated Chapel + Idyllic
> Tutor + Devour Flesh + Tragic Slip + Lightning Greaves + Damnation, never drew a third land,
> made SEVEN decisions across thirteen turns (the last at turn 5) and lost 20-to--4 without ever
> casting a spell it drew. n=2 per seat is not enough to move a floor and floors at this seam are
> owner-routed this wave; the question for the owner is whether a 2-land keep should require a
> 2-mana play plus a second source of the deck's demanding colour, or stay where it is.

*(Synthesis note: no seat patched a floor. The one guide change made anywhere at this seam is
deck125's narrow carve-out, explicitly flagged to the owner by its author. Skill amendment 38 now
forbids moving a floor from a single window.)*

### 4. ANNOUNCE_X ordering — a design question, not a bug
The menu is largest-first and states so truthfully; both observed plan-vs-choice divergences went to
option 1. The "usually-correct-first" convention argues for leading with the SMALLEST X that kills an
opponent creature (or ordering ascending so option N = X = N where affordable). This changes what the
pilot sees first at every X window in the pool. Owner's call; the per-row kill annotation (#W45-5)
ships regardless and is the stronger half.

### 5. History log shape — owner prefix vs owner tags
402 history lines name a source with no `Your`/`Opponent's` prefix, ambiguous in this pool's mirrors.
Wave-43 filed this as a narration-parity ruling and it is still open: does the log follow the owner-tag
convention used elsewhere in the prompt, or keep its current shape? (#W45-20.)

### 6. Carried owner items, unchanged
- **mana-tap asymmetry (#W42-D2r)** — still parked.
- **deck123 construction case** — OWNER-RULED OUT OF SCOPE; not reviewed this wave (play only).
- **`{room effect:}` in history** — CLOSED, deliberate keep. Do not re-file.
- **VPK queue** — menace legality, damage-order window, lethal-basis fix, fair hands, all wave-43/44
  render+engine work (everything since 9f47ce024), owner-initiated only.

---

## LATEGAME SPECIMEN (invariant 0)
Four candidates were nominated. Recommended primary:
`1787761221-ai_baka_deck152-0x5580323c50b0-vs-ai_baka_deck125.jsonl` **seq 108** — turn ~50 of the
corpus's longest game, 13-permanent board, empty hand, Teferi bookkeeping counters; the seat carrying
the `teferieffect` labels and 3 of the 6 fallbacks, i.e. several open lanes in one prompt.
Maximal-frame alternate: `1787761215-ai_baka_deck126-0x563b923035e0-vs-ai_baka_deck146.jsonl` **seq 57**
(30,710 chars, turn-26 blockers window, also carries the menace false affordance of #W45-1).
Richest attack-side alternate: `1787761184-ai_baka_deck123-0x55e881efea80-vs-ai_baka_deck126.jsonl`
**seq 59** (the #W45-3 window where the render's honesty and its incompleteness sit side by side).

## OWNER RULINGS 2026-08-26 (asked directly; these close OWNER-CALL 1-5)
1-3. MULLIGAN FLOORS — owner, verbatim: "mulligans are essentially a probability question. what
are the chances i draw what i need to make this work, as compared to my chances of winning if
drawing a smaller hand? obviously the deck 125 determined correctly.. i very seldom mulligan a
2 land hand. the chance for drawing a third land before or on the turn i can drop it is very
high, and the fourth land is mathematically likely by the 5th. dropping a 2 land hand of 7
cards is mathematically likely to get a 6 card hand with 2 land, its not a good reason to
mulligan by that metric." APPLIED: deck125's wave-44 colour-screw carve-out REMOVED (it
would have shipped the hand he called correctly kept); 2-land keeps stand everywhere; skill
amendment 46 (mulligan teaches are odds comparisons, never colour/curve floors). The 1-land
question (deck146) was put to the owner separately.
4. ANNOUNCE_X ORDER — "Keep largest-first". CLOSED; per-row kill facts (#W45-5) are the fix.
5. LOG SHAPE — "Owner-prefix every line" ('Your X' / 'Opponent's X' on all history lines,
consistent with the board frame). #W45-20 is now a RENDER LANE for wave-45 step 1b.
ONE-LAND SEVENS — owner, verbatim (2026-08-26): "the guide is the best effort suggestion, the
goal is to give a guide which when followed will usually win, but the model not following the
guide to its exact letter is acceptable behavior. if everything in the hand cost say 3 or more
mana, 1 land is a terrible hand." APPLIED: deck146 STEP 2's 'one-land = KEEP' absolute replaced
by the odds rule (one land + spells costing 2+ = ship; one land + one-drops = keep); skill #47.
Refinement, owner verbatim: "a hand of mostly 2 drops with 1 land is a hand i might take.
depending on what those were" -> one land + 3+-cost = ship; one land + cheap spells = judgment
on the exact cards (deck146 + skill #47 updated).
Owner verbatim (same thread): "a hand where if i drew two mana then i could immediately win
the game, something with spells that make mana maybe? a combo hand. I could see it happening.
in the right deck. it would be a dicey choice, expecting 4-6 rounds before reaching 2 land
draws. id have to consider the speed of my opponents deck, or disruption options." -> the
factor list in skill #46.
OWNER RULINGS 2026-08-26 (second direct ask): (a) "im not interested in designing deck
strategies at this time. that's the responsibility of the reviewing agent." -> deck123's
12-token cap goes back to the deck123 reviewer/synthesis to decide from evidence (skill #49).
(b) No mulligan floor at any size — verbatim in skill #48; deck146's STEP-1 imperative and all
'never below N' lines are to be stripped (skill #48). Both mull-to-four cascades are graded by
odds, not floors.
