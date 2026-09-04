# wave-57 deck126 guide edits - before -> after, each with its finding

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` (65,461 bytes).
Result: `wave57/deck126/strategy.txt`, **70,111 bytes** (band 41-71 KB - measured, in band).

**deck126 REMAINS THE ZERO-HOLD-TEXT CONTROL** (skill #233 / #299 sunset). The string "Hold
priority" appears in this guide ZERO times, the declined-note zero times, and no instruction about
the hold row of any kind is added by these edits. Third corpus of the control; see the seat file.

## 1. D57-1 the RE-ASK sentence is the answer; never repeat the refused name

FINDING D-F1 (PERCEPTION-adjacent STRATEGY; part of the vs146 loss). 3 `[RE-ASK]` notices this corpus, all at deck126, all reading `[RE-ASK] "Cast Sanguine Bond" is not on this list. Answer with a number from 1 to 3.` (`126v130` seq 10, `126v146` seq 31, `126v162` seq 13). D3 adjudicates PASS on the emitter - the notice names the row it refused and quotes the offending name - and 2 of 3 recovered to a real row. The third re-sent the identical "CHOICE: 1 (Cast Sanguine Bond)"; the NUMBER executed and row 1 was a dead Idyllic Tutor carrying "[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card - you would be at 7]". All three re-asks name Sanguine Bond, and in each the seat's echoed PLAN line said to cast it - the plan echo is being answered as if it were the menu.

BEFORE:
```
Three times one corpus you answered "CHOICE: 1 (Cast Sanguine Bond)" or "(Cast Exquisite
Blood)" to a menu whose row 1 was a Battlement, a Lantern or a Tribute at 0, and the fallback
player cast for you; once the next corpus ("Cast Exquisite Blood" at 3 mana with {4}{b} in hand),
and the engine asked again. When the window prints "[RE-ASK] ... is not on this list", the name
you wrote is a hand card: answer the printed rows.
```
AFTER:
```
Three times one corpus you answered "CHOICE: 1 (Cast Sanguine Bond)" or "(Cast Exquisite
Blood)" to a menu whose row 1 was a Battlement, a Lantern or a Tribute at 0, and the fallback
player cast for you; once the next corpus ("Cast Exquisite Blood" at 3 mana with {4}{b} in hand),
and the engine asked again.
WHEN THE WINDOW PRINTS "[RE-ASK] "Cast Sanguine Bond" is not on this list. Answer with a number
from 1 to N", THAT SENTENCE IS TRUE AND IT IS THE WHOLE ANSWER. The card you named is on your
HAND line, not in this menu; you cannot cast it in this window at any price. DO NOT WRITE THE SAME
NAME AGAIN. Read the numbered rows, pick one of THOSE, and copy ITS name into the parentheses.
Three re-asks this corpus, all three naming Sanguine Bond. Twice you read the sentence and moved
to a real row - `Cast Overgrown Battlement`, `Cast nothing right now` - and both were right. The
third time you re-sent "CHOICE: 1 (Cast Sanguine Bond)" to a menu whose row 1 was a dead Idyllic
Tutor carrying a "[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws
them a card]" price; the NUMBER executed, you paid 3 life and a card for a search that could find
nothing, and you were dead two turns later at -1 while holding BOTH enchantments in hand.
A RE-ASK IS ALSO A FACT ABOUT YOUR PLAN. If your PLAN line says "cast Sanguine Bond" and the
engine has just told you no such row exists, the plan is stale: rewrite it around what the menu
actually offers, or you will answer the next window the same way.
```

## 2. D57-2 one CHOICE line - a retraction is a refusal, and a refusal costs the window

FINDING D-F2 (reply discipline; the last window of the vs146 loss). `126v146` seq 34, turn 20, 2 life, 7 mana: the reply opened "CHOICE: 3 (Cast Staff of Nin)" and then ran ~900 words that correctly worked out that row 1 (Cast Exquisite Blood) was the play - and closed "So CHOICE: 1". The engine stamped `retracted_choice`, `choice: -1`, and a `recovery` record followed; the seat died that turn. This is the corpus's only `retracted_choice`. The engine seat owns the resolution policy (seat file MED #5); the guide half is that the deciding happens before line 1 is written.

BEFORE:
```
AND A CARD IN YOUR HAND WITH NO "Cast" ROW IS NOT CASTABLE THIS WINDOW.
```
AFTER:
```
ONE "CHOICE:" LINE PER REPLY, AND THE FIRST WORD YOU WRITE IS THE LAST WORD YOU GET. Do the
deciding BEFORE you write line 1. A reply that writes "CHOICE: 3" and then argues its way to a
different row further down is REFUSED by the engine - it cannot tell which of your two answers is
real - and a refused reply spends the whole window. That happened once this corpus and it was your
LAST live window of the game: at 2 life on turn 20, with "Cast Exquisite Blood" at row 1 and 7
mana, you wrote "CHOICE: 3 (Cast Staff of Nin)" and then nine hundred words that correctly reached
row 1. The reasoning was RIGHT and the game was lost anyway, because the answer the engine could
read was neither. If your first line turns out to be wrong, you find that out at the next window -
and the next window arrives.
AND A CARD IN YOUR HAND WITH NO "Cast" ROW IS NOT CASTABLE THIS WINDOW.
```

## 3. D57-3 a lethal NAMED-BY price outranks entry 1, and the substitute is named

FINDING D-F3 (STRATEGY, same game). `126v146` seq 34 row 2: "Cast Sanguine Bond {3}{b}{b} [NAMED BY THEIR Silverquill Silencer #2: casting this costs you 3 life and draws them a card - you would be at -1; this KILLS you...]". The guide's entry 1 said take Sanguine Bond when both names are missing and both rows are printed; the row said taking it kills you. The model resolved the collision correctly in prose and then lost the window to the retraction. The rule now carries its own exception and names the substitute, so the collision does not need resolving at the window. 13 `[NAMED BY THEIR` rows across my two seats, 2 taken.

BEFORE:
```
    BOTH NAMES MISSING AND BOTH ROWS PRINTED ON THE SAME MENU: TAKE SANGUINE BOND.
```
AFTER:
```
    THE ONE PRICE THAT OUTRANKS THIS ENTRY, and the row prints it finished: "[NAMED BY THEIR
    Silverquill Silencer #2: casting this costs you 3 life and draws them a card - you would be at
    -1; this KILLS you]". When an entry-1 row ends in "this KILLS you", it is NOT your answer at
    any life total - take the OTHER half if its row carries no such price (Exquisite Blood is not
    named when Sanguine Bond is), and otherwise go down the list. This is the only thing that
    turns entry 1 off, and it is a printed string, not a judgement.
    BOTH NAMES MISSING AND BOTH ROWS PRINTED ON THE SAME MENU: TAKE SANGUINE BOND.
