import sys
src='/home/magi/Projects/wagicGPT/projects/mtg/projects_placeholder'
src='/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/ai/baka/deck126_strategy.txt'
t=open(src,'rb').read().decode('ascii')
orig=len(t)

def rep(old,new,label):
    global t
    assert t.count(old)==1, (label, t.count(old))
    t=t.replace(old,new)

# ---------------- ADD 1: the HOLD row ----------------
ADD1 = """=== RULE #8 - THE HOLD ROW ENDS THE TURN'S DEAD WINDOWS ===
The LAST row of nearly every casting menu reads "Hold priority for the rest of this turn: pass now,
and do not ask me again unless the board changes (any change re-opens this window; you give up no
cast)". Read it literally: it costs NOTHING. It does not end your turn - the window re-opens
by itself the moment anything on the board changes, so every cast you could still have made this
turn is still yours.
TAKE IT instead of "Cast nothing right now" whenever either of these is printed:
  - "NO LIVE CAST ROW ON THIS MENU: all N cast rows ... reads zero" - the engine has already
    checked every row and none changes a number on the board; or
  - "[you declined this exact list N times already this turn]".
Last corpus 228 windows printed the hold row and you took it ONCE, answering "Cast nothing right
now" 147 times instead - 149 of those windows under the NO-LIVE-CAST-ROW header, and one turn ran
the note up to "declined this exact list 32 times". The once you took it, it skipped 5 windows.
"Cast nothing right now" is the answer only when you WANT to be asked again this turn: you are
holding Tribute to Hunger for a creature that has not attacked yet, or something is on the stack you
may want to answer. When you are waiting on nothing, take the hold row.
Rule #2's "'Cast nothing right now' is NEVER your answer" line is about a menu with a LIVE cast row
on it; it does not reach a menu whose every row reads zero. The hold row is not casting nothing - it
is the same decline with the turn's repeats removed.

"""
rep("=== STAFF OF NIN AND SORIN ===", ADD1+"=== STAFF OF NIN AND SORIN ===", "add1")

# ---------------- ADD 2: search pick vs the [named:] tag ----------------
old2 = """    your battlefield line read "Sanguine Bond {3}{b}{b} [enchantment]" and you picked Sanguine
    Bond. Two turns later you cast that second Bond off a "[second copy:" row, and neither the
    search nor the cast changed a number on the board. THE LINE YOU READ IS YOUR OWN BATTLEFIELD
    LINE, EVERY TIME, INCLUDING THE FIFTH SEARCH: name the half that is NOT printed there.
"""
new2 = """    your battlefield line read "Sanguine Bond {3}{b}{b} [enchantment]" and you picked Sanguine
    Bond. THE LINE YOU READ IS YOUR OWN BATTLEFIELD LINE, EVERY TIME: name the half that is NOT
    printed there.
    AND SKIP A HALF THEIR Silverquill Silencer HAS NAMED. When the OPPONENT battlefield line prints
    "[named: Sanguine Bond]" (or Exquisite Blood), casting that half costs you 3 life, and the
    search list is where you choose which half you are going to have to cast. Take the OTHER one -
    even when it is the half this entry would otherwise send you after, and even when you hold
    neither. At 1 life against a Silencer naming Sanguine Bond you picked Sanguine Bond off a list
    that also offered Exquisite Blood; the very next window priced it "casting this costs you 3
    life and draws them a card - you would be at -2; this KILLS you", and that search had bought
    you the one card you could not cast. It was your last live window of the game.
"""
rep(old2,new2,"add2")

# ---------------- ADD 3: draw punishers ----------------
anchor = """ - The prompt lists "Play <land>" options: play one. Every turn, no exceptions."""
ADD3 = """ - THE PROMPT PRINTS "DRAW PUNISHERS on the battlefield: theirs - <name>" AND A "DRAW FORECAST:
   your next draw step draws N cards ... = N life": that product is damage you WILL take before you
   get another decision, and the only half of it you control is your hand size (Teferi's Puzzle Box
   redraws your whole hand, so hand size IS the multiplier). Sixteen walls do not block an
   enchantment: against that board your life total is the whole game. So empty your hand - cast the
   cheapest permanents you hold, every turn - and take TWO cheap walls over one card-neutral Idyllic
   Tutor. A second punisher doubles the number with no warning. At 16 life you cast the Tutor over a
   Pride Guardian {w} and an Overgrown Battlement {1}{g} you could have paid for together, carried a
   five-card hand into the draw step and lost at 0 on turn 11 with no further decision. It is also
   the one board where a Tribute on a [defender] is simply life: nothing of theirs ever attacks.
"""
rep(anchor, ADD3+anchor, "add3")

