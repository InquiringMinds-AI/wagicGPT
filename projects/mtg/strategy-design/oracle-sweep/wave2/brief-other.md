# Wave-2 class: other — 27 cards

## Akoum Boulderfoot
JUDGE REASON: TEXT A includes the keyword ability 'Defender', which prevents the creature from attacking. TEXT B does not include 'Defender'. This is a functional difference in the card's abilities and behavior.
CURRENT ORACLE: When this creature enters, it deals 1 damage to any target.
```
FILE mtg.txt
[card]
name=Akoum Boulderfoot
auto=damage:1 target(anytarget)
text=Defender -- When Akoum Boulderfoot enters, it deals 1 damage to any target.
mana={4}{R}{R}
type=Creature
subtype=Giant Warrior
power=4
toughness=5
[/card]
```

## Awakening Zone
JUDGE REASON: The card's function has changed in two significant ways: 1) The ability is now optional ('you may create') in TEXT B, whereas TEXT A implies it is mandatory ('put... onto the battlefield'). 2) The man
CURRENT ORACLE: At the beginning of your upkeep, you may create a 0/1 colorless Eldrazi Spawn creature token. It has "Sacrifice this token: Add {C}."
```
FILE mtg.txt
[card]
name=Awakening Zone
auto=@each my upkeep:_ELDRAZISPAWN_
text=At the beginning of your upkeep, put a 0/1 colorless Eldrazi Spawn creature token onto the battlefield. It has "Sacrifice this creature: Add {1}."
mana={2}{G}
type=Enchantment
[/card]
```

## Enatu Golem
JUDGE REASON: The life gain amount changed from 3 in TEXT A to 4 in TEXT B.
CURRENT ORACLE: When this creature dies, you gain 4 life.
```
FILE mtg.txt
[card]
name=Enatu Golem
auto=_DIES_life:3
text=When Enatu Golem goes to the graveyard from the battlefield, you gain 3 life.
mana={6}
type=Artifact Creature
subtype=Golem
power=3
toughness=5
[/card]
```

## Land Cap
JUDGE REASON: The mana symbols produced by the land are different. TEXT A specifies adding {W} or {B} (White or Black), while TEXT B specifies adding {W} or {U} (White or Blue). This is a functional change in the '
CURRENT ORACLE: This land doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from this land. -- {T}: Add {W} or {U}. Put a depletion counter on this land.
```
FILE mtg.txt
[card]
name=Land Cap
auto=@each my upkeep:counter(0/0,-1,Depletion)
auto={T}:Add{W} and!( counter(0/0,1,Depletion) )!
auto={T}:Add{U} and!( counter(0/0,1,Depletion) )!
auto=this(counter{0/0.1.Depletion}>=1) doesnotuntap
text=Land Cap doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from Land Cap. -- {T}: Add {W} or {B}. Put a depletion counter on Land Cap.
type=Land
[/card]
```

## Lorescale Coatl
JUDGE REASON: The functional difference lies in the choice of action. TEXT A states 'you may put', granting the player the option to decline placing the counter. TEXT B states 'put', which is a mandatory effect. In
CURRENT ORACLE: Whenever you draw a card, put a +1/+1 counter on this creature.
```
FILE mtg.txt
[card]
name=Lorescale Coatl
auto=@drawof(player):may counter(1/1)
text=Whenever you draw a card, you may put a +1/+1 counter on Lorescale Coatl.
mana={1}{G}{U}
type=Creature
subtype=Snake
power=2
toughness=2
[/card]
```

## Merfolk Skyscout
JUDGE REASON: The two texts describe completely different cards with entirely different abilities. TEXT A describes a card named 'Merchant Ship' with abilities related to attacking conditions, gaining life, and a '
CURRENT ORACLE: Flying -- Whenever this creature attacks or blocks, untap target permanent.
```
FILE mtg.txt
[card]
name=Merfolk Skyscout
abilities=flying
auto=_ATTACKING_untap target(*)
auto=@combat(blocking) source(this):untap target(*)
text=Merchant Ship can't attack unless defending player controls an Island. -- Whenever Merchant Ship attacks and isn't blocked, you gain 2 life. -- When you control no Islands, sacrifice Merchant Ship.
mana={2}{U}{U}
type=Creature
subtype=Merfolk Scout
power=2
toughness=3
[/card]
```

