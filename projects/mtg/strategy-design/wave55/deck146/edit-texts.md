# deck146 - wave-55 edit texts (before -> after), each with its finding

Base = the LIVE guide `bin/Res/ai/baka/deck146_strategy.txt` (wave-54 reviewer revision 5160eb1cb
+ boundary pass dd0cd2d74). Size 60,349 -> **64,114 bytes** (band 41-71 KB). Four edits.
Corpus `matchups-20260903-074354`; deck146 3-3 (W v125 20/-1 T12, v126 22/-14 T12, v162 12/-1 T20;
L v152 -5/29 T17, v123 -2/12 T9, v130 0/14 T29). 200 decision records at the six deck146 seats.

---

## 146-A (NEW, HIGH) - the "doesn't untap" attack brake

**Finding.** `146v123` **seq 24** (T9, 21 life vs 16). Both A-lines carried
`[doesn't untap during its controller's untap step]` (their Intruder Alarm, whose full text is
printed on their battlefield line), and the header read
`They have 24 untapped creatures able to block; declaring more than 24 attackers leaves at least
(your attackers - 24) of them unblocked.` The seat answered `ATTACK: A1, A2`, dealt 4, and under
Intruder Alarm those two creatures never untapped again: at `seq 28` the same turn its own line
read `2 are creatures, 0 of them able to attack right now`, both tagged
`[tapped - cannot attack or block this turn]`. The opponent alpha-struck 24 unrestricted creatures
on T10 and the seat died at **-2**. Card fact verified: `mtg.txt:58850` Intruder Alarm
`auto=lord(creature) doesnotuntap`.
Classification: **STRATEGY**. The tag, the count header and the enchantment's own text were all on
the screen; nothing was missing or false.

**Before** (attack section):
```
- TWO NUMBERED CHECKS BEFORE ANY OTHER ATTACK RULE - STOP AT THE FIRST ONE THAT ANSWERS.
  CHECK A - YOUR LAST BODY.
```
**After:**
```
- THREE NUMBERED CHECKS BEFORE ANY OTHER ATTACK RULE - STOP AT THE FIRST ONE THAT ANSWERS.
  CHECK 0 - "doesn't untap". Read the A-line of each creature you are about to send. If it
    carries the tag [doesn't untap during its controller's untap step], declaring it taps it for
    the REST OF THE GAME, not just for their next turn - nothing untaps it on your untap step.
    Their Intruder Alarm is the usual source and prints its own text on their battlefield line.
    Under that tag you declare an attacker ONLY when the power you are sending is at or above
    their printed life THIS turn. Otherwise ATTACK: none and keep the bodies; they are the only
    blockers you will ever have again. WHAT THIS COST: at 21 life against 16, with the attackers
    header reading "They have 24 untapped creatures able to block" and BOTH A-lines carrying the
    tag, this seat sent both, dealt 4, had no untapped creature on their turn and was dead at -2
    one turn later.
  CHECK A - YOUR LAST BODY.
```

---

## 146-B (CARRY, P2 FAIL 2 of 7) - the cleanup-discard rung re-keyed from an ordered prose list
to an executable SCAN

**Finding.** Seven cleanup `discard` windows at deck146 seats (`146v162` s12/s20/s29/s43,
`146v130` s22/s23/s24). Adjudicated against the guide's own printed order:
5 correct, **2 breaks** - `146v130` **s23** sent Agadeem's Awakening while **Emeria's Call was
row 5 of the same list** (rung 1 skipped for rung 2), and `146v162` **s20** sent
`PUT: 7, 1` = a spare Plains plus Acererak `{2}{b}` while two Lolths `{3}{b}{b}` and two
Silverquill Commands `{2}{b}{w}` were left on the list. Wave 54 was also 2 of 7, so the rung is
stated but not executed. Shared shape: **both wrong answers include row 1**; 4 of the 7 answers
begin `PUT: 1`. Positional anchoring, not a rung disagreement - so the edit turns the prose order
into a numbered scan with an explicit "do not answer from the top" instruction and a comparison
step for the tail rung.
Classification: **STRATEGY** (the render prints every cost; `{spare: you control 5 lands already}`
was present and correctly used on the land half of s20).

