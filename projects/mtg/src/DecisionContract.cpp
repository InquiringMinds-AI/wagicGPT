#include "PrecompiledHeader.h"

#include "DecisionContract.h"
#include "ActionLayer.h"
#include "AllAbilities.h"
#include "CardDescriptor.h"
#include "GameObserver.h"
#include "MTGAbility.h"
#include "MTGRules.h" //#W64-AI (F4): MTGPlaneswalkerAttackRule
#include "Player.h"
#include "TargetChooser.h"

bool DecisionManager::buildDeclareAttackers(Player * p, DecisionRequest & req)
{
    req.kind = DecisionRequest::DECLARE_ATTACKERS;
    req.player = p;
    req.candidates.clear();
    CardDescriptor cd;
    cd.init();
    cd.setType("creature");
    MTGCardInstance * card = NULL;
    //#W64-AI (F4, deck152 HIGH-2): the OTHER thing an attack can be aimed at.
    //The engine has supported attacking a planeswalker for years
    //(MTGPlaneswalkerAttackRule) but no decision surface ever offered it: 0 of
    //the corpus's attackers prompts named one, and at 152v162 seq 17 a
    //3-loyalty Ob Nixilis faced three unblockable attackers on a creatureless
    //board, survived, and dealt 6 of the damage that killed the seat.
    //Enumerated in the SAME walk and the same order the rule's own choice menu
    //is built from (the opponent's battlefield, planeswalkers and battles), so
    //index i here is index i on that menu and the apply path needs no name
    //matching. `canAttack(true)` is the engine's own planeswalker-attack gate.
    req.attackTargets.clear();
    Player * opp = p->opponent();
    if (opp && opp->game && opp->game->battlefield)
    {
        MTGGameZone * bf = opp->game->battlefield;
        for (int i = 0; i < (int) bf->cards.size(); i++)
        {
            MTGCardInstance * c = bf->cards[i];
            if (c && (c->hasType(Subtypes::TYPE_PLANESWALKER) || c->hasType(Subtypes::TYPE_BATTLE)))
                req.attackTargets.push_back(c);
        }
    }
    //#W64-AK (R1, wave-64 codex review finding 1): the candidate walk runs AFTER
    //the target walk, because whether a planeswalker exists is half of whether a
    //creature has any legal attack at all. `canAttack()` gates the PLAYER
    //(CANTATTACK, FLYERSONLY); `canAttack(true)` gates a planeswalker or battle
    //(CANTPWATTACK), and it is the same call MTGPlaneswalkerAttackRule's own
    //isReactingToClick makes. A creature that fails the first and passes the
    //second has a legal attack this seat may declare, and wave 64 dropped it
    //from `candidates` before any consumer could see it - the row was never
    //rendered, so no reply could name it. Each candidate now carries which
    //destinations it may be sent at, and both later guards ask that instead of
    //re-asking the player predicate.
    req.candidateMayAttackPlayer.clear();
    req.candidateMayAttackTarget.clear();
    while ((card = cd.nextmatch(p->game->inPlay, card)))
    {
        if (card->isAttacker())
            continue;
        const bool mayPlayer = card->canAttack() != 0;
        const bool mayTarget = !req.attackTargets.empty() && card->canAttack(true) != 0
                               && !card->isPhased;
        if (!mayPlayer && !mayTarget)
            continue;
        req.candidates.push_back(card);
        req.candidateMayAttackPlayer.push_back(mayPlayer);
        req.candidateMayAttackTarget.push_back(mayTarget);
    }
    return !req.candidates.empty();
}

bool DecisionManager::buildDeclareBlockers(Player * p, DecisionRequest & req)
{
    req.kind = DecisionRequest::DECLARE_BLOCKERS;
    req.player = p;
    req.attackers.clear();
    req.blockers.clear();
    req.legalPerBlocker.clear();

    GameObserver * g = p->getObserver();
    Player * attackerP = g->currentPlayer;
    if (attackerP == p)
        return false;
    CardDescriptor ca;
    ca.init();
    ca.setType("creature");
    MTGCardInstance * a = NULL;
    while ((a = ca.nextmatch(attackerP->game->inPlay, a)))
        if (a->isAttacker())
            req.attackers.push_back(a);
    if (req.attackers.empty())
        return false;

    CardDescriptor cb;
    cb.init();
    cb.setType("creature");
    cb.unsecureSetTapped(-1);
    MTGCardInstance * blk = NULL;
    while ((blk = cb.nextmatch(p->game->inPlay, blk)))
    {
        if (blk->defenser || !blk->canBlock())
            continue;
        std::vector<MTGCardInstance*> legal;
        for (size_t j = 0; j < req.attackers.size(); j++)
            if (blk->canBlock(req.attackers[j]))
                legal.push_back(req.attackers[j]);
        if (legal.empty())
            continue;
        req.blockers.push_back(blk);
        req.legalPerBlocker.push_back(legal);
    }
    return !req.blockers.empty();
}

