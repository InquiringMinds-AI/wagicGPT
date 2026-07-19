#include "PrecompiledHeader.h"

#include "TestSuiteAI.h"
#include "MTGAbility.h"
#include "AllAbilities.h"
#include "ExtraCost.h"
#include "ManaEngine.h"
#include "MTGRules.h"
#include "ActionLayer.h"
#include "GuiCombat.h"
#include "Rules.h"
#include "GameObserver.h"
#include "GameStateShop.h"
#include "MTGDeck.h"
#include "AIPlayerBaka.h"
#ifdef QT_CONFIG
#include <QThread>
#endif

using std::string;

#ifdef TESTSUITE

// number of frames between 2 actions.
// SLOW_TEST defines the speed of the very first test. It is intentionally slow so that when you debug one specific test, you get to see what happens on the screen
// FAST_TEST is for other tests, any value below 3 is known to have bad side effects.
// Higher values == slower. Lower values == faster
#define SLOW_TEST 40
#define FAST_TEST 6

// NULL is sent in place of a MTGDeck since there is no way to create a MTGDeck without a proper deck file.
// TestSuiteAI will be responsible for managing its own deck state.
TestSuiteAI::TestSuiteAI(TestSuiteGame *tsGame, int playerId) :
    AIPlayerBaka(tsGame->observer, "testsuite", "testsuite", "baka.jpg", NULL)
{
    SAFE_DELETE(game); //game might have been set in the parent with default values
    game = tsGame->buildDeck(this, playerId);
    game->setOwner(this);

    suite = tsGame;
    timer = 0;
    aiActCounter = 1.0f;
    mAssertPhaseArmed = false;
    expectedTappedInPlay = -1;
    playMode = MODE_TEST_SUITE;
    this->deckName = "Test Suite AI";
    this->comboHint = NULL;
}

bool TestSuiteAI::parseLine(const string& s)
{
    //test-suite-only assertion keys
    static const string kTapped = "tappedinplay:";
    if (s.compare(0, kTapped.size(), kTapped) == 0)
    {
        expectedTappedInPlay = atoi(s.c_str() + kTapped.size());
        return true;
    }
    static const string kNecroed = "necroed:";
    if (s.compare(0, kNecroed.size(), kNecroed) == 0)
    {
        expectedNecroed.clear();
        string names = s.substr(kNecroed.size());
        while (names.size())
        {
            size_t comma = names.find(",");
            string name = comma == string::npos ? names : names.substr(0, comma);
            expectedNecroed.push_back(trim(name));
            names = comma == string::npos ? "" : names.substr(comma + 1);
        }
        return true;
    }
    static const string kAbility = "ability:";
    if (s.compare(0, kAbility.size(), kAbility) == 0)
    {
        expectedBasicAbilities.push_back(s.substr(kAbility.size()));
        return true;
    }
    static const string kCardText = "cardtext:";
    if (s.compare(0, kCardText.size(), kCardText) == 0)
    {
        expectedCardText.push_back(s.substr(kCardText.size()));
        return true;
    }
    return AIPlayerBaka::parseLine(s);
}

MTGCardInstance * TestSuiteAI::getCard(string action)
{
    int mtgid = Rules::getMTGId(action);
    if (mtgid)
    {
        MTGCardInstance * byId = Rules::getCardByMTGId(observer, mtgid);
        if (byId)
            return byId;
        //Fall through to the name scan: a token NAME can resolve to the
        //token model registered in the collection, whose id no live
        //instance carries (each token instance has its own generated id).
    }

    //This mostly handles tokens
    std::transform(action.begin(), action.end(), action.begin(), ::tolower);
    for (int i = 0; i < 2; i++)
    {
        Player * p = observer->players[i];
        //reveal included so tests can click cards revealed by effects like
        //Alrund's end-step type choice
        MTGGameZone * zones[] = { p->game->library, p->game->hand, p->game->inPlay, p->game->graveyard, p->game->commandzone, p->game->sideboard, p->game->removedFromGame, p->game->reveal };
        for (int j = 0; j < 8; j++)
        {
            MTGGameZone * zone = zones[j];
            for (int k = 0; k < zone->nb_cards; k++)
            {
                MTGCardInstance * card = zone->cards[k];
                if (!card) return NULL;
                string name = card->getLCName();
                if (name.compare(action) == 0) return card;
            }
        }
    }
    DebugTrace("TESTUISTEAI: Can't find card:" << action.c_str());
    //Dump what IS there: a failed lookup is usually a typo'd name or a
    //zone that never got populated, and seeing the actual board answers
    //both in one read.
    for (int i = 0; i < 2; i++)
    {
        Player * p = observer->players[i];
        MTGGameZone * zones[] = { p->game->library, p->game->hand, p->game->inPlay, p->game->graveyard, p->game->commandzone, p->game->sideboard, p->game->removedFromGame, p->game->reveal };
        const char * zoneNames[] = { "library", "hand", "inPlay", "graveyard", "command", "sideboard", "exile", "reveal" };
        for (int j = 0; j < 8; j++)
            for (int k = 0; k < zones[j]->nb_cards; k++)
                if (zones[j]->cards[k])
                    DebugTrace("TESTUISTEAI: p" << i << " " << zoneNames[j] << "[" << k << "] '" << zones[j]->cards[k]->getLCName() << "' mtgid=" << zones[j]->cards[k]->getMTGId());
    }
    return NULL;
}

Interruptible * TestSuiteGame::getActionByMTGId(int mtgid)
{
    ActionStack * as = observer->mLayers->stackLayer();
    Interruptible * action = NULL;
    while ((action = as->getNext(action, 0, 0, 1)))
    {
        if (action->source && action->source->getMTGId() == mtgid)
        {
            return action;
        }
    }
    return NULL;
}

int TestSuiteAI::displayStack()
{
    if (playMode == MODE_AI) return 0;
    return 1;
}