**Before:**
```
- The ask says "your hand has N cards and your maximum hand size is 7" -> this is the cleanup
  discard, and the FIRST name you send is EMERIA'S CALL whenever it is on the numbered list -
  seven mana is a card you are not casting this turn or next. Then Agadeem's Awakening, then a
  second planeswalker, then a land once your battlefield line already prints four or more lands,
  then the priciest remaining spell.
```
**After:**
```
- The ask says "your hand has N cards and your maximum hand size is 7" -> this is the cleanup
  discard. Run it as a SCAN of the numbered list in this order and STOP at the first name you
  find. Do not answer from the top of the list, and never answer "PUT: 1" because row 1 is first:
    (1) EMERIA'S CALL - send it. Seven mana is a card you are not casting this turn or next.
    (2) AGADEEM'S AWAKENING - send it; it is never cast.
    (3) A SECOND COPY of a planeswalker already on this list or on your battlefield line.
    (4) A LAND, once your battlefield line already prints four or more lands - its row says so
        itself, "{spare: you control N lands already}".
    (5) Otherwise the HIGHEST PRINTED MANA VALUE left on the list. Read the {..} cost off each
        row and compare the numbers; the biggest number goes, whatever position it sits in.
  WHAT THIS BROKE, twice in one corpus and twice in the one before: a PUT line sent Agadeem's
  Awakening while EMERIA'S CALL was still row 5 of the same list, and another sent Acererak
  {2}{b} while two Lolths {3}{b}{b} and two Silverquill Commands {2}{b}{w} were left on it. Both
  wrong answers were row 1.
```

---

## 146-C (NEW) - Soul Shatter takes ONE card; the `{right now:` tie list is not a kill list

**Finding.** `146v123` **seq 24** plan: *"Cast Soul Shatter to sacrifice all 24 Human tokens,
wiping their board"*, restated at **seq 28** after the cast: *"Use Soul Shatter to remove one of
the opponent's 24 Humans"* only after the row itself printed
`{right now: 24 tied at MV 0 (their highest): Human #1; Human #2; ...}`. The seat spent its turn-9
main-2 on a one-for-one against a 24-creature board and died at -2.
Card fact verified against the primitive, `borderline.txt:106068`:
`text=Each opponent sacrifices a creature or planeswalker with the highest mana value among
creatures and planeswalkers they control.` - **one** card. Scryfall was not fetched; the primitive
text and the printed Oracle wording agree.
Classification: **BOTH**. The strategy half is patched here; the render half (the tie clause has no
"they choose ONE of these" quantifier) is **E-2** in the seat file.

**Before** (cast order, entry 1):
```
    (a planeswalker or an enchantment of theirs on that list counts too; Soul Shatter takes the
    highest-mana-value CREATURE OR PLANESWALKER on their line - a planeswalker that outranks
    their creatures is the card it takes - and does nothing on a board of neither). See #1 RULE.
```
**After:** same three lines, followed by:
```
    SOUL SHATTER TAKES EXACTLY ONE CARD. Its row's "{right now: N tied at MV 0 (their highest):
    Human #1; Human #2; ...}" clause is the LIST THEY CHOOSE FROM, not the list that dies: they
    sacrifice ONE of those names and keep the other N-1. So against a token army Soul Shatter is
    a one-for-one at three mana and is NOT a board wipe - it drops off entry 1 of this order the
    moment that clause names more than about three tied cards, because a removal spell that
    removes one of twenty-four is not removal. WHAT THIS COST: two consecutive plans in one game
    read "cast Soul Shatter to sacrifice all 24 Human tokens, wiping their board"; the seat cast
    it into 24 Humans at 21 life and was dead at -2 on the next turn.
```

---

## 146-D (NEW, cross-deck) - the HOLD-priority row

**Finding.** deck146 seats: **68 windows** carried the HOLD row with no declined note (7 takes) and
**12 windows** carried it WITH `[you declined this exact list N times already this turn]` at N>=2
- **0 takes**. Across all three of my decks: 190 such windows, **0 takes**, against 88 takes in 725
comparable windows at the other four decks (12.1%). Every one of the 190 is a re-billed prompt for
an answer the seat had already given. The full text is inserted verbatim in
`deck152/edit-texts.md` (152-C) and `deck162/edit-texts.md` (162-D); deck146 carries the same block
with its own WHAT THIS COST line, placed immediately above `DECIDING SITUATIONS (recognize -> do):`.
Classification: **STRATEGY** (the row and its declined-note trigger both render; the guide had never
mentioned either).

---

## NOT CHANGED, and why

- **P1 (DRAW PRICE)** - one `[DRAW PRICE:` render at deck146 seats this corpus (`146v162`), no
  cast from it. The rule is UNTESTED, not confirmed; left exactly as written.
- **P3 (Pelakka Predation vs a wide board)** - **no Pelakka Predation window at any deck146 seat**
  this corpus. Rule left untouched.
- **The Emeria's Call / Agadeem's Awakening "NOT LANDS FOR YOU" block** - re-confirmed by the
  corpus and left as is: at `146v130` turns 21, 23 and 25 the seat sat on 2-3 Plains holding two
  Emeria's Calls and an Agadeem's Awakening and the engine offered **no land drop at all** on any
  of those turns. The guide's claim is still true; the ENGINE item is E-4 in the seat file.
- **The Silverquill Command modal block** (boundary pass) - 0 Command casts and 0 Command declines
  with a live mode at deck146 seats this corpus. Untouched.