void DecisionManager::applyDeclareAttackers(const DecisionRequest & req, const DecisionAction & act)
{
    GameObserver * g = req.player->getObserver();
    for (size_t i = 0; i < act.attackers.size(); i++)
    {
        MTGCardInstance * card = act.attackers[i];
        //re-validate against the live board AND the offered options: a
        //consumer can only play what it was asked about
        bool offered = false;
        for (size_t j = 0; !offered && j < req.candidates.size(); j++)
            offered = req.candidates[j] == card;
        //#W64-AK (R1): the PLAYER predicate is no longer the gate on being
        //declared at all - a creature offered for a planeswalker attack fails it
        //by construction. It is re-asked below, on the one route it governs.
        if (!offered || card->isAttacker())
            continue;
        //#W64-AI (F4): a declared planeswalker/battle target rides the
        //ENGINE's own planeswalker-attack rule, whose reactToClick arms a
        //choice menu over the same list buildDeclareAttackers enumerated.
        //Answer that menu in this tick: the attacker loop clicks the next
        //creature immediately, and an armed menu would swallow that click.
        //Anything the rule refuses (no walker on their board, an attack cost
        //not paid, a phased body) falls through to the ordinary attack rule,
        //so a target this seat may not legally take costs it nothing but the
        //target - the declaration itself still stands.
        MTGCardInstance * pwTarget = (i < act.attackerTargets.size()) ? act.attackerTargets[i] : NULL;
        int pwIndex = -1;
        for (size_t k = 0; pwTarget && k < req.attackTargets.size(); k++)
            if (req.attackTargets[k] == pwTarget)
                pwIndex = (int) k;
        MTGPlaneswalkerAttackRule * pwRule = NULL;
        if (pwIndex >= 0 && card->canAttack(true) && !card->attackPlaneswalkerCost)
        {
            ActionLayer * al = g->mLayers ? g->mLayers->actionLayer() : NULL;
            for (size_t k = 1; al && !pwRule && k < al->mObjects.size(); k++)
                pwRule = dynamic_cast<MTGPlaneswalkerAttackRule *>((MTGAbility *) al->mObjects[k]);
        }
        if (pwRule && pwRule->isReactingToClick(card))
        {
            g->cardClick(card, pwRule);
            ActionLayer * al = g->mLayers ? g->mLayers->actionLayer() : NULL;
            if (al && al->menuObject)
                al->ButtonPressedOnMultipleChoice(pwIndex);
            if (card->isAttacker())
                continue; //declared at the walker
        }
        //#W64-AK (R1): the ordinary attack rule sends this creature at the
        //PLAYER, so the player predicate is asked HERE. A creature that may only
        //attack a planeswalker and whose walker route did not take (no rule
        //armed, an unpaid attack cost, a stale target) simply does not attack -
        //it is never sent somewhere it may not go.
        if (!card->canAttack())
            continue;
        g->cardClick(card, MTGAbility::MTG_ATTACK_RULE);
    }
}

void DecisionManager::applyDeclareBlockers(const DecisionRequest & req, const DecisionAction & act)
{
    GameObserver * g = req.player->getObserver();
    for (size_t i = 0; i < act.blocks.size(); i++)
    {
        MTGCardInstance * blocker = act.blocks[i].first;
        MTGCardInstance * chosen = act.blocks[i].second;
        if (!blocker || !chosen)
            continue;
        bool offered = false;
        for (size_t j = 0; !offered && j < req.blockers.size(); j++)
            offered = req.blockers[j] == blocker;
        if (!offered || blocker->defenser || !blocker->canBlock(chosen))
            continue;
        //The block rule CYCLES this creature's defenser through its legal
        //attackers (and NULL); click until it lands on the chosen one.
        //Bounded so an unexpected cycle can never spin forever. This is
        //the choreography the contract exists to encapsulate.
        int guard = (int) req.attackers.size() + 2;
        g->cardClick(blocker, MTGAbility::MTG_BLOCK_RULE);
        while (blocker->defenser != chosen && guard-- > 0)
            g->cardClick(blocker, MTGAbility::MTG_BLOCK_RULE);
    }
}

namespace
{
    //The latest armed MenuAbility on the action layer - the one the open
    //multiple-choice menu belongs to (mirrors the AI's historical lookup).
    MenuAbility * currentMenuAbility(ActionLayer * object)
    {
        //#W54-I (L18): `size()-1` underflowed on an empty layer; same walk
        //(indices size-1 .. 1, index 0 skipped as before), no underflow.
        for (size_t m = object->mObjects.size(); m-- > 1;)
        {
            MenuAbility * ability = dynamic_cast<MenuAbility *>(object->mObjects[m]);
            if (ability && ability->triggered)
                return ability;
        }
        return NULL;
    }