int TestSuiteAI::Act(float)
{
    if (observer->didWin())
    {
        if (!observer->didWin(observer->players[0]))
            suite->observedGameOver = 1;
        else if (!observer->didWin(observer->players[1]))
            suite->observedGameOver = 2;
    }
    observer->setLoser(NULL); // Prevent draw rule from losing the game

    //Last bits of initialization require to be done here, after the first "update" call of the game

    if (suite->currentAction == 0)
    {
        for (int i = 0; i < 2; ++ i)
            observer->players[i]->getManaPool()->copy(suite->initState.players[i]->getManaPool());
    }

    //[AI] tests: once the game has LEFT the [ASSERT] phase and come back to
    //it, freeze the AI there and let the script's end-of-test action assert.
    //The fixtures' AICALLS numbers were hand-tuned to the OLD priority
    //model's calls-per-phase-boundary economics (every boundary rode the
    //stack and cost interrupt-answer calls); with engine-owned priority the
    //same budget carries the game PAST the asserted phase. The declared
    //assert phase is the fixture's intent - stop there; AICALLS stays a cap
    //on total AI work, not a pacing constant.
    if (playMode == MODE_AI && observer->getCurrentGamePhase() != suite->endState.phase)
        mAssertPhaseArmed = true;
    bool atAssertPhase = mAssertPhaseArmed && observer->getCurrentGamePhase() == suite->endState.phase;

    if (playMode == MODE_AI && suite->aiMaxCalls && !atAssertPhase)
    {
        //Per-instance, not function-local static: the static was shared by
        //every worker thread (unsynchronized writes) and grew across all
        //[AI] tests, so AIPlayerBaka's dt-paced timer saw schedule-dependent
        //values. Per-instance matches what a solo run sees.
        suite->aiMaxCalls--;
        suite->timerLimit = SLOW_TEST; //TODO Remove this limitation when AI is not using a stupid timer anymore...
        AIPlayerBaka::Act(aiActCounter++);//dt);
    }
    if (playMode == MODE_HUMAN)
    {
        observer->mLayers->CheckUserInput(0);
        suite->currentAction++; //hack to avoid repeating the initialization of manapool
        return 1;
    }

    timer += 1;
    if (timer < suite->timerLimit) return 1;
    timer = 0;

    string action = suite->getNextAction();
//    observer->mLayers->stackLayer()->Dump();
    //The [filename] tag makes one test's commands extractable from the
    //braided multi-worker suite log (grep "\[<testfile>\]").
    DebugTrace("TESTSUITE command: " << action << " [" << suite->filename << "]");

    //A pending decision menu whose answer the script does not provide:
    //under engine-owned priority a pending menu HOLDS the game (correctly -
    //its controller is deciding), so a script that ignores it would now
    //DEADLOCK where it used to dangle harmlessly. Mirror the interrupt-ask
    //convention below: if the next command is not a menu answer, apply the
    //suite default - decline when the menu is cancelable (Cancel is the
    //last item), take the first option when the choice is mandatory - and
    //re-queue the command.
    {
        ActionLayer * al = observer->mLayers->actionLayer();
        if (al->menuObject && al->abilitiesMenu && al->abilitiesMenu->mObjects.size()
            && action.find("choice ") == string::npos
            //cancelcost declines the OPEN pay-or-decline menu itself (choosing
            //the unpaid branch) - it must reach the menu, not be pre-answered
            //by the suite default (which would commit to the paid branch).
            && action.compare("cancelcost") != 0)
        {
            //Mana abilities pierce menus in the engine (a pending X-payment
            //may need mana floated while its menu waits - flameblast_dragon).
            //Mirror that rule: a command that resolves to an in-play card
            //with a mana producer falls through to the normal click path
            //instead of triggering the default.
            bool pierce = false;
            //Keyword commands can never be mana clicks - skip the card
            //lookup (its miss path dumps the whole board to the log).
            bool keyword = action == "" || action == "next" || action == "eot" || action == "yes"
                || action == "no" || action == "human" || action == "ai" || action == "endinterruption"
                || action.find("goto") != string::npos || action.find("reveal") != string::npos
                || action.find("p1") != string::npos || action.find("p2") != string::npos;
            MTGCardInstance * manaCard = keyword ? NULL : getCard(action);
            if (manaCard && manaCard->controller()
                && manaCard->controller()->game->inPlay->hasCard(manaCard) && !manaCard->isTapped())
            {
                for (size_t mi = 0; mi < al->manaObjects.size(); mi++)
                {
                    AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility *)al->manaObjects[mi]);
                    if (amp && amp->source == manaCard)
                    {
                        DebugTrace("TESTSUITE menu pierce: mana producer '" << action
                                   << "' [" << suite->filename << "]");
                        pierce = true;
                        break;
                    }
                }
            }
            if (!pierce)
            {
                int last = (int)al->abilitiesMenu->mObjects.size() - 1;
                bool cancelable = al->abilitiesMenu->mObjects[last]->GetId() == kCancelMenuID;
                DebugTrace("TESTSUITE menu default: " << (cancelable ? "cancel" : "first option")
                           << " pending='" << action << "' [" << suite->filename << "]");
                al->doReactTo(cancelable ? last : 0);
                suite->currentAction--;
                return 1;
            }
        }
    }

    if (observer->mLayers->stackLayer()->askIfWishesToInterrupt == this)
    {
        DebugTrace("TESTSUITE interrupt-ask for player " << ((this == observer->players[0]) ? 0 : 1) << " pending='" << action << "' latest=" << observer->mLayers->stackLayer()->getLatest(NOT_RESOLVED) << " [" << suite->filename << "]");
        if (action.compare("no") != 0 && action.compare("yes") != 0)
        {
            observer->mLayers->stackLayer()->cancelInterruptOffer();
            suite->currentAction--;
            return 1;
        }
    }

    if (action == "")
    {
        //end of game
        suite->assertGame();
        observer->setLoser(observer->players[0]);
        DebugTrace("================================    END OF TEST   =======================\n");
        return 1;
    }

    if (action.compare("eot") == 0)
    {
        if (observer->getCurrentGamePhase() != MTG_PHASE_CLEANUP) suite->currentAction--;
        observer->userRequestNextGamePhase();
    }
    else if (action.compare("human") == 0)
    {
        DebugTrace("TESTSUITE You have control");
        playMode = MODE_HUMAN;
        return 1;
    }
    else if (action.compare("ai") == 0)
    {
        DebugTrace("TESTSUITE Switching to AI");
        //BOTH seats go AI. Under the engine-owned priority model the
        //non-acting seat legitimately receives update ticks (e.g. the
        //defender when combat flips to BLOCKERS); if that seat stayed a
        //script-reader it would run the script off its end mid-combat and
        //assert at the wrong phase. An [AI] test hands the WHOLE game to
        //the AI - the shared AICALLS budget already caps total AI work.
        //(The old priority model masked this: its blanket AI-vs-AI stack
        //windows kept isInterrupting latched on the acting player, so the
        //other seat never got a tick.)
        observer->players[0]->playMode = MODE_AI;
        observer->players[1]->playMode = MODE_AI;
        return 1;
    }
    else if (action.compare("next") == 0 || action.find("goto") != string::npos)
    {
        if(action.find("goto ")!= string::npos)
        {
            size_t found = action.find("goto ");
            string phase = action.substr(found + 5);
            int phaseToGo = 0;
            for (int i = 0; i < NB_MTG_PHASES; i++)
            {
                if (phase.find(Constants::MTGPhaseCodeNames[i]) != string::npos)
                {
                    phaseToGo = i;
                }
            }
            if(observer->getCurrentGamePhase() != phaseToGo)
                suite->currentAction--;
                else
                {
                return 1;
                }
            GuiCombat * gc = observer->mLayers->combatLayer();
            if (ORDER == observer->combatStep || DAMAGE == observer->combatStep)
            {
                gc->clickOK();
            }
            else
            {
                observer->userRequestNextGamePhase();
            }
        }
        else
        {
            GuiCombat * gc = observer->mLayers->combatLayer();
            if (ORDER == observer->combatStep || DAMAGE == observer->combatStep)
                gc->clickOK();
            else
                observer->userRequestNextGamePhase();
        }
    }
    else if (action.compare("yes") == 0)
        observer->mLayers->stackLayer()->setIsInterrupting(this);
    else if (action.compare("endinterruption") == 0)
        observer->mLayers->stackLayer()->endOfInterruption();
    else if (action.compare("no") == 0)
    {
        if (observer->mLayers->stackLayer()->askIfWishesToInterrupt == this)
            observer->mLayers->stackLayer()->cancelInterruptOffer();
    }
    else if (action.find("revealok") != string::npos || action.find("revealnext") != string::npos)
    {
        //Drive an open reveal display (MTGRevealingCards). The engine's AI
        //auto-key is disabled under WAGIC_TESTSUITE, so scripts advance
        //reveals explicitly: revealok = confirm/advance (builds option two
        //when the first ability is done), revealnext = done-with-selection.
        DebugTrace("TESTSUITE reveal key");
        ActionLayer * al = observer->mLayers->actionLayer();
        MTGRevealingCards * pick = NULL;
        for (size_t i = 0; i < al->mObjects.size(); i++)
        {
            if (!al->mObjects[i]) continue;
            MTGRevealingCards * rev = dynamic_cast<MTGRevealingCards*>((ActionElement*)al->mObjects[i]);
            if (!rev) continue;
            if (!pick) pick = rev;
            //several reveals can coexist on a shared zone (parley); the key
            //must go to the one whose display is actually open
            if (observer->OpenedDisplay && rev->revealDisplay == observer->OpenedDisplay)
            {
                pick = rev;
                break;
            }
        }
        if (pick)
            pick->CheckUserInput(action.find("revealnext") != string::npos ? JGE_BTN_NEXT : JGE_BTN_OK);
    }
    else if (action.find("choice ") != string::npos)
    {
        DebugTrace("TESTSUITE choice !!!");
        int choice = atoi(action.substr(action.find("choice ") + 7).c_str());
        observer->mLayers->actionLayer()->doReactTo(choice);
    }
    else if (action.compare("cancelcost") == 0)
    {
        //Decline a pay-or-decline prompt (ward, kicker-style may-costs, etc.).
        //The reliable, rules-correct decline is to choose the UNPAID branch of
        //the still-open menu - the human's "don't pay" click - rather than
        //committing to pay and then backing out. The unpaid branch is the last
        //menu item (a cancel item when the menu is cancelable, the fizzle/
        //counter option when the paidability is mandatory). If the pay was
        //already committed and the mExtraPayment is armed (no open menu), route
        //to MenuAbility::declineExtraPayment(), the engine's own unpaid-branch
        //release used by the auto-release path.
        ActionLayer * al = observer->mLayers->actionLayer();
        if (al->menuObject && al->abilitiesMenu && al->abilitiesMenu->mObjects.size())
        {
            int last = (int)al->abilitiesMenu->mObjects.size() - 1;
            al->doReactTo(last);
            DebugTrace("TESTSUITE cancelcost: declined open pay menu (option " << last << ") [" << suite->filename << "]");
        }
        else if (observer->mExtraPayment)
        {
            if (MenuAbility * ma = dynamic_cast<MenuAbility*>(observer->mExtraPayment->action))
                ma->declineExtraPayment();
            else
            {
                observer->mLayers->actionLayer()->CheckUserInput(JGE_BTN_SEC);
                observer->mExtraPayment = NULL;
            }
            DebugTrace("TESTSUITE cancelcost: declined armed extra payment [" << suite->filename << "]");
        }
        else
        {
            std::cerr << "TESTSUITE cancelcost: no pending pay prompt (state unchanged)" << std::endl;
            DebugTrace("TESTSUITE cancelcost: no pending pay prompt [" << suite->filename << "]");
        }
    }
    else if (action.compare("paycost") == 0)
    {
        //Complete a pending mExtraPayment (the pay leg of a pay-or-decline
        //prompt). The scripted seat has no producers to click, so this mirrors
        //the human casting-payment path.
        ExtraCosts * ep = observer->mExtraPayment;
        if (!ep)
        {
            std::cerr << "TESTSUITE paycost: no pending extra payment (state unchanged)" << std::endl;
            DebugTrace("TESTSUITE paycost: no pending extra payment [" << suite->filename << "]");
            return 1;
        }
        {
            //The human completes an extra payment by tapping mana producers
            //until the pool covers the cost, at which point the engine's own
            //MenuAbility::Update / paidability auto-pays from the pool and
            //resolves the paid branch. The scripted seat has no producers to
            //click, so mirror the human casting-payment path exactly:
            //ManaEngine::autoTapForCost taps the paying player's free
            //untapped producers to cover the extra mana cost, then the engine
            //finishes the payment on its own next tick.
            MTGCardInstance * src = ep->source;
            Player * payer = src ? src->controller() : NULL;
            ManaCost * leg = NULL;
            for (size_t ec = 0; !leg && ec < ep->costs.size(); ec++)
                if (ExtraManaCost * emc = dynamic_cast<ExtraManaCost *>(ep->costs[ec]))
                    leg = emc->costToPay;
            int anytype = src ? src->has(Constants::ANYTYPEOFMANAABILITY) : 0;
            if (payer && leg && !ep->isPaymentSet())
                ManaEngine::autoTapForCost(payer, src, leg, anytype);
            if (ep->isPaymentSet() && ep->canPay())
            {
                //covered - the engine's MenuAbility::Update will consume the
                //pool and fire the paid branch on the following tick, same as
                //for a human who just tapped enough mana.
                DebugTrace("TESTSUITE paycost: extra cost covered, engine will resolve paid branch [" << suite->filename << "]");
            }
            else
            {
                std::cerr << "TESTSUITE paycost: paying player cannot cover the extra cost"
                             " (state unchanged) [" << suite->filename << "]" << std::endl;
                DebugTrace("TESTSUITE paycost: unpayable, state unchanged [" << suite->filename << "]");
            }
        }
    }
    else if (action.find(" -momir- ") != string::npos)
    {
        int start = action.find(" -momir- ");
        int cardId = Rules::getMTGId(action.substr(start + 9).c_str());
        int cardIdHand = Rules::getMTGId(action.substr(0, start).c_str());
        MTGMomirRule * a = ((MTGMomirRule *) observer->mLayers->actionLayer()->getAbility(MTGAbility::MOMIR));
        a->reactToClick(Rules::getCardByMTGId(observer, cardIdHand), cardId);
        observer->mLayers->actionLayer()->stuffHappened = 1;
    }
    else if (action.find("p1") != string::npos || action.find("p2") != string::npos)
    {
        Player * p = observer->players[1];
        size_t start = action.find("p1");
        if (start != string::npos) p = observer->players[0];
        observer->cardClick(NULL, p);
    }
    else
    {
        int mtgid = Rules::getMTGId(action);
        Interruptible * toInterrupt = NULL;
        if (mtgid)
        {
            DebugTrace("TESTSUITE CARD ID:" << mtgid);
            toInterrupt = suite->getActionByMTGId(mtgid);
        }

        if (toInterrupt)
        {
            observer->stackObjectClicked(toInterrupt);
            return 1;
        }

        MTGCardInstance * card = getCard(action);
        if (card)
        {
            DebugTrace("TESTSUITE Clicking ON: " << card->name << " [" << suite->filename << "]");
            //The "mimic library shuffle" emulates paper's shuffle-after-
            //search for clicks that resolve to library cards. But it runs
            //after EVERY pick - and while a POSITION-restricted chooser
            //(zpos, e.g. Collected Conjuring's top-six) is collecting,
            //shuffling scrambles the very positions the chooser's
            //predicate reads, randomizing the test. Suppress it for
            //zpos-restricted choosers; name/type searches keep it.
            bool zposPick = false;
            if (DescriptorTargetChooser * dtc = dynamic_cast<DescriptorTargetChooser *>(observer->getCurrentTargetChooser()))
                zposPick = dtc->cd && dtc->cd->zposComparisonMode;
            if (!zposPick)
                card->currentZone->needShuffle = true; //mimic library shuffle
            observer->cardClick(card, card);
            if (!zposPick)
                observer->forceShuffleLibraries(); //mimic library shuffle
            return 1;
        }
        //A scripted click whose card was not found is consumed and does
        //NOTHING - the prime shape of a silent test divergence. Say so.
        if (action.size())
            DebugTrace("TESTSUITE SWALLOWED (no card matched): " << action << " [" << suite->filename << "]");
    }
    return 0;
}

