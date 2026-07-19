# Wave-2 class: ability — 6 cards

## Abzan Battle Priest
JUDGE REASON: The triggered ability granted to creatures with a +1/+1 counter has changed. TEXT A grants **first strike**, while TEXT B grants **lifelink**. These are distinct mechanical abilities with different in
CURRENT ORACLE: Outlast {W} ({W}, {T}: Put a +1/+1 counter on this creature. Outlast only as a sorcery.) -- Each creature you control with a +1/+1 counter on it has lifelink.
```
FILE mtg.txt
[card]
name=Abzan Battle Priest
auto={W}{T}:counter(1/1) asSorcery
auto=lord(creature[counter{1/1.1}]|mybattlefield) lifelink
text=Outlast {W} ({W},{T}: Put a +1/+1 counter on this creature. Outlast only as a sorcery.) -- Each creature you control with a +1/+1 counter on it has first strike.
mana={3}{W}
type=Creature
subtype=Human Cleric
power=3
toughness=2
[/card]
```

## Leyline of Vitality
JUDGE REASON: The triggered ability in TEXT A is mandatory ('you gain 1 life'), whereas the ability in TEXT B is optional ('you may gain 1 life'). This change in modality constitutes a functional difference in the 
CURRENT ORACLE: If this card is in your opening hand, you may begin the game with it on the battlefield. -- Creatures you control get +0/+1. -- Whenever a creature you control enters, you may gain 1 life.
```
FILE mtg.txt
[card]
name=Leyline of Vitality
auto=lord(creature|mybattlefield) 0/1
auto=@movedTo(creature|mybattlefield):life:1
text=If Leyline of Vitality is in your opening hand, you may begin the game with it on the battlefield. -- Creatures you control get +0/+1. -- Whenever a creature enters under your control, you gain 1 life.
mana={2}{G}{G}
type=Enchantment
abilities=leyline
[/card]
```

## Ancestor's Embrace
JUDGE REASON: TEXT B includes a replacement effect that exiles the card if it would be put into a graveyard from anywhere. This ability is completely absent in TEXT A, which only grants lifelink. This is a new, non
CURRENT ORACLE: Enchant creature -- Enchanted creature has lifelink. -- If Ancestor's Embrace would be put into a graveyard from anywhere, exile it instead.
```
FILE borderline.txt
[card]
name=Ancestor's Embrace
abilities=exiledeath
backside=Kindly Ancestor
target=creature
auto=teach(creature) lifelink
text=Enchant creature -- Enchanted creature has lifelink. // Kindly Ancestor
color=white
type=Enchantment
subtype=Aura
[/card]
```

## Glorious Protector
JUDGE REASON: TEXT B includes the 'Foretell {2}{W}' ability, which is completely absent from TEXT A. This is an added ability that changes the card's functionality by providing an alternative way to cast the card.
CURRENT ORACLE: Flash -- Flying -- When this creature enters, you may exile any number of non-Angel creatures you control until this creature leaves the battlefield. -- Foretell {2}{W}
```
FILE borderline.txt
[card]
name=Glorious Protector
abilities=foretell,flash,flying
autohand=_FORETELL_
autoexile={2}{W} restriction{compare(canforetellcast)~morethan~0}:name(Cast with foretell) name(Cast with foretell) activate castcard(alternative)
auto=may name(Exile non-angel creatures) target(<anyamount>creature[-angel]|mybattlefield) (blink)forsrc
text=Flash -- Flying -- When Glorious Protector enters, you may exile any number of non-Angel creatures you control until Glorious Protector leaves the battlefield.
mana={2}{W}{W}
type=Creature
subtype=Angel Cleric
power=3
toughness=4
[/card]
```

## Necropolis Fiend
JUDGE REASON: TEXT A is missing the entire activated ability: '{X}, {T}, Exile X cards from your graveyard: Target creature gets -X/-X until end of turn.' This is a significant functional change, as TEXT A only has
CURRENT ORACLE: Delve (Each card you exile from your graveyard while casting this spell pays for {1}.) -- Flying -- {X}, {T}, Exile X cards from your graveyard: Target creature gets -X/-X until end of turn.
```
FILE borderline.txt
[card]
name=Necropolis Fiend
abilities=flying
auto={X}{T}{E<X>(*|myGraveyard)}: target(creature) -X/-X ueot
text=Flying -- Delve (You may exile any number of cards from your graveyard as you cast this spell. It costs {1} less to cast for each card exiled this way.)
mana={7}{B}{B}
other={delve} name(Delve)
type=Creature
subtype=Demon
power=4
toughness=5
[/card]
```

## Urza's Science Fair Project
JUDGE REASON: The function changed on result 3. TEXT A states 'Attacking does not cause it to tap this turn,' which is a specific restriction on the attacking process. TEXT B states 'It gains vigilance until end of
CURRENT ORACLE: {2}: Roll a six-sided die. This creature gets the indicated result. -- 1 — It gets -2/-2 until end of turn. -- 2 — Prevent all combat damage it would deal this turn. -- 3 — It gains vigilance until end of turn. -- 4 — It gains first strike until end of turn. -- 5 — It gains flying until end of turn. -- 6 — It gets +2/+2 until end of turn.
```
FILE borderline.txt
[card]
name=Urza's Science Fair Project
auto={2}:all(this) rolld6 6 winability counter(0/0,1,UrzaEffect) winabilityend loseability counter(0/0,1,UrzaEffect) loseabilityend rolld6end
auto=@dierolled(this) result(1) from(controller):this(counter{0/0.1.UrzaEffect}=1) name(Gets -2/-2) name(Gets -2/-2) all(this) transforms((,newability[-2/-2],newability[counter(0/0.-1.UrzaEffect)])) ueot
auto=@dierolled(this) result(2) from(controller):this(counter{0/0.1.UrzaEffect}=1) name(It deals no combat damage) name(It deals no combat damage) all(this) transforms((,newability[preventAllCombatDamage from(this)],newability[counter(0/0.-1.UrzaEffect)])) ueot
auto=@dierolled(this) result(3) from(controller):this(counter{0/0.1.UrzaEffect}=1) name(Gains vigilance) name(Gains vigilance) all(this) transforms((,vigilance,newability[counter(0/0.-1.UrzaEffect)])) ueot
auto=@dierolled(this) result(4) from(controller):this(counter{0/0.1.UrzaEffect}=1) name(Gains first strike) name(Gains first strike) all(this) transforms((,first strike,newability[counter(0/0.-1.UrzaEffect)])) ueot
auto=@dierolled(this) result(5) from(controller):this(counter{0/0.1.UrzaEffect}=1) name(Gains flying) name(Gains flying) all(this) transforms((,flying,newability[counter(0/0.-1.UrzaEffect)])) ueot
auto=@dierolled(this) result(6) from(controller):this(counter{0/0.1.UrzaEffect}=1) name(Gets 2/2) name(Gets 2/2) all(this) transforms((,newability[2/2],newability[counter(0/0.-1.UrzaEffect)])) ueot
text={2}: Roll a six-sided die for Urza's Science Fair Project. -- 1 It gets -2/-2 until end of turn. -- 2 It deals no combat damage this turn. -- 3 Attacking does not cause it to tap this turn. -- 4 It gains first strike until end of turn. -- 5 It gains flying until end of turn. -- 6 It gets +2/+2 until end of turn.
mana={6}
type=Artifact Creature
power=4
toughness=4
[/card]
```