```

## 4. D57-4 unblockable incoming damage lifts the edict above the walls

FINDING D-F4 (STRATEGY, the vs152 loss). `126v152`: 11 live Tribute rows over the game, 6 declined at 17, 9 and 1 life while an Elite Spellbinder (4/2 flying) and a Sigarda, Champion of Light (6/6 trample, flying) attacked - seq 5 (t6, 17 life, cast nothing), seq 7 and 8 (t8, 9 life, cast Perimeter Captain then Overgrown Battlement), seq 11 (t10, 1 life, cast Wall of Omens). The seat then cast both Tributes at 1 life and died at -1 on turn 11. Every wall it cast in that window was printed "NONE of your available blockers can block this attacker - it has flying" against the two creatures carrying 10 of the 13 damage. Rule #4 correctly ranks the "they choose which one" edict low on value; the carve-out is narrow (unblockable majority + life <= 12) and keyed to a printed parenthesis.

BEFORE:
```
 - THE ROW NAMES A VICTIM TAGGED [defender] (Shield Sphere, Fog Bank, a wall): it was never going
```
AFTER:
```
 - THE DAMAGE COMING AT YOU IS UNBLOCKABLE, AND THE COMBAT HEADER SAYS SO IN ITS OWN WORDS:
   "INCOMING THIS COMBAT: 3 attackers, 13 unblocked damage - you would be at -8; this KILLS you
   (of that, 10 from 2 attackers none of your creatures can block)". Your walls cannot touch a
   flier. While that parenthesis accounts for MOST of the damage and your life is 12 or less, a
   live Tribute row - including the "they control N creatures - they choose which one" form - is
   worth more than another wall, because it is the only card in this deck that can remove a
   creature your blockers can never reach. Move it above entry 4 (the walls) for as long as that
   parenthesis is printed. At 9 life against an Elite Spellbinder (4/2 flying) and a Sigarda (6/6
   trample, flying) you cast a Perimeter Captain and then an Overgrown Battlement off menus whose
   row 2 was a live Tribute, went to 1 life, cast both Tributes there, and lost at -1: two walls
   that could not block either flier were the two cards you spent on the way down.
 - THE ROW NAMES A VICTIM TAGGED [defender] (Shield Sphere, Fog Bank, a wall): it was never going
