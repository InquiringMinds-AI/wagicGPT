# Wave-2 class: numbers — 33 cards

## Awaken the Ancient
JUDGE REASON: The functional difference lies in the activation cost. TEXT A includes an activation cost of '{2}' to turn the land into a creature. TEXT B lacks this cost, implying the effect is continuous or static
CURRENT ORACLE: Enchant Mountain -- Enchanted Mountain is a 7/7 red Giant creature with haste. It's still a land.
```
FILE mtg.txt
[card]
name=Awaken the Ancient
target=land[mountain]
auto=transforms((Giant Creature,setpower=7,settoughness=7,red,haste))
text=Enchant Mountain -- {2}: Enchanted Mountain becomes a 7/7 red Giant creature with haste. It's still a land.
mana={1}{R}{R}{R}
type=Enchantment
subtype=Aura
[/card]
```

## Caller of the Hunt
JUDGE REASON: The timing and cost structure of the ability have changed. TEXT A describes an 'enters the battlefield' triggered ability ('As Caller of the Hunt enters...'), which happens after the card is cast and,
CURRENT ORACLE: As an additional cost to cast this spell, choose a creature type. -- Caller of the Hunt's power and toughness are each equal to the number of creatures of the chosen type on the battlefield.
```
FILE mtg.txt
[card]
name=Caller of the Hunt
auto=counter(0/1) notrg
auto=chooseatype transforms((,newability[type:creature[chosentype]:battlefield/type:creature[chosentype]:battlefield cdaactive],newability[counter(0/1.-1) notrg])) forever chooseend
text=As Caller of the Hunt enters, choose a creature type. -- Caller of the Hunt's power and toughness are each equal to the number of creatures of the chosen type on the battlefield.
mana={2}{G}
type=Creature
subtype=Human
power=*
toughness=*
[/card]
```

## Contested Cliffs
JUDGE REASON: The mana cost to activate the second ability has changed. TEXT A requires {R}{G} (one red and one green mana), while TEXT B requires {R}{G} (one red and one green mana) but the first ability's output,
CURRENT ORACLE: {T}: Add {C}. -- {R}{G}, {T}: Target Beast creature you control fights target creature an opponent controls. (Each deals damage equal to its power to the other.)
```
FILE mtg.txt
[card]
name=Contested Cliffs
auto={T}:add{1}
auto={R}{G}{T}:target(creature[beast]|mybattlefield) transforms((,newability[target(creature) dynamicability<!powerstrike eachother!>])) ueot
text={T}: Add {1}. -- {R}{G}, {T}: Choose target Beast creature you control and target creature an opponent controls. Those creatures fight each other. (Each deals damage equal to its power to the other.)
type=Land
[/card]
```

## Fallen Ferromancer
JUDGE REASON: The mana cost in the activated ability has changed. TEXT A specifies a cost of {2}{R}, while TEXT B specifies a cost of {1}{R}. This is a functional difference in the cost required to activate the 's'
CURRENT ORACLE: Infect (This creature deals damage to creatures in the form of -1/-1 counters and to players in the form of poison counters.) -- {1}{R}, {T}: This creature deals 1 damage to any target.
```
FILE mtg.txt
[card]
name=Fallen Ferromancer
abilities=infect
auto={2}{R}{T}:damage:1 target(anytarget)
text=Infect -- {2}{R}, {T}: Fallen Ferromancer deals 1 damage to any target.
mana={3}{R}
type=Creature
subtype=Phyrexian Human Shaman
power=1
toughness=1
[/card]
```

## Ferrovore
JUDGE REASON: The power/tightness bonus changed from +3/+3 in TEXT A to +3/+0 in TEXT B. This is a functional difference in the stat modification.
CURRENT ORACLE: {R}, Sacrifice an artifact: This creature gets +3/+0 until end of turn.
```
FILE mtg.txt
[card]
name=Ferrovore
auto={R}{S(artifact|myBattlefield)}:3/0
text={R}, sacrifice an artifact: Ferrovore gets +3/+3 until end of turn.
mana={2}{R}
type=Creature
subtype=Beast
power=2
toughness=2
[/card]
```

## Glacial Stalker
JUDGE REASON: The morph cost has changed from {2}{W} in TEXT A to {4}{U} in TEXT B. This is a change in the mana cost required to turn the card face up, which is a functional difference.
CURRENT ORACLE: Morph {4}{U} (You may cast this card face down as a 2/2 creature for {3}. Turn it face up any time for its morph cost.)
```
FILE mtg.txt
[card]
name=Glacial Stalker
facedown={3}
autofacedown={4}{U}:morph
text=Morph {2}{W} (You may cast this face down as a 2/2 creature for {3}. Turn it face up any time for its morph cost.)
mana={5}{U}
type=Creature
subtype=Elemental
power=4
toughness=5
[/card]
```

