#include "PrecompiledHeader.h"

#include "AIPlayer.h"
#include "GameStateDuel.h"
#include "DeckManager.h"
#include "CardSelector.h"


// Instances for Factory
#include "AIPlayerBaka.h"

#ifdef WITH_GPT_AI
#include "AIPlayerGPT.h"
#endif


int AIPlayer::totalAIDecks = -1;

//#W56-B (D2): `playerAbilityTarget` was left INDETERMINATE here, so a plain
//card action could answer a `playerAbilityTarget ?` test with garbage.
AIAction::AIAction(AIPlayer * owner, MTGCardInstance * c, MTGCardInstance * t)
    : owner(owner), ability(NULL), player(NULL), click(c), target(t), playerAbilityTarget(NULL)
{
    bool prefetch = options[Options::CARDPREFETCHING].number?true:false;
    if (prefetch && WResourceManager::Instance()->IsThreaded())
    {
        // useability tweak - assume that the user is probably going to want to see the full res card,
        // so prefetch it. The idea is that we do it here as we want to start the prefetch before it's time to render,
        // and waiting for it to actually go into play is too late, as we start drawing the card during the interrupt window.
        // This is a good intercept point, as the AI has committed to using this card.

        // if we're not in text mode, always get the thumb
        if (owner->getObserver()->getCardSelector()->GetDrawMode() != DrawMode::kText)
        {
            //DebugTrace("Prefetching AI card going into play: " << c->getImageName());
            if(owner->getObserver()->getResourceManager())
                owner->getObserver()->getResourceManager()->RetrieveCard(c, RETRIEVE_THUMB);
        
            // also cache the large image if we're using kNormal mode
            if (owner->getObserver()->getCardSelector()->GetDrawMode() == DrawMode::kNormal)
            {
                if(owner->getObserver()->getResourceManager())
                    owner->getObserver()->getResourceManager()->RetrieveCard(c);
            }
        }
    }
}

//#W56-B (D2): see the header - one resolver, both consumers.
Player * AIAction::targetedSeat() const
{
    if (playerAbilityTarget)
        return dynamic_cast<Player *>(playerAbilityTarget);
    return player;
}

int AIAction::Act()
{
    GameObserver * g = owner->getObserver();
    if (player && !playerAbilityTarget)
    {
        g->cardClick(NULL, player);
        return 1;
    }
    if (ability)
    {
        int clickResult = g->cardClick(click, ability);
        if (target && !mAbilityTargets.size())
        {
            g->cardClick(target);
            return 1;
        }
        else if(playerAbilityTarget && !mAbilityTargets.size())
        {
            g->cardClick(NULL,(Player*)playerAbilityTarget);
            return 1;
        }
        if(mAbilityTargets.size())
        {
            return clickMultiAct(mAbilityTargets);
        }
        //bare ability click (payment plans ride these): surface whether the
        //engine actually accepted it, so the pump can abort a broken plan
        //instead of floating mana and dead-ending the cast
        return clickResult;
    }
    else  if(mAbilityTargets.size())
    {
        return clickMultiAct(mAbilityTargets);
    }
    else if (click)
    { //Shouldn't be used, really...
        g->cardClick(click, click);
        if (target)
            g->cardClick(target);
        return 1;
    }
    return 0;
}

int AIAction::clickMultiAct(vector<Targetable*>& actionTargets)
{
    GameObserver * g = owner->getObserver();
    TargetChooser * tc = g->getCurrentTargetChooser();
    if(!tc) return 0;
    vector<Targetable*>::iterator ite = actionTargets.begin();
    while(ite != actionTargets.end())
    {
        MTGCardInstance * card = ((MTGCardInstance *) (*ite));
        if(card == (MTGCardInstance*)tc->source)//click source first.
        {
            g->cardClick(card);
            ite = actionTargets.erase(ite);
            continue;
        }
        ++ite;
    }

    //shuffle to make it less predictable, otherwise ai will always seem to target from right to left. making it very obvious.
    owner->getRandomGenerator()->random_shuffle(actionTargets.begin(), actionTargets.end());

    for(int k = 0 ;k < int(actionTargets.size()) && k < tc->maxtargets; k++)
    {
        if (MTGCardInstance * card = dynamic_cast<MTGCardInstance *>(actionTargets[k]))
        {
            if(k+1 == int(actionTargets.size())){
                tc->done = true;
                tc->autoChoice = false;
            }
            g->cardClick(card);
        }
    }
    tc->attemptsToFill++;
    return 1;
}