TestSuiteActions::TestSuiteActions()
{
    nbitems = 0;
}

void TestSuiteActions::add(string s)
{
    actions[nbitems] = s;
    nbitems++;
}


TestSuiteState::TestSuiteState()
{
    phase = MTG_PHASE_INVALID;
    expectedGameOver = -1;
    players.clear();
}

TestSuiteState::~TestSuiteState() 
{
    if(players.size())
    {
        if(players[0])
            SAFE_DELETE(players[0]);
        if(players[1])
            SAFE_DELETE(players[1]);
    }
};

void TestSuiteState::parsePlayerState(int playerId, string s)
{
    // MTGPlayerCards::parseLine intentionally only covers the ordinary duel
    // zones. Test state also needs the hidden deck-construction zones so a
    // target(sideboard|...) or target(commandzone|...) fixture can seed and
    // assert a real card movement without changing production parsing.
    size_t limiter = s.find("=");
    if (limiter == string::npos) limiter = s.find(":");
    if (limiter != string::npos)
    {
        string areaS = s.substr(0, limiter);
        MTGGameZone * zone = NULL;
        if (areaS.compare("sideboard") == 0)
            zone = players[playerId]->game->sideboard;
        else if (areaS.compare("commandzone") == 0)
            zone = players[playerId]->game->commandzone;
        else if (areaS.compare("temp") == 0)
            zone = players[playerId]->game->temp;
        if (zone)
        {
            zone->parseLine(s.substr(limiter + 1));
            return;
        }
    }
    if (!players[playerId]->parseLine(s))
    {
        //A typo'd key (e.g. "inhand:" for "hand:") used to vanish without a
        //trace, leaving the expected state silently empty and the test
        //vacuously green. Surface it in the results instead.
        std::cerr << "TESTSUITE: unparsed player-state line (typo'd key?): " << s << std::endl;
        DebugTrace("TESTSUITE: unparsed player-state line: " << s);
    }
}


string TestSuiteGame::getNextAction()
{
    currentAction++;
    if (actions.nbitems && currentAction <= actions.nbitems)
    {
        return actions.actions[currentAction - 1];
    }
    return "";
}

void TestSuiteGame::handleResults(bool wasAI, int error)
{
    testsuite->handleResults(wasAI, error);
};


void TestSuite::initGame(GameObserver* g)
{
    observer = g;
    //main-thread suite path: the observer is GameStateDuel's, not the
    //TestSuiteGame constructor's - flag it here too so the ASPHASES
    //auto-skips stay off for scripted cadences
    g->mSuiteGame = true;
    //The first test runs slowly, the other ones run faster.
    //This way a human can see what happens when testing a specific file,
    // or go faster when it comes to the whole test suite.
    //Warning, putting this value too low (< 3) will give unexpected results
    if (!timerLimit)
    {
        timerLimit = SLOW_TEST;
    }
    else
    {
        timerLimit = FAST_TEST;
    }

    TestSuiteGame::initGame();
}

