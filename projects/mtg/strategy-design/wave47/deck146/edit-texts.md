# deck146 - wave-47 edits (6), before -> after, each with its finding

Baseline: the LIVE guide `bin/Res/ai/baka/deck146_strategy.txt` (38,564 bytes = wave-46 reviewer
revision + boundary-pass C4). After: 41,735 bytes (**+3,171**). 0 deletions.
Corpus `matchups-20260826-182155`. Card facts re-verified against
`bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep`.

---

## 146-A (HIGH) - the lifegain-wall test could not fire on a collapsed tag, and knew nothing about a converter
**Finding:** `146 vs126` seq **50** (turn 18, life **18** vs 9). Sanguine Bond on their board; the
prompt printed the converter block, the fold `(lifelink: they gain 1, and their converter takes 1
off you)` on each A-row, and the attackers footnote. Every A-row's blocker list was COLLAPSED to
`they have 5 untapped creatures that could block this one, biggest Vampire (1/1)`. The seat sent
all five attackers. Between seq 50 and seq 52 the record reads **18 -> 7** for this seat and
**9 -> 20** for deck126: eleven life, in one swing, and the seat lost at **-1** two turns later.
The hidden defenders were `Perimeter Captain {w} (0/4)` (`auto=@combat(blocking)
source(creature[defender]|mybattlefield):may life:2 controller` - fires for EVERY defender they
block with, not just itself), `Pride Guardian {w} (0/3)` (`:life:3 controller`) and two
`Wall of Omens (0/4)`. `Sanguine Bond` = `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
opponent`, verified. Two separate defects met here:
  (a) the guide's test was **"if TWO OR MORE blockers on this creature's tag print a
      (blocking trigger: clause"** - a collapsed tag prints exactly ONE blocker, so the threshold
      is unsatisfiable by construction on every wide board;
  (b) the fold's new string is `(lifelink: they gain N, ...)`, not `blocking trigger:`, and the
      guide told the model to look only for `blocking trigger:`.
CLASSIFICATION: **STRATEGY** (the render printed everything; see E-1 for the collapse's own share).

- **before:** `THE TEST IS THAT CLAUSE, NOT A BODY COUNT. If two or more blockers on this
  creature's tag print a "(blocking trigger:" clause, the swing PAYS them ... If NO blocker on the
  tag prints that clause, their walls cost them nothing ...`
- **after:** `THE TEST IS THAT CLAUSE, NOT A BODY COUNT - AND ONE PRINTED CLAUSE IS ENOUGH. If ANY
  blocker on this creature's tag prints a "(blocking trigger:" clause or a "(lifelink: they gain"
  clause ...`, plus two new paragraphs: **A CONVERTER OF THEIRS TURNS EVERY ONE OF THOSE PAYMENTS
  INTO DAMAGE ON YOU** (with a converter of theirs printed, any priced attacker stays home; only
  bodies the tag says cannot be blocked at all go) and **AND THE PRICE YOU CAN SEE IS NOT THE PRICE
  YOU PAY WHEN THE TAG IS COLLAPSED** (read N, then read their battlefield line for the defenders
  the collapse hid; price the swing as paying for every defender on that line).

## 146-B (HIGH) - `Cast nothing right now` is answered bare, with no entry named
**Finding:** 71 cast menus, **20 answered "Cast nothing right now" - all 20 with a real cast option
on the same menu** (28%). Several carry no reasoning at all: `146 vs125` seq **29**, **35**, **65**
and **73** are the whole reply, `CHOICE: 5 (Cast nothing right now)`, on menus offering Soul
Shatter / Silverquill Command / Acererak / Pelakka Predation. The guide already says entry 9 "is
the LAST line, not a tiebreaker" - restating it a third time is not the fix; it has no observable.
CLASSIFICATION: **STRATEGY**.
- **before:** `... never because a listed card looks situational.`
- **after:** the same sentence plus **AND YOU MUST SHOW THE WORK** - the reply must name the
  highest entry on the menu and why it did not match; a bare `CHOICE: n (Cast nothing right now)`
  is declared to be the list going unread, and "if you cannot write that sentence, the answer is
  not nothing".

## 146-C (MED) - wave-46's CHECK 0 over-fired: 10 non-lethal upkeep animations
**Finding:** `becomes beholder` was offered **27** times (Upkeep 20, Main-1 6, Blockers 1) and
**taken 12** - against 3 of 46 last corpus. **Ten of the twelve are Upkeep animations at a life
total CHECK 0 does not reach**: `vs125` seq 50/51 (opp 15), 63 (opp 12), 70/71 (opp 5), 77/78
(opp 6); `vs126` seq 56 (opp 5); `vs152` seq 39 (own life **2** vs 19); `vs123` seq 21 (opp 12).
Wave-46 prediction P3 said non-lethal upkeep animations stay at 0-1: **FAIL**. My own wave-46
edit is the cause - CHECK 0 is a licence with no observable, so it fires on vibes.
CLASSIFICATION: **STRATEGY** (my edit over-fired; tightened, not removed - the lethal case it was
built for is real and is now covered by 146-E's DECIDING SITUATIONS row).
- **before:** `... plus 3 for the animated Hive. If that total is at or above their printed life,
  ANIMATE NOW ...`
- **after:** `... plus 3 for the animated Hive. WRITE THAT SUM AND THEIR PRINTED LIFE INTO YOUR
  REPLY. If you do not write the two numbers, CHECK 0 did not fire and you go straight on to
  CHECK 1 - "it might be useful" is not CHECK 0. If that total is ...`

## 146-D (MED) - the `[repeat:]` receipt is on the screen and is being paid twice
**Finding:** the engine's threshold fix landed - the tag now fires from the SECOND offer
(`vs125` seq 51, 71, 78 all read `[repeat: activated this turn 1 times already]`). deck146 was
offered **6** rows carrying the tag and **took 3** of them (`vs125` seq 51, 71, 78 - three
redundant {3}{B} activations, twelve mana). Wave-46 prediction P14 (E-2): **FAIL 3/4**. The
guide's CHECK 2 keys on the battlefield line reading `(3/3)`, which is a two-step read; the tag
is a one-step read and was not named.
- **before:** CHECK 2 ends `If it already reads (3/3), the answer is PASS.`
- **after:** + `AND THE ROW OFTEN SAYS SO ITSELF. An option printed "[repeat: activated this turn
  N times already]" is the game handing you the receipt ... That tag is an unconditional PASS -
  the cheapest observable on the screen, and it cannot be wrong.`

## 146-E (MED) - DECIDING SITUATIONS gets the two new one-line tests
**Finding:** same two above; the deck's quick-reference list is where the model actually looks
under time pressure (it is the section that produced the 43/43 land record).
- **after:** two new rows - `[repeat: ...]` printed -> PASS at any life and in any phase; and an
  Upkeep answer whose reply lacks the two numbers -> PASS.

## 146-F (MED) - the DECIDING SITUATIONS lifegain row mirrors 146-A
**Finding:** `146 vs126` seq 50 again, and seq **59** (life 6 vs 5, same collapsed rows, sent
four, died at -1 with the reply asserting "the game ends before that damage matters" - it did
not; the blocking triggers and the drain resolve on the same combat).
- **before:** `- Two or more blockers on an attacker's tag print "(blocking trigger: they gain N)"
  ... -> that swing PAYS them life.`
- **after:** `- ANY blocker on an attacker's tag prints "(blocking trigger: ...)" or
  "(lifelink: they gain N)" -> that swing PAYS them life ...` plus a new converter row naming the
  collapsed-tag trap.

---

## NOT edited, and why
- **THE WHOLE CAST ORDER** stands unchanged. First-match compliance, scored with entry 1's and
  entry 7's own conditions applied, is **39/67** (wave 46: 46/73). The residual is not a wording
  problem: **13 of the 28 breaks are `Silverquill Command` declined with nothing above it
  matching** (29 offers, 3 casts - wave-46 edit 146-B changed nothing) and **10 are entry 9 taken
  over a live entry**. 146-B (this wave) is the accountability clause aimed at both; a fourth
  restatement of the ordering imperative is not.
- `DO NOT CAST AGADEEM'S AWAKENING`: **1 offer, 0 casts.** Held on its only window.
- Vanishing Verse `the only legal targets are YOUR OWN` -> do not cast: **10/10.**
- Lolth `+0: draw card and lose life`: **16 offers, 1 taken, at printed life 11.** Wave-46
  prediction P4 (0 at life <= 6): **PASS**; the life floor is left exactly as written.
- Dungeon = LOST MINE by name **10/10.** Land drops **28/28.** No edit earned.
