# Wagic Card-Script Lexicon (ground-truth)

A complete reference for the Wagic / wagicGPT card-scripting DSL used in `projects/mtg/bin/Res/sets/primitives/*.txt` (the `mana=`, `auto=`, `target=`, `abilities=` fields and everything inside them).

**Provenance.** Extracted directly from the **parser source** — the authoritative ground truth — not from community docs:
- `src/MTGDeck.cpp` (`processConfLine` — the card-field dispatcher)
- `src/ManaCost.cpp` + `include/ExtraCost.h` / `src/ExtraCost.cpp` (mana + all costs)
- `src/MTGAbility.cpp` (`parseTrigger`, `parseMagicLine`, `getManaReduxAbility` — triggers + effects)
- `src/TargetChooser.cpp` (targets, zones, quantities, restrictions)
- `src/MTGDefinitions.cpp` / `include/MTGDefinitions.h` (the 240-entry basic-ability table + phase names)
- `include/AllAbilities.h` (the ability-class catalog that every keyword maps to)
- cross-checked against the 287k-line primitives corpus for real usage, and against the community wiki `docs/wiki/CardCode.md`.

> **The wiki is NOT authoritative.** `docs/wiki/CardCode.md` (frozen ~2015) documents roughly a third of the language and contains outright errors (documents ~90 of 240 basic abilities; misspells load-bearing keywords; claims `banding` was removed when it wasn't; presents `@combat()` traits as standalone `@attacking`/`@blocked` triggers that don't exist; omits `create()`, composite zones, quantity prefixes, the player-state trigger family, and the `ability$!…!$` / `and!(…)!` wrappers entirely). Each section below ends with a "wiki discrepancies" note. When the wiki and this lexicon disagree, **this lexicon (and the parser it cites) wins** — but the parser source itself is the final authority; verify `file:line` before relying on anything load-bearing.

---

## How to read this — SHAPE is the load-bearing dimension

The single most important property of any effect is its **shape / duration**, because that is what makes a card faithful (or subtly wrong) against its Oracle text. The same verb built with the wrong shape is a broken card. Every effect entry below is tagged with one of:

- **one-shot** — resolves once and is gone (`oneShot=1` at the call site). Most `AA*` effects. e.g. `damage:`, `draw:`, `destroy`, `token(`, `moveto(`, `prevent:N`.
- **continuous / static** — active for as long as the source is in the zone (a plain `MTGAbility` subclass). e.g. `lord(`, `becomes(` (on a permanent), P/T mods, basic-ability grants, `altercost`.
- **until-end-of-turn (UEOT)** — a continuous effect torn down at cleanup (`InstantAbility` / `*UEOT` / `*Instant`, or the `ueot` wrapper). Instants/sorceries default here unless `forever`/`uynt`/`uent` is set.
- **triggered** — fires on a `WEvent` (`@…` / `Tr*` classes). Re-fires **once per matching event**; a persistent trigger keeps firing all game.
- **replacement** — intercepts and replaces an event before it happens (`RE*`, e.g. draw replacement, mana-pool saver, flicker).
- **prevention** — a distinct category from replacement (MTG CR 615 ≠ 614). Damage prevention uses `AADamagePrevent` (`prevent:N`, one-shot pooled) / `APreventDamageTypes` (`preventall*`/`fog`, continuous), **not** the replacement system.

**Map Oracle phrasing → shape:** "as long as" → continuous/static · "until end of turn" → UEOT · "whenever / at / when" → triggered · "if … would … instead" → replacement · "prevent … damage" → prevention · a one-time verb on resolution ("draw two cards", "destroy target") → one-shot. Match the shape first, then the verb.

### Global parser gotchas (apply everywhere)
1. **Dispatch is substring `s.find()`, order-dependent.** Keywords are matched by containment, longest/most-specific first (e.g. `untap` before `tap`; `untapped(` before `tapped(`; `noncombatdamage` before `combatdamage` before `damage`). Broad matchers (`find("add")`, `find("out")`) can false-positive on substrings — a real bug surface.
2. **Everything is lower-cased before parsing.** Authoritative token forms are lowercase (`moveto`, not `moveTo`).
3. **The same keyword yields different classes by context.** Source card type (instant/sorcery vs permanent) plus `forever`/`uynt`/`uent`/`ueot`/`oneshot` flags select a continuous vs instant/UEOT variant. Never assume permanence.
4. **Misspelled-but-load-bearing tokens exist** — use them exactly: `hmodifer` (hand-size, not "hmodifier"), `reject` (= discard), `bstw` (= bestow), `cantmilllose` (triple-l), `serumpowder`.

---

## Section index
1. **Card block structure, fields & the 240 basic-ability keywords** — the `[card]` block and `key=` fields; `abilities=` vs `auto=`.
2. **Mana costs & all extra/alternative costs** — `mana=` syntax and every `{…}` cost token.
3. **Targets, zones, quantity prefixes, player-targets & duration modifiers** — the `target()` grammar, zones, `<prex>`, `[restrictions]`, `ueot`/`forever`/`limit`, macros, `ability$!…!$`.
4. **Triggered abilities** — the full `@…` vocabulary and trigger restrictions.
5. **Effect keywords, part 1** — structural/wrapper keywords + the first block of leaf effects.
6. **Effect keywords, part 2 + the ability-class catalog** — remaining effects, cost-reduction, and the `AllAbilities.h` class map.

---

## The two card-fixing rules (why this lexicon exists)
1. **Verify the Oracle first.** Before trusting any primitive, fetch the card's real Oracle text (Gatherer/Scryfall). Community authors mis-model cards routinely.
2. **Card-script before engine; faithful, not test-passing.** Prefer expressing the Oracle with existing constructs from this lexicon (card scripts are runtime data — no recompile). Match the *shape*, verify the *general* case (each/multi, both players, repeated triggers, X=0), and run the *full* test suite (persistent triggers can leak into distant tests). Only change the engine after confirming no faithful construct exists.

*(Maintenance: this lexicon is derived from the parser at the cited `file:line`s. Re-verify after engine changes that touch the parsers listed under Provenance.)*

---

# Card Block Structure, Fields & Basic-Ability Keywords

> **Ground-truth note on parsing location.** The `[card]…[/card]` block delimiters and the reprint mechanism are described in the wiki, but the authoritative **field-key parser is `MTGAllCards::processConfLine()` in `projects/mtg/src/MTGDeck.cpp:47`** (a `switch` on the *first character* of the key). `CardPrimitive.cpp` only holds the *setters* that this parser calls; it does not parse the `key=value` lines itself. Every field citation below points at the real dispatch site in `MTGDeck.cpp`, with the storage setter in `CardPrimitive.cpp` where relevant.

## 1. The `[card]` block

### Structure
A set is a `_cards.dat` file (or a `primitives/*.txt` file) containing a flat list of blocks:
```
[card]
name=Grizzly Bears
mana={1}{G}
type=Creature
subtype=Bear
power=2
toughness=2
[/card]
```
Grounding: `projects/mtg/bin/Res/sets/primitives/mtg.txt:50780-50787`.

- Lines are `key=value`, split on the **first** `=` (`MTGDeck.cpp:50`). A missing/leading `=` makes the line a no-op (`MTGDeck.cpp:51`).
- A line whose first char is `#` is a **comment**, silently skipped (`MTGDeck.cpp:49`). Corpus example header: `mtg.txt:1-3`.
- Unrecognized keys fall through to `default:` and emit a debug-only parse error; they are otherwise ignored (`MTGDeck.cpp:409-415`).
- Dispatch is on `key[0]` only, then disambiguated by later characters or full-string compares — so **the parser keys off first-letter buckets**, and several distinct fields share a `case`.

### Field keys (exact syntax → meaning → grounding)

| Key | Syntax | Meaning | Parser / setter | Real example |
|---|---|---|---|---|
| `name=` | `name=Grizzly Bears` | Card name (compulsory). Stored raw + lowercased `lcname`. Engine also injects the name into the type list (for cards like "Plague Rats"). | dispatch `MTGDeck.cpp:269-271`; setter `CardPrimitive.cpp:401` | `mtg.txt:50781` |
| `primitive=` | `primitive=Royal Assassin` | **Reprint**: bind this card's id/rarity to an already-loaded primitive of that name; all other data comes from the primitive. Looks up `primitives` map, calls `card->setPrimitive` (`MTGCard.cpp:88`). Note: shares `case 'p'`, matched by `key[1]=='r'`. | `MTGDeck.cpp:311-316` | (used in set `_cards.dat`, not primitives files) |
| `id=` | `id=174908` | Unique integer card id (compulsory). Sets `MTGCard::mtgid`. Also drives image filename `<id>.jpg` (negative id → token, `<abs>t.jpg`, `MTGCard.cpp:76-84`). | `MTGDeck.cpp:219-221`; `MTGCard.cpp:50` | (set files) |
| `grade=` | `grade=Borderline` | Code-quality grade. **Parsed by the 3rd letter of the value** (`getGrade(val[2])`, `MTGDeck.cpp:216`): `su**P**ported`→Supported, `bo**R**derline`→Borderline, `un**O**fficial`→Unofficial, `cr**A**ppy`→Crappy, `un**S**upported`→Unsupported, `da**N**gerous`→Dangerous (`getGrade`, `MTGDeck.cpp:20-43`). May appear at file top (before first `[card]`) to grade the whole file. Requires value length > 2. | `MTGDeck.cpp:215-217` | file-level header usage |
| `rarity=` | `rarity=U` | Single char: `C`ommon `U`ncommon `R`are `M`ythic `L`and `T`oken `S`pecial. Stored as the raw first char `val[0]` (`MTGCard.cpp:68`). Shares `case 'r'`, matched by substring `"rar"`. | `MTGDeck.cpp:356-360` | (set files) |
| `mana=` | `mana={2}{R}` | Mana cost; parsed by `ManaCost::parseManaCost`. Lowercased first. Also auto-derives colors (`CardPrimitive::setManaCost`, `CardPrimitive.cpp:220-230`). Shares `case 'm'`. | `MTGDeck.cpp:253-266`; setter `CardPrimitive.cpp:220` | `mtg.txt:50782` |
| `color=` | `color=red` | Force color(s): `white blue black red green artifact` (comma-separated; first entry clears others, `removeAllOthers`). Shares `case 'c'` (the non-`crewbonus` branch). Matched by first-letter fallthrough. | `MTGDeck.cpp:152-164`; setter `CardPrimitive.cpp:169` | (mtg.txt land/artifact cards) |
| `type=` | `type=Creature` | Space-separated type/supertype list; each token → `setType`→`setSubtype` (only a few hardcoded values are true "types"). Compulsory. | `MTGDeck.cpp:400-405`; setter `CardPrimitive.cpp:232` | `mtg.txt:50783` |
| `subtype=` | `subtype=Human Cleric` | Space-separated subtype list. Shares `case 's'` (non-`suspend` branch). | `MTGDeck.cpp:380-386`; setter `CardPrimitive.cpp:242` | `mtg.txt:50784`, `mtg.txt:64` |
| `power=` | `power=2` | Integer power (creatures). `atoi`. Shares `case 'p'`, the else/fallthrough branch (not `phasedout`/`primitive`/`partner`). | `MTGDeck.cpp:322-326`; setter `CardPrimitive.cpp:457` | `mtg.txt:50785` |
| `toughness=` | `toughness=2` | Integer toughness. `atoi`. Shares `case 't'`. | `MTGDeck.cpp:406`; setter `CardPrimitive.cpp:467` | `mtg.txt:50786` |
| `text=` | `text=Flying, vigilance` | Display/reminder text; use ` -- ` instead of newlines. | `MTGDeck.cpp:398-399`; setter `CardPrimitive.cpp:294` | `mtg.txt:51` |
| `target=` | `target=creature` | Spell target spec; **instants/sorceries only** (permanents target inside `auto=`). Lowercased, stored in `spellTargetType`. Shares `case 't'`. | `MTGDeck.cpp:392-397` | `mtg.txt:82` (Abduction) |
| `abilities=` | `abilities=flying,vigilance` | Comma-separated **basic-ability keywords** (see §2). See §3 for its matching semantics. | `MTGDeck.cpp:81-99` | `mtg.txt:50`, `mtg.txt:40` |
| `auto=` | `auto=life:2` | Scripted behavior (cost/trigger/effect DSL). Repeatable. Stored via `addMagicText` into `magicText`. See §3. | `MTGDeck.cpp:66-70`; `CardPrimitive.cpp:330` | `mtg.txt:60`, `mtg.txt:84` |
| `auto<ZONE>=` | `autograveyard=…`, `autohand=`, `autoexile=`, `autolibrary=`, `autocommandzone=`, … | Any key **starting with** `auto` (but not exactly `auto`): the suffix after `auto` is the zone key; stored into `magicTexts[suffix]` (`addMagicText(val, key.substr(4))`). See §3. | `MTGDeck.cpp:71-75` | `mtg.txt:10` (`autoexile=`) |
| `anyzone=` | `anyzone=…` | Convenience: registers the same auto text in **all** zone buckets (hand, library, graveyard, stack, exile, commandzone, reveal, sideboard) plus the default. Shares `case 'a'`. | `MTGDeck.cpp:101-113` | — |
| `alias=` | `alias=1194` | Integer alias id (`atoi`) — links functionally-identical cards (e.g. auras sharing enchant logic). Stored in `CardPrimitive::alias`. | `MTGDeck.cpp:76-79` | `mtg.txt:83` (Abduction) |
| `kicker=` | `kicker={1}{W}` / `kicker=multi{b}{b}` | Kicker cost. `multi` prefix → multikicker flag; optional `name(...)`. | `MTGDeck.cpp:224-251` | — |
| `other=` | `other={cost}` | Alternate cost (`setAlternative`). Shares `case 'o'`; the non-`otherrestriction` branch. Optional `name(...)`. | `MTGDeck.cpp:281-299` | — |
| `otherrestriction=` | `otherrestriction=…` | Extra cast restriction string. `case 'o'`, matched by `key[5]=='r'`. | `MTGDeck.cpp:276-280`; setter `CardPrimitive.cpp:153` | — |
| `restriction=` | `restriction=compare(isflipped)~equalto~0` | Cast restriction predicate. `case 'r'`, matched by `key[2]=='s'&&key[3]=='t'`. | `MTGDeck.cpp:330-335`; setter `CardPrimitive.cpp:137` | `mtg.txt:95` |
| `flashback=` | `flashback={5}{G}{G}` | Flashback cost (graveyard recast → exile). Also handles `facedown`(morph) via substring. Optional `name(...)`. `case 'f'`. | `MTGDeck.cpp:184-213` | `mtg.txt:50792` |
| `buyback=` | `buyback={1}{W}{W}` | Buyback alt cost. `case 'b'`, else branch. | `MTGDeck.cpp:134-140` | — |
| `bestow=` | `bestow={cost}` | Bestow cost. `case 'b'`, matched by `key[1]=='e'&&key[2]=='s'`. | `MTGDeck.cpp:118-127` | — |
| `backside=` | `backside=Perfected Form` | Double-faced back-face card name. `case 'b'`, `key[1]=='a'&&key[2]=='c'`. Stored in `backSide`. | `MTGDeck.cpp:129-133` | `mtg.txt:94` |
| `retrace=` | `retrace={cost}` | Retrace cost. `case 'r'`, `key[1]=='e'&&key[2]=='t'`. Optional `name(...)`. | `MTGDeck.cpp:336-355` | — |
| `suspend(N)=` | `suspend(3)={2}{R}` | Suspend cost + N time counters (parsed from `suspend(<n>)`). `case 's'`, substring `"suspend"`. | `MTGDeck.cpp:365-379` | — |
| `dredge=` | `dredge=dredge(3)` | Dredge amount via `parseBetween(...,"dredge(",")")`. `case 'd'`, else branch. | `MTGDeck.cpp:174-183` | — |
| `doublefaced=` | `doublefaced=…` | Double-faced flag/name (lowercased). `case 'd'`, `key=="doublefaced"`. | `MTGDeck.cpp:166-173`; setter `CardPrimitive.cpp:346` | — |
| `modular=` | `modular=…` | Modular counter value. `case 'm'`, `key=="modular"`. | `MTGDeck.cpp:256-259`; setter `CardPrimitive.cpp:390` | — |
| `partner=` | `partner=…` | Named-partner value. `case 'p'`, `key[1]=='a'&&key[2]=='r'`. | `MTGDeck.cpp:317-321` | — |
| `aicode=` | `aicode=…` | AI-replacement custom code (e.g. reveal:number). `case 'a'`, `key=="aicode"`. | `MTGDeck.cpp:61-65`; setter `CardPrimitive.cpp:357` | — |
| `crewbonus=` | `crewbonus=…` | Crew ability. `case 'c'`, `key=="crewbonus"`. | `MTGDeck.cpp:144-151`; setter `CardPrimitive.cpp:368` | — |
| `phasedoutbonus=` | `phasedoutbonus=…` | Ability while phased out. `case 'p'`, `key=="phasedoutbonus"`. | `MTGDeck.cpp:303-310`; setter `CardPrimitive.cpp:379` | — |

**Compulsory** (wiki + confirmed by usage): `name`, `id`, `rarity`, `type`; `power`/`toughness` for normal creatures; `text` if no picture.

## 2. Full basic-ability keyword list (`MTGBasicAbilities[]`)

Authoritative source: the string table `Constants::MTGBasicAbilities[]` at `MTGDefinitions.cpp:33-274`, indexed 0…239, with the parallel enum (`Constants::TRAMPLE`…`MYTREASON`, `NB_BASIC_ABILITIES=240`) at `MTGDefinitions.h:128-369`. Index = enum value = array position (verified 1:1). "D/C" column flags **damage-/combat-relevant** abilities.