AIPlayer::AIPlayer(GameObserver *observer, string file, string fileSmall, MTGDeck * deck) :
    Player(observer, file, fileSmall, deck)
{
    agressivity = 50;
    forceBestAbilityUse = false;
    playMode = Player::MODE_AI;
    mFastTimerMode = false;

}

//#W56-E: see include/AIPlayer.h.
RandomGenerator* AIPlayer::getRandomGenerator()
{
    return observer ? observer->getAIRandomGenerator() : &randomGenerator;
}

AIPlayer::~AIPlayer()
{
    while (!clickstream.empty())
    {
        AIAction * action = clickstream.front();
        SAFE_DELETE(action);
        clickstream.pop();
    }

}


int AIPlayer::Act(float)
{
    if (observer->currentPlayer == this)
        observer->userRequestNextGamePhase();
    return 1;
}



int AIPlayer::clickMultiTarget(TargetChooser * tc, vector<Targetable*>& potentialTargets)
{
    vector<Targetable*>::iterator ite = potentialTargets.begin();
    while(ite != potentialTargets.end())
    {
        MTGCardInstance * card = dynamic_cast<MTGCardInstance *>(*ite);
        if(card && card == tc->source)//if the source is part of the targetting deal with it first. second click is "confirming click".
        {
            clickstream.push(NEW AIAction(this, card));
            DebugTrace("Ai clicked source as a target: " << (card ? card->name : "None" ) << endl );
            ite = potentialTargets.erase(ite);
            continue;
        }
        else if(Player * pTarget = dynamic_cast<Player *>(*ite))
        {
            clickstream.push(NEW AIAction(this, pTarget));
            DebugTrace("Ai clicked Player as a target");
            ite = potentialTargets.erase(ite);
            continue;
        }
        ++ite;
    }

    getRandomGenerator()->random_shuffle(potentialTargets.begin(), potentialTargets.end()); //#W56-E
    if(potentialTargets.size())
        clickstream.push(NEW AIAction(this, NULL,tc->source,potentialTargets));
    while(clickstream.size())
    {
        AIAction * action = clickstream.front();
        action->Act();
        SAFE_DELETE(action);
        clickstream.pop();
    }
    return 1;
}

int AIPlayer::clickSingleTarget(TargetChooser *, vector<Targetable*>& potentialTargets, MTGCardInstance * chosenCard)
{
    int i = getRandomGenerator()->random() % potentialTargets.size(); //#W56-E

    if(MTGCardInstance * card = dynamic_cast<MTGCardInstance *>(potentialTargets[i]))
    {
        if (!chosenCard)
            clickstream.push(NEW AIAction(this, card));
    }
    else if(Player * player = dynamic_cast<Player *>(potentialTargets[i]))
    {
        clickstream.push(NEW AIAction(this, player));
    }

    return 1;
}


