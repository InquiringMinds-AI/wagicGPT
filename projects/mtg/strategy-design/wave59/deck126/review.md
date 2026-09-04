# Wave-59 per-deck review — deck126 "Sanguine Blood" (Modern WGB combo)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/`, binary master `5039427da`.
Six games, all natural, 0 crashed, 0 hung. Record **3-3**. Seat files are named
`<epoch>-ai_baka_deck126-<ptr>-vs-ai_baka_deck<N>.jsonl`; every seq below is that seat's log
unless stated. Card facts verified against `bin/Res/sets/primitives/mtg.txt` (`text=`/`auto=`).

## 1. Game by game — the deciding decisions

| opp | result | turn | end life | decided by |
|---|---|---|---|---|
| 162 | **WIN** | 15 | 40 / 0 | combo assembled and fired, textbook |
| 152 | **WIN** | 14 | 34 / -1 | Exquisite Blood T12 + Sanguine Bond T14 + Sorin -6 |
| 123 | **WIN** | 29 | 28 / -1 | Bond T7, Staff of Nin pings + lifelink Vampires as the drain |
| 130 | LOSS | 16 | 0 / 19 | 6/5 flier vs an all-`[defender]` ground board; no reach in the deck |
| 146 | LOSS | 19 | -2 / 21 | Kaya exiled Sanguine Bond T13; Soul Shatter took Perimeter Captain |
| 125 | LOSS | 87 | 3 / 95 | Cancel, Dream Fracture and Fall of the Gavel countered Blood T10, Bond T12, Tutor T18 |

**126v162 (win).** s11 Tutor -> Sanguine Bond, s14 Tutor -> Exquisite Blood, s17 cast Bond (T11),
s23 cast Blood (T13), s27 Tribute to Hunger at T15 on a 1-creature board -> life gain -> loop ->
opponent to 0, seat to 40. Guide Rules #3 and #4 executed exactly as written. The Sanguine Bond /
Exquisite Blood loop **does** run in this engine; the two wave-58-era doubts are discharged here.

**126v123 (win).** Bond down T7 (s9), then the guide's "a Bond alone is already a clock" line
carried the game: 7 Staff of Nin upkeep pings (s17-s57) each drained 1 more via the Bond, Sorin
vampires (lifelink) attacked s49/s55/s63. s29 cast a SECOND Sanguine Bond — correct play, and
against the guide's own blanket "a second copy of either enchantment does nothing at all" (see §3).

**126v130 (loss).** s21 Bond resolves T11; opponent lands Rorix Bladewing (6/5 flying haste,
primitive line 96836) T14. Four `[defender]` walls, none with reach or flying, so the engine never
opened a blockers window — correct, not a defect. s25/s26 (T15, 12 life) the hand is one redundant
Chromatic Lantern; there was no play. Variance/deck-construction, not guide.

**126v146 (loss).** Both halves never coexisted: Bond cast T12 (s13), **exiled T13** by Kaya the
Inexorable -3; Blood recast T16 (s19) with the Bond gone. Perimeter Captain (s3) had already died
to Soul Shatter, so the 3-defender block at T17 gained only Pride Guardian's 3. The fatal window is
s22 (§2, MED-1). No guide-attributable error.

**126v125 (loss).** The seat drew and cast the pair on curve and lost all of it to counterspells:
Exquisite Blood COUNTERED by Cancel T10, Sanguine Bond COUNTERED by Dream Fracture T12, Idyllic
Tutor COUNTERED by Fall of the Gavel T18. Guide Rule #3.5 ("if a piece is countered, tutor for the
next copy and cast it again") was followed each time. Every one of those casts was made into a
board line printing `Their untapped sources: 4 (colours they could make: {u}{w})` with 6-7 cards in
their hand — a real STRATEGY seam, but the guide's opposite teach (Rule #3.1, "the turn either one
appears in your casting list, CAST IT") is deliberate and I do not have corpus evidence that
holding would have been better against a deck with four counterspells and Kaya-class exile. The
long tail (turns 32-87) was already lost: Sorin +1 vampires vs two Staffs of Nin.

## 2. Engine / interface items

