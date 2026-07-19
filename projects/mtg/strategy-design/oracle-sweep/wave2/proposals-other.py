# Wave-2 "other" class oracle-drift proposals (27 cards).
# Verdicts: OK_AS_IS / TEXT_ONLY / SCRIPT_FIX / ENGINE_BLOCKED / UNSUPPORTED_STUB
# old/new present only for TEXT_ONLY and SCRIPT_FIX; old is byte-exact and unique
# within the card's [card] block. Multi-line old/new edit adjacent auto+text lines
# together so each SCRIPT_FIX carries its text= refresh. Text sanitized to ASCII
# (em-dash -> " -- ", bullet -> "*", curly apostrophe -> "'").
#
# KEY CROSS-CUTTING FINDING: several "completely different card" flags are the
# SWEEP mis-identifying the card (its "CURRENT ORACLE" pulled a joke/alternate or
# split-half printing that merely shares the name). In each such case the corpus
# SCRIPT is faithful to the REAL mainline card -> OK_AS_IS, brief-Oracle spurious.
# Verified vs Gatherer/Scryfall: Blood token vs Flesh//Blood; Fast//Furious MH2
# #123 vs the "Unknown Event" fuse joke; Red Herring MKM #142 (Clue Fish) vs the
# exchange-version; Pick Your Poison MKM #170 (each opponent sacrifices) vs the
# hallucinated "modes total 4" text; Day/Night = internal daybound/nightbound
# mechanic emblems, not playable cards.