```

## 5. D57-5 entry-1 ceiling number

(see seat file)

BEFORE:
```
    not this entry; skip it. Nine of nine one corpus and nine of nine this one - the turn the row
    appeared, it was cast.
```
AFTER:
```
    not this entry; skip it. Nine of nine one corpus and nine of nine the next - the turn the row
    appeared, it was cast. This corpus the entry was reached six times and taken six times.
```

## 6. D57-5 entry 1 FAILED 4 of 11 and three breaks are the vs146 loss - the evidence replaces the ceiling claim

FINDING D-F5 (STRATEGY, the vs146 loss). 11 windows printed a "Cast Sanguine Bond" or "Cast Exquisite Blood" row without a `[second copy:` tag for a name absent from the battlefield line; 7 taken (`126v125` seqs 93/345/374, `126v130` seqs 26/28, `126v162` seqs 15/17). The four not taken: `126v146` seq 24 (t16, 13 life, Blood at "taps you out" -> Overgrown Battlement), seq 28 (t18, 10 life, Blood at "leaves 1" -> Chromatic Lantern off SIX sources, outside the Lantern carve-out's own <=4-source shape), seq 34 (the retracted reply above), and `126v130` seq 23 (both halves at "taps you out" -> Sorin; pair two turns late, game won anyway). D105 was 9/9 in each of the last two corpora; this is a real regression at a bigger denominator and it is the loss.

BEFORE:
```
    not this entry; skip it. Nine of nine one corpus and nine of nine the next - the turn the row
    appeared, it was cast. This corpus the entry was reached six times and taken six times.
```
AFTER:
```
    not this entry; skip it.
    THIS ENTRY BROKE THIS CORPUS AND THE BREAK IS A WHOLE GAME. Eleven windows printed an entry-1
    row for a name missing from your battlefield line; you took seven. Three of the four you did
    not take are one game and they are three consecutive casts:
      - turn 16 at 13 life, row 1 "Cast Exquisite Blood {4}{b} {... casting this taps you out}":
        you cast an Overgrown Battlement (entry 4) instead;
      - turn 18 at 10 life, row 1 "Cast Exquisite Blood {4}{b} {leaves 1 of your 6 untapped mana
        sources untapped}": you cast a Chromatic Lantern (entry 7) instead, off SIX sources, which
        is not even the Lantern's own carve-out shape;
      - turn 18 again, a dead Idyllic Tutor with a 3-life price on it.
    You were dead at -1 on turn 20 holding both enchantments. NOTHING BELOW ENTRY 1 IS AN ANSWER
    WHILE AN ENTRY-1 ROW IS PRINTED - not a wall, not a Lantern, not a Tutor, not "leaves N", not
    a life total, not a plan you wrote two windows ago. Read the menu for the two names FIRST,
    every window, before you read anything else on it.
    The fourth was a menu printing BOTH halves at "taps you out" on turn 9 at 20 life; you cast
    Sorin instead and the pair landed two turns late in a game you won anyway.
```