    //ActionLayer::currentActionCard is a raw pointer with no zone-change
    //invalidation (the known dangle class): a token that dies while its
    //menu is armed is deleted for real, and the stale pointer's vtable
    //slot reads 0 - calling getDisplayName() on it jumps to address zero
    //(SIGSEGV observed live 2026-07-11, core 3266478). Hand the card out
    //only when the game still knows it - POINTER COMPARISON ONLY, no
    //deref. Garbage-parked instances (normal zone moves) are valid memory
    //and stay usable.
    MTGCardInstance * validatedCardPointer(GameObserver * g, MTGCardInstance * card)
    {
        //shared implementation - the walk now lives on GameObserver so the
        //engine (ATransformer::destroy) can use it too
        return g->validateCardPointer(card);
    }

    //#W46-6: a card that cannot name itself. `ability$!...!$` payloads do not
    //resolve on their card: ATargetedAbilityCreator::resolve builds a bare
    //MTGCardInstance carrying only the payload's magicText, parks it in the
    //garbage zone and resolves it as a Spell, stamping the REAL creator into
    //storedSourceCard. A `choice ... _ choice ...` sub-ask inside such a
    //payload therefore arms its menu on that DUMMY (Teferi, Who Slows the
    //Sunset's +1 side chooser): currentActionCard is non-NULL, so the wave-45
    //E-3 option-source fallback below never fires, but the dummy has no name
    //and no model - and the seat rendered the subject-less "A choice is
    //required - choose an option:" header over "choose your land / choose
    //opponent land" (9 records, wave-45 corpus). Hand back the stamped source
    //when the card in hand has no name of its own; every named card, and every
    //nameless one with nothing stamped, is returned untouched.
    //A card the consumer can NAME: display name, raw instance name (the
    //ability$!!$ carrier's only rung - see resolveOwningCardName in the seat),
    //or the model's name. NULL names nothing.
    bool cardNamesItself(const MTGCardInstance * card)
    {
        if (!card)
            return false;
        return !card->getDisplayName().empty() || !card->name.empty()
               || (card->model && card->model->data && !card->model->data->getName().empty());
    }

    MTGCardInstance * nameableCardPointer(GameObserver * g, MTGCardInstance * card)
    {
        if (!card)
            return NULL;
        bool namesItself = cardNamesItself(card);
        if (namesItself || !card->storedSourceCard)
            return card;
        //Same dangle rail as every other pointer this file hands out: the
        //stamped source is validated before it can be dereferenced by a
        //consumer, and an unvalidatable one leaves the dummy in place.
        if (MTGCardInstance * src = g->validateCardPointer(card->storedSourceCard))
            return src;
        return card;
    }

    //For a triggered/activated "may" ability whose effect acts on an object
    //supplied by the trigger event (all(trigger[to]) moveto - the Tergrid
    //steal class), recover that resolved object so the ask can NAME it.
    //Mirrors GenericTriggeredAbility::set/getTriggerTarget: at resolve time
    //the sacrificed/discarded card was stamped onto the ->target of the node
    //bearing the TriggerTargetChooser, and MayAbility/AAMover clones copy it,
    //so the armed menu ability still carries it. Returns NULL when no single
    //trigger-supplied card target is resolvable - every non-may menu, and any
    //may whose object cannot be named, keeps its current generic text.
    MTGCardInstance * resolveTriggerObject(MTGAbility * a)
    {
        if (!a)
            return NULL;
        if (dynamic_cast<TriggerTargetChooser *>(a->getActionTc()))
            if (MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(a->target))
                return c;
        if (NestedAbility * na = dynamic_cast<NestedAbility *>(a))
            if (MTGCardInstance * c = resolveTriggerObject(na->ability))
                return c;
        if (MultiAbility * ma = dynamic_cast<MultiAbility *>(a))
            for (size_t i = 0; i < ma->abilities.size(); i++)
                if (MTGCardInstance * c = resolveTriggerObject(ma->abilities[i]))
                    return c;
        return NULL;
    }

    //"your graveyard" / "opponent's graveyard" (whatever zone the object now
    //sits in), relative to the deciding player.
    std::string objectOrigin(MTGCardInstance * obj, Player * decider)
    {
        MTGGameZone * z = obj ? obj->getCurrentZone() : NULL;
        if (!z)
            return std::string();
        std::string possessive = (z->owner == decider) ? "your " : "opponent's ";
        return possessive + z->getName();
    }

    //#W41-14: a may-gain-life option renders as the bare verb "Life" (or
    //"Life Loss"). Evaluate the magnitude the same way the engine will when it
    //resolves (AALifer::getLife runs the WParsedInt against the source), so the
    //label states the amount instead of leaving the pilot to re-derive it off
    //the battlefield line. Empty for every effect whose magnitude is not
    //derivable here - the seat then keeps the engine's own label.
    std::string mayEffectLabelFor(MTGAbility * may)
    {
        MTGAbility * core = AbilityFactory::getCoreAbility(may);
        if (AALifer * lifer = dynamic_cast<AALifer *>(core))
        {
            int n = lifer->getLife();
            if (!n)
                return std::string();
            std::ostringstream o;
            if (n > 0)
                o << "Gain " << n << " life";
            else
                o << "Lose " << -n << " life";
            return o.str();
        }
        return std::string();
    }

