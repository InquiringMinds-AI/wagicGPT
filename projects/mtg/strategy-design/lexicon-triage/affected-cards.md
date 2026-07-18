# Affected-card inventory per engine-blocked lexicon item

For every lexical term whose conformance test is parked in `_known_failures.txt`,
the REAL cards in `Res/sets/primitives/` whose script uses that term. After fixing
an engine function, re-check its cards here (spot-play or fixture them) before
declaring the fix done. Generated 2026-07-18 by scanning script-bearing lines only
(auto*/abilities/anyzone/other/crewbonus/suspend/... — text= flavor lines excluded);
cards in unsupported.txt do not load today but become relevant if support lands.

## mutated-trigger-parse-null — @mutated trigger (<upto:mutations>/may form parses NULL)
33 card(s):
- **borderline.txt** (33): Archipelagore, Auspicious Starrix, Boneyard Lurker, Cavern Whisperer, Chittering Harvester, Cloudpiercer, Cubwarden, Dirge Bat, Dreamtail Heron, Essence Symbiote, Everquill Phoenix, Gemrazer, Glowstone Recluse, Huntmaster Liger, Illuna, Apex of Wishes, Insatiable Hemophage, Lore Drakkis, Majestic Auricorn, Migratory Greathorn, Mindleecher, Mysterious Egg, Necropanther, Nethroi, Apex of Death, Pouncing Shoreshark, Regal Leosaur, Sawtusk Demolisher, Snapdax, Apex of the Hunt, Souvenir Snatcher, Surgeon General Commander, Trumpeting Gnarr, Vadrok, Apex of Thunder, Vulpikeet, Zagoth Mamba

## gift-alternative-parse-null — RESCOPED 2026-07-18: 23 cards (Gift family, gift branch only)
NOT a parser bug; ~220 other `paid(alternative)` cards (Kicker/Cleave/Adventure/
Overload/...) verified working. The Gift family's `other=`==`mana=` makes
`paid(alternative)` unreachable, so every gift branch is dead (defaults work).
Re-check after the engine gains a promise/yes-no additional-cost primitive:
- **borderline.txt** (23): Blooming Blast, Coiling Rebirth, Consumed by Greed, Crumb and Get It, Dawn's Truce, Dewdrop Cure, Into the Flood Maw, Kitnap, Long River's Pull, Longstalk Brawl, Mind Spiral, Nocturnal Hunger, Octomancer, Parting Gust, Peerless Recycling, Perch Protection, Sazacap's Brew, Scrapshooter, Starfall Invocation, Starforged Sword, Valley Rally, Wear Down, Wildfire Howl

## battle-defense-parse-null — Battle defense-counter line parses NULL on real cards
36 card(s):
- **borderline.txt** (36): Invasion of Alara, Invasion of Amonkhet, Invasion of Arcavios, Invasion of Azgol, Invasion of Belenon, Invasion of Dominaria, Invasion of Eldraine, Invasion of Ergamon, Invasion of Fiora, Invasion of Gobakhan, Invasion of Ikoria, Invasion of Innistrad, Invasion of Ixalan, Invasion of Kaladesh, Invasion of Kaldheim, Invasion of Kamigawa, Invasion of Karsus, Invasion of Kylem, Invasion of Lorwyn, Invasion of Mercadia, Invasion of Moag, Invasion of Muraganda, Invasion of New Capenna, Invasion of New Phyrexia, Invasion of Pyrulea, Invasion of Ravnica, Invasion of Regatha, Invasion of Segovia, Invasion of Shandalar, Invasion of Tarkir, Invasion of Theros, Invasion of Tolvada, Invasion of Ulgrotha, Invasion of Vryn, Invasion of Xerex, Invasion of Zendikar

## noentertrg — noentertrg unimplemented
3 card(s):
- **borderline.txt** (3): Hushbringer, Hushwing Gryff, Tocatli Honor Guard