## Odric, Lunarch Marshal
JUDGE REASON: The list of abilities granted by the card has changed. TEXT A includes 'lying' (likely a typo for 'flying' or a distinct error) and omits 'menace' and 'skulk'. TEXT B correctly lists 'flying', 'menace
CURRENT ORACLE: At the beginning of each combat, creatures you control gain first strike until end of turn if a creature you control has first strike. The same is true for flying, deathtouch, double strike, haste, hexproof, indestructible, lifelink, menace, reach, skulk, trample, and vigilance.
```
FILE mtg.txt
[card]
name=Odric, Lunarch Marshal
auto=@each combatbegins restriction{type(*[menace]|mybattlefield)~morethan~0}:all(creature|mybattlefield) menace ueot
auto=@each combatbegins restriction{type(*[first strike]|mybattlefield)~morethan~0}:all(creature|mybattlefield) first strike ueot
auto=@each combatbegins restriction{type(*[flying]|mybattlefield)~morethan~0}:all(creature|mybattlefield) flying ueot
auto=@each combatbegins restriction{type(*[deathtouch]|mybattlefield)~morethan~0}:all(creature|mybattlefield) deathtouch ueot
auto=@each combatbegins restriction{type(*[double strike]|mybattlefield)~morethan~0}:all(creature|mybattlefield) double strike ueot
auto=@each combatbegins restriction{type(*[haste]|mybattlefield)~morethan~0}:all(creature|mybattlefield) haste ueot
auto=@each combatbegins restriction{type(*[hexproof]|mybattlefield)~morethan~0}:all(creature|mybattlefield) hexproof ueot
auto=@each combatbegins restriction{type(*[indestructible]|mybattlefield)~morethan~0}:all(creature|mybattlefield) indestructible ueot
auto=@each combatbegins restriction{type(*[lifelink]|mybattlefield)~morethan~0}:all(creature|mybattlefield) lifelink ueot
auto=@each combatbegins restriction{type(*[reach]|mybattlefield)~morethan~0}:all(creature|mybattlefield) reach ueot
auto=@each combatbegins restriction{type(*[trample]|mybattlefield)~morethan~0}:all(creature|mybattlefield) trample ueot
auto=@each combatbegins restriction{type(*[vigilance]|mybattlefield)~morethan~0}:all(creature|mybattlefield) vigilance ueot
text=at the beginning of each combat, creatures you control gain first strike until end of turn if you control a creature with first strike, the same is true for lying, deathtouch, double strike, haste, hexproof, indestructible, lifelink, reach, trample, and vigilance
mana={3}{W}
type=Legendary Creature
subtype=Human Soldier
power=3
toughness=3
[/card]
```

## River Delta
JUDGE REASON: The mana symbols produced by the land are different. TEXT A specifies adding {W} (White) or {B} (Black), while TEXT B specifies adding {U} (Blue) or {B} (Black). This is a functional change in the set
CURRENT ORACLE: This land doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from this land. -- {T}: Add {U} or {B}. Put a depletion counter on this land.
```
FILE mtg.txt
[card]
name=River Delta
auto=@each my upkeep:counter(0/0,-1,Depletion)
auto={T}:Add{U} and!( counter(0/0,1,Depletion) )!
auto={T}:Add{B} and!( counter(0/0,1,Depletion) )!
auto=this(counter{0/0.1.Depletion}>=1) doesnotuntap
text=River Delta doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from River Delta. -- {T}: Add {W} or {B}. Put a depletion counter on River Delta.
type=Land
[/card]
```