int TestSuiteGame::Log(const char * text)
{
    ofstream file;
    if (JFileSystem::GetInstance()->openForWrite(file, "test/results.html", ios_base::app))
    {
        file << text;
        file << "\n";
        file.close();
    }

    DebugTrace(text);
    return 1;

}

void TestSuiteGame::assertGame()
{
    mAsserted = true;
    mMutex.lock();
    //compare the game state with the results
    char result[4096];
    sprintf(result, "<h3>%s</h3>", filename.c_str());
    Log(result);

    int error = 0;
    bool wasAI = false;

    if (endState.expectedGameOver >= 0 && observedGameOver != endState.expectedGameOver)
    {
        sprintf(result, "<span class=\"error\">==game-over problem. Expected %i, got %i==</span><br />",
            endState.expectedGameOver, observedGameOver);
        Log(result);
        error++;
    }

    if (observer->getCurrentGamePhase() != endState.phase)
    {
        //A garbage/uninitialized expected phase means the test file's
        //[ASSERT] section was missing or never parsed - report it instead
        //of indexing MTGPhaseNames out of bounds (used to crash).
        const char * expectedName = (endState.phase >= 0 && endState.phase < NB_MTG_PHASES)
            ? Constants::MTGPhaseNames[endState.phase].c_str()
            : "INVALID - missing/unparsed [ASSERT] section?";
        sprintf(result, "<span class=\"error\">==phase problem. Expected [ %s ](%i), got [ %s ](%i)==</span><br />",
            expectedName, endState.phase,
            Constants::MTGPhaseNames[observer->getCurrentGamePhase()].c_str(), observer->getCurrentGamePhase());
        Log(result);
        error++;
    }
    for (int i = 0; i < 2; i++)
    {
        TestSuiteAI * p = (TestSuiteAI *) (observer->players[i]);
        if (p->playMode == Player::MODE_AI) wasAI = true;

        if (p->life != endState.players[i]->life)
        {
            sprintf(result, "<span class=\"error\">==life problem for player %i. Expected %i, got %i==</span><br />", i,
                            endState.players[i]->life, p->life);
            Log(result);
            error++;
        }
        if (p->poisonCount != endState.players[i]->poisonCount)
        {
            sprintf(result, "<span class=\"error\">==poison counter problem for player %i. Expected %i, got %i==</span><br />", i,
                            endState.players[i]->poisonCount, p->poisonCount);
            Log(result);
            error++;
        }
        if (endState.players[i]->expectedTappedInPlay >= 0)
        {
            int tapped = 0;
            for (int k = 0; k < p->game->inPlay->nb_cards; k++)
                if (p->game->inPlay->cards[k]->isTapped())
                    tapped++;
            if (tapped != endState.players[i]->expectedTappedInPlay)
            {
                sprintf(result, "<span class=\"error\">==tapped battlefield cards problem for player %i. Expected %i, got %i==</span><br />", i,
                                endState.players[i]->expectedTappedInPlay, tapped);
                Log(result);
                error++;
            }
        }        if (!p->getManaPool()->canAfford(endState.players[i]->getManaPool(),0))
        {
            sprintf(result, "<span class=\"error\">==Mana problem. Was expecting %i but got %i for player %i==</span><br />",
                            endState.players[i]->getManaPool()->getConvertedCost(), p->getManaPool()->getConvertedCost(), i);
            Log(result);
            error++;
        }
        if (!endState.players[i]->getManaPool()->canAfford(p->getManaPool(),0))
        {
            sprintf(result, "<span class=\"error\">==Mana problem. Was expecting %i but got %i for player %i==</span><br />",
                            endState.players[i]->getManaPool()->getConvertedCost(), p->getManaPool()->getConvertedCost(), i);
            Log(result);

            if ( endState.players[i]->getManaPool()->getConvertedCost() == p->getManaPool()->getConvertedCost())
            {
                sprintf(result, "<span class=\"error\">====(Apparently Mana Color issues since converted cost is the same)==</span><br />");
                Log(result);
            }
            error++;

        }
        MTGGameZone * playerZones[] = { p->game->graveyard, p->game->library, p->game->hand, p->game->inPlay, p->game->commandzone, p->game->sideboard, p->game->removedFromGame, p->game->temp };
        MTGGameZone * endstateZones[] = { endState.players[i]->game->graveyard,
                                         endState.players[i]->game->library,
                                         endState.players[i]->game->hand,
                                         endState.players[i]->game->inPlay,
                                         endState.players[i]->game->commandzone,
                                         endState.players[i]->game->sideboard,
                                         endState.players[i]->game->removedFromGame,
                                         endState.players[i]->game->temp };
        for (int j = 0; j < 8; j++)
        {
            MTGGameZone * zone = playerZones[j];
            if (zone->nb_cards != endstateZones[j]->nb_cards)
            {
                sprintf(
                                result,
                                "<span class=\"error\">==Card number not the same in player %i's %s==, expected %i, got %i</span><br />",
                                i, zone->getName(), endstateZones[j]->nb_cards, zone->nb_cards);
                Log(result);
                error++;
            }
            for (size_t k = 0; k < (size_t)endstateZones[j]->nb_cards; k++)
            {
                MTGCardInstance* cardToCheck = (k<endstateZones[j]->cards.size())?endstateZones[j]->cards[k]:0;
                if(cardToCheck)
                {   // Can be NULL if used "*" in the testcase.
                    // Resolving the expected id to ONE instance anywhere in the game
                    // (Rules::getCardByMTGId, global first match) made a same-id card in
                    // any other zone shadow this zone's copy and fail the check spuriously.
                    // Compare per-id counts between the expected and the actual zone instead.
                    bool firstOccurrence = true;
                    int expectedCount = 0;
                    for (size_t m = 0; m < (size_t)endstateZones[j]->nb_cards && m < endstateZones[j]->cards.size(); m++)
                    {
                        MTGCardInstance* other = endstateZones[j]->cards[m];
                        if (other && other->getId() == cardToCheck->getId())
                        {
                            if (m < k) firstOccurrence = false;
                            expectedCount++;
                        }
                    }
                    if (!firstOccurrence) continue; // already checked this id
                    int actualCount = 0;
                    for (int m = 0; m < zone->nb_cards; m++)
                    {
                        MTGCardInstance * actual = zone->cards[m];
                        if (!actual || actual->getMTGId() != cardToCheck->getId())
                            continue;
                        // Face-aware id match for transformed DOUBLE-FACED cards. A DFC
                        // keeps its FRONT MTGId after flipping in test-suite mode
                        // (AAFlip::resolve skips setMTGId under MODE_TEST_SUITE,
                        // AllAbilities.cpp:5308) while its name IS updated to the current
                        // face (_target->setName(myFlip->name)). An id-only match would
                        // let an expected pre-flip face name silently pass against a card
                        // that has transformed to its other face - so no fixture could
                        // observe a flip. When the actual card is double-faced, require
                        // its CURRENT face name to equal the expected name for the id hit
                        // to count.
                        //   The gate is scoped precisely to double-faced cards
                        // (backSide non-empty - set for every DFC/MDFC/werewolf face,
                        // AllAbilities.cpp:5279) and excludes COPIES (isACopier). This is
                        // deliberately narrow: OTHER effects also rename an instance while
                        // keeping its id, and the fixtures that use them assert by the
                        // ORIGINAL id, which is exactly the check they intend -
                        //   * copies: clone*, body_double, Living_Death (Clone keeps its
                        //     id, takes the copied creature's name; isACopier gates it),
                        //   * type-change "becomes": Evil Presence renames a Forest to
                        //     Swamp (backSide stays empty; the backSide gate excludes it).
                        // isFlipped is NOT usable as the discriminator - the day/night
                        // transform path leaves it 0. Tokens (negative id) keep pure id
                        // matching per the token note below. Non-DFC cards are unaffected
                        // (backSide empty). [DO] click-targeting (getCard) stays permissive
                        // by design - players click the physical card; face identity only
                        // matters at the ASSERT.
                        if (!actual->backSide.empty() && !actual->isACopier &&
                            cardToCheck->getId() >= 0 &&
                            actual->getName() != cardToCheck->getName())
                            continue;
                        actualCount++;
                    }
                    if (actualCount < expectedCount)
                    {
                        // Same-name entries can resolve to different printings between the
                        // INIT and ASSERT parsers (and zone moves rebuild instances), so a
                        // strict printing-id mismatch falls back to name-level counting -
                        // the test author's semantic. Token (-id) entries stay id-matched.
                        int expectedByName = 0, actualByName = 0;
                        for (size_t m = 0; m < (size_t)endstateZones[j]->nb_cards && m < endstateZones[j]->cards.size(); m++)
                        {
                            MTGCardInstance* other = endstateZones[j]->cards[m];
                            if (other && other->getName() == cardToCheck->getName())
                                expectedByName++;
                        }
                        for (int m = 0; m < zone->nb_cards; m++)
                        {
                            if (zone->cards[m] && zone->cards[m]->getName() == cardToCheck->getName())
                                actualByName++;
                        }
                        if (cardToCheck->getName().empty() || actualByName < expectedByName)
                        {
                            sprintf(result, "<span class=\"error\">==Card ID not the same. Didn't find %i</span><br />", cardToCheck->getId());
                            Log(result);
                            error++;
                        }
                    }
                }
            }
        }
        MTGGameZone * flagZones[] = { p->game->library, p->game->hand, p->game->inPlay,
                                      p->game->graveyard, p->game->commandzone,
                                      p->game->sideboard, p->game->removedFromGame,
                                      p->game->temp };
        //The expected necroed/ability/cardtext lists are parsed into the END
        //STATE players; the live players' copies are always empty, and reading
        //them made these asserts silently vacuous (they could never fail).
        TestSuiteAI * expectedP = endState.players[i];
        for (size_t n = 0; n < expectedP->expectedNecroed.size(); n++)
        {
            bool foundFlag = false;
            for (size_t z = 0; z < sizeof(flagZones) / sizeof(flagZones[0]); z++)
            {
                MTGGameZone * zone = flagZones[z];
                for (int k = 0; k < zone->nb_cards; k++)
                {
                    MTGCardInstance * card = zone->cards[k];
                    if (card && card->getLCName() == expectedP->expectedNecroed[n])
                        foundFlag = foundFlag || card->has(Constants::NECROED);
                }
            }
            if (!foundFlag)
            {
                sprintf(result, "<span class=\"error\">==necroed flag missing for %s in player %i==</span><br />",
                        expectedP->expectedNecroed[n].c_str(), i);
                Log(result);
                error++;
            }
        }
        MTGGameZone * abilityZones[] = { p->game->library, p->game->hand, p->game->inPlay,
                                         p->game->graveyard, p->game->commandzone,
                                         p->game->sideboard, p->game->removedFromGame,
                                         p->game->temp };
        for (size_t n = 0; n < expectedP->expectedBasicAbilities.size(); n++)
        {
            string spec = expectedP->expectedBasicAbilities[n];
            size_t divider = spec.rfind("|");
            bool foundAbility = false;
            if (divider != string::npos)
            {
                string cardName = spec.substr(0, divider);
                string abilityName = spec.substr(divider + 1);
                std::transform(cardName.begin(), cardName.end(), cardName.begin(), ::tolower);
                std::transform(abilityName.begin(), abilityName.end(), abilityName.begin(), ::tolower);
                int abilityIndex = -1;
                for (int a = 0; a < Constants::NB_BASIC_ABILITIES; a++)
                {
                    if (abilityName == Constants::MTGBasicAbilities[a])
                    {
                        abilityIndex = a;
                        break;
                    }
                }
                if (abilityIndex >= 0)
                {
                    for (size_t z = 0; z < sizeof(abilityZones) / sizeof(abilityZones[0]); z++)
                    {
                        MTGGameZone * zone = abilityZones[z];
                        for (int k = 0; k < zone->nb_cards; k++)
                        {
                            MTGCardInstance * card = zone->cards[k];
                            if (card && card->getLCName() == cardName && card->has(abilityIndex))
                                foundAbility = true;
                        }
                    }
                }
            }
            if (!foundAbility)
            {
                sprintf(result, "<span class=\"error\">==basic ability assertion failed for %s in player %i==</span><br />",
                        expectedP->expectedBasicAbilities[n].c_str(), i);
                Log(result);
                error++;
            }
        }
        for (size_t n = 0; n < expectedP->expectedCardText.size(); n++)
        {
            string spec = expectedP->expectedCardText[n];
            size_t divider = spec.rfind("|");
            bool foundText = false;
            if (divider != string::npos)
            {
                string cardName = spec.substr(0, divider);
                string expectedText = spec.substr(divider + 1);
                std::transform(cardName.begin(), cardName.end(), cardName.begin(), ::tolower);
                for (size_t z = 0; z < sizeof(abilityZones) / sizeof(abilityZones[0]); z++)
                {
                    MTGGameZone * zone = abilityZones[z];
                    for (int k = 0; k < zone->nb_cards; k++)
                    {
                        MTGCardInstance * card = zone->cards[k];
                        if (card && card->getLCName() == cardName && card->text == expectedText)
                            foundText = true;
                    }
                }
            }
            if (!foundText)
            {
                sprintf(result, "<span class=\"error\">==card text assertion failed for %s in player %i==</span><br />",
                        expectedP->expectedCardText[n].c_str(), i);
                Log(result);
                error++;
            }
        }
    }
    handleResults(wasAI, error);

    if(!error)
        Log("<span class=\"success\">==Test Successful !==</span>");
    else
        Log("<span class=\"error\">==Test Failed !==</span>");
#ifdef CAPTURE_STDERR
    if (error)
    {
        OutputCapturer::debugAndClear();
    } else
    {
        OutputCapturer::clear();
    }
#endif
    mMutex.unlock();
}