| # | Keyword | Enum | Meaning | D/C |
|---|---|---|---|---|
|0|`trample`|TRAMPLE|Excess combat damage to blocked creature carries to player|✓|
|1|`forestwalk`|FORESTWALK|Unblockable if defender controls a Forest|✓|
|2|`islandwalk`|ISLANDWALK|Unblockable if defender controls an Island|✓|
|3|`mountainwalk`|MOUNTAINWALK|Unblockable if defender controls a Mountain|✓|
|4|`swampwalk`|SWAMPWALK|Unblockable if defender controls a Swamp|✓|
|5|`plainswalk`|PLAINSWALK|Unblockable if defender controls a Plains|✓|
|6|`flying`|FLYING|Only blockable by flyers/reach|✓|
|7|`first strike`|FIRSTSTRIKE|Deals combat damage first|✓|
|8|`double strike`|DOUBLESTRIKE|Deals both first-strike and normal combat damage|✓|
|9|`fear`|FEAR|Only blockable by artifact/black creatures|✓|
|10|`flash`|FLASH|May cast any time you could cast an instant| |
|11|`haste`|HASTE|Can attack/tap the turn it enters|✓|
|12|`lifelink`|LIFELINK|Damage it deals also gains you that much life|✓|
|13|`reach`|REACH|Can block flyers|✓|
|14|`shroud`|SHROUD|Can't be targeted| |
|15|`vigilance`|VIGILANCE|Doesn't tap to attack|✓|
|16|`defender` (`DEFENSER` alias)|DEFENDER=16|Can't attack|✓|
|17|`banding`|BANDING|Banding (present in table despite wiki claiming removal)|✓|
|18|`protection from green`|PROTECTIONGREEN|Protection: green|✓|
|19|`protection from blue`|PROTECTIONBLUE|Protection: blue|✓|
|20|`protection from red`|PROTECTIONRED|Protection: red|✓|
|21|`protection from black`|PROTECTIONBLACK|Protection: black|✓|
|22|`protection from white`|PROTECTIONWHITE|Protection: white|✓|
|23|`unblockable`|UNBLOCKABLE|Can't be blocked|✓|
|24|`wither`|WITHER|Deals damage as -1/-1 counters|✓|
|25|`persist`|PERSIST|Returns with a -1/-1 counter on death| |
|26|`retrace`|RETRACE|Retrace keyword flag| |
|27|`exalted`|EXALTED|+1/+1 when it attacks alone|✓|
|28|`nofizzle`|NOFIZZLE|Can't be countered| |
|29|`shadow`|SHADOW|Only blocks/blocked by shadow|✓|
|30|`reachshadow`|REACHSHADOW|Can block shadow creatures|✓|
|31|`foresthome`|FORESTHOME|Home-land variant (must control Forest to attack)|✓|
|32|`islandhome`|ISLANDHOME|Islandhome|✓|
|33|`mountainhome`|MOUNTAINHOME|Mountainhome|✓|
|34|`swamphome`|SWAMPHOME|Swamphome|✓|
|35|`plainshome`|PLAINSHOME|Plainshome|✓|
|36|`cloud`|CLOUD|Can only block flyers|✓|
|37|`cantattack`|CANTATTACK|Can't attack|✓|
|38|`mustattack`|MUSTATTACK|Attacks each combat if able|✓|
|39|`cantblock`|CANTBLOCK|Can't block|✓|
|40|`doesnotuntap`|DOESNOTUNTAP|Doesn't untap during untap step| |
|41|`hexproof`|HEXPROOF|Can't be targeted by opponents| |
|42|`indestructible`|INDESTRUCTIBLE|Can't be destroyed|✓|
|43|`intimidate`|INTIMIDATE|Only blockable by artifacts/shared color|✓|
|44|`deathtouch`|DEATHTOUCH|Any damage it deals is lethal|✓|
|45|`horsemanship`|HORSEMANSHIP|Only blocked by horsemanship|✓|
|46|`cantregen`|CANTREGEN|Can't be regenerated|✓|
|47|`oneblocker`|ONEBLOCKER|Can be blocked by only one creature|✓|
|48|`infect`|INFECT|Damage as -1/-1 (creatures) / poison (players)|✓|
|49|`poisontoxic`|POISONTOXIC|Toxic 1|✓|
|50|`poisontwotoxic`|POISONTWOTOXIC|Toxic 2|✓|
|51|`poisonthreetoxic`|POISONTHREETOXIC|Toxic 3|✓|
|52|`phantom`|PHANTOM|Prevents damage, removes a +1/+1 counter instead|✓|
|53|`wilting`|WILTING|Source takes damage as -1/-1 counters|✓|
|54|`vigor`|VIGOR|Instead of taking damage, gains +1/+1 counters|✓|
|55|`changeling`|CHANGELING|Is every creature type| |
|56|`absorb`|ABSORB|Prevent 1 of any damage that would be dealt to it|✓|
|57|`treason`|TREASON|Sacrifice at end of turn (Sneak Attack style)| |
|58|`unearth`|UNEARTH|Unearth flag| |
|59|`cantlose`|CANTLOSE|Controller can't lose| |
|60|`cantlifelose`|CANTLIFELOSE|Can't lose via 0 life| |
|61|`cantmilllose`|CANTMILLLOSE|Can't lose via empty library (note triple-l)| |
|62|`snowlandwalk`|SNOWWALK|Landwalk: snow lands|✓|
|63|`nonbasiclandwalk`|NONBASICWALK|Landwalk: nonbasic|✓|
|64|`strong`|STRONG|Can't be blocked by lower-power creatures|✓|
|65|`storm`|STORM|Copy per spell cast this turn| |
|66|`phasing`|PHASING|Phases out/in each turn| |
|67|`split second`|SPLITSECOND|No responses while on stack| |
|68|`weak`|WEAK|Can't block higher-power creatures|✓|
|69|`affinityartifacts`|AFFINITYARTIFACTS|Costs 1 less per artifact| |
|70|`affinityplains`|AFFINITYPLAINS|Costs 1 less per Plains| |
|71|`affinityforests`|AFFINITYFOREST|Costs 1 less per Forest| |
|72|`affinityislands`|AFFINITYISLAND|Costs 1 less per Island| |
|73|`affinitymountains`|AFFINITYMOUNTAIN|Costs 1 less per Mountain| |
|74|`affinityswamps`|AFFINITYSWAMP|Costs 1 less per Swamp| |
|75|`affinitygreencreatures`|AFFINITYGREENCREATURES|Costs 1 less per green creature| |
|76|`cantwin`|CANTWIN|Controller can't win / opp can't lose| |
|77|`nomaxhand`|NOMAXHAND|No maximum hand size| |
|78|`leyline`|LEYLINE|May start in play (Leyline cycle)| |
|79|`playershroud`|PLAYERSHROUD|Player can't be targeted| |
|80|`controllershroud`|CONTROLLERSHROUD|Controller can't be targeted| |
|81|`sunburst`|SUNBURST|Enters with counters per mana color spent| |
|82|`flanking`|FLANKING|Non-flanking blockers get -1/-1|✓|
|83|`exiledeath`|EXILEDEATH|Goes to exile instead of graveyard on death| |
|84|`legendarylandwalk`|LEGENDARYWALK|Landwalk: legendary lands|✓|
|85|`desertlandwalk`|DESERTWALK|Landwalk: Deserts|✓|
|86|`snowforestlandwalk`|SNOWFORESTWALK|Landwalk: snow Forest|✓|
|87|`snowplainslandwalk`|SNOWPLAINSWALK|Landwalk: snow Plains|✓|
|88|`snowmountainlandwalk`|SNOWMOUNTAINWALK|Landwalk: snow Mountain|✓|
|89|`snowislandlandwalk`|SNOWISLANDWALK|Landwalk: snow Island|✓|
|90|`snowswamplandwalk`|SNOWSWAMPWALK|Landwalk: snow Swamp|✓|
|91|`canattack`|CANATTACK|Overrides can't-attack|✓|
|92|`hydra`|HYDRA|Hydra (variable P/T) flag|✓|
|93|`undying`|UNDYING|Returns with a +1/+1 counter on death| |
|94|`poisonshroud`|POISONSHROUD|Player can't get poison counters| |
|95|`noactivatedability`|NOACTIVATED|Activated abilities can't be used| |
|96|`notapability`|NOACTIVATEDTAP|Can't use tap abilities| |
|97|`nomanaability`|NOMANA|Mana abilities can't be used| |
|98|`onlymanaability`|ONLYMANA|Only mana abilities usable| |
|99|`poisondamager`|POISONDAMAGER|Deals its damage to players as poison counters|✓|
|100|`soulbond`|SOULBOND|Soulbond pairing| |
|101|`lure`|LURE|All able creatures must block it|✓|
|102|`nolegend`|NOLEGEND|Legend rule not enforced| |
|103|`canplayfromgraveyard`|CANPLAYFROMGRAVEYARD|May play from graveyard| |
|104|`tokenizer`|TOKENIZER|Doubles tokens produced (Parallel Lives)| |
|105|`mygraveexiler`|MYGRAVEEXILER|My cards exile instead of graveyard| |
|106|`oppgraveexiler`|OPPGRAVEEXILER|Opp cards exile instead of graveyard| |
|107|`librarydeath`|LIBRARYDEATH|On death goes to library| |
|108|`shufflelibrarydeath`|SHUFFLELIBRARYDEATH|On death shuffled into library| |
|109|`offering`|OFFERING|Offering keyword| |
|110|`evadebigger`|EVADEBIGGER|Can't be blocked by bigger creatures|✓|
|111|`spellmastery`|SPELLMASTERY|Spell mastery (alt-cost condition)| |
|112|`nolifegain`|NOLIFEGAIN|Controller can't gain life| |
|113|`nolifegainopponent`|NOLIFEGAINOPPONENT|Opponent can't gain life| |
|114|`auraward`|AURAWARD|Aura protection exception| |
|115|`madness`|MADNESS|Madness keyword| |
|116|`protectionfromcoloredspells`|PROTECTIONFROMCOLOREDSPELLS|Protection from colored spells|✓|
|117|`mygcreatureexiler`|MYGCREATUREEXILER|My creatures exile on death| |
|118|`oppgcreatureexiler`|OPPGCREATUREEXILER|Opp creatures exile on death| |
|119|`zerocast`|PAYZERO|Can be cast for {0}| |
|120|`trinisphere`|TRINISPHERE|Spells cost at least 3| |
|121|`canplayfromexile`|CANPLAYFROMEXILE|May play from exile| |
|122|`libraryeater`|LIBRARYEATER|Library-eater effect| |
|123|`devoid`|DEVOID|Colorless| |
|124|`cantchangelife`|CANTCHANGELIFE|Life total can't change| |
|125|`combattoughness`|COMBATTOUGHNESS|Assigns combat damage by toughness|✓|
|126|`cantpaylife`|CANTPAYLIFE|Can't pay life| |
|127|`cantbesacrified`|CANTBESACRIFIED|Can't be sacrificed| |
|128|`skulk`|SKULK|Can't be blocked by higher-power creatures|✓|
|129|`menace`|MENACE|Must be blocked by 2+ creatures|✓|
|130|`nosolo`|NOSOLO|Can't attack alone|✓|
|131|`mustblock`|MUSTBLOCK|Blocks each turn if able|✓|
|132|`dethrone`|DETHRONE|+1/+1 counter attacking the highest-life player|✓|
|133|`overload`|OVERLOAD|Overload keyword| |
|134|`shackler`|SHACKLER|Shackler effect| |
|135|`flyersonly`|FLYERSONLY|Can attack only if it has flying|✓|
|136|`tempflashback`|TEMPFLASHBACK|Temporary flashback| |
|137|`legendruleremove`|NOLEGENDRULE|Removes legend rule| |
|138|`canttransform`|CANTTRANSFORM|Can't transform| |
|139|`asflash`|ASFLASH|May be cast as though it had flash| |
|140|`conduited`|CONDUITED|Conduit flag| |
|141|`canblocktapped`|CANBLOCKTAPPED|Can block while tapped|✓|
|142|`oppnomaxhand`|OPPNOMAXHAND|Opponent has no max hand| |
|143|`cantcrew`|CANTCREW|Can't crew Vehicles| |
|144|`hiddenface`|HIDDENFACE|Face-hidden (test)| |
|145|`anytypeofmana`|ANYTYPEOFMANA|May spend mana as any color (casting)| |
|146|`necroed`|NECROED|Necro-hidden flag| |
|147|`cantpwattack`|CANTPWATTACK|Can't attack planeswalkers|✓|
|148|`canplayfromlibrarytop`|CANPLAYFROMLIBRARYTOP|Play any from top of library| |
|149|`canplaylandlibrarytop`|CANPLAYLANDTOPLIBRARY|Play lands from top| |
|150|`canplaycreaturelibrarytop`|CANPLAYCREATURETOPLIBRARY|Play creatures from top| |
|151|`canplayartifactlibrarytop`|CANPLAYARTIFACTTOPLIBRARY|Play artifacts from top| |
|152|`canplayinstantsorcerylibrarytop`|CANPLAYINSTANTSORCERYTOPLIBRARY|Play instants/sorceries from top| |
|153|`showfromtoplibrary`|SHOWFROMTOPLIBRARY|Top of own library revealed| |
|154|`showopponenttoplibrary`|SHOWOPPONENTTOPLIBRARY|Top of opp library revealed| |
|155|`totemarmor`|TOTEMARMOR|Totem armor (destroy → remove aura)|✓|
|156|`discardtoplaybyopponent`|DISCARDTOPLAYBYOPPONENT|Opponent may play your discards| |
|157|`modular`|MODULAR|Modular (counters transfer on death)|✓|
|158|`mutate`|MUTATE|Can mutate| |
|159|`adventure`|ADVENTURE|Has an Adventure| |
|160|`mentor`|MENTOR|Mentor (+1/+1 to lesser attacker)|✓|
|161|`prowess`|PROWESS|+1/+1 on noncreature spell|✓|
|162|`nofizzle alternative`|NOFIZZLEALTERNATIVE|No fizzle if paid with alt cost| |
|163|`hasotherkicker`|HASOTHERKICKER|Kicker is non-mana (life/tap)| |
|164|`partner`|PARTNER|Has partner| |
|165|`canbecommander`|CANBECOMMANDER|Can be a commander| |
|166|`poisonfourtoxic`|POISONFOURTOXIC|Toxic 4|✓|
|167|`threeblockers`|THREEBLOCKERS|Must be blocked by 3+|✓|
|168|`handdeath`|HANDDEATH|On death goes to hand| |
|169|`inplaydeath`|INPLAYDEATH|On death returns to play untapped| |
|170|`inplaytapdeath`|INPLAYTAPDEATH|On death returns tapped| |
|171|`gainedexiledeath`|GAINEDEXILEDEATH|Granted exile-on-death| |
|172|`gainedhanddeath`|GAINEDHANDDEATH|Granted hand-on-death| |
|173|`cycling`|CYCLING|Has cycling| |
|174|`foretell`|FORETELL|Has foretell| |
|175|`anytypeofmanaability`|ANYTYPEOFMANAABILITY|Any-color mana for abilities| |
|176|`boast`|BOAST|Has boast| |
|177|`twoboast`|TWOBOAST|Boast twice| |
|178|`replacescry`|REPLACESCRY|Scry replacement| |
|179|`hasnokicker`|HASNOKICKER|Kicker not a real kicker (Fuse)| |
|180|`undamageable`|UNDAMAGEABLE|Can't be damaged|✓|
|181|`lifefaker`|LIFEFAKER|Modifies life-gain amount| |
|182|`doublefacedeath`|DOUBLEFACEDEATH|DFC → temp zone on death| |
|183|`gaineddoublefacedeath`|GAINEDDOUBLEFACEDEATH|Granted DFC-death| |
|184|`twodngtrg`|TWODNGTRG|Room abilities trigger twice| |
|185|`nodngopp`|NODNGOPP|Opponent can't venture| |
|186|`nodngplr`|NODNGPLR|Controller can't venture| |
|187|`canplayauraequiplibrarytop`|CANPLAYAURAEQUIPTOPLIBRARY|Play auras/equipment from top| |
|188|`counterdeath`|COUNTERDEATH|Gains a 1/1 counter on graveyard return| |
|189|`dungeoncompleted`|DUNGEONCOMPLETED|Dungeon completed| |
|190|`perpetuallifelink`|PERPETUALLIFELINK|Perpetual lifelink|✓|
|191|`perpetualdeathtouch`|PERPETUALDEATHTOUCH|Perpetual deathtouch|✓|
|192|`noncombatvigor`|NONCOMBATVIGOR|Non-combat damage → +1/+1 counters|✓|
|193|`nomovetrigger`|NOMOVETRIGGER|No zone-change triggers| |
|194|`canloyaltytwice`|CANLOYALTYTWICE|Loyalty abilities twice/turn| |
|195|`showopponenthand`|SHOWOPPONENTHAND|Opp plays with hand revealed| |
|196|`showcontrollerhand`|SHOWCONTROLLERHAND|Controller plays hand revealed| |
|197|`hasreplicate`|HASREPLICATE|Kicker is replicate| |
|198|`isprey`|ISPREY|Haunted by another card| |
|199|`hasdisturb`|HASDISTURB|Retrace is disturb| |
|200|`daybound`|DAYBOUND|Daybound| |
|201|`nightbound`|NIGHTBOUND|Nightbound| |
|202|`decayed`|DECAYED|Decayed (can't block; sac after attack)|✓|
|203|`hasstrive`|HASSTRIVE|Kicker is strive| |
|204|`isconspiracy`|ISCONSPIRACY|Is a conspiracy| |
|205|`hasaftermath`|HASAFTERMATH|Flashback is aftermath| |
|206|`noentertrg`|NOENTERTRG|ETB triggers suppressed| |
|207|`nodietrg`|NODIETRG|Death triggers suppressed| |
|208|`training`|TRAINING|Training|✓|
|209|`energyshroud`|ENERGYSHROUD|Can't get energy counters| |
|210|`expshroud`|EXPSHROUD|Can't get experience counters| |
|211|`countershroud`|COUNTERSHROUD|Can't get any counters| |
|212|`nonight`|NONIGHT|Can't become night| |
|213|`nodamageremoved`|NODAMAGEREMOVED|Damage not removed each turn|✓|
|214|`backgroundpartner`|BACKGROUNDPARTNER|Can choose Background partner| |
|215|`bottomlibrarydeath`|BOTTOMLIBRARYDEATH|On death → bottom of library| |
|216|`noloyaltydamage`|NOLOYALTYDAMAGE|Damage doesn't remove loyalty|✓|
|217|`nodefensedamage`|NODEFENSEDAMAGE|Damage doesn't remove defense (Battle)|✓|
|218|`affinityallcreatures`|AFFINITYALLCREATURES|1 less per creature (all battlefields)| |
|219|`affinitycontrollercreatures`|AFFINITYCONTROLLERCREATURES|1 less per your creature| |
|220|`affinityopponentcreatures`|AFFINITYOPPONENTCREATURES|1 less per opp creature| |
|221|`affinityalldeadcreatures`|AFFINITYALLDEADCREATURES|1 less per creature that died this turn| |
|222|`affinityparty`|AFFINITYPARTY|1 less per party member| |
|223|`affinityenchantments`|AFFINITYENCHANTMENTS|1 less per your enchantment| |
|224|`affinitybasiclandtypes`|AFFINITYBASICLANDTYPES|1 less per basic land type| |
|225|`affinitytwobasiclandtypes`|AFFINITYTWOBASICLANDTYPES|2 less per basic land type| |
|226|`affinitygravecreatures`|AFFINITYGRAVECREATURES|1 less per creature in your graveyard| |
|227|`affinityattackingcreatures`|AFFINITYATTACKINGCREATURES|1 less per attacking creature| |
|228|`affinitygraveinstsorc`|AFFINITYGRAVEINSTSORC|1 less per instant/sorcery in graveyard| |
|229|`poisonfivetoxic`|POISONFIVETOXIC|Toxic 5|✓|
|230|`poisonsixtoxic`|POISONSIXTOXIC|Toxic 6|✓|
|231|`poisonseventoxic`|POISONSEVENTOXIC|Toxic 7|✓|
|232|`poisoneighttoxic`|POISONEIGHTTOXIC|Toxic 8|✓|
|233|`poisonninetoxic`|POISONNINETOXIC|Toxic 9|✓|
|234|`poisontentoxic`|POISONTENTOXIC|Toxic 10|✓|
|235|`eqpasinst`|EQPASINST|Can equip as instant| |
|236|`canloyaltyasinst`|CANLOYALTYASINST|Loyalty abilities as instant| |
|237|`canplayenchantmentlibrarytop`|CANPLAYENCHANTMENTTOPLIBRARY|Play enchantments from top| |
|238|`affinitytwoalldeadcreatures`|AFFINITYTWOALLDEADCREATURES|2 less per creature that died this turn| |
|239|`mytreason`|MYTREASON|Sacrifice at controller's end step only (Thirsting Axe)| |

## 3. `auto=` vs `ability=`/`abilities=` and other ability-bearing fields

- **`abilities=`** is the **only** field that writes into the compact `basicAbilities` bitset (`CardPrimitive::basicAbilities`, a `std::bitset`). It comma-splits, lowercases, and for each token scans the `MTGBasicAbilities[]` table (`MTGDeck.cpp:81-99`). **Ground-truth matching semantics (important, undocumented in wiki):**
  - Match is `token.find(tableKeyword) != npos` — i.e. the table keyword must be a **substring of** the user token, not an equality test (`MTGDeck.cpp:93`).
  - Iteration runs **from the highest index (239) down to 0** and **breaks on the first hit** (`MTGDeck.cpp:91-97`). So when one keyword is a substring of another, the **higher-indexed** ability wins. (E.g. `poisontwotoxic` (50) is tried before `poisontoxic` (49); `affinitytwoalldeadcreatures` (238) before `affinityalldeadcreatures` (221).) This reverse-scan is deliberate to disambiguate the prefix-overlapping toxic/affinity families.
  - Consequence: one token sets **at most one** ability bit (the `break`). Multiple abilities require the comma list.
- There is **no `ability=` (singular) key** in the parser. The wiki/reference term is `abilities=`. The corpus and parser use `abilities=` exclusively.
- **`auto=`** does **not** touch `basicAbilities`. It stores a free-form DSL string into `magicText` via `addMagicText` (`MTGDeck.cpp:66-70`; `CardPrimitive.cpp:330`), interpreted later by the ability engine. Basic-ability keywords *can* also appear as `auto=` effects (e.g. `auto=first strike` at `mtg.txt:8`, `auto=flying ueot`), where they are granted dynamically/temporarily rather than as a static printed bit.
  - **Rule of thumb from the source:** `abilities=` = permanent, printed keyword → bitset; `auto=` = scripted/conditional/temporary grant or any non-keyword behavior → magicText.
- **Zone-scoped auto variants** (`autograveyard=`, `autohand=`, `autoexile=`, `autolibrary=`, `autocommandzone=`, etc.) are any key starting with `auto`; the 4+ suffix is used as the `magicTexts` bucket key (`MTGDeck.cpp:71-75`). `anyzone=` fans one string into all buckets (`MTGDeck.cpp:101-113`).

## 4. Wiki discrepancies (`docs/wiki/CardCode.md` vs parser)

1. **Ability list is a small subset.** The wiki's `abilities` section (`CardCode.md:285-364`) lists ~90 keywords; the parser table has **240** (`MTGDefinitions.cpp:33-274`). Everything from index ~114 onward (madness, hexproof, indestructible, deathtouch, menace, skulk, prowess, mentor, training, daybound/nightbound, decayed, the toxic 1–10 family, the whole `affinity*` expansion, all the `*death`/`*librarytop` families, mutate, adventure, foretell, boast, etc.) is **missing from the wiki**.
2. **`banding` is NOT removed.** Wiki says banding "never worked right and [has] been removed" (`CardCode.md:380`). The parser **still has `banding` at index 17** (`MTGDefinitions.cpp:51`, enum `BANDING=17`). The wiki claim is stale/wrong for this version.
3. **Keyword spelling errors in the wiki** (would silently fail against the substring matcher):
   - `cantmillose` (`CardCode.md:296`) — parser keyword is **`cantmilllose`** (triple-l, `MTGDefinitions.cpp:95`).
   - `posiontwotoxic` / `poisonthreetoxis` (`CardCode.md:335`) — parser is **`poisontwotoxic`** / **`poisonthreetoxic`** (`MTGDefinitions.cpp:84-85`).
   - `opponentshroud` (`CardCode.md:331`) — no such table entry; parser has **`playershroud`** (79) and **`controllershroud`** (80).
4. **`frozen` and `rampage` are not basic abilities.** Wiki lists `frozen` (`CardCode.md:314`) and `rampage (??)` (`CardCode.md:337`) under `abilities=`. Neither is in `MTGBasicAbilities[]`; `rampage` is an `auto=` effect (`rampage(p/t,n)`, `CardCode.md:846`) and `frozen`/`doesnotuntap` behavior maps to `doesnotuntap` (40). Putting these literally in `abilities=` matches nothing.
5. **Undocumented `abilities=` matching behavior.** The wiki presents `abilities=` as an exact keyword list. The real parser does **substring containment + reverse-index scan + break** (§3). This is invisible in the wiki and is a real correctness trap for card authors (a token can match an unintended longer/higher-index keyword).
6. **Missing/undocumented card fields.** The wiki's key list (`CardCode.md:33-52`) omits several keys the parser accepts: `aicode`, `crewbonus`, `phasedoutbonus`, `modular`, `partner`, `backside`, `doublefaced`, `restriction`, `otherrestriction`, `bestow`, `retrace`, `suspend(N)`, `dredge`, and the `color=` key is documented but the multi-value/`removeAllOthers` first-clears-others behavior (`MTGDeck.cpp:157-161`) is not.
7. **`grade` parsing is by 3rd letter, not string compare.** Wiki lists the six grade words (`CardCode.md:93`) but the parser only reads `val[2]` (`MTGDeck.cpp:216`, `getGrade`). Any grade word whose 3rd letter collides would misgrade; the six official words happen to have unique 3rd letters (`suPported`, `boRderline`, `unOfficial`, `crAppy`, `unSupported`, `daNgerous`). Also confirmed: default grade when unset is **Borderline**, not "Supported" as the wiki states (`CardCode.md:93` vs `MTGDeck.cpp:560,737` `maxGrade = GRADE_BORDERLINE`).
8. **`rarity`** — wiki omits **`S` (Special)**, which is a valid rarity (`MTGDefinitions.h:371`, `RARITY_S='S'`). Parser stores whatever first char is given (`MTGCard.cpp:68`), so any char is technically accepted.

---

# Mana Costs & Extra/Alternative Costs (ground-truth lexicon)

All costs are parsed by `ManaCost::parseManaCost()` in `projects/mtg/src/ManaCost.cpp:15-431`. The string is scanned token-by-token, each token wrapped in `{...}`. Every field that holds a cost (`mana=`, `kicker=`, `other=`, `flashback=`, `buyback=`, `bestow=`, `retrace=`, `suspend(N)=`, `facedown=`, and every ability `cost:` in an `auto=` line) runs through this **same** parser, so any token below is legal in any of those fields.

**Important loader fact:** card cost strings are lower-cased before parsing (`MTGDeck.cpp:263`, and again inside the parser at `ManaCost.cpp:99`). So `{W}`, `{X}`, `{S}` as written by a human become `w`, `x`, `s`. Documentation here shows the canonical (usually lowercase) form the parser actually matches.

---

## 1. Basic mana syntax

| Form | Meaning | Parse site |
|---|---|---|
| `{w}` `{u}` `{b}` `{r}` `{g}` | One colored pip (White/blUe/Black/Red/Green). Matched as exact single letters **before** the extra-cost switch. | `ManaCost.cpp:64-83` |
| `{N}` (e.g. `{3}`, `{16}`, `{0}`) | Generic mana, N of any color. Falls to `default:` → `add(MTG_COLOR_ARTIFACT, atoi)`. | `ManaCost.cpp:414-417` |
| `{c}` | **True colorless** (Eldrazi/"waste") mana — must be paid by colorless, distinct from generic. `add(MTG_COLOR_WASTE,1)`. | `ManaCost.cpp:372-376` |
| `{x}` (`{X}`) | Variable X. `manaCost->x()` sets the X flag; the announced value is `source->setX`. Two X are allowed: `{x}{x}` (e.g. Builder's Bane `mana={X}{X}{R}`, `mtg.txt:15581`). | `ManaCost.cpp:102-106`, `ManaCost::x()` 552-560 |
| `{x:color}` | **Color-restricted X** — X must be paid in one color. Builds a "specificX" (`specificX(color)`), so `Diff()` only drains that color for X. Example Atalya `auto={X:white}{T}:...` (`mtg.txt:6715`). | `ManaCost.cpp:107-123`, `specificX()` 574-582 |
| `{cw}` … / `{2r}` … (two symbols in one brace) | **Hybrid pip.** In `default:`, if the token is >1 char and `atoi<10`, each of the two chars is read as either a digit (→ generic value) or a color char (→ that color, value 1), then `addHybrid(c0,v0,c1,v1)`. So `{wb}` = W-or-B; `{2r}` = 2-generic-or-R (Kicker Awaken style). Example Act of Aggression path & `mana={U}{WB}`. | `ManaCost.cpp:386-413`, `addHybrid()` 981-985 |
| `{p(color)}` | **Phyrexian mana** — pay the color **or** 2 life. Builds `LifeorManaCost`. `getConvertedCost()` counts it as 1. Example Act of Aggression `mana={3}{p(R)}{p(R)}` (`mtg.txt:762`). | `ManaCost.cpp:286-296` → `LifeorManaCost` (`ExtraCost.h:108-120`, `ExtraCost.cpp:355-416`) |
| `{i}` | **Snow** mana (Wagic's snow symbol is `i`, **not** `{S}`). Adds 1 generic AND a `SnowCost` that additionally requires ≥1 snow-source mana be spent. Example Adarkar Windform `auto={1}{i}:...` (`mtg.txt:858`). | `ManaCost.cpp:304-309` → `SnowCost` (`ExtraCost.h:218-226`, `ExtraCost.cpp:134-234`) |

**`{X}` / `<prex>` interaction (X-spells):** for a normal X-spell the mana pool drains all leftover colored mana into X inside `ManaCost::Diff()` (`ManaCost.cpp:1180-1201`, with `hasSpecificX()` handling color-restricted X). When an **extra cost** must consume X objects (not mana), it reads `source->setX` directly — see `ExileTargetCost::requiredExiles()`/`setPayment()` (`ExtraCost.cpp:943-961`): a `<prex>` target-count prefix on an exile cost (Skeletal Scrying's `{E(<prex>...)}`) exiles exactly the announced X.

Two dynamic (computed) mana tokens also exist — they add generic mana at parse/cast time, not an ExtraCost:
- `{value:...}` → evaluates a `WParsedInt` and adds that many generic (`ManaCost.cpp:125-132`; Circular Logic `mtg.txt:20013`).
- `{eval(expr)}` → same idea, adds generic from a parsed variable (`ManaCost.cpp:168-180`).
- `{mycost}` → adds this card's own printed mana cost; `{myevictcost}` → adds the imprinted card's cost (`ManaCost.cpp:232-251`).
- `{chosencolor}` → adds 1 mana of the card's chosen color (`ManaCost.cpp:331-335`; Abundant Growth `mtg.txt:292`).

---

## 2. Every extra-cost token (complete enumeration of the parser switch)

The switch is on the first character of the (lower-cased) token, `ManaCost.cpp:100-420`. Enumerated in switch order. A `(target)` argument, when present, is any target string and becomes a `TargetChooser` (`ManaCost.cpp:88-96`).

| Token(s) | ExtraCost subclass (decl / impl) | Semantics | Parse line | Real example (primitives) |
|---|---|---|---|---|
| `{t}` | `TapCost` (`ExtraCost.h:207-215` / `ExtraCost.cpp:733-767`) | Tap this permanent (fails if tapped/summoning-sick). | `133-137` | Loam Dryad `auto={T}{t(...)}:add{G}` `mtg.txt:67943` |
| `{t(target)}` | `TapTargetCost` (`ExtraCost.h:253-261` / `ExtraCost.cpp:811-876`) | Tap another permanent matching target. | `138-141` | Loam Dryad (above), tap another creature |
| `{crew(target)}` | `TapTargetCost` with `crew=true` | Vehicle crew: tap creatures, fires the card's crew ability; respects `CANTCREW`. | `340-343` | Aradara Express `auto={crew(other creature[power>=4]|myBattlefield)}:...` `mtg.txt:4720` |
| `{q}` | `UnTapCost` (`ExtraCost.h:242-250` / `ExtraCost.cpp:769-808`) | Untap this permanent (the {Q} untap symbol). | `311-315` | Crackleburr `auto={UR}{UR}{q(...)}{q(...)}{q}:...` `mtg.txt:23214` |
| `{q(target)}` | `UnTapTargetCost` (`ExtraCost.h:263-270` / `ExtraCost.cpp:879-920`) | Untap another permanent. | `316-318` | Crackleburr (above) |
| `{s}` | `SacrificeCost` (`ExtraCost.h:75-82` / `ExtraCost.cpp:1488-1524`) | Sacrifice this permanent (respects `CANTBESACRIFIED`). | `156-159` | Emrakul's Evangel `auto={s(other creature[-eldrazi]|mybattlefield)}{s}{t}:...` `mtg.txt:35706` |
| `{s(target)}` | `SacrificeCost` (with tc) | Sacrifice a permanent matching target. | `156-159` | Emrakul's Evangel (above) |
| `{s2l}` / `{s2l(target)}` | `ToLibraryCost` (`ExtraCost.h:151-157` / `ExtraCost.cpp:525-551`) | Put target (from anywhere) on top of library. | `144-147` | Hidden Retreat `auto={s2l(*|myhand)}:...` `mtg.txt:54471` |
| `{s2g}` / `{s2g(target)}` | `ToGraveCost` (`ExtraCost.h:160-166` / `ExtraCost.cpp:554-580`) | Put target (from anywhere) into graveyard. | `148-151` | Oracle of Dust `auto={2}{s2g(*|opponentexile)}:...` `mtg.txt:82683` |
| `{saclands}` | `SacLandsCost` (`ExtraCost.h:187-193` / `ExtraCost.cpp:648-681`) | Sacrifice ALL your lands. | `152-155` | Tomb of Urami `auto={2}{B}{B}{T}{saclands}:...` `mtg.txt:122246` |
| `{e(target)}` (or bare `{e}`) | `ExileTargetCost` (`ExtraCost.h:273-282` / `ExtraCost.cpp:923-1004`) | Exile target card(s). Handles X-count via `setX` (see §1). | `190-192` | Dread Defiler `auto={3}{c}{e(creature|mygraveyard)}:...` `mtg.txt:31678` |
| `{e:N}` | `EnergyCost` (`ExtraCost.h:229-239` / `ExtraCost.cpp:236-265`) | Pay N energy counters. | `181-189` | Aether Chaser `...pay({e:2})...` `mtg.txt:1202` |
| `{emerge}` | `Offering` with `emerge=true` (`ExtraCost.h:335-344` / `ExtraCost.cpp:1346-1485`) | Emerge: sacrifice a creature, reduce this cost by its mana value. Default tc `creature|mybattlefield`. | `162-167` | Abundant Maw (Emerge {6}{B}) `mtg.txt:301` |
| `{h}` / `{h(target)}` | `BounceTargetCost` (`ExtraCost.h:285-291` / `ExtraCost.cpp:1006-1033`) | Return target to owner's hand. | `194-195` | Mina and Denn `auto={R}{G}{h(land|mybattlefield)}:...` `mtg.txt:73843` |
| `{l}` | `LifeCost` (`ExtraCost.h:85-92` / `ExtraCost.cpp:267-307`) | Pay **exactly 1** life (hard-coded `loseLife(1)`). | `202-205` | (pay-1-life costs) |
| `{l:N}` | `SpecificLifeCost` (`ExtraCost.h:95-105` / `ExtraCost.cpp:309-353`) | Pay N life. | `206-214` | Aetherflux Reservoir `auto={l:50}:...` `mtg.txt:1414` |
| `{l2e}` | `MillExileCost` (`ExtraCost.h:179-184` / `ExtraCost.cpp:623-645`) | Mill (library→**exile**) top card of your library. | `198-201` | Arc-Slogger `auto={R}{l2e}{l2e}...:...` `mtg.txt:4887` |
| `{d}` | `DiscardRandomCost` (`ExtraCost.h:123-130` / `ExtraCost.cpp:420-457`) | Discard a card at random. | `223-225` | Apathy `auto=teach(creature) {d}:...` `mtg.txt:4428` |
| `{discard(target)}` (any `{d...}` with target that isn't delve/`d`) | `DiscardCost` (`ExtraCost.h:133-139` / `ExtraCost.cpp:458-489`) | Discard a **chosen** card. | `227-229` | (targeted discard) |
| `{delve}` / `{delve(target)}` | `Delve` (`ExtraCost.h:315-323` / `ExtraCost.cpp:1208-1274`) | Exile cards from graveyard to pay {1} each of the generic cost. Default tc `*|mygraveyard`. | `217-221` | Become Immense `other={delve}` `mtg.txt:10168` |
| `{m}` | `MillCost` (`ExtraCost.h:169-176` / `ExtraCost.cpp:582-620`) | Mill (library→graveyard) top card of your library. One `{m}` per card milled. | `248-249` | Rot Farm Skeleton `autograveyard={2}{B}{G}{m}{m}{m}{m}:...` `mtg.txt:96869` |
| `{n}` | `Ninja` (`ExtraCost.h:294-302` / `ExtraCost.cpp:1035-1082`) | Ninjutsu: return an unblocked attacker you control to hand (only in blockers phase). tc forced `creature|myBattlefield`. | `252-257` | (ninjutsu cards) |
| `{kgoblin}` `{kfox}` `{kmoonfolk}` `{krat}` `{ksnake}` | `Offering` (non-emerge) (`ExtraCost.h:335-344` / `ExtraCost.cpp:1346-1485`) | Offering: sacrifice a creature of that subtype, reduce cost by its mana cost (Patron cycle). | `259-284` | Patron of the Nezumi `other={krat}` `mtg.txt:84827`; Patron of the Orochi `other={ksnake}` `mtg.txt:84840` |
| `{convoke}` / `{convoke(target)}` | `Convoke` (`ExtraCost.h:304-313` / `ExtraCost.cpp:1086-1206`) | Tap creatures to pay for colored/generic mana of this spell. Default tc `creature|mybattlefield`. | `325-330` | Autochthon Wurm `other={convoke}` `mtg.txt:7287` |
| `{improvise}` / `{improvise(target)}` | `Improvise` (`ExtraCost.h:325-333` / `ExtraCost.cpp:1276-1342`) | Tap artifacts to pay {1} each of generic cost. Default tc `artifact[-tapped]|myBattlefield`. | `297-303` | Barricade Breaker `other={improvise}` `mtg.txt:9303` |
| `{cycle}` | `CycleCost` (`ExtraCost.h:142-148` / `ExtraCost.cpp:491-522`) | Cycling: discard this card (fires discard + cycle events). | `336-339` | Absorb Vis `autohand={1}{B}{cycle}:...` `mtg.txt:254` |
| `{c(counterspec[,...,target])}` | `CounterCost` (`ExtraCost.h:346-358` / `ExtraCost.cpp:1526-1663`) | Add (nb≥0) or remove (nb<0) counters as a cost. Counter parsed via `AbilityFactory::parseCounter`. | `344-371` | (cumulative-upkeep / counter-removal costs) |
| `{unattach}` | `UnattachCost` (`ExtraCost.h:196-205` / `ExtraCost.cpp:683-730`) | Unequip/unattach this Equipment/Aura. | `381-385` | Blinding Powder `auto=teach(creature) {unattach}:...` `mtg.txt:11758` |

Non-token-built subclass: **`ExtraManaCost`** (`ExtraCost.h:64-73` / `ExtraCost.cpp:90-132`) — wraps a `ManaCost` as an extra cost; it is attached programmatically by abilities (not produced by a `{...}` token in the cost string).

Two switch cases are pure **guards** that emit no cost (they exist so `power(...)`/`compare(...)` target-filter substrings don't get mis-parsed): `p`→`{power...}` breaks (`ManaCost.cpp:289-290`); `c`→`{compare(...}` breaks (`ManaCost.cpp:323-324`).

---

## 3. Card-level alternative / additional cost fields

These are separate card keys, each parsed in `MTGDeck::parseLine`-style switch in `projects/mtg/src/MTGDeck.cpp`. Each stores a **whole ManaCost** (via `parseManaCost`) into a named member of the card's `ManaCost` (members declared/reset in `ManaCost::init()` `ManaCost.cpp:611-634`: `kicker`, `alternative`, `BuyBack`, `FlashBack`, `Retrace`, `morph`, `suspend`, `Bestow`). Because each calls `parseManaCost`, **every token from §1–§2 is legal inside them** (e.g. life/discard/sacrifice in a flashback cost).

| Card key | Stored as | Parse site | Notes / example |
|---|---|---|---|
| `kicker=...` | `setKicker()` | `MTGDeck.cpp:224-251` | Optional additional cost. Supports `multi` prefix (multikicker → `isMulti=true`) and `name(...)` label. Apex Hawks `kicker=multi{1}{W}` `mtg.txt:4448`. Paid at cast (`MTGRules.cpp:532-551`). |
| `other=...` | `setAlternative()` | `MTGDeck.cpp:274-300` | The "other" / **alternative** cost (paid instead of mana). `name(...)` supported. If key is `otherrestrictions` instead → `setOtherRestrictions`. Autochthon Wurm `other={convoke}` `mtg.txt:7287`. |
| `flashback=...` | `setFlashback()` | `MTGDeck.cpp:184-213` | Alternate cost to cast from graveyard (card then exiled). `name(...)` supported. Acorn Harvest `flashback={L:3}{1}{G}` `mtg.txt:723`. |
| `facedown=...` | `setMorph()` | `MTGDeck.cpp:189-194` | Morph face-down cost — same `f` case, chosen when the line contains `facedown`. Abomination of Gudul `facedown={3}` `mtg.txt:167`. |
| `buyback=...` | `setBuyback()` | `MTGDeck.cpp:134-140` | Additional cost that returns the spell to hand on resolve; in Wagic written as an alternate cost so it **includes the base mana**. Allay `buyback={1}{W}{3}` `mtg.txt:2725`. |
| `bestow=...` | `setBestow()` | `MTGDeck.cpp:118-127` | Aura-creature bestow cost. Baleful Eidolon `bestow={4}{B}` `mtg.txt:8511`. |
| `backside=...` | `primitive->backSide` (not a cost) | `MTGDeck.cpp:128-133` | Shares the `b` case; sets the flip/back face, not a mana cost. |
| `retrace=...` | `setRetrace()` | `MTGDeck.cpp:336-355` | Retrace cost (recast from graveyard by discarding a land). `name(...)` supported. Call the Skybreaker `retrace={5}{UR}{UR}{D(land|myhand)}` `mtg.txt:16490`. |
| `suspend(N)=...` | `setSuspend()` + `suspendedTime=N` | `MTGDeck.cpp:363-379` | Suspend cost with N time counters. Aeon Chronicler `suspend(0)={X}{3}{U}` `mtg.txt:1021`. |

The keyword-rule abilities that actually *offer* these at cast time live in `MTGAbility.cpp` (`kickerrule` 2079, `alternativecostrule` 2086, `buybackrule` 2093, `flashbackrule` 2100, `bestowrule` 2108, `retracerule` 2115, `suspendrule` 2122, `morphrule` 2129, `unearthrule` 2247). The internal cast-cost keyword list is `MTGAbility.cpp:36-44` (`kicker, alternative, buyback, flashback, retrace, suspended, bestow`).

---

## 4. Wiki discrepancies (`docs/wiki/CardCode.md`)

The wiki's cost documentation (mana section ~L140-167; auto-cost section ~L543-583) is a **small, partly-wrong subset** of the parser:

1. **Snow is `{i}`, not `{S}`.** Wiki L153 is correct that snow = `{i}`. Any assumption that snow uses the standard MTG `{S}` symbol is wrong here — in Wagic `{s}` is **Sacrifice**. (Facet-prompt's "snow {S}" is incorrect against source.)
2. **`{L}` vs `{L:N}` mislabeled.** Wiki L555-556 lists `{L:number}` twice ("life loss" and "specific life loss") and warns "only {L} causes bugs." Source: bare `{l}` is a valid cost = pay **exactly 1** life (`LifeCost`, hard-coded `loseLife(1)`); `{l:N}` = `SpecificLifeCost` for N life. Both are real; the true distinction is 1-life vs N-life, not "buggy vs specific."
3. **Massive omissions.** The wiki never documents: energy `{e:N}`, phyrexian `{p(color)}`, `{convoke}`, `{delve}`, `{improvise}`, offering (`{emerge}`, `{kgoblin/kfox/kmoonfolk/krat/ksnake}`), crew `{crew(...)}`, untap `{q}`/`{q(...)}`, `{saclands}`, `{cycle}`, `{unattach}`, counter costs `{c(...)}`, true-colorless `{c}`, `{chosencolor}`, `{mycost}`/`{myevictcost}`, and the dynamic `{value:...}`/`{eval(...)}` mana. All are live in `ManaCost.cpp:100-420`.
4. **`{N}` overloaded.** Wiki L559 documents `{N}` as "return an unblocked attacker" (Ninjutsu). But `{N}` where N is an integer is generic mana (default case). The ninja cost is actually keyed on token first-char `n` being non-numeric (`{n}`), while numeric tokens go to the generic/hybrid path — worth flagging as an ambiguous doc.
5. **`{M}` wording.** Wiki L558 calls `{M}` "puts a card from the top of your library" — vague; it mills (library→graveyard). Correct token, poor description.
6. **`mana=` "no extra costs" limitation (L167) is misleading.** The parser runs the full extra-cost switch on *every* field including `mana=`; extra-cost tokens in a `mana=` line **do** build ExtraCosts. The limitation is a stylistic convention, not a parser restriction.
7. **`{X:color}` (color-restricted X) is undocumented.** Wiki only mentions `{X}`/`{X}{X}` (L158). The `x:color` specific-X path (`ManaCost.cpp:107-123`) and its dedicated `Diff()` handling (`hasSpecificX`) are absent from the wiki.
8. **`other=` under-described.** Wiki "other (cost)" (L184-187) omits that the same key also parses `otherrestrictions` (`MTGDeck.cpp:276-279`) and that `other=` is the general alternative-cost slot used for convoke/delve/improvise/offering.

---

# Targets, Zones, Quantities, Player-Targets & Effect Modifiers

**Ground-truth files** (repo-relative): `projects/mtg/src/TargetChooser.cpp` (grammar), `projects/mtg/src/MTGAbility.cpp` (modifiers/wrappers), `projects/mtg/src/WParsedInt.cpp` (`prex`/`prexx`), `projects/mtg/bin/Res/sets/primitives/*.txt` (cards + `_macros.txt`). All target strings are parsed by `TargetChooserFactory::createTargetChooser(string, card, ability)` at `TargetChooser.cpp:12`.

The same grammar is used everywhere a target string appears: the card key `target=…`, and inside `auto=` rules as `target(…)`, `notatarget(…)`, `lord(…)`, `all(…)`, `foreach(…)`, `aslongas(…)`, `this(…)`, cost sub-targets `{S(…)}`/`{T(…)}`/`{E(…)}`/`{H(…)}`/`{discard(…)}`, and trigger targets `@movedTo(…)`, `@damaged(…)`, etc.

---

## 1. `target()` grammar & type selectors

Overall shape (`TargetChooser.cpp:263–549`):

```
[<quantity>] [other ] type1[,type2…][[restrictions]] [|zone1[,zone2…]]
```

The string is split on `|` into a **type part** (`s1`) and a **zone part** (`TargetChooser.cpp:264–475`). If there is no `|`, zones default to `{MY_BATTLEFIELD, OPPONENT_BATTLEFIELD}` (`:476–482`). The type part is then comma-split into type tokens (`:536–549`), each optionally carrying a `[…]` restriction block.

**Type selectors** (evaluated left-to-right against each token):

| Token | Meaning | file:line |
|---|---|---|
| any type/subtype word (e.g. `creature`, `goblin`, `land`, `artifact`, `aura`) | matches that type/subtype; builds a `TypeTargetChooser` | `:1432`, matching in `TypeTargetChooser::canTarget :1808` |
| `*` | any card (a pure `TargetZoneChooser`, i.e. "anything in the zone") | `:1408–1411` |
| `this` | the source card itself (`CardTargetChooser` bound to `card`) | `:1412–1415` |
| `sourcecard` | the source card, in **all** zones (`setAllZones`) | `:1416–1421` |
| `mysource` | `card->storedSourceCard` (the real granting card for granted abilities) | `:1422–1425` |
| `mytgt` / `mytgtforced` | the source card's current target (or the ability's target unless `forced`) | `:64–72` |
| multiple types via comma | e.g. `creature,planeswalker` → adds each type (`addType`) | `:1437`, `TypeTargetChooser :1748–1773` |

**Separators inside the type part:**
- `,` — token separator (OR across type tokens) — `:538`.
- `^` — **alias for `,`**, both in the type part (`replace(typeName…'^'…',' )` at `:552`) and in the zone part (`:272`). Used so a card name containing a comma can be targeted, or to list multiple zones. **Wiki does not document `^`.**
- `|` — separates types from zones — `:264`.

`other` / `another`: the literal token `other ` (with trailing space) anywhere in the string sets the `other` flag and is erased (`:100–105`); it excludes the source card and its previous incarnations from legal targets (`TargetChooser::canTarget :1526–1534`). Example: `target(other creature|mybattlefield)` — **Adarkar Valkyrie**, `mtg.txt:838`.

**Special whole-string chooser keywords** (checked before the generic grammar; each returns a bespoke chooser):

| Keyword | Chooser / meaning | file:line |
|---|---|---|
| `blockable` | creatures the source can block (`BlockableChooser`) | `:21–26` |
| `pairable` | Soulbond partners | `:28–33` |
| `dredgeable` | dredge candidates in your graveyard | `:35–41` |
| `mychild` | Aura children of source | `:43–48` |
| `mytotem` | totem-armor Aura on source | `:50–55` |
| `myeqp` | Equipment attached to source | `:57–62` |
| `mycurses` | Curses attached to controller (unlimited) | `:242–247` |
| `proliferation` | proliferate chooser (unlimited) | `:249–254` |
| `propagation` | proliferate chooser (1 target) | `:256–261` |
| `children` / `parents` | `ParentChildChooser`, may nest a `[…]` deeper TC | `:524–534` |
| `anytarget` | any **damageable** (player, creature, planeswalker, battle); supports `<…>` quantity | `:202–240` |
| `trigger` / `trigger[to]` / `trigger[from]` | the triggering card/event; `trigger[to]<1>` caps at 1 | `:107–127` |

Example `anytarget`: `prevent:1 target(anytarget)` — **Abuna Acolyte**, `mtg.txt:271`.

**ID-fallback choosers** (when the string can't be parsed, dispatch by card id — `TargetChooser.cpp:1445–1497`): Spell counter (Spellblast id 1224 → `SpellTargetChooser`), Spell-or-Permanent laces (`:1470–1477`), color-filtered spell/permanent (Blue/Red Elemental Blast `:1479–1487`), damage-history (Eye for an Eye `:1489`).

---

## 2. Zones

The zone part (after `|`) is comma/`^`-split; each name maps to one or more `MTGGameZone` ids (`TargetChooser.cpp:285–473`). Bare (unprefixed) multi-player names expand to BOTH players' zones; `my`/`opponent` prefixes restrict to one side.

**Simple / both-sides names:**

| Zone word | Expands to | file:line |
|---|---|---|
| `*` | `ALL_ZONES` | `:288–291` |
| `battlefield` / `inplay` | MY + OPP battlefield | `:312–316` |
| `graveyard` | MY + OPP graveyard | `:307–311` |
| `hand` | MY + OPP hand | `:317–321` |
| `library` | MY + OPP library | `:327–331` |
| `exile` | MY + OPP exile | `:350–354` |
| `stack` | MY + OPP stack | `:345–349` |
| `reveal` | MY + OPP reveal | `:302–306` |
| `sideboard` | MY + OPP sideboard | `:292–296` |
| `commandzone` | MY + OPP command zone | `:297–301` |

**Prefixed single zones** are resolved by `MTGGameZone::zoneStringToId(zoneName)` in the `else` branch (`:469–472`) — the prefixes `my`, `opponent`, `owner`, `targetcontroller`, `targetowner` fuse with the zone name into one word (e.g. `mybattlefield`, `opponentgraveyard`, `mylibrary`, `myhand`, `myexile`, `mystack`, `mygraveyard`, `myreveal`, `mysideboard`, `mycommandzone`, and the `opponent…` counterparts). Prefixes are always from the perspective of the source card's controller.

**Composite / macro zones** (single tokens expanding to a fixed set — none of these are in the wiki):

| Token | Expands to | file:line |
|---|---|---|
| `mybattlefieldhand` | MY hand + MY battlefield | `:322–326` |
| `nonbattlezone` | both graveyards, libraries, hands, exiles, command zones | `:332–344` |
| `mycastingzone` | MY graveyard, library, hand, exile, command zone | `:355–362` |
| `myrestrictedcastingzone` | MY hand + command zone | `:363–367` |
| `mycommandplay` | MY battlefield + command zone | `:368–372` |
| `myhandlibrary` | MY hand + library | `:373–377` |
| `mygravelibrary` | MY graveyard + library | `:378–382` |
| `opponentgravelibrary` | OPP graveyard + library | `:383–387` |
| `mygraveexile` | MY exile + graveyard | `:388–392` |
| `opponentgraveexile` | OPP exile + graveyard | `:393–397` |
| `opponentcastingzone` | OPP graveyard, library, hand, exile, command zone | `:398–405` |
| `opponentrestrictedcastingzone` | OPP hand + command zone | `:406–410` |
| `opponentcommandplay` | OPP battlefield + command zone | `:411–415` |
| `opponenthandlibrary` | OPP hand + library | `:416–420` |
| `mynonplaynonexile` | MY graveyard, library, hand, command zone | `:421–427` |
| `opponentnonplaynonexile` | OPP graveyard, library, hand, command zone | `:428–434` |
| `myhandexilegrave` | MY graveyard, exile, hand | `:435–440` |
| `opponenthandexilegrave` | OPP graveyard, exile, hand | `:441–446` |
| `myzones` | ALL of MY zones (bf, stack, gy, lib, hand, exile, sideboard, cmd) | `:447–457` |
| `opponentzones` | ALL of OPP zones | `:458–468` |

Zone membership is checked in `TargetZoneChooser::canTarget` (`:1988–2010`); `ALL_ZONES` always matches (`:1995`).

---

## 3. Quantity prefixes & how `maxtargets` is evaluated

The quantity prefix is a `<…>` block at the **start** of the type part (parsed at `TargetChooser.cpp:489–522`; the identical logic for `anytarget` is at `:207–238`). Default `maxtargets = 1`, `targetMin = false` (`:485–486`).

Inside `<…>`:
- `<N>` (plain integer, e.g. `<3>`) → `maxtargets = N` **and `targetMin = true`** — you must choose **exactly/at least** N (the min-count is enforced) — `:504–518`.
- `<upto:N>` → `maxtargets = N`, `targetMin` stays **false** — "up to N" (0…N legal) — `:497–502`.
- `<anyamount>` → `maxtargets = TargetChooser::UNLITMITED_TARGETS`, `targetMin = false` — `:508–512`.
- The value `N` is itself run through `WParsedInt` (`:515`), so it may be a **variable**, not just a literal — including the two X-prefixes below.

**`<prex>` = the spell's X** (`WParsedInt.cpp:182–194`): if `card->setX > -1` (X already announced) → that value; otherwise the leftover mana pool minus the card's mana cost (the X being paid). So `target=<prex>creature` means "target X creatures." Real cards: `target=<prex>*|myhand` — **Abandon Hope**, `borderline.txt:56`; `target=<prex>land[snow]` — **Avalanche**, `mtg.txt:7396`.

**`<prexx>` = X for `{X}{X}` costs** (`WParsedInt.cpp:195–208`): same as `prex` but halved (the `{X}{X}` announce menu indexes by total). Prevents X=0 casts from still targeting.

`<prex>` composes with `upto:`: `target=<upto:prex>artifact` = "up to X artifacts" — **Builder's Bane**, `mtg.txt:15579`. Literal example: `target=<upto:2>creature` — **Abandon Reason**, `mtg.txt:6`.

**`player`-target quantity** (`:129–200`): the `player` branch reads its own `<N>` (via `WParsedInt`, `:133–142`) and `<anyamount>` (`:143–144`) for `maxtargets`.

**maxtargets evaluation / completeness** — `TargetChooser::targetsReadyCheck()` (`:1591–1618`):
- `targetMin == false` + 0 targets + player forced-finish → `TARGET_OK_FULL` (empty allowed for `upto:`/`anyamount`) — `:1593`.
- `maxtargets == 0` → complete from birth (an X=0 cast) — `:1597–1604`.
- 0 targets otherwise → `TARGET_NOK`; `full()` → `TARGET_OK_FULL`; not `ready()` → `TARGET_OK_NOT_READY` — `:1605–1617`.
- `targetMin` (set by plain `<N>` / bare `s`-plural) is what forces the min-count; `upto:`/`anyamount` clear it.

---

## 4. Player targets & `foreach` / `this`

**Player-target keywords** — each must appear at **position 0** of the string (`found == 0`) to match, and returns a `PlayerTargetChooser` (`TargetChooser.cpp:2064`):

| Keyword | Player | file:line |
|---|---|---|
| `controller` | source's controller | `:92–98` |
| `opponent` | source's controller's opponent | `:84–90` |
| `targetedplayer` | the card's / ability's stored `playerTarget` | `:74–82` |
| `player` | any player (+ optional `<N>`/`<anyamount>`; also merges with damageable types) | `:129–200` |
| `abilitycontroller` | `storedSourceCard->controller()` | `:1426–1429` |

`PlayerTargetChooser::canTarget` also enforces `CONTROLLERSHROUD` / `PLAYERSHROUD` (`:2069–2087`). `player` combined with `creature`/`planeswalker`/`battle` (in any order/separator) yields a `DamageableTargetChooser` covering player + those permanent types (`:145–199`) — this is how "any target"-style damage is coded.

**As `auto=` playertarget** (the trailing player word on an effect line, per wiki §auto playertarget): `controller`, `opponent`, `targetcontroller`. When omitted the engine infers from the `target=`/`target()`.

**`foreach(targets) effect [other] [<n|>n]`**: applies `effect` once per matching card (`MTGAbility.cpp` parses `foreach(`; grammar per wiki §auto effects). The target string inside is the full grammar above. Real cards: `foreach(Accumulated Knowledge|graveyard) draw:1` — **Accumulated Knowledge**, `mtg.txt:605`; `foreach(creature[attacking]|myBattlefield) 1/0 ueot` — **Akroan Hoplite**, `mtg.txt:2192`. Siblings `lord()`, `all()`, `aslongas()` share the same target-string grammar.

**`this(...)`**: as a *target selector* `this` = the source card (`:1412`). As an *auto-condition wrapper* `this(<expr>)` gates the effect on a condition about the source (e.g. `this(counter{1/1}<1)`, `this(variable{opponentturn})`, `this(cantargetcard(...))`) — see `_ADAPT1_`, `_RENOWN_` in `_macros.txt:98,204`.

---

## 5. Restriction brackets `[...]`

A `[...]` block after a type token builds a `CardDescriptor` (`TargetChooser.cpp:553–1387`). Multiple restrictions inside are separated by:
- `;` → `CD_OR` mode (`:565–570`)
- `&` → `CD_AND` mode (`:571–576`) — **wiki omits `&`; it documents only `;`.**
- a leading `-` on any restriction means "not" and forces overall `CD_AND` (`:582–588, 1386–1387`).

**Comparison operators** on numeric restrictions (`:592–637`): find `=`, then look at the char before it — `<` → `AT_MOST` (`>` if minus-negated), `>` → `AT_LEAST` (`<` if minus-negated), else `EQUAL` (`UNEQUAL` if minus). So the real operators are `=`, `<=`, `>=` (and negated forms). The RHS is a `WParsedInt`, so it can be dynamic (e.g. `manacost<=power`, Dreadhorde Arcanist `:1113–1122`; `toughness=toughness:lowest:creature:battlefield`, Purging Scythe `:1091–1100`).

**Restriction keywords** (each maps to a `CardDescriptor` field):

Combat/state: `attacking` (`:641`), `blocking` (`:653`), `blocked` (`:665`), `tapped` (`:677`), `fresh` (put in zone this turn, `:869`), `recent` (`:880`), `modified` (`:749`), `damaged` (`:938`), `enchanted` (`:926`), `geared`/`equipped` (`:891`), `attached` (`:902`), `token` (`:797`), `isflipped` (`:809`), `permanent` (`:785`), `leveler` (`:914`), `iscommander` (`:821`), `ringbearer` (`:833`).

Cost/keyword-presence: `foretold` (`:689`), `kicked` (`:695`), `haskicker` (`:701`), `hasconvoke` (`:713`), `hasflashback` (`:725`), `hasbackside` (`:737`), `hastoxic` (`:761`), `haspartner` (`:773`), `hasx` (`:845`), `discarded` (`:857`).

Damage-history: `damager` (`:974`), `opponentdamager` (`:950`), `controllerdamager` (`:962`).

Mana production: `cmana`,`manag`,`manau`,`manar`,`manab`,`manaw` (can-produce colorless/G/U/R/B/W, each sets `CD_OR`) (`:986–1057`).

Numeric: `power` (`:1080`), `toughness` (`:1086`), `manacost` (`:1108`), `zpos` (library position, `:1102`), `numofcols` (`:1074`), `counter{name}`/`counter{any}`/`counter{notany}` (`:1199–1232`).

Color/name: bare color words `white/blue/black/red/green/artifact` (`:1238–1248`), `colorless` (`:1250`), `multicolor` (`:1058`), `chosencolor` (`:1264`), `chosentype` (`:1273`), and name-compare tokens `chosenname`,`lastnamechosen`,`evictname`,`backname`,`partname`,`storedname`,`preyname` (`:1286–1354`), plus the `share!…!`/`notshare!…!` "match the current target's name/color/types" operator (`:1124–1198`).

Fallthrough: unmatched tokens are tried as basic-ability names (`:1359–1369`), then as subtypes (`:1372–1383`).

⚠ **Ability/creature-type name collisions.** That fallthrough order means a creature TYPE whose name is also a basic ability is read as the ABILITY and never as the type. `hydra` is exactly that (`Constants::HYDRA`, `MTGDefinitions.cpp:126`), so `creature[-hydra]` silently meant "without the hydra ability", not "not a Hydra" — which is what made Wildwood Scourge trigger off Mossborn Hydra. Prefix the token with **`sub_`** to force the subtype reading and skip every keyword scan: `from(other creature[-sub_hydra]|mybattlefield)` (`TargetChooser.cpp`, in the `[` attribute loop, right after the `-` strip). The separator is `_` deliberately: `:` ends the trigger text in `parseMagicLine` and `=` is this parser's comparison operator.

Examples: `destroy target(creature[-black;-artifact])` (Terror pattern) — `mtg.txt:13585`; `target=creature[-black;-artifact]` — **Expunge**, `mtg.txt:37967`; `destroy target(creature[flying]|opponentbattlefield)` — `mtg.txt:4812`.

---

## 6. Duration / scope modifiers

Parsed as flags in `MTGAbility.cpp` (main block `:4326–4344`). They tell the factory whether the produced effect ends, and when. For instants/sorceries the engine defaults to UEOT unless one of the "longer" flags is set (`:4940`, `:5576`, etc.).

| Keyword | Flag | Meaning | file:line |
|---|---|---|---|
| `ueot` | `forceUEOT` | until end of (current) turn | `:4330–4332` |
| `forever` | `forceForever` | permanent (never auto-removed) | `:4336–4338` |
| `uynt` | `untilYourNextTurn` | until the controller's next turn | `:4339–4341` |
| `uent` | `untilYourNextEndTurn` | until the controller's next end step | `:4342–4344` |
| `oneshot` | `oneShot` | resolves once then is removed (used with `fog`, etc.) | `:4333–4335` |

Additional lifetime/scope modifiers:

- **`ueot ` as a leading wrapper**: if the string *starts* with `ueot ` (`s.find("ueot ")==0`), it wraps the rest as a `GenericInstantAbility` (UEOT) — `:2874–2882`.
- **`limit:N`** — caps activations per turn. Parsed from `sWithoutTc.find("limit:")` and stored on the ability (`amp->limit`, generic `->limit`) — `:2690–2695`. Also `limit^side^uses^` form for a side-effect after N uses (`:2670–2681`). Example: `{S(land|mybattlefield)}:draw:2 controller limit:1` — **Aggressive Mining**, `mtg.txt:1752`.
- **`nonstatic`** — dynamic P/T applied **on top of** base P/T (higher priority); produces `PTInstant`/`APowerToughnessModifier` with the nonstatic flag — `:5647–5669`.
- **`cdaactive`** — dynamic P/T applied **directly as** the card's base P/T (lowest priority; characteristic-defining) — `:5658–5667`.
- **`phaseaction[...]` / `phaseactionmulti`** — schedules a delayed effect at a future phase (`parsePhaseActionAbility`) — `:3477–3481`, `:2927–2931`. The bracket carries phase + qualifiers, e.g. `phaseaction[end once checkex]` (fire at end step, once) — see `_BLINK_UEOT_`, `_macros.txt:208`; `phaseaction[myupkeep next once sourceinplay]` — `_GOAD_`, `_macros.txt:25`.
- **`activate `** (leading) → `GenericAddToGame` (adds the ability into the game) — `:2885–2893`. **`emblem `** (leading) → permanent extra-rule effect — `:2896–2904`. `forcedalive`/`dontremove` keep an ability from being cleaned up — `:2936–2942`.

Activation-time restrictions (per wiki §auto restrictions, applied to costed abilities): `myturnonly`, `assorcery`, `my[phase]only` (e.g. `myupkeeponly`), plus non-activated `this(variable{controllerturn})` / `this(variable{opponentturn})`.

---

## 7. The inline-ability wrapper `ability$! … !$` and `and!( … )!`

**`ability$! … !$`** (`MTGAbility.cpp:2448–2466`): text between `ability$!` and the matching `!$` is lifted out into `storedAbilityString` and erased from the working line, so a **granted/nested ability** (its own full auto-line, including its own duration/target/playertarget) can ride inside another effect verbatim. The factory searches the mutated string first, then the unchanged copy (`:2456–2465`). It is the vehicle for grant-style keywords (`grant`, `becomes`, `transforms`, `reveal`, `scry`, `pay`) that need to carry a whole sub-ability — the parser explicitly checks for `ability$!` alongside `transforms((` at `:2316, 2345, 2368, 2389, 2409, 2427`.

Real uses in `_macros.txt`:
- `_MANAOFANYCOLOR_`: `ability$! choice Add{W} _ choice Add{U} _ … !$ controller` (`:243`) — the `_`-separated choices are the wrapped payload; `controller` is the outer playertarget.
- `_MUST_BE_BLOCKD_`: `…:ability$! notatarget(creature[-tapped]|myBattlefield) transforms((,newability[mustblock])) ueot!$ opponent` (`:229`) — a whole targeted, UEOT, transforms-granting ability nested inside a trigger, aimed at `opponent`.

**`and!( … )!`** (`:2497–2536`): lifts a **follow-up ability** into `storedAndAbility`, matching the closing `)!` at **depth 0** so nested `and!( )!` (e.g. Doomsday's per-pick chain) don't truncate it. Widely used to chain a second effect after a token/move, e.g. `_TREASURE_ token(Treasure…) and!( transforms((,newability[{T}{S}:Add{W}]…)) forever )!` (`_macros.txt:387`).

Related grant/dynamic markers: **`newability[ … ]`** — a granted sub-ability slot inside `transforms((…))` (`:2481`); **`dynamicability<! … !>`** — a runtime-resolved ability payload (`:3286`, e.g. `_PUNCH_`/`_FIGHT_`, `_macros.txt:223,226`); **`pay[[ … ]]`** — a may-pay sub-cost preserved for later-firing grants (`:2468–2495, 3484–3491`).

---

## 8. Card-script macros (`#AUTO_DEFINE`)

Defined in `projects/mtg/bin/Res/sets/primitives/_macros.txt`. Rules (`_macros.txt:1–7`): macros are **global** (definable anywhere), **case-insensitive**, replaced by exact-substring text substitution; names must be unique-and-delimited (convention: bracket with `__`/`_`) because `MACRO` would also match inside `MACRO2`; a parameter must not contain `()`.

Syntax: `#AUTO_DEFINE __NAME__([$param…]) <replacement auto-code>` — the card writer then writes `auto=__NAME__(args)` and the engine expands it to the replacement, substituting `$param`.

Representative macros (all `_macros.txt`): `__CYCLING__($cost)` `:10`, `__BASIC_LANDCYCLING__($cost)` `:13`, `_DIES_` `:16`, `_TRAINING_` `:19`, `_GOAD_` `:25`, `_REBOUND_` `:28`, `_ATTACKING_`/`_BLOCKED_` `:37,40`, `_HEROIC_`/`_VALIANT_` `:43,46`, `_LANDFALL_` `:52`, `_CONSTELLATION_` `:58`, the `_AMASSZOMBIE1..5_`/`_AMASSORC…_`/`_AMASSSLIVER…_` families `:61–79`, `_SCRY1..5_` `:82–86`, `_SURVEIL1..3_` `:157–161`, `_BLINK_UEOT_` `:208`, `_CREW1_/_CREW2_/_CREW2COMPLEMENT_` `:250–254`, `_ENDURE1..20_` `:257–276`, `_FORETELL_`/`_PLOT_`/`_PLOTCAST_` `:176–182`, `_UNEARTH_` `:188`, `_MANIFEST_DREAD_` `:246`, and the large `create(...)`/`token(...)` token library `:284–390` (`_ANGELTOKEN_`, `_TREASURE_`, `_CLUE_`, `_FOOD_`, `_BLOOD_`, `_VEHICLE_`, etc.). All shipped definitions are **commented with `#`** in this file (they are examples/registry, activated where the build wires them in).

---

## Wiki discrepancies (`docs/wiki/CardCode.md`)

The wiki is roughly correct on the *concepts* but materially incomplete and has several errors vs. the parser:

1. **Quantity prefixes entirely undocumented.** Wiki §target only mentions the trailing `s` plural ("1 or more", `CardCode.md:404`). It never documents the `<N>`, `<upto:N>`, `<anyamount>`, `<prex>`, `<prexx>` prefix syntax — the actual mechanism in `TargetChooser.cpp:489–522` — nor that plain `<N>` sets a *minimum* (`targetMin`) while `upto:` does not.
2. **Comparison-operator typo.** Wiki lists `=`, `<=`, `<=` ("greater than or equal") at `CardCode.md:445` — the second should be `>=`. Parser confirms `<=`/`>=`/`=` at `TargetChooser.cpp:602–635`.
3. **`^` separator unmentioned.** The `^`-as-`,` alias (`:272, 552`) is used throughout primitives (e.g. token defs `Zombie Army^Creature Zombie Army^0/0^black`) but absent from the wiki.
4. **`&` AND-separator unmentioned.** Wiki §target-restrictions (`:443`) explains `;` and minus-sign semantics only; the explicit `&` → `CD_AND` operator (`:571`) is missing.
5. **Zone list drastically incomplete.** Wiki §target-zones (`:463–487`) lists only `battlefield, graveyard, library, hand, stack, exile, *` plus the 5 prefixes. It omits `reveal`, `sideboard`, `commandzone`, and every composite zone (`mycastingzone`, `nonbattlezone`, `mybattlefieldhand`, `myzones`, `opponentzones`, `mygravelibrary`, `myhandexilegrave`, … — ~20 tokens at `:322–468`).
6. **Special choosers undocumented.** `anytarget`, `other`, `trigger[to]/[from]`, `mysource`, `sourcecard`, `abilitycontroller`, `targetedplayer`, and the whole-string choosers (`blockable`, `pairable`, `dredgeable`, `mychild`, `mytotem`, `myeqp`, `mycurses`, `proliferation`, `propagation`, `children`/`parents`) are absent.
7. **Restriction-keyword list is a small subset.** Wiki §target-restrictions (`:421–458`) lists ~15 keywords; the parser recognizes ~50 (`TargetChooser.cpp:641–1354`), including all the `has*`, damage-history, mana-production, counter, name-compare, `zpos`, `numofcols`, `token`, `fresh`/`recent`, `iscommander`, `ringbearer`, etc.
   - Wiki's color-combo keywords `blackandgreen`, `blackandwhite`, `redandblue`, `blueandgreen`, `redandwhite` (`:433–438`) have **no matching branch** in this parser's restriction loop (colors are matched individually via `MTGColorStrings`, `:1238`). Treat these as **unverified/possibly legacy** until located elsewhere.
8. **Duration flags incomplete.** Wiki §restrictions documents `ueot`, `limit`, phase restrictions, `nonstatic`/`cdaactive` — but omits `forever`, `oneshot`, `uynt` (until-your-next-turn), `uent` (until-your-next-end-turn), and the `phaseaction[...]`/`activate`/`emblem` wrappers (`MTGAbility.cpp:4336–4344, 2874–2904, 3477`).
9. **Inline-ability wrappers undocumented.** `ability$! … !$`, `and!( … )!`, `newability[…]`, `dynamicability<! … !>`, `pay[[ … ]]` — all core to modern card code and pervasive in `_macros.txt` — appear nowhere in the wiki.
10. **Wiki freshness.** Header says "current as of 10/22/2015" (`:10`); the parser has years of additions beyond it. Treat the wiki as a conceptual primer only; the parser (`TargetChooser.cpp` / `MTGAbility.cpp`) is authoritative.

---

# Triggered Abilities — the `@…` Trigger Vocabulary

**Scope of this facet:** everything the parser recognizes between the `@` delimiter and the terminating `:` of a triggered ability. Ground truth is `AbilityFactory::parseTrigger` (`projects/mtg/src/MTGAbility.cpp:1241–2009`), the trigger classes in `projects/mtg/include/AllAbilities.h` / `projects/mtg/include/MTGAbility.h`, and the `WEvent*` types in `projects/mtg/include/WEvent.h`.

## How a trigger is parsed (mechanics you must know first)

A triggered ability is written `auto=@<trigger>: <effect>`. The dispatcher splits on the delimiters:

```
vector<string> splitTrigger = parseBetween(s, "@", ":");   // MTGAbility.cpp:2538
trigger = parseTrigger(splitTrigger[1], ...);              // text between @ and : is the trigger
// splitTrigger[2] (after the :) is parsed as the effect ability
```

So `parseTrigger` receives the trigger text **with the leading `@` already stripped**. Every keyword below is matched as a **plain substring** of that text (`s.find(...)`), and target-bearing forms are matched by `parseSimpleTC(s, "<keyword>", card)`, which searches for the literal string **`"<keyword>("`** and reads the target spec up to the matching `)` (`MTGAbility.cpp:1210–1236`).

**Two correctness consequences of substring dispatch (order-dependent):**
1. Keywords that are substrings of other keywords are checked **longest/most-specific first**. `untapped(` is tested before `tapped(` (line 1317 vs 1321) because `"untapped("` contains `"tapped("`. Likewise `noncombatdamage*` and `combatdamage*` are tested before plain `damage*`; `totalcounteradded(` before `counteradded(`. Reordering these would misclassify triggers.
2. The trailing `(` in `parseSimpleTC` is the disambiguator: `@tappedformana(...)` does **not** match the `tapped(` probe because the literal is `tappedformana(`.

**Trigger firing engine** (`Trigger::triggerOnEvent`, `MTGAbility.cpp:8019`): every event trigger listens for a broadcast `WEvent`, and `triggerOnEventImpl` decides if this event matches. It fires **once per matching event broadcast**. If `once` was set, the trigger self-deactivates after the first fire. Phased-out sources never fire. `castRestriction` (see §3) is re-checked at fire time.

---

## 1. Phase-based triggers — the `@each` / `@next` family

**Syntax** (`parseTrigger` lines 1964–2000):
```
@[next|each] [my|opponent|targetcontroller|targetedplayer] <phase> [sourcenottapped] [foelost(N)]: <effect>
```

The parser first reads the player-scope `who` from **space-prefixed** tokens (lines 1964–1972), then scans for the first phase code name present:

| Token in script | `who` | Meaning |
|---|---|---|
| ` my` | 1 | fires only on the card controller's turn |
| ` opponent` | -1 | fires only on the opponent's turn |
| ` targetcontroller` | -2 | fires on the turn of the target's controller (falls back to controller) |
| ` targetedplayer` | -3 | fires on the turn of the stored `playerTarget` |
| *(none)* | 0 | fires on **either** player's turn |

**Recognized phase names** — the complete ground-truth list is `Constants::MTGPhaseCodeNames[]` (`MTGDefinitions.cpp:322–338`), matched by substring in index order:

```
beginofturn  untap  upkeep  draw  firstmain  combatbegins
attackers  blockers  combatdamage  combatends  secondmain
end  cleanup  beforenextturn
```

- **`@each <phase>` → `TriggerAtPhase`** (`AllAbilities`/`MTGAbility.h:418`; fire logic `MTGAbility.cpp:8257`). Fires **every time** that phase begins, on each qualifying turn, for the life of the ability. Fire condition: `currentPhase != newPhase && newPhase == phaseId`, gated by `who`, then by `sourceUntapped`/`sourceTap`/`lifelost` and `castRestriction`.
- **`@next <phase>` → `TriggerNextPhase`** (`MTGAbility.h:434`; `testDestroy` at `MTGAbility.cpp:8339`). Fires the **next** occurrence of that phase, then self-destroys. Used for "until end of turn, return it" chains.
- **`@rebounded` → `TriggerRebound`** (`MTGAbility.h:447`; `MTGAbility.cpp:8364`, parsed at 2003). A specialized next-controller's-upkeep trigger for the Rebound keyword (fixed to `phaseId=2` upkeep, `who=1`). Fires once on the controller's next upkeep, then destroys.

**Examples:**
- `auto=@each my upkeep:...` — `mtg.txt:97` (Depletion-style upkeep trigger)
- `auto=@each my firstmain sourcenottapped:all(artifact|mybattlefield) add{1}` — `mtg.txt:11810`
- `auto=@each opponent firstmain sourcenottapped:...` — `mtg.txt:11811`
- `@each combatbegins:...` — `planeswalkers.txt:381`
- `@next end:moveTo(previousbattlefield)` (treason/borrow return) — `planeswalkers.txt:217`
- `@next opponent end:...` — `planeswalkers.txt:218`
- `@rebounded:may name(Cast rebounded card) activate castcard(normal)` — `_macros.txt:28` (`_REBOUND_`)

Note `foelost(N)` (parsed at line 1253–1254 into `lifelost`/`lifeamount`) is a phase-trigger condition: the phase trigger only fires if the opponent lost at least N life this turn (`MTGAbility.cpp:8261–8266`).

---

## 2. Event triggers — the `@<event>(target)` forms

All of these bind a `TargetChooser` via `parseSimpleTC` and listen for a specific `WEvent`. Unless noted, they fire **once per matching event**, on either player's turn (scope comes from the target spec or the `of`/`foeof` suffix, not from `my`/`opponent`).

### Zone change
| Trigger syntax | Class → WEvent | Firing semantics |
|---|---|---|
| `@movedto($target) [from($zone)]` | `TrCardAddedToZone` (AllAbilities.h:160) → `WEventZoneChange` (WEvent.h:40) | Fires when a card matching `$target` **enters** the target's zone (optionally only if it came from `from(...)`). First arg is a **full target spec**, not just a zone, so `@movedto(goblin|battlefield)` = "whenever a Goblin ETBs". `from`/`to` default to all zones. This is how ETB, dies (→graveyard), and "when cast" (→stack) are all expressed — **there is no separate `@dies`, `@cast`, or `@etb` keyword.** `dontremove` (line 1255) sets `forcedAlive`. |

**Examples:** `auto=@movedto(*[cycling]|mygraveyard) from(...):...` — `borderline.txt:83`; `@movedto(creature[-angel]|graveyard) from(mybattlefield) once:...` — `mtg.txt:5166`; cast-detection `auto=@movedTo( *|stack[white]: may life:1 controller` — wiki `CardCode.md:767`.

### Tap / mana / face / phase / exert
| Syntax | Class → WEvent | Semantics |
|---|---|---|
| `@untapped($tc)` | `TrCardTapped(...,false)` (231) → `WEventCardTap` (WEvent.h:150) | Fires when a matching card becomes **untapped**. Checked before `tapped` (substring guard). |
| `@tapped($tc)` | `TrCardTapped(...,true)` → `WEventCardTap` | Fires when a matching card becomes **tapped**. |
| `@tappedformana($tc)` | `TrCardTappedformana` (264) → `WEventCardTappedForMana` (158) | Fires when a card is tapped specifically to produce mana. |
| `@producedmana($tc)` | `TrCardManaproduced` (295) → `WEventCardManaProduced` (165) | Fires when a card produces mana. |
| `@transformed($tc)` | `TrCardTransformed` (379) → `WEventCardTransforms` (362) | Fires when a card transforms (DFC). |
| `@facedup($tc)` | `TrCardFaceUp` (351) → `WEventCardFaceUp` (356) | Fires when a face-down card turns face up (morph/manifest). |
| `@phasedin($tc)` | `TrCardPhasesIn` (323) → `WEventCardPhasesIn` (350) | Fires when a card phases in. |
| `@exerted($tc)` | `TrCardExerted` (407) → `WEventCardExerted` (238) | Fires when a creature is exerted. |

**Examples:** `@tapped(this):...` — `borderline.txt:3499`, `:5228`; `@untapped(this):...` — `mtg.txt:1126`, `:4801`.

### Combat — the `@combat(...)` multiplexer
```
@combat(<trait>[,<trait>...]) source($tc) [from($tc)] [turnlimited] [sourcenottap] [opponentpoisoned]: <effect>
```
`TrCombatTrigger` (AllAbilities.h:435; parsed at `MTGAbility.cpp:1350–1384`). **`source(...)` is required**; if absent the parser returns NULL (no trigger). Recognized combat traits (line 1364–1373), each binding a distinct WEvent:

| Trait | WEvent | Fires when… |
|---|---|---|
| `attacking` | `WEventCreatureAttacker` (173) / `WEventCardAttacked` (180) | source is declared as an attacker |
| `attackedalone` | `WEventCardAttackedAlone` (186) | source attacked as the only attacker |
| `notblocked` | `WEventCardAttackedNotBlocked` (192) | attacking source is unblocked |
| `blocked` | `WEventCardAttackedBlocked` (198) / `WEventCardBlocked` (205) | attacking source becomes blocked |
| `blocking` | `WEventCreatureBlocker` (246) | source is declared as a blocker |

**Examples:** `@combat(attacking) source(this) restriction{trainer}:...` — `_macros.txt:19` (`_TRAINING_`); `@combat(blocked,turnlimited) source(this):...` — `_macros.txt:40` (`_BLOCKED_`); `@combat(attacking) source(this):` — `_macros.txt:37` (`_ATTACKING_`).

> Wiki lists `@attacking`, `@blocking`, `@blocked`, `@notblocked`, `@attackedalone` as if they were top-level keywords (`CardCode.md:599–616`). In the current parser they are **traits inside `@combat(...)`**, not standalone keywords — there is no `parseSimpleTC(s,"attacking",...)` branch. Real cards always use `@combat(...)` (confirmed: zero `@attacking(` hits in primitives). Flag as a wiki discrepancy.

### Damage
`TrDamaged` (AllAbilities.h:1417) → `WEventDamage` (WEvent.h:50). The 4th ctor arg selects damage class: **0 = any, 1 = combat, 2 = noncombat**. Each class has three player-scope variants:

| Keyword | Class / scope | Fires when… |
|---|---|---|
| `@damaged($tc) [from($tc)]` | any, static | any damage dealt to a matching card/player (line 1762) |
| `@damageof($tc)` | any, controller | the card's **current controller** is damaged (1748) |
| `@damagefoeof($tc)` | any, opponent | the card's **current opponent** is damaged (1755) |
| `@combatdamaged` / `@combatdamageof` / `@combatdamagefoeof` | combat (1727–1745) | combat damage only |
| `@noncombatdamaged` / `@noncombatdamageof` / `@noncombatdamagefoeof` | noncombat (1706–1724) | non-combat damage only |

`from(...)` restricts the damage source. `sourcenottap`, `turnlimited`, `once` apply. **Example:** `@damaged(this):` — `_macros.txt:92` (`_ENRAGE_`); `@damaged(*|opponentBattlefield) from(other *[red]|*):...` — `planeswalkers.txt:1652`; `@combatdamaged(player) from(this):counter(1/1.$c)...` — `_macros.txt:204` (`_RENOWN_`).

### Life gain / loss
`TrLifeGained` (AllAbilities.h:1482) → `WEventLife` (WEvent.h:88). 6th ctor arg: **0 = life gained, 1 = life lost**. Three scopes each; all accept `from(...)` and an `except(...)` loop-guard TC (lines 1772, 1805).

| Gain (0) | Loss (1) | Scope |
|---|---|---|
| `@lifed` | `@lifeloss` | static / any player |
| `@lifeof` | `@lifelostof` | controller |
| `@lifefoeof` | `@lifelostfoeof` | opponent |

### Card draw
`TrcardDrawn` (AllAbilities.h:836) → `WEventcardDraw` (WEvent.h:262).
- `@drawn($tc)` — **static** binding: `drawn(controller)` = owner forever, `drawn(opponent)` = opponent forever, `drawn(player)` = any (line 1577).
- `@drawof($tc)` / `@drawfoeof($tc)` — **dynamic** binding that re-resolves the current controller/opponent even after control changes (lines 1569, 1573).

**Examples:** `@drawn(targetedplayer):may draw:1 controller` — `mtg.txt:89455`; `@drawof(player) restriction{...}:...` — `planeswalkers.txt:1538`, `_macros.txt:95` (`_SECOND_DRAW_`).

### Counters
| Syntax | Class → WEvent | Mode |
|---|---|---|
| `@counteradded(<counter>|any) [from($tc)] [except($tc)] [duplicate(all)]` | `TrCounter` (1615) → `WEventCounters` (58) | mode 1 = added (line 1916) |
| `@counterremoved(<counter>|any)` | `TrCounter` | mode 0 = removed (1933) |
| `@countermod(<counter>|any)` | `TrCounter` | mode 2 = added-or-removed (1950) |
| `@totalcounteradded(<counter>) [plus(N)] [duplicate(all)] [half(all)] [nocost]` | `TrTotalCounter` (1665) → `WEventTotalCounters` (72) | mode 1, counts N counters in one event (1848) |
| `@totalcounterremoved(...)` | `TrTotalCounter` | mode 0 (1882) |

`(any)` matches any counter kind (skips `parseCounter`). `except(...)` is a loop-guard added to break proliferate/counter feedback loops — note it tests the counter's **source** (the card that placed it), NOT the card that received it; the receiving card is what `from(...)` describes. **Example:** `@counteradded(0/0.1.ExchangeEffect) from(this):...` — `planeswalkers.txt:183`.

`bycontroller` (W54, `MTGAbility.cpp` totalcounteradded/removed + counteradded/removed blocks; `TrTotalCounter::byController` / `TrCounter::byController`, AllAbilities.h) restricts the trigger to counters placed by a spell/ability **this card's controller controls** — i.e. Oracle "Whenever **you** put one or more … counters on …" (Hapatra, Vizier of Poisons; All Will Be One; Generous Patron; Stocking the Pantry). Without it the trigger fires on ANY player's counter placement. An engine path that raises the event with no causing card (`e->source == NULL`: evolve, wither, persist…) is unattributable and does not fire. Cards worded passively ("whenever one or more counters **are put** on a creature you control" — Shalai and Hallar, Enduring Scalelord, Animation Module, Cloaked Cadet, Moss-Pit Skeleton) must NOT carry it.

### Targeting
`@targeted($tc) [from($tc)] [turnlimited]` → `TrTargeted` (AllAbilities.h:1578) → `WEventTarget` (WEvent.h:134). Fires when a matching card becomes the target of a spell/ability; `from(...)` restricts what did the targeting (line 1842). **Examples:** `@targeted(this) from(*[instant;sorcery;aura]|myCastingzone):` — `_macros.txt:43` (`_HEROIC_`); `@targeted(this) from(...) turnlimited:` — `_macros.txt:46` (`_VALIANT_`).

### "Killed this turn" / vampire
`@vampired($tc) [from($tc)]` → `TrVampired` (AllAbilities.h:1536) → `WEventVampire` (WEvent.h:125). Fires when a card that was damaged by the source this turn is put into a graveyard (line 1835). **Example:** `@vampired(creature) from(this):...` — `mtg.txt:30`, `:8001`.

### Zone-leaving / keyword-action event triggers
All bind `parseSimpleTC` + `WEventCardUpdate` subclasses; all accept `once`/`turnlimited`:

| Trigger | Class → WEvent | Fires when a matching card… |
|---|---|---|
| `@sacrificed($tc)` | `TrCardSacrificed` (1293) → `WEventCardSacrifice` (212) | is sacrificed |
| `@exploited($tc)` | `TrCardExploited` (1335) → `WEventCardExploited` (219) | is exploited |
| `@discarded($tc)` | `TrCardDiscarded` (1377) → `WEventCardDiscard` (226) | is discarded |
| `@cycled($tc)` | `TrCardDiscarded(...,true)` → `WEventCardCycle` (232) | is cycled (discard variant, cycled flag) |
| `@tokencreated($tc)` | `TrTokenCreated` (1264) → `WEventTokenCreated` (525) | a matching token is created |
| `@mutated($tc)` | `TrCardMutated` (871) → `WEventCardMutated` (519) | mutates |
| `@boasted($tc)` | `TrCardBoasted` (958) → `WEventCardBoasted` (442) | performs Boast |
| `@defeated($tc)` | `TrCardDefeated` (986) → `WEventCardDefeated` (448) | (battle) is defeated |
| `@surveiled($tc)` | `TrCardSurveiled` (1014) → `WEventCardSurveiled` (454) | surveils |
| `@foretold($tc)` | `TrCardForetold` (1042) → `WEventCardForetold` (460) | is foretold |
| `@trained($tc)` | `TrCardTrained` (1070) → `WEventCardTrained` (466) | trains |
| `@scryed($tc)` | `TrCardScryed` (1098) → `WEventCardScryed` (472) | scrys |
| `@ninjutsued($tc)` | `TrCardNinja` (1128) → `WEventCardNinja` (478) | uses Ninjutsu |
| `@explored($tc)` | `TrCardExplored` (899) → `WEventCardExplored` (484) | explores |
| `@bearerchosen($tc)` | `TrCardBearerChosen(...,false)` (927) → `WEventCardBearerChosen` (490) | a Ring-bearer is chosen |
| `@bearernewchosen($tc)` | `TrCardBearerChosen(...,true)` | a **different** Ring-bearer is chosen |
| `@dungeoncompleted($tc) [total(N)] [from(controller|opponent)]` | `TrCardDungeonCompleted` (1156) → `WEventCardDungeonCompleted` (497) | completes a dungeon (optional total count / player filter) |
| `@dierolled($tc) [result(N|max)] [from(...)]` | `TrCardRolledDie` (1190) → `WEventCardRollDie` (505) | rolls a die (optional result match; `max` = highest face) |
| `@coinflipped($tc) [result(head|tail(s)|won|lost)] [from(...)]` | `TrCardFlippedCoin` (1226) → `WEventCardFlipCoin` (512) | flips a coin (optional result match) |

### Player-state event triggers (`…of` / `…foeof` scoping)
Each has a controller (`of`) and an opponent (`foeof`) variant. `WEventplayer*` types in WEvent.h:374–434.

| Trigger pair | Class → WEvent | Fires when… |
|---|---|---|
| `@poisonedof` / `@poisonedfoeof` `[plus(N)|duplicate(all)|half(all)]` | `TrplayerPoisoned` (554) → `WEventplayerPoisoned` (374) | player gets poison counters |
| `@energizedof` / `@energizedfoeof` `[plus/duplicate/half]` | `TrplayerEnergized` (599) → `WEventplayerEnergized` (383) | player gets energy |
| `@experiencedof` / `@experiencedfoeof` `[plus/duplicate/half]` | `TrplayerExperienced` (644) → `WEventplayerExperienced` (392) | player gets experience |
| `@proliferateof` / `@proliferatefoeof` `[except($tc)]` | `TrplayerProliferated` (747) → `WEventplayerProliferated` (417) | player proliferates (`except` breaks loops, e.g. Tekuthal) |
| `@ringtemptedof` / `@ringtemptedfoeof` | `TrplayerTempted` (718) → `WEventplayerTempted` (409) | player is tempted by the Ring |
| `@becomesmonarchof` / `@becomesmonarchfoeof` | `TrplayerMonarch` (689) → `WEventplayerMonarch` (401) | player becomes the monarch |
| `@takeninitiativeof` / `@takeninitiativefoeof` | `TrplayerInitiative` (778) → `WEventplayerInitiative` (426) | player takes the initiative |
| `@shuffledof` / `@shuffledfoeof` | `TrplayerShuffled` (807) → `WEventplayerShuffled` (434) | player shuffles their library |

`plus(1..5)`, `duplicate(all)`, `half(all)` scale how many times the effect fires relative to the counters/amount involved (parsed inline, e.g. lines 1391–1404). **Example:** `@becomesmonarchof(player):...` — `borderline.txt:24585`; `@becomesmonarchfoeof(player):...` — `borderline.txt:38454`.

---

## 3. Trigger restrictions & conditions

### Inline restriction flags (parsed at top of `parseTrigger`, lines 1247–1255)
Appended anywhere in the trigger text (space-separated). **Not every trigger honors every flag** — they're wired per-class as needed.

| Flag in script | Parser var | Effect |
|---|---|---|
| `once` | `once` | Trigger fires **once in its lifetime**, then deactivates (`Trigger::triggerOnEvent`, MTGAbility.cpp:8029). |
| `sourcenottapped` (a.k.a. `sourcenottap`) | `sourceUntapped` | Fires only while the source is **untapped** (checked `.find("sourcenottap")`). |
| `sourcetap` | `sourceTap` | Fires only while the source is **tapped**. (Phase triggers only.) |
| `turnlimited` | `limitOnceATurn` | Fires **at most once per turn**. |
| `suspended` | `isSuspended` | Suspend-context flag (movedto only). |
| `opponentpoisoned` | `opponentPoisoned` | Combat/damage triggers fire only if opponent is poisoned. |
| `foelost(N)` | `lifelost` + `lifeamount=N` | Phase trigger fires only if opponent lost ≥ N life this turn. |
| `dontremove` | `neverRemove` | `movedto` trigger sets `forcedAlive=1; forceDestroy=-1` (never garbage-collected). |
| `except($tc)` | — | Loop-guard exclusion on counter / life / proliferate triggers. Tests the event's **source** card, not the affected one. |
| `bycontroller` | `byController` | Counter triggers only: the counter must have been placed by a source **this card's controller controls** (Oracle "whenever **you** put … counters"). |

**Note on precedence:** wiki (`CardCode.md:705`) documents the flag as `sourcenottap`; the parser matches the substring `"sourcenottap"`, so both `sourcenottap` and the real-card spelling `sourcenottapped` work (the latter is what primitives use — `mtg.txt:11810`).

### `restriction{...}` — cast-restriction conditions
Attached after `parseTrigger` returns (`MTGAbility.cpp:2542–2553`). The text inside is stored as `trigger->castRestriction` and evaluated by `parseCastRestrictions` **at fire time**, so the trigger only actually fires when the condition passes (`Trigger::triggerOnEvent` line 8032; `TriggerAtPhase::trigger` line 8310).

- `restriction{ ... }` — single-brace form.
- `restriction{{ ... }}` — double-brace form (parsed with `{{`/`}}` delimiters, line 2548).

### `~<op>~` comparison syntax
Comparisons live **inside** `restriction{...}` (and other condition contexts), written as `<lhs>~<op>~<value>`. Operators seen in real cards: `~morethan~`, `~lessthan~`, `~equalto~`. The LHS is a dynamic query such as `compare(<variable>)` or `type(<targetspec>)`.

**Real examples:**
- `@drawof(player) restriction{compare(pdrewcount)~equalto~2}:` — `_macros.txt:95` (`_SECOND_DRAW_`, fires on the player's 2nd draw of the turn)
- `@drawof(player) restriction{type(*|mylibrary)~equalto~0}:wingame` — `planeswalkers.txt:1538` (win when you'd draw from an empty library)
- `if compare(oppotgt)~morethan~0 then ... transforms((,newability[@drawn(targetedplayer):...]))` — `borderline.txt:24328`

There is no numeric `limit:N` restriction on triggers themselves — per-turn limiting is `turnlimited` (boolean, once/turn). `limit:N` / `limit` as an integer cap belongs to **activated** abilities (`ActivatedAbility::limitPerTurn`), not to `parseTrigger`.

---

## 4. Complete keyword index (every substring `parseTrigger` matches)

Restriction flags: `once` · `sourcenottap[ped]` · `sourcetap` · `turnlimited` · `suspended` · `opponentpoisoned` · `foelost(N)` · `dontremove` · `except(...)` · `plus(1..5)` · `duplicate(all)` · `half(all)` · `nocost`.

Trigger keywords, in parser dispatch order (file:line in MTGAbility.cpp):
`movedto(` 1258 · `untapped` 1317 · `tapped` 1321 · `tappedformana` 1325 · `producedmana` 1329 · `transformed` 1333 · `facedup` 1337 · `phasedin` 1341 · `exerted` 1345 · `combat(` 1350 · `poisonedof`/`poisonedfoeof` 1387/1409 · `energizedof`/`energizedfoeof` 1431/1453 · `experiencedof`/`experiencedfoeof` 1475/1497 · `proliferateof`/`proliferatefoeof` 1519/1528 · `ringtemptedof`/`ringtemptedfoeof` 1537/1541 · `becomesmonarchof`/`becomesmonarchfoeof` 1545/1549 · `takeninitiativeof`/`takeninitiativefoeof` 1553/1557 · `shuffledof`/`shuffledfoeof` 1561/1565 · `drawof`/`drawfoeof` 1569/1573 · `drawn` 1577 · `mutated` 1581 · `boasted` 1585 · `defeated` 1589 · `surveiled` 1593 · `foretold` 1597 · `trained` 1601 · `scryed` 1605 · `ninjutsued` 1609 · `explored` 1613 · `bearerchosen` 1617 · `bearernewchosen` 1621 · `dungeoncompleted` 1625 · `dierolled` 1642 · `coinflipped` 1663 · `tokencreated` 1686 · `sacrificed` 1690 · `exploited` 1694 · `discarded` 1698 · `cycled` 1702 · `noncombatdamageof`/`noncombatdamagefoeof`/`noncombatdamaged` 1706/1713/1720 · `combatdamageof`/`combatdamagefoeof`/`combatdamaged` 1727/1734/1741 · `damageof`/`damagefoeof`/`damaged` 1748/1755/1762 · `lifeof`/`lifefoeof`/`lifed` 1769/1780/1791 · `lifelostof`/`lifelostfoeof`/`lifeloss` 1802/1813/1824 · `vampired` 1835 · `targeted` 1842 · `totalcounteradded(`/`totalcounterremoved(` 1848/1882 · `counteradded(`/`counterremoved(`/`countermod(` 1916/1933/1950 · `next <phase>` 1975 · `each <phase>` 1989 · `rebounded` 2003.

---

## 5. Wiki discrepancies (`docs/wiki/CardCode.md` vs parser)

1. **Combat traits presented as standalone triggers.** Wiki lists `@attacking`, `@blocking`, `@blocked`, `@notblocked`, `@attackedalone` as top-level keywords (`CardCode.md:599–616`, plus dead anchor links). The parser has **no such branches** — all five are traits inside `@combat(<trait>,...)` with a mandatory `source(...)` (MTGAbility.cpp:1350–1384). Zero `@attacking(` / `@blocked(` usages exist in primitives; every real card uses `@combat(...)`. **Correct the wiki to document `@combat(...)`.**

2. **Phase name `endofturn` vs `end`.** The wiki phase list (`CardCode.md:648`) writes `endofturn`, but the engine's canonical token is **`end`** (`MTGDefinitions.cpp:335`). It happens to still resolve (substring `end` is found inside `endofturn`), but every real card uses `end` (e.g. `@each my upkeep`, `@next end` at `planeswalkers.txt:217`). The wiki phase list is otherwise accurate (`beginofturn … beforenextturn`).

3. **Missing `targetedplayer` scope.** Wiki documents `my`/`opponent`/`targetcontroller` for phase triggers (`CardCode.md:655`) but omits the fourth scope **`targetedplayer`** (`who=-3`, MTGAbility.cpp:1971; used e.g. `@drawn(targetedplayer)` at `mtg.txt:89455`).

4. **Large undocumented trigger set.** The wiki's "auto triggers" list (`CardCode.md:596–642`) is a small subset. Absent from the wiki but present in the parser: all player-state triggers (`poisonedof`, `energizedof`, `experiencedof`, `proliferateof`, `ringtemptedof`, `becomesmonarchof`, `takeninitiativeof`, `shuffledof`), keyword-action triggers (`surveiled`, `foretold`, `trained`, `scryed`, `ninjutsued`, `explored`, `boasted`, `defeated`, `mutated`, `bearerchosen`/`bearernewchosen`, `dungeoncompleted`, `dierolled`, `coinflipped`, `tokencreated`, `exploited`, `cycled`), the counter family (`counteradded`/`counterremoved`/`countermod`/`totalcounter*`), plus `transformed`, `facedup`, `phasedin`, `exerted`, `producedmana`, `tappedformana`, and `@rebounded`. Treat the wiki trigger list as **incomplete, not authoritative.**

5. **No `@dies` / `@cast` / `@etb` / `@becomes` keywords.** These common MTG concepts are **not** parser keywords. "Dies" = `@movedto(...|graveyard) from(...battlefield)`; "when cast" = `@movedto(...|stack)`; ETB = `@movedto(...|battlefield)`. `@becomes…` in primitives is only ever `@becomesmonarch{of,foeof}` — there is no generic `@becomes`. The wiki does not claim these exist, but AI/codegen consumers frequently assume them; state the substitution explicitly.

6. **`turnlimited` semantics.** Wiki says "fire once per turn" (`CardCode.md:708`) — accurate; it maps to `limitOnceATurn`, distinct from lifetime-`once`. There is no `limit:N` integer trigger cap (that is an activated-ability field).

---

# Effect Keywords — Part 1 (`AbilityFactory::parseMagicLine`, lines ~2046–4061)

**Scope note.** This range of `parseMagicLine` (projects/mtg/src/MTGAbility.cpp) is the **early half** of the effect dispatcher: engine-rule registrations, the structural/wrapper keywords (targeting, cost, conditionals, duration, lord/foreach), and a first block of leaf effects (tokens, zone-movers, fizzle, copy, blink, etc.). The **classic point effects the task named — `draw:`, `life:`, `damage:`, `prevent:`, `counter(`, `becomes(`, `transforms((` (the main handler), `setpower`/`settoughness`, `destroy`, `exile`, `tap`/`untap`, `regenerate` — all live PAST line 4050** (verified: `destroy` @4222, `damage:` @4419, `prevent:` @4667, `life:` @4744, `draw:` @4786, `counter(` @5076, `becomes(` @5207, `transforms((` main @5323, `regenerate` @5842, `tap`/`untap` @5880–5898). Those belong to the other agent's range (4050–end). Do not expect them below.

**How duration/shape is set.** For almost every leaf effect in this range the parser sets `a->oneShot = 1` *explicitly at the call site* — that is the ground truth for shape, independent of the class. The wrapper keywords (`ueot`, `activate`, `emblem`, lord family, `this` family) instead control continuity structurally. I note both.

---

## A. Engine-rule registrations (NOT card effects) — lines 2065–2266

These match a bare token anywhere in the line, register a **single global rule observer**, and `return NULL` (no ability attached to the card). They appear in `_rules` primitives, not on normal cards. One-shot/continuous is N/A — they are permanent engine singletons. Complete list in range, each `NEW`-ing the named class via `observer->addObserver(...)`:

`bonusrule`→MTGEventBonus · `putinplayrule`→MTGPutInPlayRule · `kickerrule`→MTGKickerRule · `alternativecostrule`→MTGAlternativeCostRule · `buybackrule`→MTGBuyBackRule · `flashbackrule`→MTGFlashBackRule (+MTGTempFlashBackRule) · `bestowrule`→MTGBestowRule · `retracerule`→MTGRetraceRule · `suspendrule`→MTGSuspendRule · `morphrule`→MTGMorphCostRule · `payzerorule`→MTGPayZeroRule · `overloadrule`→MTGOverloadRule · `attackrule`→MTGAttackRule · `attackcostrule`→MTGAttackCostRule · `blockrule`→MTGBlockRule · `blockcostrule`→MTGBlockCostRule · `soulbondrule`→MTGSoulbondRule · `dredgerule`→MTGDredgeRule (added to `replacementEffects`) · `combattriggerrule`→MTGCombatTriggersRule · `legendrule`→MTGNewLegend · `planeswalkerrule`→MTGNewPlaneswalker · `planeswalkerdamage`→MTGPlaneswalkerDamage · `planeswalkerattack`→MTGPlaneswalkerAttackRule · `tokencleanuprule`→MTGTokensCleanup · `persistrule`→MTGPersistRule · `vampirerule`→MTGVampireRule · `unearthrule`→MTGUnearthRule · `lifelinkrule`→MTGLifelinkRule · `deathtouchrule`→MTGDeathtouchRule.

Gotcha: these are substring `find`, matched before any effect parsing. Never use these bare tokens as card text.

---

## B. Structural / wrapper keywords (they parse a *nested* effect and modify its shape)

### `@TRIGGER:EFFECT` — triggered ability
- **Syntax:** `@<trigger>:<effect line>` (e.g. `@movedto(...)`, `@combat(attacking)`, `@each my upkeep`). Optional `restriction{...}`/`restriction{{...}}` sets `castRestriction`.
- **Builds:** `GenericTriggeredAbility` (MTGAbility.cpp:2563), wrapping the sub-parse of the effect. Handled at ~2538.
- **Shape:** triggered. The wrapped effect keeps its own shape.
- **Example:** borderline.txt:460 `@discarded(land|opponentHand):create(bat:...)`.

### `tutorial(MSG)` / `message(MSG)`
- **Builds:** `ATutorialMessage` (:2574 / :2581). Not a game effect — displays on-screen text (tutorials). `message(` variant passes flag 0.

### `if COND then EFFECT [else EFFECT2]` / `ifnot COND then ...`
- **Syntax:** `if <cond> then <effect> [ else <effect2> ]` — must appear at position 0 of the (post-target) string. `ifnot` inverts (checkIf 1 vs 2).
- **Builds:** `IfThenAbility` (AllAbilities.h:1879, `: public InstantAbility`), MTGAbility.cpp:2774.
- **Shape:** **one-shot** (`a->oneShot = true`, `canBeInterrupted=false`). Evaluates `cond` at resolution and runs `a1` or `a2` once.
- **Example:** borderline.txt:704 `rolld6 6 winability ... loseability if compare(lastrollresult)~lessthan~10 then may ... else moveto(hand) ...`.

### `pay(COST)` / `pay[[COST]]` — "you may pay"
- **Builds:** `GenericPaidAbility` (MTGAbility.cpp:2786 for `pay(`, :3487 for `pay[[`). `oneShot=1`, `canBeInterrupted=false`. The cost text is pre-stripped earlier (2313/2468) into `storedPayString`; `asAlternate` set when combined with `castcard(restricted`.
- **Shape:** one-shot optional payment gate.

### `may EFFECT` / `choice EFFECT`
- **Syntax:** keyword must be at position 0. `may ` = optional (mayMust=false), `choice ` = mandatory choose (mayMust=true).
- **Builds:** wraps the sub-effect in `GenericTargetAbility` or `GenericActivatedAbility`, then `MayAbility` (AllAbilities.h:1894), MTGAbility.cpp:2808.
- **Shape:** inherits nested effect's shape; adds a yes/no (or forced-choice) prompt.
- **Example:** borderline.txt:91 `may name(Exile nonland permanents) target(...) transforms((...))`.

### Plain leaf effect carrying its own `target(...)` inside a resolving spell — auto-`may` wrap (2820–2871)
- Not a keyword: a safety net. When `tc && spell` and the line is a *bare leaf* (`damage:`, a `±P/±T`, `moveto(`, or `counter(` at position 0, with no `and!(`/`transforms(`/`newability`/`teach(`/`&&`), it is wrapped `GenericTargetAbility`→`MayAbility(...,true)` so the target is prompted at resolution. Fixes silently-fizzling secondary spell legs.

### `ueot EFFECT` — until end of turn
- **Syntax:** `ueot <effect>` at position 0 (strips 5 chars).
- **Builds:** `GenericInstantAbility` (AllAbilities.h:3065, `: public InstantAbility, NestedAbility`), MTGAbility.cpp:2881.
- **Shape:** **until-end-of-turn continuous.** `addToGame()` calls the nested ability's `addToGame()` (activating it continuously, `forceDestroy=-1`); the wrapper is an InstantAbility removed at cleanup, tearing down the nested effect at EOT.
- **Gotcha:** this is the canonical way to make any static/continuous effect last only the turn. Wiki §ueot (CardCode.md:1090–1104) documents it and warns the engine's auto-duration guess is sometimes wrong — force with `ueot`.
- **Example:** borderline.txt:2958 `{2}{W}{T}:(blink)ueot target(other creature|battlefield)`; borderline.txt:229 `... moveto(exile) and!( transforms((...)) ueot )!`.

### `activate EFFECT`
- **Syntax:** `activate ` or ` activate ` (strips 9 chars).
- **Builds:** `GenericAddToGame` (AllAbilities.h:3247), MTGAbility.cpp:2892.
- **Shape:** one-shot "addToGame" — at resolution clones the nested ability and adds it to the game (turning a would-be static effect into an added instance). Used to inject an ability into play once.

### `emblem EFFECT`
- **Syntax:** `emblem ` at position 0 (strips 7).
- **Builds:** `GenericAbilityMod` (AllAbilities.h:3200), MTGAbility.cpp:2903, attached to `ExtraRules` (the game-wide emblem holder).
- **Shape:** **permanent/neverending** — an emblem-style effect that lives on the ExtraRules object, not on a removable permanent. Typically paired with `forever dontremove`.
- **Example:** borderline.txt:201 `emblem transforms((,newability[lord(...) changecost(colorless:-1) forcedalive])) forever dontremove`.

### Lord family: `lord(TC) EFFECT`, `foreach(TC) EFFECT`, `aslongas(TC) EFFECT`, `teach(TC) EFFECT`, `all(TC) EFFECT`
- **Keyword table** (MTGAbility.cpp:19): `{ "lord(", "foreach(", "aslongas(", "teach(", "all(" }`. Dispatch at 3093–3226.
- **Builds / shape:**
  - `lord(` → **ALord** (AllAbilities.h:3607, ListMaintainerAbility) — **continuous static** buff/grant to every card matching TC while source is in play.
  - `foreach(` → **AForeach** (:4450) — continuous, scales an effect per matching card (supports ` >N`/` <N` mini/maxi bounds parsed at 3165–3177).
  - `aslongas(` → **AAsLongAs** (:3438) — continuous conditional (active while the count of TC meets the `>`/`<`/`compare` bound).
  - `teach(` → **ATeach** (:3749) — grants an ability to matching cards (each learns the nested effect).
  - `all(` → **ALord** again (case 4) but `oneShot=true` forced — apply nested effect once to every current match (mass one-shot, e.g. "each X does Y now").
- **Modifiers:** ` other` excludes self (`lordIncludeSelf=0`). `oneshot` forces one-shot; `while ` forces continuous; sorcery/instant/activated context also forces one-shot. `forcedalive`/`dontremove` (parsed 2933–2942) pin the effect against removal.
- **Wiki:** CardCode.md:1022 documents `lord($targets) [$effect]`. Note the parenthesized TC is the *group*, not a chosen target.
- **Example:** borderline.txt:210 `all(*[shapeshifter]|mybattlefield) transforms((...))`; borderline.txt:40531 `all(this) flipacoin ...`.

### This family: `this(DESC) EFFECT`, `thisforeach(DESC) EFFECT`, `while(DESC) EFFECT`
- **Keyword table** (:22): `{ "this(", "thisforeach(", "while(" }`. Dispatch at 2957–3067.
- **Builds / shape:**
  - `this(` → **AThis** (AllAbilities.h:4543) with a ThisDescriptor — conditional on a property of the *source card itself*.
  - `thisforeach(` → **AThisForEach** (:4641) — scales the nested effect by a count derived from the source (e.g. Yannik-style power buckets).
  - `while(` → **AThis** (case 2, descriptor kept as string).
- **Shape:** continuous by default; `oneShot` forced true when the line has ` oneshot`, is on an instant/sorcery, `activated`, or the nested ability is itself one-shot; ` while ` forces continuous.
- **Gotcha (in-code):** a `&&` chain must split before this block — the parser deliberately skips `thisforeach(` when `&&` is present (comment at 2966) so a leading leg isn't discarded.
- **Wiki:** CardCode.md:428 shows `this(gear !=0)` and `thisforeach(equip)`.

### `EFFECT_A && EFFECT_B [&& ...]` — multi-ability under one cost
- **Builds:** `MultiAbility` (AllAbilities.h:1989), MTGAbility.cpp:3075. Splits on `&`, parses each leg, adds all.
- **Shape:** `oneShot = 1`. Each leg keeps its own effect semantics.
- **Wiki:** CardCode.md:1000. **Example:** borderline.txt:12555 `all(this) removefromcombat && untap`.

### `soulbond EFFECT`
- **Builds:** `APaired` (AllAbilities.h:4355), MTGAbility.cpp:3236 — grants the nested effect to this card's `myPair`.
- **Shape:** continuous while paired. **Example:** borderline.txt:13893.

### `poolsave(COLOR)` / `mypoolsave` / `opponentpoolsave`
- **Syntax:** `poolsave(<color>)`; also detects `opponentpool` and `terminate` flags.
- **Builds:** `AManaPoolSaver` (AllAbilities.h:6082), MTGAbility.cpp:3244.
- **Shape:** **static** while in play — the listed mana color is not emptied from the pool at phase change. `terminate` ends the effect when the source leaves. Wiki: CardCode.md:905.

### `replacedraw ABILITY` / `opponentreplacedraw ABILITY`
- **Builds:** `ADrawReplacer` (AllAbilities.h:6095), MTGAbility.cpp:3255/3267 (`opponent...` passes `otherPlayer=true`).
- **Shape:** **replacement effect** (installs `REDrawReplacement`) — active while in play; replaces the draw with the nested ability. Wiki: CardCode.md:747/751.

---

## C. Leaf effects in this range (all one-shot unless noted)

### `dynamicability<! ... !>`
- **Syntax:** `dynamicability<!<source/type/effect/who keywords> <stored ability>!>`. Keyword tables `kDynamicSource/Type/Effect/Who...` resolve 4 integer selectors.
- **Builds:** `AADynamic` (AllAbilities.h:5222), MTGAbility.cpp:3344. `oneShot=1`.
- **Shape:** one-shot; computes an amount dynamically (from a game quantity) and applies the stored effect.

### `flipacoin ... [winability ... winabilityend] [loseability ... loseabilityend] flipend`
- **Builds:** `GenericFlipACoin` (AllAbilities.h:7527), MTGAbility.cpp:3359. `oneShot=1`, `canBeInterrupted=false`. Optional leading digit = user's called side.
- **Shape:** one-shot; runs win/lose sub-ability by coin result.
- **Example:** borderline.txt:46830 `flipacoin loseability sacrifice notaTarget(creature|mybattlefield) loseabilityend flipend`.

### `rolld4 / rolld6 / rolld8 / rolld10 / rolld12 / rolld20 ... rolld<N>end`
- **Syntax:** `rolld<N> <payload> ... rolld<N>end`; d10/d12/d20 parse a 2-digit leading user-choice.
- **Builds:** `GenericRollDie` (AllAbilities.h:7557) with die size N (MTGAbility.cpp:3375–3474). `oneShot=1`, `canBeInterrupted=false`.
- **Shape:** one-shot; result available via `lastrollresult`. Payload uses `winability`/`loseability`/`compare(lastrollresult)`.
- **Example:** borderline.txt:102 (d20), :704 (d6).

### `ninjutsu` / `readytofight`
- **Builds:** `ANinja` (AllAbilities.h:2266), MTGAbility.cpp:3504 (`ninjutsu`, bool `true`) and :3513 (`readytofight`, bool `false`). `oneShot=1`.
- **Shape:** one-shot. `ninjutsu` = return unblocked attacker to hand (as cost) and put this in tapped & attacking; `readytofight` = the false-variant (return-a-creature-to-battlefield form). Wiki: CardCode.md:889.
- **Example:** ninjutsu borderline.txt:5316; readytofight borderline.txt:25326.

### `removefromcombat`
- **Builds:** `ACombatRemoval` (AllAbilities.h:2315), MTGAbility.cpp:3522. `oneShot=1`. Removes target from combat. **Example:** borderline.txt:12555.

### `shackle`
- **Builds:** `AShackleWrapper` (AllAbilities.h:5999, `: public InstantAbility`), MTGAbility.cpp:3531. `oneShot=1`; at resolve installs an inner `AShackle`.
- **Shape:** one-shot that establishes a **conditional-continuous** control effect: gain control of target **until source is untapped or leaves battlefield** (comment at 3527).

### `grant EFFECT ... grantend`
- **Syntax:** the granted ability is pre-extracted (2364) into `storedAbilityString`; the `grant ` at :3537 consumes it.
- **Builds:** `AGrantWrapper` (AllAbilities.h:6031)→inner `AGrant` (:6012), MTGAbility.cpp:3541. `oneShot=1`.
- **Shape:** one-shot that grants an ability **while the source is untapped / still valid** (comment at 3536). Distinct from `teach(` (lord-style) — `grant`/`AGrant` tracks a single blessed target.

### `(blink)` [`(blink)ueot`] [`(blink)forsrc`] [`hand(blink)`] `return(EFFECT)`
- **Builds:** `ABlinkGeneric` (AllAbilities.h:6066)→inner `ABlink` (:6045), MTGAbility.cpp:3563. `oneShot=1`.
- **Shape:** one-shot momentary exile-and-return (flicker). Flags: `ueot` = return at end of turn (else immediately); `forsrc` = track the source card; `hand(blink)` = return to hand path. `return(...)` supplies an ability to run on the returned card. Blink source falls back to `storedSourceCard` for `ability$!!$` grants (Parallax Nexus fix).
- **Example:** borderline.txt:1802 `target(creature|opponentbattlefield) (blink)forsrc`; :2958 `(blink)ueot`.

### `spellmover(ZONE)` / `fizzleto(ZONE)`
- **Builds:** `AAFizzler` (AllAbilities.h:1852), MTGAbility.cpp:3600, with `fizzleMode` set from ZONE. `oneShot=1`. `spellMover` flag set when `spellmover(`.
- **Shape:** one-shot. Counters a spell on the stack AND puts it into a chosen zone. Supported zones: `hand`, `exile`, `exileimp` (imprint-exile), `librarytop`, `librarysecond`, `librarybottom` (default = graveyard).

### `fizzle` — counterspell
- **Builds:** `AAFizzler` (AllAbilities.h:1852), MTGAbility.cpp:3614. `oneShot=1`.
- **Shape:** one-shot; counters the next spell target. Guarded so `nofizzle` (a static "can't be countered" grant) doesn't accidentally trigger it (3609).
- **Wiki:** CardCode.md:811 (target defaults to `stack`). **Example:** borderline.txt:192.

### `ability$ ... ` (targeted-ability creator; payload pre-stashed in `ability$! ... !$`)
- **Syntax:** payload extracted (2448) into `storedAbilityString` between `ability$!` and `!$`; the trailing `ability$` at :3633 triggers construction. A player selector (`controller`/`opponent`/`owner`/`targetcontroller`/`targetedplayer`) sets `who`.
- **Builds:** `ATargetedAbilityCreator` (AllAbilities.h:4264), MTGAbility.cpp:3646. `oneShot=1`.
- **Shape:** one-shot — makes the chosen player perform the stashed ability (e.g. forced sacrifice on each player).
- **Example:** borderline.txt:257 `ability$!sacrifice notaTarget(<2>creature|mybattlefield)!$ controller && ... opponent`.

### `token(NAME,TYPES,P/T[,ABILITIES][,COLORS])[ * N]` — comma-delimited token
- **Builds:** `ATokenCreator` (AllAbilities.h:3933), MTGAbility.cpp:3657–3770. `oneShot=1`.
- **Forms:** `token(<id>)` (numeric — looks up a T-rarity card by id; negative id convention); `token(<cardname>)` (by name, 1–2 params, supports `,notrigger`); or full `token(name,types,p/t,abilities...)`. `* N` (or `* <variable>`) multiplier via `WParsedInt`. `^` is accepted in place of `,` (for nesting inside `transforms((`). P/T supports `x/x`,`xx/xx` (scales by `card->X`). `livingweapon` → `forceDestroy=1`. `storedAndAbility` (from `and!(...)`) becomes `andAbility`. `who` selector controls which player gets the token.
- **Shape:** one-shot creation (the token itself persists per game rules).
- **Wiki:** CardCode.md:801–807. **Example:** borderline.txt:291 (Spirit), :804 (Dragon Roost).

### `create(NAME:TYPES:P/T[:ABILITIES][:COLORS])[ * N]` — colon-delimited token (alt syntax)
- **Builds:** same `ATokenCreator` (MTGAbility.cpp:3773–3879). `oneShot=1`. Identical semantics to `token(` but split on `:` instead of `,`. Also supports numeric id and by-name lookup.
- **Example:** borderline.txt:460 `create(bat:creature bat:1/1:black:flying)`; :714 `create(zombie druid:creature zombie druid:2/2:black)`.
- **Discrepancy:** the wiki (CardCode.md) documents only `token(`; **`create(` is undocumented in the wiki** — flag as an omission.

### `equip` / `reconfigure` / `attach`
- **Builds:** `AEquip` (AllAbilities.h:3909, `: public TargetAbility`), MTGAbility.cpp:3887/3894/3904.
- **Shape:** **activated** (persistent activated ability, NOT one-shot — it's an equip action available while in play). `equip`: main-phase-only equip (default TC `creature|mybattlefield`, guarded against matching substrings `equipment`/`equipped`). `reconfigure`: sets `isReconfiguration=true`. `attach`: like equip but any-priority (`NO_RESTRICTION`), marked deprecated in favor of retarget.
- **Wiki:** CardCode.md:819/821.

### `moveto(ZONE)` / `hiddenmoveto(ZONE)`
- **Builds:** `AAMover` (AllAbilities.h:2156), MTGAbility.cpp:3916. `oneShot=true`.
- **Shape:** one-shot zone move of target. `hiddenmoveto` sets `necro=true` (hidden move). Aura self-move hack: for `TYPE_AURA` the target is forced to `card` (issue #120). `storedAndAbility`→`andAbility`.
- **Wiki:** CardCode.md:791. **Example:** borderline.txt:83 `... moveto(myexile)`; :1213 `{B}{B}:moveTo(myBattlefield)`.

### `moverandom(TC) from(ZONE) to(ZONE)`
- **Builds:** `AARandomMover` (AllAbilities.h:2176), MTGAbility.cpp:3936. `oneShot=true`. Requires both `from(` and `to(` or returns NULL. `andAbility` supported.
- **Shape:** one-shot; moves a random matching card between zones (e.g. "seek a land").
- **Example:** borderline.txt:13364 `moverandom(land) from(mylibrary) to(myhand)`.

### `placefromthetop(N)`
- **Builds:** `AALibraryPosition` (AllAbilities.h:2028), MTGAbility.cpp:3957. `oneShot=1`. Position parsed via `WParsedInt`.
- **Shape:** one-shot; puts a card at position N from the top of owner's library.

### `bottomoflibrary`
- **Builds:** `AALibraryBottom` (AllAbilities.h:2041), MTGAbility.cpp:3973. `oneShot=1`. `andAbility` supported.
- **Shape:** one-shot; puts target on the bottom of library. **Example:** borderline.txt:249.

### `copy [options(...)]` [`notatarget(...)`]
- **Builds:** `AACopier` (AllAbilities.h:2053), MTGAbility.cpp:3995. `oneShot=1`, `canBeInterrupted=false`, `isactivated` tracks the `activated` flag. Optional `options(...)` string; `andAbility` supported.
- **Shape:** one-shot; source becomes a copy of target (clone). Often used with `notatarget(` (shroud-bypass, e.g. Clone).
- **Wiki:** CardCode.md:813, and CardCode.md:1024 on `notatarget()`. **Example:** borderline.txt:210.

### `imprint`
- **Builds:** `AAImprint` (AllAbilities.h:2076), MTGAbility.cpp:4015. `oneShot=1`. Guarded against matching `imprintedcard`. `andAbility` supported.
- **Shape:** one-shot; exiles/records the imprinted card on the source.
- **Example:** borderline.txt:15909 `... imprint`; :17444 `imprint target(*|graveyard)`.

### `haunt`
- **Builds:** `AAHaunt` (AllAbilities.h:2087), MTGAbility.cpp:4034. `oneShot=1`. Guarded against `haunted`. `andAbility` supported.
- **Shape:** one-shot; attaches the haunting card to a target creature.
- **Example:** borderline.txt:180 `... target(creature) haunt`.

### `dotrain` (block straddles line 4050 — completed here)
- **Builds:** `AATrain` (AllAbilities.h:2098), MTGAbility.cpp:4051. `oneShot=1`. `andAbility` supported.
- **Shape:** one-shot; "train" a creature (put a +1/+1 counter as part of the Training mechanic).
- **Example:** _macros.txt:19 `#AUTO_DEFINE _TRAINING_ @combat(attacking) source(this) restriction{trainer}:name(Training) dotrain`.

**Boundary marker:** the very next keyword, `conjure` (MTGAbility.cpp:4064, `AAConjure`), begins *after* line 4050 and is the other agent's responsibility.

---

## D. Cost/target/name preamble (context the keywords above rely on)

Before any effect keyword matches, `parseMagicLine` (2585–2733) strips and stores:
- **`name(...)`** → menu/display text (`newName`), erased so it isn't parsed as an effect.
- **`target(...)` / `notatarget(...)`** → builds the `TargetChooser` via `TargetChooserFactory`; `notatarget` bypasses shroud/targeting legality (2615–2646). AI targeting judged by the ability's own effect (`belongsToAbility`), fixing issue #594.
- **`{cost}:`** (mana + extra costs, `}:` delimiter) → wraps the effect as a costed activated ability: `AManaProducer` (mana abilities set `oneShot=0`, i.e. **repeatable**), `AEquip`, `GenericTargetAbility`, or `GenericActivatedAbility` (2648–2731). `limit:`/`limit^...^` side-effect and use-count handling here.
- **`restriction{...}` / `restriction{{...}}`** → `castRestriction`.
- **`forcedalive` / `dontremove`** (2933–2942) → pin abilities against removal (used with lords/emblems).

These are not effects themselves but determine whether an effect keyword becomes a **one-shot spell resolution**, an **activated ability** (repeatable, with a cost), or a **static** effect.

---

## E. Wiki cross-check summary (CardCode.md, docs/wiki)

- **Documented & consistent:** `moveto`/`moveTo`, `token`, `fizzle`/`nofizzle`, `copy`+`notatarget`, `equip`/`attach`, `ninjutsu`, `soulbond`, `poolsave`/`mypoolsave`/`opponentpoolsave`, `replacedraw`/`opponentreplacedraw`, `&&`, `lord(...)`, `ueot`, `this`/`thisforeach`.
- **Undocumented in the wiki (omissions to flag):** `create(` (colon-delimited token — entirely absent; only `token(` is documented), `moverandom(`, `placefromthetop(`, `bottomoflibrary`, `shackle`, `grant`/`grantend`, `(blink)`/`return(`, `imprint`, `haunt`, `dotrain`, `readytofight`, `removefromcombat`, `conjure`, `emblem`, `activate`, `spellmover(`/`fizzleto(`, `dynamicability<!...!>`, `flipacoin`, `rolld4..rolld20`, `ability$!...!$`, `reconfigure`, and the entire block of engine-rule keywords (`putinplayrule`, `attackrule`, `legendrule`, etc.). These are real, parser-recognized keywords with live card usage in `primitives/borderline.txt` but have no wiki entry — the wiki `CardCode.md` is materially incomplete for this range.
- **Case sensitivity note:** the parser uses `s.find(...)` on lowercase tokens; wiki examples show mixed case like `moveTo`/`fizzle $target`. Card files use lowercase in the `auto=` lines (`moveto(`), and `moveTo` in text appears only in prose/older examples — the `find` is a substring match so `moveTo` inside `moveto` would not match; **authoritative form is lowercase** as in primitives.
- **Shape correctness warning (per the task's `prevent:` lesson, applied to this range):** `token(`/`create(`, `moveto(`, `copy`, `imprint`, `haunt`, `fizzle`, `blink`, `dotrain`, dice/coin — all are **one-shot** (`oneShot=1` at the call site). They do NOT persist. Only `ueot` (until-end-of-turn), the lord family / `this` family / `soulbond` / `poolsave` / `emblem` (static/continuous), and `replacedraw`/`shackle`/`grant` (installed replacement/conditional-continuous effects) carry duration beyond resolution. Getting `equip`/`attach`/`reconfigure` right matters too: they are **repeatable activated abilities**, not one-shots.

---

# Wagic Card DSL — Effect Keywords (Part 2) + Ability-Class Catalog

**Ground truth**: `projects/mtg/src/MTGAbility.cpp` `AbilityFactory::parseMagicLine` (lines 4050–5951) and `getManaReduxAbility` (7227–7256); classes from `projects/mtg/include/AllAbilities.h`. Examples cite `projects/mtg/bin/Res/sets/primitives/mtg.txt`.

**⚠ Wiki cross-check (major finding)**: `docs/wiki/CardCode.md` is missing the overwhelming majority of the effect keywords below. Confirmed **absent** from the wiki: `conjure, manifest, exert, provoke, proliferate, propagate, cascade, vanishing, fading, spiritlink, exchangelife, phaseout, steal, shackle, produceextra, producecolor, becomesmonarch, surveil, explores, meld, meldfrom, doubleside, flip, morph, manafaceup, swap, freeze, frozen, retarget, newtarget, rehook, newhook, mutateover/under, castcard, lifeleech, evolve, serumpowder, countershroud, countertrack, duplicatecounters, removesinglecountertype, removeallcounters, wingame, theringtempts, taketheinitiative, reveal, scry, connect, connectrule, block, clone, bushido (only P/T note), phasealter, affinity, bstw/bestow, epic, forcefield, resetdamage, donothing`, and all the `alter*` player-resource effects (`alterpoison, alterenergy, alterexperience, altermutationcounter, altersurvoffset, alterdevoffset, alteryidarocount, completedungeon`). Only `becomes, transforms, protection, deplete, ingest, removemana, frozen` have any wiki mention. Treat the wiki as **radically incomplete** for effects.

---

## PART A — Effect keywords (part 2) + cost/mana effects

### Duration flags (parsed once, apply to several effects below)
Set at MTGAbility.cpp:4326–4344. These modifiers are read from the effect string and change the *shape* of the ability that gets built:
- `ueot` → `forceUEOT` (until end of turn)
- `oneshot` → `oneShot` (resolve once, don't persist)
- `forever` → `forceForever` (permanent)
- `uynt` → `untilYourNextTurn`
- `uent` → `untilYourNextEndTurn`

The general rule for continuous effects (`becomes`, `transforms`, P/T mods, basic-ability grants, protection, cant-be-blocked, cast restrictions): if the source is an **Instant/Sorcery** and no `forever/uent/uynt` is set (or `ueot` is forced), an **Instant/UEOT variant** class is built; otherwise a **continuous** class. This is the key Oracle-faithfulness lever.

### Sacrifice / destroy / removal family (all `oneShot=1`)
| Keyword | Syntax | Class (file:line) | Shape | Notes / example |
|---|---|---|---|---|
| `bury` | `bury [target]` | `AABuryCard` (4211) | one-shot | Destroy, no-regen. Supports `andAbility`. Ex: Abolish-adjacent removal. |
| `destroy` | `destroy [target]` | `AADestroyCard` (4224) | one-shot | Regen-allowed destroy. `andAbility`. Ex: **Abolish** mtg.txt:139 |
| `sacrifice` / `exploits` | `sacrifice [target]` | `AASacrificeCard` (4237) | one-shot | `exploits` sets `isExploited=true` (fires Exploit trigger). `andAbility`. Ex: **Abhorrent Overlord** mtg.txt:118 |
| `reject` | `reject [target]` | `AADiscardCard` (4251) | one-shot | Discard (note: keyword is "reject", not "discard"). `andAbility` |
| `resetdamage` | `resetdamage` | `AAResetDamage` (4383) | one-shot | Clears damage marked on cards |

### Prevent / fog family (**PREVENTION**, not replacement)
- `preventallcombatdamage` / `preventallnoncombatdamage` / `preventalldamage` / `fog` — MTGAbility.cpp:4346–4377. Builds `APreventDamageTypes` (continuous) or `APreventDamageTypesUEOT` / one-shot for `fog` (types: combat=0, all=1, noncombat=2). Optional `to(...)` / `from(...)` target scoping.
- `prevent:N` — MTGAbility.cpp:4667. `AADamagePrevent` (one-shot), prevents next N damage (shield counter-like). This is **prevention**.

### Damage / life / poison / player-resource alterations (all `oneShot=1`, mostly `ActivatedAbilityTP`)
| Keyword | Syntax | Class (file:line) | Notes / example |
|---|---|---|---|
| `damage:N` | `damage:N` | `AADamager` (4419) | Deal N damage to `spell->getNextTarget()`. `andAbility`. Ex: **Acceptable Losses** mtg.txt:567 |
| `life:N` | `life:±N` | `AALifer` (4744, `false`) | Gain/lose life. `andAbility`. Ex: **Absorb** mtg.txt:243 |
| `lifeleech:N` | `lifeleech:N` | `AALifer` (4760, `true`) | Damage-drain: gain life equal to life lost this way |
| `lifeset:N` | `lifeset:N` | `AALifeSet` (4733) | Set life *total* to N (WParsedInt) |
| `wingame` | `wingame` | `AAWinGame` (4776) | Target player wins the game |
| `alterpoison:N` | | `AAAlterPoison` (4435) | Add/remove poison counters |
| `alterenergy:N` | | `AAAlterEnergy` (4451) | Energy counters |
| `alterexperience:N` | | `AAAlterExperience` (4467) | Experience counters |
| `completedungeon:N` | | `AAAlterDungeonCompleted` (4483) | Dungeon-completed count |
| `alteryidarocount:N` | | `AAAlterYidaroCount` (4499) | Yidaro-specific counter |
| `theringtempts:N` | `theringtempts:N` (default 1) | `AAAlterRingTemptations` (4515) | The Ring tempts you N times. `andAbility` |
| `becomesmonarch` | | `AAAlterMonarch` (4537) | Become the monarch |
| `taketheinitiative` | | `AAAlterInitiative` (4547) | Take the initiative |
| `altersurvoffset:N` | | `AAAlterSurveilOffset` (4635) | Surveil offset (Enhanced Surveillance) |
| `alterdevoffset:N` | | `AAAlterDevotionOffset` (4651) | Devotion offset (Altar of the Pantheon) |

### Mutation counters (direct card-field writes, **no ability object** — side-effecting parse)
- `altermutationcounter:N` (4557) — adds to `card->mutation`, fires `WEventCardMutated`. **Returns nothing** (falls through).
- `mutationover:N` (4570) / `mutationunder:N` (4579) — adjust `card->mutation`, no event. **Bug note**: both read `splitMutated[1]` (the `altermutationcounter` split), not their own — likely a copy-paste defect; verify before relying.

### Event-performing effects (`oneShot=1`)
| Keyword | Class (file:line) | Notes |
|---|---|---|
| `doboast` | `AABoastEvent` (4589) | Perform boast |
| `surveil` | `AASurveilEvent` (4599) | Surveil (see `scry` for the interactive form) |
| `explores` | `AAExploresEvent` (4609) | Creature explores |
| `becomesringbearer` | `AARingBearerChosen` (4619) | Becomes the Ring-bearer. `andAbility` |
| `doforetell` | `AAForetell` (4096) | Foretell |

### Card creation / cloning / casting
| Keyword | Syntax | Class (file:line) | Shape | Notes / example |
|---|---|---|---|---|
| `conjure` | `conjure cards(NAME) zone(ZONE)` | `AAConjure` (4064) | one-shot, non-interruptible | `NAME=myname` → source card's name. `^`→`,` escaping. `andAbility` |
| `clone` | `clone with(...) addtype(...) options(...)` | `AACloner` (4174) | one-shot | Copy a permanent. `^`→`,`. `andAbility`. Ex: **Cackling Counterpart** mtg.txt:16227 |
| `castcard(...)` | flags: `restricted,copied,normal,madness,noevent,putinplay,alternative,flashback,flipped`, `named!:X:!`, `kicked!:N:!`, `costx!:N:!` | `AACastCard` (4262) | **`oneShot=false`** | Cast/play a card free. `trigger[to]` wires a `TriggerTargetChooser`. `andAbility` |
| `train` (implied) | | `AATrain` (4051) | one-shot | Train mechanic. `andAbility` |

### Transform / become / animate (continuous vs instant per duration rule)
| Keyword | Syntax | Class (file:line) | Shape | Notes / example |
|---|---|---|---|---|
| `becomes(TYPES,P/T,abilities...)` | `^`→`,` | `ATransformer` (5207/5240) or `ATransformerInstant` (5238) if `oneShot\|ueot\|forever` | continuous OR instant | Animate/become. Passes `forceForever/uynt/uent`. Ex: **Academy Rector** mtg.txt:522 |
| `remake(TYPES:P/T:abilities)` | `:`-delimited variant of becomes | same classes (5244) | same | Alternate syntax for becomes |
| `transforms((TYPES,abilities))` | nested `setpower= / settoughness= / newability[...]` | `ATransformer` / `ATransformerInstant` (5322–5391) | continuous OR instant | Mass/self type-change (Hivestone, living enchantment). Handles nested transforms. Ex: **Abbot of Keral Keep** mtg.txt:72 |
| `flip(STATS) forcetype(...) undocpy` | | `AAFlip` (5470) | one-shot | Flip cards; `undocpy` reverts copy at EOT; kamiflip → transmode. `andAbility` |
| `doubleside(NAME)` | | `AATurnSide` (5455) | one-shot | Turn a DFC to other side (not allowed on battlefield) |
| `morph` | | `AAMorph` (5774) | one-shot | Turn face up |
| `manafaceup` | | `AAMorph` (5782, `face=true`) | one-shot | Face-up via mana |
| `manifest` | `manifest [withenchant]` | `AManifest` (4113) | one-shot | Cloudform/rageform/lightform. `andAbility`. Ex: **Cloudform** mtg.txt:20876 |
| `meldfrom(NAMES)` | `^`→`,` | `AAMeldFrom` (5419) | one-shot | Meld helper |
| `meld(NAME)` | | `AAMeld` (5434) | one-shot | Meld. `andAbility` |

### Counters (on cards)
| Keyword | Class (file:line) | Shape | Notes / example |
|---|---|---|---|
| `counter(SPEC)` | `AACounter` (5076) | one-shot | Add/remove counters; `notrg` suppresses `@counter` trigger. Ex: **Abzan Advantage** mtg.txt:388 |
| `countershroud(TYPE)(tc)` | `ACounterShroud` (5105) | continuous | Prevent counters of a type (`any` = all) |
| `countertrack(NAME)` | `ACounterTracker` (5133) | continuous | Track an effect by counter name |
| `duplicatecounters(all\|single)` | `AADuplicateCounters` (5140) | one-shot, non-interruptible | Double counters |
| `removesinglecountertype(N[,all])` | `AARemoveSingleCounter` (5154) | one-shot | Remove one counter of any type |
| `removeallcounters(SPEC\|all)` | `AARemoveAllCounter` (5181) | one-shot | Remove all counters of a type |
| `proliferate [noproftrg]` | `AAProliferate` (5691, `allcounters=true`) | one-shot, non-interruptible | Proliferate. Ex: **Contagion Engine** mtg.txt:22190 |
| `propagate [noproftrg]` | `AAProliferate` (5704) | one-shot, non-interruptible | Proliferate-all-counters variant |

### Static keyword-ability grants and combat restrictions (continuous vs instant per duration rule)
| Keyword | Class (file:line) | Shape | Notes / example |
|---|---|---|---|
| basic ability name (e.g. `flying`, `-trample`) | `ABasicAbilityModifier` (5851) / `AInstantBasicAbilityModifierUntilEOT` / `ABasicAbilityAuraModifierUntilEOT` | continuous / UEOT / aura-UEOT | Grant/remove (`-` prefix removes) one of `Constants::MTGBasicAbilities`. `absorb`/`flanking` special-cased |
| `loseabilities` | `ALoseAbilities` (5069) | continuous | Strip all abilities |
| `losesubtypesof(TYPE)` | `ALoseSubtypes` (4904, `false`) | continuous | Lose all subtypes of a parent type |
| `losesatype(TYPE)` | `ALoseSubtypes` (4911, `true`) | continuous | Lose one specific type |
| `protection from(SPEC)` | `AProtectionFrom` (5558) wrapped in `GenericInstantAbility` for instant/sorcery | continuous OR instant | Quality-based widens to all zones; zone-scoped (`|`) kept. Ex: **Angelic Curator** mtg.txt:3937 |
| `cantbetargetof(SPEC)` | `ACantBeTargetFrom` (5587) | continuous | Sets all-zones; only non-activated |
| `cantbeblockedby(SPEC)` | `ACantBeBlockedBy` (5607) | continuous | Ex: **Amrou Kithkin** mtg.txt:3178 |
| `cantbeblockerof(SPEC\|this)` | `ACantBeBlockerOf` (5627) | continuous | `this` → blocker of source |
| P/T change (e.g. `+2/+2`, `nonstatic`, `cdaactive`) | `APowerToughnessModifier` (5645) / `PTInstant` (instant/UEOT) | continuous / instant | Parsed by `WParsedPT`; `cdaactive` sets `forcedAlive` |

### Combat keyword mechanics (mostly permanent static abilities)
| Keyword | Class (file:line) | Shape | Example |
|---|---|---|---|
| `rampage(P/T,max)` | `ARampageAbility` (4959) | static | `^`→`,`. Ex: **Aerathi Berserker** mtg.txt:1031 |
| `flanker` | `AFlankerAbility` (5022) | static | Flanking. Ex: **Agility** mtg.txt:1770 |
| `bushido(P/T)` | `ABushidoAbility` (5040) | static | Ex: **Araba Mothrider** mtg.txt:4691 |
| `spiritlink` / `combatspiritlink` | `ASpiritLinkAbility` (5029) | static (triggered on damage) | combat-only variant. Ex: **Armadillo Cloak** mtg.txt:5740 |
| `provoke` | `AProvoke` (4145) | one-shot (instant) | `andAbility`. Ex: **Brontotherium** mtg.txt:15211 |
| `setblocker` | `AProvoke` (4159, `setblocker=true`) | one-shot | Force-block variant |
| `exert` | `AExert` (4131) | one-shot | `andAbility` |
| `evolve` | `AEvolveAbility` (4980) | triggered/static | Evolve |
| `bloodthirst:N` | `ABloodThirst` (5280) | ETB static | Ex: **Battering Wurm** mtg.txt:9620 |
| `vanishing:N` | `AVanishing` (5293, `"time"`) | upkeep static | Time counters. Ex: **Maelstrom Djinn** mtg.txt:69500 |
| `fading:N` | `AVanishing` (5306, `"fade"`) | upkeep static | Ex: **Ancient Hydra** mtg.txt:3567 |
| `regenerate` | `AStandardRegenerate` (5842) | one-shot | Ex: **Albino Troll** mtg.txt:2521 |
| `swap` | `ASwapPTUEOT` (5824) | one-shot (UEOT) | Swap target's P/T. Ex: **Calcite Snapper** mtg.txt:16373 |
| `exchangelife` | `AAExchangeLife` (5831) | one-shot | Exchange life (creature: toughness=life). Ex: **Magus of the Mirror** mtg.txt:70020 |
| `bstw` (bestow) | `ABestow` (5094) | one-shot | Ex: **Baleful Eidolon** mtg.txt:8508 |

### Zones / tap / phase / control
| Keyword | Class (file:line) | Shape | Notes |
|---|---|---|---|
| `tap` / `tap(noevent)` | `AATapper` (5894) | one-shot | `andAbility`. Icy-Manipulator-style |
| `untap` | `AAUntapper` (5879) | one-shot | `andAbility`. Ley Druid |
| `frozen` | `AAFrozen` (5716, `false`) | one-shot | Won't untap next untap step. `andAbility` |
| `freeze` | `AAFrozen` (5731, `true`) | one-shot | Freeze variant. `andAbility` |
| `phaseout` | `AAPhaseOut` (4104) | one-shot | Ex: **Crystal Golem** mtg.txt:24599 |
| `phasealter(add/remove,X,Y[,after<Z>]) [nextphase]` | `APhaseAlter` via `GenericAbilityMod` (5049) | triggered | Add/remove phases; non-interruptible |
| `steal` | `ASeizeWrapper` (5935) | one-shot | Control until source leaves battlefield. `andAbility` |
| `block` | `AABlock` (5917) | one-shot | Standard block |
| `connect` | `AAConnect` (5926) | one-shot | Associate cards |
| `connectrule` | `ParentChildRule` observer (5909) | rule install | Destroy children if parent dies; **returns NULL** |
| `retarget`/`newtarget` | `AANewTarget` (5747) | one-shot | Refresh/re-target; `untp` untaps, `fromplay` flag |
| `rehook`/`newhook` | `AANewTarget` (5757) | one-shot | Re-equip (Puresteel Paladin/Stonehewer); `untp` |
| `mutateover`/`mutateunder` | `AANewTarget` (5767) | one-shot | Mutation re-target (1=over, 2=under) |

### Library / hand / deck
| Keyword | Class (file:line) | Shape | Notes / example |
|---|---|---|---|
| `draw:N [noreplace]` | `AADrawer` (4786) | one-shot | `andAbility`; `noreplace` skips draw-replacement. Ex: **Abeyance** mtg.txt:109 |
| `discard:N` | `AARandomDiscarder` (4948) | one-shot | Random discard |
| `shuffle` | `AAShuffle` (4864) | one-shot | `andAbility`. Ex: **Absorb Vis** mtg.txt:254 |
| `serumpowder` | `AAMulligan` (4880) | one-shot | Serum Powder mulligan |
| `deplete:N` (`colordeplete`/`namedeplete`) | `AADepleter` (4801, `ingest=false`) | one-shot | Mill; color/name repeat flags. `andAbility` |
| `ingest:N` (`coloringest`/`nameingest`) | `AADepleter` (4823, `ingest=true`) | one-shot | Exile-mill variant |
| `cascade:N` | `AACascade` (4845) | one-shot | Ex: **Ardent Plea** mtg.txt:5522 |
| `maxlevel:N` | `AAWhatsMax` (5792) | one-shot | Leveler cap |
| `reveal:X ... revealend` | `GenericRevealAbility` (5394) | one-shot, non-interruptible | Reveal then run stored ability |
| `scry:X ... scryend` | `GenericScryAbility` (5407) | one-shot, non-interruptible | Scry |

### Hand-size / turns / misc
| Keyword | Class (file:line) | Notes |
|---|---|---|
| `hmodifer:X` | `AModifyHand` (4683) | Modify max hand size (note spelling **hmodifer**) |
| `sethand:N` | `AASetHand` (4716) | Set hand size |
| `turns:X` | `AAModTurn` (4854) | Extra/modify turns |
| `count(SPEC)` | `AACountObject` (5807) | Count objects → dynamic value |
| `countb(SPEC)` | `AACountObjectB` (5815) | Count objects (variant B) |
| `donothing` | `AAFakeAbility` (4388) | No-op placeholder |
| `epic` | `AAEPIC` (4398) | Epic spell |
| `forcefield` | `AAEPIC` (4408, `"Forcefield"`, forcefield flag) | Forcefield effect |

### Mana production / cost alteration (the mana-redux family)
| Keyword | Syntax | Class (file:line) | Shape | Notes / example |
|---|---|---|---|---|
| `add<mana>` / `out<mana>` (`doesntempty`) | e.g. `add{G}{G}` | `AManaProducer` (5524 / 5544) | one-shot | Produce mana; `doesntempty` = doesn't empty till EOT; `out` = canproduce-exempt variant. `andAbility`, `menutext` |
| `produceextra:X` | | `AProduceMana` (4985) | static | Extra mana when tapped for mana. Ex: **Bubbling Muck** mtg.txt:15543 |
| `producecolor:X` | | `AEngagedManaAbility` (4991) | static | Extra mana when a mana is engaged |
| `removemana(SPEC) [forceclean]` | | `AARemoveMana` (4890) | one-shot | Remove mana from pool |
| `reduceto:X` | | `AReduceToAbility` (4997) | static | Reduce life to a specific value |
| **`altercost(COLOR,±N)`** | color ∈ colorless/red/white/black/blue/green | **`AAlterCost`** via `getManaReduxAbility` (5318→7227) | **continuous/static** | ±N cost change. **Cannot be used in activated abilities or instants/sorceries** (per wiki). Ex: **Alabaster Leech** mtg.txt:2407 |
| `changecost(COLOR:±N)` | `:`-delimited alt syntax | `AAlterCost` (5500) | continuous/static | Alternate spelling. Ex: **Cloud Key** mtg.txt:20749 |
| `modbenchant(COLOR:N)` | | `AAuraIncreaseReduce` (4691) | continuous | Bestow cost increase/reduce |
| `affinity(tc)reduce(mana)` | | `ANewAffinity` (5676) | continuous/static | Affinity for a targetchooser-defined set |

**`getManaReduxAbility` semantics (7227–7256)**: parses the color from `Constants::kMana*` strings, parses the ±amount via `WParsedInt`, returns `AAlterCost(observer,id,card,target,amount,color)`. Colorless maps to color index 0. Returns NULL (with DebugTrace) if no valid color found. This is a **static continuous** modifier, not one-shot.

### Cast/play restrictions
- `kMaxCastKeywords[i](TC) N` (MTGAbility.cpp:4919) → `ACastRestriction` (continuous) or `AInstantCastRestrictionUEOT` (instant/sorcery, UEOT). `+`/`-` in value → `modifyExisting`. Limits how many of a card type can be cast per turn.

### `andAbility` chaining (pervasive)
Many one-shot effects read a module-level `storedAndAbility`; if present it is recursively `parseMagicLine`-parsed and attached as `->andAbility`, letting one keyword run a follow-up effect. Classes supporting it: AATrain, AAConjure, AManifest, AExert, AProvoke, AACloner, AABuryCard, AADestroyCard, AASacrificeCard, AADiscardCard, AACastCard, AADamager, AAAlterRingTemptations, AARingBearerChosen, AALifer (×2), AADrawer, AADepleter (×2), AAShuffle, AAMeld, AAFlip, AManaProducer, AAFrozen (×2), AAUntapper, AATapper, ASeizeWrapper.

---

## PART B — Ability-class catalog (AllAbilities.h)

Base hierarchy: `MTGAbility` → {`ActivatedAbility`, `InstantAbility`, `TriggeredAbility`, `ListMaintainerAbility`, `TargetAbility`, `AbilityTP`}. **TP** suffix = "targeted player" variant (has `who`/target-player plumbing). **`InstantAbility` = resolves-then-dies (UEOT/one-shot) shape**; **plain `MTGAbility` subclass = continuous/static**; **`TriggeredAbility` = fires on events**; **`Tr*` = event Triggers (listeners), not effects**.

### Triggers (event listeners — `Tr*`, all `: public Trigger`) — lines 160–1754
These detect `WEvent`s and fire an attached ability. They are the `@`/trigger side, not effects:
`TrCardAddedToZone` (160, zone-change/ETB/LTB/dies), `TrCardTapped` (231), `TrCardTappedformana` (264), `TrCardManaproduced` (295), `TrCardPhasesIn` (323), `TrCardFaceUp` (351), `TrCardTransformed` (379), `TrCardExerted` (407), `TrCombatTrigger` (435, attacks/blocks/deals combat dmg), `TrplayerPoisoned` (554), `TrplayerEnergized` (599), `TrplayerExperienced` (644), `TrplayerMonarch` (689), `TrplayerTempted` (718), `TrplayerProliferated` (747), `TrplayerInitiative` (778), `TrplayerShuffled` (807), `TrcardDrawn` (836), `TrCardMutated` (871), `TrCardExplored` (899), `TrCardBearerChosen` (927), `TrCardBoasted` (958), `TrCardDefeated` (986), `TrCardSurveiled` (1014), `TrCardForetold` (1042), `TrCardTrained` (1070), `TrCardScryed` (1098), `TrCardNinja` (1128), `TrCardDungeonCompleted` (1156), `TrCardRolledDie` (1190), `TrCardFlippedCoin` (1226), `TrTokenCreated` (1264), `TrCardSacrificed` (1293), `TrCardExploited` (1335), `TrCardDiscarded` (1377), `TrDamaged` (1417), `TrLifeGained` (1482), `TrVampired` (1536), `TrTargeted` (1578), `TrCounter` (1615, counter added), `TrTotalCounter` (1665).

### Damage / prevention / replacement — the distinction matters
| Class (line) | Kind | Purpose / keyword |
|---|---|---|
| `AADamager` (4805) | one-shot effect | Deal damage — `damage:` |
| `TADamager` (5028) | targeted | Targeted damage variant |
| `AFireball` (6741) | instant | Fireball X-damage split |
| `AADamagePrevent` (4822) | **PREVENTION** | `prevent:N` — prevent next N damage |
| `APreventDamageTypes` (5718) | **PREVENTION** (continuous) | `preventall*` / `fog` |
| `APreventDamageTypesUEOT` (5782) | **PREVENTION** (instant/UEOT) | fog / ueot forms |
| `ACircleOfProtection` (3300) | **PREVENTION** | CoP-style color prevention |
| `AAResetDamage` (1822/4805) | one-shot | `resetdamage` |
| `ADrawReplacer` (6112) | **REPLACEMENT** | Replaces draw events |
| `AManaPoolSaver` (6082) | **REPLACEMENT** | Mana doesn't empty (keep mana) |
| `AConservator` (6258), `AIslandSanctuary` (6771), `AForcefield`/`AAEPIC` (1841) | replacement/prevention | Named legacy prevention cards |

### Counters
`AACounter` (1782, add/remove), `AARemoveAllCounter` (1803), `AAProliferate` (1953), `AADuplicateCounters` (1965), `AARemoveSingleCounter` (1976), `ACounterShroud` (5732, prevent counters), `ACounterTracker` (5745), `AModularAbility` (7892, modular +1/+1 on death).

### Tokens / card creation
`ATokenCreator` (3933, `token(...)`), `ATargetedAbilityCreator` (4264), `AAConjure` (2109, `conjure`), `AAImprint` (2076), `AAHaunt` (2087), `AACopier` (2053), `AACloner` (2133, `clone`), `AManifest` (7591, `manifest`).

### Movement / zones
`AAMover` (2156, move card between zones), `AARandomMover` (2176), `AABuryCard` (2193, `bury`), `AADestroyCard` (2205, `destroy`), `AASacrificeCard` (2216, `sacrifice`), `AADiscardCard` (2228, `reject`), `AALibraryPosition` (2028), `AALibraryBottom` (2041), `AAPhaseOut` (2067, `phaseout`), `ABlink` (6045) / `ABlinkGeneric` (6066, flicker), `AAForetell` (2124), `AAMeldFrom` (5167) / `AAMeld` (5178, `meld`), `AATurnSide` (5192, `doubleside`), `AAFlip` (5204, `flip`), `AAMorph` (5155, `morph`), `AAnimateDead` (6416, reanimate).

### Control / steal
`ASeize` (5948) / `ASeizeWrapper` (5967, `steal`), `AShackle` (5981) / `AShackleWrapper` (5999, tap-lock steal), `AControlStealAura` (5470, aura control), `AInstantControlSteal` (7388), `AGrant` (6012) / `AGrantWrapper` (6031, grant control/ability), `AKjeldoranFrostbeast` (6356), `AErgRaiders` (6457).

### Mana / cost
`AManaProducer` (via ATokenCreator area / built at parse), `AProduceMana` (5814, `produceextra`), `AEngagedManaAbility` (5830, `producecolor`), `AARemoveMana` (7077, `removemana`), `AAlterCost` (5538, `altercost`/`changecost`), `ANewAffinity` (1867, `affinity`), `AAuraIncreaseReduce` (4747, bestow cost), `AReduceToAbility` (7192, `reduceto`), `AManaPoolSaver` (6082), `AFastbond` (6506), `AJandorsRing` (6578).

### Life / player resources
`AALifer` (2397, `life`/`lifeleech`), `AALifeSet` (4791, `lifeset`), `AAExchangeLife` (5348, `exchangelife`), `AAWinGame` (2413, `wingame`), `ASpellCastLife` (2583), `ARegularLifeModifierAura` (3397), `ALifeZoneLink` (5361), `APowerLeak` (6614), `AAAlterPoison/Energy/Experience/DungeonCompleted/YidaroCount/RingTemptations/Monarch/Initiative/SurveilOffset/DevotionOffset` (4835–5014, the `alter*` player-resource family), `AAModTurn` (7038, `turns`), `AAShuffle` (7051), `AAMulligan` (7064, `serumpowder`), `AADepleter` (7004, `deplete`/`ingest`), `AARandomDiscarder` (7091, `discard`), `AADrawer` (2347, `draw`), `AACascade` (7022, `cascade`), `AModifyHand` (4763, `hmodifer`), `AASetHand` (4777, `sethand`).

### P/T & type/ability modification (static/continuous vs instant)
| Class (line) | Shape | Keyword |
|---|---|---|
| `APowerToughnessModifier` (2862) | continuous | `+N/+N` etc. |
| `PTInstant` (5624) | instant/UEOT | instant P/T |
| `ASwapPT` (5290) / `ASwapPTUEOT` (5707) | instant | `swap` |
| `ABasicAbilityModifier` (2429) | continuous | grant/remove keyword ability |
| `AInstantBasicAbilityModifierUntilEOT` (2479) | instant/UEOT | grant keyword UEOT |
| `ABasicAbilityAuraModifierUntilEOT` (2530) | aura UEOT | activated keyword grant |
| `ATransformer` (5557) | continuous | `becomes`/`transforms`/`remake` |
| `ATransformerInstant` (5600) | instant | one-shot/ueot/forever transform |
| `ALoseAbilities` (5758) | continuous | `loseabilities` |
| `ALoseSubtypes` (5769) | continuous | `losesubtypesof`/`losesatype` |
| `AAlterCost` (5538) | static | cost mod |
| `AEarthbind` (6716) | continuous | grounds flyers + damage |

### Combat static mechanics
`ARampageAbility` (7104, `rampage`), `AFlankerAbility` (7223, `flanker`), `ABushidoAbility` (7272, `bushido`), `ASpiritLinkAbility` (7355, `spiritlink`), `AProvoke` (7699, `provoke`/`setblocker`), `AExert` (7756, `exert`), `AEvolveAbility` (7161, `evolve`), `ABloodThirst` (5505, `bloodthirst`), `AVanishing` (5797, `vanishing`/`fading`), `AExalted` (5642, exalted, `TriggeredAbility`), `AUnBlocker` (2654), `ACombatRemoval` (2315), `ANinja` (2266, ninjutsu), `AACastCard` (7321, `castcard`), `AEquip` (3909, equip), `APaired` (4355, soulbond), `PairCard` (3884), `dredgeCard` (3892, dredge), `AAttackSetCost` (5921, `attackcost`), `ABlockSetCost` (5935, `blockcost`).

### Attack/block-cost & regen
`AStandardRegenerate` (3362, `regenerate`), `ATriggerRegen` (7808), `ASacrifice` (6683).

### Grant/lord/list-maintainer (mass continuous effects over a set)
`ALord` (3607, `lord(...)` — apply nested ability to matching cards), `AAsLongAs` (3438, conditional continuous), `ATeach` (3749, grant activated ability), `AForeach` (4450, per-matching-card scaling), `AThis` (4543, self-referential), `AThisForEach` (4641), `APaired` (4355). These wrap a `NestedAbility`.

### Wrappers / control-flow (build/attach other abilities)
`GenericActivatedAbility` (2008), `GenericTargetAbility` (2240), `GenericInstantAbility` (3065), `GenericAbilityMod` (3200), `GenericAddToGame` (3247), `GenericPaidAbility` (7572), `GenericRevealAbility` (92, `reveal`), `GenericScryAbility` (142, `scry`), `MayAbility` (1894, optional), `MenuAbility` (1925, choose-one), `MultiAbility` (1989, sequence), `IfThenAbility` (1879, conditional), `ADeferredOneShot` (3122), `AEvokeSacrifice` (3159, evoke), `AUpkeep` (5857, upkeep cost). Choice helpers: `GenericChooseTypeColorName` (7491) / `AASetColorChosen` (7453) / `AASetTypeChosen` (7465) / `AASetNameChosen` (7478); `GenericFlipACoin` (7527)/`AASetCoin` (7513); `GenericRollDie` (7557)/`AASetDie` (7542).

### Utility / misc / named-legacy
`AACounter`-adjacent: `AAFizzler` (1852, counter spell), `AAResetDamage` (1822). Value helpers: `AAWhatsX` (5082), `AAWhatsMax` (5098, `maxlevel`), `AACountObject` (5108, `count`) / `AACountObjectB` (5118, `countb`), `AADynamic` (5222, dynamic effect). `AAFrozen` (5128, `frozen`/`freeze`), `AANewTarget` (5140, `retarget`/`newhook`/`mutate*`), `AATapper` (5057, `tap`) / `AAUntapper` (5070, `untap`), `AAConnect` (3900, `connect`), `AABlock` (3875, `block`), `AAFakeAbility` (1831, `donothing`), `AAEPIC` (1841, `epic`/`forcefield`). Named single-card legacy classes: `AAladdinsLamp` (6112), `AArmageddonClock` (6194), `AConservator` (6258), `AKjeldoranFrostbeast` (6356), `AAnimateDead` (6416), `AErgRaiders` (6457), `AFastbond` (6506), `AJandorsRing` (6578), `APowerLeak` (6614), `AIslandSanctuary` (6771), `AStrongLandLinkCreature` (5429), `AProduceMana`/`AEngagedManaAbility`. UI/tutorial: `ATutorialMessage` (1754), `MTGEventText` (31), `MTGRevealingCards` (42), `RevealDisplay` (83), `MTGScryCards` (106). Phase-scheduling: `APhaseAction` (5880), `APhaseActionGeneric` (5906), `APhaseAlter` (6839, `phasealter`). Triggered-legacy: `ATriggerTotem` (7853), `ATriggerRegen` (7808).

### REPLACEMENT-EFFECT / continuous classes to flag for Oracle fidelity
- **Replacement**: `ADrawReplacer` (6112), `AManaPoolSaver` (6082), `AAnimateDead` (6416, ETB-as replacement), `ABlink`/`ABlinkGeneric` (LTB→return).
- **Prevention (NOT replacement)**: `AADamagePrevent`, `APreventDamageTypes(UEOT)`, `ACircleOfProtection`, `AConservator`, `AIslandSanctuary`.
- **Continuous/static** (plain `MTGAbility` subclass, persists while source in zone): `APowerToughnessModifier`, `ABasicAbilityModifier`, `ATransformer`, `ALord`, `AAsLongAs`, `AProtectionFrom`, `ACantBe*`, `AAlterCost`, `AProduceMana`, `AVanishing`.
- **Instant/UEOT** (`InstantAbility` subclass, resolve-then-die): all `*Instant`, `*UEOT`, `PTInstant`, `AInstant*`, `GenericInstantAbility`.

**Gotchas for the LLM opponent / faithfulness**:
1. Keyword matching in parseMagicLine is **`s.find()` substring**, order-dependent — earlier branches win. E.g. `untap` is checked (5879) *before* `tap` (5894) so `tap` won't mis-fire on `untap`; but `add`/`out` mana matching (5524/5544) is broad `find("add")`/`find("out")` and can false-positive on substrings — a real bug surface.
2. `mutationover:`/`mutationunder:` read the wrong split variable (likely bug).
3. Misspelled but load-bearing tokens: **`hmodifer`** (not "hmodifier"), **`reject`** (= discard), **`bstw`** (= bestow), **`serumpowder`**.
4. Duration is governed by the source card type + `forever/uent/uynt/ueot/oneshot` flags — the same keyword yields a continuous vs instant class depending on them; the LLM must not assume permanence.

---