## Soldevi Excavations
JUDGE REASON: The mana production ability has changed. TEXT A produces {1}{U} (one colorless and one blue mana), while TEXT B produces {C}{U} (one colorless and one blue mana). While {1} and {C} are functionally in
CURRENT ORACLE: If this land would enter, sacrifice an untapped Island instead. If you do, put this land onto the battlefield. If you don't, put it into its owner's graveyard. -- {T}: Add {C}{U}. -- {1}, {T}: Scry 1.
```
FILE mtg.txt
[card]
name=Soldevi Excavations
aicode=activate transforms((,newability[all(*[zpos<=1]|mylibrary) transforms((,newability[may name(Put on bottom of library) moveto(myreveal) and!( bottomoflibrary )!])) oneshot])) oneshot
auto={1}{T}:name(Scry 1) _SCRY1_
auto=if type(island[-tapped]|mybattlefield)~morethan~0 then sacrifice notaTarget(island[-tapped]|mybattlefield) oneshot else sacrifice
auto={T}:add{1}{U}
text=If Soldevi Excavations would enter the battlefield, sacrifice an untapped Island instead. If you do, put Soldevi Excavations onto the battlefield. If you don't, put it into its owner's graveyard. -- {T}: Add {1}{U}. -- {1}, {T}: Scry 1.
type=Land
[/card]
```

## Suncrusher
JUDGE REASON: The two texts describe completely different abilities and effects. TEXT A describes an ability that removes two counters from a different card ('Sawtooth Thresher') to give it +4/+4. TEXT B describes:
CURRENT ORACLE: Sunburst (This creature enters with a +1/+1 counter on it for each color of mana spent to cast it.) -- {4}, {T}, Remove a +1/+1 counter from this creature: Destroy target creature. -- {2}, Remove a +1/+1 counter from this creature: Return this creature to its owner's hand.
```
FILE mtg.txt
[card]
name=Suncrusher
abilities=sunburst
auto=counter(1/1,sunburst)
auto={C(1/1,-1)}{4}{T}:destroy target(creature)
auto={C(1/1,-1)}{2}:moveTo(hand)
text=Sunburst (This enters with a +1/+1 counter on it for each color of mana spent to cast it.) -- Remove two +1/+1 counters from Sawtooth Thresher: Sawtooth Thresher gets +4/+4 until end of turn.
mana={9}
type=Artifact Creature
subtype=Construct
power=3
toughness=3
[/card]
```

## Blood
JUDGE REASON: The two texts describe completely different card effects. TEXT A describes an activated ability of an artifact that requires discarding a card and sacrificing itself to draw a card. TEXT B describes a
CURRENT ORACLE: Target creature you control deals damage equal to its power to any target. -- Fuse (You may cast one or both halves of this card from your hand.)
```
FILE borderline.txt
[card]
name=Blood
auto={1}{T}{D(*|myhand)}{S}:name(Draw a card) draw:1 controller
text={1}, {T}, Discard a card, Sacrifice this artifact: Draw a card.
type=Artifact
subtype=Blood
[/card]
```

## Chicken Egg
JUDGE REASON: The token created in TEXT A is a 'Giant Chicken' that counts as a 'Chicken', whereas the token in TEXT B is a 'Giant Bird' creature. This is a change in the token's name and creature type, which can a
CURRENT ORACLE: At the beginning of your upkeep, roll a six-sided die. If you roll a 6, sacrifice this creature and create a 4/4 red Giant Bird creature token.
```
FILE borderline.txt
[card]
name=Chicken Egg
auto=@each my upkeep:rolld6 6 winability donothing winabilityend rolld6end
auto=@dierolled(this) result(6) from(controller):all(this) sacrifice && token(Giant Chicken,Creature Giant Chicken,4/4,red) myupkeeponly
text=During your upkeep, roll a six-sided die. On a 6, sacrifice Chicken Egg and put a Giant Chicken token into play. Treat this token as a 4/4 red creature that counts as a Chicken.
mana={1}{R}
type=Creature
subtype=Egg
power=0
toughness=1
[/card]
```

