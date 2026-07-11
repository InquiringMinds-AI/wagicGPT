#include "PrecompiledHeader.h"

#include "DecisionContract.h"
#include "ActionLayer.h"
#include "AllAbilities.h"
#include "CardDescriptor.h"
#include "GameObserver.h"
#include "MTGAbility.h"
#include "Player.h"

bool DecisionManager::buildDeclareAttackers(Player * p, DecisionRequest & req)
{
    req.kind = DecisionRequest::DECLARE_ATTACKERS;
    req.player = p;
    req.candidates.clear();
    CardDescriptor cd;
    cd.init();
    cd.setType("creature");
    MTGCardInstance * card = NULL;
    while ((card = cd.nextmatch(p->game->inPlay, card)))
        if (!card->isAttacker() && card->canAttack())
            req.candidates.push_back(card);
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
        if (!offered || card->isAttacker() || !card->canAttack())
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
        for (size_t m = object->mObjects.size() - 1; m > 0; m--)
        {
            MenuAbility * ability = dynamic_cast<MenuAbility *>(object->mObjects[m]);
            if (ability && ability->triggered)
                return ability;
        }
        return NULL;
    }
}

bool DecisionManager::buildMenuChoice(Player * p, DecisionRequest & req)
{
    GameObserver * g = p->getObserver();
    ActionLayer * object = g->mLayers->actionLayer();
    if (!object->menuObject || !object->abilitiesMenu)
        return false;

    req.player = p;
    req.contextCard = object->currentActionCard;
    req.optionTexts.clear();
    req.menuIndices.clear();
    req.canDecline = false;

    if (object->abilitiesMenu->isMultipleChoice && object->currentActionCard)
    {
        MenuAbility * menu = currentMenuAbility(object);
        if (!menu || menu->abilities.empty())
            return false;

        if (dynamic_cast<AAWhatsX *>(menu->abilities[0]))
        {
            //X announcement: the menu's buttons ARE the X values; option
            //index == X value. maxX < 0 means the pool no longer covers
            //the base cost - no announceable X, let the caller fall back.
            int maxX = p->getManaPool()->getConvertedCost()
                       - menu->abilities[0]->source->getManaCost()->getConvertedCost();
            if (maxX < 0)
                return false;
            int shown = maxX > 50 ? 50 : maxX; //bound the menu for degenerate pools
            for (int x = 0; x <= shown; x++)
            {
                std::ostringstream o;
                o << "X = " << x;
                req.optionTexts.push_back(o.str());
            }
            req.kind = DecisionRequest::ANNOUNCE_X;
            return true;
        }

        for (size_t mk = 0; mk < menu->abilities.size(); mk++)
            req.optionTexts.push_back(menu->abilities[mk]->getMenuText());
        req.kind = DecisionRequest::CHOOSE_MODE;
        return true;
    }

    //Regular menu: items with GetId() > 0 map to action-layer abilities;
    //the trailing cancel item (when the menu is cancellable) is the decline.
    for (unsigned int k = 0; k < object->abilitiesMenu->mObjects.size(); k++)
    {
        if (object->abilitiesMenu->mObjects[k]->GetId() <= 0)
            continue;
        MTGAbility * ab = (MTGAbility *) object->mObjects[object->abilitiesMenu->mObjects[k]->GetId()];
        req.optionTexts.push_back(ab ? ab->getMenuText() : string("(option)"));
        req.menuIndices.push_back((int) k);
    }
    if (req.optionTexts.empty())
        return false;
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
