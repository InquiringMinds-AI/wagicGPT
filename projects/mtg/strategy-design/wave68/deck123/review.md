# deck123 (Intruders of Thraben, WBU token combo) — wave-68 seat review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/`, six seat files
`*deck123-*-vs-*`. Record 0/6 (wave 67: 3/6). 358 seat records total, 3 attack windows and 1 block
window across all six games.

## 0. The primary question: did the seven cuts in `rewrite/deck123/changes.md` (c) cost the games?

**No. Zero of the six losses is attributable to a cut rule.** Window census for each cut, from the
rendered `prompt`:

| cut rule | windows it could fire in | what happened |
|---|---|---|
| Lightmine Field attack width | vs deck125: the card is on their board from seq76 to seq151 (75 windows) — but deck123 had **0 attack windows in that game** (no `attackers` record; its board was empty from turn 34 on) | UNTESTED — reachable, no window |
| basic-fetch carve-out vs nonbasic-only land destruction | no land destruction in the six opponents' logs | UNTESTED — no window |
| `[from exile]` pricing | vs deck152 seq27, 28, 33, 40, 42: `3. Cast Lightning Greaves {4} [from exile] {… it costs {2} more than printed}` | 5 offers, **0 takes** — declined correctly without the rule |
| `[NAMED BY THEIR …]` surcharge | 0 renders in six games | UNTESTED — no window |
| tutored-Alarm `{U}`-count check | Tutor cast vs deck146 seq27, vs deck125 seq111 | both times `{U}` was available (it cast Vision Skeins `{1}{u}` at deck146 seq30) — the check would have passed |
| edict-vs-planeswalker | vs deck146 seq14-21: Lolth, Spider Queen `[planeswalker]` on their line with a Tribute row live | pilot targeted the **creature** (seq17, killed the Spider) — correct without the rule |
| blocking GANG-BLOCK bullet | one block window in the whole corpus (deck152 seq48), header read `no block saves you` | UNTESTED — no window |

The one cut whose window did arise and where the pilot then misplayed is **annihilator sacrifice
order**: vs deck125 seq143-148 (Emrakul, turn 67) deck123 sacrificed six permanents and gave up
**Intruder Alarm at seq146** among them. It was at 5 life against 89 with an empty board — the
decision cost a permanent, not the game. It is still the one cut worth carrying back, and it is
cheap: see edit E3.

## 1. Game by game — outcome and the deciding decision

**(a) vs deck125 — lost 89 / -10, turn 66** (`1788720093-…deck123-…-vs-…deck125.jsonl`).
Opponent is hard-lock control: Supreme Verdict (seq16), Fall of the Gavel (seq26), Path to Exile
×3 (seq37-38 exiles the Doomsayer), Cancel (seq60), Final Judgment (seq68), Essence Scatter
(seq110), Dream Fracture (seq128), Sphinx's Revelation, Staff of Nin, Emrakul (seq130). deck123
resolved a maker four times and held one for zero turns; its battlefield line reads
`of which 0 are creatures` from seq54 to the end. **Decider: matchup ceiling.** No cut, no
compression, no render fault. Guide break alongside it (does not decide, but wastes the only cards
that could have mattered): Damnation taken at `destroys 0 of their creatures … 0 of yours` at
**seq68, seq94, seq95, seq99** — four takes against the kept rule "N at 0: never".

**(b) vs deck126 — lost 51 / 0, turn 17.** Sanguine Bond resolves by seq32 and Exquisite Blood by
seq33; the loop fires on their turn 17 and takes deck123 from 26 to 1 in one window (seq35 shows
`Opponent life trend: … now 51 (+25)` and `ON THE STACK: 1 damage to you … that would KILL you`).
At seq33 deck123 held 7 mana, 3× Tragic Slip, Idyllic Tutor and Greaves against Wall of Omens
(0/4) and Pride Guardian (0/3) — **-1/-1 kills neither and the deck owns no enchantment removal.**
**Decider: matchup ceiling + variance** (no maker drawn after seq12). Guide breaks alongside:
Tribute to Hunger taken at N=3 (seq15) and N=2 (seq23), both against the kept rule "N not exactly
1: no edict"; both let deck126 choose its worst body while assembling the loop.

