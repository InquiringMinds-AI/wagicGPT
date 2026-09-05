# deck126 (Sanguine Blood — WGB wall prison) — wave-62 review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058/`, binary 12be0b79c.
Six games, all natural, **0 fallbacks, 0 `[RE-ASK]` prompts, 0 out-of-range CHOICE numbers, 0
`transport`/`empty_reply` stamps** across 335 records. Record 2-4.
Seat files are `1788633NNN-ai_baka_deck126-<ptr>-vs-ai_baka_deckM.jsonl`; seqs below are that file's.

## 1. Game by game

| vs | file ptr | result | turn | end life | deciding decisions |
|---|---|---|---|---|---|
| 130 | ...073081 | **WIN** | 13 | 40–0 | seq9 t9 Exquisite Blood, seq12 t11 tutor→Sanguine Bond, seq14 cast it, seq16 t13 Tribute on "Dwarven Blastminer (1/1) is sacrificed" → loop. Rule #2 executed exactly. |
| 123 | ...073085 | **WIN** | 15 | 40–0 | seq14 t9 Bond, seq17 t11 Blood, seq19 Sorin, seq23 t15 `ATTACK: A1` with the lone lifelink Vampire → loop. Rule #2 + Rule #6 win button. |
| 162 | ...102 | loss | 12 | 0–20 | Correct play throughout: seq8 t7 tutor→Bond, seq11 t9 Bond, seq14 t11 Blood at 6 life with `LOOP HALF PENDING` printed. Died turn 12 to Underworld Dreams + Fate Unraveler + Ob Nixilis draw punishers with the pair complete and **no starter** (no Tribute, no Staff, no Sorin, no block window — deck162 never attacked). Not a guide failure. |
| 152 | ...079 | loss | 11 | -1–20 | Colour screw: kept a 2-land seven whose lands were two Woodland Cemetery ({B}/{G}, enter tapped) under Pride Guardian + 2 Perimeter Captain + Sanguine Bond — the guide's third MULLIGAN bullet says KEEP that hand. No {W} until Savannah on t10. **seq7 t10**: at 10 life under `CRACK-BACK NEXT TURN: ... up to 11 ... you would be at -1 or lower; that would KILL you`, took row 3 Idyllic Tutor (taps out, adds no board) over row 1/2/4 blockers. Rule #7 entry 2 ranks the Tutor above every wall and carries **no survival carve-out**; Rule #4 and Rule #5 both have one. See §3. |
| 146 | ...092 | loss | 29 | -1–11 | Both halves cast (Bond t10 seq14, Blood t12 seq15) and **both Kaya-exiled** (t11, t15). From t18 the model sat at 34–38 life vs 17 with two Staff of Nin and never reassembled; retutored Bond at seq65/67 on t28 at 1 life and died t29. Carried-plan corruption (§2 HIGH-1) ran turns 17→26. |
| 125 | ...073 | loss | 61 | 0–52 | The corpus's longest game and the one that pays for the guide edit. Bond+Blood out t18–t20; Blood later hit the graveyard, Bond stayed on the battlefield to the end. Opponent: 24 lands, Elixir of Immortality, their own Staff of Nin, and **Lightmine Field**. Sorin reached **21 loyalty**; the model took `+1: create a 1/1 vampire` every single turn (seq23…seq194) and sent lone Vampires at seq30/155/163/171/178/185 — the log shows `Opponent's Lightmine Field dealt 1 damage to Vampire / Your Vampire died` five times, plus more dying at home to their Staff of Nin. **Sorin's `-2: emblem: "creatures get +1/+0"` was printed as row 2 of that same menu every turn** (seq184 verbatim) and was never taken. |

Card facts verified against the primitives (not Scryfall):
`mtg.txt:67059 Lightmine Field auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)` — N attackers ⇒ N damage to **each**, so a lone attacker takes 1;
`planeswalkers.txt:3058 Sorin auto={C(0/0,-2,Loyalty)}: … emblem transforms((,newability[lord(creature|mybattlefield) 1/0])) forever dontremove` — permanent +1/+0 to your creatures.
A 2/1 Vampire therefore survives both a lone-attacker Lightmine trigger and an opposing Staff of Nin ping; a 1/1 survives neither.

## 2. Engine / interface / card items

**HIGH-1 — `PLAN:` is being used as a reasoning scratchpad, and BOTH the answer parser and the
carry-forward read it. Two distinct harms, one root cause.**

*(a) the executed row is not the model's stated answer.* 7 of 335 windows carry
`latched_coded_line: 2`, i.e. a later coded token in the reply overrode the first `CHOICE:` line.
Repro (file → seq → first reply line → `chosen_text`):

| file | seq | turn | first line the model wrote | row the engine ran |
|---|---|---|---|---|
| …vs_deck146 | 18 | 14 | `CHOICE: 2 (Cast Tribute to Hunger)` | Cast Staff of Nin |
| …vs_deck146 | 36 | 20 | `CHOICE: 2 (Cast Chromatic Lantern)` | Cast Staff of Nin (second copy) |
| …vs_deck146 | 38 | 20 | `CHOICE: 6 (Deal 1 damage with Staff of Nin #2 …)` | Hold priority |
| …vs_deck125 | 42 | 28 | `CHOICE: 1 (+1: create a 1/1 vampire …)` | Hold priority |
| …vs_deck125 | 55 | 30 | `CHOICE: 1 (Cast Tribute to Hunger)` | Hold priority |
| …vs_deck125 | 91 | 40 | `CHOICE: 3 (Cast Idyllic Tutor)` | Cast nothing right now |
| …vs_deck125 | 123 | 46 | `CHOICE: 3 (Deal 1 damage with Staff of Nin …)` | pass |

Every one of the seven is a reply whose `PLAN:` line is a multi-paragraph deliberation stream (the
records carry `[+N bytes written past the PLAN line trimmed from this record]`, N up to 5445 at
deck146 seq18). Two of the seven dropped a Staff of Nin ping (deck125 seq123, deck146 seq38); in the
deck125 game the ping was the only damage the deck ever dealt and the opponent finished on 52.
deck146 seq18's first-line answer was the Rule #7b play (their only creature was Silverquill Silencer,
non-defender) and the engine ran a Staff of Nin instead.
**I could not determine whether these latches are D9 working as designed or a bound failure**: the
recorded `reply` is truncated well before the latching token, so where the plan PARAGRAPH ended is
not visible in the corpus. That adjudication needs the engine seat / a `planBlockEndOffset` probe.
Whichever it is, the *deck-seat* fact stands: the executed row differed from the model's stated
answer in 7 windows, only in the games it lost.