# ---------------- TRIMS ----------------
old_t1 = """THE ZERO STOP: the engine lists Tribute even when they have NO creature - the row says
"at 0 this does nothing". Never cast it there. It sacrifices nothing, gains nothing and taps you
out for {2}{B}. Thirty-nine such rows last corpus and you cast it TWICE, both at 6 life, in two
windows of the same turn, off a two-row menu whose other row was "Cast nothing right now".
BOTH TIMES YOUR OWN PLAN LINE SAID NOT TO - "Avoid casting Tribute to Hunger as there are no
creatures to target", and then "The opponent has no creatures, so Tribute to Hunger does nothing.
Pass the turn." THE ENGINE EXECUTES THE NUMBER ON YOUR CHOICE LINE, NOT THE SENTENCE UNDER IT. On a
two-row menu, "CHOICE: 1" is a cast; the decline is row 2. Before you write the number, check it
against the sentence you are about to write after it: if your PLAN says pass, the number is the
"Cast nothing right now" row."""
new_t1 = """THE ZERO STOP: the engine lists Tribute even when they have NO creature - the row says
"at 0 this does nothing". Never cast it there. It sacrifices nothing, gains nothing and taps you
out for {2}{B}. You have this one right: 160 such rows last corpus, nought cast. THE ENGINE EXECUTES
THE NUMBER ON YOUR CHOICE LINE, NOT THE SENTENCE UNDER IT - on a two-row menu "CHOICE: 1" is a cast
and the decline is row 2, so check the number against the sentence you are about to write after it."""
rep(old_t1,new_t1,"t1")

old_t2 = """Twenty-seven such rows last corpus, eight taken, every one of the eight for a spell off that list.
The thought "I should ramp first so I can afford Sanguine Bond" is ALWAYS FALSE in the upkeep
window: mana made there does not survive to the main phase where the enchantment is cast."""
new_t2 = """The thought "I should ramp first so I can afford Sanguine Bond" is ALWAYS FALSE in the upkeep
window: mana made there does not survive to the main phase where the enchantment is cast."""
rep(old_t2,new_t2,"t2")

old_t3 = """This is not theory. In a game you lost you tutored up Sanguine Bond, tapped two Battlements for ten
floating green mana with one black source on the board, could not cast it, passed - and the two
walls you tapped could not block on their turn. You went from 6 life to 1 that turn.
Taking that upkeep option instead does two bad things at once: the green mana is gone
before your main phase ever starts, and a tapped Battlement is a 0/4 wall that cannot block on the
opponent's next turn - a wall you needed."""
new_t3 = """This is not theory. In a game you lost you tutored up Sanguine Bond, tapped two Battlements for ten
floating green mana with one black source on the board, could not cast it, passed - and the two
walls you tapped could not block on their turn. You went from 6 life to 1. The green is gone before
your main phase starts, and a tapped Battlement is a 0/4 that cannot block on their next turn."""
rep(old_t3,new_t3,"t3")

old_t4 = """    Bond, Exquisite Blood or Idyllic Tutor is on your hand line AND your "Mana available:" line
    reads 4 or less. Then the Lantern is the fifth mana the enchantment needs, a turn earlier than
    the land drop brings it - you cast it over a Wall of Omens at 4 sources with Exquisite Blood in
    hand and the Blood came down the next turn; over two walls at 3 sources with Sanguine Bond in
    hand and the Bond came down the turn after; both games were won."""
new_t4 = """    Bond, Exquisite Blood or Idyllic Tutor is on your hand line AND your "Mana available:" line
    reads 4 or less. Then the Lantern is the fifth mana the enchantment needs, a turn earlier than
    the land drop brings it - twice you cast it over a wall at 3 and 4 sources with a half in hand,
    the half came down the next turn, and both games were won."""
rep(old_t4,new_t4,"t4")

old_t5 = """Read the NAME on your
    battlefield line. Six such rows and six declines two corpora ago - including three in a game
    where it was the ONLY cast row on the menu, which was right, because an unwinnable window is not
    a reason to spend three mana. This corpus you took it twice, at 12 and at 8 life, off menus
    whose only other Cast row was a Tribute at 0 - see Rule #2's list of rows that are not cast rows."""