void TestSuite::handleResults(bool wasAI, int error)
{
    if (wasAI)
    {
        nbAITests++;
        if (error)
        {
            nbAIFailed++;
        }
    }
    else
    {
        nbTests++;
        if (error)
        {
            nbFailed++;
        }
    }
}

TestSuite::~TestSuite()
{
  mProcessing = false;
  while(mWorkerThread.size())
  {
    mWorkerThread.back()->join();
    SAFE_DELETE(mWorkerThread.back());
    mWorkerThread.pop_back();
  }

  observer = 0;
}

TestSuite::TestSuite(const char * filename)
    : TestSuiteGame(0), mRules(0), mProcessing(false)
{
    timerLimit = 0;
    testsuite = this;
    
    std::string s;
    nbfiles = 0;
    currentfile = 0;
    nbFailed = 0;
    nbTests = 0;
    nbAIFailed = 0;
    nbAITests = 0;
    int comment = 0;
    seed = 0;
    forceAbility = false;
    aiMaxCalls = -1;
    startTime = JGEGetTime();
    endTime = startTime;
    std::string contents;
    const char * testPrimitivesFile = getenv("WAGIC_TEST_PRIMITIVES_FILE");
    //Lexicon fixtures reference synthetic cards, and some of them (the landwalk
    //family) are in the MAIN registry — without the synthetic set the canonical
    //no-env invocation fails those tests. So the lexicon primitives are the
    //DEFAULT when the env var is unset; the env var still overrides (scoped runs
    //point it at a probe copy), and a missing default file is not an error.
    bool defaultedPrimitives = false;
    if (getenv("WAGIC_TESTSUITE") && (!testPrimitivesFile || !testPrimitivesFile[0]))
    {
        testPrimitivesFile = "test/lexicon/test_primitives.txt";
        defaultedPrimitives = true;
    }
    if (getenv("WAGIC_TESTSUITE") && testPrimitivesFile && testPrimitivesFile[0])
    {
        string error;
        if (!MTGCollection()->loadTestPrimitives(testPrimitivesFile, error))
        {
            if (!defaultedPrimitives)
            {
                fprintf(stderr, "WAGIC_TEST_PRIMITIVES_FILE: %s\n", error.c_str());
                exit(EXIT_FAILURE);
            }
        }
        else
            fprintf(stderr, "WAGIC_TEST_PRIMITIVES_FILE: loaded %s\n", testPrimitivesFile);
    }
    if (JFileSystem::GetInstance()->readIntoString(filename, contents))
    {
        std::stringstream stream(contents);
        while (std::getline(stream, s))
        {
            if (!s.size()) continue;
            if (s[s.size() - 1] == '\r') s.erase(s.size() - 1); //Handle DOS files
            if (!s.size()) continue;
            if (s[0] == '/' && s[1] == '*') comment = 1;
            if (s[0] && s[0] != '#' && !comment)
            {
                files[nbfiles] = s;
                nbfiles++;
            }
            if (s[0] == '*' && s[1] == '/') comment = 0;
        }
    }

    //If more than 1 test, prefecth names to make the suite run faster
    if (nbfiles > 1)
        MTGCollection()->prefetchCardNameCache();


    ofstream file2;
    if (JFileSystem::GetInstance()->openForWrite(file2, "/test/results.html"))
    {
        file2 << "<html><head>";
#ifdef WIN32
        file2 << "<meta http-equiv=\"refresh\" content=\"10\" >";
#endif
        file2 << "<STYLE type='text/css'>";
        file2 << ".success {color:green}\n";
        file2 << ".error {color:red}\n";
        file2 << "</STYLE></head><body>\n";
        file2.close();
    }

    SAFE_DELETE(observer);
}

