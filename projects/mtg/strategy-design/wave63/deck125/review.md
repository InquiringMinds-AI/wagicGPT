# wave-63 deck125 review — Revelation Fracture (Modern UW Control)

Seat logs: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-191148/*deck125-*.jsonl` (binary 98d13050f).
Six games, all natural, 0 crashes, 0 hangs. Record **4-2**. 1033 model decisions, 1 fallback
(`distinct_index_reask`, 125v146 seq 121 — the guard fired correctly on a reply repeating index 11
three times). Median latency 3.2 s (125v126) to 29 s (125v162).

## 1. Game by game

| game | result | turn | end life | the decision that decided it |
|---|---|---|---|---|
| 125v162 | **LOSS** | 23 | 0 / 20 | variance + one keep: seq 2 kept a 1-land 7 (Plains + Cancel/Verdict/Final Judgment/Lightmine/Path/Elixir); no second land until T7 (seq 25). By T18 Ob Nixilis + Howling Mine + Teferi's Puzzle Box were down and a single draw step went 12 -> 4 (seq 52-54). |
| 125v123 | WIN | 31 | 40 / 0 | mulliganed to 6 (seq 2), curved out, two Staffs online T~20; 18 face pings closed it. Clean game. |
| 125v146 | WIN | 75 | 94 / 19 | six Supreme Verdicts / two Final Judgments held a Lolth+Barrowin board; nine Revelations paid for it; Staff pings finished. |
| 125v130 | WIN | 50 | 77 / -3 | Essence Scatters + one Final Judgment, then Staff clock; Emrakul cast at seq ~100 was decoration, the Staff had it. |
| 125v126 | **LOSS** | 68 | 90 / 23 | **decked itself while winning** — seq 527/528, see HIGH-1. |
| 152v125 | WIN | 37 | 25 / -2 | Path/Scatter on Sigarda's support, two Staffs, 14 pings. |

The only loss that was not variance is 125v126, and it was a loss **from 70 life, 47 ahead, with
the opponent on 13 cards of library**. That decision is item HIGH-1 and it is what this review is
mostly about.

## 2. Engine / interface / card items

### HIGH-1 — the X "best/largest affordable" marker is computed from mana alone and calls a self-milling X the best one
`1788653533-ai_baka_deck125-0x5588e7d58200-vs-ai_baka_deck126.jsonl`, **seq 527** (cast row) and
**seq 528** (X menu), turn 68, life 70 vs 23, board line printed `Your library: 12 cards`.

seq 527 rendered:
```
2. Cast Sphinx's Revelation {u}{u}{w}{x} {X pricing: max affordable X=20 (23 mana total); ...}
   [<- best X for this cast: X=20 - largest affordable X - X=20 gains 20 life and draws 20 cards;
   no listed X does more]
```
seq 528 rendered:
```
1. X = 20 {X pricing: X=20 - you gain 20 life and draw 20 cards} {leaves 0 of your 23 floating
   mana unspent - this X spends your whole pool} [<- largest affordable X - X=20 gains 20 life
   and draws 20 cards; no listed X does more]
```
The model answered `CHOICE: 1 (X = 20)`, went to 90 life, and lost on the draw (`gameend` seq 529,
`won:false`, `my_life:90`).

Two false statements in one marker, on a 12-card library: **"best X for this cast"** and **"no
listed X does more"**. Under the trust doctrine a rendered verdict is an instruction; this one
instructed a lethal line while the true fact (`Your library: 12 cards`) sat ~3 kB earlier in the
same prompt, unlinked to the option. Fix shape: clamp `max affordable X` / the "best X" marker to
`library - 1` (minus one per untapped Staff of Nin, which draws at upkeep), and print the binding
reason on the row (`{library ceiling: 11 - X above this draws you out and you lose}`). Do **not**
just delete the marker — silent omission is worse; render the true ceiling. This is one card in one
deck but the emitter is generic (any `draw:X` announcement), so it is a corpus-wide class.

### HIGH-2 — "(this ask has no pass row)" is printed on 846 of deck125's 1033 windows, 680 of which DO offer a pass row
`src/AIPlayerGPT.cpp:307` `kNoPassRowFact`; selected at `:28679` by the `declineRowOffered` flag
added in wave 62 (D5). Only the menu family passes it (`:32875` `req.canDecline`). The CAST seam
(`:30706`, `askModel(q.str(), menu, false)`) and the priority seam do **not**, so their menus —
which end in `Cast nothing right now` and `Hold priority: pass now, and do not ask me again ...` —
are capped with the sentence "this ask has no pass row".

Counts over deck125's six games: 843 windows offered a `Cast nothing`/`Hold priority` row; **680 of
them carried "(this ask has no pass row)"**. Only 24 windows got the true alternative
("the LAST row of this menu declines: it is a real answer, not a fallback").

Repro: `1788653533-...-vs-ai_baka_deck126.jsonl` seq 201, turn 30 — options 1 Cast Path to Exile,
2 Cast nothing right now, 3 Hold priority ...; and 12 lines later
`... write CHOICE: followed by the number of your choice (this ask has no pass row) ...`.

This is a flat contradiction on 4 out of 5 decisions, and it is the best available explanation for
HIGH-3. Fix: pass `declineRowOffered = true` from the cast/priority seams (or add a third fact
string naming both decline rows).

### HIGH-3 — the hold row is still not taken: 390 tagged windows, 18 holds
Windows carrying `[you declined this exact list N times already this turn ...]` across the six
games: **390; the hold was taken 18 times (4.6%)**. Per game: 125v126 292/15, 125v146 44/1,
125v162 26/0, 125v130 12/2, 125v152 11/0, 125v123 5/0. The deck125 guide has taught this since
wave 62 in its loudest paragraph and the rate moved from 2% to 4.6% — **prose is not the failing
layer**, which is why I am NOT proposing a general-guide change for it.

Two mechanical causes visible in the render, both cheap:
1. HIGH-2 above tells the model there is no pass row.
2. The two decline rows are ordered **weaker first**: `2. Cast nothing right now` then
   `3. Hold priority: ...`. They are the same pass; only the second suppresses the next identical
   ask. The model takes the earlier one (286 opponent-turn `Cast nothing` answers in 125v126
   alone). Suggested fix: when the `declined this exact list N times` tag fires, stop offering
   the plain decline at all — the hold IS the decline at that point — or auto-hold after N.

Cost in inference: 544 of deck125's 1033 decisions (53%) were `Cast nothing right now`; 350 of
those were opponent-turn windows. 125v126 spent 380 no-op decisions on a board it was never losing.

### MED-1 — the tapped-land row is annotated with its cost and not its value; 44/44 declined land drops were tapped rows
`1788653528-ai_baka_deck125-0x557a449671c0-vs-ai_baka_deck146.jsonl` seq 148, turn 52:
```
1. Play Seachrome Coast [enters TAPPED - it makes no mana this turn (you control 20 other lands):
   "Seachrome Coast enters tapped unless you control two or fewer other lands."]
2. Play no land right now
```
`CHOICE: 2`. Across the six games deck125 declined 44 land drops. **All 44 were rows tagged
`[enters TAPPED]`; zero untapped rows were declined.** The tag is true and it is a complete
description of the turn it is played and of nothing after it. Because this deck's X spells and its
{15} Emrakul both scale off land count, and lands in hand are cleanup-discard fodder, the decline
is a small but systematic loss. Fix shape: extend the tag, e.g. `[enters TAPPED - no mana this
turn; an untapped source from your next turn on]`. Same shape as the wave-31 "can attack next turn"
finding: the annotation must be restriction-first but must not omit the residual value.

### MED-2 — the hold row can be taken on your own upkeep and covers the rest of your own turn
`...vs-ai_baka_deck126.jsonl` seq 526, turn 68 Upkeep, own turn: the model took
`Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows
above changes`. The row it was holding away included `Life with Elixir of Immortality [cost: {2},
Tap]` — the activation that shuffles the graveyard back and would have prevented HIGH-1 two
windows later. A hold taken in an UPKEEP window is a hold over your own main phases. Worth a look
at whether the hold should be offered at all in your own pre-main windows, or should expire at
your main phase.

### LOW-1 — `distinct_index_reask` fired correctly
125v146 seq 121: reply `PUT: 11, 11, 11, 12, 13, 15, 1, 7`. Refused, re-asked, game continued.
Recorded as the guard working, not a defect.

### LOW-2 — the PLAN channel is inert for this deck
The deck125 guide (line 37) supplies a verbatim PLAN sentence and the model echoes it unchanged;
in 125v162 the identical PLAN line appears on the mulligan ask (seq 2) and on every one of the
next 59 windows. That is the guide's deliberate design, but it means the carried-plan mechanism
costs prompt bytes on every window and carries zero decision-relevant state for this seat. Noted
for whoever prices the plan carry, not a defect.

## 3. Guide verdict: **EDIT** — `wave63/deck125/strategy.txt` written (70985 bytes, live guide was
70962; pool band ceiling observed at 70999)

Every edit below either **removes a claim this corpus falsified** or **resolves a contradiction
between two of the guide's own rules**. No edit adds a new teach against a surface that contradicts
it (HIGH-2/HIGH-3/MED-1 are routed to the engine instead, deliberately).

**E1 — the largest-X absolute now sits under the library ceiling, not above it.**
before (X section):
> `YOUR X IS THE LARGEST NUMBER ON THE MENU. Answer OPTION 1, every time, with no arithmetic in front of it.`
after:
> `READ ONE LINE BEFORE YOU ANSWER: "Your library: N cards" ... Subtract 2, and one more for EACH Staff of Nin ... THAT NUMBER IS YOUR CEILING, and it outranks everything else on this page ... At 125v126 seq 528 you were at 70 life, ahead 70 to 23, with "Your library: 12 cards" printed above the menu, and you answered option 1 for X = 20 ... WITH THAT CEILING SATISFIED, YOUR X IS THE LARGEST NUMBER ON THE MENU.`
Paid by **125v126 seq 528**. The guide already carried the library floor — but three sections
lower, as bullet one of "THE TWO THINGS THAT CAN LOWER IT", underneath an absolute that said "with
no arithmetic in front of it". The exception has been hoisted above the imperative it excepts.

**E2 — deleted a receipt this corpus falsified.**
before: `Every X menu this corpus was answered option 1. The ceiling is held and the answer here is never the problem; an X=1 menu was wrong one decision EARLIER ...`
after: `THE MARKER ON OPTION 1 DOES NOT READ YOUR LIBRARY. "[<- largest affordable X ... no listed X does more]" ... are computed from your MANA alone; at 125v126 seq 527 that marker called X = 20 the best X on a 12-card library ...`
Paid by **125v126 seq 527**. "the answer here is never the problem" is now false and was the
sentence standing between the pilot and its own floor. The library bullet below it was folded into
E1 rather than duplicated.

**E3 — the Staff-ping absolute now names the one row that outranks it.**
before: `- Keep taking the "Deal 1 Damage with Staff of Nin ..." option in every window it is offered, including upkeep, and with EVERY Staff you control.`
after: same, plus `THE ONE ROW THAT OUTRANKS THE PING is a "Life with Elixir of Immortality" row in the SAME window while "Your library: N cards" reads 16 or less ...`
Paid by **125v126 seq 508, 524 and 125v130 seq 97** — three windows at library 15/15/13 with an
untapped Elixir row on the menu, all answered with the Staff ping. The guide's Elixir section
already said the Elixir goes "ahead of a Staff ping"; the Staff section said "every window" with no
carve-out. Two guide rules, direct collision, and the unconditional one won three times out of
three. (Corpus total: 7 windows at library <= 16 with an Elixir row offered, 2 activations.)

**E4 — the land receipt corrected, and the tapped case named.**
before: `Every land window this corpus was answered with a land - at its ceiling, nothing to think about.`
after: `THE ONE ROW YOU KEEP DECLINING IS THE TAPPED ONE. Every land you passed this corpus - 44 of them, 29 in 125v146 alone (seq 148 ...) - was a row tagged "[enters TAPPED ...]". That tag is true and it is not a reason ...`
Paid by **44 declines, 125v146 seq 148 quoted**. This one does add a teach against a surface, but
the surface is not contradicting it — the tag omits the next-turn value rather than denying it, so
the guide can supply what the render leaves out until MED-1 lands.

**E5 — the hold receipt refreshed to this corpus's numbers.**
before: `204 windows carried that tag this corpus and you held on FOUR - 147 of the misses are one game, 125v126, which ran 60 turns and 324 decisions ...`
after: `390 windows carried that tag this corpus and you held on EIGHTEEN - 277 of the 372 misses are one game, 125v126, which ran 68 turns and 511 decisions ... "Cast nothing right now" and the hold row are both passes; only one of them stops the next identical ask.`
Paid by the counts in HIGH-3. Numbers only plus one clarifying sentence; no new imperative, because
HIGH-2/HIGH-3 say the layer is the render.

**Budget:** E1-E5 added ~1.9 kB; to stay in band I cut the same amount from (a) the "THE MENU'S OWN
LINE TELLS YOU WHY" paragraph (its content is on every row's own annotation), (b) the FORCED
FRUITION section, for a card that has now not appeared in **three** corpora (kept as a compact
rule, not deleted), (c) three prior-corpus anecdotes compressed without losing their receipt, (d)
the "145 of 154 Revelation rows" statistic, whose rule survives without the count. Guide verified:
831 lines, 0 U+FFFD bytes, no `#HINT` lines touched (they live in `deck125.txt`).

## 4. Optional proposals

**None.** No `general-proposals.md`, no `skill-proposals.md`.
- The general guide has no hold-priority teach, so a proposal was available — but this deck's guide
  already teaches it as hard as it can be taught and moved the rate 2% -> 4.6%. Adding the same
  prose one layer up is prose against a surface that says "this ask has no pass row". HIGH-2/HIGH-3
  are the fix; the general guide is not.
- The skill lesson my E1/E3 edits embody — *an unconditional imperative will beat its own exception
  three sections later; hoist the exception above it* — is already amendment 70 ("AN EXCEPTION
  LIVES ABOVE THE IMPERATIVE IT EXCEPTS, AND AN UNCONDITIONAL RUNG ..."), and the largest-X case
  specifically is already amendment 14 ("UNCONDITIONAL NUMERIC TRIGGERS AGE BADLY. Wave-40's
  celebrated largest-X ..."). Restating either would be noise. What this corpus adds is
  *confirmation*, which the engine seat can carry.

## 5. What I did NOT check

- The **opponent seats' translogs**: read only for the 125v126 gameend cross-check. Opponent
  decision quality, and the 126/146/130/152/162/123 sides of these six games, were not traced.
- The **lane predictions** in `wave63/lane-{AB,AC,AD,AE,AF}.md` — brief assigns those to the engine
  seat; I read none of them and adjudicated none. My HIGH-2 may overlap lane AD's E10 (per-seam
  HOLD) and my HIGH-1 may overlap lane AF's X-cap item; the engine seat should reconcile.
- `wave63/known-bugs.md`, `codex-review.md`, `corpus-hang.md` — not read.
- **`softlock-blockers-1788650768.md`**: no citation possible from this deck. deck125 is creature-
  less apart from an Emrakul; across the six games it received **zero `blockers` decisions** and two
  `attackers` decisions, so no window of the reported shape arose in my seat.
- The two hung games (152v162, 152v130) and their reruns — not deck125 pairings.
- I did **not** verify the deck125 guide's card facts wholesale. I verified against
  `bin/Res/sets/primitives/mtg.txt` only the three cards my edits depend on: Sphinx's Revelation
  (`auto=life:X && draw:X`, `mana={X}{W}{U}{U}`), Elixir of Immortality
  (`auto={2}{T}:life:5 && moveTo(mylibrary) all(this) && moveTo(mylibrary) all(*|mygraveyard) &&
  shuffle`), Staff of Nin (upkeep draw + `{T}` 1 damage, from its rendered option text and the
  guide's existing quote). Scryfall was not queried; the primitives agreed with the rendered text
  in every case, so there was nothing to reconcile.
- No engine source was modified and no game was run; `src/` was read only to locate the HIGH-2
  emitter (`AIPlayerGPT.cpp:307`, `:28679`, `:30706`; `AIPlayerGPT.h:381`).
