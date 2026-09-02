# deck146 - wave-52 edits (before -> after), each with its finding

Corpus `matchups-20260901-223430/`, deck146 seats (6 logs, 612 records, 3 fallbacks).
Record 4-2. Every edit below is paid for by a cited seq. Four edits; nothing else changed.

---

## 146-Q - the "would not cover any spell in it" line is a ONE-LAND test
**Finding.** Three of the four mulligans taken at deck146 this corpus were taken on a hand the
guide's STEP-2 ship list does not contain, and all three cited the coverage line:
`146v123` s1 (7 cards, **2 lands** {W}2, "would not cover any spell in it" -> MULLIGAN),
`146v162` s1 (7 cards, **2 lands**, same line -> MULLIGAN),
`146v126` s2 (at (keeping 6), **3 lands**, same line -> MULLIGAN, bare `CHOICE: 2` with no PLAN;
it then kept a 2-lander at (keeping 5) and bottomed Lolth and Nadaar).
STEP 2 names exactly three ships (zero lands / all lands / ONE land not covering); the seat
generalised the one-land coverage test to two and three lands.

**Before** (STEP 2, second sentence):
```
  line "Playing every land in this hand would not cover any spell in it". That coverage line is
  the engine's own arithmetic and it is the whole one-land test: ...
```
**After** - the same sentence, preceded by:
```
  THAT COVERAGE LINE IS A ONE-LAND TEST AND NOTHING ELSE. Read the engine's land count FIRST.
  At TWO OR MORE lands the sentence "would not cover any spell in it" is not a ship and never
  was: two lands cast a two-drop the moment you draw one, and this deck's whole curve is two
  and three. The three ships named above are the WHOLE list - a hand outside them is a KEEP
  however the coverage line reads. WHAT THIS COST, three times in one corpus: [the three above]
```

---

## 146-R - the KEEP line is not a colour gate
**Finding.** `146v123` s1's reply gave its reason as *"Keep a hand with 2-5 lands including at
least one white and one black source"* - a verbatim quote of the guide's own KEEP line - on a
2-land hand making only {W}. The wave-51 cut (146-M) removed the colour clause from STEP 1/2
prose but left it standing in this line, where the model reads it as a test.

**Before**
```
- KEEP a 7 or a 6 with 2-5 lands including a white source and a black source. Shineshadow Snarl
  alone counts as BOTH colors. Brightclimb Pathway is your white source, Hive and Swamp are black.
```
**After**
```
- KEEP a 7 or a 6 with 2-5 lands. Shineshadow Snarl alone counts as BOTH colors; Brightclimb
  Pathway is your white source, Hive and Swamp are black. THIS LINE STATES NO COLOUR
  REQUIREMENT AND IT IS NOT A TEST: a 2-land hand making only white, or only black, is inside
  it. "Keep 2-5 lands including a white source and a black source" is the sentence this seat
  quoted back to itself as its reason to ship a two-land seven; there is no such requirement,
  at any look - see COLOUR IS NEVER A REASON TO SHIP above.
```

---

## 146-S - the empty-blocker-tag SEND absolute (deck146 did not have one)
**Finding.** deck146 met 39 A-lines printed `[no creature they control can block this attacker]`
and sent 38. The one miss is `146v125` s107 (T23, 22/17): `ATTACK: none` with A1 Silverquill
Silencer (3/2) and that line as its only tag - three free damage declined in a game that later
went to 52 turns. deck162's guide carries this absolute; deck146's did not.

**Before**: `COMBAT - ATTACKING TAPS YOUR BLOCKERS...` opened straight into CHECK A/CHECK B.
**After**: the same section, prefaced by a new paragraph -
```
NO "their untapped blockers" TAG AT ALL ON AN A-LINE MEANS NOTHING THEY CONTROL CAN BLOCK IT.
The engine prints "[no creature they control can block this attacker]" in its place. That is
free, unopposed damage: SEND it, at every life total, ahead of every rule below except CHECK A.
Nothing on the screen prices such an attacker, so no tag rule, no wall rule and no converter
line reaches it - the clauses those rules read are exactly the clauses this line does not have.
... WHAT THIS COST: at 22 life against 17 this seat answered ATTACK: none with a Silverquill
Silencer whose only tag was that line.
```

---

## 146-T - the show-the-work sentence, narrowed to menus where it decides something
**Finding.** The wave-51 absolute ("your reply names the highest entry that was on this menu...")
was met in **20 of 84** own-turn `Cast nothing right now` answers (24%). It fired correctly in
exactly the windows where a live cast row existed (`146v125` s138/s140 both produced the full
"Entry 1 (Soul Shatter) ... did not match because ..." form). In the other ~63 windows the menu
was two rows - "Cast nothing" plus a row the engine had already marked dead ("at 0 this does
nothing", "the only legal targets are YOUR OWN right now") - and the ceremony had nothing to
decide. Separately, the cast order itself did NOT break: of 21 own-main `Cast nothing` answers
with a live cast row, 10 were Pelakka Predation at "Opponent hand size: 0" (entry 7 requires 3+),
8 were Agadeem's Awakening (never cast), 2 Silverquill Command with none of its three conditions
true, 1 a `[legendary:` second Nadaar. Zero misroutes.

**Before**: `AND YOU MUST SHOW THE WORK: when you answer "Cast nothing right now", your reply
names...` + `A bare "CHOICE: n (Cast nothing right now)" with no such sentence is the same.`
**After**: `SHOW THE WORK ON THE MENUS WHERE IT DECIDES ANYTHING: when you answer "Cast nothing
right now" AND the menu holds at least one other "Cast ..." row, your reply names...` and, in
place of the bare-CHOICE sentence: `When "Cast nothing right now" is the ONLY other row, or every
other row is one the engine has already marked dead (...), answer with the bare CHOICE line and
no sentence: there is nothing on that menu to show.`

---

## Considered and NOT made
- **Soul Shatter cast three times into one Emrakul** (`146v125` s327/s328/s329). NOT a guide edit:
  the row printed `{right now: they sacrifice Emrakul, the Aeons Torn (MV 15, their highest)}`
  and the opponent's battlefield still listed Emrakul after each resolution. The pilot read the
  screen correctly every time. Routed to the seat file as engine item E-1 (HIGH).
- **Lolth's 0 floor**: 12 `+0: draw card and lose life` rows offered, 1 taken, at 21 life. The
  wave-51 break is gone; no restatement added.
- **Kaya self-target**: 15 rows printed `{this hits YOUR permanent}` at deck146, 0 taken. Rule
  holds; nothing added.
- **`146v152` s15** (Goblin token chumped a 5/4 Tracker at 20 life, rung 3 above the floor of 10).
  One window, a free token, no life cost - not worth a line.
