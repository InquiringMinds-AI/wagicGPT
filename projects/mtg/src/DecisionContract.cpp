#include "PrecompiledHeader.h"

#include "DecisionContract.h"
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