int TestSuite::loadNext()
{
    mAsserted = false; //fresh test: its assert has not run yet
    endTime = JGEGetTime();
    summoningSickness = 0;
    seed = 0;
    aiMaxCalls = -1;
    if (!nbfiles) return 0;

    filename = getNextFile();
    if (filename == "") {
        // we let GameStateDuel delete the latest gameObserver.
        mProcessing = false;
        observer = 0;
        return 0;
    }

    if (filename == "+pregametests")
    {
        pregameTests();
        return loadNext();
    }

    if(!mProcessing)
    {   // "I don't like to wait" mode
        joinWorkers();
        mProcessing = true;

        size_t thread_count = 1;
#ifdef QT_CONFIG
        thread_count = QThread::idealThreadCount();
#elif defined(IOS)
        thread_count = 6;
#else
        thread_count = 4;
#endif
        //WAGIC_TESTSUITE_THREADS=1 runs the suite single-threaded (main
        //thread only) - the discriminator for the threaded-flake family
        //(CS-007/CS-017): a test that fails multi-threaded but is clean
        //single-threaded is a race, not a game bug.
        if (const char * tcount = getenv("WAGIC_TESTSUITE_THREADS"))
        {
            int n = atoi(tcount);
            if (n >= 1 && n <= 64)
                thread_count = (size_t)n;
        }
        for(size_t i = 0; i < (thread_count-1); i++)
            mWorkerThread.push_back(new boost::thread(ThreadProc, this));
    }

    cleanup();
    bool loaded;
    {
        //Serialized with the worker threads' loads (see ThreadProc)
        boost::mutex::scoped_lock lock(mMutex);
        loaded = load();
    }
    if (!loaded)
    {
        //A registered test file that cannot be loaded used to be skipped
        //silently, understating the test count and hiding dead entries.
        char buf[4096];
        sprintf(buf, "<h3>%s</h3>", filename.c_str());
        Log(buf);
        Log("<span class=\"error\">==Could not load test file==</span><br />");
        handleResults(false, 1);
        return loadNext();
    }
    else
        cout << "Starting test : " << files[currentfile - 1] << endl;
    return currentfile;
}

void TestSuite::joinWorkers()
{
    mProcessing = false;
    while(mWorkerThread.size())
    {
        mWorkerThread.back()->join();
        SAFE_DELETE(mWorkerThread.back());
        mWorkerThread.pop_back();
    }
}

void TestSuite::ThreadProc(void* inParam)
{
    LOG("Entering TestSuite::ThreadProc");
    TestSuite* instance = reinterpret_cast<TestSuite*>(inParam);
#ifdef CAPTURE_STDERR
    OutputCapturer::debugAndClear();
#endif
    if (instance)
    {
        string filename;
        while(instance->mProcessing && (filename = instance->getNextFile()) != "")
        {
            //Per-test, not per-worker: this counter is each Update's dt.
            //Initialized once per thread it kept growing across the whole
            //queue, so a test's dt values depended on how many ticks every
            //PREVIOUS test on that worker consumed - i.e. on the dynamic
            //file distribution, i.e. on thread scheduling. Any dt-gated
            //layer then behaved differently run to run. Resetting per game
            //gives every test the same dt sequence a solo run gets
            //(1,2,3,...), the regime all witnesses are tuned against.
            float counter = 1.0f;
            TestSuiteGame * theGame = NULL;
            {
                //File reads and the lazily-populated card collection are not
                //thread-safe: unserialized concurrent loads spuriously failed,
                //making tests randomly report "Could not load test file" (or,
                //before that was reported at all, silently vanish from the
                //results). Serialize the load; the game itself runs unlocked.
                boost::mutex::scoped_lock lock(mMutex);
                theGame = NEW TestSuiteGame(instance, filename);
            }
            if(theGame->isOK)
            {
                theGame->observer->loadTestSuitePlayer(0, theGame);
                theGame->observer->loadTestSuitePlayer(1, theGame);

                theGame->observer->startGame(theGame->gameType, /*instance->mRules*/Rules::getRulesByFilename("testsuite.txt"));
                //Mirror the GameStateDuel path: without this, a test's "seed"
                //directive only took effect when the MAIN thread ran it -
                //worker-run games kept the ctor's time(0) seed and seeded
                //tests (die rolls, coin flips) failed randomly in the suite
                //while passing solo.
                if (theGame->seed)
                    theGame->observer->resetSeed(theGame->seed);
                theGame->initGame();

                while(!theGame->observer->didWin())
                {
                    theGame->observer->Update(counter++);
                    //A game that cannot end (infinite trigger loop, wedged
                    //modal state) used to spin this worker forever: the run
                    //then hung at join time, or the test silently vanished
                    //from the final accounting (CS-010). Legit tests finish
                    //in a few thousand updates; the cap is ~200x that so it
                    //only ever fires on a genuine runaway - which must be
                    //REPORTED AS A FAILURE, never dropped.
                    if (counter > 1000000.0f)
                    {
                        char buf[4096];
                        sprintf(buf, "<h3>%s</h3>", filename.c_str());
                        theGame->Log(buf);
                        theGame->Log("<span class=\"error\">==Test timed out (game never ended)==</span><br />");
                        theGame->handleResults(false, 1);
                        break;
                    }
                }
                //The game can end BY ITSELF (a lethal trigger loop drains a
                //player) before the script reaches its end-of-actions
                //assert - the test then used to vanish from the accounting.
                //Assert the declared end state against the finished game.
                if (!theGame->mAsserted && counter <= 1000000.0f)
                    theGame->assertGame();
            }
            else
            {
                //Report unloadable tests instead of skipping them silently
                char buf[4096];
                sprintf(buf, "<h3>%s</h3>", filename.c_str());
                theGame->Log(buf);
                theGame->Log("<span class=\"error\">==Could not load test file==</span><br />");
                theGame->handleResults(false, 1);
            }
            SAFE_DELETE(theGame);
        }
    }
    LOG("Leaving TestSuite::ThreadProc");
}

int TestSuite::run()
{
  mProcessing = true;
//  loadNext();
  ThreadProc(this);

  return nbFailed + nbAIFailed;
}


void TestSuiteActions::cleanup()
{
    nbitems = 0;
}

void TestSuiteState::cleanup(TestSuiteGame* tsGame)
{
    phase = MTG_PHASE_INVALID;
    expectedGameOver = -1;
    for (size_t i = 0; i < players.size(); i++)
    {
        SAFE_DELETE(players[i]);
    }
    players.clear();

    players.push_back(new TestSuiteAI(tsGame, 0));
    players.push_back(new TestSuiteAI(tsGame, 1));
}

void TestSuite::cleanup()
{
    currentAction = 0;
    observer = 0;
    initState.cleanup(this);
    endState.cleanup(this);
    actions.cleanup();
}