## Kabira Vindicator
JUDGE REASON: The power and toughness boost provided to other creatures changes between the two texts. TEXT A states that other creatures get +1/+1 at both Level 2-4 and Level 5+. TEXT B states that other creatures
CURRENT ORACLE: Level up {2}{W} ({2}{W}: Put a level counter on this. Level up only as a sorcery.) -- LEVEL 2-4 -- 3/6 -- Other creatures you control get +1/+1. -- LEVEL 5+ -- 4/8 -- Other creatures you control get +2/+2.
```
FILE mtg.txt
[card]
name=Kabira Vindicator
auto={2}{W}:counter(0/0,1,Level) asSorcery
auto=this(counter{0/0.2.Level}) lord(other creature|myBattlefield) 1/1
auto=this(counter{0/0.2.Level}) 1/2
auto=this(counter{0/0.5.Level}) lord(other creature|myBattlefield) 1/1
auto=this(counter{0/0.5.Level}) 1/2
auto=maxlevel:5
text=Level up {2}{W} -- [Level 2-4] Other creatures you control get +1/+1. (3/6) -- [Level 5+] Other creatures you control get +1/+1. (4/8)
mana={3}{W}
type=Creature
subtype=Human Knight
power=2
toughness=4
[/card]
```

## Leaping Master
JUDGE REASON: TEXT A describes a static ability or a continuous effect that grants flying without specifying a cost or trigger, implying the creature might always have flying or gain it automatically. TEXT B is an 
CURRENT ORACLE: {2}{W}: This creature gains flying until end of turn.
```
FILE mtg.txt
[card]
name=Leaping Master
auto={2}{W}:flying ueot
text=Leaping Master gains flying until end of turn.
mana={1}{R}
type=Creature
subtype=Human Monk
power=2
toughness=1
[/card]
```

## Mana Seism
JUDGE REASON: The mana produced is different. TEXT A produces generic mana ({1}), while TEXT B produces colorless mana ({C}). In Magic: The Gathering, generic mana can be used to pay for any cost, whereas colorless
CURRENT ORACLE: Sacrifice any number of lands, then add that much {C}.
```
FILE mtg.txt
[card]
name=Mana Seism
target=<anyamount>land|myBattlefield
auto=sacrifice && add{1}
text=Sacrifice any number of lands. Add {1} for each land sacrificed this way.
mana={1}{R}
type=Sorcery
[/card]
```

## Pilfered Plans
JUDGE REASON: The number of cards milled has changed from three in TEXT A to two in TEXT B. This is a functional difference in the game effect.
CURRENT ORACLE: Target player mills two cards. Draw two cards.
```
FILE mtg.txt
[card]
name=Pilfered Plans
target=player
auto=deplete:3
auto=draw:2 controller
text=Target player mills three cards. -- Draw two cards.
mana={1}{U}{B}
type=Sorcery
[/card]
```

## Spike Colony
JUDGE REASON: The number of +1/+1 counters the creature enters with has changed. TEXT A states it enters with one counter, while TEXT B states it enters with four. This is a functional difference in the card's game
CURRENT ORACLE: This creature enters with four +1/+1 counters on it. -- {2}, Remove a +1/+1 counter from this creature: Put a +1/+1 counter on target creature.
```
FILE mtg.txt
[card]
name=Spike Colony
text=Spike Colony enters with a +1/+1 counter on it. -- {2}, Remove a +1/+1 counter from Spike Colony: Put a +1/+1 counter on target creature.
mana={4}{G}
type=Creature
subtype=Spike
auto=counter(1/1,4)
auto={2}{C(1/1,-1)}:counter(1/1) target(creature)
power=0
toughness=0
[/card]
```

## Spike Breeder
JUDGE REASON: The number of +1/+1 counters the creature enters with has changed. TEXT A states it enters with one counter, while TEXT B states it enters with three counters. This is a functional difference in the '
CURRENT ORACLE: This creature enters with three +1/+1 counters on it. -- {2}, Remove a +1/+1 counter from this creature: Put a +1/+1 counter on target creature. -- {2}, Remove a +1/+1 counter from this creature: Create a 1/1 green Spike creature token.
```
FILE mtg.txt
[card]
name=Spike Breeder
text=Spike Breeder enters with a +1/+1 counter on it. -- {2}, Remove a +1/+1 counter from Spike Breeder: Put a +1/+1 counter on target creature. -- {2}, Remove a +1/+1 counter from Spike Breeder: Put a 1/1 green Spike creature token onto the battlefield.
mana={3}{G}
type=Creature
subtype=Spike
auto=counter(1/1,3)
auto={2}{C(1/1,-1)}:counter(1/1) target(creature)
auto={2}{C(1/1,-1)}:token(Spike,creature spike, 1/1,green)
power=0
toughness=0
[/card]
```