new_t5 = """Read the NAME on your
    battlefield line. Six such rows and six declines two corpora ago, and no such row printed at all
    last corpus - an unwinnable window is not a reason to spend three mana."""
rep(old_t5,new_t5,"t5")

old_t6 = """ - A "Cast Sanguine Bond" or "Cast Exquisite Blood" row carries "[second copy: you already control
   <that same name>; ...]": that name is already on your battlefield line, so this is not Rule #7
   entry 1. Skip it, even when the only other row is "Cast nothing right now" - then that is your
   answer.
"""
new_t6 = """ - A "Cast Sanguine Bond" or "Cast Exquisite Blood" row carries "[second copy: you already control
   <that same name>; ...]": that name is on your battlefield line, so it is not Rule #7 entry 1.
   Skip it; if no other row is live, take the hold row (Rule #8).
"""
rep(old_t6,new_t6,"t6")

old_t7 = """ - That row reads "at 0 this does nothing": never cast Tribute to Hunger. It does nothing and
   taps you out. On the two-row menu where it is the only cast row, the answer is the OTHER row -
   and check your CHOICE number against your own PLAN sentence before you send it.
"""
new_t7 = """ - That row reads "at 0 this does nothing": never cast Tribute to Hunger. It does nothing and taps
   you out. On the two-row menu where it is the only cast row, the answer is the hold row (Rule #8)
   - and check your CHOICE number against your own PLAN sentence before you send it.
"""
rep(old_t7,new_t7,"t7")

old_t8 = """ - Your Upkeep, and the only offered action taps Overgrown Battlement for mana: answer CHOICE: 0 (pass)
   and keep the wall untapped for their turn.
 - ANY window whose only offered actions are "Add N green mana with Overgrown Battlement": pass,
   unless you can name the spell you are casting with it in this same decision AND its coloured
   pips come from your untapped lands.
"""
new_t8 = """ - ANY window whose only offered action taps Overgrown Battlement for mana (your upkeep or any
   other): pass, and keep the wall untapped for their turn - unless you can name the spell you are
   casting with it in this same decision AND its coloured pips come from your untapped lands.
"""
rep(old_t8,new_t8,"t8")

old_t9 = """Three times one corpus you answered "CHOICE: 1 (Cast Sanguine Bond)" or "(Cast Exquisite
Blood)" to a menu whose row 1 was a Battlement, a Lantern or a Tribute at 0, and the fallback
player cast for you; once the next corpus ("Cast Exquisite Blood" at 3 mana with {4}{b} in hand),
and the engine asked again."""
new_t9 = """Three times one corpus you answered "CHOICE: 1 (Cast Sanguine Bond)" or "(Cast Exquisite
Blood)" to a menu whose row 1 was a Battlement, a Lantern or a Tribute at 0, and the fallback
player cast for you; once the next corpus, and once last corpus - "CHOICE: 1 (Cast Exquisite
Blood)" to a menu whose only cast row was a Tribute at 0, with the Blood sitting on your HAND line
one mana source short. The engine refused the whole reply and the window was spent."""
rep(old_t9,new_t9,"t9")


old_t10 = """    THIS ENTRY BROKE THIS CORPUS AND THE BREAK IS A WHOLE GAME. Eleven windows printed an entry-1
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
"""
new_t10 = """    NOTHING BELOW ENTRY 1 IS AN ANSWER WHILE AN ENTRY-1 ROW IS PRINTED - not a wall, not a Lantern,
    not a Tutor, not "leaves N", not a life total, not a plan you wrote two windows ago. Read the
    menu for the two names FIRST, every window, before you read anything else on it. That read
    instruction fixed this entry: it broke 4 of 11 two corpora ago and went 10 of 10 last corpus,
    the only declines being three rows priced "this KILLS you" below.
"""
rep(old_t10,new_t10,"t10")