**(c) vs deck146 — lost 19 / 11, turn 17. ENGINE.** The game ends with **deck123 at 11 life, its
library at 42 cards, the opponent at 19 with 43 cards, no lethal on the board and no crash**, one
step after Vision Skeins resolved. `game-146v123-1788720096.stderr:1946-1952`:
`Action added to stack: 10DrawAction / Resolving … / Action added … / Resolving …` then
`AIPlayerGPT[ai_baka_deck123]: game end (turn 17, lost)` and
`WAGIC_SELFPLAY_RESULT winner=0 life0=19 life1=11 turn=17`. No loss condition is satisfied on any
printed surface. **Decider: an engine defect** (HIGH-1). Note also that the Idyllic Tutor cast at
seq27 put a `StackAbility. (Source: Idyllic Tutor)` on the stack (stderr:1930) that never prints a
`Resolving` line, and that its find never decremented the library.

**(d) vs deck152 — lost 27 / -3, turn 18.** deck152 curves out (Katilda, Tovolar's Huntmaster,
4 Wolves); seq48 is the only block window in the corpus and its header says
`no block saves you - no assignment of your blockers survives this`, so the block was already
lost. **Decider: outraced — the deck never had a maker before turn 14 (seq28).** Guide breaks
alongside: Devour Flesh taken twice at `they control 7 creatures - they choose which one`
(**seq40, seq42**), the second naming **the opponent**, gaining them 3 (24 -> 27) at deck123's own
6 life — two kept rules broken in one turn ("N not exactly 1: no edict"; "never cast an edict when
their life is at or below the life it gives them" is not the breach here, the N gate is).

**(e) vs deck162 — lost 19 / 0, turn 15. THE DECIDING DECISION OF THE CORPUS.** The engine
*worked*: on turn 9 deck123 built **21 Humans** off Doomsayer + Greaves + Intruder Alarm
(seq21-27), correctly stopped at the stop (`repeat_past_stop` at seq27), and its own PLAN read
"Next turn: Attack with all 21 creatures for lethal" against an opponent at **19 life**.
Then, at **seq34 (turn 10, its own main phase)**, with row 5 rendered
`Cast Damnation … destroys 1 of their creature (1 without a restriction against attacking), 21 of
yours … [<- board sweep … THEIRS 1 / YOURS 21 …]`, the reply was:

> `CHOICE: 5 (Cast Damnation)` / `PLAN: … cast Damnation. … Actually, let's re-read the guide. "M
> at 4+: NEVER, whatever N says. Four bodies is a board you win with." You have 21 bodies. **You
> should NOT cast Damnation. You should attack.** … Opponent life is 19. You have 20 Humans (1/1)
> [+928 bytes written past the PLAN line trimmed from this record]`

The pilot **quoted the correct rule and reversed itself in prose**, but the CHOICE line was
already written, the record shows `reply_truncated 1`, `reply_overrun 2253`,
`post_answer_overrun 2253` — the cap trimmed whatever came after — and `fallback: None`: the
`decision_reversed_in_prose` predicate **did not fire at this CHOICE seam**. Row 5 executed and
swept deck123's own 21 bodies to kill one 3/4. It never had a creature again; the game ended five
turns later at 0.
**Decider: the guide's gate fires too late (it lives in the PLAN line, which is written AFTER the
CHOICE line) + an engine reversal-predicate miss + the reply cap.** Not a cut — the rule is
present and was read; it was consulted one line too late. This is the loss the edit below is for.