    //What the SEAT actually renders for this may - the evaluated effect label
    //when one exists, else the engine's own menu label. Batch identity is
    //decided on this string, so two triggers only group when the pilot would
    //read them as the same question.
    std::string renderedMayEffect(MTGAbility * may, const std::string & menuLabel)
    {
        std::string label = mayEffectLabelFor(may);
        return label.empty() ? menuLabel : label;
    }

    //The MayAbility behind a single-option may menu, or NULL. Batch identity
    //(#W41-7) requires the ability to be a `may` with NO per-instance target
    //choice and NO cast-restriction condition: a chooser makes each instance a
    //different question, and a condition can evaluate differently per instance.
    MayAbility * batchableMay(MTGAbility * a)
    {
        MayAbility * may = dynamic_cast<MayAbility *>(a);
        if (!may || !may->source || !may->ability)
            return NULL;
        if (dynamic_cast<MenuAbility *>(may))
            return NULL; //a custom multi-option menu is not an accept/decline may
        if (!may->Cond.empty())
            return NULL;
        if (dynamic_cast<TargetAbility *>(may->ability))
            return NULL;
        if (may->ability->getActionTc())
            return NULL;
        return may;
    }
}

bool DecisionManager::buildMenuChoice(Player * p, DecisionRequest & req)
{
    GameObserver * g = p->getObserver();
    ActionLayer * object = g->mLayers->actionLayer();
    if (!object->menuObject || !object->abilitiesMenu)
        return false;

    req.player = p;
    //validated: consumers render contextCard's name into prompts; a stale
    //pointer must become NULL (generic prompt text), never a deref
    //#W46-6: unwrap an ability$! payload dummy to the card that spawned it -
    //the menu's subject is the permanent the pilot can see, never the
    //nameless carrier the engine resolved the payload on.
    req.contextCard = nameableCardPointer(g, validatedCardPointer(g, object->currentActionCard));
    //#W48 D6 (wave-47 D6 = R7; 4/4 bare headers, Silverquill Command x3 + Peer
    //into the Abyss): the arm-time name. Both cards are `auto=choice name(...)`
    //lists, which parse into MayAbilities whose menu arms on the RESOLVING
    //spell - a card that names itself in full but sits in no game zone, so
    //validatedCardPointer refuses it (verified by probe: disp='Silverquill
    //Command' ... valid=(nil)) and BOTH the contextCard and the wave-45
    //option-source rescue below came back empty. A name captured at arm time
    //cannot dangle, so it survives the validation the pointer cannot.
    req.contextName = object->menuObjectName;
    req.optionTexts.clear();
    req.menuIndices.clear();
    req.canDecline = false;
    req.nameChoiceMenu = false; //#W55-D (D22)
    //a reused request must not carry a previous menu's may annotations
    req.mayObjectName.clear();
    req.mayObjectOrigin.clear();
    req.mayEffectLabel.clear();

    if (object->abilitiesMenu->isMultipleChoice && object->currentActionCard)
    {
        MenuAbility * menu = currentMenuAbility(object);
        if (!menu || menu->abilities.empty())
            return false;

        if (dynamic_cast<AAWhatsX *>(menu->abilities[0]))
        {
            if (menu->announcing && menu->announceCost)
            {
                //pay[[{X}]] announcement round: the menu already enumerates
                //the affordable X range (pool + producers, bounded at build
                //time); option index == X value. The pool-based computation
                //below assumes the cast flow's pre-floated payment and would
                //report "no announceable X" here, where the pool is empty.
                for (size_t x = 0; x < menu->abilities.size(); x++)
                {
                    std::ostringstream o;
                    o << "X = " << x;
                    req.optionTexts.push_back(o.str());
                }
                req.kind = DecisionRequest::ANNOUNCE_X;
                //#W62-Y (D5): the announcement round is a cancellable menu
                //(ActionLayer::setCustomMenuObject added a "Cancel" row because
                //the caller passed must=false), and MenuAbility::reactToChoiceClick
                //treats an out-of-range choice on an announcing menu as "the
                //player backed out of paying entirely; decline cleanly". Say so
                //on the contract - the seat had no way to reach a row a human
                //has always had.
                req.canDecline = !object->checkCantCancel();
                return true;
            }
            //X announcement: the menu's buttons ARE the X values; option
            //index == X value. maxX < 0 means the pool no longer covers
            //the base cost - no announceable X, let the caller fall back.
            int maxX = p->getManaPool()->getConvertedCost()
                       - menu->abilities[0]->source->getManaCost()->getConvertedCost();
            if (maxX < 0)
                return false;
            //#W63-AF (R1, wave-63 codex review finding 1). THE HARD CAP IS GONE.
            //`maxX > 50 ? 50 : maxX` withheld every affordable X above 50 while
            //announceXHeader told the pilot that "higher values are NOT offered
            //(they are unaffordable)" - a legal choice removed AND a false
            //reason printed for its absence, which is the doctrine breach twice
            //over. The engine's own menu carries a row per X (MTGRules.cpp
            //builds `options = pool - cost + 1`, plus the convoke credit and
            //the cost-reduction discount), so the bound that is REAL is the
            //affordable maximum, and the only other bound is the number of rows
            //the engine actually armed - an index past those lands on no
            //ability at all. Nothing else is clamped.
            const int engineRows = (int) menu->abilities.size();
            int shown = maxX;
            if (engineRows > 0 && shown > engineRows - 1)
                shown = engineRows - 1; //never offer an index the menu has no row for
            if (shown < 0)
                return false;
            for (int x = 0; x <= shown; x++)
            {
                std::ostringstream o;
                o << "X = " << x;
                req.optionTexts.push_back(o.str());
            }
            req.kind = DecisionRequest::ANNOUNCE_X;
            //#W62-Y (D5, deck130 HIGH-1): the CAST route's X announcement.
            //MTGRules.cpp arms it with setCustomMenuObject(card, false, selection),
            //so the engine's own "Cancel" row is on the menu and cantCancel is 0 -
            //clicking it leaves mClone NULL and MenuAbility::reactToChoiceClick
            //returns without announcing, i.e. the cast is backed out and the card
            //is NOT spent. `130 s21/s22` offered "Cast Starstorm {X pricing: your
            //mana affords only X=0 ... kills nothing}" and then a ONE-row X menu
            //headed "this ask has no pass row" - a menu that could only destroy
            //the card. The row exists in the engine; it was simply never carried
            //onto the decision contract.
            //#W62-AA (R1, wave-62 codex review finding 1): and it is carried
            //HERE too. The announcing branch above assigned canDecline; this
            //one - the ordinary cast route, which is the route deck130 s21/s22
            //actually took - set req.kind and returned, leaving canDecline at
            //its initialised false. The Decline row was therefore still absent
            //on exactly the path the finding names, and a max-affordable X of 0
            //still burned the card. Same engine fact, same call.
            req.canDecline = !object->checkCantCancel();
            return true;
        }

        for (size_t mk = 0; mk < menu->abilities.size(); mk++)
            req.optionTexts.push_back(menu->abilities[mk]->getMenuText());
        //#W55-D (D22): the rows of a `chooseaname` menu are AASetNameChosen
        //options and their menu text is a card NAME. Read off the engine's own
        //option objects, never guessed from the strings.
        req.nameChoiceMenu = (dynamic_cast<AASetNameChosen *>(menu->abilities[0]) != NULL);
        req.kind = DecisionRequest::CHOOSE_MODE;
        return true;
    }

    //Regular menu: items with GetId() > 0 map to action-layer abilities;
    //the trailing cancel item (when the menu is cancellable) is the decline.
    MTGAbility * soleOptionAbility = NULL;
    MTGAbility * firstOptionAbility = NULL;
    for (unsigned int k = 0; k < object->abilitiesMenu->mObjects.size(); k++)
    {
        //#W58-F (F1): resolve the row by identity. A row whose ability left the
        //game since the menu was armed is simply not offered - the surviving
        //rows stay legal choices, and applyMenuChoice's optionTexts gate makes
        //an answer built against the old row set drop for a fresh ask.
        int slot = 0;
        if (!object->getMenuControlId((int) k, slot))
            continue;
        if (slot <= 0)
            continue;
        MTGAbility * ab = (MTGAbility *) object->mObjects[slot];
        req.optionTexts.push_back(ab ? ab->getMenuText() : string("(option)"));
        req.menuIndices.push_back((int) k);
        soleOptionAbility = ab;
        if (!firstOptionAbility && ab)
            firstOptionAbility = ab;
    }
    if (req.optionTexts.empty())
        return false;
    //#W45 (wave-44 E-3): a menu armed while its spell RESOLVES (a modal
    //command's choose-one, already off the stack) has no currentActionCard,
    //so the consumer rendered the subject-less "A choice is required" header
    //and the reply's self-reference ("CHOICE: 6 (Silverquill Command)") had
    //no source to anchor to - INDEX-WINS read a valid answer as a stale echo
    //and fell back (deck146 vs126 seq 35). The option abilities still carry
    //their source: name it when the action card is gone.
    //#W47 (wave-46 E-1): the rescue used to fire only on a NULL pointer. An
    //`auto=choice` payload (Silverquill Command) arms its menu on a NAMELESS
    //carrier with no storedSourceCard, so nameableCardPointer handed the dummy
    //back and the rescue was skipped - 4 bare headers, 2 stale_echo fallbacks
    //(deck146 vs152 seq 13). Gate on the NAME being unresolvable, not the pointer.
    if (firstOptionAbility && !cardNamesItself(req.contextCard))
        if (MTGCardInstance * alt = nameableCardPointer(g, validatedCardPointer(g, firstOptionAbility->source)))
            if (cardNamesItself(alt))
                req.contextCard = alt;
    //Name the object of a single-option "may"-ability ask. Only when there is
    //exactly ONE real option (the Tergrid/steal shape - one may leg + an
    //implicit decline) so the recovered name cannot be attached to the wrong
    //leg of a multi-choice menu. The strings ride the request; the seat
    //renders them and leaves req.optionTexts (the staleness key) untouched.
    if (req.optionTexts.size() == 1)
        if (MayAbility * may = dynamic_cast<MayAbility *>(soleOptionAbility))
        {
            if (MTGCardInstance * obj = resolveTriggerObject(may))
            {
                req.mayObjectName = obj->getName();
                req.mayObjectOrigin = objectOrigin(obj, p);
            }
            //#W41-14: state the magnitude on the option. Representation only -
            //req.optionTexts (the staleness key) is untouched, so the answer
            //index and the apply path are unchanged.
            req.mayEffectLabel = mayEffectLabelFor(may);
        }
    req.canDecline = !object->checkCantCancel();
    req.kind = DecisionRequest::CHOOSE_MENU;
    return true;
}