## banding — banding unimplemented
22 card(s):
- **unsupported.txt** (22): Ayesha Tanaka, Benalish Hero, Benalish Infantry, Camel, Icatian Phalanx, Icatian Skirmishers, Kjeldoran Escort, Kjeldoran Knight, Kjeldoran Phalanx, Kjeldoran Skycaptain, Kjeldoran Skyknight, Kjeldoran Warrior, Knights of Thorn, Mesa Pegasus, Mishra's War Machine, Noble Elephant, Pikemen, Shield Bearer, Teremko Griffin, Timber Wolves, Volunteer Reserves, War Elephant

## phasing — phasing unimplemented (incl. phaseout effect)
44 card(s):
- **mtg.txt** (25): Breezekeeper, Cloak of Invisibility, Crystal Golem, Dream Fighter, Frenetic Efreet, Katabatic Winds, Merfolk Raiders, Mist Dragon, Rainbow Efreet, Reality Ripple, Sandbar Crocodile, Shimmer, Taniwha, Teferi's Curse, Teferi's Drake, Teferi's Honor Guard, Teferi's Imp, Teferi's Isle, Teferi's Realm, Teferi's Veil, Tolarian Drake, Vanishing, Vaporous Djinn, Vodalian Illusionist, Warping Wurm
- **borderline.txt** (15): Blink Dog, Clever Concealment, Creeping Inn, Divine Smite, Guardian of Faith, King of the Oathbreakers, March of Swirling Mist, Out of Time, Perch Protection, Robe of Stars, Sapphire Charm, Slip Out the Back, Spectral Adversary, Teferi's Protection, Unite the Coalition
- **planeswalkers.txt** (4): Kaito Shizuki, Teferi, Master of Time, Teferi, Timeless Voyager, Vronos, Masked Inquisitor

## leyline — leyline unimplemented
16 card(s):
- **mtg.txt** (7): Leyline of Anticipation, Leyline of Lightning, Leyline of Sanctity, Leyline of Singularity, Leyline of Vitality, Leyline of the Meek, Leyline of the Void
- **borderline.txt** (9): Leyline Axe, Leyline of Abundance, Leyline of Combustion, Leyline of Hope, Leyline of Lifeforce, Leyline of Mutation, Leyline of Punishment, Leyline of Transformation, Leyline of the Guildpact

## daynight — day/night machinery broken (daybound/nightbound/nonight)
81 card(s):
- **mtg.txt** (1): Vildin-Pack Alpha
- **borderline.txt** (78): Angel of Eternal Dawn, Avabruck Caretaker, Ballista Watcher, Ballista Wielder, Baneblade Scoundrel, Baneclaw Marauder, Bird Admirer, Blossom-Clad Werewolf, Brutal Cathar, Burly Breaker, Child of the Pack, Curse of Leeches, Day, Dire-Strain Anarchist, Dire-Strain Brawler, Dire-Strain Demolisher, Fangblade Brigand, Fangblade Eviscerator, Fearful Villager, Fearsome Werewolf, Frenzied Trapbreaker, Graveyard Glutton, Graveyard Trespasser, Harvesttide Assailant, Harvesttide Infiltrator, Hollowhenge Huntmaster, Hookhand Mariner, Hound Tamer, Howlpack Avenger, Howlpack Piper, Ill-Tempered Loner, Infestation Expert, Infested Werewolf, Into the Night, Kessig Naturalist, Lambholt Raconteur, Lambholt Ravager, Leeching Lurker, Lord of the Ulvenwald, Moonlit Ambusher, Moonmist, Moonrage Brute, Night, Oakshade Stalker, Outland Liberator, Rahilda, Feral Outlaw, Rahilda, Wanted Cutthroat, Reckless Stormseeker, Riphook Raider, Savage Packmate, Seafaring Werewolf, Shady Traveler, Spellrune Howler, Spellrune Painter, Stalking Predator, Storm-Charged Slasher, Suspicious Stowaway, Tavern Ruffian, Tavern Smasher, The Celestus, Tireless Hauler, Tovolar's Huntmaster, Tovolar's Packleader, Tovolar, Dire Overlord, Tovolar, the Midnight Scourge, Unnatural Moonrise, Untamed Pup, Village Reavers, Village Watch, Volatile Arsonist, Waxing Moon, Weary Prisoner, Weaver of Blossoms, Wedding Crasher, Wildsong Howler, Wing Shredder, Wolfkin Outcast, Wrathful Jailbreaker
- **planeswalkers.txt** (2): Arlinn, the Moon's Fury, Arlinn, the Pack's Hope

