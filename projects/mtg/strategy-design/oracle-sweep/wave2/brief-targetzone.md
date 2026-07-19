# Wave-2 class: targetzone — 4 cards

## Godtracker of Jund
JUDGE REASON: The target of the ability has changed. TEXT A places the +1/+1 counter on 'Godtracker of Jund' itself. TEXT B places the +1/+1 counter on 'this creature' (the creature that entered the battlefield). A
CURRENT ORACLE: Whenever a creature you control with power 5 or greater enters, you may put a +1/+1 counter on this creature.
```
FILE mtg.txt
[card]
name=Godtracker of Jund
auto=@movedTo(creature[power>=5]|myBattlefield):may counter(1/1)
text=Whenever a creature with power 5 or greater enters under your control, you may put a +1/+1 counter on Godtracker of Jund.
mana={1}{R}{G}
type=Creature
subtype=Elf Shaman
power=2
toughness=2
[/card]
```

## Yawgmoth's Agenda
JUDGE REASON: TEXT B explicitly includes the ability to play **lands** from the graveyard, whereas TEXT A only mentions playing **cards** (which, in the context of the restriction on casting spells, implies only sp
CURRENT ORACLE: You can't cast more than one spell each turn. -- You may play lands and cast spells from your graveyard. -- If a card would be put into your graveyard from anywhere, exile it instead.
```
FILE mtg.txt
[card]
name=Yawgmoth's Agenda
abilities=mygraveexiler
auto=lord(*|mygraveyard) canPlayFromGraveyard
auto=maxCast(*)1
text=You can't cast more than one spell each turn. -- You may play cards from your graveyard. -- If a card would be put into your graveyard from anywhere, exile it instead.
mana={3}{B}{B}
type=Enchantment
[/card]
```

## Kronch Wrangler
JUDGE REASON: The target of the ability has changed. TEXT A specifies putting a +1/+1 counter on 'Kronch Wrangler' (the source of the ability), whereas TEXT B specifies putting the counter on 'this creature' (the *
CURRENT ORACLE: Trample -- Whenever a creature you control with power 4 or greater enters, put a +1/+1 counter on this creature.
```
FILE borderline.txt
[card]
name=Kronch Wrangler
abilities=trample
auto=@movedTo(creature[power>=4]|myBattlefield):counter(1/1)
text=Trample -- Whenever a creature with power 4 or greater enters under your control, put a +1/+1 counter on Kronch Wrangler.
mana={1}{G}
type=Creature
subtype=Human Warrior
power=2
toughness=1
[/card]
```

## Oubliette
JUDGE REASON: The two texts describe fundamentally different game mechanics. TEXT A describes an **exile** effect: the creature is removed from the game entirely and returned later with specific conditions (counted
CURRENT ORACLE: When this enchantment enters, target creature phases out until this enchantment leaves the battlefield. Tap that creature as it phases in this way. (Auras and Equipment phase out with it. While permanents are phased out, they're treated as though they don't exist.)
```
FILE unsupported.txt
[card]
name=Oubliette
text=When Oubliette enters the battlefield, exile target creature and all Auras attached to it. Note the number and kind of counters that were on that creature. -- When Oubliette leaves the battlefield, return the exiled card to the battlefield under its owner's control tapped with the noted number and kind of counters on it. If you do, return the exiled Aura cards to the battlefield under their owner's control attached to that permanent.
mana={1}{B}{B}
type=Enchantment
[/card]
```