**HIGH-1 — the carried `YOUR PLAN` echo is unbounded and re-serves board claims the same prompt
contradicts; cost one window 2 extra model calls (137.0 s).**
Repro: `1788544893-ai_baka_deck126-0x556d186614d0-vs-ai_baka_deck125.jsonl`, **seq 47 -> 48 -> 49 ->
50**, turn 32 Main 1.
- s47 (72.9 s) the model casts Exquisite Blood; it resolves.
- s48's prompt narration prints `- You cast Exquisite Blood` / `- Your Exquisite Blood resolved and
  entered the battlefield`, and the battlefield line ends
  `...; Exquisite Blood {4}{b} [enchantment] {effect: "Whenever an opponent loses life, you gain that much life."}`.
  Four lines below, the engine re-serves the model's own previous 1,236-character deliberation
  stream as
  `YOUR PLAN (as you last stated it, 1 window ago on turn 32; "Sanguine Bond" is no longer on your menu): ... No Sanguine Bond. No Exquisite Blood. I just drew Exquisite Blood. ... Wait, I have Sorin. ...`
- s48 reply: `CHOICE: 4 (Cast Exquisite Blood)` on a **three-row** menu -> `named_row_reask`
  (51.6 s wasted), s49 `recovery`, s50 re-ask (85.3 s) answering row 2.
Scale in this seat: 410 plan echoes, **148 over 400 chars**, 46 carrying live deliberation markers
(`Wait,` / `?` / `But I `), longest 1,599 chars (126v125 s52). The plan channel is the one part of
the prompt the engine copies back verbatim without vetting, so under the trust doctrine it is
functionally a false surface competing with a true one. Concrete asks: cap the plan at emit (first
sentence, or N chars), and drop the echo entirely when the board line contradicts a card-presence
claim in it — or at minimum stop re-serving a plan that names a permanent the battlefield line
prints. This is the whole cost of the s48-s50 window and is invisible to the fallback counter
(one `named_row_reask` records as one fallback, not as 137 s).

**HIGH-2 — the GAME LOG is 76 % of every prompt byte this seat sent, with no compaction.**
Repro: same file. Prompt bytes across the game = 5,830,144, of which the region before
`--- CURRENT SITUATION ---` = 4,443,187 (**76 %**). Growth: seq 13 (turn 10) 2,453 of 6,328 = 39 %;
seq 50 (t32) 12,125 of 17,394 = 70 %; **seq 120 (t51) 21,923 of 28,198 = 78 %**; seq 245 (t85)
20,994 of 27,843 = 75 %. The final log is 518 lines, of which 50 are `- Opponent drew a card`,
31 `- Phase: Draw`, 28 byte-identical `- Opponent used: Deal 1 damage with Staff of Nin #2
targeting you`, 16 identical `- Your Sorin, Lord of Innistrad created a 1/1 Vampire token`
triplets. Median prompt in that game = 26,998 bytes for choices like
`1. Cast Chromatic Lantern / 2. Cast nothing / 3. Hold priority`. Against the owner's standing
criterion (decision value per token) this is the largest single lever in the seat's corpus. The
cheap concrete version: collapse an exactly-repeated log line to `x N` the way the board line
already collapses `Scrubland #1-#3 x3`, and drop `- Opponent drew a card` / bare `- Phase: Draw`
when the turn header already carries them.

**MED-1 — the `INCOMING THIS COMBAT` parenthetical claims blocking capacity it does not have,
and the model visibly burned a reply resolving the contradiction.**
Repro: `1788544856-ai_baka_deck126-0x558d6662acc0-vs-ai_baka_deck146.jsonl`, **seq 22**, turn 19,
9 life, one blocker vs five attackers. Rendered line:
```
INCOMING THIS COMBAT: 5 attackers, 18 unblocked damage - you would be at -9; this KILLS you (your creatures may legally block every attacker in that total) - best case with every blocker assigned: you would be at -5; no block saves you
```
The seat has exactly ONE creature (`B1. Pride Guardian (0/3) [defender]`). The clause is presumably
about per-attacker legality (no evasion excludes any), but as printed it reads as capacity. The
model's reply opens with the illegal `BLOCKS: B1:A1, B1:A2, B1:A3, B1:A4, B1:A5`, then:
"*I cannot assign it to five attackers ... Wait, the prompt says 'Assign each blocker to AT MOST
ONE attacker'*", and only then reaches `BLOCKS: B1:A1`. The parser's first-wins/dedupe rules
recovered the right assignment, so no game state was harmed — but a whole reply's reasoning budget
went on a contradiction the render created. The clause prints on **4 of 6** INCOMING windows in
this seat including this one, with no blocker-count condition; the evasion branch (126v152 s24,
`of that, 6 from 1 attacker none of your creatures can block`) shows the emitter already knows how
to qualify itself. Ask: make it capacity-aware — "each of those attackers is legally blockable, but
you have 1 blocker for 5 attackers, so at most 1 can be blocked".

**MED-2 — an Idyllic Tutor search renders the entire library for a 3-way choice.**
Repro: `1788544900-ai_baka_deck126-0x557414cf3010-vs-ai_baka_deck162.jsonl`, **seq 14**, turn 9.
43 numbered rows, ~4.5 KB, of which **40 carry `[does NOT qualify - goes to "shuffle"]`** and 3 are
`[eligible for "choose card"]` (rows 23, 25, 34). Every ineligible row still prints its full
`{text: ...}` — three Wall of Omens rules texts, four Chromatic Lantern rules texts, the lands.
Deck126 runs 4 Idyllic Tutors, and this shape appeared 6 times in the slate. The information the
decision needs is the 3 eligible rows plus the count of what is being shuffled. Ask: collapse
ineligible rows to a single summary line (`40 other cards do not qualify and go to "shuffle"`) and
keep full annotation only on eligible rows. (Sibling of wave-58 deck126 MED "search rows unpriced";
that one did not recur in a decision-relevant form here.)