## reduceto — reduceto no-op
8 card(s):
- **mtg.txt** (5): Ali from Cairo, Elderscale Wurm, Fortune Thief, Sustaining Spirit, Worship
- **borderline.txt** (2): Angel of Grace, Angel's Grace
- **planeswalkers.txt** (1): Serra the Benevolent

## combattoughness — combattoughness deals power to players
15 card(s):
- **mtg.txt** (2): Assault Formation, Doran, the Siege Tower
- **borderline.txt** (12): Ancient Lumberknot, Arcades, the Strategist, Belligerent Brontodon, Bill the Pony, Felothar the Steadfast, Gauntlets of Light, High Alert, Rasaad yn Bashir, Solid Footing, Streetwise Negotiator, Treefolk Umbra, Walking Bulwark
- **planeswalkers.txt** (1): Huatli, the Sun's Heart

## totemarmor — totemarmor aura not consumed
16 card(s):
- **mtg.txt** (12): Bear Umbra, Boar Umbra, Crab Umbra, Drake Umbra, Eel Umbra, Eland Umbra, Felidar Umbra, Hyena Umbra, Indrik Umbra, Mammoth Umbra, Snake Umbra, Spider Umbra
- **borderline.txt** (4): Lion Umbra, Mask Est, Octopus Umbra, Treefolk Umbra

## noloyaltydamage — noloyaltydamage blocks the cast
1 card(s):
- **borderline.txt** (1): Spark Rupture

## alterdevoffset — alterdevoffset gain over-counts
1 card(s):
- **borderline.txt** (1): Altar of the Pantheon

## donothing-and — donothing composed with and! (follow-up dropped)
6 card(s):
- **mtg.txt** (2): Cranial Archive, Thran Foundry
- **borderline.txt** (4): Borborygmos and Fblthp, Diviner's Lockbox, Druid of the Emerald Grove, Emberwilde Djinn

## counted-amount — count/countb counted value not passed
27 card(s):
- **mtg.txt** (10): Arjun, the Shifting Flame, Bane of Progress, Chain Reaction, Craterhoof Behemoth, Dark Deal, Forgotten Creation, Head Games, Mass Polymorph, Teferi's Puzzle Box, Tree of Perdition
- **borderline.txt** (17): Ancient Excavation, Annihilation Rooms, Awaken the Erstwhile, Death Begets Life, Ecological Appreciation, Evra, Halcyon Witness, Glimpse of Tomorrow, Harness Infinity, Incendiary Command, Jester's Mask, Khorvath's Fury, Last One Standing, Oversimplify, Phyrexian Rebirth, Soulgorger Orgg, Synthetic Destiny, Valakut Exploration

## amass — _AMASS_ interactive payload fizzle
41 card(s):
- **borderline.txt** (40): Aven Eternal, Barad-dur, Bleeding Edge, Book of Mazarbul, Callous Dismissal, Corsairs of Umbar, Crush Dissent, Deceive the Messenger, Dreadhorde Invasion, Dreadhorde Twins, Dunland Crebain, Easterling Vanguard, Enter the God-Eternals, Eternal Skylord, Fall of Cair Andros, Gleaming Overseer, Gothmog, Morgul Lieutenant, Grima Wormtongue, Honor the God-Pharaoh, Invading Manticore, Lazotep Plating, Lazotep Reaver, Lazotep Sliver, March from the Black Gate, Mindless Conscription, Mordor Muster, Moria Scavenger, Orcish Bowmasters, Orcish Medicine, Relentless Advance, Saruman's Trickery, Saruman, the White Hand, Sauron, Lord of the Rings, Sauron, the Dark Lord, Swarming of Moria, The Torment of Gollum, Toll of the Invasion, Treason of Isengard, Vizier of the Scorpion, Warbeast of Gorgoroth
- **planeswalkers.txt** (1): Angrath, Captain of Chaos