void DecisionManager::applyMenuChoice(const DecisionRequest & req, const DecisionAction & act)
{
    GameObserver * g = req.player->getObserver();
    ActionLayer * object = g->mLayers->actionLayer();

    //Staleness gate: the menu must still be the one the request described.
    //A drifted menu (different kind or options) drops the answer - the
    //consumer's next poll gets a fresh request.
    DecisionRequest live;
    if (!buildMenuChoice(req.player, live))
        return;
    if (live.kind != req.kind || live.optionTexts != req.optionTexts)
        return;

    if (req.kind == DecisionRequest::CHOOSE_MODE || req.kind == DecisionRequest::ANNOUNCE_X)
    {
        //#W62-Y (D5): a declined X announcement clicks the menu's own Cancel
        //row (the last item; the same index the CHOOSE_MENU decline below
        //uses). Only where the request said the menu is cancellable - on a
        //can't-cancel menu the answer still has to land on a real option.
        if (act.choice < 0 && req.canDecline && req.kind == DecisionRequest::ANNOUNCE_X
            && object->abilitiesMenu && object->abilitiesMenu->mObjects.size())
        {
            object->doReactTo((int) object->abilitiesMenu->mObjects.size() - 1);
            return;
        }
        if (act.choice < 0 || act.choice >= (int) req.optionTexts.size())
            return;
        object->ButtonPressedOnMultipleChoice(act.choice);
        return;
    }

    //CHOOSE_MENU
    if (act.choice >= 0 && act.choice < (int) req.optionTexts.size())
    {
        object->doReactTo(req.menuIndices[act.choice]);
        return;
    }
    //decline: the last menu item is the cancel when cancellable; on a
    //can't-cancel menu the same key clicks the last real option (the
    //engine's own convention - an answer must always land)
    object->doReactTo((int) object->abilitiesMenu->mObjects.size() - 1);
}