## Spike Soldier
JUDGE REASON: The number of +1/+1 counters the creature enters with has changed. TEXT A states it enters with one counter, while TEXT B states it enters with three. This is a functional difference in the initial状态.
CURRENT ORACLE: This creature enters with three +1/+1 counters on it. -- {2}, Remove a +1/+1 counter from this creature: Put a +1/+1 counter on target creature. -- Remove a +1/+1 counter from this creature: This creature gets +2/+2 until end of turn.
```
FILE mtg.txt
[card]
name=Spike Soldier
text=Spike Soldier enters with a +1/+1 counter on it. -- {2}, Remove a +1/+1 counter from Spike Soldier: Put a +1/+1 counter on target creature. -- {2}, Remove a +1/+1 counter from Spike Soldier: Spike Soldier gets +2/+2 until end of turn.
mana={2}{G}{G}
type=Creature
subtype=Spike Soldier
auto=counter(1/1,3)
auto={2}{C(1/1,-1)}:counter(1/1) target(creature)
auto={C(1/1,-1)}:2/2
power=0
toughness=0
[/card]
```

## Spike Feeder
JUDGE REASON: The number of +1/+1 counters the creature enters with has changed. TEXT A states it enters with one counter, while TEXT B states it enters with two counters. This is a functional difference in the 'on
CURRENT ORACLE: This creature enters with two +1/+1 counters on it. -- {2}, Remove a +1/+1 counter from this creature: Put a +1/+1 counter on target creature. -- Remove a +1/+1 counter from this creature: You gain 2 life.
```
FILE mtg.txt
[card]
name=Spike Feeder
text=Spike Feeder enters with a +1/+1 counter on it. -- {2}, Remove a +1/+1 counter from Spike Feeder: Put a +1/+1 counter on target creature. -- {2}, Remove a +1/+1 counter from Spike Feeder: You gain 2 life.
mana={1}{G}{G}
type=Creature
subtype=Spike
auto=counter(1/1,2)
auto={2}{C(1/1,-1)}:counter(1/1) target(creature)
auto={C(1/1,-1)}:life:2
power=0
toughness=0
[/card]
```

## Spike Worker
JUDGE REASON: The number of +1/+1 counters the creature enters with has changed. TEXT A states it enters with one counter, while TEXT B states it enters with two. This is a functional difference in the initial game
CURRENT ORACLE: This creature enters with two +1/+1 counters on it. -- {2}, Remove a +1/+1 counter from this creature: Put a +1/+1 counter on target creature.
```
FILE mtg.txt
[card]
name=Spike Worker
text=Spike Worker enters with a +1/+1 counter on it. -- {2}, Remove a +1/+1 counter from Spike Worker: Put a +1/+1 counter on target creature.
mana={2}{G}
type=Creature
subtype=Spike
auto=counter(1/1,2)
auto={2}{C(1/1,-1)}:counter(1/1) target(creature)
power=0
toughness=0
[/card]
```

## Spike Hatcher
JUDGE REASON: The card's function has changed in two specific ways: 1) The number of +1/+1 counters it enters with is different (Text A implies 1, Text B specifies 6). 2) The mana cost to regenerate the creature is
CURRENT ORACLE: This creature enters with six +1/+1 counters on it. -- {2}, Remove a +1/+1 counter from this creature: Put a +1/+1 counter on target creature. -- {1}, Remove a +1/+1 counter from this creature: Regenerate this creature.
```
FILE mtg.txt
[card]
name=Spike Hatcher
text=Spike Hatcher enters with a +1/+1 counter on it. -- {2}, Remove a +1/+1 counter from Spike Hatcher: Put a +1/+1 counter on target creature. -- {2}, Remove a +1/+1 counter from Spike Hatcher: Regenerate Spike Hatcher.
mana={6}{G}
type=Creature
subtype=Spike
auto=counter(1/1,6)
auto={2}{C(1/1,-1)}:counter(1/1) target(creature)
auto={1}{C(1/1,-1)}:regenerate
power=0
toughness=0
[/card]
```