## monarch-initiative — _MONARCH_OPPONENT_/_INITIATIVE_OPPONENT_ rider fizzle + takesinitiative
3 card(s):
- **borderline.txt** (3): Denethor, Stone Seer, Eomer, King of Rohan, Jared Carthalion, True Heir

## exploits — exploits over-sacrifice + @exploited never fires
22 card(s):
- **mtg.txt** (8): Minister of Pain, Qarsi Sadist, Rakshasa Gravecaller, Sidisi's Faithful, Sidisi, Undead Vizier, Silumgar Butcher, Silumgar Sorcerer, Vulturous Aven
- **borderline.txt** (14): Diver Skaab, Fell Stinger, Graf Reaver, Gurmag Drowner, Infernal Captor, Loathsome Curator, Mindleech Ghoul, Overcharged Amalgam, Profaner of the Dead, Repository Skaab, Rot-Tide Gargantua, Silumgar Scavenger, Skull Skaab, Stitched Assistant

## foretell — doforetell/@foretold event missing + no cast from exile
49 card(s):
- **borderline.txt** (49): Adios Wormhole, Alrund's Epiphany, Augury Raven, Battle Mammoth, Behold the Multiverse, Cosmic Intervention, Cosmos Charger, Crush the Weak, Delayed Blast Fireball, Demon Bolt, Depart the Realm, Doomskar, Doomskar Oracle, Doomskar Titan, Dream Devourer, Dual Strike, Dwarven Reinforcements, Ethereal Valkyrie, Glorious Protector, Gods' Hall Guardian, Haunting Voyage, Iron Verdict, Jarl of the Forsaken, Karfell Harbinger, Kaya's Onslaught, Lupine Harbingers, Mammoth Growth, Mystic Reflection, Niko Defies Destiny, Poison the Cup, Quakebringer, Ranar the Ever-Watchful, Ravenform, Return Upon the Tide, Rise of the Dread Marn, Sage of the Beyond, Sarulf's Packmate, Saw It Coming, Scorn Effigy, Shepherd of the Cosmos, Skull Raid, Spectral Deluge, Starnheim Unleashed, Stoic Farmer, Struggle for Skemfar, Tales of the Ancestors, Tergrid's Shadow, Vengeful Reaper, Warhorn Blast

## afterrevealed — RESCOPED 2026-07-18: 0 real cards affected
Probe-verified: all 410 real `afterrevealed` lines carry the mandatory
`optionone`/`optiontwo` drain block and all four structural variants WORK. The
broken shape (reveal with NO drain block) exists only in the synthetic witness,
now reauthored and green. Residual action: parse-time rejection of drain-less
reveal (silent card-stranding today).