PROPOSALS = [

    # ---- TEXT_ONLY (6) ----
    {
        "name": "Akoum Boulderfoot",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Defender -- When Akoum Boulderfoot enters, it deals 1 damage to any target.",
        "new": "text=When Akoum Boulderfoot enters, it deals 1 damage to any target.",
        "why": "Script (auto=damage:1 target(anytarget), no abilities=defender) is faithful; only the text= display line falsely prepends 'Defender'. Real Akoum Boulderfoot has no Defender.",
        "probe": "Cast Akoum Boulderfoot; assert it can be declared as an attacker (no Defender) and ETB deals 1 to a chosen target.",
    },
    {
        "name": "Merfolk Skyscout",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": (
            "auto=@combat(blocking) source(this):untap target(*)\n"
            "text=Merchant Ship can't attack unless defending player controls an Island. -- Whenever Merchant Ship attacks and isn't blocked, you gain 2 life. -- When you control no Islands, sacrifice Merchant Ship."
        ),
        "new": (
            "auto=@combat(blocking) source(this):untap target(*)\n"
            "text=Flying -- Whenever Merfolk Skyscout attacks or blocks, untap target permanent."
        ),
        "why": "Script (flying + untap target on attack/block, 2/3) matches real Merfolk Skyscout Oracle exactly (verified Scryfall ROE/CN2). The text= is a leftover from a different card, Merchant Ship.",
        "probe": "Attack (and separately block) with Merfolk Skyscout; each event offers untap-target-permanent on a tapped permanent.",
    },
    {
        "name": "Suncrusher",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": (
            "auto={C(1/1,-1)}{2}:moveTo(hand)\n"
            "text=Sunburst (This enters with a +1/+1 counter on it for each color of mana spent to cast it.) -- Remove two +1/+1 counters from Sawtooth Thresher: Sawtooth Thresher gets +4/+4 until end of turn."
        ),
        "new": (
            "auto={C(1/1,-1)}{2}:moveTo(hand)\n"
            "text=Sunburst (This creature enters with a +1/+1 counter on it for each color of mana spent to cast it.) -- {4}, {T}, Remove a +1/+1 counter from Suncrusher: Destroy target creature. -- {2}, Remove a +1/+1 counter from Suncrusher: Return Suncrusher to its owner's hand."
        ),
        "why": "Script (sunburst counter + {4}{T}remove:destroy + {2}remove:return-to-hand) matches real Suncrusher (Fifth Dawn). The text= describes an unrelated card, Sawtooth Thresher.",
        "probe": "Cast Suncrusher with N colors; assert N counters; activate destroy leg and hand-return leg, each removing a counter.",
    },
    {
        "name": "Land Cap",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Land Cap doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from Land Cap. -- {T}: Add {W} or {B}. Put a depletion counter on Land Cap.",
        "new": "text=Land Cap doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from Land Cap. -- {T}: Add {W} or {U}. Put a depletion counter on Land Cap.",
        "why": "Script already produces {W} and {U} (two auto={T}:Add lines). Only the text= reads a stale '{W} or {B}'.",
        "probe": "Tap Land Cap; assert the two mana options are {W} and {U} and a Depletion counter is placed.",
    },
    {
        "name": "River Delta",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=River Delta doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from River Delta. -- {T}: Add {W} or {B}. Put a depletion counter on River Delta.",
        "new": "text=River Delta doesn't untap during your untap step if it has a depletion counter on it. -- At the beginning of your upkeep, remove a depletion counter from River Delta. -- {T}: Add {U} or {B}. Put a depletion counter on River Delta.",
        "why": "Script already produces {U} and {B} (two auto={T}:Add lines). Only the text= reads a stale '{W} or {B}'.",
        "probe": "Tap River Delta; assert the two mana options are {U} and {B} and a Depletion counter is placed.",
    },
    {
        "name": "Mire's Grasp",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Enchant creature -- Enchanted creature gets -2/-2.",
        "new": "text=Enchant creature -- Enchanted creature gets -3/-3.",
        "why": "Script auto=-3/-3 already matches the Oracle magnitude; the text= line is the only thing that still says -2/-2.",
        "probe": "Enchant a 4/4 with Mire's Grasp; assert it becomes 1/1 (-3/-3).",
    },

    # ---- SCRIPT_FIX (8) ----
    {
        "name": "Enatu Golem",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto=_DIES_life:3\n"
            "text=When Enatu Golem goes to the graveyard from the battlefield, you gain 3 life."
        ),
        "new": (
            "auto=_DIES_life:4\n"
            "text=When Enatu Golem dies, you gain 4 life."
        ),
        "why": "Life gain drifted: Oracle is 4, script pays 3. _DIES_ macro expands to @movedTo(this|graveyard) from(battlefield); only the payload amount is wrong.",
        "probe": "Kill Enatu Golem; assert its controller gains 4 life.",
    },
    {
        "name": "Lorescale Coatl",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto=@drawof(player):may counter(1/1)\n"
            "text=Whenever you draw a card, you may put a +1/+1 counter on Lorescale Coatl."
        ),
        "new": (
            "auto=@drawof(player):counter(1/1)\n"
            "text=Whenever you draw a card, put a +1/+1 counter on Lorescale Coatl."
        ),
        "why": "Oracle is MANDATORY ('put'), script has an optional 'may'. Drop 'may' so the counter is not skippable. (Mandatory +1/+1 is the corpus-common form for this trigger.)",
        "probe": "Draw a card with Lorescale Coatl in play; assert a +1/+1 counter is placed with no may-prompt.",
    },
    {
        "name": "Awakening Zone",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto=@each my upkeep:_ELDRAZISPAWN_\n"
            "text=At the beginning of your upkeep, put a 0/1 colorless Eldrazi Spawn creature token onto the battlefield. It has \"Sacrifice this creature: Add {1}.\""
        ),
        "new": (
            "auto=@each my upkeep:may _ELDRAZISPAWN_\n"
            "text=At the beginning of your upkeep, you may create a 0/1 colorless Eldrazi Spawn creature token. It has \"Sacrifice this token: Add {C}.\""
        ),
        "why": "Oracle is optional ('you may create', verified Scryfall); script is mandatory. Add 'may' (precedent: @each my upkeep:may _BEASTTOKEN_/_SPIRITTOKEN_). Token mana is already correct: the _ELDRAZISPAWN_ macro already grants '{S}:Add{C}', so no macro edit needed.",
        "probe": "Upkeep with Awakening Zone in play; assert a may-prompt precedes the 0/1 Eldrazi Spawn, and the token's sac ability adds {C}.",
    },
    {
        "name": "Odric, Lunarch Marshal",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto=@each combatbegins restriction{type(*[vigilance]|mybattlefield)~morethan~0}:all(creature|mybattlefield) vigilance ueot\n"
            "text=at the beginning of each combat, creatures you control gain first strike until end of turn if you control a creature with first strike, the same is true for lying, deathtouch, double strike, haste, hexproof, indestructible, lifelink, reach, trample, and vigilance"
        ),
        "new": (
            "auto=@each combatbegins restriction{type(*[vigilance]|mybattlefield)~morethan~0}:all(creature|mybattlefield) vigilance ueot\n"
            "auto=@each combatbegins restriction{type(*[skulk]|mybattlefield)~morethan~0}:all(creature|mybattlefield) skulk ueot\n"
            "text=At the beginning of each combat, creatures you control gain first strike until end of turn if a creature you control has first strike. The same is true for flying, deathtouch, double strike, haste, hexproof, indestructible, lifelink, menace, reach, skulk, trample, and vigilance."
        ),
        "why": "Script already covers menace + 11 keywords but omits skulk, which the current Oracle lists. Add a skulk line mirroring the other 12 (skulk is a supported grantable keyword: 'lord(...) skulk', 'teach(creature) skulk'). Non-regressive: if the [skulk] filter fails to resolve, the line safely no-ops. Also fixes the text= typo 'lying' and the omitted menace/skulk.",
        "probe": "Combat begins while you control a creature with skulk; assert all your creatures gain skulk until end of turn (and menace still propagates).",
    },
    {
        "name": "Chicken Egg",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto=@dierolled(this) result(6) from(controller):all(this) sacrifice && token(Giant Chicken,Creature Giant Chicken,4/4,red) myupkeeponly\n"
            "text=During your upkeep, roll a six-sided die. On a 6, sacrifice Chicken Egg and put a Giant Chicken token into play. Treat this token as a 4/4 red creature that counts as a Chicken."
        ),
        "new": (
            "auto=@dierolled(this) result(6) from(controller):all(this) sacrifice && token(Giant Bird,Creature Giant Bird,4/4,red) myupkeeponly\n"
            "text=At the beginning of your upkeep, roll a six-sided die. If you roll a 6, sacrifice Chicken Egg and create a 4/4 red Giant Bird creature token."
        ),
        "why": "Current Oracle re-templates the token to a 4/4 red 'Giant Bird' (verified Scryfall UGL #41); script still makes a 'Giant Chicken'. Change token name/type only; the roll-and-sacrifice mechanic is intact.",
        "probe": "Force a 6 on Chicken Egg's upkeep roll (seed the fixture); assert it is sacrificed and a 4/4 red Giant Bird token enters.",
    },
    {
        "name": "Mana Screw",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto={1}:flipacoin winability add{2} winabilityend flipend\n"
            "text={1}: Flip a coin. If you win the flip, add {2}. Play this ability only any time you could play an instant."
        ),
        "new": (
            "auto={1}:flipacoin winability add{C}{C} winabilityend flipend\n"
            "text={1}: Flip a coin. If you win the flip, add {C}{C}. Activate only as an instant."
        ),
        "why": "Oracle adds {C}{C} (two colorless); script adds {2} (generic, castable on anything). Add{C}{C} is well-supported in the corpus (29+ uses). The instant-speed restriction is a pre-existing minor gap left unchanged (no verified auto token; not the flagged drift).",
        "probe": "Activate Mana Screw and win the flip; assert exactly two {C} appear in the mana pool.",
    },
    {
        "name": "Weaver of Currents",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto={T}:Add{2}\n"
            "text={T}: Add {1}{1}."
        ),
        "new": (
            "auto={T}:Add{C}{C}\n"
            "text={T}: Add {C}{C}."
        ),
        "why": "Oracle produces {C}{C} (two colorless); script produces {2} (generic). {C} is supported. Distinct resource type: {C} can only pay generic/colorless costs, unlike the generic {2} approximation.",
        "probe": "Tap Weaver of Currents; assert exactly two {C} in the mana pool.",
    },
    {
        "name": "Soldevi Excavations",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": (
            "auto={T}:add{1}{U}\n"
            "text=If Soldevi Excavations would enter the battlefield, sacrifice an untapped Island instead. If you do, put Soldevi Excavations onto the battlefield. If you don't, put it into its owner's graveyard. -- {T}: Add {1}{U}. -- {1}, {T}: Scry 1."
        ),
        "new": (
            "auto={T}:add{C}{U}\n"
            "text=If Soldevi Excavations would enter the battlefield, sacrifice an untapped Island instead. If you do, put Soldevi Excavations onto the battlefield. If you don't, put it into its owner's graveyard. -- {T}: Add {C}{U}. -- {1}, {T}: Scry 1."
        ),
        "why": "Oracle mana is {C}{U}; script produces {1}{U} (generic instead of colorless). Only the mana leg drifts; the sac-an-Island ETB replacement and the scry ability are correct.",
        "probe": "Tap Soldevi Excavations; assert the pool gains {C} and {U} (not generic {1}).",
    },

    # ---- OK_AS_IS (6) ----  (brief 'CURRENT ORACLE' is a wrong-card/joke-printing lookup)
    {
        "name": "Blood",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "why": "Name collision. This card (type=Artifact, subtype=Blood) is the Innistrad Blood TOKEN and its script/text ({1},{T},Discard,Sacrifice: Draw a card) is faithful. The brief's 'Oracle' is the Blood half of the split card Flesh // Blood -- a different card that merely shares the name. No drift.",
        "probe": "n/a -- script matches the real Blood token; sweep Oracle is a name collision.",
    },
    {
        "name": "Fast // Furious",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "why": "Sweep pulled the wrong printing. Script + text match the real Modern Horizons 2 #123 Fast // Furious (Fast: discard then draw two; Furious: 3 damage to each creature without flying; a split card, NO fuse). The brief's 'Oracle' (haste / unblockable-except-Vehicles / +3/+0 / Fuse) is the unofficial 'Unknown Event' joke version. Faithful as-is.",
        "probe": "n/a -- script matches MH2 Fast // Furious; sweep Oracle is a different printing.",
    },
    {
        "name": "Red Herring",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "why": "Name collision. Script (Artifact Creature Clue Fish 2/2, haste, attacks-if-able, {2}+Sacrifice: draw) is faithful to the mainline Red Herring, Murders at Karlov Manor #142. The brief's 'Oracle' (exchange from hand with a permanent) is a different same-named card. No drift.",
        "probe": "n/a -- script matches MKM #142 Red Herring; sweep Oracle is a name collision.",
    },
    {
        "name": "Pick Your Poison",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "why": "Sweep Oracle is hallucinated. Real Pick Your Poison (MKM #170) IS 'Choose one -- each opponent sacrifices an artifact / an enchantment / a creature with flying', which the three choice lines and text= already implement faithfully. The brief's 'modes total exactly 4 / snake tokens / all creatures -2/-2' matches no real printing. No drift.",
        "probe": "n/a -- script matches real MKM Pick Your Poison; sweep Oracle is spurious.",
    },
    {
        "name": "Day",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "why": "Not a playable card. type=Emblem with backside=Night and the day/night tracking autos -- this is the engine's implementation of the daybound/nightbound day-night mechanic, not a spell. The brief's 'Oracle' ('creatures target player controls get +1/+1') is a spurious lookup. Do not touch the mechanic infrastructure.",
        "probe": "n/a -- internal day/night mechanic emblem, not a drift target.",
    },
    {
        "name": "Night",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "why": "Not a playable card. type=Emblem with backside=Day and the night-tracking autos -- the engine's day/night mechanic implementation. The brief's 'Oracle' ('target creature gets -1/-1') is a spurious lookup. Do not touch.",
        "probe": "n/a -- internal day/night mechanic emblem, not a drift target.",
    },

    # ---- UNSUPPORTED_STUB (7) ----
    {
        "name": "Curse of the Fire Penguin",
        "file": "borderline.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "Un-card (Unhinged) 'Curse of the Fire Penguin Creature' is played as an Aura that turns the enchanted creature into the penguin -- creature-cast-as-aura with in-place type/stat replacement has no faithful card-script construct. Current script (Penguin creature w/ inplaydeath+Trample) is a non-faithful placeholder; leave stubbed.",
        "probe": "n/a -- un-card mechanic (creature played as Aura) unsupported.",
    },
    {
        "name": "Checks and Balances",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "In unsupported.txt, no auto= script. Requires multiplayer politics (each opponent may discard to counter; 3+ player restriction) -- no faithful construct in the DSL. Stub stands.",
        "probe": "n/a -- multiplayer voting/discard-to-counter unsupported.",
    },
    {
        "name": "Dark Sphere",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "In unsupported.txt, no auto= script. 'Next time a source of your choice would deal damage to you this turn, prevent half, rounded down' is a one-shot source-locked half-prevention replacement -- no faithful construct (prevent:N is fixed-pooled, not source-chosen-fractional). Stub stands.",
        "probe": "n/a -- choose-a-source next-damage half-prevention unsupported.",
    },
    {
        "name": "Merseine",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "In unsupported.txt, no auto= script. Aura entering with 3 net counters + doesn't-untap-while-countered + 'pay enchanted creature's mana cost: remove a net counter' (cost equal to another object's mana cost, controller-restricted) has no faithful construct. Stub stands.",
        "probe": "n/a -- pay-enchanted-creature's-mana-cost activated cost unsupported.",
    },
    {
        "name": "Personal Incarnation",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "In unsupported.txt, no auto= script. Owner-restricted damage redirection ({0}: next 1 damage to this is dealt to owner instead) plus death-triggered 'owner loses half their life, rounded up' -- redirection-to-owner + owner-only activation have no faithful construct. Stub stands.",
        "probe": "n/a -- owner-locked damage redirect unsupported.",
    },
    {
        "name": "Thran Turbine",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "In unsupported.txt, no auto= script. 'You may add {C}{C}, this mana can't be spent to cast spells' -- restricted-use mana (spendable only on abilities/costs, not spells) has no faithful construct. Note: the brief's TEXT-A '{1} or {2}' is itself the stale pre-errata wording; current Oracle is {C}{C}. Stub stands.",
        "probe": "n/a -- 'can't be spent to cast spells' restricted mana unsupported.",
    },
    {
        "name": "Wall of Caltrops",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "why": "In unsupported.txt (only abilities=defender). The banding grant is conditional on the block state ('at least one OTHER Wall creature is blocking that creature AND no non-Wall creatures are blocking it') -- inspecting co-blockers' Wall-ness at block time has no faithful construct. Stub stands.",
        "probe": "n/a -- co-blocker-conditional banding grant unsupported.",
    },
]