**(f) vs deck130 — lost 20 / -5, turn 24.** Burn/Hammer of Bogardan. Doomsayer at seq14 (turn 13)
does not survive; Damnation at seq20 (`destroys 3 … 0 of yours`) is a correct take; from seq22 on
the board is empty and every row on the menu is priced dead. **Decider: variance** (no Alarm+maker
pairing ever assembled). Guide break alongside: seq26 takes
`Cast Lightning Greaves {2} {right now: you control 0 creatures - this equips nothing}` on a menu
whose other rows all read `at 0 this does nothing` — the kept "P8 / this equips nothing" rule.

### Attribution summary (the seat's answer)
| loss | decided by |
|---|---|
| deck125 | variance / matchup ceiling |
| deck126 | variance / matchup ceiling (no enchantment removal in the deck) |
| deck146 | **engine defect** — game terminated with no loss condition met |
| deck152 | variance (outraced) |
| deck162 | **guide-placement + engine** (gate after CHOICE; reversal predicate missed; reply cap) |
| deck130 | variance |

**A cut rule decided none of the six.** The short guide is not what cost the 3/6 -> 0/6 swing; the
swing is one catastrophic decision (deck162 seq34), one engine-terminated game (deck146), and four
games in which the deck never got its engine onto the table.

## 2. Engine / interface / card items

**HIGH-1 — a game ends with no loss condition met.** `game-146v123-1788720096.stderr:1952`
`WAGIC_SELFPLAY_RESULT winner=0 life0=19 life1=11 turn=17`, immediately after two `10DrawAction`
resolutions from Vision Skeins (`all(player) draw:2`, mtg.txt:129547). Both lives positive
(19 / 11), both libraries large (seat record seq30 prints `Your library: 42 cards`; the same
prompt prints `Opponent library: 43 cards`). Repro: replay 146v123 and watch the transition after
the Vision Skeins resolution. This defect cost deck123 one of its six games outright.

**HIGH-2 — `decision_reversed_in_prose` does not fire at a CHOICE seam that explicitly reverses.**
File `1788720122-ai_baka_deck123-…-vs-…deck162.jsonl`, seq34, turn 10, `fallback: None`. The reply
contains the literal string `You should NOT cast Damnation. You should attack.` after
`CHOICE: 5 (Cast Damnation)`. Lane BA/J6 predicts a reversal predicate at CHOICE; here it missed
the clearest reversal in the corpus and the answer that ran lost the game. Same record:
`reply_truncated 1`, `post_answer_overrun 2253`, `max_tokens` cap on the ask seam — the 928+ bytes
trimmed are exactly where a `Correction:` + second CHOICE line would have had to live, so the cap
and the predicate failed together. (Engine seat: this is one decision in which a cap DID lose a
decision, against the corpus-level "the rest kept their answer" reading.)

**MED-1 — Idyllic Tutor leaves an unresolved stack ability and never decrements the library.**
`game-146v123-1788720096.stderr:1929-1930`
(`reveal put 1 of 42 to option one in one reply` / `Action added to stack: StackAbility. (Source:
Idyllic Tutor)`) has no matching `Resolving Action on stack: StackAbility`. In the deck125 game the
same cast at seq111 leaves `Your library: 27 cards` unchanged at seq112-114 (it only drops to 26 at
seq115 on the natural turn-50 draw), so the found card is not being taken out of the printed count.
Primitive `mtg.txt:57031` shows the two-branch `optionone` / `optiontwo` reveal — worth checking
that `optionone`'s `moveto(hand)` and the `and!(shuffle)!` both run.

**MED-2 — the repeat row's emit format costs two windows every time it is used.**
deck162 seq23 and seq25 both return `repeat_count_reask`: the pilot wrote
`CHOICE: 2 (Create human with Thraben Doomsayer, repeated N times, then stop)` — it copied the
ROW'S OWN NAME, which contains the literal letter `N`, instead of substituting. The row's example
string (`e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x<N>)"`) and the row's own printed
name disagree about what the pilot should copy; the CHOICE-format instruction at the foot of the
prompt says to copy "the name only". Cheapest render fix: print the row's short name already
carrying the placeholder (`Create human with Thraben Doomsayer x<N>`).