*(b) the carried plan asserts board facts the same prompt contradicts, for many turns.* The engine
quotes the plan's HEAD back as `YOUR PLAN (as you last stated it, …)` with no consistency check
against the battlefield / hand lines it prints two paragraphs above. Repro, all in
`…deck126-0x55591d8b8040-vs-ai_baka_deck146.jsonl`:
- **seq29, t18**: `YOUR PLAN (as you last stated it, 1 window ago on turn 17): With both enchantments (Exquisite Blood and Sanguine Bond) on the battlefield, any life gain wins the game.` — the same prompt's own battlefield line lists neither (Bond exiled t11, Blood exiled t15).
- **seq36, t20**: the carried HEAD is a mid-sentence fragment — `With both Exquisite Blood and Sanguine Bond on the battlefield (Exquisite Blood is in hand, Sanguine Bond was exiled but wait—Sanguine Bond was exiled on turn 11. Let's re-read carefully.`
- **seq61, t26**: plan echo still asserts "Exquisite Blood is in hand" while the prompt reads `Your hand (1 card): Chromatic Lantern`. The model spent that reply re-deriving the truth from scratch.

D16(a)'s size note ("the rest of your plan was not carried: N further characters") fires in these
windows and does not help: the problem is not the *cut*, it is that the surviving HEAD is scratch
text presented as a stated belief. The engine already prints a menu-scoped staleness note
(`note: this decision's list does not contain the actions your plan names`, seq61) — the same
mechanism applied to ZONE facts would kill this: when the carried plan names a card that is on
neither battlefield line and not in hand, say so on the plan line.