old_t11 = """    THE ROW SAYS WHICH IT IS, AND FOR THESE TWO CARDS THE TAG IS THE BARE ONE: "[second copy: you
    already control Sanguine Bond; both stay on the battlefield - no legend rule]", with nothing
    after "rule". A Chromatic Lantern's second copy now prints a longer tag that tells you outright
    the effect is already on; YOURS DOES NOT, and the absence of that sentence is not permission.
    The tag means the copy is legal, NOT that it is worth casting; a second Blood or a second Bond
    changes nothing about who is winning. You cast a second Sanguine Bond off that bare tag at 20
    life on a two-row menu last corpus - the other row was "Cast nothing right now", and "Cast
    nothing right now" was the answer.
    At 11 life with that tag on the row you tapped out for a second Exquisite Blood while "Cast
    Tribute to Hunger {right now: they control 2 creatures}" sat two rows below it at "leaves 2",
    and you were dead three turns later. When the tag is printed, this entry has NOT matched -
    go down the list.
"""
new_t11 = """    THE ROW SAYS WHICH IT IS, AND FOR THESE TWO CARDS THE TAG IS THE BARE ONE: "[second copy: you
    already control Sanguine Bond; both stay on the battlefield - no legend rule]", with nothing
    after "rule". The tag means the copy is LEGAL, not that it is worth casting; a second Blood or
    a second Bond changes nothing about who is winning, and the absence of an explaining sentence
    after "rule" is not permission. When the tag is printed, this entry has NOT matched - go down
    the list. No such row printed at all last corpus.
"""
rep(old_t11,new_t11,"t11")

old_t12 = """    countered, exiled or drawn past. Twice last corpus you read that clause and took a wall or a
    Tutor instead - once at 25 life, where the Sanguine Bond then arrived SIX TURNS LATE and you
    lost that game by five life; once at 9 life, where it cost you two turns of a game you won
    anyway. When the row is entry 1 and the row says "casting this taps you out", CAST IT.
"""
new_t12 = """    countered, exiled or drawn past. Two corpora ago you read that clause twice and took a wall or a
    Tutor instead; last corpus you took every one of them, and three of your four combo kills came
    off a half cast at "taps you out". When the row is entry 1 and says "casting this taps you out",
    CAST IT.
"""
rep(old_t12,new_t12,"t12")

old_t13 = """ - Both on your battlefield and NONE of those four available (their line shows no creature, and you
   control no Staff of Nin, no Sorin and no Vampire): the loop has no starter and nothing the
   opponent does will start it. Cast Staff of Nin, else Sorin, else Wall of Omens, else any wall -
   every turn, until one of the four appears. Do not answer "Cast nothing right now" here.
"""
new_t13 = """ - Both on your battlefield and NONE of those four available (their line shows no creature, and you
   control no Staff of Nin, no Sorin and no Vampire): the loop has no starter and nothing the
   opponent does will start it. Cast Staff of Nin, else Sorin, else Wall of Omens, else any wall -
   every turn, until one of the four appears; only when the menu prints no live cast row at all is
   the hold row (Rule #8) your answer.
"""
rep(old_t13,new_t13,"t13")



old_t14 = """You got that right once this corpus (turn 20, a dead Tutor beside nothing else - you declined) and
wrong six times in one game: three dead Tutors, a second Sanguine Bond, and two second Chromatic
Lanterns at 12 and 8 life, every one off a menu whose only other Cast row was a Tribute at 0. Six
spends, six turns, and not one of them changed a number on the board.
"""
new_t14 = """Two corpora ago you got that wrong six times in one game - three dead Tutors, a second Sanguine
Bond and two second Chromatic Lanterns, every one off a menu whose only other Cast row was a Tribute
at 0. Six spends, six turns, and not one changed a number on the board. Last corpus: none of them,
off 160 Tribute-at-0 rows and 4 dead-Tutor rows. When the menu is empty like that, the hold row
(Rule #8) is the answer, not another decline.
"""
rep(old_t14,new_t14,"t14")

old_t15 = """the wrong answer to five windows in a row with a Wall of Omens on the menu; that is exactly what
you answered at 9, 7, 5, 3 and 1 life in the game above - AND AGAIN LAST CORPUS, twice in one turn
at 4 life with the pair out and "Cast Wall of Omens" printed on both menus. You died the next turn
to four attackers with one blocker on your board. A wall you cast is a blocker you have; while both
names are on your battlefield line, "Cast nothing right now" is NEVER your answer to a menu that
prints any "Cast" row at all.
"""
new_t15 = """the wrong answer to five windows in a row with a Wall of Omens on the menu; that is exactly what
you answered at 9, 7, 5, 3 and 1 life in the game above, and again in the corpus after it at 4 life
with the pair out. A wall you cast is a blocker you have; while both names are on your battlefield
line, "Cast nothing right now" is NEVER your answer to a menu that prints any live "Cast" row.
"""
rep(old_t15,new_t15,"t15")


new=t.encode('ascii')
open('/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave58/deck126/strategy.txt','wb').write(new)
print('orig',orig,'new',len(new),'delta',len(new)-orig)