## surveil — surveil put-in-graveyard strands cards in temp zone
104 card(s):
- **mtg.txt** (2): Grim Flayer, Sultai Ascendancy
- **borderline.txt** (100): Appendage Amalgam, Barrier of Bones, Basilica Stalker, Blood Operative, Boulderborn Dragon, Broodheart Engine, Broodspinner, Candlestick, Case of the Shifting Visage, Cephalid Inkmage, Charnel Serenade, Citywatch Sphinx, Clandestine Meddler, Coastal Bulwark, Commercial District, Conduit Pylons, Connive // Concoct, Consider, Corroding Dragonstorm, Cruel Truths, Curate, Darkblade Agent, Dazzling Lights, Deadly Visit, Deeproot Wayfinder, Dimir Informant, Dimir Spybug, Diresight, Discovery // Dispersal, Disinformation Campaign, Dogged Detective, Doom Whisperer, Drag the Canal, Dragon's Rage Channeler, Dream Eater, Elegant Parlor, Eloise, Nephalia Sleuth, Ephara's Dispersal, Essence Anchor, Faerie Dreamthief, Failed Conversion, Fangkeeper's Familiar, Fear of Surveillance, Foul Watcher, Gift of Compleation, Glarb, Calamity's Augur, Gossip's Talent, Grim Bauble, Hedge Maze, Hidden Grotto, House Guildmage, Kishla Village, Lazav, the Multifarious, Lightshell Duo, Lush Portico, Master of Death, Mephitic Vapors, Meticulous Archive, Mind Drill Assailant, Mindwhisker, Mission Briefing, Naga Oracle, Neutralize the Guards, Nightblade Brigade, Nightveil Sprite, Notion Rain, Otherworldly Gaze, Pile On, Price of Fame, Raucous Theater, Rubblebelt Maverick, Rune-Sealed Wall, Sanitation Automaton, Seer of Stolen Sight, Shadowy Backstreet, Sinister Sabotage, Sinister Starfish, Skullsnap Nuisance, Snarling Gorehound, Spellgyre, Sterling Hound, Sword of Once and Future, Thought Erasure, Thoughtbound Phantasm, Thundering Falls, Tocasia's Dig Site, Tocasia, Dig Site Mentor, Unauthorized Exit, Undercity Sewers, Underground Mortuary, Unexplained Disappearance, Vanguard Seraph, Vanish from Sight, Wary Thespian, Wary Watchdog, Watcher in the Mist, Whisper Agent, Whispering Snitch, Wreckage Wickerfolk, Wretched Doll
- **planeswalkers.txt** (2): Dakkon, Shadow Slayer, Kaito, Bane of Nightmares

## manifest — manifest battlefield leg (Manifest Dread confirmed; manifest family shares the path)
31 card(s):
- **mtg.txt** (11): Cloudform, Ethereal Ambush, Fierce Invocation, Formless Nurturing, Lightform, Mastery of the Unseen, Qarsi High Priest, Rageform, Soul Summons, Temur War Shaman, Wildcall
- **borderline.txt** (19): Abhorrent Oculus, Arashin War Beast, Bashful Beastie, Break Down the Door, Curator Beastie, Defiant Survivor, Disturbing Mirth, Guardian of the Forgotten, Innocuous Rat, Manifest Dread, Omarthis, Ghostfire Initiate, Reality Shift, Scroll of Fate, Soul-Strike Technique, Turn Inside Out, Twist Reality, Ugin's Mastery, Unsettling Twins, Unwanted Remake
- **planeswalkers.txt** (1): Tezzeret, Cruel Machinist

## cantbetargetof — cantbetargetof granted-to-another never applies
31 card(s):
- **mtg.txt** (15): Artifact Ward, Bartel Runeaxe, Consecrate Land, Dense Foliage, Fiendslayer Paladin, Gaea's Revenge, Goblin Brawler, Karplusan Strider, Mercenary Informer, Rebel Informer, Spectral Shield, Spellbane Centaur, Suq'Ata Firewalker, Tetravite, Tetsuo Umezawa
- **borderline.txt** (16): Anti-Magic Aura, Breaker of Creation, Dennick, Pious Apprentice, Display of Dominance, Elenda, Saint of Dusk, Garruk's Harbinger, General Ferrous Rokiric, Knight of Grace, Knight of Malice, Nevinyrral, Urborg Tyrant, Shanna, Sisay's Legacy, Sphinx of the Guildpact, Sporeweb Weaver, Tomik, Distinguished Advokist, Underworld Cerberus, Veil of Summer