bool DecisionManager::inspectMayBatch(Player * p, const DecisionRequest & req, MayBatch & out)
{
    out = MayBatch();
    if (!p || req.kind != DecisionRequest::CHOOSE_MENU)
        return false;
    //accept-or-decline shape only: one real option plus a legal decline. A
    //multi-option menu is not a mass yes/no question.
    if (req.optionTexts.size() != 1 || !req.canDecline)
        return false;

    GameObserver * g = p->getObserver();
    ActionLayer * object = g->mLayers->actionLayer();
    if (!object->menuObject || !object->abilitiesMenu || req.menuIndices.size() != 1)
        return false;
    int item = req.menuIndices[0];
    if (item < 0 || (size_t) item >= object->abilitiesMenu->mObjects.size())
        return false;
    int slot = 0;
    if (!object->getMenuControlId(item, slot) || slot <= 0) //#W58-F (F1)
        return false;
    if ((size_t) slot >= object->mObjects.size())
        return false;
    MayAbility * may = batchableMay((MTGAbility *) object->mObjects[slot]);
    if (!may)
        return false;
    MTGCardInstance * src = may->source;
    Player * controller = src->controller();
    if (controller != p)
        return false; //only the deciding seat's own repeated trigger
    //The group is the SOURCE CLASS, not the source instance: the measured
    //case (wave-40 126v139 t24) was TWO Perimeter Captains x FOUR blocking
    //defenders = eight asks, so keying on the instance pointer would split
    //the window into per-Captain groups and re-ask for each. Identity is
    //therefore: same printed card, and the same text the SEAT RENDERS - the
    //evaluated effect label when one exists (so two copies whose magnitudes
    //differ render differently and do NOT group), else the engine's own menu
    //label. Both halves must match; an unverifiable pair simply does not join.
    const std::string sourceClass = src->getName();
    const std::string effect = renderedMayEffect(may, req.optionTexts[0]);
    if (sourceClass.empty() || effect.empty())
        return false;

    //Every sibling still waiting on the stack. The engine pushes the whole
    //batch of triggers before resolving the first, so this count is the rest
    //of the window - not a guess about the future.
    ActionStack * stack = g->mLayers->stackLayer();
    for (size_t i = 0; i < stack->mObjects.size(); i++)
    {
        StackAbility * sa = dynamic_cast<StackAbility *>((Interruptible *) stack->mObjects[i]);
        if (!sa || sa->state != NOT_RESOLVED || !sa->ability)
            continue;
        //the trigger on the stack wraps the may (GenericTriggeredAbility ->
        //MayAbility); an unwrapped may on the stack counts too.
        MayAbility * sib = batchableMay(sa->ability);
        if (!sib)
        {
            NestedAbility * na = dynamic_cast<NestedAbility *>(sa->ability);
            sib = na ? batchableMay(na->ability) : NULL;
        }
        if (!sib || sib == may)
            continue;
        if (!sib->source || sib->source->getName() != sourceClass
            || sib->source->controller() != controller)
            continue;
        if (renderedMayEffect(sib, sib->getMenuText()) != effect)
            continue;
        out.pendingSiblings++;
    }

    out.totalInWindow = out.pendingSiblings + 1;
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //dev-only counting probe (WAGIC_MAYPROBE=1): what the batch inspector saw.
    //Compiled out of release builds, per the project's diagnostics convention.
    if (getenv("WAGIC_MAYPROBE"))
        fprintf(stderr, "MAYPROBE batch src=%s effect='%s' pending=%d stack=%d\n",
                sourceClass.c_str(), effect.c_str(), out.pendingSiblings,
                (int) stack->mObjects.size());
#endif //_DEBUG || WAGIC_DEVLOGS
    out.sourceName = sourceClass;
    //Key the group by source, effect and turn. Two separate windows on the
    //same turn merge only if the first is fully answered before the second
    //arms - and a verdict is consumed exactly totalInWindow times, so a
    //later window re-asks rather than inheriting a spent verdict.
    std::ostringstream k;
    k << sourceClass << "|" << effect << "|" << g->turn;
    out.groupKey = k.str();
    out.batchable = out.totalInWindow >= 2;
    return out.batchable;
}

