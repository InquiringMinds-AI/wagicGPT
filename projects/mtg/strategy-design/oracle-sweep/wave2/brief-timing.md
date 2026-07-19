# Wave-2 class: timing — 21 cards

## Celestial Sword
JUDGE REASON: The effect that removes the creature at the beginning of the next end step has changed. TEXT A states the creature is 'destroyed' and explicitly notes it 'can't be regenerated'. TEXT B states the 'sac
CURRENT ORACLE: {3}, {T}: Target creature you control gets +3/+3 until end of turn. Its controller sacrifices it at the beginning of the next end step.
```
FILE mtg.txt
[card]
name=Celestial Sword
auto={3}{T}:target(creature|mybattlefield) transforms((,newability[phaseaction[end once] bury],newability[3/3])) ueot
text={3}, {T}: Target creature you control gets +3/+3 until end of turn. Destroy it at the beginning of the next end step. A creature destroyed this way can't be regenerated.
mana={6}
type=Artifact
[/card]
```

## Grizzled Wolverine
JUDGE REASON: The functional mechanics of the ability have changed significantly. TEXT A describes a **triggered ability** that activates automatically when the creature becomes blocked, giving the player a choice:
CURRENT ORACLE: {R}: This creature gets +2/+0 until end of turn. Activate only during the declare blockers step, only if at least one creature is blocking this creature, and only once each turn.
```
FILE mtg.txt
[card]
name=Grizzled Wolverine
auto=@combat(blocked) source(this):pay({R}) 2/0 ueot
text=Whenever Grizzled Wolverine becomes blocked, you may pay {R}. If you do, Grizzled Wolverine gets +2/+0 until end of turn.
mana={1}{R}{R}
type=Creature
subtype=Wolverine
power=2
toughness=2
[/card]
```

## Homarid
JUDGE REASON: The condition for removing the counters has changed. TEXT A triggers the removal only when there are **exactly four** tide counters. TEXT B triggers the removal when there are **four or more** tide. A
CURRENT ORACLE: This creature enters with a tide counter on it. -- At the beginning of your upkeep, put a tide counter on this creature. -- As long as there is exactly one tide counter on this creature, it gets -1/-1. -- As long as there are exactly three tide counters on this creature, it gets +1/+1. -- Whenever there are four or more tide counters on this creature, remove all tide counters from it.
```
FILE mtg.txt
[card]
name=Homarid
auto=counter(0/0,1,Tide)
auto=@each my upkeep:all(homarid[counter{0/0.4.Tide}]) removeallcounters(0/0,1,Tide)
auto=@each my upkeep:counter(0/0,1,Tide)
auto=this(counter{0/0,1,Tide}=) -1/-1
auto=this(counter{0/0,3,Tide}=) 1/1
text=Homarid enters with a tide counter on it. -- At the beginning of your upkeep, put a tide counter on Homarid. -- As long as there is exactly one tide counter on Homarid, it gets -1/-1. -- As long as there are exactly three tide counters on Homarid, it gets +1/+1. -- Whenever there are four tide counters on Homarid, remove all tide counters from it.
mana={2}{U}
type=Creature
subtype=Homarid
power=2
toughness=2
[/card]
```

## Soul's Attendant
JUDGE REASON: The addition of the word 'may' in TEXT B changes the card from a mandatory triggered ability to a optional one. In TEXT A, the player is forced to gain 1 life whenever the condition is met. In TEXT B,
CURRENT ORACLE: Whenever another creature enters, you may gain 1 life.
```
FILE mtg.txt
[card]
name=Soul's Attendant
auto=@movedTo(other creature|battlefield):life:1
text=Whenever another creature enters, you gain 1 life.
mana={W}
type=Creature
subtype=Human Cleric
power=1
toughness=1
[/card]
```

## World at War
JUDGE REASON: The text differs on a critical functional condition: TEXT A specifies the effect triggers 'After the **first** main phase', whereas TEXT B specifies 'After the **second** main phase'. This changes the
CURRENT ORACLE: After the second main phase this turn, there's an additional combat phase followed by an additional main phase. At the beginning of that combat, untap all creatures that attacked this turn. -- Rebound (If you cast this spell from your hand, exile it as it resolves. At the beginning of your next upkeep, you may cast this card from exile without paying its mana cost.)
```
FILE mtg.txt
[card]
name=World at War
auto=nextphasealter(add,combatphaseswithmain,controller,after<postbattle>)
auto=all(creature|mybattlefield) transforms((,newability[@each my combatends turnlimited:phaseaction[combatbegins once sourceinplay] untap])) ueot
auto=_REBOUND_
text=After the first second main phase this turn, there's an additional combat phase followed by an additional main phase. At the beginning of that combat, untap all creatures that attacked this turn. -- Rebound (If you cast this spell from your hand, exile it as it resolves. At the beginning of your next upkeep, you may cast this card from exile without paying its mana cost.)
mana={3}{R}{R}
type=Sorcery
[/card]
```