## ward — ward counter/fizzle unreliable
87 card(s):
- **mtg.txt** (1): Armguard Familiar
- **borderline.txt** (84): Abuelo, Ancestral Echo, Adrix and Nev, Twincasters, Aegis Sculptor, Ambling Stormshell, Ancient Imperiosaur, Archive Dragon, Archmage of Echoes, Armored Armadillo, Bloomwielder Dryads, Bronze Guardian, Burly Breaker, Cackling Prowler, Cactusfolk Sureshot, Canopy Gargantuan, Chains of Custody, Chapel Shieldgeist, Chaplain of Alms, Colossal Skyturtle, Combat Research, Dancing Sword Construct, Dire-Strain Demolisher, Dirgur Island Dragon // Skimming Strike, Dreadlight Monstrosity, Dusk Rose Reliquary, Eshki Dragonclaw, Frostfist Strider, Gargantuan Slabhorn, Gavony Dawnguard, Giant Ankheg, Gitaxian Spellstalker, Grey Host Reinforcements, Hamlet Vanguard, Hexbane Tortoise, Hoverstone Pilgrim, Hulking Raptor, Imoen, Mystic Trickster, Interdisciplinary Mascot, Iymrith, Desert Doom, Jin-Gitaxias, Kairi, the Swirling Sky, Kappa Cannoneer, Kiora, Sovereign of the Deep, Koma, World-Eater, Lavaspur Boots, Leonin Lightbringer, Lionheart Glimmer, Long River Lurker, Marauding Brinefang, Miirym, Sentinel Wyrm, Mirrorshell Crab, Nashi, Moon's Legacy, Neverwinter Hydra, Obsessive Collector, Octavia, Living Thesis, Oildeep Gearhulk, Patchwork Automaton, Pippin, Guard of the Citadel, Purple Worm, Radagast, Wizard of Wilds, Ratadrabik of Urborg, Rimeshield Frost Giant, Rimewall Protector, Sailors' Bane, Saruman the White, Seal from Existence, Shadow Puppeteers, Shelob, Child of Ungoliant, Sheltered by Ghosts, Sleep-Cursed Faerie, Storm of Saruman, Su-Chi Cave Guard, Tangletrove Kelp, Toadstool Admirer, Tolarian Terror, Tomakul Honor Guard, Torrent Sculptor, Trapped in the Screen, Treebeard, Gracious Host, Tyrranax Rex, Varis, Silverymoon Ranger, Voja, Jaws of the Conclave, Waterfall Aerialist, Wilson, Refined Grizzly, Winter, Misanthropic Guide
- **planeswalkers.txt** (1): Teferi Akosa of Zhalfir
- **unsupported.txt** (1): Aboleth Spawn

## crewbonus — crewbonus not applied to crewed vehicle
3 card(s):
- **mtg.txt** (3): Gearshift Ace, Speedway Fanatic, Veteran Motorist

## discarded — @discarded only fires via cycling path
57 card(s):
- **mtg.txt** (14): Abyssal Nocturnus, Confessor, Geth's Grimoire, Liliana's Caress, Megrim, Nath of the Gilt-Leaf, Nezumi Shortfang, Quest for the Nihil Stone, Sangromancer, Spirit Cairn, Telekinetic Bonds, Tolarian Winds, Waste Not, Wharf Infiltrator
- **borderline.txt** (42): Aclazotz, Deepest Betrayal, Asmoranomardicadaistinaculdacar, Bag of Holding, Bone Miser, Brallin, Skyshark Rider, Complicate, Conspiracy Theorist, Containment Construct, Cryptcaller Chariot, Cunning Survivor, Dying to Serve, Feast of Sanity, Fell Specter, Gilt-Blade Prowler, Glint-Horn Buccaneer, Grisly Survivor, Gutmorn, Pactbound Servant, Hollow One, Hostile Investigator, Inti, Seneschal of the Sun, Lazotep Chancellor, Library of Leng, Magmakin Artillerist, Marauding Mako, Mishra, Excavation Prodigy, Mystic Redaction, Nephalia Academy, Occult Epiphany, Ominous Sphinx, Orvar, the All-Form, Pitchstone Wall, Pyre of the World Tree, Raiders' Wake, Rielle, the Everwise, Scrounging Skyray, Shipwreck Sifters, Spiritual Focus, Surly Badgersaur, Tergrid, God of Fright, The Raven Man, Tourach, Dread Cantor, Urza, Powerstone Prodigy
- **planeswalkers.txt** (1): Chandra Ablaze

