# Owner Vita play notes — received 2026-09-01 (verbatim, order = as noted; blank line separates entries)

We need a hybrid test harness. Something that lets an ai agent query the interface for state info, and send inputs in the human way by keyboard or mouse. A... Wagicgpt gui MCP. That would make it possible to drive the real interface to test for bugs that don't repro in the headless test suite, without relying on costly and slow vision tests.

Claude says open for next time: Metamorph→Muse, Hatcher counter indicator, autotap "worst combo" — none reproduced on desktop; need a concrete board from Vita play. The heap-corruption crash needs on-device evidence (desktop ASAN is exhausted). Frame lag: no mechanism found.

My vita vpk should log the sequence of every match, so that when I experience an issue, the investigating agent can directly repro the offending play sequence. This can be a regular diagnostic function for the development build, that should would not be included in a release build. Or? Include it? Replay feature? After the match, there should be a dialog so I can classify it. Some category ideas: bug, bad blocking, bad targeting, bad blocking, bad mana selection, no problems.

Hurkyl, master wizard, after selecting a card, and put on bottom, the card selector section refused to close, across all vita buttons, the game refused to continue, forced to exit to main menu.
Worked every subsequent time

Ajani pridemate has card art for ajani strength of the pride.

Core dump. The game immediately previous had extreme framelag in the last several rounds of the match. After losing, the crash happened post loss screen.

1
Forest
Bloodforged battle-axe
O
Swamp
2
Swamp
Hapatra vizier of poisons
O
Swamp
Walking dead
3
Swamp
Attach equipment
Attack all
(A good opponent should have blocked, but it doesn't)
Hapatras ability targets walking dead, makes a snake token
O
Swamp
Champion of the perished
Scarscale ritual putting counter on champion.
And then the bug is that a snake token is created under my control.

I have a squirrel sovereign with rancor attached(4/2)
Opponent used rakdos return to make me discard
I discard 3 cards including wilt-leaf liege, which goes to the battlefield per its ability.
The bug is my squirrel sovereign is still (4/2)
Later creatures cast do not benefit from its lord abilities either.

Soft locked, took a picture or board state and returned to main menu.

With 1 plains, 3 forests, and a squirrel nest enchanted to a forest, autotap doesn't give a fuck, and all tap that squirrel nest forest to cast a yavimaya enchantress, leaving me a useless forest that cannot tap to make a squirrel

Coralhelm guide is indicating it can use an ability when it can't, I took a picture of one such board state.

Another crash, after winning a match, with a core dump.

Experiment one 1/1 and vulshock sorceror in play, tap sorceror to ping opponent for 1 damage, cast scab-clan mauler which comes in with bloodthirst 2 making it a 3/3 as it enters, and the bug is experiment one does not evolve

Ai cast show of confidence with no creatures of its own in play, buffing my creature

With 3 mana, and trancendant envoy in play, leafcrown dryad doesn't let me cast it for its bestow, it just casts normally and leaves me the one floating mana

With ajanis chosen, banishing light and a 2/2 cat token in play, cast ancestral mask on cat token, ajanis chosen makes new token and I elect to attach aura to it, auramoves. The bug is: power/toughness of the original cat token remains buffed by the aura, which moved to the new token which is not buffed by the aura. - bug repeated with ajanis chosens ability, ethereal armor moved to new token, with its effects staying on the creature it was previously attached to.

Light paws, emperors voice
Turn cycle
Ancestral mark to fox
Ability retrieved ethereal armor to fox
Attack
Turn cycle
Aura gnarlid
Etherial armor to fox
Bug is ability fetched snake umbra to fox.

Possible bug, complex rule interaction
Starfield mystic
Lightspeed emperors voice
Bestow leafcrown dryad
Foxes ability only letting me target Etherial armor, no three cost auras (no 2 cost in deck)
Is the cost of the leafcrown dryad aura the bestow cost or the creature cost? If it's the bestow cost, then bug.
- then cast etherial armor, and bugged again, it let me get a 3 cost enchantment wtf?

Another core dump, this one after I selected to return to main menu during a match

Core dump from a midmatch crash

Rootcast apprenticeship only offers 2 of its 4 options.

Wildwood scourge is triggering its ability when mossborn hydra gets a counter from proliferate activated from karns bastion

Coredump

all the psal basic lands of a type have only one cared art, its very obnoxious,