bool DecisionManager::buildChooseTarget(Player * p, TargetChooser * tc, DecisionRequest & req)
{
    if (!tc || !tc->source)
        return false;
    GameObserver * g = p->getObserver();

    req.kind = DecisionRequest::CHOOSE_TARGET;
    req.player = p;
    req.sourceCard = tc->source;
    req.targetMin = tc->targetMin ? 1 : 0;
    req.maxTargets = tc->maxtargets;
    req.targetCandidates.clear();

    for (int i = 0; i < 2; i++)
    {
        Player * pl = g->players[i];
        if (tc->canTarget((Targetable *) pl) && !tc->alreadyHasTarget(pl))
            req.targetCandidates.push_back(pl);
        MTGPlayerCards * pz = pl->game;
        MTGGameZone * zones[] = { pz->hand, pz->library, pz->inPlay, pz->graveyard,
                                  pz->stack, pz->exile, pz->commandzone, pz->sideboard, pz->reveal };
        for (int j = 0; j < 9; j++)
        {
            MTGGameZone * zone = zones[j];
            for (int k = 0; k < zone->nb_cards && req.targetCandidates.size() < 40; k++)
            {
                MTGCardInstance * t = zone->cards[k];
                if (!tc->canTarget(t) || tc->alreadyHasTarget(t))
                    continue;
                req.targetCandidates.push_back(t);
            }
        }
    }
    return !req.targetCandidates.empty();
}