## nomaxhand — (opp)nomaxhand cleared when beneficiary battlefield empty
31 card(s):
- **mtg.txt** (9): Anvil of Bogardan, Enter the Infinite, Graceful Adept, Praetor's Counsel, Price of Knowledge, Reliquary Tower, Spellbook, Thought Vessel, Venser's Journal
- **borderline.txt** (18): Ancient Silver Dragon, Body of Knowledge, Curiosity Crafter, Decanter of Endless Water, Finale of Revelation, Folio of Fancies, Kruphix, God of Horizons, Library of Leng, Marina Vendrell's Grimoire, Morska, Undersea Sleuth, Nezahal, Primal Tide, Niv-Mizzet, Visionary, Sea Gate Restoration, The Great Synthesis, The Magic Mirror, Tishana, Voice of Thunder, Triskaidekaphile, Wizard Class
- **planeswalkers.txt** (4): Mordenkainen, Tamiyo, Seasoned Scholar, Tamiyo, the Moon Sage, Wrenn and Seven

## grant — grant: {0}-cost targeted grant fails to bind
26 card(s):
- **mtg.txt** (22): Amber Prison, Ashnod's Battle Gear, Deserter's Quarters, Endoskeleton, Everglove Courier, Flamestick Courier, Flowstone Armor, Frightshroud Courier, Ghosthelm Courier, Hisoka's Guard, Ice Floe, Mana Leech, Mole Worms, Pearlspear Courier, Phyrexian Gremlins, Rust Tick, Sand Squid, Spirit Shield, Tawnos's Weaponry, Thalakos Dreamsower, Thran Weaponry, Zelyon Sword
- **borderline.txt** (4): Entrancing Lyre, Immovable Rod, Kill Switch, Whip Vine

## metalcraft — _METALCRAFT_ macro missing trailing space (affects concatenated uses)
No real cards use this form (synthetic/test-only or macro-definition issue).

## scavenge / eternalize — RESOLVED 2026-07-18: not bugs
Real cards carry the exile-self as the `{E}` cost token in their own activation
lines; the witnesses had omitted it. Both fixtures green and back in `_tests.txt`.
No card re-check needed.


## enlist — _ENLIST_ granted ability undrivable
10 card(s):
- **borderline.txt** (10): Argivian Cavalier, Balduvian Berserker, Barkweave Crusher, Benalish Faithbonder, Coalition Skyknight, Coalition Warbrute, Hexbane Tortoise, Keldon Flamesage, Linebreaker Baloth, Yavimaya Steelcrusher

## offering — offering alt-cost (suite-undrivable; verify after any alt-cost work)
5 card(s):
- **mtg.txt** (5): Patron of the Akki, Patron of the Kitsune, Patron of the Moon, Patron of the Nezumi, Patron of the Orochi

## suspend — suspend alt-cost (suite-undrivable; verify after any alt-cost work)
52 card(s):
- **mtg.txt** (35): Aeon Chronicler, Ancestral Vision, Benalish Commander, Corpulent Corpse, Deep-Sea Kraken, Detritivore, Divine Congregation, Durkwood Baloth, Duskrider Peregrine, Errant Ephemeron, Giant Dustwasp, Heroes Remembered, Infiltrator il-Kor, Ith, High Arcanist, Ivory Giant, Keldon Halberdier, Knight of Sursi, Living End, Lotus Bloom, Mindstab, Nantuko Shaman, Nihilith, Phthisis, Plunder, Restore Balance, Rift Bolt, Riftmarked Knight, Riftwing Cloudskate, Roiling Horror, Search for Tomorrow, Shade of Trokair, Shivan Meteor, Veiling Oddity, Viscerid Deepwalker, Wheel of Fate
- **borderline.txt** (17): Chronomantic Escape, Crashing Footfalls, Curse of the Cabal, Gaea's Will, Gargadon, Glimpse of Tomorrow, Greater Gargadon, Hypergenesis, Inevitable Betrayal, Knight of Old Benalia, Mox Tantalite, Petrified Plating, Profane Tutor, Resurgent Belief, Rift Sower, Shivan Sand-Mage, Sol Talisman