**MED-1 — nothing tells the model a permanent it controlled has LEFT.** The prompt has a
`LOOP HALF PENDING` header when a half is on the battlefield and a `LIFE-TO-DAMAGE CONVERTER` header
while a converter is out, but no header fires when a half is exiled or dies. In the deck146 game the
two exiles are visible only as two lines buried in a 100-line GAME LOG (t11, t15), and the model
missed both for eight turns. A one-line `LOOP HALF LOST: your Sanguine Bond was exiled on turn 11 —
neither half is on your battlefield` would be the cheapest possible fix and reuses the state the
converter header already computes.

**MED-2 — the `LOOP HALF PENDING` / converter headers describe the loop but never say it needs a
starter of yours.** deck126 vs 162 seq14 (t11): `…when it closes any life THEY lose, and any life
you gain, chains until they are at 0. You CAN cast Exquisite Blood in this window` — true, and the
model cast it, and then died on turn 12 with the pair complete because the opponent had no attack to
block, it had no Tribute, no Staff, no Sorin and no Vampire. The guide teaches the starter rule
(Rule #2's "BUT THE LOOP NEEDS A STARTER"); the header, read alone, reads like a win.

**LOW-1 — the guide-pool band is saturated.** `deck125 70999`, `deck123 70982`, `deck130 70927`,
`deck126 70903`, `deck146 70343` bytes against a 71 KB ceiling. Every future edit to the top five
guides is now strictly zero-sum — an addition must be paid for by a deletion. Worth the core loop's
attention as a process fact, not a defect.

**No card bug found.** Lightmine Field and Sorin behaved exactly as their primitives and the Oracle
text say; Kaya's `-3` at 3 loyalty (dies, ability still resolves) is correct; Tribute to Hunger's
edict, Perimeter Captain's per-defender trigger and Pride Guardian's 3 all fired as written.

## 3. Guide verdict: **EDIT**

`wave62/deck126/strategy.txt`, started from the live `bin/Res/ai/baka/deck126_strategy.txt`.
**70992 bytes** (live: 70903) — inside the 41–71 KB band, and the pool is at 70999 max, so the edit
is byte-neutral by construction: every addition below is paid for by a deletion of already-duplicated
prose, not of any rule.

Four things the corpus says to KEEP as they are, so they are untouched: Rule #2's win button (it won
both wins outright, seq16/t13 vs 130 and seq23/t15 vs 123); Rule #7's reply-protocol block (0
fallbacks, 0 re-asks, 0 out-of-range numbers in 335 windows — this used to be the deck's biggest
leak); Rule #3's search rule (every one of the 7 tutor picks named the half missing from the
battlefield line); Rule #8's hold row (73 of 234 offered, no dead-window spiral in the 61-turn game).

### Edits

**E1 — Rule #6, after the Lightmine Field stop (line 425).** ADDED:
> THE EMBLEM IS THE WAY PAST IT, AND IT IS ON YOUR OWN MENU. Sorin's `-2: emblem: "creatures get +1/+0"` makes every Vampire a 2/1, and a 2/1 LIVES through the 1 damage a LONE attacker draws. Against a Lightmine Field: take the emblem first, then send ONE Vampire per turn — one attacker is 1 damage and it survives, two attackers is 2 and both die. Until it is on, ATTACK: none.

Paid for by: deck125 seq184 (t58) — the `-2` row printed at 21 loyalty with the `+1` taken instead —
and seq155/163/171/178/185, five lone Vampires killed on declaration by Lightmine Field, opponent's
total never moving. Before, the guide's only answer to Lightmine Field was `ATTACK: none` forever,
which leaves this deck with no clock at all once the opponent stops attacking.

**E2 — the `STAFF OF NIN AND SORIN` section (after line 610).** ADDED a paragraph making the `-2`
emblem a first-class Sorin line: what the row reads, that it costs 2 loyalty and is permanent, and
the trigger condition — *take it once, in the first window that prints it, whenever anything on their
battlefield line kills a 1/1 on sight (a Lightmine Field, a Staff of Nin of theirs, any repeatable 1
damage)*, with the 21-loyalty/eighteen-turn record as the evidence. Before, the section said only
"take +1 every turn" and the `-2` and `-6` did not exist in the guide at all.

