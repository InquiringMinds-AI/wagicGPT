# Wave-46 reviewer boundary pass (2026-08-26)

Lane: DECK REVIEWER. Inputs: `synthesis-notes.md` "Guide boundary findings" C1-C5,
`strategy-writing-skill.md` amendments 1-87 (68-87 read in full), `seats/*.md` for the corpus
evidence, and the live guides `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (verified
byte-identical to each `wave46/deckN/strategy.txt` before editing). Card facts re-verified with
`/usr/bin/grep` against `projects/mtg/bin/Res/sets/primitives/*.txt`. No `src/`, no commit, no
wagic run. Guides remain ASCII (`git diff | grep -c U+FFFD` = 0).

Standing rule applied throughout: **every line added must be paid for by a cited seq.** Two of the
five findings are flagged PREVENTATIVE by synthesis itself; both are accepted only at the seat
where the corpus demonstrates the failure and rejected elsewhere.

---

## Verdicts

### C1 - hoist RULE -1 to the top of the file (amendment 69)
**ACCEPT for deck152. REJECT for deck125, deck126, deck130, deck162.**

- **deck152 ACCEPT - paid.** `seat-146-152-162.md` L34/L59-61 and P10: `152 vs162` seq 3 is a
  **STEP-1 HARD FLOOR VIOLATION** - price line `(keeping 5)`, a land in hand, mulliganed anyway,
  cascading a three-mulligan chain down to a four-card hand. That is the deck123 mechanism at a
  live seat, and deck152's correct STEP 1 text sits at line 323 of 419 (77% of the file), the
  deepest mulligan section in the pool. Text right, placement wrong - amendment 69 exactly.
- **REJECT deck130** - `seat-123-130.md` L13, L202: 6 games, 6 straight keeps, **PASS 6/6**
  including two one-land sevens (wave-45 B7 holding). Nothing to pay for the bytes.
- **REJECT deck125, deck126** - `seat-125-126.md` L27-36: one mulligan each, **both keeps
  guide-legal**, no chain. deck126 additionally already carries the floor in DECIDING SITUATIONS
  (line 321), i.e. in its nearest-the-decision clause, which is #24's placement.
- **REJECT deck162** - `seat-146-152-162.md` L43: **zero mulligans, section entirely UNTESTED
  (0 windows).** Hoisting an untested section to the top of the file on no evidence is growing a
  guide for its own sake; synthesis itself records deck162's stop as a correct alternate key.

### C2 - the carried-plan clause (amendment 69, second half)
**ACCEPT for deck152 (appended to the hoisted RULE -1). REJECT for deck125, deck126, deck130, deck162.**
The trap only exists once a chain starts. deck152 is the only one of the five that ran one
(`152 vs162`, three consecutive looks, `seat-146-152-162.md` L34/L59-61). deck125/126 took a
single mulligan each and deck130/162 took none, so at those four seats the ask never repeated and
the clause has no window it could have changed. Re-file if any of them produces a chain.

### C3 - deck126's body-count surplus line has no defender-allocation statement (amendments 54, 29, 51)
**ACCEPT (deck126), with the suggested wording trimmed.**
Paid: the surplus line is a fresh wave-46 edit (F4, licensing `vs130` seq 40 where four Vampires
into one Siege-Gang won the game), and `seat-146-152-162.md` records deck146's wave-46 break as
this exact falsehood in a new shape, costing a card. Sweep confirmed **0 defender-allocation
statements anywhere in deck126** vs 3 in deck146, 1 each in deck152/deck123 - a blank cell under
amendment 81. **Trimmed:** synthesis's "send when the surplus is worth more than the biggest single
Vampire you can lose" is a DERIVED comparison and a soft one, which amendment 72(a) forbids and
which would bench the attack that D25 pre-registers as `>= 1/1`. Shipped instead: the principle in
amendment 77's generalising form, an explicit "send them anyway", and the attackers-do-not-block
corollary. The rule's behaviour is unchanged; only the false belief behind it is refuted.

### C4 - the "(neither dies)" outnumber clause with its refutation 50-70 lines below (amendment 70)
**ACCEPT (deck146 and deck152).**
Paid by amendment 70's own three-seat, three-loss evidence for the geometry (deck126 S5, deck162
S3, deck146 S2), and it is an ordering fix, not new doctrine: 177 bytes at deck146. The
refutation below is NOT deleted (it governs the stop-tag section, a different context, and #35
forbids removing the only stop); the principle is stated once above the count and the count is
made conditional on it. The one-name corollary and the Shield Sphere exception stay put.

### C5 - one ordered first-match cast list (amendments 62, 76)
**ACCEPT for deck126 as a totality close only - the premise is wrong. REJECT for deck123, deck152, deck125.**

- **Premise correction:** deck126 **already ships one ordered first-match list** - RULE #7, "Take
  the highest entry on this list that appears in your casting menu", numbered 1-7 with 7a/7b.
  Wave 45's B13 was satisfied; synthesis's "0 ordering markers" sweep missed it. Writing a second
  list would VIOLATE #62. What RULE #7 actually lacked is amendment 76's totality close: Tribute
  to Hunger was reachable only through the 7b survival floor, and `Cast nothing right now` was a
  condition ("only when it is the ONLY option left") rather than the last entry.
- **Shipped:** entry 8 (Tribute to Hunger, when nothing above matched), entry 9
  (`Cast nothing right now`, last), and the `EVERY CARD YOU CAN CAST HAS A LINE ABOVE` roll-call
  naming all eleven castable names. Paid by deck146's 0/1 -> 7/8 headline metric on exactly this
  shape and deck162's 20/21 single numbered list.
- **REJECT deck123, deck152, deck125** - synthesis reports these at lower confidence itself; none
  produced an adjudicable cast-order misplay this corpus, their casting rules are already keyed to
  option rows, and deck152's 50/50 deploy floor is its equivalent. Unpaid bytes.

---

## Edits, by guide

| guide | edits | bytes before | after | delta |
|---|---|---|---|---|
| `deck126_strategy.txt` | C3 (defender-allocation principle appended to the DECIDING SITUATIONS surplus line, trimmed wording) + C5 (RULE #7 entries 8 and 9 plus the totality roll-call) | 26,595 | 27,533 | **+938** |
| `deck146_strategy.txt` | C4 (defender-allocation principle above the "(neither dies)" count; count made conditional) | 38,387 | 38,564 | **+177** |
| `deck152_strategy.txt` | C1 + C2 (RULE -1 hoisted to line 1, above the deck thesis, with the carried-plan clause and the cost record) + C4 | 34,157 | 35,941 | **+1,784** |
| `deck123`, `deck125`, `deck130`, `deck162` | none | - | - | 0 |

Pool total: **+2,899 bytes** across three of seven guides. Four guides untouched.

## Card facts verified for this pass
`Tribute to Hunger` - `mana={2}{B}`, `type=Instant`, `target=opponent`,
`auto=ability$!name(sacrifice) notaTarget(creature|mybattlefield) ... !$ targetedplayer`: it
targets a PLAYER and they sacrifice a creature of their choice, so the new entry 8's "on an empty
opposing board it does nothing" is exact and the edict framing is unchanged.
`Perimeter Captain` - `{W}`, `abilities=defender`, `@combat(blocking) source(creature[defender]|
mybattlefield):may life:2 controller`: the four-wall roll-call in the C5 close names it correctly
as a wall. No other new card claim was introduced by any edit.

## For wave 47
- **P-C1/C2 (deck152):** 0 mulligan answers at `(keeping 5)` or lower with a land in hand.
  Falsified by any one. Wave 46 baseline: 1 of 18 pool openers, all of it at this seat.
- **P-C3 (deck126):** D25 unchanged (`ATTACK: none` on an untagged A-line = 0; all Vampires attack
  when they outnumber the tag's N `>= 1/1`) AND zero replies asserting an attacker of theirs will
  be blocked by a chosen attacker of ours. If D25 regresses, the C3 clause over-fired - cut it.
- **P-C4 (deck146, deck152):** zero "(neither dies)" swings justified in the reply by a raw
  attacker-count surplus.
- **P-C5 (deck126):** zero own-turn `Cast ...` takes of a card outside RULE #7's entries, and
  `Cast nothing right now` taken only where no entry 1-8 was on the menu.
- **Not re-filed:** C1/C2 at deck125, deck126, deck130, deck162 and C5 at deck123, deck152,
  deck125 are REJECTED ON EVIDENCE, not deferred. Re-file only with a cited seq.