**LOW-1 — the forced-sacrifice (annihilator) ask offers combo pieces with no verdict text.**
deck125 seq143-148: rows are bare permanents (`Arcane Sanctum [land] [your battlefield] - "…"`,
`Intruder Alarm [enchantment] [your battlefield] - "…"`). Nothing on the row prices what giving up
the Alarm costs, and the pilot fed it at seq146. A `{this is your only …}` annotation of the kind
the cast rows already carry would answer it at the render.

**LOW-2 — `Devour Flesh` row text is correct and matches the primitive** (`mtg.txt:29135-29137`,
`Target player sacrifices a creature, then gains life equal to that creature's toughness`) — no
item; recorded because the guide's "the player you NAME sacrifices AND gains the life" was
re-verified this wave. Damnation, Tribute to Hunger, Intruder Alarm, Thraben Doomsayer (fateful
hour `controllerlife < 6`), Bloodline Keeper (`{B}` + `morethan~4` vampires), Tragic Slip, Idyllic
Tutor and Lightning Greaves were all re-checked against the primitives and match the guide.

## 3. Guide verdict: **EDIT** (three edits, byte-neutral)

`wave68/deck123/strategy.txt`, started from the live
`bin/Res/ai/baka/deck123_strategy.txt` (19,996 B). New size **19,977 bytes** (`stat -c %s`).
A332 grep
(`corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]`)
prints **0**; the frequency-adjective grep prints **0**.

The guide grew by 647 B for the three edits and was cut by 666 B elsewhere (list below), so no rule
was added at another rule's expense — every byte spent came out of duplication or of prose that
restated a printed surface.

### E1 — every gate is a pre-CHOICE comparison (ANSWER FORMAT)
before: (nothing; the section ended at the carried-PLAN bullet)
after:
> `- EVERY GATE IN THIS GUIDE IS READ BEFORE THE CHOICE LINE, NEVER AFTER IT. A row's own numbers -`
> `"M of yours", "they control N creatures", "at 0 this does nothing", "this equips nothing" - decide`
> `whether that row's NUMBER may be written at all. A refusal you write after the CHOICE line, or in`
> `the PLAN, does not run; the row you already named does.`

Paid by **deck162 seq34** (the reply names row 5 and then refuses it in the PLAN), and by the same
shape at **deck152 seq40/seq42** (edict at N=7), **deck126 seq15/seq23** (edict at N=3, N=2),
**deck130 seq26** (`this equips nothing`), **deck125 seq68/94/95/99** (Damnation at N=0). Every
one of those rows carries its own refusing number and every one was taken anyway.
Prediction: on the next corpus, of the cast/edict/Damnation rows whose own `{right now:}` clause
refuses them, takes fall from 10 of 10 named above to 2 or fewer; a hit is a break only if the row
printed the refusing number at the moment of the take. Removal condition (belief-correcting): the
render carries the refusal AND zero such takes across two corpora.

### E2 — the Damnation gate moves above the PLAN template
before: `restriction" is K = 0. Whenever it is listed this is the ONLY plan line:`
after: `restriction" is K = 0. FIRST, BEFORE THE CHOICE LINE, read M and N off that row: M at 4 or`
`more, or N at 0, and Damnation's number is not written this window - answer another row. Only if`
`that comparison passes do you go on. Whenever it is listed this is the ONLY plan line:`