## Curse of the Fire Penguin
JUDGE REASON: The two texts describe completely different card types and mechanics. TEXT A describes a creature with Trample and a self-recurring ability. TEXT B describes an Aura enchantment with a static effect. 
CURRENT ORACLE: Enchant creature -- This enchantment consumes and confuses enchanted creature.
```
FILE borderline.txt
[card]
name=Curse of the Fire Penguin
abilities=inplaydeath
text=Trample -- When this creature is put into a graveyard from play, return Curse of the Fire Penguin from your graveyard to play.
mana={4}{R}{R}
type=Creature
subtype=Penguin
power=6
toughness=5
[/card]
```

## Day
JUDGE REASON: The two texts describe completely different game effects. TEXT A describes the mechanics of the 'Day and Night' cycle (transforming permanents and tracking the state of the game). TEXT B describes a +
CURRENT ORACLE: Creatures target player controls get +1/+1 until end of turn.
```
FILE borderline.txt
[card]
name=Day
abilities=shroud,indestructible,doublefacedeath,nofizzle
backside=Night
auto=@each end:name(Check if day or night) moveto(opponentbattlefield) and!( if thisturn(*|opponentstack)~equalto~0 then if type(*[nonight]|battlefield)~equalto~0 then name(It becomes night next turn) name(It becomes night next turn) name(It becomes night next turn) phaseaction[untap once sourceinplay] flip(backside) )!
auto=lord(*[nightbound]|battlefield) flip(backside)
text=(If it becomes day or night or if a daybound permanent enters, track day/night for the rest of the game.) -- As it becomes day, transform all nightbound permanents. -- If a player casts no spells during their own turn, it becomes night next turn.
type=Emblem
[/card]
```