## Base Camp
JUDGE REASON: TEXT B includes the static ability 'This land enters tapped,' which is absent in TEXT A. This is a functional change regarding the timing of when the land becomes available to use (it cannot be used '
CURRENT ORACLE: This land enters tapped. -- {T}: Add {C}. -- {T}: Add one mana of any color. Spend this mana only to cast a Cleric, Rogue, Warrior, or Wizard spell or to activate an ability of a Cleric, Rogue, Warrior, or Wizard.
```
FILE borderline.txt
[card]
name=Base Camp
auto=tap(noevent)
auto={T}:add{C}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:myrestrictedcastingzone}>0) {T}:add{B}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:mybattlefield}>0) {T}:add{B}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:myrestrictedcastingzone}>0) {T}:add{W}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:mybattlefield}>0) {T}:add{W}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:myrestrictedcastingzone}>0) {T}:add{R}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:mybattlefield}>0) {T}:add{R}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:myrestrictedcastingzone}>0) {T}:add{U}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:mybattlefield}>0) {T}:add{U}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:myrestrictedcastingzone}>0) {T}:add{G}
auto=this(variable{type:creature[cleric;rogue;warrior;wizard]:mybattlefield}>0) {T}:add{G}
text={T}: Add {C}. -- {T}: Add one mana of any color. Spend this mana only to cast a Cleric, Rogue, Warrior, or Wizard spell, or to activate an ability of a Cleric, Rogue, Warrior, or Wizard.
type=Land
[/card]
```

## Blind Fury
JUDGE REASON: TEXT A includes a casting restriction ('Cast Blind Fury only before the combat damage step') that is absent in TEXT B. This restriction limits the timing of when the spell can be cast, which is a core
CURRENT ORACLE: All creatures lose trample until end of turn. If a creature would deal combat damage to a creature this turn, it deals double that damage to that creature instead.
```
FILE borderline.txt
[card]
name=Blind Fury
restriction=before battle damage
auto=all(creature) transforms((,newability[-trample],newability[@combatdamaged(creature) from(this):damage:thatmuch all(trigger[to])])) ueot
text=Cast Blind Fury only before the combat damage step. -- All creatures lose trample until end of turn. If a creature would deal combat damage to a creature this turn, it deals double that damage to that creature instead.
mana={2}{R}{R}
type=Instant
[/card]
```

## Expedition Supplier
JUDGE REASON: The trigger condition has changed. TEXT A triggers on 'Human' creatures entering, while TEXT B triggers on 'Humans and/or Warriors'. This adds a new creature type (Warrior) that can trigger the card's
CURRENT ORACLE: Whenever one or more Humans and/or Warriors you control enter, conjure a card named Utility Knife onto the battlefield. This ability triggers only once each turn.
```
FILE borderline.txt
[card]
name=Expedition Supplier
auto=name(Conjure a card) conjure cards(Utility Knife) zone(mybattlefield)
auto=@movedTo(other *[human]|myBattlefield) turnlimited:name(Conjure a card) conjure cards(Utility Knife) zone(mybattlefield)
text=Whenever Expedition Supplier or another Human enters under your control, conjure a card named Utility Knife onto the battlefield. This ability triggers only once each turn.
mana={2}{W}
type=Creature
subtype=Human Warrior
power=2
toughness=2
[/card]
```

## Inquisitor Captain
JUDGE REASON: The functional condition for the triggered ability has changed. TEXT A triggers simply when the card enters the battlefield if the graveyard/hand/library condition is met. TEXT B adds a specific, new,
CURRENT ORACLE: Vigilance -- When this creature enters, if you cast it and there are twenty or more creature cards with mana value 3 or less among cards in your graveyard, hand, and library, seek two creature cards with mana value 3 or less. Put one of them onto the battlefield and shuffle the other into your library.
```
FILE borderline.txt
[card]
name=Inquisitor Captain
abilities=vigilance
auto=if type(creature[manacost<=3]|mynonplaynonexile)~morethan~19 then name(Seek 2 creatures) name(Seek 2 creatures) moverandom(creature[manacost<=3]) from(mylibrary) to(myhand) and!( moverandom(creature[manacost<=3]) from(mylibrary) to(myhand) and!( name(Put in play) target(creature[fresh]|myhand) moveto(mybattlefield) and!( name(Shuffle back) target(creature[fresh]|myhand) moveto(mylibrary) and!( shuffle )! )! )! )!
text=Vigilance -- When Inquisitor Captain enters, if there are twenty or more creature cards with mana value 3 or less among cards in your graveyard, hand, and library, seek two creature cards with mana value 3 or less. Put one of them onto the battlefield and shuffle the other into your library.
mana={3}{W}
type=Creature
subtype=Human Cleric
power=3
toughness=3
[/card]
```

## Lanterns' Lift
JUDGE REASON: TEXT B includes a new triggered ability ('If Lanterns' Lift would be put into a graveyard from anywhere, exile it instead.') that is completely absent in TEXT A. This changes the card's behavior by ex
CURRENT ORACLE: Enchant creature -- Enchanted creature gets +1/+1 and has flying. -- If Lanterns' Lift would be put into a graveyard from anywhere, exile it instead.
```
FILE borderline.txt
[card]
name=Lanterns' Lift
abilities=exiledeath
backside=Lantern Bearer
target=creature
text=Enchant creature -- Enchanted creature gets +1/+1 and has flying. // Lantern Bearer
color=blue
type=Enchantment
subtype=Aura
[/card]
```

## Millicent, Restless Revenant
JUDGE REASON: The triggered ability in TEXT A triggers when the Spirit 'deals damage to a player' (which includes non-combat damage, such as from a spell or ability). The triggered ability in TEXT B triggers only '
CURRENT ORACLE: Affinity for Spirits (This spell costs {1} less to cast for each Spirit you control.) -- Flying -- Whenever Millicent or another nontoken Spirit you control dies or deals combat damage to a player, create a 1/1 white Spirit creature token with flying.
```
FILE borderline.txt
[card]
name=Millicent, Restless Revenant
abilities=flying
anyzone=affinity(spirit|mybattlefield) reduce({1})
auto=@damaged(player) from(spirit[-token]|myBattlefield):name(Create Spirit) _SPIRITTOKEN_
auto=@movedto(spirit[-token]|graveyard) from(myBattlefield):name(Create Spirit) _SPIRITTOKEN_
text=This spell costs {1} less to cast for each Spirit you control. -- Flying -- Whenever Millicent, Restless Revenant or another nontoken Spirit you control dies or deals damage to a player, create a 1/1 white Spirit creature token with flying.
mana={5}{W}{U}
type=Legendary Creature
subtype=Spirit Soldier
power=4
toughness=4
[/card]
```

## Oglor, Devoted Assistant
JUDGE REASON: The source zone condition for the triggered ability has changed. TEXT A specifies the ability triggers only when a creature card is put into the graveyard **from your library**. TEXT B expands this to
CURRENT ORACLE: At the beginning of your upkeep, look at the top two cards of your library, then put one of them into your graveyard. -- Whenever a creature card is put into your graveyard from your library or hand, it perpetually gains "When this card leaves your graveyard, create a tapped 2/2 black Zombie creature token."
```
FILE borderline.txt
[card]
name=Oglor, Devoted Assistant
aicode=activate transforms((,newability[target(*[zpos<=2]|mylibrary) moveto(mygraveyard)])) ueot
auto=@each my upkeep:name(Look top 2 card) reveal:2 optionone name(Put in graveyard) target(*|reveal) moveto(mylibrary) and!( moveto(mygraveyard) )! optiononeend optiontwo name(Put back) target(*|reveal) moveto(mylibrary) optiontwoend revealend
auto=@movedto(creature|mygraveyard) from(mylibrary):name(Gains ability) all(trigger[to]) name(Gains ability) counter(0/0.1.PerpetualZombie) notrg
auto=emblem transforms((,newability[@movedto(*|myzones) from(mygraveyard):name(Create zombie) all(trigger[to]) name(Create zombie) transforms((,newability[if compare(hascntperpetualzombie)~morethan~0 then _ZOMBIETOKEN_ and!( tap(noevent) )!])) oneshot])) forever dontremove
text=At the beginning of your upkeep, look at the top two cards of your library, then put one of them into your graveyard. -- Whenever a creature card is put into your graveyard from your library, it perpetually gains "When this card leaves your graveyard, create a 2/2 tapped black Zombie creature token."
mana={1}{U}
type=Legendary Creature
subtype=Homunculus
power=1
toughness=1
[/card]
```

## Pirated Copy
JUDGE REASON: The triggered ability in TEXT A uses the modal phrase 'you may draw a card,' giving the controller a choice to draw or not. The ability in TEXT B uses the mandatory phrase 'you draw a card,' removing 
CURRENT ORACLE: You may have this creature enter as a copy of any creature on the battlefield, except it's a Pirate in addition to its other types and it has "Whenever this creature or another creature with the same name deals combat damage to a player, you draw a card."
```
FILE borderline.txt
[card]
name=Pirated Copy
auto=may name(Copy creature) notaTarget(creature|battlefield) copy and!( transforms((,newability[@combatdamaged(player) from(creature[share!name!]|battlefield):may name(Draw a card) draw:1 controller])) forever )!
text=You may have Pirated Copy enter the battlefield as a copy of any creature on the battlefield, except it's a Pirate in addition to its other types and it has "Whenever this creature or another creature with the same name deals combat damage to a player, you may draw a card."
mana={4}{U}
type=Creature
subtype=Shapeshifter Pirate
power=0
toughness=0
[/card]
```

## Town-razer Tyrant
JUDGE REASON: The timing of the triggered ability on the land has changed. TEXT A specifies 'At the beginning of your end step', while TEXT B specifies 'At the beginning of your upkeep'. This is a functional change
CURRENT ORACLE: Flying -- When this creature enters the battlefield, target nonbasic land you don't control loses all abilities except mana abilities and gains "At the beginning of your upkeep, this permanent deals 2 damage to you unless you sacrifice it."
```
FILE borderline.txt
[card]
name=Town-razer Tyrant
abilities=flying
auto=name(Land loses abilities) target(land[-basic]|battlefield) transforms((,newability[onlymanaability],newability[@each my upkeep:ability$!name(Sacrifice or damage) choice name(Sacrifice) all(mysource) sacrifice _ choice name(Damage) damage:2 controller!$ controller])) forever
text=Flying -- When Town-Razer Tyrant enters, target nonbasic land you don't control loses all abilities except mana abilities and gains "At the beginning of your end step, this permanent deals 2 damage to you unless you sacrifice it."
mana={2}{R}{R}
type=Creature
subtype=Dragon
power=4
toughness=4
[/card]
```

## Twilight Prophet
JUDGE REASON: TEXT B includes the keyword ability 'Flying', which is completely absent from TEXT A. This is a functional change to the card's abilities, affecting how it interacts with combat and blockers.
CURRENT ORACLE: Flying -- Ascend (If you control ten or more permanents, you get the city's blessing for the rest of the game.) -- At the beginning of your upkeep, if you have the city's blessing, reveal the top card of your library and put it into your hand. Each opponent loses X life and you gain X life, where X is that card's mana value.
```
FILE borderline.txt
[card]
name=Twilight Prophet
abilities=flying
auto=_ASCEND_
auto=@movedTo(*|myBattlefield) restriction{type(City's Blessing|mybattlefield)~equalto~0}:_ASCEND_
auto=@each my upkeep restriction{type(City's Blessing|mybattlefield)~morethan~0}:name(Reveal top card) all(*[zpos=1]|mylibrary) moveto(hand) and!( transforms((,newability[damage:manacost opponent],newability[life:manacost controller])) ueot )!
text=Ascend (If you control ten or more permanents, you get the city's blessing for the rest of the game.) -- At the beginning of your upkeep, if you have the city's blessing, reveal the top card of your library and put it into your hand. Each opponent loses X life and you gain X life, where X is that card's mana value.
mana={2}{B}{B}
type=Creature
subtype=Vampire Cleric
power=2
toughness=4
[/card]
```

## Virtue of Persistence
JUDGE REASON: TEXT A includes two abilities that are completely absent from TEXT B: a static effect that gives a target creature -3/-3 until end of turn, and a triggered ability that allows you to gain 2 life. TEXT
CURRENT ORACLE: At the beginning of your upkeep, put target creature card from a graveyard onto the battlefield under your control.
```
FILE borderline.txt
[card]
name=Virtue of Persistence
abilities=adventure
other={1}{B} name(Adventure Locthwain Scorn)
auto=alternative target(creature) -3/-3 ueot
auto=alternative life:2
auto=alternative _ADVENTURE_
auto=@each my upkeep:target(creature|graveyard) moveto(mybattlefield)
text=Adventre Locthwain Scorn Target creature gets -3/-3 until end of turn. You gain 2 life. At the beginning of your upkeep, put target creature card from a graveyard onto the battlefield under your control.
mana={5}{B}{B}
type=Enchantment
[/card]
```

## Akki Lavarunner
JUDGE REASON: TEXT A includes the ability 'Protection from red' and a triggered ability that increases damage dealt by red sources to players ('If a red source would deal damage to a player, it deals that much +1'—
CURRENT ORACLE: Haste -- Whenever this creature deals damage to an opponent, flip it.
```
FILE unsupported.txt
[card]
name=Akki Lavarunner
text=Haste -- Whenever Akki Lavarunner deals damage to an opponent, flip it. // Tok-Tok, Volcano Born -- Legendary Creature - Goblin Shaman -- Protection from red -- If a red source would deal damage to a player, it deals that much damage plus 1 to that player instead. -- 2/2
mana={3}{R}
type=Creature
subtype=Goblin Warrior
power=1
toughness=1
[/card]
```

## Aretopolis
JUDGE REASON: The third ability in TEXT A triggers on 'Whenever you roll {K}', whereas TEXT B triggers on 'Whenever chaos ensues'. In Magic: The Gathering, 'chaos' is a specific keyword ability found on cards like 
CURRENT ORACLE: When you planeswalk to Aretopolis and at the beginning of your upkeep, put a scroll counter on Aretopolis, then you gain life equal to the number of scroll counters on it. -- When Aretopolis has ten or more scroll counters on it, planeswalk. -- Whenever chaos ensues, put a scroll counter on Aretopolis, then draw cards equal to the number of scroll counters on it.
```
FILE unsupported.txt
[card]
name=Aretopolis
text=When you planeswalk to Aretopolis or at the beginning of your upkeep, put a scroll counter on Aretopolis, then you gain life equal to the number of scroll counters on it. -- When Aretopolis has ten or more scroll counters on it, planeswalk. -- Whenever you roll {K}, put a scroll counter on Aretopolis, then draw cards equal to the number of scroll counters on it.
type=Plane
subtype=Kephalai
[/card]
```

## Kilnspire District
JUDGE REASON: The two texts describe fundamentally different triggers and targets. TEXT A triggers on 'rolling {K}' (a specific die result) and targets 'creature or player'. TEXT B triggers on 'chaos ensues' (a new
CURRENT ORACLE: When you planeswalk to Kilnspire District and at the beginning of your first main phase, put a charge counter on Kilnspire District, then add {R} for each charge counter on it. -- Whenever chaos ensues, you may pay {X}. If you do, Kilnspire District deals X damage to any target.
```
FILE unsupported.txt
[card]
name=Kilnspire District
text=When you planeswalk to Kilnspire District or at the beginning of your precombat main phase, put a charge counter on Kilnspire District, then add {R} to your mana pool for each charge counter on it. -- Whenever you roll {K}, you may pay {X}. If you do, Kilnspire District deals X damage to target creature or player.
type=Plane
subtype=Ravnica
[/card]
```

## Tember City
JUDGE REASON: The second ability in TEXT A triggers on 'Whenever you roll {K}', whereas the second ability in TEXT B triggers on 'Whenever chaos ensues'. In Magic: The Gathering, 'chaos' is a specific keyworded or 
CURRENT ORACLE: Whenever a player taps a land for mana, Tember City deals 1 damage to that player. -- Whenever chaos ensues, each other player sacrifices a nonland permanent of their choice.
```
FILE unsupported.txt
[card]
name=Tember City
text=Whenever a player taps a land for mana, Tember City deals 1 damage to that player. -- Whenever you roll {K}, each other player sacrifices a nonland permanent.
type=Plane
subtype=Kinshala
[/card]
```

## Trail of the Mage-Rings
JUDGE REASON: The second ability has a functional change in its trigger condition. TEXT A triggers on 'Whenever you roll {K}' (referring to the Chaos symbol on a die), while TEXT B triggers on 'Whenever chaos ensue
CURRENT ORACLE: Instant and sorcery spells have rebound. (The spell's controller exiles the spell as it resolves if they cast it from their hand. At the beginning of that player's next upkeep, they may cast that card from exile without paying its mana cost.) -- Whenever chaos ensues, you may search your library for an instant or sorcery card, reveal it, put it into your hand, then shuffle.
```
FILE unsupported.txt
[card]
name=Trail of the Mage-Rings
text=Instant and sorcery spells have rebound. (The spell's controller exiles the spell as it resolves if he or she cast it from his or her hand. At the beginning of that player's next upkeep, he or she may cast that card from exile without paying its mana cost.) -- Whenever you roll {K}, you may search your library for an instant or sorcery card, reveal it, put it into your hand, then shuffle your library.
type=Plane
subtype=Vryn
[/card]
```

