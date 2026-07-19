# Wave-2 class: scope — 35 cards

## Anafenza, the Foremost
JUDGE REASON: The replacement effect in TEXT B has a significantly broader scope than TEXT A. TEXT A only exiles creature cards moving to an opponent's graveyard from 'anywhere'. TEXT B adds two major functional扩 (
CURRENT ORACLE: Whenever Anafenza attacks, put a +1/+1 counter on another target tapped creature you control. -- If a nontoken creature an opponent owns would die or a creature card not on the battlefield would be put into an opponent's graveyard, exile that card instead.
```
FILE mtg.txt
[card]
name=Anafenza, the Foremost
abilities=oppgcreatureexiler
auto=_ATTACKING_counter(1/1) target(other creature[tapped]|mybattlefield)
text=When Anafenza, the Foremost attacks, put a +1/+1 counter on another target tapped creature you control. -- If a creature card would be put into an opponent's graveyard from anywhere, exile it instead.
mana={W}{B}{G}
type=Legendary Creature
subtype=Human Soldier
power=4
toughness=4
[/card]
```

## Beasts of Bogardan
JUDGE REASON: The functional scope of the buff condition has changed. TEXT A requires an opponent to control 'a white permanent', which includes token creatures. TEXT B specifically requires a 'nontoken white', exl
CURRENT ORACLE: Protection from red -- This creature gets +1/+1 as long as an opponent controls a nontoken white permanent.
```
FILE mtg.txt
[card]
name=Beasts of Bogardan
abilities=protection from red
auto=aslongas(*[white]|opponentBattlefield) 1/1
text=Protection from red -- Beasts of Bogardan gets +1/+1 as long as an opponent controls a white permanent.
mana={4}{R}
type=Creature
subtype=Beast
power=3
toughness=3
[/card]
```

## Concussive Bolt
JUDGE REASON: The scope of the target and the scope of the blocking restriction have changed. TEXT A targets only a 'player' and restricts blocking by 'that player's' creatures. TEXT B targets a 'player or planes'w
CURRENT ORACLE: Concussive Bolt deals 4 damage to target player or planeswalker. -- Metalcraft — If you control three or more artifacts, creatures controlled by that player or by that planeswalker's controller can't block this turn.
```
FILE mtg.txt
[card]
name=Concussive Bolt
target=player
auto=damage:4
auto=aslongas(artifact|mybattlefield) all(creature|opponentbattlefield) cantblock ueot >2
text=Concussive Bolt deals 4 damage to target player. -- Metalcraft - If you control three or more artifacts, creatures that player controls can't block this turn.
mana={3}{R}{R}
type=Sorcery
[/card]
```

## Fruit of the First Tree
JUDGE REASON: The scope of the enchantment has changed. TEXT A restricts the target to 'Enchant creature you control', whereas TEXT B allows the card to enchant any creature ('Enchant creature'). This is a changein
CURRENT ORACLE: Enchant creature -- When enchanted creature dies, you gain X life and draw X cards, where X is its toughness.
```
FILE mtg.txt
[card]
name=Fruit of the First Tree
target=creature|mybattlefield
auto=@movedto(mytgt|graveyard) from(Battlefield):choice life:toughness controller && draw:toughness controller
text=Enchant creature you control -- When enchanted creature dies, you gain X life and draw X cards, where X is its toughness.
mana={3}{G}
type=Enchantment
subtype=Aura
[/card]
```

## Harmless Assault
JUDGE REASON: The scope of damage prevention has changed. TEXT A prevents all combat damage from any source (including blockers), while TEXT B specifically limits the prevention to damage dealt by attacking only. A
CURRENT ORACLE: Prevent all combat damage that would be dealt this turn by attacking creatures.
```
FILE mtg.txt
[card]
name=Harmless Assault
auto=preventAllCombatDamage from(creature[attacking]|battlefield) ueot
text=Prevent all combat damage that would be dealt this turn.
mana={2}{W}{W}
type=Instant
[/card]
```

