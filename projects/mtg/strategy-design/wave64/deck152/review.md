# Wave-64 per-deck review — deck 152 (Bant Human midrange)

Corpus `matchups-20260906-001533`. Seat files are named
`...-ai_baka_deck152-<ptr>-vs-ai_baka_deck<N>.jsonl`; below I cite them as `152v<N>`.
Record: **6-0**, 171 decisions, **0 fallbacks**, 0 re-asks, 0 hung/crashed games.

## 1. Game by game

| game | file | result | deciding decisions |
|---|---|---|---|
| 152v146 | 1788671740-...deck152-0x55e72bfb52c0-vs-...deck146 | WIN, T12, 32 to -5 | s10/s11 double-spell turn (Katilda, then Adversary #2 paid by tapping Adversary #1 through Katilda — the cast row warned `{paying this taps: Intrepid Adversary - it cannot att...}`, and the seat correctly accepted losing that attack); s13/s18/s27 declined three chump blocks vs a first-strike deathtoucher and took 3 total; s35 all-in for lethal through two Spiders |
| 152v123 | 1788671744-...deck152-0x5641f0936fc0-vs-...deck123 | WIN, T9, 25 to -8 | s10 Spellbinder on curve, s11 exiled Tribute to Hunger (their only removal in hand); s17 paid 1 valor counter with exactly 2 spendable — the only window this wave where a counter was affordable; s19/s22 lethal-shaped attacks |
| 152v162 | 1788671750-...deck152-0x561491501fe0-vs-...deck162 | WIN, T11, 35 to -4 | s17 Teferi on curve into s18 `-2` dig; **s21/s22 Fateful Absence on Ob Nixilis, the Hate-Twisted** off the `{kills whichever you target: THEIRS - ...}` row — the wave-63 loss to this seat was Ob Nixilis dealing 6 unanswered; s26 lethal |
| 152v125 | 1788671752-...deck152-0x56122c4aac80-vs-...deck125 | WIN, T17, 20 to -3 | T11 Elite Spellbinder countered by Fall of the Gavel, T12 Final Judgment exiled the whole board (4 permanents); s26/s29 rebuilt from zero creatures at 7 opp life and closed in 4 turns without ever falling below 20 life. The slow finish is opponent removal, not guide effect |
| 152v130 | 1788671760-...deck152-0x5582116ffe80-vs-...deck130 | WIN, T13, 18 to -5 | s9 Ranger Class, s14 level 2, then a single Wolf token carried the game (2/2 -> 6/6, s15/s19/s23/s25); s18 Cathar, s21/s22 Spellbinder took Lay Waste |
| 152v126 | 1788671764-...deck152-0x55ee4aa93df0-vs-...deck126 | WIN, T11, 20 to -8 | s9/s10 Spellbinder offered Staff of Nin vs **Exquisite Blood** and took the Blood — the exact choice the guide's `WHAT THIS COST` paragraph (deck152_strategy.txt:424-435) was written for; s13 Tovolar's Huntmaster; s21/s29 two all-in swings |

No game in this set was lost or nearly lost, and no decision in it was a fallback. Hands were
real: 152v125 kept a slow hand and got wrathed; that is variance, not instruction.

## 2. Engine / interface / card items

### HIGH-1 — the blockers header's "best case" assignment is computed lone-block-only, and then claims no better one exists
`152v146` **seq 13**, rendered verbatim:

```
INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at 19 ... best case with every
blocker assigned: you would be at 20 - one legal assignment that reaches it, chosen for the LIFE ONLY -
it does not preserve your material and no better material reaches that life figure:
Katilda, Dawnhart Prime blocks Triumphant Adventurer; 1 of those blockers die - taking it SPENDS Katilda
A1. Triumphant Adventurer (1/1) deals 1 [first strike, deathtouch] [GANG BLOCK: any 2 of yours together
    deal 1 past everything its own first strike can kill, enough to kill this attacker; ...]
B1. Katilda, Dawnhart Prime (1/1) - may block A1 (your blocker dies, attacker lives)
B2. Intrepid Adversary #2 (3/1) [lifelink] - may block A1 (your blocker dies, attacker lives)
```

The next line advertises a gang block that reaches **the same life figure (20)**, spends **the same one
blocker**, and additionally **kills the attacker**. So `no better material reaches that life figure` is
false: the header's search enumerates lone blocks only, while the F7 gang-block clause is computed by a
separate, gang-aware pass. Same defect, same wave, at **`152v146` seq 27** (Brutal Cathar 3/3 + Wolf 2/2
vs the same 1/1 first-strike deathtoucher; header suggests Cathar chumps alone at 29 life, the gang kills
it for the same price). That is **2 of 2** windows in this seat where a gang block existed.

Corpus-wide check I ran while here: of 10 header suggestion lines in the whole run, **0** ever name two
blockers on one attacker; 2 of those 10 sit in prompts that print a `GANG BLOCK:` clause — both are the
deck152 windows above. Cost in these six games: **zero** — this seat declined both times and was right to.
Ranked HIGH anyway because the sentence is a false claim about the search space, and a pool seat is on
record copying this suggestion verbatim (known-bugs F8, deck123 3/3). Fix shape: make the header's
assignment search gang-aware, or drop the `no better material` clause whenever a `GANG BLOCK:` clause is
printed for any attacker in the same window.

### MED-1 — the repeat-payment window prints 20 rows that the prompt itself says are all no-ops
`152v146` **seq 7** (and seq 12; `152v162` seq 7 and seq 11): Intrepid Adversary's valor-counter ask
renders `1. don't add any counter` plus `2.`-`21.` `add N counters`, each annotated
`{repeat cost: N x {1}{w} = 2N mana for all N; you have 0 spendable now, which pays for 0 of them and stops}`,
under a header that already states `With no spendable mana left, every option adds 0 counters`.
**4 of the 5** such windows this seat saw had **zero** affordable counters, and the fifth (`152v123` seq 17,
2 spendable) had exactly one. Two fixes, either sufficient: clamp the row list to
`floor(spendable / per-counter cost)` and print the ceiling (the F1 treatment), and/or skip the ask
entirely when 0 counters are payable — that alone removes 2 of the 36 decisions in `152v146` (5.5%) with
no loss of agency, since every row is provably identical.

### MED-2 — the Elite Spellbinder target window renders the ability by bare name, with no tax rider and no decline row
`152v126` **seq 10**, rendered verbatim:
`TARGET CHOICE for Elite Spellbinder - its "exile a non-land card" ability ...` followed by two bare card
rows and `(this ask has no pass row)`. Two gaps against the Oracle text (verified: Scryfall and
`bin/Res/sets/primitives/borderline.txt:33384` agree — *"You **may** exile a nonland card from it. For as
long as that card remains exiled, its owner may play it. A spell cast this way costs 2 more to cast."*):
- the window never says the exile is a **tax, not removal** — and the model's own replies show it reading
  the exile as removal: `152v123` seq 11 `PLAN: Exile Tribute to Hunger ... to deny their removal`.
  The engine does implement the rider (`newability[canplayfromexile forever]`,
  `newability[changecost(colorless:2) forcedalive]`), so the surface is behind the engine, not the reverse.
  This is the F6 class (bare `name(...)` label) on a target window rather than a modal row.
- the script's `auto=if type(*[-land]|opponenthand)~morethan~0 then name(Exile a non-land card) target(...)`
  has no `may`, so the seat cannot decline; the Oracle says it may. Low practical cost (declining is
  almost never right), but it is a fidelity gap in the card, not the engine.
Also at `152v130` seq 22, `152v146` seq 32.

### LOW-1 — the planeswalker-attack section never states that a planeswalker cannot block
`152v146` **seq 35** prints the new W rows correctly (`W1. Lolth, Spider Queen [planeswalker] [1 loyalty
left...]`, `A#>W#` grammar, `They have 2 untapped creatures able to block`), and then:
`Blocking works the same either way: they may block an attacker whichever it is aimed at.`
The seat's reply reads `... the surplus attackers guarantee damage through even if Lolth blocks two.
This kills Lolth` — while sending no attacker at W1. It read the walker as a blocker and as a casualty of
combat damage it never assigned. Free of cost here (the swing was lethal), but the sentence above is the
one line in the section a reader can turn into "the walker blocks". A clause such as *"a planeswalker
never blocks; only their creatures do"* closes it.

### LOW-2 — `{text: ...}` is dropped from some rows of a reveal list whose primitive has a `text=` field
`152v125` **seq 22**: rows 1-2 render `Branchloft Pathway (land) {text: {T}: Add {G}. // Boulderloft Pathway}`
but row 5 renders `5. Hengegate Pathway (land) [does NOT qualify - goes to "put on bottom"]` — no `{text:}`,
though `borderline.txt:52393` has one. Not position-determined: in `146v126` seq 11 `Isolated Chapel` copy 1
carries its text and copy 2 does not, while in `123v152` seq 7 both copies carry it. Lands the seat was
going to bottom anyway, hence LOW, but it is a renderer inconsistency that would matter on a spell.

### Fixes that held (falsifiable predictions this seat can adjudicate)
- **F3 (this seat's wave-63 HIGH-1) is fixed.** A pathway FRONT face was played in main 1 five times
  (`152v123` s15, `152v130` s6, `152v125` s7, `152v126` s6, `152v146` s9) and **5 of 5** were followed by a
  further same-turn main-1 cast (s16 Intrepid, s7 Katilda, s8 Intrepid, s7 Katilda, s10 Katilda). Wave-63
  measured 0 of 15.
- **F4 is fixed**: `152v146` seq 35 offered `W1.` and the `A#>W#` grammar (see LOW-1 for the residual).
- **F2 is fixed for this seat**: `this ask has no pass row` appears on 29 windows, and on **0** of them do
  `Cast nothing` / `Hold priority` rows also appear.
- **F7 gang-block strings are live and correct**: `152v146` s13 `deal 1 past everything its own first
  strike can kill` (worst-case survivor is Katilda's 1 after the 3/1 is deathtouched) and s27 `deal 2`
  (survivor is the 2/2 Wolf). Both conservative and both right.
- **F8/F11**: the blockers suggestion is now labelled truthfully (`chosen for the LIFE ONLY`, `SPENDS
  <card>`, `This assignment is an OPTION, not an instruction`) — the residual is HIGH-1, the search itself.
  `CRACK-BACK NEXT TURN` at `152v146` s35 (`3 of their creatures ... for up to 5 - you would be at 23`)
  matches the board exactly (1+2+2), and folds the exiled Silverquill Silencer as `1 more of theirs
  (3 power) comes back from exile`.
- No-op decision rate for this seat: **21 of 171 (12.3%)**, against the 53% deck125 measured in wave 63.

## 3. Guide verdict: **KEEP** (deck152_strategy.txt, 68,040 bytes = 66.4 KB, inside the 41-71 KB band)

6-0 with 0 fallbacks, and the guide's two most expensive lessons were both executed under live pressure:
- `152v126` s10 took **Exquisite Blood** over Staff of Nin — the guide's ELITE SPELLBINDER paragraph
  (lines 424-435) exists because two earlier corpora took the Staff/Sorin and lost to the Bond-Blood loop.
- `152v162` s21/s22 spent Fateful Absence on **Ob Nixilis** — the planeswalker that killed this seat in
  wave 63 (known-bugs F4).
- The mulligan rule (RULE -1) held: six keeps, zero mulligan chains, no game lost to a hand.
- `152v146` s10/s11 shows the Katilda-taps-Humans cost being read off the cast row and accepted knowingly;
  `152v125` s26/s29 shows the post-wrath rebuild the guide asks for.
No decision in these six games was a STRATEGY misplay against the guide, so nothing here pays for an edit,
and the file is near the top of the size band — adding unpaid text would only crowd the rules that worked.
No `strategy.txt` written.

## 4. Proposals

None. No evidence in these six games supports a general-guide or skill amendment; every item above is an
engine/render/card fix, not an instruction change.

## 5. What I did NOT check

- I did not read the opponents' seat files end to end — only `146v152` seq 23 and the duplicate-text rows
  quoted in LOW-2, plus the corpus-wide grep over `blockers` prompts used in HIGH-1.
- I did not open `src/` to locate the code behind HIGH-1, MED-1 or LOW-1 (the brief forbids editing it and
  I did not read it either), so the fix shapes I name are behavioural, not line-level.
- I did not verify the F5/F6/F9/F10/F12/F13/F14 predictions: no window in this seat's six games rendered a
  stack-trap decline sentence, a modal `choice` row, a `KILLS you` header, a draw-step forecast, a granted
  ability, or a latch record. Those are UNTESTED from here, not passing.
- I did not adjudicate the two lane-flagged engine questions (`flyersonly` planeswalker attacks; AK R5 vs
  wave-63 E6b) — no window in this seat bears on either.
- Card facts verified against primitives for Elite Spellbinder, Hengegate/Mistgate, Branchloft/Boulderloft,
  Barkchannel/Tidechannel, Isolated Chapel and Scrubland only; Scryfall cross-checked for Hengegate
  Pathway (primitive and Scryfall AGREE: front face taps for {W}) and Elite Spellbinder. Every other card
  in the six games I took from the rendered prompt.
- Latency, `async_drops`, `transport` and timeout accounting are the engine seat's; I read only `fallback`.