## Triassic Egg
JUDGE REASON: The two texts describe fundamentally different mechanics for the final ability. TEXT A requires sacrificing the artifact AND removing two counters as a cost to put a creature from hand OR graveyard. (
CURRENT ORACLE: {3}, {T}: Put a hatchling counter on this artifact. -- Sacrifice this artifact: Choose one. Activate only if there are two or more hatchling counters on this artifact. -- • You may put a creature card from your hand onto the battlefield. -- • Return target creature card from your graveyard to the battlefield.
```
FILE mtg.txt
[card]
name=Triassic Egg
auto={3}{T}:counter(0/0,1,Hatchling)
auto={C(0/0,-2,Hatchling)}{S}:moveTo(myBattlefield) target(other creature|myhand,mygraveyard)
text={3}{T}: Put a hatchling counter on Triassic Egg. -- Remove two hatchling counters from Triassic Egg, Sacrifice Triassic Egg: You may put a creature card from your hand or graveyard onto the battlefield.
mana={4}
type=Artifact
[/card]
```

## Viridian Claw
JUDGE REASON: The Equip cost has changed from {2} in TEXT A to {1} in TEXT B. This is a change in the mana cost required to activate the ability, which is a functional difference.
CURRENT ORACLE: Equipped creature gets +1/+0 and has first strike. -- Equip {1}
```
FILE mtg.txt
[card]
name=Viridian Claw
auto={1}:equip
auto=teach(creature) 1/0
auto=teach(creature) first strike
text=Equipped creature gets +1/+0 and has first strike. -- Equip {2}
mana={2}
type=Artifact
subtype=Equipment
[/card]
```

## Arms Scavenger
JUDGE REASON: TEXT B includes an additional ability ('Equip abilities you activate cost {1} less to activate') that is completely absent from TEXT A. This represents a functional change where the card gained a new,
CURRENT ORACLE: At the beginning of your upkeep, draft a card from this creature's spellbook, then exile it. Until end of turn, you may play that card. -- Equip abilities you activate cost {1} less to activate.
```
FILE borderline.txt
[card]
name=Arms Scavenger
auto=@each my upkeep:name(Draft a card) transforms((,newability[choice name(Boots of Speed) conjure cards(Boots of Speed) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Cliffhaven Kitesail) conjure cards(Cliffhaven Kitesail) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Colossus Hammer) conjure cards(Colossus Hammer) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Dueling Rapier) conjure cards(Dueling Rapier) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Spare Dagger) conjure cards(Spare Dagger) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Tormentor's Helm) conjure cards(Tormentor's Helm) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Goldvein Pick) conjure cards(Goldvein Pick) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Jousting Lance) conjure cards(Jousting Lance) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Mask of Immolation) conjure cards(Mask of Immolation) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Mirror Shield) conjure cards(Mirror Shield) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Relic Axe) conjure cards(Relic Axe) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Rogue's Gloves) conjure cards(Rogue's Gloves) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Scavenged Blade) conjure cards(Scavenged Blade) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Shield of the Realm) conjure cards(Shield of the Realm) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!],newability[choice name(Ceremonial Knife) conjure cards(Ceremonial Knife) zone(myhand) and!( moveto(myexile) and!( canplayfromexile ueot )! )!])) oneshot
text=At the beginning of your upkeep, draft a card from Arms Scavenger's spellbook, then exile it. Until the end of turn, you may play that card.
mana={1}{R}
type=Creature
subtype=Human Warrior
power=2
toughness=2
[/card]
```

## Assemble from Parts
JUDGE REASON: The mana cost to activate the ability has changed. TEXT A requires {3}{B} (4 mana total), while TEXT B requires {1}{B}{B} (3 mana total). This is a functional change in the cost of the ability.
CURRENT ORACLE: Target creature card in your graveyard perpetually gains "{1}{B}{B}, Exile this card from your graveyard: Shuffle it into its owner's library. Create a token that's a copy of it, except it's a 4/4 black Zombie in addition to its other types. Activate only as a sorcery."
```
FILE borderline.txt
[card]
name=Assemble from Parts
target=creature|mygraveyard
auto=counter(0/0.1.PerpetualAssemble) notrg
auto=emblem transforms((,newability[lord(*[counter{0/0.1.PerpetualAssemble}]|graveyard) transforms((,newability[{3}{B}:name(Shuffle and copy) moveto(myexile) and!( moveto(mylibrary) and!( clone and!( transforms((Zombie,black,setpower=4,settoughness=4,newability[shuffle])) forever )! )! )! asSorcery]))])) forever dontremove
text=Target creature card in your graveyard perpetually gains "{3}{B}, Exile this card from your graveyard: Shuffle it into your library. Create a token that's a copy of it, except it's a 4/4 black Zombie in addition to its other types. Activate only as a sorcery."
mana={B}
type=Instant
[/card]
```

## Bounty of the Luxa
JUDGE REASON: The mana cost produced by the ability has changed. TEXT A adds {1}{G}{U} (one colorless mana plus green and blue), while TEXT B adds {C}{G}{U} (one colorless mana plus green and blue). In Magic: The G
CURRENT ORACLE: At the beginning of your first main phase, remove all flood counters from this enchantment. If no counters were removed this way, put a flood counter on this enchantment and draw a card. Otherwise, add {C}{G}{U}.
```
FILE borderline.txt
[card]
name=Bounty of the Luxa
auto=@each my firstmain restriction{compare(hascntflood)~equalto~0}:name(Put counter and draw) name(Put counter and draw) counter(0/0,1,flood) && draw:1 controller
auto=@each my firstmain restriction{compare(hascntflood)~morethan~0}:name(Remove all counters and add mana) name(Remove all counters and add mana) removeallcounters(0/0,1,flood) && add{1}{G}{U} controller
text=At the beginning of your precombat main phase, remove all flood counters from Bounty of the Luxa. If no counters were removed this way, put a flood counter on Bounty of the Luxa and draw a card. Otherwise, add {1}{G}{U}.
mana={2}{G}{U}
type=Enchantment
[/card]
```

## Brightcap Badger // Fungus Frolic
JUDGE REASON: TEXT B includes an additional effect in the second half of the card ('Create two 1/1 green Saproling creature tokens') that is completely absent from TEXT A. This represents a significant change in a)
CURRENT ORACLE: Each Fungus and Saproling you control has "{T}: Add {G}." -- At the beginning of your end step, create a 1/1 green Saproling creature token. -- Create two 1/1 green Saproling creature tokens. (Then exile this card. You may cast the creature later from exile.)
```
FILE borderline.txt
[card]
name=Brightcap Badger // Fungus Frolic
abilities=adventure
other={2}{G} name(Adventure)
auto=alternative _SAPROLINGTOKEN_*2
auto=lord(*[Fungus;Saproling]|mybattlefield) {T}:Add{G}
auto=@each my end:_SAPROLINGTOKEN_
text=Each Fungus and Saproling you control has "{T}: Add {G}." -- At the beginning of your end step, create a 1/1 green Saproling creature token.
mana={3}{G}
type=Creature
subtype=Badger Druid
power=3
toughness=4
[/card]
```