AIPlayer * AIPlayerFactory::createAIPlayer(GameObserver *observer, MTGAllCards * collection, Player * opponent, int deckid)
{
    string deckFile; //#W54-K (L22): was a char[512] sprintf of a profile-chosen path
    string avatarFilename = ""; // default imagename
    string deckFileSmall;
    
    if (deckid == GameStateDuel::MENUITEM_EVIL_TWIN)
    { //Evil twin
        deckFile = opponent->deckFile;
        DebugTrace("Evil Twin => " << opponent->deckFile);
        avatarFilename = "avatar.jpg";
        deckFileSmall = "ai_baka_eviltwin";
    }
    else
    {
        if (!deckid)
        {
            //random deck
            int nbdecks = MIN(AIPlayer::getTotalAIDecks(), options[Options::AIDECKS_UNLOCKED].number);
            if (!nbdecks)
                return NULL;
            deckid = 1 + std::rand() % (nbdecks); //#W56-E: WRand was a bare alias for rand()
        }
        char buf[64];
        sprintf(buf, "ai/baka/deck%i.txt", deckid);
        deckFile = buf;
        DeckMetaData *aiMeta = observer->getDeckManager()->getDeckMetaDataByFilename( deckFile, true);
        if(aiMeta)
            avatarFilename = aiMeta->getAvatarFilename();
        sprintf(buf, "ai_baka_deck%i", deckid);
        deckFileSmall = buf;
    }

    AIPlayer * ai = createAIPlayerFromDeckFile(observer, collection, opponent, deckFile, deckFileSmall, avatarFilename);
    if (ai)
        ai->deckId = deckid;
    return ai;
}

//The single construction point for every AI opponent: it decides AIPlayerGPT vs AIPlayerBaka,
//so any caller routed through here honours the player's configured AI.
AIPlayer * AIPlayerFactory::createAIPlayerFromDeckFile(GameObserver *observer, MTGAllCards * collection, Player * opponent,
                                                      const string& deckFile, const string& deckFileSmall,
                                                      const string& avatarFilename)
{
    int deckSetting = EASY;
    if ( opponent )
    {
        bool isOpponentAI = opponent->isAI() == 1;
        DeckMetaData *meta = observer->getDeckManager()->getDeckMetaDataByFilename( opponent->deckFile, isOpponentAI);
        if ( meta && meta->getVictoryPercentage() >= 65)
            deckSetting = HARD;
    }

    // AIPlayerBaka will delete MTGDeck when it's time
    AIPlayerBaka * baka = NULL;
#ifdef WITH_GPT_AI
    if (AIPlayerGPT::isEnabled())
        baka = NEW AIPlayerGPT(observer, deckFile, deckFileSmall, avatarFilename, NEW MTGDeck(deckFile, collection,0, deckSetting));
#endif
    if (!baka)
        baka = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, avatarFilename, NEW MTGDeck(deckFile, collection,0, deckSetting));
    baka->comboHint = NULL;
    if (baka->opponent() && baka->opponent()->isHuman())
        baka->setFastTimerMode(false);
    return baka;
}

int AIPlayer::receiveEvent(WEvent *)
{
    return 0;
}

void AIPlayer::Render()
{

}

bool AIPlayer::parseLine(const string& s)
{
    size_t limiter = s.find("=");
    if (limiter == string::npos) limiter = s.find(":");
    string areaS;
    if (limiter != string::npos)
    {
        areaS = s.substr(0, limiter);
        if (areaS.compare("rvalues") == 0)
        {
            getRandomGenerator()->loadRandValues(s.substr(limiter + 1)); //#W56-E
            return true;
        }
    }

    return Player::parseLine(s);
}


int AIPlayer::getTotalAIDecks() 
{
    if (totalAIDecks == -1)
        totalAIDecks = countTotalDecks(0,0,1);
    return totalAIDecks;
}

int AIPlayer::countTotalDecks(int lower, int higher, int current) {

    char buffer[512];
    sprintf(buffer, "ai/baka/deck%i.txt", current);
    if (JFileSystem::GetInstance()->FileExists(buffer))
    {
        if (higher == current + 1)
            return current;
        int newlower = current;
        int newcurrent = higher ? (higher + newlower)/2 : current * 2;
        return countTotalDecks(newlower, higher, newcurrent);
    } else {
        if (!lower)
            return 0;
        if (lower == current - 1)
            return lower;

        int newhigher = current;
        int newcurrent = (lower + newhigher) / 2;
        return countTotalDecks(lower, newhigher, newcurrent);
    }
}

void AIPlayer::invalidateTotalAIDecks() 
{
    totalAIDecks = -1;
}