**LOW-1 — K8's `CRACK-BACK NEXT TURN` upper bound excludes visible repeatable direct damage, and
its "you would be at K" phrasing invites a survival read.**
Repro: `1788544853-ai_baka_deck126-0x56432abf3e30-vs-ai_baka_deck130.jsonl`, **seq 25 and 26**,
turn 15, 12 life:
`CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack (tapped ones untap first), for up to 9 - you would be at 3`
The same board line prints `Pyrite Spellbomb {1} [artifact] {effect: "{R}, Sacrifice ...: deals 2 damage to any target..."}`
and `Siege-Gang Commander {3}{r}{r} (2/2)` (primitive: `{1}{R}, Sacrifice a Goblin: deals 2 damage to
any target`). The seat took 12 the following turn and died at turn 16. Lane J scoped the number to
attackers deliberately and worded it as an upper bound, so this is a scope note, not a false
statement — and **no decision turned on it** (the hand was one redundant Chromatic Lantern). Filed
only because the "you would be at 3" tail is the part a pilot acts on.

**Observations for the engine seat (not defects).**
- K8 renders 58 times in this seat, **all on the seat's own turn, all Main 1 (54) / Main 2 (4)**;
  0 on the opponent's turn. Its attackers-seam form went UNTESTED here: the seat's 12 attackers
  windows all had 0 opposing creatures.
- K10: **0 byte-identical repeated ask windows** anywhere in the slate (keying on the rendered
  block below `--- CURRENT SITUATION ---` with the decline counter stripped). The wave-58
  126v130 shape did not recur. `identical_ask_answers_reserved` = 6 in 126v123, 0 elsewhere;
  `identical_option_asks_resolved` = 0 everywhere (expected — different mechanism).
  `[you declined this exact list N times already this turn]` reached 18 in 126v123 (t23, t27) and
  126v125 (t30), but those are engine decline counts, most of them absorbed by the HOLD row —
  126v125 skipped 895 hold windows and 308 mana-only windows against 201 asks.
- Discard channel: the one `{dead right now:}` badge in the slate (126v162 s26, Tribute to Hunger,
  `they control 0 creatures - at 0 this does nothing`) is a genuine zero verdict. Wave-58 HIGH-4
  did not recur here.
- 0 `empty_reply`, 0 `unparsed_reply`, 0 `transport` failures, 0 `async_drops`, 0 wall misses,
  0 timeouts across all six games; the only fallback in the slate is the `named_row_reask` of HIGH-1.

## 3. Guide verdict — **KEEP as is**

`bin/Res/ai/baka/deck126_strategy.txt` (70,997 bytes, top of the 41-71 KB band) is not edited this
wave. It earned that:
- All three wins are the guide's own script executed line by line — Rule #3 ("the turn either one
  appears in your casting list, CAST IT"), Rule #3.4 (read the battlefield line, pick the OTHER
  half: 3 of 3 tutor picks correct this corpus), Rule #4 (Tribute to Hunger cast on the row that
  names the victim), and the "a Bond alone is already a clock" teach that won 126v123 outright.
- All three losses trace to opposing cards, not to a guide teach: an unanswerable flier, Kaya's
  exile, and three counterspells. No decision in the corpus went wrong *because* of what the guide
  says, which is the bar this wave sets for an EDIT.
- The one model-side blunder (126v125 s48) is already covered verbatim by Rule #3's "Read both
  names off your battlefield line"; it was caused by the stale plan echo (HIGH-1), and guide prose
  against a contradicting rendered surface loses. Routing it to the guide would be the exact
  perception-vs-strategy mis-route the skill warns about.

One residual noted for a future wave, deliberately NOT edited because nothing in this corpus paid
for it: Rule #3 states "a second copy of either enchantment does nothing at all" and, eleven lines
later, "a SECOND Bond doubles it". The second sentence is the rules-correct one (two Sanguine Bonds
give two triggers), and the model already played it correctly in 126v123 s29 — so there is no
misplay to cite, and the guide is at the band ceiling. If a later corpus shows a declined second
Bond, that is the seq that buys the edit.

## 4. Proposals

None. No general-strategy change and no strategy-writing-skill amendment is supported by this
seat's evidence — HIGH-1, HIGH-2, MED-1 and MED-2 are all engine/render surfaces, which is where
they are filed. (No `general-proposals.md` / `skill-proposals.md` written.)

## What I did NOT check

- I did not read the wave-58 deck126 guide history or diff the guide against its wave-58 version; I
  read the live guide only.
- I did not adjudicate any lane prediction as PASS/FAIL — that is the engine seat's job. The K8/K10
  numbers above are this seat's counts only, not a corpus-wide verdict.
- I did not read the opponents' seat logs except for the six shared narrations inside my own seat's
  prompts, so opponent-side render defects are out of scope here.
- I did not verify the counterspell/exile cards of decks 125 and 146 against their primitives; I
  took the narration's own event lines for them.
- I did not reproduce HIGH-1, HIGH-2, MED-1 or MED-2 against the source (no code read, no build, no
  run) — each item is evidenced from the rendered `prompt` text only, and the emitter sites are
  unnamed on purpose.
- I did not measure whether the 126v125 counterspell losses would have changed under a
  play-around-counters teach; that is an untested hypothesis, not a finding.