## Iroas, God of Victory
JUDGE REASON: The card's function has changed regarding the combat ability granted to creatures. TEXT A states that creatures you control 'can't be blocked except by two or more creatures' (a specific blocking rule
CURRENT ORACLE: Indestructible -- As long as your devotion to red and white is less than seven, Iroas isn't a creature. -- Creatures you control have menace. -- Prevent all damage that would be dealt to attacking creatures you control.
```
FILE mtg.txt
[card]
name=Iroas, God of Victory
abilities=indestructible
auto=lord(creature|mybattlefield) menace
auto=preventalldamage to(creature[attacking]|mybattlefield)
auto=this(variable{boros}<7) transforms((removetypes,newability[becomes(Legendary Enchantment God)]))
auto=this(variable{boros}>6) transforms((Legendary Enchantment Creature))
text=Indestructible -- As long as your devotion to red and white is less than seven, Iroas isn't a creature. -- Creatures you control can't be blocked except by two or more creatures. -- Prevent all damage that would be dealt to attacking creatures you control.
mana={2}{R}{W}
type=Legendary Enchantment Creature
subtype=God
power=7
toughness=4
[/card]
```

## Magus of the Will
JUDGE REASON: The scope of playable cards has changed. TEXT A allows playing 'cards' from the graveyard, which includes lands, artifacts, enchantments, creatures, and instants/sorceries. TEXT B explicitly restricts
CURRENT ORACLE: {2}{B}, {T}, Exile this creature: Until end of turn, you may play lands and cast spells from your graveyard. If a card would be put into your graveyard from anywhere this turn, exile that card instead.
```
FILE mtg.txt
[card]
name=Magus of the Will
auto={2}{B}{T}{E}:name(Can play from graveyard) emblem transforms((,newability[lord(*|mygraveyard) canPlayFromGraveyard],newability[@movedTo(*|mygraveyard):all(trigger[to]) moveTo(exile)])) ueot
text={2}{B}, {T}, Exile Magus of the Will: Until end of turn, you may play cards from your graveyard. If a card would be put into your graveyard from anywhere this turn, exile that card instead.
mana={2}{B}
type=Creature
subtype=Human Wizard
power=3
toughness=3
[/card]
```

## Obelisk of Undoing
JUDGE REASON: The scope of the target has changed. TEXT A allows targeting any permanent you own and control (which includes permanents you own that you do not control, such as those stolen by an opponent, provided
CURRENT ORACLE: {6}, {T}: Return target permanent you both own and control to your hand.
```
FILE mtg.txt
[card]
name=Obelisk of Undoing
auto={6}{T}:moveto(hand) target(*|myBattlefield)
text={6}, {T}: Return target permanent you own and control to your hand.
mana={1}
type=Artifact
[/card]
```

## Aspect of Lamprey
JUDGE REASON: The card's function has changed in three significant ways: 1. The trigger condition changed from 'target player' to 'target opponent', restricting who must discard. 2. The scope of the enchantment was
CURRENT ORACLE: Enchant creature you control -- When this Aura enters, target opponent discards two cards. -- Enchanted creature has lifelink.
```
FILE borderline.txt
[card]
name=Aspect of Lamprey
target=creature|myBattlefield
auto=target(opponent) ability$!name(discard 2 cards) target(<2>*|myhand) reject!$ targetedplayer
auto=teach(creature) lifelink
text=Enchant creature -- When Strands of Undeath enters, target player discards two cards. -- {B}: Regenerate enchanted creature.
mana={3}{B}
type=Enchantment
subtype=Aura
[/card]
```

## Burn at the Stake
JUDGE REASON: The target scope has changed. TEXT A restricts the damage to a 'target creature or player', whereas TEXT B allows the damage to be dealt to 'any target'. In current Magic: The Gathering rules, 'any' (
CURRENT ORACLE: As an additional cost to cast this spell, tap any number of untapped creatures you control. -- Burn at the Stake deals damage to any target equal to three times the number of creatures tapped this way.
```
FILE borderline.txt
[card]
name=Burn at the Stake
target=anytarget
auto=if type(creature[-tapped]|myBattlefield)~morethan~0 then choice name(Tap 1 creature) name(Tap 1 creature) damage:3 && ability$!name(Tap 1 creature) name(Tap 1 creature) target(creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~1 then choice name(Tap 2 creatures) name(Tap 2 creatures) damage:6 && ability$!name(Tap 2 creatures) name(Tap 2 creatures) target(<2>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~2 then choice name(Tap 3 creatures) name(Tap 3 creatures) damage:9 && ability$!name(Tap 3 creatures) name(Tap 3 creatures) target(<3>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~3 then choice name(Tap 4 creatures) name(Tap 4 creatures) damage:12 && ability$!name(Tap 4 creatures) name(Tap 4 creatures) target(<4>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~4 then choice name(Tap 5 creatures) name(Tap 5 creatures) damage:15 && ability$!name(Tap 5 creatures) name(Tap 5 creatures) target(<5>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~5 then choice name(Tap 6 creatures) name(Tap 6 creatures) damage:18 && ability$!name(Tap 6 creatures) name(Tap 6 creatures) target(<6>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~6 then choice name(Tap 7 creatures) name(Tap 7 creatures) damage:21 && ability$!name(Tap 7 creatures) name(Tap 7 creatures) target(<7>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~7 then choice name(Tap 8 creatures) name(Tap 8 creatures) damage:24 && ability$!name(Tap 8 creatures) name(Tap 8 creatures) target(<8>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~8 then choice name(Tap 9 creatures) name(Tap 9 creatures) damage:27 && ability$!name(Tap 9 creatures) name(Tap 9 creatures) target(<9>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~9 then choice name(Tap 10 creatures) name(Tap 10 creatures) damage:30 && ability$!name(Tap 10 creatures) name(Tap 10 creatures) target(<10>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~10 then choice name(Tap 11 creatures) name(Tap 11 creatures) damage:33 && ability$!name(Tap 11 creatures) name(Tap 11 creatures) target(<11>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~11 then choice name(Tap 12 creatures) name(Tap 12 creatures) damage:36 && ability$!name(Tap 12 creatures) name(Tap 12 creatures) target(<12>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~12 then choice name(Tap 13 creatures) name(Tap 13 creatures) damage:39 && ability$!name(Tap 13 creatures) name(Tap 13 creatures) target(<13>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~13 then choice name(Tap 14 creatures) name(Tap 14 creatures) damage:42 && ability$!name(Tap 14 creatures) name(Tap 14 creatures) target(<14>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~14 then choice name(Tap 15 creatures) name(Tap 15 creatures) damage:45 && ability$!name(Tap 15 creatures) name(Tap 15 creatures) target(<15>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~15 then choice name(Tap 16 creatures) name(Tap 16 creatures) damage:48 && ability$!name(Tap 16 creatures) name(Tap 16 creatures) target(<16>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~16 then choice name(Tap 17 creatures) name(Tap 17 creatures) damage:51 && ability$!name(Tap 17 creatures) name(Tap 17 creatures) target(<17>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~17 then choice name(Tap 18 creatures) name(Tap 18 creatures) damage:54 && ability$!name(Tap 18 creatures) name(Tap 18 creatures) target(<18>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~18 then choice name(Tap 19 creatures) name(Tap 19 creatures) damage:57 && ability$!name(Tap 19 creatures) name(Tap 19 creatures) target(<19>creature|myBattlefield) tap!$ controller
auto=if type(creature[-tapped]|myBattlefield)~morethan~19 then choice name(Tap 20 creatures) name(Tap 20 creatures) damage:60 && ability$!name(Tap 20 creatures) name(Tap 20 creatures) target(<20>creature|myBattlefield) tap!$ controller
text=As an additional cost to cast Burn at the Stake, tap any number of untapped creatures you control. -- Burn at the Stake deals damage to target creature or player equal to three times the number of creatures tapped this way.
mana={2}{R}{R}{R}
type=Sorcery
[/card]
```

## Davriel's Withering
JUDGE REASON: The scope of the target has changed. TEXT A allows targeting any creature (including your own), while TEXT B restricts the target to 'an opponent controls'. This is a functional change in targeting.
CURRENT ORACLE: Target creature an opponent controls perpetually gets -1/-2.
```
FILE borderline.txt
[card]
name=Davriel's Withering
target=creature
auto=counter(-1/-2,1,PerpetualPT)
text=Target creature perpetually gets -1/-2.
mana={B}
type=Instant
[/card]
```

## Furnace of Rath
JUDGE REASON: The scope of the effect changed. TEXT A specifies 'creature or player', while TEXT B specifies 'permanent or player'. This change expands the card's functionality to include non-creature permanents (e
CURRENT ORACLE: If a source would deal damage to a permanent or player, it deals double that damage to that permanent or player instead.
```
FILE borderline.txt
[card]
name=Furnace of Rath
auto=@damaged(creature) from(*[-Furnace of Rath]|*):damage:thatmuch all(trigger[to])
auto=@damaged(player) from(*[-Furnace of Rath]|*):damage:thatmuch all(trigger[to])
text=If a source would deal damage to a creature or player, it deals double that damage to that creature or player instead.
mana={1}{R}{R}{R}
type=Enchantment
[/card]
```

## Haldan, Avid Arcanist
JUDGE REASON: The scope of cards that can be played from exile has changed. TEXT A only allows playing 'noncreature cards', whereas TEXT B explicitly adds 'lands' to the list of playable cards ('You may play lands'
CURRENT ORACLE: Partner with Pako, Arcane Retriever (When this creature enters, target player may put Pako into their hand from their library, then shuffle.) -- You may play lands and cast noncreature spells from among cards you exiled that have fetch counters on them, and you may spend mana as though it were mana of any color to cast those spells.
```
FILE borderline.txt
[card]
name=Haldan, Avid Arcanist
abilities=partner
partner=Pako, Arcane Retriever
auto=_PARTNER_
auto=lord(*[-creature;counter{0/0.1.Fetch}]|myexile) transforms((,newability[canplayfromexile],newability[anytypeofmana]))
auto={0}:name(Cast opponent card) name(Cast opponent card) target(*[-creature;counter{0/0.1.Fetch}]|opponentexile) moveto(myexile) and!( transforms((,newability[counter(0/0.1.Fetch)],newability[phaseaction[end once checkex] moveTo(ownerexile)],newability[phaseaction[untap once checkex] counter(0/0.1.Fetch)])) ueot )!
text=Partner with Pako, Arcane Retriever (When this creature enters, target player may put Pako into their hand from their library, then shuffle.) -- You may play noncreature cards from exile with fetch counters on them if you exiled them, and you may spend mana as though it were mana of any color to cast those spells.
mana={2}{U}
type=Legendary Creature
subtype=Human Wizard
power=1
toughness=4
[/card]
```

## Laelia, the Blade Reforged
JUDGE REASON: The second ability in TEXT A includes a restrictive condition: 'Whenever a spell or ability **you control** exiles...'. This limits the trigger to only exile events caused by the player controlling La
CURRENT ORACLE: Haste -- Whenever Laelia attacks, exile the top card of your library. You may play that card this turn. -- Whenever one or more cards are put into exile from your library and/or your graveyard, put a +1/+1 counter on Laelia.
```
FILE borderline.txt
[card]
name=Laelia, the Blade Reforged
abilities=haste
auto=_ATTACKING_name(Exile top card) all(*[zpos=1]|mylibrary) moveto(myexile) and!( transforms((,newability[canplayfromexile])) ueot )!
auto=@movedto(*|myexile) from(mylibrary,mygraveyard):name(Put 1/1 counter) counter(1/1)
text=Haste -- Whenever Laelia, the Blade Reforged attacks, exile the top card of your library. You may play that card this turn. -- Whenever a spell or ability you control exiles one or more cards from your library and/or your graveyard, put a +1/+1 counter on Laelia.
mana={2}{R}
type=Legendary Creature
subtype=Spirit Warrior
power=2
toughness=2
[/card]
```

## Nightveil Specter
JUDGE REASON: The scope of playable cards changed. TEXT A states 'You may play cards exiled with Nightveil Specter,' which implies all card types (including lands, artifacts, enchantments, creatures, and sorceries)
CURRENT ORACLE: Flying -- Whenever this creature deals combat damage to a player, that player exiles the top card of their library. -- You may play lands and cast spells from among cards exiled with this creature.
```
FILE borderline.txt
[card]
name=Nightveil Specter
abilities=flying
auto=@combatdamaged(player) from(this):moveto(exile) and!( transforms((,canplayfromexile)))! all(*[zpos=1]|opponentlibrary)
text=Whenever Nightveil Specter deals combat damage to a player, that player exiles the top card of their library. -- You may play cards exiled with Nightveil Specter.
mana={UB}{UB}{UB}
type=Creature
subtype=Specter
power=2
toughness=3
[/card]
```

## Pyromancer's Swath
JUDGE REASON: The scope of the damage bonus changed. TEXT A specifies 'creature or player', while TEXT B specifies 'permanent or player'. This change allows the ability to apply to damage dealt to artifacts, lands,
CURRENT ORACLE: If an instant or sorcery source you control would deal damage to a permanent or player, it deals that much damage plus 2 to that permanent or player instead. -- At the beginning of each end step, discard your hand.
```
FILE borderline.txt
[card]
name=Pyromancer's Swath
auto=@damaged(creature|myBattlefield) from(*[instant;sorcery]|myzones):all(trigger[to]) damage:2
auto=@damaged(creature|opponentBattlefield) from(*[instant;sorcery]|*):all(trigger[to]) damage:2
auto=@damageof(player) from(*[instant;sorcery]|myzones):damage:2 opponent
auto=@damagefoeof(player) from(*[instant;sorcery]|*):damage:2 opponent
auto=@each end:name(Discard hand) all(*|myhand) reject
text=If an instant or sorcery source you control would deal damage to a creature or player, it deals that much damage plus 2 to that creature or player instead. -- At the beginning of each end step, discard your hand.
mana={2}{R}
type=Enchantment
[/card]
```

## Sen Triplets
JUDGE REASON: The functional scope of the ability to use the opponent's cards has changed. TEXT A states 'You may play cards from that player's hand', which in the context of older Magic rules (and the specific phr
CURRENT ORACLE: At the beginning of your upkeep, choose target opponent. This turn, that player can't cast spells or activate abilities and plays with their hand revealed. You may play lands and cast spells from that player's hand this turn.
```
FILE borderline.txt
[card]
name=Sen Triplets
auto=@each my upkeep:all(*|opponentBattlefield) noactivatedability && all(*|opponentcastingzone) maxCast(*)0 && may castcard(normal) target(*|opponenthand)
text=At the beginning of your upkeep, choose target opponent. This turn, that player can't cast spells or activate abilities and plays with their hand revealed. You may play cards from that player's hand this turn.
mana={2}{W}{U}{B}
type=Legendary Artifact Creature
subtype=Human Wizard
power=3
toughness=3
[/card]
```

## Shadow of the Second Sun
JUDGE REASON: The scope of the trigger condition has changed. TEXT A triggers only during the 'second main phase' (implying the second main phase of a turn, which typically only exists if an extra phase is added or
CURRENT ORACLE: Enchant player -- At the beginning of each of enchanted player's postcombat main phases, there is an additional beginning phase after this phase. (The end step happens after the added untap, upkeep, and draw steps.)
```
FILE borderline.txt
[card]
name=Shadow of the Second Sun
target=player
auto=@each targetedplayer secondmain:nextphasealter(add,untap,controller,after<this>)
auto=@each targetedplayer secondmain:nextphasealter(add,upkeep,controller,after<this>)
auto=@each targetedplayer secondmain:nextphasealter(add,draw,controller,after<this>)
text=Enchant player -- At the beginning of enchanted player's second main phase, there is an additional beginning phase after this phase. (The end step happens after the added untap, upkeep, and draw steps.)
mana={4}{U}{U}
type=Enchantment
subtype=Aura
[/card]
```

## Soul of Shandalar
JUDGE REASON: The functional scope of the damage target has changed. TEXT A restricts the damage to a 'target player' and a creature that player controls. TEXT B expands the target to include 'target player or **or
CURRENT ORACLE: First strike -- {3}{R}{R}: This creature deals 3 damage to target player or planeswalker and 3 damage to up to one target creature that player or that planeswalker's controller controls. -- {3}{R}{R}, Exile this card from your graveyard: It deals 3 damage to target player or planeswalker and 3 damage to up to one target creature that player or that planeswalker's controller controls.
```
FILE borderline.txt
[card]
name=Soul of Shandalar
abilities=first strike
auto={3}{R}{R}:damage:3 target(creature|opponentbattlefield) && damage:3 opponent
auto={3}{R}{R}:damage:3 target(creature|mybattlefield) && damage:3 controller
autograveyard={3}{R}{R}{E}:damage:3 target(creature|opponentbattlefield) && damage:3 opponent
autograveyard={3}{R}{R}{E}:damage:3 target(creature|mybattlefield) && damage:3 controller
text=First strike -- {3}{R}{R}: Soul of Shandalar deals 3 damage to target player and 3 damage to up to one target creature that player controls. -- {3}{R}{R}, Exile Soul of Shandalar from your graveyard: Soul of Shandalar deals 3 damage to target player and 3 damage to up to one target creature that player controls.
mana={4}{R}{R}
type=Creature
subtype=Avatar
power=6
toughness=6
[/card]
```

## Tireless Angler
JUDGE REASON: The scope of the trigger condition has changed. TEXT A only triggers on an 'Island' entering, while TEXT B triggers on either an 'Island' or a 'Swamp' entering. This adds a new condition (Swamp) that,
CURRENT ORACLE: Whenever an Island or Swamp enters the battlefield under your control, draft a card from this creature's spellbook.
```
FILE borderline.txt
[card]
name=Tireless Angler
auto=@movedTo(island|myBattlefield):name(Draft a card) transforms((,newability[choice name(Fleet Swallower) conjure cards(Fleet Swallower) zone(myhand)],newability[choice name(Moat Piranhas) conjure cards(Moat Piranhas) zone(myhand)],newability[choice name(Mystic Skyfish) conjure cards(Mystic Skyfish) zone(myhand)],newability[choice name(Nadir Kraken) conjure cards(Nadir Kraken) zone(myhand)],newability[choice name(Pouncing Shoreshark) conjure cards(Pouncing Shoreshark) zone(myhand)],newability[choice name(Sea-Dasher Octopus) conjure cards(Sea-Dasher Octopus) zone(myhand)],newability[choice name(Spined Megalodon) conjure cards(Spined Megalodon) zone(myhand)],newability[choice name(Stinging Lionfish) conjure cards(Stinging Lionfish) zone(myhand)],newability[choice name(Voracious Greatshark) conjure cards(Voracious Greatshark) zone(myhand)],newability[choice name(Archipelagore) conjure cards(Archipelagore) zone(myhand)],newability[choice name(Serpent of Yawning Depths) conjure cards(Serpent of Yawning Depths) zone(myhand)],newability[choice name(Wormhole Serpent) conjure cards(Wormhole Serpent) zone(myhand)],newability[choice name(Sigiled Starfish) conjure cards(Sigiled Starfish) zone(myhand)],newability[choice name(Riptide Turtle) conjure cards(Riptide Turtle) zone(myhand)],newability[choice name(Ruin Crab) conjure cards(Ruin Crab) zone(myhand)])) oneshot
text=Whenever an Island enters under your control, draft a card from Tireless Angler's spellbook.
mana={2}{U}
type=Creature
subtype=Human Rogue
power=1
toughness=4
[/card]
```

## Turn // Burn
JUDGE REASON: The 'Burn' half of the card has a functional change in its targeting scope. TEXT A specifies 'target creature or player', while TEXT B specifies 'any target'. Under current rules, 'any target' allows 
CURRENT ORACLE: Until end of turn, target creature loses all abilities and becomes a red Weird with base power and toughness 0/1. -- Fuse (You may cast one or both halves of this card from your hand.) -- Burn deals 2 damage to any target. -- Fuse (You may cast one or both halves of this card from your hand.)
```
FILE borderline.txt
[card]
name=Turn // Burn
abilities=hasnokicker
other={1}{R} name(Burn)
kicker={1}{R} name(Fuse)
auto=alternative ability$!name(Deal Damage) name(Deal Damage) damage:2 target(anytarget)!$ controller
auto=if paid(kicker) then ability$!name(Deal Damage) name(Deal Damage) damage:2 target(anytarget)!$ controller
auto=ifnot paid(alternative) then target(creature) transforms((Weird,red,setpower=0,settoughness=1,newability[loseabilities])) ueot
text=Target creature loses all abilities and becomes a 0/1 red Weird until end of turn. // Burn deals 2 damage to target creature or player. -- Fuse (You may cast one or both halves of this card from your hand.)
mana={2}{U}
type=Instant
[/card]
```

## Uba Mask
JUDGE REASON: The scope of playable cards changed. TEXT A allows players to play 'cards' (which includes spells and lands) from exile. TEXT B explicitly restricts this ability to 'lands and cast spells'. While 'sp'
CURRENT ORACLE: If a player would draw a card, that player exiles that card face up instead. -- Each player may play lands and cast spells from among cards they exiled with this artifact this turn.
```
FILE borderline.txt
[card]
name=Uba Mask
auto=replacedraw reveal:1 optionone all(*|reveal) moveto(exile) and!( transforms((,newability[canplayfromexile])) ueot)! optiononeend revealend
auto=opponentreplacedraw reveal:1 revealzone(opponentlibrary) optionone all(*|reveal) moveto(exile) and!( transforms((,newability[canplayfromexile])) ueot)! optiononeend revealend
text=If a player would draw a card, that player exiles that card face up instead. -- Each player may play cards they exiled with Uba Mask this turn.
mana={4}
type=Artifact
[/card]
```

## Nicol Bolas, Planeswalker
JUDGE REASON: The ultimate ability (-9) has a functional change in scope. TEXT A restricts the target to a 'target player' and the subsequent effects to that player. TEXT B expands the target to 'target player or [
CURRENT ORACLE: +3: Destroy target noncreature permanent. -- −2: Gain control of target creature. -- −9: Nicol Bolas deals 7 damage to target player or planeswalker. That player or that planeswalker's controller discards seven cards, then sacrifices seven permanents of their choice.
```
FILE planeswalkers.txt
[card]
name=Nicol Bolas, Planeswalker
auto=counter(0/0,5,loyalty)
auto={C(0/0,3,Loyalty)}:name(+3: Destroy target noncreature permanent) destroy target(*[-creature]|battlefield)
auto={C(0/0,-2,Loyalty)}:name(-2: Gain control of a creature) moveTo(mybattlefield) target(creature)
auto={C(0/0,-9,Loyalty)}:name(-9: Deals 7 damage, sacrifice and discards seven) target(player) damage:7 && all(this) transforms((,newability[ability$!notatarget(<7>*|mybattlefield) sacrifice!$ targetedplayer],newability[ability$!notatarget(<7>*|myhand) reject!$ targetedplayer]))
text=+3: Destroy target noncreature permanent. -- -2: Gain control of target creature. -- -9: Nicol Bolas, Planeswalker deals 7 damage to target player. That player discards seven cards, then sacrifices seven permanents.
mana={4}{U}{B}{B}{R}
type=Legendary Planeswalker
subtype=Bolas
[/card]
```

## Tamiyo, Field Researcher
JUDGE REASON: The scope of the emblem ability created by the -7 loyalty ability has changed. TEXT A restricts the free casting to 'nonland cards', whereas TEXT B allows casting 'spells' (which includes nonland inst
CURRENT ORACLE: +1: Choose up to two target creatures. Until your next turn, whenever either of those creatures deals combat damage, you draw a card. -- −2: Tap up to two target nonland permanents. They don't untap during their controller's next untap step. -- −7: Draw three cards. You get an emblem with "You may cast spells from your hand without paying their mana costs."
```
FILE planeswalkers.txt
[card]
name=Tamiyo, Field Researcher
auto=counter(0/0,4,loyalty)
auto={C(0/0,1,Loyalty)}:name(+1: Creatures deal damage causes to draw a card) target(<upto:2>creature) transforms((,newability[@combatdamaged(player) from(this):draw:1 controller])) uynt oneshot
auto={C(0/0,-2,Loyalty)}:name(-2: Tap permanent and doesn't untap) target(<upto:2>*[-land]) freeze
auto={C(0/0,-7,Loyalty)}:name(-7: Draw three and Emblem cast zero) draw:3 controller && emblem transforms((,newability[lord(*[-land]|myhand) zerocast])) forever dontremove
text=+1: Choose up to two target creatures. Until your next turn, whenever either of those creatures deals combat damage, you draw a card. -- -2: Tap up to two target nonland permanents. They don't untap during their controller's next untap step. -- -7: Draw three cards. You get an emblem with "You may cast nonland cards from your hand without paying their mana costs."
mana={1}{G}{W}{U}
type=Legendary Planeswalker
subtype=Tamiyo
[/card]
```

## Teferi Akosa of Zhalfir
JUDGE REASON: The third ability has a functional change in scope and cost. TEXT A requires tapping exactly 'X' creatures where X is an implicit variable, but the phrasing 'Tap X untapped creatures' without defining
CURRENT ORACLE: +1: Draw two cards. Then discard two cards unless you discard a creature card. -- −2: You get an emblem with "Knights you control get +1/+0 and have ward {1}." -- −3: Tap any number of untapped creatures you control. When you do, shuffle target nonland permanent an opponent controls with mana value X or less into its owner's library, where X is the number of creatures tapped this way.
```
FILE planeswalkers.txt
[card]
name=Teferi Akosa of Zhalfir
auto=counter(0/0,4,loyalty)
auto={C(0/0,1,Loyalty)}:name(+1: Draw and discard) draw:2 controller && transforms((,newability[if type(creature|myhand)~morethan~0 then choice name(Discard a creature) name(Discard a creature) target(creature|myhand) reject],newability[choice name(Discard 2 cards) target(<2>*[-creature]|myhand) reject])) oneshot
auto={C(0/0,-2,Loyalty)}:name(-2: Emblem for knights) emblem transforms((,newability[lord(knight|mybattlefield) transforms((,newability[1/0],newability[_WARD1_]))])) forever dontremove
auto={C(0/0,-3,Loyalty)}:name(-3: Tap 0 creatures and return permanent) target(*[-land&manacost<=0]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 1 creature and return permanent) target(*[-land&manacost<=1]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 2 creatures and return permanent) target(*[-land&manacost<=2]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 3 creatures and return permanent) target(*[-land&manacost<=3]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 4 creatures and return permanent) target(*[-land&manacost<=4]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 5 creatures and return permanent) target(*[-land&manacost<=5]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 6 creatures and return permanent) target(*[-land&manacost<=6]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 7 creatures and return permanent) target(*[-land&manacost<=7]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 8 creatures and return permanent) target(*[-land&manacost<=8]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 9 creatures and return permanent) target(*[-land&manacost<=9]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 10 creatures and return permanent) target(*[-land&manacost<=10]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 11 creatures and return permanent) target(*[-land&manacost<=11]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 12 creatures and return permanent) target(*[-land&manacost<=12]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 13 creatures and return permanent) target(*[-land&manacost<=13]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 14 creatures and return permanent) target(*[-land&manacost<=14]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 15 creatures and return permanent) target(*[-land&manacost<=15]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 16 creatures and return permanent) target(*[-land&manacost<=16]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 17 creatures and return permanent) target(*[-land&manacost<=17]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 18 creatures and return permanent) target(*[-land&manacost<=18]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 19 creatures and return permanent) target(*[-land&manacost<=19]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
auto={C(0/0,-3,Loyalty)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}{T(creature[-tapped]|mybattlefield)}:name(-3: Tap 20 creatures and return permanent) target(*[-land&manacost<=20]|opponentbattlefield) moveto(ownerlibrary) and!( transforms((,newability[shuffle])) oneshot )!
text=[+1]: Draw two cards. Then discard two cards unless you discard a creature card. -- [-2]: You get an emblem with "Knights you control get +1/+0 and have ward {1}." -- [-3]: Tap X untapped creatures you control. When you do, shuffle target nonland permanent an opponent controls with mana value X or less into its owner's library.
color=white,blue
type=Legendary Planeswalker
subtype=Teferi
[/card]
```

## Benevolent Unicorn
JUDGE REASON: The scope of the ability has changed. TEXT A applies only to 'creature or player', whereas TEXT B applies to 'permanent or player'. This change includes artifacts, enchantments, and lands in the scope
CURRENT ORACLE: If a spell would deal damage to a permanent or player, it deals that much damage minus 1 to that permanent or player instead.
```
FILE unsupported.txt
[card]
name=Benevolent Unicorn
text=If a spell would deal damage to a creature or player, it deals that much damage minus 1 to that creature or player instead.
mana={1}{W}
type=Creature
subtype=Unicorn
power=1
toughness=2
[/card]
```

## Chaos Orb
JUDGE REASON: The scope of destruction changed. TEXT A destroys 'all permanents' it touches, while TEXT B destroys 'all nontoken permanents' it touches. This is a functional difference because TEXT B explicitly spc
CURRENT ORACLE: {1}, {T}: If this artifact is on the battlefield, flip it onto the battlefield from a height of at least one foot. If this artifact turns over completely at least once during the flip, destroy all nontoken permanents it touches. Then destroy this artifact.
```
FILE unsupported.txt
[card]
name=Chaos Orb
text={1}, {T}: If Chaos Orb is on the battlefield, flip Chaos Orb onto the battlefield from a height of at least one foot. If Chaos Orb turns over completely at least once during the flip, destroy all permanents it touches. Then destroy Chaos Orb.
mana={2}
type=Artifact
[/card]
```

## Divine Presence
JUDGE REASON: The scope of the effect changed. TEXT A applies only to 'creature or player', whereas TEXT B applies to 'permanent or player'. This means TEXT B also prevents damage to non-creature permanents (suchas
CURRENT ORACLE: If a source would deal 4 or more damage to a permanent or player, that source deals 3 damage to that permanent or player instead.
```
FILE unsupported.txt
[card]
name=Divine Presence
text=If a source would deal 4 or more damage to a creature or player, that source deals 3 damage to that creature or player instead.
mana={2}{W}
type=Enchantment
[/card]
```

## Equal Treatment
JUDGE REASON: The scope of the replacement effect has changed. TEXT A applies only to damage dealt to a 'creature or player', whereas TEXT B applies to damage dealt to a 'permanent or player'. This change expands X
CURRENT ORACLE: If any source would deal 1 or more damage to a permanent or player this turn, it deals 2 damage to that permanent or player instead. -- Draw a card.
```
FILE unsupported.txt
[card]
name=Equal Treatment
text=If any source would deal 1 or more damage to a creature or player this turn, it deals 2 damage to that creature or player instead. -- Draw a card.
mana={1}{W}
type=Instant
[/card]
```

## Framed!
JUDGE REASON: The scope of the effect has changed. TEXT A applies to **all** permanents by the chosen artist, regardless of their current tapped/untapped state (implying a forced state change for all of them). TEXT
CURRENT ORACLE: Choose an artist. Tap all untapped permanents with art by that artist or untap all tapped permanents with art by that artist.
```
FILE unsupported.txt
[card]
name=Framed!
text=Tap or untap all permanents by the artist of your choice.
mana={1}{U}
type=Instant
[/card]
```

## Ghosts of the Innocent
JUDGE REASON: The scope of the effect changed. TEXT A applies only to 'creature or player', whereas TEXT B applies to 'permanent or player'. This change includes artifacts, enchantments, and lands in the effect's范围
CURRENT ORACLE: If a source would deal damage to a permanent or player, it deals half that damage, rounded down, to that permanent or player instead.
```
FILE unsupported.txt
[card]
name=Ghosts of the Innocent
text=If a source would deal damage to a creature or player, it deals half that damage, rounded down, to that creature or player instead.
mana={5}{W}{W}
type=Creature
subtype=Spirit
power=4
toughness=5
[/card]
```

## Naar Isle
JUDGE REASON: The trigger condition and target scope have changed. TEXT A triggers on 'rolling {C}' (a specific die roll mechanic) and targets a 'player'. TEXT B triggers on 'chaos ensues' (a broader keyworded game
CURRENT ORACLE: At the beginning of your upkeep, put a flame counter on Naar Isle, then Naar Isle deals damage to you equal to the number of flame counters on it. -- Whenever chaos ensues, Naar Isle deals 3 damage to target player or planeswalker.
```
FILE unsupported.txt
[card]
name=Naar Isle
text=At the beginning of your upkeep, put a flame counter on Naar Isle, then Naar Isle deals damage to you equal to the number of flame counters on it. -- Whenever you roll {C}, Naar Isle deals 3 damage to target player.
type=Plane
subtype=Wildfire
[/card]
```

## Rage Extractor
JUDGE REASON: The scope of the target has changed. TEXT A restricts the damage to 'target creature or player', whereas TEXT B allows the damage to be dealt to 'any target'. In current Magic: The Gathering rules, 'a
CURRENT ORACLE: ({R/P} can be paid with either {R} or 2 life.) -- Whenever you cast a spell with {H} in its mana cost, this artifact deals damage equal to that spell's mana value to any target.
```
FILE unsupported.txt
[card]
name=Rage Extractor
text=({PR} can be paid with either {R} or 2 life.) -- Whenever you cast a spell with Phyrexian in its mana cost, Rage Extractor deals damage equal to that spell's converted mana cost to target creature or player.
mana={4}{PR}
type=Artifact
[/card]
```

## Stronghold Furnace
JUDGE REASON: The scope of the damage doubling effect has changed. TEXT A specifies 'creature or player', while TEXT B specifies 'permanent or player'. This is a functional change because TEXT B now also doubles (1
CURRENT ORACLE: If a source would deal damage to a permanent or player, it deals double that damage instead. -- Whenever chaos ensues, Stronghold Furnace deals 1 damage to any target.
```
FILE unsupported.txt
[card]
name=Stronghold Furnace
text=If a source would deal damage to a creature or player, it deals double that damage to that creature or player instead. -- Whenever you roll {C}, Stronghold Furnace deals 1 damage to target creature or player.
type=Plane
subtype=Rath
[/card]
```

## X
JUDGE REASON: The activated ability with cost {3}{U}{B} has a changed scope. TEXT A restricts the effect to 'a card in the same hand as X', which implies any card type (including lands, artifacts, enchantments, etc
CURRENT ORACLE: As long as X is in X's owner's opponent's hand, X's owner may cast X and activate X's abilities. That opponent can't cast X and plays with their hand revealed. -- {U}{B}, {T}: Put X into target opponent's hand. -- {3}{U}{B}: You may play a land or cast a spell from the hand X is in. If you cast a spell this way, you cast it without paying its mana cost.
```
FILE unsupported.txt
[card]
name=X
text=As long as X is in X's owner's opponent's hand, X's owner may cast X and activate X's abilities. That opponent can't cast X and plays with their hand revealed. -- {U}{B}, {T}: Put X into target opponent's hand. -- {3}{U}{B}: You may play a card in the same hand as X without paying its mana cost.
mana={U}{B}
type=Legendary Creature
subtype=Human Spy
power=2
toughness=2
[/card]
```