## Wood Elemental
JUDGE REASON: The two texts define the card's power and toughness differently. TEXT A states the Elemental enters with +1/+1 counters for each Forest sacrificed, meaning the power/toughness increase is permanent (a
CURRENT ORACLE: As this creature enters, sacrifice any number of untapped Forests. -- Wood Elemental's power and toughness are each equal to the number of Forests sacrificed as it entered.
```
FILE mtg.txt
[card]
name=Wood Elemental
auto=may target(<anyamount>forest[-tapped]|mybattlefield) sacrifice && counter(1/1) all(this)
text=As Wood Elemental enters, sacrifice any number of untapped Forests. Wood Elemental enters with a +1/+1 counter for each Forest sacrificed this way.
mana={3}{G}
type=Creature
subtype=Elemental
power=0
toughness=0
[/card]
```

## Charred Graverobber
JUDGE REASON: The escape cost in TEXT A is {3}{B}, while the escape cost in TEXT B is {3}{B}{B}. This is a change in the mana cost required to activate the ability.
CURRENT ORACLE: When this creature enters, return target outlaw card from your graveyard to your hand. -- Escape—{3}{B}{B}, Exile four other cards from your graveyard. (You may cast this card from your graveyard for its escape cost.) -- This creature escapes with a +1/+1 counter on it.
```
FILE borderline.txt
[card]
name=Charred Graverobber
auto=target(Assassin,Mercenary,Pirate,Rogue,Warlock|mygraveyard) moveto(hand)
retrace={3}{B}{B}{E(other *|myGraveyard)}{E(other *|myGraveyard)}{E(other *|myGraveyard)}{E(other *|myGraveyard)} name(Escape)
auto=if paid(retrace) then counter(1/1)
text=When Charred Graverobber enters, return target outlaw card from your graveyard to your hand. -- Escape-{3}{B}, Exile four other cards from your graveyard. (You may cast this card from your graveyard for its escape cost.) -- Charred Graverobber escapes with a +1/+1 counter on it.
mana={2}{B}
type=Creature
subtype=Skeleton Mercenary
power=3
toughness=1
[/card]
```