void DecisionManager::applyChooseTarget(const DecisionRequest & req, const DecisionAction & act,
                                        bool skipCardClick)
{
    GameObserver * g = req.player->getObserver();
    TargetChooser * tc = g->getCurrentTargetChooser();
    //the chooser must still be the one the request described; a closed or
    //replaced chooser drops the answer (the consumer re-asks on fresh state)
    if (!tc || tc->source != req.sourceCard)
        return;

    //re-validate: only offered, still-legal, not-yet-chosen, non-duplicate
    //picks survive
    std::vector<Targetable*> picks;
    for (size_t i = 0; i < act.targets.size(); i++)
    {
        Targetable * t = act.targets[i];
        if (!t || tc->alreadyHasTarget(t) || !tc->canTarget(t))
            continue;
        bool offered = false;
        for (size_t j = 0; !offered && j < req.targetCandidates.size(); j++)
            offered = req.targetCandidates[j] == t;
        bool dup = false;
        for (size_t j = 0; !dup && j < picks.size(); j++)
            dup = picks[j] == t;
        if (offered && !dup)
            picks.push_back(t);
    }
    if (picks.empty())
        return;

    //Single-target chooser: one click commits it. skipCardClick preserves
    //the historical chosenCard semantics (card already clicked upstream;
    //player targets still click).
    if (req.maxTargets == 1)
    {
        if (MTGCardInstance * card = dynamic_cast<MTGCardInstance *>(picks[0]))
        {
            if (!skipCardClick)
                g->cardClick(card);
        }
        else if (Player * pl = dynamic_cast<Player *>(picks[0]))
            g->cardClick(NULL, pl);
        return;
    }

    //Multi-target choreography (moved from AIPlayer::clickMultiTarget /
    //AIAction::clickMultiAct): the source's own confirming click first,
    //then player targets, then the card batch with done/autoChoice set on
    //the final click so the chooser closes. No shuffle: a contract
    //consumer's pick order is deliberate.
    std::vector<Targetable*>::iterator ite = picks.begin();
    while (ite != picks.end())
    {
        MTGCardInstance * card = dynamic_cast<MTGCardInstance *>(*ite);
        if (card && card == tc->source)
        {
            g->cardClick(card);
            ite = picks.erase(ite);
            continue;
        }
        if (Player * pl = dynamic_cast<Player *>(*ite))
        {
            g->cardClick(NULL, pl);
            ite = picks.erase(ite);
            continue;
        }
        ++ite;
    }
    if (picks.empty())
        return; //players/source only - the chooser fills through those clicks
    for (int k = 0; k < (int) picks.size() && k < tc->maxtargets; k++)
    {
        if (MTGCardInstance * card = dynamic_cast<MTGCardInstance *>(picks[k]))
        {
            if (k + 1 == (int) picks.size())
            {
                tc->done = true;
                tc->autoChoice = false;
            }
            g->cardClick(card);
        }
    }
    tc->attemptsToFill++;
}

bool DecisionManager::buildCastSpell(Player * p, ManaEngine::ManaPolicy & policy, ManaCost * pMana,
                                     bool instantSpeedOnly, DecisionRequest & req)
{
    req.kind = DecisionRequest::CAST_SPELL;
    req.player = p;
    req.casts = LegalActionsOracle::legalCasts(p, policy, pMana, instantSpeedOnly);
    return !req.casts.empty();
}

bool DecisionManager::planCastSpell(const DecisionRequest & req, const DecisionAction & act,
                                    ManaEngine::ManaPolicy & policy,
                                    std::vector<MTGAbility*> & producers)
{
    producers.clear();
    if (act.choice < 0 || act.choice >= (int) req.casts.size())
        return false;
    const LegalActionsOracle::Cast & pick = req.casts[act.choice];
    MTGCardInstance * card = pick.card;
    Player * p = req.player;

    //c5a plain-case fences: hand zone, normal cost, no extra costs / X /
    //kicker. Everything else keeps the caller's legacy commit path (the
    //fences retreat with c5b/c5c as those decisions join the contract).
    if (pick.viaAlternative || !pick.zoneLabel.empty())
        return false;
    if (!p->game->hand->hasCard(card))
        return false;
    ManaCost * cost = card->getManaCost();
    if (!cost || cost->extraCosts || cost->hasX() || cost->getKicker())
        return false;

    //staleness gate: the same card+variant must still be a legal cast
    ManaCost * pMana = ManaEngine::potentialMana(p, policy, card);
    if (!pMana)
        return false;
    pMana->add(p->getManaPool());
    DecisionRequest live;
    bool offered = false;
    if (buildCastSpell(p, policy, pMana, false, live))
        for (size_t i = 0; !offered && i < live.casts.size(); i++)
            offered = live.casts[i].card == card && !live.casts[i].viaAlternative;
    delete pMana;
    if (!offered)
        return false;

    int anytype = card->has(Constants::ANYTYPEOFMANA);
    if (!p->getManaPool()->canAfford(cost, anytype))
    {
        std::vector<MTGAbility*> plan = ManaEngine::planPayment(p, policy, card, cost, anytype);
        if (plan.empty())
            return false;
        //validate the whole plan - a partial payment would float mana for
        //nothing (only producer shapes the click layer knows how to drive)
        for (size_t k = 0; k < plan.size(); k++)
            if (!dynamic_cast<AManaProducer*>(plan[k]) && !dynamic_cast<GenericActivatedAbility*>(plan[k]))
                return false;
        producers = plan;
    }
    return true;
}