bool TestSuiteGame::load()
{
    summoningSickness = 0;
    forceAbility = false;
    gameType = GAME_TYPE_CLASSIC;
    //TestSuite (the single-threaded path) IS a TestSuiteGame reused across every
    //test — without this reset, any earlier test whose game actually ended leaks
    //its result into later gameover: asserts (order-dependent false failures).
    observedGameOver = 0;

    std::string s;

    int state = -1;

    std::string contents;
    if (JFileSystem::GetInstance()->readIntoString("test/"+filename, contents))
    {
        std::stringstream stream(contents);
        while (std::getline(stream, s))
        {
            if (!s.size()) continue;
            if (s[s.size() - 1] == '\r') s.erase(s.size() - 1); //Handle DOS files
            if (!s.size()) continue;
            if (s[0] == '#') continue;
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            if (s.compare("summoningsickness") == 0)
            {
                summoningSickness = 1;
                continue;
            }
            if (s.compare("forceability") == 0)
            {
                forceAbility = true;
                continue;
            }
            if (s.find("seed ") == 0)
            {
                seed = atoi(s.substr(5).c_str());
                continue;
            }
            if (s.find("aicalls ") == 0)
            {
                aiMaxCalls = atoi(s.substr(8).c_str());
                continue;
            }
            if (s.compare("momir") == 0)
            {
                gameType = GAME_TYPE_MOMIR;
                continue;
            }
            switch (state)
            {
            case -1:
                if (s.compare("[init]") == 0) state++;
                break;
            case 0:
                if (s.compare("[player1]") == 0)
                {
                    state++;
                }
                else
                {
                    initState.phase = PhaseRing::phaseStrToInt(s);
                }
                break;
            case 1:
                if (s.compare("[player2]") == 0)
                {
                    state++;
                }
                else
                {
                    initState.parsePlayerState(0, s);
                }
                break;
            case 2:
                if (s.compare("[do]") == 0)
                {
                    state++;
                }
                else
                {
                    initState.parsePlayerState(1, s);
                }
                break;
            case 3:
                if (s.compare("[assert]") == 0)
                {
                    state++;
                }
                else
                {
                    actions.add(s);
                }
                break;
            case 4:
                if (s.compare("[player1]") == 0)
                {
                    state++;
                }
                else if (s.compare(0, 9, "gameover:") == 0)
                {
                    string value = s.substr(9);
                    if (value == "none") endState.expectedGameOver = 0;
                    else if (value == "p1") endState.expectedGameOver = 1;
                    else if (value == "p2") endState.expectedGameOver = 2;
                    else endState.expectedGameOver = -1;
                }
                else
                {
                    endState.phase = PhaseRing::phaseStrToInt(s);
                }
                break;
            case 5:
                if (s.compare("[player2]") == 0)
                {
                    state++;
                }
                else
                {
                    endState.parsePlayerState(0, s);
                }
                break;
            case 6:
                if (s.compare("[end]") == 0)
                {
                    state++;
                }
                else
                {
                    endState.parsePlayerState(1, s);
                }
                break;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

void TestSuite::pregameTests()
{
    //Test Booster Generation
    srand(1024);
    char result[1024];
    ShopBooster sb;
    for (int i = 0; i < 5; i++)
    {
        nbTests++;
        sprintf(result, "<h3>pregame/BoosterTest#%i</h3>", i);
        Log(result);
        if (!sb.unitTest()) nbFailed++;
    }
}

boost::mutex TestSuiteGame::mMutex;

TestSuiteGame::~TestSuiteGame()
{
    SAFE_DELETE(observer);
}

TestSuiteGame::TestSuiteGame(TestSuite* testsuite)
    : summoningSickness(0), forceAbility(false), mAsserted(false), gameType(GAME_TYPE_CLASSIC), timerLimit(0),
      currentAction(0), observer(0), observedGameOver(0), testsuite(testsuite)
{
}

TestSuiteGame::TestSuiteGame(TestSuite* testsuite, string _filename)
    : summoningSickness(0), forceAbility(false), mAsserted(false), gameType(GAME_TYPE_CLASSIC), timerLimit(FAST_TEST),
      currentAction(0), observer(0), observedGameOver(0), testsuite(testsuite)
{
    filename = _filename;
    observer = new GameObserver();
    observer->mSuiteGame = true; //keep ASPHASES auto-skips off even after `ai` flips playModes

    initState.cleanup(this);
    endState.cleanup(this);

    isOK = load();
}

void TestSuiteGame::ResetManapools()
{
    for (int i = 0; i < 2; ++ i)
        observer->players[i]->getManaPool()->copy(initState.players[i]->getManaPool());
}


void TestSuiteGame::initGame()
{
    //The obs= pointer keys this test's game-side trace lines (ZONE
    //SHUFFLE etc.) back to its filename in the braided threaded log.
    DebugTrace("TESTSUITE Init Game [" << filename << "] obs=" << (void *)observer);
    observer->phaseRing->goToPhase(initState.phase, observer->players[0], false);
    observer->setCurrentGamePhase(initState.phase);

    observer->resetStartupGame();

    for (int i = 0; i < 2; i++)
    {        
        AIPlayerBaka * p = (AIPlayerBaka *) (observer->players[i]);
        p->forceBestAbilityUse = forceAbility;
        p->life = initState.players[i]->life;
        p->poisonCount = initState.players[i]->poisonCount;
        stringstream stream;
        initState.players[i]->getRandomGenerator()->saveLoadedRandValues(stream);
        p->getRandomGenerator()->loadRandValues(stream.str());
        MTGGameZone * playerZones[] = { p->game->graveyard, p->game->library, p->game->hand, p->game->inPlay, p->game->commandzone, p->game->sideboard, p->game->removedFromGame };
        MTGGameZone * loadedPlayerZones[] = { initState.players[i]->game->graveyard,
                                              initState.players[i]->game->library,
                                              initState.players[i]->game->hand,
                                              initState.players[i]->game->inPlay,
                                              initState.players[i]->game->commandzone,
                                              initState.players[i]->game->sideboard,
                                              initState.players[i]->game->removedFromGame };
        for (int j = 0; j < 7; j++)
        {
            MTGGameZone * zone = playerZones[j];
            for (size_t k = 0; k < loadedPlayerZones[j]->cards.size(); k++)
            {
                MTGCardInstance * card = Rules::getCardByMTGId(observer, loadedPlayerZones[j]->cards[k]->getId(), p);
                if (card && zone != p->game->library)
                {
                    if (zone == p->game->inPlay)
                    {
                        //MTGCardInstance * copy = p->game->putInZone(card, p->game->library, p->game->stack);
                        MTGCardInstance * copy = zone->owner->game->putInZone(card, p->game->library, p->game->stack);
                        if (!copy)
                        {
                            LOG ("TESTSUITE ERROR, could not move card to stack\n");
                            continue;
                        }
                        Spell * spell = NEW Spell(observer, copy);
                        spell->resolve();
                        if (!summoningSickness && (size_t)p->game->inPlay->nb_cards > k) p->game->inPlay->cards[k]->summoningSickness = 0;
                        //[INIT] tappedinplay:N taps the first N battlefield cards (no
                        //triggers - setup state, not a game action). In [ASSERT] the
                        //same key is the tapped-count expectation.
                        TestSuiteAI * initPlayer = initState.players[i];
                        if (initPlayer->expectedTappedInPlay > 0 && k < (size_t)initPlayer->expectedTappedInPlay
                            && (size_t)p->game->inPlay->nb_cards > k)
                            p->game->inPlay->cards[k]->tap(true);
                        delete spell;
                    }
                    else
                    {
                        if (!p->game->library->hasCard(card))
                        {
                            LOG ("TESTUITE ERROR, CARD NOT FOUND IN LIBRARY\n");
                        }
                        //p->game->putInZone(card, p->game->library, zone);
                        zone->owner->game->putInZone(card, p->game->library, zone);
                    }
                }
                else
                {
                    if (!card)
                    {
                        LOG ("TESTUITE ERROR, card is NULL\n");
                    }
                }
            }
            zone->cardsSeenThisTurn.clear(); //don't consider those cards as having moved in this area during this turn
        }
        p->game->stack->cardsSeenThisTurn.clear(); //don't consider those cards as having moved in this area during this turn

    }
    DebugTrace("TESTUITE Init Game Done !");
}

MTGPlayerCards * TestSuiteGame::buildDeck(Player* player, int playerId)
{
    int list[100];
    int nbcards = 0;
    MTGPlayerCards * deck = NULL;

    if(initState.players.size() > (size_t)playerId)
    {
        MTGGameZone * loadedPlayerZones[] = { initState.players[playerId]->game->graveyard,
            initState.players[playerId]->game->library,
            initState.players[playerId]->game->hand,
            initState.players[playerId]->game->removedFromGame,
            initState.players[playerId]->game->commandzone,
            initState.players[playerId]->game->sideboard,
            initState.players[playerId]->game->inPlay };

        for (int j = 0; j < 7; j++)
        {
            for (size_t k = 0; k < loadedPlayerZones[j]->cards.size(); k++)
            {
                int cardid = loadedPlayerZones[j]->cards[k]->getId();
                list[nbcards] = cardid;
                nbcards++;
            }
        }
        deck = NEW MTGPlayerCards(player, list, nbcards);
    }
    else
    {
        deck = NEW MTGPlayerCards();
    }

    return deck;
}

// ---------------------------------------------------------------------------
// Card-script validator (WAGIC_VALIDATE=1)
//
// Drives EVERY collection card's ability lines through the REAL ability-factory
// parse path (AbilityFactory::getAbilities -> parseMagicLine), the same code
// gameplay uses. A line that fails to produce an ability is reported as one
// machine-readable VALIDATE-FAIL record; a summary and exit code follow. The
// engine's NULL detection is tapped in place via gAbilityParseFailCallback (see
// MTGAbility.cpp), so there is no parallel parser to drift from the real one.
// ---------------------------------------------------------------------------
namespace
{
    struct ValFailure { string name; int id; string kind; string line; };

    static GameObserver *      gValObserver   = NULL;
    static vector<ValFailure> * gValFailures  = NULL;
    static map<string,int> *   gValSkip       = NULL;

    // Map a getAbilities dest zone back to the CardPrimitive key / line "kind".
    static string valKindForDest(MTGGameZone * dest)
    {
        if (!dest || !gValObserver) return "auto";
        Player * p = gValObserver->players[0];
        MTGPlayerCards * g = p->game;
        if (dest == g->hand)        return "autohand";
        if (dest == g->graveyard)   return "autograveyard";
        if (dest == g->stack)       return "autostack";
        if (dest == g->exile)       return "autoexile";
        if (dest == g->commandzone) return "autocommandzone";
        if (dest == g->library)     return "autolibrary";
        return "autozone";
    }

    static void valFailSink(MTGCardInstance * card, const string & line, MTGGameZone * dest, bool byDesign)
    {
        if (byDesign)
        {
            if (gValSkip) (*gValSkip)["skip:alt-cost-unpaid(by-design)"]++;
            return;
        }
        if (!gValFailures) return;
        ValFailure f;
        f.name = card ? card->name : string("?");
        f.id   = card ? card->getId() : 0;
        f.kind = valKindForDest(dest);
        f.line = line;
        gValFailures->push_back(f);
    }
}

int runCardScriptValidation()
{
    // Output target: stdout by default, or the file named by WAGIC_VALIDATE_OUT.
    FILE * out = stdout;
    bool closeOut = false;
    if (const char * of = getenv("WAGIC_VALIDATE_OUT"))
        if (of[0])
        {
            FILE * f = fopen(of, "w");
            if (f) { out = f; closeOut = true; }
            else fprintf(stderr, "WAGIC_VALIDATE: could not open %s for write; using stdout\n", of);
        }

    // Optional synthetic primitives (validate those too); default is the real sets only.
    if (const char * tp = getenv("WAGIC_TEST_PRIMITIVES_FILE"))
        if (tp[0])
        {
            string err;
            if (!MTGCollection()->loadTestPrimitives(tp, err))
                fprintf(stderr, "WAGIC_VALIDATE: could not load test primitives %s: %s\n", tp, err.c_str());
            else
                fprintf(stderr, "WAGIC_VALIDATE: loaded test primitives %s\n", tp);
        }

    MTGCollection()->prefetchCardNameCache();

    // Full 2-player game context via the normal startGame path. A bare GameObserver
    // is not enough: some ability constructors fire game events on creation (e.g.
    // fading:N -> AVanishing -> Counters::addCounter -> GameObserver::receiveEvent ->
    // DuelLayers), which dereference the observer's duel infrastructure. startGame
    // builds mLayers/phaseRing/currentPlayer and seeds each player's zones. We never
    // enter the duel loop; we exit() after the pass. Deck contents are irrelevant.
    GameObserver * obs = new GameObserver(WResourceManager::Instance(), JGE::GetInstance());
    for (int i = 0; i < 2; ++i)
    {
        MTGDeck * d = NEW MTGDeck("ai/baka/deck1.txt", MTGCollection(), 0, 0);
        AIPlayerBaka * p = NEW AIPlayerBaka(obs, "ai/baka/deck1.txt", "ai_baka_deck1", "", d);
        obs->loadPlayer(i, p);
    }
    obs->startGame(GAME_TYPE_CLASSIC, Rules::getRulesByFilename("classic.txt"));
    Player * p0 = obs->players[0];

    vector<ValFailure> failures;
    map<string,int> skips;
    gValObserver  = obs;
    gValFailures  = &failures;
    gValSkip      = &skips;
    gAbilityParseLineCount   = 0;
    gAbilityParseFailCallback = valFailSink;

    // Zones getAbilities routes text for via dest. facedown/faceup/skill texts are
    // parsed only from specific card STATE (morph/equip) rather than a dest, so they
    // are counted as an explicit SKIP category below rather than parsed cold (which
    // would flood false positives). reveal/sideboard texts come only from anyzone=,
    // whose lines are ALSO in the default text and thus already validated there.
    MTGGameZone * destZones[] = { NULL, p0->game->hand, p0->game->graveyard,
                                  p0->game->stack, p0->game->exile, p0->game->commandzone };
    const char * stateOnlyKeys[] = { "facedown", "faceup", "skill" };

    long cardCount = 0;

    vector<int> & ids = MTGCollection()->ids;
    for (size_t i = 0; i < ids.size(); ++i)
    {
        MTGCard * mc = MTGCollection()->getCardById(ids[i]);
        if (!mc || !mc->data) continue;
        cardCount++;

        // Fresh factory PER CARD, exactly as every production call site does
        // (ActionLayer, AIPlayerBaka construct one per parse). A shared factory
        // leaks its stored parse-state members (storedAbilityString/PayString/
        // String/AndAbility) between cards, and a card that leaves them dirty
        // corrupts a LATER card's parse — that ordering artifact produced 71
        // false positives (incl. all 10 shocklands) in the first corpus run.
        AbilityFactory af(obs);

        MTGCardInstance * inst = NEW MTGCardInstance(mc, p0->game);
        // Seed runtime-context fields that some target choosers dereference during a
        // COLD parse (no live ability/spell): "abilitycontroller"/"mysource" read
        // card->storedSourceCard->controller(), NULL until an ability resolves.
        inst->storedSourceCard = inst;
        // Parse with a Spell context, exactly as gameplay does (AbilityFactory::
        // addAbilities builds a Spell before magicText/getAbilities). spell=NULL made
        // ability$!...!$ / mysource / choice ETB lines (e.g. shocklands) spuriously
        // NULL because they read spell->source. payResult stays MANA_UNPAID so unpaid
        // alternative-cost branches still take the by-design SKIP path.
        Spell * sp = NEW Spell(obs, inst);

        for (size_t z = 0; z < sizeof(destZones)/sizeof(destZones[0]); ++z)
        {
            MTGGameZone * dest = destZones[z];
            if (dest)
            {
                // Only parse a zone the card actually has authored/derived text for.
                string key = valKindForDest(dest).substr(4); // strip "auto"
                map<string,string>::iterator it = inst->magicTexts.find(key);
                if (it == inst->magicTexts.end() || it->second.empty())
                    continue;
            }
            vector<MTGAbility *> v;
            af.getAbilities(&v, sp, inst, 0, dest);
            for (size_t k = 0; k < v.size(); ++k)
                SAFE_DELETE(v[k]);
        }
        SAFE_DELETE(sp);

        for (size_t u = 0; u < sizeof(stateOnlyKeys)/sizeof(stateOnlyKeys[0]); ++u)
        {
            map<string,string>::iterator it = inst->magicTexts.find(stateOnlyKeys[u]);
            if (it != inst->magicTexts.end() && !it->second.empty())
                skips[string("skip:state-context-only:") + stateOnlyKeys[u]]++;
        }

        SAFE_DELETE(inst);
    }

    gAbilityParseFailCallback = NULL;
    gValFailures = NULL;
    gValSkip     = NULL;
    gValObserver = NULL;

    for (size_t i = 0; i < failures.size(); ++i)
        fprintf(out, "VALIDATE-FAIL\t%s\t%d\t%s\t%s\n",
                failures[i].name.c_str(), failures[i].id,
                failures[i].kind.c_str(), failures[i].line.c_str());

    for (map<string,int>::iterator it = skips.begin(); it != skips.end(); ++it)
        fprintf(stderr, "VALIDATE-SKIP\t%s\tcount=%d\n", it->first.c_str(), it->second);

    fprintf(out, "VALIDATE-SUMMARY\tcards=%ld\tlines=%ld\tfailures=%zu\n",
            cardCount, gAbilityParseLineCount, failures.size());
    fflush(out);
    if (closeOut) fclose(out);

    SAFE_DELETE(obs);

    return (int) failures.size();
}

#endif