## Flaccify
JUDGE REASON: The mana cost required to counter the spell has changed. TEXT A requires {3} (three generic mana), while TEXT B requires {3}{½} (three generic mana plus one colorless mana, represented by the half-mu/
CURRENT ORACLE: Counter target spell unless its controller pays {3}{½}.
```
FILE borderline.txt
[card]
name=Flaccify
target=*|stack
auto=transforms((,newability[pay[[{3}]] name(pay 3 mana) donothing?fizzle])) oneshot
text=Counter target spell unless its controller pays {3}.
mana={2}{U}
type=Instant
[/card]
```

## Nullpriest of Oblivion
JUDGE REASON: The Kicker cost has changed from {4}{B} in TEXT A to {3}{B} in TEXT B. This is a change in the mana cost required to activate the ability, which constitutes a functional difference.
CURRENT ORACLE: Kicker {3}{B} (You may pay an additional {3}{B} as you cast this spell.) -- Lifelink -- Menace (This creature can't be blocked except by two or more creatures.) -- When this creature enters, if it was kicked, return target creature card from your graveyard to the battlefield.
```
FILE borderline.txt
[card]
name=Nullpriest of Oblivion
abilities=menace,lifelink
kicker={3}{B}
auto=if paid(kicker) then moveTo(myBattlefield) target(creature|myGraveyard)
text=Kicker {4}{B} -- Menace, lifelink -- When Nullpriest of Oblivion enters, if it was kicked, return target creature card from your graveyard to the battlefield.
mana={1}{B}
type=Creature
subtype=Vampire Cleric
power=2
toughness=1
[/card]
```

## Old-Growth Troll
JUDGE REASON: The functional difference lies in the state of the created token. TEXT B explicitly states the token is created **tapped** ('Create a tapped 4/4...'), whereas TEXT A omits this condition ('Create a 4/
CURRENT ORACLE: Trample -- When Old-Growth Troll dies, if it was a creature, return it to the battlefield. It's an Aura enchantment with enchant Forest you control and "Enchanted Forest has '{T}: Add {G}{G}' and '{1}, {T}, Sacrifice this land: Create a tapped 4/4 green Troll Warrior creature token with trample.'"
```
FILE borderline.txt
[card]
name=Old-Growth Troll
abilities=trample
auto=this(cantargetcard(*[creature]) transforms((,newability[_DIES_name(Return as enchantment) target(Old-Growth Troll[fresh]|mygraveyard) activate castcard(alternative)]))
auto=alternative name(Enchant Forest) name(Enchant Forest) target(land[forest]|mybattlefield) transforms((,newability[counter(0/0.1.TrollEffect)],newability[this(counter{0/0.1.TrollEffect}>0) {T}:add{G}{G}],newability[this(counter{0/0.1.TrollEffect}>0) {1}{T}{S}:token(Troll^Creature Troll Warrior^4/4^green^trample)])) forever
auto=alternative transforms((removetypes,newability[becomes(enchantment aura)])) forever
auto=alternative transforms((,newability[_DIES_name(Terminate effect) target(forest[counter{0/0.1.TrollEffect}]|mybattlefield) counter(0/0.-1.TrollEffect)]))
auto=alternative transforms((,newability[@movedTo(forest[counter{0/0.1.TrollEffect}]|mygraveyard) from(mybattlefield):name(Terminate effect) target(Old-Growth Troll[aura]|mybattlefield) moveto(mygraveyard)]))
text=Trample -- When Old-Growth Troll dies, if it was a creature, return it to the battlefield. it's an Aura enchantment with enchant Forest you control and Enchanted Forest has "{T}: Add {G}{G}" and "{1}, {T}, Sacrifice this land: Create a 4/4 green Troll Warrior creature token with trample."
mana={G}{G}{G}
type=Creature
subtype=Troll Warrior
power=4
toughness=4
[/card]
```

## Soulhunter Rakshasa
JUDGE REASON: The two texts describe fundamentally different mechanics and outcomes. TEXT A specifies a fixed cost of 5 damage to a target opponent upon entering. TEXT B introduces a conditional trigger (only if 'c
CURRENT ORACLE: This creature can't block. -- When this creature enters, if you cast it from your hand, it deals 1 damage to target opponent for each Swamp you control.
```
FILE borderline.txt
[card]
name=Soulhunter Rakshasa
abilities=cantblock
auto=damage:5 target(opponent)
text=Soulhunter Rakshasa can't block. -- When Soulhunter Rakshasa enters, it deals 5 damage to target opponent.
mana={3}{B}{B}
type=Creature
subtype=Demon
power=5
toughness=5
[/card]
```

## Garruk, Wrath of the Wilds
JUDGE REASON: The loyalty cost for the ultimate ability (the -X ability) has changed. TEXT A specifies a cost of -5, while TEXT B specifies a cost of -6. This is a functional change to the card's cost structure.
CURRENT ORACLE: +1: Choose a creature card in your hand. It perpetually gets +1/+1 and perpetually gains "This spell costs {1} less to cast." -- −1: Draft a card from Garruk, Wrath of the Wilds's spellbook and put it onto the battlefield. -- −6: Until end of turn, creatures you control get +3/+3 and gain trample.
```
FILE planeswalkers.txt
[card]
name=Garruk, Wrath of the Wilds
auto=counter(0/0,3,loyalty)
auto={C(0/0,1,Loyalty)}:name(+1: Creature perpetually gets 1/1) target(creature|myhand) transforms((,newability[counter(1/1.1.PerpetualPT) notrg],newability[changecost(colorless:-1) forcedalive])) forever
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Mosscoat Goriak) conjure cards(Mosscoat Goriak) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Sylvan Brushstrider) conjure cards(Sylvan Brushstrider) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Murasa Rootgrazer) conjure cards(Murasa Rootgrazer) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Dire Wolf Prowler) conjure cards(Dire Wolf Prowler) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Ferocious Pup) conjure cards(Ferocious Pup) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Pestilent Wolf) conjure cards(Pestilent Wolf) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Garruk's Uprising) conjure cards(Garruk's Uprising) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Dawntreader Elk) conjure cards(Dawntreader Elk) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Nessian Hornbeetle) conjure cards(Nessian Hornbeetle) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Territorial Scythecat) conjure cards(Territorial Scythecat) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Trufflesnout) conjure cards(Trufflesnout) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Wary Okapi) conjure cards(Wary Okapi) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Scurrid Colony) conjure cards(Scurrid Colony) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Barkhide Troll) conjure cards(Barkhide Troll) zone(mybattlefield)
auto={C(0/0,-1,Loyalty)}:name(-1: Draft Underdark Basilisk) conjure cards(Underdark Basilisk) zone(mybattlefield)
auto={C(0/0,-5,Loyalty)}:name(-3: Your creatures get 3/3) all(creature|myBattlefield) transforms((,newability[3/3],trample)) ueot
text=+1: Choose a creature card in your hand. it perpetually gets +1/+1 and perpetually gains "This spell costs {1} less to cast." -- -1: Draft a card from Garruk, Wrath of the Wild's spellbook and put it onto the battlefield. -- -5: Until end of turn, creatures you control get +3/+3 and gain trample.
mana={2}{G}{G}
type=Legendary Planeswalker
subtype=Garruk
[/card]
```

## Liliana of the Veil
JUDGE REASON: The -6 ability has a fundamental functional change. TEXT A forces the opponent to sacrifice exactly half (rounded up) of their permanents, which is a fixed quantity regardless of the total number of a
CURRENT ORACLE: +1: Each player discards a card. -- −2: Target player sacrifices a creature. -- −6: Separate all permanents target player controls into two piles. That player sacrifices all permanents in the pile of their choice.
```
FILE planeswalkers.txt
[card]
name=Liliana of the Veil
auto=counter(0/0,3,loyalty)
auto={C(0/0,1,Loyalty)}:name(+1: discard) transforms((,newability[ability$!name(discard) notatarget(*|myhand) reject!$ controller],newability[ability$!name(discard) notatarget(*|myhand) reject!$ opponent])) ueot
auto={C(0/0,-2,Loyalty)}:name(-2: sacrifice creature) transforms((,newability[target(player) ability$!name(sacrifice creature) notatarget(creature|myBattlefield) sacrifice!$ targetedplayer])) ueot
auto={C(0/0,-6,Loyalty)}:name(-6: sacrifice half) transforms((,newability[target(player) ability$!name(sacrifice half permanents) notatarget(<halfuptype:*:myBattlefield>*|myBattlefield) sacrifice!$ targetedplayer])) ueot
text=+1: each player discards a card -- -2: target player sacrifices a creature -- -6: an opponent sacrifices half the permanents they control rounded up
mana={1}{B}{B}
type=Legendary Planeswalker
subtype=Liliana
[/card]
```

## Tibalt, Wicked Tormentor
JUDGE REASON: The damage amount in the second ability changed from 3 in TEXT A to 4 in TEXT B. This is a functional change in the number of damage dealt.
CURRENT ORACLE: +1: Add {R}{R}. Draft a card from Tibalt, Wicked Tormentor's spellbook, then exile it. Until end of turn, you may cast that card. -- +1: Tibalt, Wicked Tormentor deals 4 damage to target creature or planeswalker unless its controller has Tibalt deal 4 damage to them. If they do, you may discard a card. If you do, draw a card. -- −X: Create X 1/1 red Devil creature tokens with "When this creature dies, it deals 1 damage to any target."
```
FILE planeswalkers.txt
[card]
name=Tibalt, Wicked Tormentor
auto=counter(0/0,3,Loyalty)
auto={C(0/0,1,Loyalty)}:name(+1: Draft Chained Brute) conjure cards(Chained Brute) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Charmbreaker Devils) conjure cards(Charmbreaker Devils) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Festival Crasher) conjure cards(Festival Crasher) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Forge Devil) conjure cards(Forge Devil) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Frenzied Devils) conjure cards(Frenzied Devils) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Havoc Jester) conjure cards(Havoc Jester) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Hellrider) conjure cards(Hellrider) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Hobblefiend) conjure cards(Hobblefiend) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Pitchburn Devils) conjure cards(Pitchburn Devils) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Sin Prodder) conjure cards(Sin Prodder) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Spiteful Prankster) conjure cards(Spiteful Prankster) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Tibalt's rager) conjure cards(Tibalt's rager) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Torch Fiend) conjure cards(Torch Fiend) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Brimstone Vandal) conjure cards(Brimstone Vandal) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Draft Devil's Play) conjure cards(Devil's Play) zone(myexile) and!( transforms((,newability[canplayfromexile],newability[add{R}{R}])) ueot )!
auto={C(0/0,1,Loyalty)}:name(+1: Damage opponent creature or planeswalker) target(*[creature;planeswalker]|opponentbattlefield) transforms((,newability[choice name(Damage target) damage:3],newability[choice name(Damage controller) damage:3 controller && ability$!may name(Discard and draw) name(Discard and draw) target(*|myhand) reject and!( draw:1 controller )! !$ opponent])) oneshot
auto={C(0/0,1,Loyalty)}:name(+1: Damage your creature or planeswalker) target(*[creature;planeswalker]|mybattlefield) transforms((,newability[choice name(Damage target) damage:3],newability[choice name(Damage controller) damage:3 controller && ability$!may name(Discard and draw) name(Discard and draw) target(*|myhand) reject and!( draw:1 controller )! !$ controller])) oneshot
auto={C(0/0,-1,Loyalty)}:name(-1: Create 1 devil) token(Devil,Creature Devil,1/1,red) and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-2,Loyalty)}:name(-2: Create 2 devils) token(Devil,Creature Devil,1/1,red)*2 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-3,Loyalty)}:name(-3: Create 3 devils) token(Devil,Creature Devil,1/1,red)*3 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-4,Loyalty)}:name(-4: Create 4 devils) token(Devil,Creature Devil,1/1,red)*4 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-5,Loyalty)}:name(-5: Create 5 devils) token(Devil,Creature Devil,1/1,red)*5 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-6,Loyalty)}:name(-6: Create 6 devils) token(Devil,Creature Devil,1/1,red)*6 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-7,Loyalty)}:name(-7: Create 7 devils) token(Devil,Creature Devil,1/1,red)*7 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-8,Loyalty)}:name(-8: Create 8 devils) token(Devil,Creature Devil,1/1,red)*8 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-9,Loyalty)}:name(-9: Create 9 devils) token(Devil,Creature Devil,1/1,red)*9 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-10,Loyalty)}:name(-10: Create 10 devils) token(Devil,Creature Devil,1/1,red)*10 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-11,Loyalty)}:name(-11: Create 11 devils) token(Devil,Creature Devil,1/1,red)*11 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-12,Loyalty)}:name(-12: Create 12 devils) token(Devil,Creature Devil,1/1,red)*12 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-13,Loyalty)}:name(-13: Create 13 devils) token(Devil,Creature Devil,1/1,red)*13 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-14,Loyalty)}:name(-14: Create 14 devils) token(Devil,Creature Devil,1/1,red)*14 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-15,Loyalty)}:name(-15: Create 15 devils) token(Devil,Creature Devil,1/1,red)*15 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-16,Loyalty)}:name(-16: Create 16 devils) token(Devil,Creature Devil,1/1,red)*16 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-17,Loyalty)}:name(-17: Create 17 devils) token(Devil,Creature Devil,1/1,red)*17 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-18,Loyalty)}:name(-18: Create 18 devils) token(Devil,Creature Devil,1/1,red)*18 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-19,Loyalty)}:name(-19: Create 19 devils) token(Devil,Creature Devil,1/1,red)*19 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
auto={C(0/0,-20,Loyalty)}:name(-20: Create 20 devils) token(Devil,Creature Devil,1/1,red)*20 and!( transforms((,newability[@movedto(this|graveyard) from(battlefield):name(Deal 1 damage) target(anytarget)])) forever )!
text=+1: Add {R}{R}. Draft a card from Tibalt, Wicked Tormenter's spellbook, then exile it. Until end of turn, you may cast that card. -- +1: Tibalt, Wicked Tormenter deals 3 damage to target creature or planeswalker unless its controller has Tibalt deal 3 damage to them. If they do, you may discard a card. If you do, draw card. -- -X: Create X 1/1 red Devil creature tokens with "When this creature dies, it deals 1 damage to any target."
mana={3}{R}{R}
type=Legendary Planeswalker
subtype=Tibalt
[/card]
```

## Cheap Ass
JUDGE REASON: The cost reduction amount is different. TEXT A implies a reduction of the entire cost (making spells costless), whereas TEXT B specifies a reduction of only {½} (half a mana). This is a significant,量化
CURRENT ORACLE: Spells you cast cost {½} less to cast.
```
FILE unsupported.txt
[card]
name=Cheap Ass
text=Spells you play costless to play.
mana={1}{W}
type=Creature
subtype=Donkey Townsfolk
power=1
toughness=3{1/2}
[/card]
```

## Old Fogey
JUDGE REASON: The cumulative upkeep cost has changed. TEXT A specifies a cost of {1}, while TEXT B specifies a cost of {G}{G}. This is a functional difference in the mana cost required to keep the creature in play.
CURRENT ORACLE: Phasing, cumulative upkeep {1}, echo {G}{G}, fading 3, bands with other Dinosaurs, protection from Homarids, snow-covered plainswalk, flanking, rampage 2
```
FILE unsupported.txt
[card]
name=Old Fogey
text=Phasing, cumulative upkeep {1}, echo, fading 3, bands with other Dinosaurs, protection from Homarids, snow-covered plainswalk, flanking, rampage 2
mana={G}{G}
type=Summon
subtype=Dinosaur
[/card]
```

