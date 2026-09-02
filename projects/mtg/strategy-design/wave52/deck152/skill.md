# deck152 -> strategy-writing skill: proposals from wave-52 evidence

## S4 - A GUIDE'S "WORST CASE" PARAGRAPH LEAKS ONTO THE ADJACENT, MILDER CASE
**Evidence, the cleanest cross-window failure I have.** deck152's converter section has two
branches: BOTH names printed (Sanguine Bond **and** Exquisite Blood) -> *"ATTACK: none, every
combat, fliers included - 'no creature they control can block this attacker' is exactly why it
kills you"*; ONE name printed -> *"the race is real: race it with your EVASION"*. In `152v126`
only Sanguine Bond was ever in play. At s24 (their life 28) and s45 (their life 32) the seat held
**five** A-lines that all read `[no creature they control can block this attacker]`, and its
replies quote the both-names logic: *"Do not attack due to Sanguine Bond converting opponent life
gains to damage"*, *"attack with evasion if available (currently none)"* - with the evasion
printed in front of it. The both-names paragraph is longer, more vivid, carries the WHAT THIS
COST, and names the exact literal; the one-name branch is one clause with no literal in it.

**Proposal.** In a two-branch rule where one branch is catastrophic and one is ordinary, the skill
should require the branches to be **symmetrically specified**: the milder branch names its own
engine literal, states its own action in the same imperative register, and carries its own
WHAT-THIS-BOUGHT/COST. An asymmetric pair is read as one rule with a caveat. Diagnostic to run at
review time: **strip the headings and read only the imperatives - if one branch has three and the
other has none, the pair will collapse toward the loud one.**

## S5 - RE-KEY A RULE TO THE NEWEST, MOST LOCAL LITERAL AS SOON AS THE ENGINE PRINTS ONE
**Evidence.** deck152's #2a RULE tells the pilot to read the mana line's `"Those sources, one per
untapped card:"` list and check whether it names a creature it means to attack with - an
inference across two lines of the prompt. The engine now prints the answer **on the option row
itself**: `{paying this taps: Katilda, Dawnhart Prime, Luminarch Aspirant, Luminarch Aspirant -
they cannot attack this turn}`. deck152 met 10 such rows and took 3, including a Katilda counter
activation in UPKEEP that tapped three Aspirants out of the combat that followed (`152v162` s11,
s19, s27). The rule was correct and unfollowed; the literal was on screen and unquoted.

**Proposal.** The skill should treat each new emitter clause as a **re-key obligation on every
guide whose rule it makes local** - the review pass asks, for each rule, "is there now a printed
string that states this rule's conclusion?" and moves the rule's trigger onto it, keeping the old
derivation as the fallback sentence rather than the primary. This is the third wave in a row where
a rule survived only in its derived form while the direct form went unquoted.

## S6 - AN ABSOLUTE DEPLOY/ACT FLOOR NEEDS AN EXPLICIT DEAD-CARD CARVE-OUT, OR IT SCORES CORRECT
PLAY AS A BREAK
**Evidence.** deck152's #3 RULE ("on YOUR main phase, if a creature is offered, cast one") gave 10
apparent breaks; **9** were menus whose only creature row was
`Cast Katilda, Dawnhart Prime {g}{w} (1/1) [legendary: you already control Katilda, Dawnhart Prime
- legend rule: casting this sends one copy to your graveyard]`. Declining is right there; the
model got it right nine times against its own guide. Only one break (`152v130` s7) was real.

**Proposal.** When the skill adds a floor of the form "if X is offered, take one", it should
require the floor to enumerate the **engine-marked dead forms of X** that the floor does not
reach - `[legendary:` second copies, rows the engine prints "at 0 this does nothing", rows whose
only targets are the pilot's own. Otherwise the floor generates false breaks that cost review
attention and, worse, invite an edit that would make play worse. This is cheap: the engine
already marks every one of these.