**E3 — SITUATIONS, the three Vampire bullets → two bullets + one new one.** BEFORE: three bullets
restating Rule #6's surplus/GANG-BLOCK rules at length. AFTER: the same operative clauses in two
tighter bullets, plus
> Anything of theirs deals 1 damage on repeat to your creatures (Lightmine Field, their Staff of Nin): take Sorin's -2 emblem in its first window — a 2/1 Vampire lives where a 1/1 dies — and until it is on, ATTACK: none.

Same seq citations as E1/E2. Net −0 rules, +1 rule, −230 bytes.

**E4–E7 — the deletions that pay for E1–E3.** No rule removed; only case-history sentences already
stated verbatim elsewhere in the same file: the triple Idyllic-Tutor-pick anecdote in Rule #3 entry 4
(→ one sentence; all 7 picks in this corpus were correct); the two-Tributes-on-one-creature anecdote
in Rule #4 (→ one sentence; 0 occurrences this corpus); the out-of-range-number anecdote in Rule #7
(→ shortened; 0 occurrences this corpus); the Tribute and `[named:]` anecdotes in SITUATIONS that
repeat Rule #4/#7b/#7-entry-1 word for word (→ cross-references); the one-land-mulligan example the
guide itself declares non-load-bearing ("A win with one land does not move this line and neither does
a loss") (→ compressed).

### Considered and NOT edited

- **Rule #7 entry 2 has no survival carve-out** (deck152 seq7: Idyllic Tutor taken at 10 life under a
  printed `this would KILL you` crack-back, over a wall). Real, and it is the one hole in Rule #7 that
  Rules #4 and #5 both plug. Not edited because the game was already lost on that board (11 incoming,
  3 lands, one {W} source, walls that cannot block either flier — a Wall of Omens leaves you at 2 and
  dead next turn), so a single game cannot tell the rule change from the variance. Flagged for a
  future wave if a second window appears where the wall would have mattered.
- **The 152 mulligan** (two tapped {B}/{G} lands under three {W} spells, kept per the guide, never
  drew {W} until t10). Hands are real; one loss does not move a mulligan line.
- **Second-copy Chromatic Lantern** — my first read scored three violations of Rule #7 entry 7; on
  checking `chosen_text` against `latched_coded_line` only the seq52/seq61 casts are real, both on
  menus whose ONLY cast row was that Lantern at 11+ untapped sources. Harmless, and the guide's
  absolute ban is arguably the thing that is slightly wrong. Not enough to pay for an edit.

## 4. Proposals

None. No `general-proposals.md`, no `skill-proposals.md`. HIGH-1's model-side half is already the
subject of lane-Z D9's shipped `kReplyProtocol` sentence ("An answer written inside your PLAN
sentence is part of the plan, not your answer"); restating it in the general guide would be noise,
and the part that is *not* covered — the carried plan asserting contradicted zone facts, and the
7 latched flips — is an engine change, not a guide line. It is filed as HIGH-1 above.

## 5. What I did NOT check

- The opponent seats, except deck162's (read in full to explain the turn-12 loss). deck125's,
  deck146's, deck152's, deck123's and deck130's seat files were not read.
- The hung 152v125 game, `corpus-hang.md`, and the 20260905-161801 rerun — no deck126 pairing.
- Any prediction in `lane-{W,X,Y,Z,AA}.md` — engine-seat work; HIGH-1 touches D9/D16(a) and I have
  deliberately reported it as evidence rather than as a verdict.
- Whether the 7 `latched_coded_line: 2` windows are inside or outside the plan paragraph. The
  recorded replies are truncated before the latching token; this needs a probe, not the corpus.
- Scryfall. Lightmine Field and Sorin, Lord of Innistrad were verified against the primitives only
  (`mtg.txt:67058`, `planeswalkers.txt:3055`); the primitives match the Oracle text as I know it, and
  no disagreement surfaced. Exquisite Blood, Sanguine Bond, Tribute to Hunger, Perimeter Captain,
  Pride Guardian and Overgrown Battlement were read off the prompts' own `{card text: …}` renders,
  not re-verified against the primitive this wave.
- No build, no wagic run, no git, and nothing written outside `wave62/deck126/`.
