# Wave-2 "scope" class — 35 cards. Verdicts for the oracle-drift sweep.
# Constraints honored (Wave-1 engine-gap ledger):
#   - No non-interactive TRIGGER payload converted to a targeted chooser.
#   - No card given two interactive-chooser ETB legs.
#   - target=player,planeswalker / target(player,planeswalker) parse correctly.
# Corpus idioms verified against primitives: [-token;white] filter, @damaged(planeswalker)
# leg (Dictate of the Twin Gods template), comma-OR in @movedTo(a,b|zone), target=creature|opponentbattlefield.

PROPOSALS = [

    # 1
    {"name": "Anafenza, the Foremost", "file": "mtg.txt", "verdict": "TEXT_ONLY",
     "old": "text=When Anafenza, the Foremost attacks, put a +1/+1 counter on another target tapped creature you control. -- If a creature card would be put into an opponent's graveyard from anywhere, exile it instead.",
     "new": "text=When Anafenza, the Foremost attacks, put a +1/+1 counter on another target tapped creature you control. -- If a nontoken creature an opponent owns would die or a creature card not on the battlefield would be put into an opponent's graveyard, exile that card instead.",
     "why": "Script's abilities=oppgcreatureexiler + attack counter already model current Oracle; only the display text kept the old broad-replacement wording.",
     "probe": None},

    # 2
    {"name": "Beasts of Bogardan", "file": "mtg.txt", "verdict": "SCRIPT_FIX",
     "old": "auto=aslongas(*[white]|opponentBattlefield) 1/1\ntext=Protection from red -- Beasts of Bogardan gets +1/+1 as long as an opponent controls a white permanent.",
     "new": "auto=aslongas(*[-token;white]|opponentBattlefield) 1/1\ntext=Protection from red -- Beasts of Bogardan gets +1/+1 as long as an opponent controls a nontoken white permanent.",
     "why": "Oracle now requires a NONTOKEN white permanent; the [-token;white] filter (256 corpus uses) excludes tokens faithfully.",
     "probe": "Opponent controls only a white TOKEN -> Beasts stays 3/3; opponent controls a nontoken white -> 4/4."},

    # 3
    {"name": "Concussive Bolt", "file": "mtg.txt", "verdict": "SCRIPT_FIX",
     "old": "target=player",
     "new": "target=player,planeswalker",
     "why": "2017 errata: 4 damage now to target player OR planeswalker; spell-level target= widening is the shipped Wave-1 idiom.",
     "probe": "Cast at an opposing planeswalker with 3+ artifacts out -> 4 loyalty removed."},
    {"name": "Concussive Bolt", "file": "mtg.txt", "verdict": "SCRIPT_FIX",
     "old": "text=Concussive Bolt deals 4 damage to target player. -- Metalcraft - If you control three or more artifacts, creatures that player controls can't block this turn.",
     "new": "text=Concussive Bolt deals 4 damage to target player or planeswalker. -- Metalcraft - If you control three or more artifacts, creatures controlled by that player or by that planeswalker's controller can't block this turn.",
     "why": "Text refresh accompanying the target widening.",
     "probe": None},

    # 4
    {"name": "Fruit of the First Tree", "file": "mtg.txt", "verdict": "SCRIPT_FIX",
     "old": "target=creature|mybattlefield",
     "new": "target=creature",
     "why": "Oracle is 'Enchant creature' (any creature); the |mybattlefield restriction wrongly limits the aura to your own creatures.",
     "probe": "Cast Fruit on an opponent's creature -> aura attaches; on its death controller of Fruit gains/draws X."},
    {"name": "Fruit of the First Tree", "file": "mtg.txt", "verdict": "SCRIPT_FIX",
     "old": "text=Enchant creature you control -- When enchanted creature dies, you gain X life and draw X cards, where X is its toughness.",
     "new": "text=Enchant creature -- When enchanted creature dies, you gain X life and draw X cards, where X is its toughness.",
     "why": "Text refresh accompanying the enchant-scope widening.",
     "probe": None},

    # 5
    {"name": "Harmless Assault", "file": "mtg.txt", "verdict": "TEXT_ONLY",
     "old": "text=Prevent all combat damage that would be dealt this turn.",
     "new": "text=Prevent all combat damage that would be dealt this turn by attacking creatures.",
     "why": "Script already restricts prevention to from(creature[attacking]); only the text dropped the 'by attacking creatures' clause.",
     "probe": None},

    # 6
    {"name": "Iroas, God of Victory", "file": "mtg.txt", "verdict": "TEXT_ONLY",
     "old": "text=Indestructible -- As long as your devotion to red and white is less than seven, Iroas isn't a creature. -- Creatures you control can't be blocked except by two or more creatures. -- Prevent all damage that would be dealt to attacking creatures you control.",
     "new": "text=Indestructible -- As long as your devotion to red and white is less than seven, Iroas isn't a creature. -- Creatures you control have menace. -- Prevent all damage that would be dealt to attacking creatures you control.",
     "why": "Script already grants menace (lord ... menace); menace IS 'can't be blocked except by two or more', so only the text is pre-errata wording.",
     "probe": None},

    # 7
    {"name": "Magus of the Will", "file": "mtg.txt", "verdict": "TEXT_ONLY",
     "old": "text={2}{B}, {T}, Exile Magus of the Will: Until end of turn, you may play cards from your graveyard. If a card would be put into your graveyard from anywhere this turn, exile that card instead.",
     "new": "text={2}{B}, {T}, Exile Magus of the Will: Until end of turn, you may play lands and cast spells from your graveyard. If a card would be put into your graveyard from anywhere this turn, exile that card instead.",
     "why": "'play cards' and 'play lands and cast spells' are the SAME set (every card is a land or a spell); canPlayFromGraveyard is faithful, text is old templating.",
     "probe": None},

    # 8
    {"name": "Obelisk of Undoing", "file": "mtg.txt", "verdict": "OK_AS_IS",
     "why": "target(*|myBattlefield) already restricts to permanents you control (the normal own+control set); no owner-filter token exists in the DSL to also express 'you own', and the control-not-own edge is unreachable in 1v1 without engine work. Text 'you own and control' already matches Oracle."},

    # 9
    {"name": "Aspect of Lamprey", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=Enchant creature -- When Strands of Undeath enters, target player discards two cards. -- {B}: Regenerate enchanted creature.",
     "new": "text=Enchant creature you control -- When Aspect of Lamprey enters, target opponent discards two cards. -- Enchanted creature has lifelink.",
     "why": "Script already is target=creature|myBattlefield + ETB target(opponent) discard-2 + teach(creature) lifelink = current Oracle; the text is a leftover from a different card (Strands of Undeath).",
     "probe": None},

    # 10
    {"name": "Burn at the Stake", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=As an additional cost to cast Burn at the Stake, tap any number of untapped creatures you control. -- Burn at the Stake deals damage to target creature or player equal to three times the number of creatures tapped this way.",
     "new": "text=As an additional cost to cast Burn at the Stake, tap any number of untapped creatures you control. -- Burn at the Stake deals damage to any target equal to three times the number of creatures tapped this way.",
     "why": "Script already uses target=anytarget; only the text kept 'target creature or player'.",
     "probe": None},

    # 11
    {"name": "Davriel's Withering", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "target=creature",
     "new": "target=creature|opponentbattlefield",
     "why": "Oracle restricts the target to a creature an opponent controls; |opponentbattlefield is the established restriction idiom.",
     "probe": "Attempt to target your own creature -> not a legal target; opponent's creature -> perpetual -1/-2."},
    {"name": "Davriel's Withering", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "text=Target creature perpetually gets -1/-2.",
     "new": "text=Target creature an opponent controls perpetually gets -1/-2.",
     "why": "Text refresh accompanying the target restriction.",
     "probe": None},

    # 12
    {"name": "Furnace of Rath", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "auto=@damaged(player) from(*[-Furnace of Rath]|*):damage:thatmuch all(trigger[to])",
     "new": "auto=@damaged(player) from(*[-Furnace of Rath]|*):damage:thatmuch all(trigger[to])\nauto=@damaged(planeswalker) from(*[-Furnace of Rath]|*):damage:thatmuch all(trigger[to])",
     "why": "Oracle is 'permanent or player'; @damaged(planeswalker) is a supported leg (Dictate of the Twin Gods = identical Oracle, has all three legs) so add the missing planeswalker doubling.",
     "probe": "Deal 2 to an opposing planeswalker with Furnace out -> 4 loyalty lost."},
    {"name": "Furnace of Rath", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "text=If a source would deal damage to a creature or player, it deals double that damage to that creature or player instead.",
     "new": "text=If a source would deal damage to a permanent or player, it deals double that damage to that permanent or player instead.",
     "why": "Text refresh accompanying the planeswalker leg.",
     "probe": None},

    # 13
    {"name": "Haldan, Avid Arcanist", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=Partner with Pako, Arcane Retriever (When this creature enters, target player may put Pako into their hand from their library, then shuffle.) -- You may play noncreature cards from exile with fetch counters on them if you exiled them, and you may spend mana as though it were mana of any color to cast those spells.",
     "new": "text=Partner with Pako, Arcane Retriever (When this creature enters, target player may put Pako into their hand from their library, then shuffle.) -- You may play lands and cast noncreature spells from among cards you exiled that have fetch counters on them, and you may spend mana as though it were mana of any color to cast those spells.",
     "why": "'noncreature cards' == 'lands and noncreature spells' (a noncreature card is a land or a noncreature spell); script's [-creature] filter is faithful, text is old wording.",
     "probe": None},

    # 14
    {"name": "Laelia, the Blade Reforged", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=Haste -- Whenever Laelia, the Blade Reforged attacks, exile the top card of your library. You may play that card this turn. -- Whenever a spell or ability you control exiles one or more cards from your library and/or your graveyard, put a +1/+1 counter on Laelia.",
     "new": "text=Haste -- Whenever Laelia, the Blade Reforged attacks, exile the top card of your library. You may play that card this turn. -- Whenever one or more cards are put into exile from your library and/or your graveyard, put a +1/+1 counter on Laelia.",
     "why": "Errata dropped the 'a spell or ability you control exiles' restriction; the script's @movedto(*|myexile) from(mylibrary,mygraveyard) already has NO restriction (matches new, broader Oracle). (Pre-existing per-card vs per-event counter-count nuance is unchanged by this drift.)",
     "probe": None},

    # 15
    {"name": "Nightveil Specter", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=Whenever Nightveil Specter deals combat damage to a player, that player exiles the top card of their library. -- You may play cards exiled with Nightveil Specter.",
     "new": "text=Flying -- Whenever Nightveil Specter deals combat damage to a player, that player exiles the top card of their library. -- You may play lands and cast spells from among cards exiled with Nightveil Specter.",
     "why": "'play cards' == 'play lands and cast spells'; canplayfromexile is faithful. Also restores the Flying line (abilities=flying) to text.",
     "probe": None},

    # 16
    {"name": "Pyromancer's Swath", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "auto=@damaged(creature|myBattlefield) from(*[instant;sorcery]|myzones):all(trigger[to]) damage:2\nauto=@damaged(creature|opponentBattlefield) from(*[instant;sorcery]|*):all(trigger[to]) damage:2",
     "new": "auto=@damaged(creature|myBattlefield) from(*[instant;sorcery]|myzones):all(trigger[to]) damage:2\nauto=@damaged(creature|opponentBattlefield) from(*[instant;sorcery]|*):all(trigger[to]) damage:2\nauto=@damaged(planeswalker|myBattlefield) from(*[instant;sorcery]|myzones):all(trigger[to]) damage:2\nauto=@damaged(planeswalker|opponentBattlefield) from(*[instant;sorcery]|*):all(trigger[to]) damage:2",
     "why": "Oracle is 'permanent or player'; add the two planeswalker legs mirroring the existing creature legs (supported @damaged(planeswalker) idiom).",
     "probe": "Your instant deals 3 to an opposing planeswalker with Swath out -> 5 loyalty lost."},
    {"name": "Pyromancer's Swath", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "text=If an instant or sorcery source you control would deal damage to a creature or player, it deals that much damage plus 2 to that creature or player instead. -- At the beginning of each end step, discard your hand.",
     "new": "text=If an instant or sorcery source you control would deal damage to a permanent or player, it deals that much damage plus 2 to that permanent or player instead. -- At the beginning of each end step, discard your hand.",
     "why": "Text refresh accompanying the planeswalker legs.",
     "probe": None},

    # 17
    {"name": "Sen Triplets", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=At the beginning of your upkeep, choose target opponent. This turn, that player can't cast spells or activate abilities and plays with their hand revealed. You may play cards from that player's hand this turn.",
     "new": "text=At the beginning of your upkeep, choose target opponent. This turn, that player can't cast spells or activate abilities and plays with their hand revealed. You may play lands and cast spells from that player's hand this turn.",
     "why": "'play cards' == 'play lands and cast spells'; script castcard(normal) target(*|opponenthand) is faithful, text is old wording.",
     "probe": None},

    # 18
    {"name": "Shadow of the Second Sun", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=Enchant player -- At the beginning of enchanted player's second main phase, there is an additional beginning phase after this phase. (The end step happens after the added untap, upkeep, and draw steps.)",
     "new": "text=Enchant player -- At the beginning of each of enchanted player's postcombat main phases, there is an additional beginning phase after this phase. (The end step happens after the added untap, upkeep, and draw steps.)",
     "why": "'second main phase' was renamed 'postcombat main phase' (same phase); script's secondmain trigger fires each turn and is faithful, text is old naming.",
     "probe": None},

    # 19
    {"name": "Soul of Shandalar", "file": "borderline.txt", "verdict": "ENGINE_BLOCKED",
     "why": "Oracle needs 'target player OR planeswalker' for the 3-damage leg PLUS a linked 'up to one creature that player or that planeswalker's controller controls' second target. The player leg is non-interactive (damage:3 opponent) and the two interactive legs would ride an &&-chain, which fizzles the second target (CS-012). Faithful widening needs a player-or-planeswalker interactive leg linked to a same-controller creature chooser -- no such primitive exists (same class as Wave-1 'linked-controller primitive missing')."},

    # 20
    {"name": "Tireless Angler", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "@movedTo(island|myBattlefield)",
     "new": "@movedTo(island,swamp|myBattlefield)",
     "why": "Oracle triggers on an Island OR Swamp entering; comma-OR in a @movedTo predicate is a supported idiom (@movedTo(instant,sorcery|mystack)) so one line covers both without a duplicated payload.",
     "probe": "A Swamp entering under your control -> draft menu fires; an Island still fires; neither double-fires."},
    {"name": "Tireless Angler", "file": "borderline.txt", "verdict": "SCRIPT_FIX",
     "old": "text=Whenever an Island enters under your control, draft a card from Tireless Angler's spellbook.",
     "new": "text=Whenever an Island or Swamp enters the battlefield under your control, draft a card from Tireless Angler's spellbook.",
     "why": "Text refresh accompanying the Swamp trigger.",
     "probe": None},

    # 21
    {"name": "Turn // Burn", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=Target creature loses all abilities and becomes a 0/1 red Weird until end of turn. // Burn deals 2 damage to target creature or player. -- Fuse (You may cast one or both halves of this card from your hand.)",
     "new": "text=Target creature loses all abilities and becomes a 0/1 red Weird until end of turn. // Burn deals 2 damage to any target. -- Fuse (You may cast one or both halves of this card from your hand.)",
     "why": "Both Burn legs already use target(anytarget); only the text kept 'target creature or player'.",
     "probe": None},

    # 22
    {"name": "Uba Mask", "file": "borderline.txt", "verdict": "TEXT_ONLY",
     "old": "text=If a player would draw a card, that player exiles that card face up instead. -- Each player may play cards they exiled with Uba Mask this turn.",
     "new": "text=If a player would draw a card, that player exiles that card face up instead. -- Each player may play lands and cast spells from among cards they exiled with Uba Mask this turn.",
     "why": "'play cards' == 'play lands and cast spells'; both players' canplayfromexile branches are faithful, text is old wording.",
     "probe": None},

    # 23
    {"name": "Nicol Bolas, Planeswalker", "file": "planeswalkers.txt", "verdict": "ENGINE_BLOCKED",
     "why": "The -9 needs '7 damage to target player OR planeswalker' AND the linked discard-7/sacrifice-7 to fall on 'that player or that planeswalker's controller' (via targetedplayer). No corpus card links target(player,planeswalker) to a targetedplayer payload, so widening the target risks the discard/sac legs no-opping against a planeswalker target (targetedplayer -> pw-controller resolution unverified). Faithfulness gate: needs verified targetedplayer resolution for a planeswalker target before conversion.",
     "probe": "Probe: does targetedplayer resolve to a planeswalker's controller? Point a target(player,planeswalker) discard payload at a pw and observe whether the controller discards."},

    # 24
    {"name": "Tamiyo, Field Researcher", "file": "planeswalkers.txt", "verdict": "TEXT_ONLY",
     "old": "text=+1: Choose up to two target creatures. Until your next turn, whenever either of those creatures deals combat damage, you draw a card. -- -2: Tap up to two target nonland permanents. They don't untap during their controller's next untap step. -- -7: Draw three cards. You get an emblem with \"You may cast nonland cards from your hand without paying their mana costs.\"",
     "new": "text=+1: Choose up to two target creatures. Until your next turn, whenever either of those creatures deals combat damage, you draw a card. -- -2: Tap up to two target nonland permanents. They don't untap during their controller's next untap step. -- -7: Draw three cards. You get an emblem with \"You may cast spells from your hand without paying their mana costs.\"",
     "why": "'cast spells' == 'cast nonland cards' (lands aren't spells); emblem's lord(*[-land]|myhand) zerocast is faithful, text is old wording.",
     "probe": None},

    # 25
    {"name": "Teferi Akosa of Zhalfir", "file": "planeswalkers.txt", "verdict": "OK_AS_IS",
     "why": "The -3 X-ladder (tap 0..20 creatures -> shuffle target nonland permanent an opponent controls with mana value <=X) faithfully models 'Tap any number... X = number tapped', matching the Burn-at-the-Stake ladder idiom; target(*[-land&manacost<=N]|opponentbattlefield) matches 'nonland permanent an opponent controls'. Judge reason ('X undefined') is spurious; text already matches Oracle."},

    # 26-35: unsupported.txt stubs (no auto=/target= script present)
    {"name": "Benevolent Unicorn", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub, no script to fix."},
    {"name": "Chaos Orb", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub (physical-dexterity card), no script to fix."},
    {"name": "Divine Presence", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub, no script to fix."},
    {"name": "Equal Treatment", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub, no script to fix."},
    {"name": "Framed!", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub (choose-an-artist), no script to fix."},
    {"name": "Ghosts of the Innocent", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub, no script to fix."},
    {"name": "Naar Isle", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub (Plane), no script to fix."},
    {"name": "Rage Extractor", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub, no script to fix."},
    {"name": "Stronghold Furnace", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub (Plane), no script to fix."},
    {"name": "X", "file": "unsupported.txt", "verdict": "UNSUPPORTED_STUB",
     "why": "unsupported.txt: text-only stub, no script to fix."},
]