## Fast // Furious
JUDGE REASON: The two texts describe completely different card effects. TEXT A describes a card that forces a discard, draws two cards, and deals damage to creatures without flying. TEXT B describes a split card (F
CURRENT ORACLE: Target creature gains haste until end of turn. It can't be blocked this turn except by Vehicles or by creatures with haste. -- Fuse (You may cast one or both halves of this card from your hand.) -- Target creature gets +3/+0 until end of turn. -- Fuse (You may cast one or both halves of this card from your hand.)
```
FILE borderline.txt
[card]
name=Fast // Furious
other={3}{R}{R} name(Furious)
otherrestriction=can play sorcery
auto=alternative all(creature[-flying]|battlefield) damage:3
auto=ifnot paid(alternative) then target(*|myHand) reject and!( transforms((,newability[name(Draw 2 cards) draw:2 controller])) oneshot )!
text=Discard a card, then draw two cards. -- Furious deals 3 damage to each creature without flying.
mana={2}{R}
type=Instant
[/card]
```

## Mana Screw
JUDGE REASON: The mana output changed from {2} (two mana of any color) in TEXT A to {C}{C} (two colorless mana) in TEXT B. This is a functional change in the amount and type of resource produced.
CURRENT ORACLE: {1}: Flip a coin. If you win the flip, add {C}{C}. Activate only as an instant.
```
FILE borderline.txt
[card]
name=Mana Screw
auto={1}:flipacoin winability add{2} winabilityend flipend
text={1}: Flip a coin. If you win the flip, add {2}. Play this ability only any time you could play an instant.
mana={1}
type=Artifact
[/card]
```

## Mire's Grasp
JUDGE REASON: The numerical values of the effect have changed. TEXT A grants -2/-2, while TEXT B grants -3/-3. This is a functional change in the magnitude of the stat modification.
CURRENT ORACLE: Enchant creature -- Enchanted creature gets -3/-3.
```
FILE borderline.txt
[card]
name=Mire's Grasp
target=creature
auto=-3/-3
text=Enchant creature -- Enchanted creature gets -2/-2.
mana={1}{B}
type=Enchantment
subtype=Aura
[/card]
```

## Night
JUDGE REASON: The two texts describe completely different card effects. TEXT A describes the mechanics of the 'Day/Night' cycle (transforming permanents, entering as nightbound, and the condition to become day). It
CURRENT ORACLE: Target creature gets -1/-1 until end of turn.
```
FILE borderline.txt
[card]
name=Night
abilities=shroud,indestructible,doublefacedeath,nofizzle
backside=Day
auto=@each end:name(Check if day or night) moveto(opponentbattlefield) and!( if thisturn(*|opponentstack)~morethan~1 then name(It becomes day next turn) name(It becomes day next turn) phaseaction[untap once sourceinplay] flip(backside) )!
auto=lord(*[daybound]|battlefield) flip(backside)
text=As it becomes night, transform all daybound permanents. -- Permanents enter the battlefield nightbound. -- If a player casts at least two spells during their own turn, it becomes day next turn.
type=Emblem
[/card]
```

## Pick Your Poison
JUDGE REASON: The two texts describe completely different card effects. TEXT A describes a 'Choose one' effect where opponents sacrifice artifacts, enchantments, or flying creatures. TEXT B describes a 'Choose any'
CURRENT ORACLE: Choose any number of modes that total exactly 4. You may choose the same mode more than once. -- [1] Put a +1/+1 counter on target creature. -- [2] Create a 1/1 black Snake creature token with deathtouch. -- [4] All creatures get -2/-2 until end of turn.
```
FILE borderline.txt
[card]
name=Pick Your Poison
auto=choice name(opponent sacrifices an artifact) ability$!name(Sacrifice an artifact) notaTarget(artifact|mybattlefield) sacrifice!$ opponent
auto=choice name(opponent sacrifices an enchantment) ability$!name(Sacrifice an enchantment) notaTarget(enchantment|mybattlefield) sacrifice!$ opponent
auto=choice name(opponent sacrifices a creature with flying) ability$!name(Sacrifice a creature with flying) notaTarget(creature[flying]|mybattlefield) sacrifice!$ opponent
text=Choose one - -- - Each opponent sacrifices an artifact. -- - Each opponent sacrifices an enchantment. -- - Each opponent sacrifices a creature with flying.
mana={G}
type=Sorcery
[/card]
```

## Red Herring
JUDGE REASON: The two texts describe completely different card abilities. TEXT A describes a creature with Haste and a sacrifice ability to draw a card. TEXT B describes an instant/sorcery (or similar) that swaps a
CURRENT ORACLE: {1}{U}: Exchange Red Herring from your hand with a permanent you control on the battlefield or a spell you control on the stack. If that permanent or spell was the target of a spell or ability, change that target to Red Herring.
```
FILE borderline.txt
[card]
name=Red Herring
abilities=haste,mustattack
auto={2}{S}:draw:1
text=Haste -- Red Herring attacks each combat if able. -- {2}, Sacrifice Red Herring: Draw a card.
mana={1}{R}
type=Artifact Creature
subtype=Clue Fish
power=2
toughness=2
[/card]
```

## Weaver of Currents
JUDGE REASON: The mana produced is different. TEXT A produces two colorless mana symbols that were historically represented as {1}{1} (two generic mana) in older templates, but the specific notation {1}{1} in Magic
CURRENT ORACLE: {T}: Add {C}{C}.
```
FILE borderline.txt
[card]
name=Weaver of Currents
auto={T}:Add{2}
text={T}: Add {1}{1}.
mana={1}{G}{U}
type=Creature
subtype=Snake Druid
power=2
toughness=2
[/card]
```

## Checks and Balances
JUDGE REASON: The core mechanic of the card has fundamentally changed. In TEXT A, the spell is countered only if **all** other players (excluding the caster and teammates) **agree** to discard a card. This implies:
CURRENT ORACLE: Cast this spell only if there are three or more players in the game. -- Whenever a player casts a spell, each of that player's opponents may discard a card. If they do, counter that spell.
```
FILE unsupported.txt
[card]
name=Checks and Balances
text=Whenever any spell is played, counter that spell if each player, other than the caster and his or her teammates, agrees to choose and discard a card. Those players must discard those cards after agreeing. -- Checks and Balances may be played only in a game with three or more players.
mana={2}{U}
type=Enchantment
[/card]
```

## Dark Sphere
JUDGE REASON: The two texts describe fundamentally different mechanical effects regarding damage mitigation. TEXT A states that the damage is **dealt to you instead** (implying the damage event still occurs, but is
CURRENT ORACLE: {T}, Sacrifice this artifact: The next time a source of your choice would deal damage to you this turn, prevent half that damage, rounded down.
```
FILE unsupported.txt
[card]
name=Dark Sphere
text={T}, Sacrifice Dark Sphere: The next time a source of your choice would deal damage to you this turn, it deals half that damage, rounded up, to you instead.
mana={0}
type=Artifact
[/card]
```

## Merseine
JUDGE REASON: The activation condition for the ability to remove a net counter has changed. TEXT A allows 'Any player' to activate the ability (provided they control the enchanted creature), whereas TEXT B restrict
CURRENT ORACLE: Enchant creature -- This Aura enters with three net counters on it. -- Enchanted creature doesn't untap during its controller's untap step if this Aura has a net counter on it. -- Pay enchanted creature's mana cost: Remove a net counter from this Aura. Only the controller of the enchanted creature may activate this ability.
```
FILE unsupported.txt
[card]
name=Merseine
text=Enchant creature  -- Merseine enters the battlefield with three net counters on it.  -- Enchanted creature doesn't untap during its controller's untap step if Merseine has a net counter on it. -- Pay enchanted creature's mana cost: Remove a net counter from Merseine. Any player may activate this ability, but only if he or she controls the enchanted creature.
mana={2}{U}{U}
type=Enchantment
subtype=Aura
[/card]
```

## Personal Incarnation
JUDGE REASON: The activation condition for the ability has changed. TEXT A states 'Any player may activate this ability, but only if he or she owns Personal Incarnation,' which implies that a player who does not *n
CURRENT ORACLE: {0}: The next 1 damage that would be dealt to this creature this turn is dealt to its owner instead. Only this creatures owner may activate this ability. -- When this creature dies, its owner loses half their life, rounded up.
```
FILE unsupported.txt
[card]
name=Personal Incarnation
text={0}: The next 1 damage that would be dealt to Personal Incarnation this turn is dealt to its owner instead. Any player may activate this ability, but only if he or she owns Personal Incarnation. -- When Personal Incarnation dies, its owner loses half his or her life, rounded up.
mana={3}{W}{W}{W}
type=Creature
subtype=Avatar Incarnation
power=6
toughness=6
[/card]
```

## Thran Turbine
JUDGE REASON: The mana production amount and type have changed. TEXT A allows the player to choose between adding {1} (one colorless mana) or {2} (two colorless mana). TEXT B only allows adding {C}{C} (two colorous
CURRENT ORACLE: At the beginning of your upkeep, you may add {C}{C}. This mana can't be spent to cast spells.
```
FILE unsupported.txt
[card]
name=Thran Turbine
text=At the beginning of your upkeep, you may add {1} or {2} to your mana pool. You can't spend this mana to cast spells.
mana={1}
type=Artifact
[/card]
```

## Wall of Caltrops
JUDGE REASON: The condition for gaining banding has changed. TEXT A requires 'no non-Wall creatures are blocking that creature' (implying Wall of Caltrops is the only blocker, or the only Wall blocker, but the phr'
CURRENT ORACLE: Defender (This creature can't attack.) -- Whenever this creature blocks a creature, if at least one other Wall creature is blocking that creature and no non-Wall creatures are blocking that creature, this creature gains banding until end of turn. (If any creatures with banding you control are blocking a creature, you divide that creature's combat damage, not its controller, among any of the creatures it's being blocked by.)
```
FILE unsupported.txt
[card]
name=Wall of Caltrops
abilities=defender
text=Defender (This creature can't attack.) -- Whenever Wall of Caltrops blocks a creature, if no non-Wall creatures are blocking that creature, Wall of Caltrops gains banding until end of turn. (If any creatures with banding you control are blocking a creature, you divide that creature's combat damage, not its controller, among any of the creatures it's being blocked by.)
mana={1}{W}
type=Creature
subtype=Wall
power=2
toughness=1
[/card]
```