Paid by **deck162 seq34** (M=21, N=1 — the pilot quoted "M at 4+: NEVER" *after* naming the row)
and **deck125 seq68, 94, 95, 99** (N=0, four takes). Damnation offer census: 35 windows, 8 takes,
of which **5 break a rule the guide already states** — the rule is not missing, its position is.
Prediction: Damnation takes at M>=4 or N=0 drop to 0 of the next corpus's Damnation takes; a hit is
a break only where the row printed `of yours` >= 4 or `destroys 0`. Removal condition: none (this
is the deck's single highest-cost row).

### E3 — the forced-sacrifice ask gets its owner line
before: `Cleanup: a listed "copy 2 of 2" …` (the section header said "DISCARD AND SACRIFICE ASKS"
but the body only governed the Cleanup discard)
after: `A forced sacrifice ask (annihilator, or an edict naming you): give up a land, then a spare`
`copy, then a token; never the Alarm, your only maker or Damnation.` + `Cleanup discard: …`

Paid by **deck125 seq143-148**, where six permanents were given to Emrakul's annihilator and
**seq146 gave up Intruder Alarm**. This is the one place a dropped rule (annihilator sacrifice
order) had a real window; it is restored as one sentence, not as the dropped section.
Prediction: at the next forced-sacrifice ask with a land on the menu, the Alarm/maker is not the
answer; UNTESTED if no such ask arises (one window in six games).

### Trims that paid for the additions (all duplication or restated render text)
- `Costs are MANA: {2}{u} is THREE …` -> replaced by the trust-doctrine sentence: the row prints
  `[castable now]` / `[cannot pay now: need …]` and there were zero unpayable picks in six games.
- `The Alarm is symmetric: no creature untaps … ANY creature entering untaps everything` — the same
  fact opens the guide ("any creature entering untaps all creatures") and prints verbatim on the
  battlefield line; the non-obvious half ("it untaps THEIR creatures too") was kept.
- `A token is not a maker - put a Human or Vampire in front of the biggest "deals N"` — STEP 1
  already says tokens are spent before cards.
- `and nothing in this deck removes an enchantment` (Damnation) — CHECK 1 says it.
- Prose compressions with no rule change: the "THAT WOULD KILL YOU" paragraph, CHECK 1 B2,
  CHECK 3, STEP 0b, the BLOCKING header sentence, the fateful-hour note, the P8 sentence, the
  Doomsayer/Lord row list, `Never bottom both ways to find a maker`, and
  `or they are tapped out` in the timing bullet.
- Literal repair (not a trim): the HOLDS section quoted `"Hold priority for the rest of this turn"`,
  a string that renders **0 times** in this corpus — the row reads
  `Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows
  above changes`. The guide now quotes the rendered prefix. (The rule itself held: 101 hold takes
  across 259 hold-row windows; I found no window where a hold demonstrably suppressed a live row,
  though I did not enumerate the 1,029 engine-skipped windows to prove it.)

**Why not KEEP:** KEEP would be the verdict if the six losses had been ceiling and variance alone,
which four of them are. But deck162 seq34 is a decision the guide's own rule answers correctly and
the guide's *layout* answered too late, and the same layout fault is behind eight further
rule-refusing takes. That is a decision the corpus shows going wrong, and it is fixable inside the
byte ceiling.

## 4. What I did NOT check
- I did not determine the mechanism of HIGH-1 (why 146v123 ended). I read the seat records, both
  seats' library/life renders and the tail of the stderr; I did not read `GameObserver` /
  `Player.cpp` loss-condition code, and I did not run the game (the brief forbids it).
- I did not audit the opponents' seat files beyond the windows cited for context.
- I made no per-seat latency or prompt-byte comparison against wave 67 — that is the engine seat's
  lane, and my six games' `latency_ms` were not aggregated (I read only seq34's 199,807 ms).
- I did not verify every literal in the guide against the corpus; I ran the audit on the strings my
  edits touch plus the hold-row literal, and found one dead literal (repaired above).
- No general-strategy or skill proposals: nothing in this seat's evidence is a general fault. The
  pre-CHOICE-gate finding is already the skill's A314 ("Never invite a mid-reply change of mind;
  every check is a PRE-condition on the CHOICE line") — restating it would be noise.
