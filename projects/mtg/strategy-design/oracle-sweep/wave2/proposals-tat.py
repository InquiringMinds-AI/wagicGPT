# Wave-2 oracle-drift proposals: timing (21) + ability (6) + targetzone (4) = 31 cards
# Verdicts: OK_AS_IS / TEXT_ONLY / SCRIPT_FIX / ENGINE_BLOCKED / UNSUPPORTED_STUB
# `old` strings are byte-exact and unique within each card's block.
# SCRIPT_FIX entries fold the text= refresh into the same old/new span.
# Constraints honored (engine-bugs Wave 1): no non-interactive->chooser conversion in a
# trigger payload; no 2nd chooser-bearing ETB line; `may` = yes/no MayAbility (not a target
# chooser) is a live corpus idiom for optional triggers, NOT a blocked conversion.

PROPOSALS = [

    # ================= TIMING =================

    {
        "name": "Celestial Sword",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto={3}{T}:target(creature|mybattlefield) transforms((,newability[phaseaction[end once] bury],newability[3/3])) ueot
text={3}, {T}: Target creature you control gets +3/+3 until end of turn. Destroy it at the beginning of the next end step. A creature destroyed this way can't be regenerated.""",
        "new": """auto={3}{T}:target(creature|mybattlefield) transforms((,newability[phaseaction[end once] sacrifice],newability[3/3])) ueot
text={3}, {T}: Target creature you control gets +3/+3 until end of turn. Its controller sacrifices it at the beginning of the next end step.""",
        "why": "Oracle errata: 'destroy, can't be regenerated' -> 'its controller sacrifices it'. sacrifice != destroy (indestructible/regen/replacement differ). `bury`->`sacrifice` inside a phaseaction newability is a proven corpus form (mtg.txt:118220 newability[phaseaction[end once] sacrifice all(this)]).",
        "probe": "Pump a creature with a regeneration shield up; advance to next end step; assert it is sacrificed (shield does not save it).",
    },
    {
        "name": "Grizzled Wolverine",
        "file": "mtg.txt",
        "verdict": "ENGINE_BLOCKED",
        "old": "",
        "new": "",
        "why": "Oracle is now an ACTIVATED ability ('{R}: +2/+0; activate only during the declare blockers step, only if at least one creature is blocking this creature, only once each turn') -- a shape change from the current 'whenever blocked, may pay {R}' trigger. No card-script token gates an activated ability to the declare-blockers step or to 'only if this creature is blocked'; every such card (Icy Blast, Lesser Werewolf, etc.) lives in unsupported.txt. Existing triggered approximation left in place.",
        "probe": "",
    },
    {
        "name": "Homarid",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Homarid enters with a tide counter on it. -- At the beginning of your upkeep, put a tide counter on Homarid. -- As long as there is exactly one tide counter on Homarid, it gets -1/-1. -- As long as there are exactly three tide counters on Homarid, it gets +1/+1. -- Whenever there are four tide counters on Homarid, remove all tide counters from it.",
        "new": "text=Homarid enters with a tide counter on it. -- At the beginning of your upkeep, put a tide counter on Homarid. -- As long as there is exactly one tide counter on Homarid, it gets -1/-1. -- As long as there are exactly three tide counters on Homarid, it gets +1/+1. -- Whenever there are four or more tide counters on Homarid, remove all tide counters from it.",
        "why": "Judge flagged exactly-four vs four-or-more. The removal is upkeep-gated and counters only increment by 1/upkeep (added then checked same upkeep), so the threshold is reached at exactly 4 and never exceeded in normal play -- the distinction is unobservable and the script is functionally faithful. Only the display text is stale ('four' -> 'four or more'). A true continuous state-trigger for '>=4' is not cleanly supported.",
        "probe": "",
    },
    {
        "name": "Soul's Attendant",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto=@movedTo(other creature|battlefield):life:1
text=Whenever another creature enters, you gain 1 life.""",
        "new": """auto=@movedTo(other creature|battlefield):may life:1
text=Whenever another creature enters, you may gain 1 life.""",
        "why": "Oracle gained 'may' (optional). Add the `may` MayAbility prefix -- a live corpus idiom for optional triggers (borderline.txt:15412 `:may draw:1`). This is a yes/no prompt, not a target-chooser conversion, so the trigger-payload chooser gap does not apply.",
        "probe": "Another creature enters -> forced yes/no menu on Soul's Attendant (choice 0 gains 1 life, choice 1 declines with no life change).",
    },
    {
        "name": "World at War",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=After the first second main phase this turn, there's an additional combat phase followed by an additional main phase. At the beginning of that combat, untap all creatures that attacked this turn. -- Rebound (If you cast this spell from your hand, exile it as it resolves. At the beginning of your next upkeep, you may cast this card from exile without paying its mana cost.)",
        "new": "text=After the second main phase this turn, there's an additional combat phase followed by an additional main phase. At the beginning of that combat, untap all creatures that attacked this turn. -- Rebound (If you cast this spell from your hand, exile it as it resolves. At the beginning of your next upkeep, you may cast this card from exile without paying its mana cost.)",
        "why": "Script already schedules the extra phases `after<postbattle>` = after the second (postcombat) main phase -- Oracle-correct. Only the text= carries a garbled 'first second main phase' artifact; corrected to 'second main phase'.",
        "probe": "",
    },
    {
        "name": "Base Camp",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text={T}: Add {C}. -- {T}: Add one mana of any color. Spend this mana only to cast a Cleric, Rogue, Warrior, or Wizard spell, or to activate an ability of a Cleric, Rogue, Warrior, or Wizard.",
        "new": "text=This land enters tapped. -- {T}: Add {C}. -- {T}: Add one mana of any color. Spend this mana only to cast a Cleric, Rogue, Warrior, or Wizard spell, or to activate an ability of a Cleric, Rogue, Warrior, or Wizard.",
        "why": "The 'enters tapped' clause the judge flagged is already implemented in script via `auto=tap(noevent)`. Only the display text omitted it; prepend 'This land enters tapped.'",
        "probe": "",
    },
    {
        "name": "Blind Fury",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": """restriction=before battle damage
auto=all(creature) transforms((,newability[-trample],newability[@combatdamaged(creature) from(this):damage:thatmuch all(trigger[to])])) ueot
text=Cast Blind Fury only before the combat damage step. -- All creatures lose trample until end of turn. If a creature would deal combat damage to a creature this turn, it deals double that damage to that creature instead.""",
        "new": """auto=all(creature) transforms((,newability[-trample],newability[@combatdamaged(creature) from(this):damage:thatmuch all(trigger[to])])) ueot
text=All creatures lose trample until end of turn. If a creature would deal combat damage to a creature this turn, it deals double that damage to that creature instead.""",
        "why": "Current Oracle (verified via Gatherer) dropped the 'only before the combat damage step' cast restriction -- that was earlier-printing wording. Remove the `restriction=before battle damage` line and refresh text=. Double-damage grant unchanged.",
        "probe": "Cast Blind Fury during/after the combat damage step (now legal); trample removed, double combat damage still applied.",
    },
    {
        "name": "Expedition Supplier",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto=@movedTo(other *[human]|myBattlefield) turnlimited:name(Conjure a card) conjure cards(Utility Knife) zone(mybattlefield)
text=Whenever Expedition Supplier or another Human enters under your control, conjure a card named Utility Knife onto the battlefield. This ability triggers only once each turn.""",
        "new": """auto=@movedTo(other *[human;warrior]|myBattlefield) turnlimited:name(Conjure a card) conjure cards(Utility Knife) zone(mybattlefield)
text=Whenever one or more Humans and/or Warriors you control enter, conjure a card named Utility Knife onto the battlefield. This ability triggers only once each turn.""",
        "why": "Oracle triggers on Humans AND/OR Warriors (script only saw Humans). Add Warriors via the OR-type form `[human;warrior]` (corpus borderline.txt:19341 `*[blue;black]`). Kept `other` deliberately -- the safe change addressing the flagged drift. RESIDUAL: Oracle also counts Expedition Supplier's own ETB (it is a Human Warrior you control); dropping `other` to catch self-entry is not verifiable without running and risks a regression, so left for a follow-up probe.",
        "probe": "Play a non-Human Warrior -> conjure fires. Play a second Human same turn -> no second conjure (turnlimited). (Follow-up: does dropping `other` make Expedition Supplier's own ETB conjure?)",
    },
    {
        "name": "Inquisitor Captain",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "old": "",
        "new": "",
        "why": "Script core is faithful: `~morethan~19` = 20-or-more creature cards mv<=3 across graveyard+hand+library (`mynonplaynonexile`), seek two mv<=3 creatures, put one onto battlefield, shuffle the other in. The only drift the judge flagged is the added intervening-if 'if you cast it', which has NO card-script construct -- the corpus universally carries 'if you cast it' in text= only and drops it in script (Arcane Proxy, Bortuk Bonerattle, etc.). No faithful improvement available; only relevant on non-cast ETB (blink/reanimate).",
        "probe": "",
    },
    {
        "name": "Lanterns' Lift",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": """target=creature
text=Enchant creature -- Enchanted creature gets +1/+1 and has flying. // Lantern Bearer""",
        "new": """target=creature
auto=teach(creature) 1/1
auto=teach(creature) flying
text=Enchant creature -- Enchanted creature gets +1/+1 and has flying. -- If Lanterns' Lift would be put into a graveyard from anywhere, exile it instead. // Lantern Bearer""",
        "why": "The judge focused on the exile clause (already handled by `abilities=exiledeath`), but the real bug is the aura's ENTIRE buff was missing -- no auto line granted +1/+1 or flying (compare sibling Ancestor's Embrace `auto=teach(creature) lifelink`). Add two teach lines (no combined P/T+flying teach form exists in the corpus; `teach(creature) flying` verified at borderline.txt:965). Text refreshed with the exile clause too.",
        "probe": "Attach Lanterns' Lift to a 2/2 -> it becomes 3/3 with flying. Send Lanterns' Lift to a graveyard -> exiled instead.",
    },
    {
        "name": "Millicent, Restless Revenant",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto=@damaged(player) from(spirit[-token]|myBattlefield):name(Create Spirit) _SPIRITTOKEN_
auto=@movedto(spirit[-token]|graveyard) from(myBattlefield):name(Create Spirit) _SPIRITTOKEN_
text=This spell costs {1} less to cast for each Spirit you control. -- Flying -- Whenever Millicent, Restless Revenant or another nontoken Spirit you control dies or deals damage to a player, create a 1/1 white Spirit creature token with flying.""",
        "new": """auto=@combatdamaged(player) from(spirit[-token]|myBattlefield):name(Create Spirit) _SPIRITTOKEN_
auto=@movedto(spirit[-token]|graveyard) from(myBattlefield):name(Create Spirit) _SPIRITTOKEN_
text=This spell costs {1} less to cast for each Spirit you control. -- Flying -- Whenever Millicent, Restless Revenant or another nontoken Spirit you control dies or deals combat damage to a player, create a 1/1 white Spirit creature token with flying.""",
        "why": "Oracle says 'deals COMBAT damage to a player'; script used `@damaged(player)` which also fires on noncombat (spell/ability) damage. Narrow to `@combatdamaged(player)`. Dies leg unchanged.",
        "probe": "A nontoken Spirit deals noncombat damage to a player -> no token. Same Spirit deals combat damage -> token. Spirit dies -> token.",
    },
    {
        "name": "Oglor, Devoted Assistant",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto=@movedto(creature|mygraveyard) from(mylibrary):name(Gains ability) all(trigger[to]) name(Gains ability) counter(0/0.1.PerpetualZombie) notrg
auto=emblem transforms((,newability[@movedto(*|myzones) from(mygraveyard):name(Create zombie) all(trigger[to]) name(Create zombie) transforms((,newability[if compare(hascntperpetualzombie)~morethan~0 then _ZOMBIETOKEN_ and!( tap(noevent) )!])) oneshot])) forever dontremove
text=At the beginning of your upkeep, look at the top two cards of your library, then put one of them into your graveyard. -- Whenever a creature card is put into your graveyard from your library, it perpetually gains "When this card leaves your graveyard, create a 2/2 tapped black Zombie creature token.\"""",
        "new": """auto=@movedto(creature|mygraveyard) from(mylibrary,myhand):name(Gains ability) all(trigger[to]) name(Gains ability) counter(0/0.1.PerpetualZombie) notrg
auto=emblem transforms((,newability[@movedto(*|myzones) from(mygraveyard):name(Create zombie) all(trigger[to]) name(Create zombie) transforms((,newability[if compare(hascntperpetualzombie)~morethan~0 then _ZOMBIETOKEN_ and!( tap(noevent) )!])) oneshot])) forever dontremove
text=At the beginning of your upkeep, look at the top two cards of your library, then put one of them into your graveyard. -- Whenever a creature card is put into your graveyard from your library or hand, it perpetually gains "When this card leaves your graveyard, create a 2/2 tapped black Zombie creature token.\"""",
        "why": "Oracle source zone is 'from your library OR HAND'; script only listened to `from(mylibrary)`. Multi-zone `from(...)` is valid (corpus borderline.txt:30577 `from(battlefield,library)`), so widen to `from(mylibrary,myhand)`. (The perpetual-gain / zombie machinery is left untouched.)",
        "probe": "Discard a creature card from hand -> it gains the 'leaves graveyard -> tapped 2/2 Zombie' perpetual trigger, same as a creature milled from library.",
    },
    {
        "name": "Pirated Copy",
        "file": "borderline.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto=may name(Copy creature) notaTarget(creature|battlefield) copy and!( transforms((,newability[@combatdamaged(player) from(creature[share!name!]|battlefield):may name(Draw a card) draw:1 controller])) forever )!
text=You may have Pirated Copy enter the battlefield as a copy of any creature on the battlefield, except it's a Pirate in addition to its other types and it has "Whenever this creature or another creature with the same name deals combat damage to a player, you may draw a card.\"""",
        "new": """auto=may name(Copy creature) notaTarget(creature|battlefield) copy and!( transforms((,newability[@combatdamaged(player) from(creature[share!name!]|battlefield):name(Draw a card) draw:1 controller])) forever )!
text=You may have Pirated Copy enter the battlefield as a copy of any creature on the battlefield, except it's a Pirate in addition to its other types and it has "Whenever this creature or another creature with the same name deals combat damage to a player, you draw a card.\"""",
        "why": "Oracle's granted trigger is now MANDATORY ('you draw a card'); script had `may` on the inner draw. Remove that inner `may` only. The OUTER `may` (optionally enter as a copy) is correct per Oracle ('You may have ... enter ... as a copy') and stays.",
        "probe": "Copy a creature; a same-named creature deals combat damage to a player -> forced draw with no yes/no menu.",
    },
    {
        "name": "Town-razer Tyrant",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": 'text=Flying -- When Town-Razer Tyrant enters, target nonbasic land you don\'t control loses all abilities except mana abilities and gains "At the beginning of your end step, this permanent deals 2 damage to you unless you sacrifice it."',
        "new": 'text=Flying -- When Town-Razer Tyrant enters, target nonbasic land you don\'t control loses all abilities except mana abilities and gains "At the beginning of your upkeep, this permanent deals 2 damage to you unless you sacrifice it."',
        "why": "Script grants the land ability with `@each my upkeep` -- already Oracle-correct (upkeep). Only the display text said 'end step'; corrected to 'upkeep'.",
        "probe": "",
    },
    {
        "name": "Twilight Prophet",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Ascend (If you control ten or more permanents, you get the city's blessing for the rest of the game.) -- At the beginning of your upkeep, if you have the city's blessing, reveal the top card of your library and put it into your hand. Each opponent loses X life and you gain X life, where X is that card's mana value.",
        "new": "text=Flying -- Ascend (If you control ten or more permanents, you get the city's blessing for the rest of the game.) -- At the beginning of your upkeep, if you have the city's blessing, reveal the top card of your library and put it into your hand. Each opponent loses X life and you gain X life, where X is that card's mana value.",
        "why": "Flying is already granted via `abilities=flying`; only the display text omitted it. Prepend 'Flying -- '.",
        "probe": "",
    },
    {
        "name": "Virtue of Persistence",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Adventre Locthwain Scorn Target creature gets -3/-3 until end of turn. You gain 2 life. At the beginning of your upkeep, put target creature card from a graveyard onto the battlefield under your control.",
        "new": "text=Adventure -- Locthwain Scorn {1}{B} Sorcery -- Target creature gets -3/-3 until end of turn. You gain 2 life. -- Virtue of Persistence -- At the beginning of your upkeep, put target creature card from a graveyard onto the battlefield under your control.",
        "why": "Judge saw only the enchantment-side Oracle and read the -3/-3 + gain-2-life as extra. They are the ADVENTURE half (Locthwain Scorn), correctly scripted as the `alternative ...` lines; Locthwain Scorn's Oracle is verified '-3/-3 until end of turn. You gain 2 life.' (Scryfall WOE #115). Script faithful; only the garbled 'Adventre ...' text= needed a clean rewrite.",
        "probe": "",
    },
    {
        "name": "Akki Lavarunner",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "old": "",
        "new": "",
        "why": "Flip card (Akki Lavarunner // Tok-Tok, Volcano Born) with no auto script -- a text-only stub in unsupported.txt. The 'flip it' mechanic and the flipped side's protection/damage-replacement are unimplemented.",
        "probe": "",
    },
    {
        "name": "Aretopolis",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "old": "",
        "new": "",
        "why": "Plane card (Planechase). No planar deck / planeswalk / chaos-ensues engine; text-only stub. The 'roll {K}' -> 'chaos ensues' errata is moot with no dice/plane mechanic implemented.",
        "probe": "",
    },
    {
        "name": "Kilnspire District",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "old": "",
        "new": "",
        "why": "Plane card (Planechase); no plane/chaos engine. Text-only stub; 'roll {K}' -> 'chaos ensues' errata moot.",
        "probe": "",
    },
    {
        "name": "Tember City",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "old": "",
        "new": "",
        "why": "Plane card (Planechase); no plane/chaos engine. Text-only stub; 'roll {K}' -> 'chaos ensues' errata moot.",
        "probe": "",
    },
    {
        "name": "Trail of the Mage-Rings",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "old": "",
        "new": "",
        "why": "Plane card (Planechase); no plane/chaos engine. Text-only stub; 'roll {K}' -> 'chaos ensues' errata moot.",
        "probe": "",
    },

    # ================= ABILITY =================

    {
        "name": "Abzan Battle Priest",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Outlast {W} ({W},{T}: Put a +1/+1 counter on this creature. Outlast only as a sorcery.) -- Each creature you control with a +1/+1 counter on it has first strike.",
        "new": "text=Outlast {W} ({W},{T}: Put a +1/+1 counter on this creature. Outlast only as a sorcery.) -- Each creature you control with a +1/+1 counter on it has lifelink.",
        "why": "Script already grants LIFELINK (`lord(creature[counter{1/1.1}]|mybattlefield) lifelink`) -- Oracle-correct. Only the display text still said 'first strike'.",
        "probe": "",
    },
    {
        "name": "Leyline of Vitality",
        "file": "mtg.txt",
        "verdict": "SCRIPT_FIX",
        "old": """auto=@movedTo(creature|mybattlefield):life:1
text=If Leyline of Vitality is in your opening hand, you may begin the game with it on the battlefield. -- Creatures you control get +0/+1. -- Whenever a creature enters under your control, you gain 1 life.""",
        "new": """auto=@movedTo(creature|mybattlefield):may life:1
text=If Leyline of Vitality is in your opening hand, you may begin the game with it on the battlefield. -- Creatures you control get +0/+1. -- Whenever a creature you control enters, you may gain 1 life.""",
        "why": "Oracle gained 'may' (optional life gain). Add the `may` MayAbility prefix (live corpus idiom for optional triggers). Trigger scope `creature|mybattlefield` = 'a creature you control' is correct.",
        "probe": "A creature you control enters -> yes/no menu (choice 0 gains 1 life, choice 1 declines).",
    },
    {
        "name": "Ancestor's Embrace",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Enchant creature -- Enchanted creature has lifelink. // Kindly Ancestor",
        "new": "text=Enchant creature -- Enchanted creature has lifelink. -- If Ancestor's Embrace would be put into a graveyard from anywhere, exile it instead. // Kindly Ancestor",
        "why": "The exile-instead-of-graveyard replacement the judge flagged is already implemented via `abilities=exiledeath` (and `auto=teach(creature) lifelink` grants lifelink). Judge reason spurious; only the display text omitted the exile clause.",
        "probe": "",
    },
    {
        "name": "Glorious Protector",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Flash -- Flying -- When Glorious Protector enters, you may exile any number of non-Angel creatures you control until Glorious Protector leaves the battlefield.",
        "new": "text=Flash -- Flying -- When Glorious Protector enters, you may exile any number of non-Angel creatures you control until Glorious Protector leaves the battlefield. -- Foretell {2}{W}",
        "why": "Foretell is already implemented (`abilities=foretell,flash,flying`, `autohand=_FORETELL_`, `autoexile=... castcard(alternative)`). Judge reason spurious; only the display text omitted 'Foretell {2}{W}'.",
        "probe": "",
    },
    {
        "name": "Necropolis Fiend",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=Flying -- Delve (You may exile any number of cards from your graveyard as you cast this spell. It costs {1} less to cast for each card exiled this way.)",
        "new": "text=Delve (Each card you exile from your graveyard while casting this spell pays for {1}.) -- Flying -- {X}, {T}, Exile X cards from your graveyard: Target creature gets -X/-X until end of turn.",
        "why": "The activated ability the judge said was missing IS present in script (`auto={X}{T}{E<X>(*|myGraveyard)}: target(creature) -X/-X ueot`), and Delve is handled via `other={delve}`. Judge reason spurious; the text= just never listed the activated ability. Refreshed to current Oracle.",
        "probe": "",
    },
    {
        "name": "Urza's Science Fair Project",
        "file": "borderline.txt",
        "verdict": "TEXT_ONLY",
        "old": "text={2}: Roll a six-sided die for Urza's Science Fair Project. -- 1 It gets -2/-2 until end of turn. -- 2 It deals no combat damage this turn. -- 3 Attacking does not cause it to tap this turn. -- 4 It gains first strike until end of turn. -- 5 It gains flying until end of turn. -- 6 It gets +2/+2 until end of turn.",
        "new": "text={2}: Roll a six-sided die. This creature gets the indicated result. -- 1 - It gets -2/-2 until end of turn. -- 2 - Prevent all combat damage it would deal this turn. -- 3 - It gains vigilance until end of turn. -- 4 - It gains first strike until end of turn. -- 5 - It gains flying until end of turn. -- 6 - It gets +2/+2 until end of turn.",
        "why": "Result 3 already grants VIGILANCE in script (`transforms((,vigilance,...))`) -- Oracle-correct; the text still carried the pre-errata 'attacking does not cause it to tap' wording. Result 2 also refreshed to Oracle's 'prevent all combat damage it would deal' (script `preventAllCombatDamage from(this)` already matches). Text-only refresh.",
        "probe": "",
    },

    # ================= TARGETZONE =================

    {
        "name": "Godtracker of Jund",
        "file": "mtg.txt",
        "verdict": "OK_AS_IS",
        "old": "",
        "new": "",
        "why": "Judge misread 'this creature'. On this triggered ability, 'put a +1/+1 counter on this creature' = the ability's SOURCE (Godtracker of Jund), not the entering creature (that would read 'that creature'). Script `counter(1/1)` with no target puts the counter on the source = correct. Original printing literally said 'on Godtracker of Jund'; the Oracle just templated the name to 'this creature'. No drift.",
        "probe": "",
    },
    {
        "name": "Yawgmoth's Agenda",
        "file": "mtg.txt",
        "verdict": "TEXT_ONLY",
        "old": "text=You can't cast more than one spell each turn. -- You may play cards from your graveyard. -- If a card would be put into your graveyard from anywhere, exile it instead.",
        "new": "text=You can't cast more than one spell each turn. -- You may play lands and cast spells from your graveyard. -- If a card would be put into your graveyard from anywhere, exile it instead.",
        "why": "Script grants `canPlayFromGraveyard` over `*` (ALL cards in graveyard), which the engine already extends to land plays (corpus: `lord(land|mygraveyard) canPlayFromGraveyard`, borderline.txt:2842,21478). So lands are already playable; only the display text said 'play cards' -- refreshed to 'play lands and cast spells'.",
        "probe": "",
    },
    {
        "name": "Kronch Wrangler",
        "file": "borderline.txt",
        "verdict": "OK_AS_IS",
        "old": "",
        "new": "",
        "why": "Same 'this creature' misread as Godtracker. 'Put a +1/+1 counter on this creature' = the source (Kronch Wrangler), not the entering creature. Script `counter(1/1)` on source = correct; original printing said 'on Kronch Wrangler'. No drift.",
        "probe": "",
    },
    {
        "name": "Oubliette",
        "file": "unsupported.txt",
        "verdict": "UNSUPPORTED_STUB",
        "old": "",
        "new": "",
        "why": "Current Oracle was errata'd to a PHASING effect ('target creature phases out until this enchantment leaves ... tap it as it phases in'); the stub text still describes the old exile-and-return version. No auto script, and phasing is unimplemented -- a genuine mechanic change on an unsupported card.",
        "probe": "",
    },

]
