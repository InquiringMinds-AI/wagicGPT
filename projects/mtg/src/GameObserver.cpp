#include "PrecompiledHeader.h"
#include "AllAbilities.h" //W53-DELVER: MTGRevealingCards for humanDisplayOpen()

#include "GameObserver.h"
#if defined(VITA) && defined(WAGIC_VITAMEMLOG)
//Vita heap/vitaGL probe (JGE/src/Vitamain.cpp). Declared at file scope: a linkage
//specification is not allowed inside a function body (vpk9 build, 2026-09-01).
extern "C" void vitaMemProbe(const char*, int);
//Frame-time telemetry (same gate, same memlog file): counts published per
//tick, one aggregate line per turn written beside the untap memory probe.
extern "C" void vitaFrameCounts(int, int, int, int, int);
extern "C" void vitaFrameTurnLog(int);
#endif
#include "PreGamePhase.h"
#include "LegalActions.h"
#ifdef WITH_GPT_AI
#include "GptConfig.h"
#endif
#include "CardGui.h"
#include "Damage.h"
#include "Rules.h"
#include "ExtraCost.h"
#include "Subtypes.h"
#include <JLogger.h>
#include <JRenderer.h>
#include "MTGGamePhase.h"
#include "GuiPhaseBar.h"
#include "AIPlayerBaka.h"
#include "MTGRules.h"
#include "Trash.h"
#include "DeckManager.h"
#include "GuiCombat.h"
#include <algorithm>
#ifdef WAGIC_TRANSCRIPT_ON
#ifdef VITA
#include <psp2/io/stat.h>
#else
#include <sys/stat.h>
#endif
#endif
#ifdef TESTSUITE
#include "TestSuiteAI.h"
#endif
#ifdef NETWORK_SUPPORT
#include "NetworkPlayer.h"
#endif


void GameObserver::cleanup()
{
    SAFE_DELETE(targetChooser);
    SAFE_DELETE(mLayers);
    SAFE_DELETE(phaseRing);
    SAFE_DELETE(replacementEffects);
    for (size_t i = 0; i < players.size(); ++i)
    {
        if(players[i])
            SAFE_DELETE(players[i]);
    }
    players.clear();

    mSuiteHumanSeat = NULL;
    currentPlayer = NULL;
    currentActionPlayer = NULL;
    isInterrupting = NULL;
    currentPlayerId = 0;
    mCurrentGamePhase = MTG_PHASE_INVALID;
    targetChooser = NULL;
    cardWaitingForTargets = NULL;
    mExtraPayment = NULL;
    gameOver = NULL;
    phaseRing = NULL;
    replacementEffects = NEW ReplacementEffects();
    combatStep = BLOCKERS;
    connectRule = false;
    LPWeffect = false;
    actionsList.clear();
    gameTurn.clear();
    OpenedDisplay = NULL;
    mSettledPhase = -1; mSettledTurn = -1; mSettledStep = -1; mPhaseTicks = 0; //W53-DELVER
    AffinityNeedsUpdate = false;
    mAbilityEpoch = 1; //#W54-H (A6b): never equals a fresh ability's 0
}

GameObserver::~GameObserver()
{
    LOG("==Destroying GameObserver==");
    SAFE_DELETE(mPregame);

    for (size_t i = 0; i < players.size(); ++i)
    {
        if(players[i])
            players[i]->End();
    }
    SAFE_DELETE(targetChooser);
    SAFE_DELETE(mLayers);
    SAFE_DELETE(phaseRing);
    SAFE_DELETE(replacementEffects);
    for (size_t i = 0; i < players.size(); ++i)
    {
        if(players[i])
            SAFE_DELETE(players[i]);
    }
    players.clear();
    delete[] ExtraRules;
    ExtraRules = 0;
    LOG("==GameObserver Destroyed==");
    SAFE_DELETE(mTrash);
    SAFE_DELETE(mDeckManager);

}

GameObserver::GameObserver(WResourceManager *output, JGE* input)
    : mSeed((unsigned int)time(0)), randomGenerator(mSeed, true), mResourceManager(output), mJGE(input)

{
    ExtraRules = new MTGCardInstance[2]();

    mGameType = GAME_TYPE_CLASSIC;
    currentPlayer = NULL;
    currentActionPlayer = NULL;
    isInterrupting = NULL;
    currentPlayerId = 0;
    mCurrentGamePhase = MTG_PHASE_INVALID;
    targetChooser = NULL;
    cardWaitingForTargets = NULL;
    mExtraPayment = NULL;
    OpenedDisplay = NULL;
    mSettledPhase = -1; mSettledTurn = -1; mSettledStep = -1; mPhaseTicks = 0; //W53-DELVER
    guiOpenDisplay = NULL;
    gameOver = NULL;
    phaseRing = NULL;
    replacementEffects = NEW ReplacementEffects();
    combatStep = BLOCKERS;
    mRules = NULL;
    connectRule = false;
    LPWeffect = false;
    mSnapshotPostPregame = false;
    mLoading = false;
    mSuiteGame = false;
    mGameEndNoted = false;
    mGameEndMemlogged = false;
    mSuiteHumanSeat = NULL;
    mLayers = NULL;
    mTrash = new Trash();
    mDeckManager = new DeckManager();
}

GamePhase GameObserver::getCurrentGamePhase()
{
    return mCurrentGamePhase;
}

const string& GameObserver::getCurrentGamePhaseName()
{
    return phaseRing->phaseName(mCurrentGamePhase);
}

const string& GameObserver::getNextGamePhaseName()
{
    return phaseRing->phaseName((mCurrentGamePhase + 1) % MTG_PHASE_CLEANUP);
}

Player * GameObserver::opponent()
{
    int index = (currentPlayerId + 1) % players.size();
    return players[index];
}

Player * GameObserver::nextTurnsPlayer()
{
    int nextTurnsId = 0;
    if(!players[currentPlayerId]->extraTurn)
        nextTurnsId = (currentPlayerId + 1) % players.size();
    else
    {
        nextTurnsId = currentPlayerId;
    }
    if(players[currentPlayerId]->skippingTurn)
    {
        nextTurnsId = (currentPlayerId + 1) % players.size();
    }
    return players[nextTurnsId];
}

void GameObserver::nextPlayer()
{
    turn++;
    if(!players[currentPlayerId]->extraTurn)
        currentPlayerId = (currentPlayerId + 1) % players.size();
    else
    {
        players[currentPlayerId]->extraTurn--;
    }
    if(players[currentPlayerId]->skippingTurn)
    {
        players[currentPlayerId]->skippingTurn--;
        currentPlayerId = (currentPlayerId + 1) % players.size();
    }
    currentPlayer = players[currentPlayerId];
    currentActionPlayer = currentPlayer;
    combatStep = BLOCKERS;
}

void GameObserver::nextGamePhase()
{
    Phase * cPhaseOld = phaseRing->getCurrentPhase();
    if (cPhaseOld->id == MTG_PHASE_COMBATDAMAGE)
        if ((FIRST_STRIKE == combatStep) || (END_FIRST_STRIKE == combatStep) || (DAMAGE == combatStep))
        {
            nextCombatStep();
            return;
        }

    if (cPhaseOld->id == MTG_PHASE_COMBATBLOCKERS)
        if (BLOCKERS == combatStep || TRIGGERS == combatStep)
        {
            nextCombatStep();
            return;
        }

    phaseRing->forward();

    //Go directly to end of combat if no attackers
    if (cPhaseOld->id == MTG_PHASE_COMBATATTACKERS && !(currentPlayer->game->inPlay->getNextAttacker(NULL)))
    {
        phaseRing->forward();
        phaseRing->forward();
    }

    Phase * cPhase = phaseRing->getCurrentPhase();
    mCurrentGamePhase = cPhase->id;

    // CR 103.8a / 504.1: in a two-player game the starting player skips the draw
    // step of their first turn. turn == 0 is the starting player first turn. Skip
    // past the draw step WITHIN this same nextGamePhase call so no Update observes
    // the draw phase: the draw-step rule (@each my draw:draw:1) triggers by polling
    // getCurrentGamePhase between Updates, so a phase the game never rests in never
    // draws. Suite games are exempt (fixtures manufacture arbitrary turn-0 phase
    // states and drive their own draws).
    if (mCurrentGamePhase == MTG_PHASE_DRAW && turn == 0 && !mSuiteGame)
    {
        phaseRing->forward();
        cPhase = phaseRing->getCurrentPhase();
        mCurrentGamePhase = cPhase->id;
    }

    if (MTG_PHASE_COMBATDAMAGE == mCurrentGamePhase)
        nextCombatStep();
    if (MTG_PHASE_COMBATEND == mCurrentGamePhase)
        combatStep = BLOCKERS;

    //if (currentPlayer != cPhase->player)
    //    nextPlayer();//depreciated; we call this at EOT step now. unsure what the purpose of this was originally.fix for a bug?

    //init begin of turn
    if (mCurrentGamePhase == MTG_PHASE_BEFORE_BEGIN)
    {
        cleanupPhase();
        currentPlayer->damageCount = 0;
        currentPlayer->nonCombatDamage = 0;
        currentPlayer->drawCounter = 0;
        currentPlayer->raidcount = 0;
        currentPlayer->cycledCount = 0;
        currentPlayer->dealsdamagebycombat = 0; //clear check for restriction
        currentPlayer->opponent()->raidcount = 0;
        currentPlayer->opponent()->cycledCount = 0;
        currentPlayer->prowledTypes.clear();
        currentPlayer->opponent()->damageCount = 0; //added to clear odcount
        currentPlayer->opponent()->nonCombatDamage = 0;
        currentPlayer->preventable = 0;
        mLayers->actionLayer()->cleanGarbage(); //clean abilities history for this turn;
        mLayers->stackLayer()->garbageCollect(); //clean stack history for this turn;
        mLayers->actionLayer()->Update(0);
        currentPlayer->game->library->miracle = false;
        currentPlayer->opponent()->game->library->miracle = false;
        for (int i = 0; i < 2; i++)
        {
            //Cleanup of each player's gamezones
            players[i]->game->beforeBeginPhase();
        }
        combatStep = BLOCKERS;
        return nextGamePhase();
    }

    if (mCurrentGamePhase == MTG_PHASE_AFTER_EOT)
    {
        //CR 514.2 backstop (2026-08-07): damage removal and "until end of
        //turn" expiry must be SIMULTANEOUS. The scripted heal (@each
        //cleanup ... resetDamage in rules/*.txt) resolves through the
        //stack; live play's phase auto-skip (ASKIP, forced OFF in suite
        //games) can pull the whole cleanup->AFTER_EOT->next-turn transit
        //into one synchronous chain before that trigger resolves - the
        //actionLayer Update below then strips ueot effects, BEFORE_BEGIN
        //garbage-collects the unresolved trigger, and the next SBA pass
        //destroys creatures whose lethal marked damage was about to be
        //removed (live: Rootborn Defenses survivors dying at end of
        //turn). Heal synchronously before ueot retirement; mirrors
        //AAResetDamage::resolve() including its exceptions.
        for (int rdp = 0; rdp < 2; rdp++)
        {
            MTGGameZone * rdz = players[rdp]->game->inPlay;
            for (int rdi = 0; rdi < rdz->nb_cards; rdi++)
            {
                MTGCardInstance * rdc = rdz->cards[rdi];
                if (rdc->has(Constants::NODAMAGEREMOVED))
                    continue;
                if (!rdc->isCreature() && rdc->hasType(Subtypes::TYPE_PLANESWALKER))
                {
                    if (rdc->counters && rdc->counters->hasCounter("loyalty", 0, 0))
                        rdc->life = rdc->counters->hasCounter("loyalty", 0, 0)->nb;
                }
                else if (!rdc->isCreature() && rdc->hasType(Subtypes::TYPE_BATTLE))
                {
                    if (rdc->counters && rdc->counters->hasCounter("defense", 0, 0))
                        rdc->life = rdc->counters->hasCounter("defense", 0, 0)->nb;
                }
                else
                    rdc->life = rdc->toughness;
            }
        }

        int handmodified = 0;
        handmodified = currentPlayer->handsize+currentPlayer->handmodifier;
        //Auto Hand cleaning, in case the player didn't do it himself
        if(handmodified < 0)
            handmodified = 0;
        while (currentPlayer->game->hand->nb_cards > handmodified && currentPlayer->nomaxhandsize == false)
        {
            WEvent * e = NEW WEventCardDiscard(currentPlayer->game->hand->cards[0]);
            receiveEvent(e);
            currentPlayer->game->putInGraveyard(currentPlayer->game->hand->cards[0]);
        }
        mLayers->actionLayer()->Update(0);
        currentPlayer->drawCounter = 0;
        currentPlayer->prowledTypes.clear();
        currentPlayer->lifeLostThisTurn = 0;
        currentPlayer->opponent()->lifeLostThisTurn = 0;
        currentPlayer->lifeGainedThisTurn = 0;
        currentPlayer->opponent()->lifeGainedThisTurn = 0;
        currentPlayer->doesntEmpty->remove(currentPlayer->doesntEmpty);
        currentPlayer->opponent()->doesntEmpty->remove(currentPlayer->opponent()->doesntEmpty);
        nextPlayer();
        return nextGamePhase();
    }

    //Phase Specific actions
    switch (mCurrentGamePhase)
    {
    case MTG_PHASE_UNTAP:
        DebugTrace("Untap Phase -------------   Turn " << turn );
#if defined(VITA) && defined(WAGIC_VITAMEMLOG)
        vitaMemProbe("untap", turn);
        vitaFrameTurnLog(turn);
#endif
#ifdef WAGIC_TRANSCRIPT_ON
        writeTranscript("turn");
#endif
        phasingPhase();
        untapPhase();
        break;
    case MTG_PHASE_COMBATBLOCKERS:
        receiveEvent(NEW WEventAttackersChosen());
        break;
    default:
        break;
    }
}

int GameObserver::cancelCurrentAction()
{
    SAFE_DELETE(targetChooser);
    return mLayers->actionLayer()->cancelCurrentAction();
}

void GameObserver::nextCombatStep()
{
    switch (combatStep)
    {
    case BLOCKERS:
        receiveEvent(NEW WEventBlockersChosen());
        receiveEvent(NEW WEventCombatStepChange(combatStep = TRIGGERS));
        return;

    case TRIGGERS:
        receiveEvent(NEW WEventCombatStepChange(combatStep = ORDER));
        return;
    case ORDER:
        receiveEvent(NEW WEventCombatStepChange(combatStep = FIRST_STRIKE));
        return;
    case FIRST_STRIKE:
        receiveEvent(NEW WEventCombatStepChange(combatStep = END_FIRST_STRIKE));
        return;
    case END_FIRST_STRIKE:
        receiveEvent(NEW WEventCombatStepChange(combatStep = DAMAGE));
        return;
    case DAMAGE:
        receiveEvent(NEW WEventCombatStepChange(combatStep = END_DAMAGE));
        return;
    case END_DAMAGE:
        ; // Nothing : go to next phase
    }
}

GameObserver::CombatDecision GameObserver::pendingCombatDecision(Player * p)
{
    //No combat decision exists while anything else is in flight: an
    //unsettled stack (attack triggers must RESOLVE before blocks are
    //declared), an open menu, a live target chooser, or a pending extra
    //payment. The stack gate is what kills the historical silent
    //blocker-skip: the decision is deferred, not dropped.
    //N-146s (wave-36 diagnostic, dev builds only): in the B3 122B game the
    //caster of a fused Silverquill Command received NO attacker asks for nine
    //consecutive turns while holding legal attackers; the opponent-granted
    //sacrifice chooser surfaced ~9 turns late. A scripted probe proved the
    //grant resolves promptly once the receiver clicks, so the fault is in
    //WHICH of these gates stays raised on live AI seats. This tracer names
    //the suppressing gate once per (turn, phase, seat) at combat phases, to
    //stderr (harvested per game by the selfplay harness), so the next corpus
    //occurrence carries its own diagnosis. Compiled out of release builds.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
#define WAGIC_COMBATTRACE_SUPPRESS(reason) \
    do { if ((mCurrentGamePhase == MTG_PHASE_COMBATATTACKERS && p == currentPlayer) \
          || (mCurrentGamePhase == MTG_PHASE_COMBATBLOCKERS && p != currentPlayer)) { \
        static int lastTurn = -1; static int lastPhase = -1; static void * lastSeat = NULL; \
        if (lastTurn != turn || lastPhase != (int) mCurrentGamePhase || lastSeat != (void *) p) { \
            lastTurn = turn; lastPhase = (int) mCurrentGamePhase; lastSeat = (void *) p; \
            fprintf(stderr, "[combattrace] t%d ph%d seat=%p combat decision suppressed by %s\n", \
                    turn, (int) mCurrentGamePhase, (void *) p, reason); \
        } } } while (0)
#else
#define WAGIC_COMBATTRACE_SUPPRESS(reason) do { } while (0)
#endif
    if (mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED))
    {
        WAGIC_COMBATTRACE_SUPPRESS("unresolved-stack");
        return COMBAT_DECISION_NONE;
    }
    if (mLayers->actionLayer()->menuObject)
    {
        WAGIC_COMBATTRACE_SUPPRESS("open-menu");
        return COMBAT_DECISION_NONE;
    }
    if (getCurrentTargetChooser())
    {
        WAGIC_COMBATTRACE_SUPPRESS("live-target-chooser");
        return COMBAT_DECISION_NONE;
    }
    if (mExtraPayment)
    {
        WAGIC_COMBATTRACE_SUPPRESS("extra-payment");
        return COMBAT_DECISION_NONE;
    }
    if (mCurrentGamePhase == MTG_PHASE_COMBATATTACKERS)
    {
        if (p != currentPlayer)
            return COMBAT_DECISION_NONE;
        if (!LegalActionsOracle::hasLegalAttacker(p))
        {
            WAGIC_COMBATTRACE_SUPPRESS("no-legal-attacker");
            return COMBAT_DECISION_NONE;
        }
        return COMBAT_DECISION_ATTACKERS;
    }
    if (mCurrentGamePhase == MTG_PHASE_COMBATBLOCKERS && combatStep == BLOCKERS)
    {
        if (p == currentPlayer)
            return COMBAT_DECISION_NONE;
        if (!currentPlayer->game->inPlay->getNextAttacker(NULL))
            return COMBAT_DECISION_NONE;
        if (!LegalActionsOracle::hasLegalBlock(p))
            return COMBAT_DECISION_NONE;
        return COMBAT_DECISION_BLOCKERS;
    }
    return COMBAT_DECISION_NONE;
}

bool GameObserver::humanDisplayOpen()
{
    if (!mLayers || !mLayers->actionLayer())
        return false;
    ActionLayer * al = mLayers->actionLayer();
    for (size_t i = 0; i < al->mObjects.size(); i++)
    {
        MTGRevealingCards * rev = dynamic_cast<MTGRevealingCards *>((ActionElement *) al->mObjects[i]);
        if (!rev || !rev->source)
            continue;
        Player * owner = rev->source->controller();
        if (!owner)
            continue;
        bool human = mSuiteHumanSeat ? (owner == mSuiteHumanSeat) : !owner->isAI();
        if (!human)
            continue;
        //open (its display is THE open display) or pending (resolved, display
        //not built yet - the ability is reaped only after it closes)
        if ((OpenedDisplay && rev->revealDisplay == OpenedDisplay) || !rev->revealDisplay)
            return true;
    }
    return false;
}

void GameObserver::userRequestNextGamePhase(bool allowInterrupt, bool log)
{
    //W53-DELVER (owner Vita report 2026-08-28; generalised per his note to every
    //step trigger - end of turn, the opponent's upkeep, ...): no phase advance
    //while the human seat has a reveal display open or about to open. Applies
    //to the AI's own pass on ITS turn too, so a "beginning of each upkeep"
    //display of the human's is not stepped over by the opponent's advance.
    if (allowInterrupt && humanDisplayOpen())
        return;
    //W53-DELVER, the REQUEST side (reproduced on build 9 in the desktop GUI:
    //the phase-advance key is HELD across ticks by JGE - a right-click here, the
    //trigger on the Vita - so the request that pushed the turn along landed in
    //the very tick the upkeep was entered, and the upkeep was never polled by a
    //single trigger). A request may only leave a phase that has survived a full
    //Update in which the layers (and every phase trigger) saw it: the counter is
    //advanced at the end of the tick, so >= 2 means one complete tick has run on
    //this phase since it was entered. Suite-driven games step phases on their
    //own pump cadence and are unaffected; the AI simply re-requests next tick.
    //The settled record is written at the END of a tick, so inside the tick the
    //phase changed it still names the previous phase - that mismatch is the
    //"not yet polled" signal; once it matches, one completed tick (>= 1) is
    //enough, because every layer (triggers included) ran on this phase in it.
    //Suite-driven seats are exempt: the harness pump is not a held key, and its
    //back-to-back `next` commands must land on the tick they are issued (an
    //end-of-script assert follows immediately - a one-tick refusal under CPU
    //contention read as "phase one step short", lexicon/basic_legendarylandwalk).
    const bool scriptedSeat = mSuiteGame || mLoading
        || (currentPlayer && currentPlayer->playMode == Player::MODE_TEST_SUITE);
    if (allowInterrupt && !scriptedSeat && !(mSettledPhase == mCurrentGamePhase && mSettledTurn == turn
        && mSettledStep == (int) combatStep && mPhaseTicks >= 1))
        return;

    if(getCurrentTargetChooser() && getCurrentTargetChooser()->maxtargets == 1000)
    {
        getCurrentTargetChooser()->done = true;
        getCurrentTargetChooser()->autoChoice = false;
        if(getCurrentTargetChooser()->source)
            cardClick(getCurrentTargetChooser()->source, 0, false);
    }
    if (allowInterrupt && mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED))
        return;
    if (getCurrentTargetChooser())
        return;
    //if (mLayers->actionLayer()->isWaitingForAnswer())
    //    return;
    // Wil 12/5/10: additional check, not quite understanding why TargetChooser doesn't seem active at this point.
    // If we deem that an extra cost payment needs to be made, don't allow the next game phase to proceed.
    // Here's what I find weird - if the extra cost is something like a sacrifice, doesn't that imply a TargetChooser?
    if (WaitForExtraPayment(NULL)) 
        return;
    //W43-1 (CR 509.1c): the declare-blockers step is not ACCEPTED while the
    //declaration is illegal. A menace attacker blocked by exactly one creature
    //(or a "three or more" attacker blocked by two) is not a legal set, and the
    //engine used to accept it and then silently delete the blockers after the
    //fact - the option was offered, the narration said it happened, and nothing
    //ever told the player otherwise. Refusing the advance is this codebase's
    //own idiom for an illegal act (the target-chooser and extra-payment gates
    //directly above do exactly this); the human always has both repairs in
    //hand, since clicking the assigned blocker removes it and clicking another
    //creature adds the second one.
    //HUMAN ONLY, deliberately: an AI seat that somehow still built an illegal
    //set would spin here forever, so their declarations are made legal at the
    //source (AIPlayerBaka's sweep, AIPlayerGPT's validator) and the rules-layer
    //net stays behind them.
    if (mCurrentGamePhase == MTG_PHASE_COMBATBLOCKERS && combatStep == BLOCKERS
        && !isInterrupting && opponent() && !opponent()->isAI()
        && LegalActionsOracle::illegalBlockDeclaration(opponent()))
        return;
    /*if (OpenedDisplay)//dont let us fly through all the phases with grave and library box still open.
    {
        return;//I want this here, but it locks up on opponents turn, we need to come up with a clever way to close opened
        //displays, it makes no sense that you travel through 4 or 5 phases with library or grave still open.
    }*/
    Phase * cPhaseOld = phaseRing->getCurrentPhase();
    //Priority at phase boundaries (auto-pass with stops): a window is
    //created when the step is a forced stop (combat sub-steps, the
    //per-phase interrupt option, the per-player stop phase) or when the
    //NON-acting player could actually respond right now
    //(LegalActionsOracle::hasInstantResponse). Everyone else auto-passes
    //and the phase advances directly - no window, no wasted decision.
    //(Replaces the old blanket opponent()->isAI() window.)
    Player * phaseResponder = (currentlyActing() == players[0]) ? players[1] : players[0];
    //Record the request only once every refusal gate above has passed: a
    //request the engine did not act on (stack busy, chooser open, illegal
    //blockers) used to be logged too, and a replay could not tell "refused,
    //then repeated" from "advanced" - the recorded intents must be exactly
    //the effective ones for a transcript to replay.
    if(log) {
        stringstream stream;
        stream << "next " << allowInterrupt << " " <<mCurrentGamePhase;
        logAction(currentPlayer, stream.str());
    }
    if (allowInterrupt && ((cPhaseOld->id == MTG_PHASE_COMBATBLOCKERS && combatStep == ORDER)
        || (cPhaseOld->id == MTG_PHASE_COMBATBLOCKERS && combatStep == TRIGGERS)
        || (cPhaseOld->id == MTG_PHASE_COMBATDAMAGE)
        || options[Options::optionInterrupt(mCurrentGamePhase)].number
        || currentPlayer->offerInterruptOnPhase - 1 == mCurrentGamePhase
        || LegalActionsOracle::hasInstantResponse(phaseResponder)
    ))
    {
        mLayers->stackLayer()->AddNextGamePhase();
    }
    else
    {
       nextGamePhase(); 
    }
}

void GameObserver::shuffleLibrary(Player* p)
{
    if(!p)
    {
        DebugTrace("FATAL: No Player To Shuffle");
        return;
    }
    logAction(p, "shufflelib");
    MTGLibrary * library = p->game->library;
    if(!library)
    {
        DebugTrace("FATAL: Player has no zones");
        return;
    }
    library->shuffle();

    for(unsigned int k = 0;k < library->placeOnTop.size();k++)
    {
        MTGCardInstance * toMove = library->placeOnTop[k];
        assert(toMove);
        p->game->putInZone(toMove,  p->game->temp, library);
    }
    library->placeOnTop.clear();

}


int GameObserver::forceShuffleLibraries()
{
    int result = 0;
    for (int i = 0; i < 2 ; ++i)
    {
        if (players[i]->game->library->needShuffle)
        {
            shuffleLibrary(players[i]);
            players[i]->game->library->needShuffle = false;
            ++result;
        }
    }

    return result;
}

void GameObserver::resetStartupGame()
{
    stringstream stream;
    startupGameSerialized = "";
    stream << *this;
    startupGameSerialized = stream.str();
#ifdef WAGIC_TRANSCRIPT_ON
    if (mTranscriptPath.size())
    {
        writeTranscript("rebaseline");
        return;
    }
    mTranscriptNotes = "";
    mGameEndNoted = false;
    mGameEndMemlogged = false;
    //Suite / PARSETEST processes never keep a transcript: mSuiteGame is set
    //AFTER the first baseline on the main-thread path, and 51 junk
    //"testsuite-vs-testsuite" files had piled up in User/transcripts.
    if (!mSuiteGame && !mLoading && !getenv("WAGIC_REPLAY") && !getenv("WAGIC_TESTSUITE") && !getenv("WAGIC_GPT_PARSETEST")
        && players.size() == 2 && players[0] && players[1])
    {
        string names[2];
        for (int i = 0; i < 2; i++)
        {
            string f = players[i]->deckFile;
            size_t dot = f.rfind(".txt");
            if (dot != string::npos) f = f.substr(0, dot);
            for (size_t k = 0; k < f.size(); k++)
                if (f[k] == '/' || f[k] == '\\' || f[k] == ':' || f[k] == ' ') f[k] = '_';
            if (f.empty()) f = players[i]->isAI() ? "ai" : "human";
            names[i] = f;
        }
        std::stringstream p;
#ifdef VITA
        sceIoMkdir("ux0:data/Wagic/transcripts", 0777);
        p << "ux0:data/Wagic/transcripts/";
#else
        mkdir("User/transcripts", 0755);
        p << "User/transcripts/";
#endif
        p << time(0) << "-" << names[0] << "-vs-" << names[1] << ".txt";
        mTranscriptPath = p.str();
        writeTranscript("start");
    }
#endif // WAGIC_TRANSCRIPT_ON
//    DebugTrace("startGame\n");
//    DebugTrace(startupGameSerialized);
}

//The whole game as a replayable dump (operator<<: seed, rand values, the
//startup snapshot, every recorded action) rewritten in place - a crash
//mid-game still leaves the previous untap's record. Notes (#result,
//#classification) ride after [end] as comments load() skips.
void GameObserver::writeTranscript(const char * tag)
{
#ifdef WAGIC_TRANSCRIPT_ON
    if (mTranscriptPath.empty() || mLoading) return;
    std::stringstream ss;
    ss << "#transcript " << tag << " turn=" << turn
       << " life=" << (players[0] ? players[0]->life : 0) << "/" << (players[1] ? players[1]->life : 0) << "\n";
    ss << *this;
    ss << mTranscriptNotes;
    FILE * fp = fopen(mTranscriptPath.c_str(), "wb");
    if (!fp) return;
    const string & out = ss.str();
    fwrite(out.data(), 1, out.size(), fp);
    fclose(fp);
#else
    (void) tag;
#endif
}

void GameObserver::appendTranscriptNote(const string & note)
{
#ifdef WAGIC_TRANSCRIPT_ON
    if (mTranscriptPath.empty()) return;
    mTranscriptNotes += "#" + note + "\n";
    writeTranscript("note");
#else
    (void) note;
#endif
}

#if defined(WAGIC_MEMPROBE) && defined(PSP)
#include <pspkernel.h>
#include <stdio.h>
//startGame stage timer: appends to the menuprobe log; delta since previous
//sg mark. Built for the 2026-08-07 docket item (startGame 1.7-2.8s).
void sgMark(const char * tag)
{
    static unsigned int last = 0;
    unsigned int now = sceKernelGetSystemTimeLow();
    FILE * f = fopen("User/wagic-menuprobe.log", "a");
    if (f)
    {
        unsigned int d = last ? now - last : 0;
        fprintf(f, "+%6u.%03ums   sg %s\n", d / 1000, d % 1000, tag);
        fclose(f);
    }
    last = sceKernelGetSystemTimeLow();
}
#else
#define sgMark(x) ((void)0)
#endif

void GameObserver::startGame(GameType gtype, Rules * rules)
{
    sgMark("begin");
    mGameType = gtype;
#if defined(VITA) && defined(WAGIC_VITAMEMLOG)
    vitaMemProbe("gamestart", 0);
#endif
    turn = 0;
    mRules = rules;
    if (rules) 
        rules->initPlayers(this);
    sgMark("initPlayers");

    options.automaticStyle(players[0], players[1]);

    mLayers = NEW DuelLayers(this);
    sgMark("DuelLayers");

    currentPlayerId = 0;
    currentPlayer = players[currentPlayerId];
    currentActionPlayer = currentPlayer;
    phaseRing = NEW PhaseRing(this);
    sgMark("phaseRing");

    resetStartupGame();
    sgMark("serialize");

    if (rules) 
        rules->initGame(this);
    sgMark("initGame");

    //CR pre-game procedure (opening hands + London mulligan + 103.6
    //actions) runs before turn 1 of real/selfplay/demo games. Suite games
    //seed hands via INIT and assume the old start, so they skip it; story
    //mode and any test-suite seat skip it too.
    mPregame = NULL;
    mPregameDone = false;
    if (!mSuiteGame && gtype != GAME_TYPE_STORY
        && players.size() >= 2
        && players[0]->playMode != Player::MODE_TEST_SUITE
        && players[1]->playMode != Player::MODE_TEST_SUITE)
    {
        mPregame = NEW PreGamePhase(this);
    }

    //Preload images from hand
    if (!players[0]->isAI())
    {
        for (int i = 0; i < players[0]->game->hand->nb_cards; i++)
        {
            WResourceManager::Instance()->RetrieveCard(players[0]->game->hand->cards[i], CACHE_THUMB);
            WResourceManager::Instance()->RetrieveCard(players[0]->game->hand->cards[i]);
        }
     }
    sgMark("hand preload");

    startedAt = time(0);

    //Difficult mode special stuff
    if (!players[0]->isAI() && players[1]->isAI())
    {
        int difficulty = options[Options::DIFFICULTY].number;
        if (options[Options::DIFFICULTY_MODE_UNLOCKED].number && difficulty)
        {
            Player * p = players[1];
            for (int level = 0; level < difficulty; level++)
            {
                MTGCardInstance * card = NULL;
                MTGGameZone * z = p->game->library;
                for (int j = 0; j < z->nb_cards; j++)
                {
                    MTGCardInstance * _card = z->cards[j];
                    if (_card->isLand())
                    {
                        card = _card;
                        j = z->nb_cards;
                    }
                }
                if (card)
                {
                    MTGCardInstance * copy = p->game->putInZone(card, p->game->library, p->game->stack);
                    Spell * spell = NEW Spell(this, copy);
                    spell->resolve();
                    delete spell;
                }
            }
        }
    }

    switch(gtype) {
        case GAME_TYPE_MOMIR:
        {
            addObserver(NEW MTGMomirRule(this, -1, MTGCollection()));
            break;
        }
        case GAME_TYPE_STONEHEWER:
        {
            addObserver(NEW MTGStoneHewerRule(this, -1,MTGCollection()));
            break;
        }
        case GAME_TYPE_HERMIT:
        {
            addObserver(NEW MTGHermitRule(this, -1));
            break;
        }
        default:
            break;
    }
}

void GameObserver::addObserver(MTGAbility * observer)
{
    mLayers->actionLayer()->Add(observer);
    bumpAbilityEpoch(); //#W54-H (A6b): a new ability is a state change
}

//Returns true if the Ability was correctly removed from the game, false otherwise
//Main (valid) reason of returning false is an attempt at removing an Ability that has already been removed
MTGCardInstance * GameObserver::validateCardPointer(MTGCardInstance * card)
{
    if (!card)
        return NULL;
    for (int i = 0; i < 2; i++)
    {
        MTGPlayerCards * pz = players[i]->game;
        MTGGameZone * zones[] = { pz->hand, pz->library, pz->inPlay, pz->graveyard,
                                  pz->stack, pz->exile, pz->commandzone, pz->sideboard,
                                  pz->reveal, pz->garbage, pz->garbageLastTurn, pz->temp };
        for (int j = 0; j < 12; j++)
            for (int k = 0; k < zones[j]->nb_cards; k++)
                if (zones[j]->cards[k] == card)
                    return card;
    }
    return NULL;
}

bool GameObserver::removeObserver(ActionElement * observer)
{
    if (!observer)
        return false;
    return mLayers->actionLayer()->moveToGarbage(observer);

}

bool GameObserver::operator==(const GameObserver& aGame)
{
    int error = 0;

    if (aGame.mCurrentGamePhase != mCurrentGamePhase)
    {
        error++;
    }
    for (int i = 0; i < 2; i++)
    {
        Player * p = aGame.players[i];

        if (p->life != players[i]->life)
        {
            error++;
        }
        if (p->poisonCount != players[i]->poisonCount)
        {
            error++;
        }
        if (!p->getManaPool()->canAfford(players[i]->getManaPool(),0))
        {
            error++;
        }
        if (!players[i]->getManaPool()->canAfford(p->getManaPool(),0))
        {
            error++;
        }
        MTGGameZone * aZones[] = { p->game->graveyard, p->game->library, p->game->hand, p->game->inPlay, p->game->exile, p->game->commandzone, p->game->sideboard };
        MTGGameZone * thisZones[] = { players[i]->game->graveyard,
                                         players[i]->game->library,
                                         players[i]->game->hand,
                                         players[i]->game->inPlay,
                                         players[i]->game->exile,
                                         players[i]->game->commandzone,
                                         players[i]->game->sideboard };
        for (int j = 0; j < 7; j++)
        {
            MTGGameZone * zone = aZones[j];
            if (zone->nb_cards != thisZones[j]->nb_cards)
            {
                error++;
            }
            for (size_t k = 0; k < (size_t)thisZones[j]->nb_cards; k++)
            {
                MTGCardInstance* cardToCheck = (k<thisZones[j]->cards.size())?thisZones[j]->cards[k]:0;
                MTGCardInstance* card = (k<aZones[j]->cards.size())?aZones[j]->cards[k]:0;
                if(!card || !cardToCheck || cardToCheck->getId() != card->getId())
                {
                    error++;
                }
            }
        }
    }

    return (error == 0);
}

void GameObserver::dumpAssert(bool val)
{
    if(!val)
    {
        cerr << *this << endl;
        assert(0);
    }
}


void GameObserver::Update(float dt)
{
    //Pre-game phase gate: while it runs, the normal game loop is
    //suppressed (the board still renders via GameObserver::Render). On
    //completion the phase is torn down and mPregameDone latches, which
    //closes the old in-game mulligan/leyline paths.
    if (mPregame)
    {
        mPregame->Update(dt);
        if (mPregame->isDone())
        {
            SAFE_DELETE(mPregame);
            mPregameDone = true;
            //Re-baseline the replay record here: the pre-game (opening
            //hands, mulligans, leylines) is UI/AI-driven and unrecorded, so
            //a replay from the pre-shuffle snapshot cannot reproduce it.
            //From this point the dump = dealt state + rand values + clicks.
            if (!mLoading)
            {
                mSnapshotPostPregame = true;
                actionsList.clear();
                randomGenerator.loadRandValues("");
                resetStartupGame();
            }
        }
        return;
    }
    Player * player = currentPlayer;
    if (MTG_PHASE_COMBATBLOCKERS == mCurrentGamePhase && BLOCKERS == combatStep)
    {
        player = player->opponent();
    }
    if(getCurrentTargetChooser() && getCurrentTargetChooser()->Owner && player != getCurrentTargetChooser()->Owner)
    {
        if(getCurrentTargetChooser()->Owner != currentlyActing())
        {
            player = getCurrentTargetChooser()->Owner;
            isInterrupting = player;
        }
    }
    currentActionPlayer = player;
    if (isInterrupting) 
        player = isInterrupting;
#if defined(VITA) && defined(WAGIC_VITAMEMLOG)
    //Publish the counts that scale a frame, for the telemetry lines the main
    //loop writes. Four container sizes - no walk, no allocation.
    vitaFrameCounts(turn,
                    (players[0] && players[0]->game) ? players[0]->game->inPlay->nb_cards : 0,
                    (players[1] && players[1]->game) ? players[1]->game->inPlay->nb_cards : 0,
                    (players[0] && players[0]->game) ? players[0]->game->hand->nb_cards : 0,
                    mLayers ? (int) mLayers->actionLayer()->mObjects.size() : 0);
#endif
    if(mLayers)
    {
        if (oldGamePhase != mCurrentGamePhase)
            bumpAbilityEpoch(); //#W54-H (A6b)
        mLayers->Update(dt, player);
        while (mLayers->actionLayer()->stuffHappened)
        {
            mLayers->actionLayer()->Update(0);
        }
        gameStateBasedEffects();
    }
    oldGamePhase = mCurrentGamePhase;
}

//applies damage to creatures after updates
//Players life test
//Handles game state based effects
void GameObserver::gameStateBasedEffects()
{
    if(getCurrentTargetChooser() && int(getCurrentTargetChooser()->getNbTargets()) == getCurrentTargetChooser()->maxtargets){
        getCurrentTargetChooser()->done = true;
        getCurrentTargetChooser()->autoChoice = false;
    }
    /////////////////////////////////////
    for (int d = 0; d < 2; d++)
    {
        ////check snow count
        if (players[d]->snowManaC > players[d]->getManaPool()->getCost(0) + players[d]->getManaPool()->getCost(6))
            players[d]->snowManaC = players[d]->getManaPool()->getCost(0) + players[d]->getManaPool()->getCost(6);
        if (players[d]->snowManaC < 0)
            players[d]->snowManaC = 0;
        if (players[d]->snowManaG > players[d]->getManaPool()->getCost(1))
            players[d]->snowManaG = players[d]->getManaPool()->getCost(1);
        if (players[d]->snowManaG < 0)
            players[d]->snowManaG = 0;
        if (players[d]->snowManaU > players[d]->getManaPool()->getCost(2))
            players[d]->snowManaU = players[d]->getManaPool()->getCost(2);
        if (players[d]->snowManaU < 0)
            players[d]->snowManaU = 0;
        if (players[d]->snowManaR > players[d]->getManaPool()->getCost(3))
            players[d]->snowManaR = players[d]->getManaPool()->getCost(3);
        if (players[d]->snowManaR < 0)
            players[d]->snowManaR = 0;
        if (players[d]->snowManaB > players[d]->getManaPool()->getCost(4))
            players[d]->snowManaB = players[d]->getManaPool()->getCost(4);
        if (players[d]->snowManaB < 0)
            players[d]->snowManaB = 0;
        if (players[d]->snowManaW > players[d]->getManaPool()->getCost(5))
            players[d]->snowManaW = players[d]->getManaPool()->getCost(5);
        if (players[d]->snowManaW < 0)
            players[d]->snowManaW = 0;

        MTGGameZone * dzones[] = { players[d]->game->inPlay, players[d]->game->graveyard, players[d]->game->hand, players[d]->game->library, players[d]->game->exile, players[d]->game->stack, players[d]->game->commandzone, players[d]->game->sideboard, players[d]->game->reveal };
        for (int k = 0; k < 9; k++)
        {
            MTGGameZone * zone = dzones[k];
            if (mLayers->stackLayer()->count(0, NOT_RESOLVED) == 0)
            {
                for (int c = zone->nb_cards - 1; c >= 0; c--)
                {
                    zone->cards[c]->forcedBorderA = 0;
                    zone->cards[c]->forcedBorderB = 0;
                }
            }

            ///while checking all these zones, lets also strip devoid cards of thier colors
            for (int w = 0; w < zone->nb_cards; w++)
            {
                MTGCardInstance * card = zone->cards[w];
                for (int i = Constants::MTG_COLOR_GREEN; i <= Constants::MTG_COLOR_WHITE; ++i)
                {
                    if (card->has(Constants::DEVOID))
                    {
                        card->removeColor(i);
                    }
                }
                //clear prey
                if(card && isInExile(card) && card->hauntedCard)
                {
                    if(!isInPlay(card->hauntedCard)) 
                    {
                        card->hauntedCard = 0;
                    }
                }
                //reset alternate paid
                if(card && (isInGrave(card)||isInHand(card)||isInExile(card)))
                {
                    for (int i = 0; i < ManaCost::MANA_PAID_WITH_BESTOW +1; i++)
                        card->alternateCostPaid[i] = 0;
                }
                //test zone position
                if(card && (isInGrave(card)||isInHand(card)||isInExile(card)))
                {
                    card->zpos = w+1;
                }
                else if(card && (isInLibrary(card)))
                {//invert so we get the top one...
                    int onum = w+1;
                    card->zpos = abs(onum - zone->nb_cards)+1;
                }
                //last controller override
                if(card && zone->owner)
                    card->lastController = zone->owner;
            }


        }//check for losers if its GAMEOVER clear the stack to allow gamestateeffects to continue
        players[d]->DeadLifeState();
    }
    ////////////////////////////////////
    //Loss by life/poison is decision-independent: do not let the pending-
    //menu / target-chooser / reveal early-returns below starve the
    //check=true pass further down - a lifeleech kill that resolved into
    //an armed ask left the loser at -10 life issuing decisions for turns
    //(wave-4 corpus, deck140v133 run 230636 seq58-61). DeadLifeState is
    //idempotent; the full state-effects pass still runs on clean ticks.
    for (int i = 0; i < 2; i++)
        players[i]->DeadLifeState(true);
    //i think this must be limited to reveal display only but we can make an auto close like on android after a targetchooser...
    //lets see so far... adding this fixes some cards that rely on card count in hand or library or any zone the needs constant card count...
    if (OpenedDisplay && (players[0]->game->reveal->cards.size() || players[1]->game->reveal->cards.size()))
        return;
    if (mLayers->stackLayer()->count(0, NOT_RESOLVED) != 0)
        return;
    if (mLayers->actionLayer()->menuObject) 
        return;
    if (getCurrentTargetChooser() || mLayers->actionLayer()->isWaitingForAnswer()) 
        return;
    ////////////////////////
    //---apply damage-----//
    //after combat effects//
    ////////////////////////
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * zone = players[i]->game->inPlay;
        for (int j = zone->nb_cards - 1; j >= 0; j--)
        {
            MTGCardInstance * card = zone->cards[j];
            card->entersBattlefield = 0;
            card->LKIpower = card->power;
            card->LKItoughness = card->toughness;
            card->LKIbasicAbilities = card->basicAbilities;
            card->afterDamage();
            card->mPropertiesChangedSinceLastUpdate = false;
            if(card->hasType(Subtypes::TYPE_PLANESWALKER) && (!card->counters||!card->counters->hasCounter("loyalty",0,0)))
                players[i]->game->putInGraveyard(card);
            if(card->hasType(Subtypes::TYPE_BATTLE) && (!card->counters||!card->counters->hasCounter("defense",0,0))){
                if(!card->isDefeated){
                    card->isDefeated = true;
                    WEvent * e = NEW WEventCardDefeated(card);
                    receiveEvent(e);
                }
            }
            if(!card->isCreature() && card->hasType(Subtypes::TYPE_PLANESWALKER) && card->counters->hasCounter("loyalty", 0, 0))
                card->life = card->counters->hasCounter("loyalty", 0, 0)->nb;
            if(!card->isCreature() && card->hasType(Subtypes::TYPE_BATTLE) && card->counters->hasCounter("defense", 0, 0))
                card->life = card->counters->hasCounter("defense", 0, 0)->nb;
            if(card->myPair && !isInPlay(card->myPair))
            {
                card->myPair->myPair = NULL;
                card->myPair = NULL;
            }
            ///clear imprints
            if(isInPlay(card) && card->imprintedCards.size())
            {
                //#W54-I (L18): walk backwards - erasing inside a forward loop skipped the next entry
                for(size_t ic = card->imprintedCards.size(); ic-- > 0;)
                {
                    if(!isInExile(card->imprintedCards[ic])) 
                    {
                        card->imprintG = 0;
                        card->imprintU = 0;
                        card->imprintR = 0;
                        card->imprintB = 0;
                        card->imprintW = 0;
                        card->currentimprintName = "";
                        card->imprintedNames.clear();
                        card->imprintedCards.erase(card->imprintedCards.begin() + ic);
                    }
                }
            }
            card->bypassTC = false; //turn off bypass
            ///////////////////////////
            //reset extracost shadows//
            ///////////////////////////
            card->isExtraCostTarget = false;
            if (mExtraPayment != NULL)
            {
                for (unsigned int ec = 0; ec < mExtraPayment->costs.size(); ec++)
                {

                    if (mExtraPayment->costs[ec]->tc)
                    {
                        vector<Targetable*>targetlist = mExtraPayment->costs[ec]->tc->getTargetsFrom();
                        for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
                        {
                            Targetable * cardMasked = *it;
                            dynamic_cast<MTGCardInstance*>(cardMasked)->isExtraCostTarget = true;
                        }

                    }
                }
            }
            ////////////////////////////////////////////////////
            //Unattach Equipments that dont have valid targets//
            ////////////////////////////////////////////////////
            if (card->hasType(Subtypes::TYPE_EQUIPMENT)||card->hasType("fortification"))
            {
                if(isInPlay(card))
                {
                    for (size_t i = 1; i < mLayers->actionLayer()->mObjects.size(); i++)
                    {
                        MTGAbility * a = ((MTGAbility *) mLayers->actionLayer()->mObjects[i]);
                        AEquip * eq = dynamic_cast<AEquip*> (a);
                        if (eq && eq->source == card)
                        {
                            if(card->target)//unattach equipments from cards that has protection from quality ex. protection from artifacts
                            {
                                if((card->target)->protectedAgainst(card)||card->isCreature()||(!card->target->isCreature()))
                                    ((AEquip*)a)->unequip();
                                else if((!card->target->isLand() && card->hasType("fortification")))
                                    ((AEquip*)a)->unequip();
                            }
                            if(card->controller() && !card->mutation && ((AEquip*)a)->getActionTc())
                                ((AEquip*)a)->getActionTc()->Owner = card->controller();
                            //fix for equip ability when the equipment changed controller... 
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////
            //Remove auras that don't have a valid target anymore//
            ///////////////////////////////////////////////////////
            if (card->target && !isInPlay(card->target) && card->isBestowed && card->hasType("aura"))
            {
                card->removeType("aura");
                card->addType("creature");
                card->target = NULL;
                card->isBestowed = false;
            }

            if ((card->target||card->playerTarget) && !card->hasType(Subtypes::TYPE_EQUIPMENT))
            {
                if(card->target && !isInPlay(card->target))
                    players[i]->game->putInGraveyard(card);
            }
            card->enchanted = false;
            if (card->target && isInPlay(card->target) && !card->hasType(Subtypes::TYPE_EQUIPMENT) && card->hasSubtype(Subtypes::TYPE_AURA))
            {
                card->target->enchanted = true;
            }

            //704.5n If an Aura is attached to an illegal object or player,
            //or is not attached to an object or player, that Aura is put into its owner’s graveyard.
            if (card->target && isInPlay(card->target) && !card->hasType(Subtypes::TYPE_EQUIPMENT) && card->hasSubtype(Subtypes::TYPE_AURA))
            {
                bool unattachB = (!card->target->isCreature() && card->isBestowed)?true:false;
                bool protectionfromQ = ((card->target)->protectedAgainst(card) && !card->has(Constants::AURAWARD))?true:false;
                int found = 0;
                string stypes = card->spellTargetType;
                if(stypes.size() && !card->hasType("curse"))
                {
                    if(stypes.find("artifact") != string::npos && card->target->hasType("artifact"))
                        found++;
                    if(stypes.find("creature") != string::npos && card->target->hasType("creature"))
                        found++;
                    if(stypes.find("enchantment") != string::npos && card->target->hasType("enchantment"))
                        found++;
                    if(stypes.find("land") != string::npos && card->target->hasType("land"))
                        found++;
                    if(stypes.find("planeswalker") != string::npos && card->target->hasType("planeswalker"))
                        found++;
                    if(stypes.find("battle") != string::npos && card->target->hasType("battle"))
                        found++;
                }

                if((!found || protectionfromQ) && !card->isBestowed)
                {
                    players[i]->game->putInGraveyard(card);
                }
                else if(card->isBestowed && (protectionfromQ || unattachB))
                {
                    card->removeType("aura");
                    card->addType("creature");
                    card->target = NULL;
                    card->isBestowed = false;
                }
                
            }
            //////////////////////
            //reset morph hiding//
            //////////////////////
            if((card->previous && card->previous->morphed && !card->turningOver) || (card->morphed && !card->turningOver))
            {
                card->morphed = true;
                card->isMorphed = true;
            }
            else
            {
                card->isMorphed = false;
                card->morphed = false;
            }
            //(phasing flips now happen synchronously in phasingPhase(), called
            //as the untap step begins - see nextGamePhase. Only the attachment
            //phase-sync below stays in this per-tick pass.)
            if (card->target && isInPlay(card->target) && (card->hasSubtype(Subtypes::TYPE_EQUIPMENT) || card->hasSubtype(Subtypes::TYPE_AURA)))
            {
                card->isPhased = card->target->isPhased;
                card->phasedTurn = card->target->phasedTurn;
                if(card->view && card->target->view)
                    card->view->alpha = card->target->view->alpha;
            }
            //////////////////////////  
            //forceDestroy over ride//
            //////////////////////////
            if(card->isInPlay(this))
            {
                card->graveEffects = false;
                card->exileEffects = false;
                card->commandZoneEffects = false;

                if(card->isCreature())
                {
                    if(card->life < 1 && !card->has(Constants::INDESTRUCTIBLE))
                        card->destroy();//manor gargoyle... recheck
                    if(card->toughness <= 0 && card->has(Constants::INDESTRUCTIBLE))
                        card->toGrave(true);// Fixed a bug when indestructible creatures have toughness = 0 (e.g. March of the Machines with manacost = 0 artifacts).
                }
            }

            if(card->childrenCards.size())
            {
                MTGCardInstance * check = NULL;
                MTGCardInstance * matched = NULL;
                sort(card->childrenCards.begin(),card->childrenCards.end());
                for(size_t wC = 0; wC < card->childrenCards.size();wC++)
                {
                    check = card->childrenCards[wC];
                    for(size_t wCC = 0; wCC < card->childrenCards.size();wCC++)
                    {
                        if(check->isInPlay(this))
                        {
                            if(check->getName() == card->childrenCards[wCC]->getName() && check != card->childrenCards[wCC])
                            {
                                card->isDualWielding = true;
                                matched = card->childrenCards[wCC];
                            }
                        }
                    }
                    if(matched)
                        wC = card->childrenCards.size();
                }
                if(!matched)
                    card->isDualWielding = false;
            }
        }
    }
    //-------------------------------------

    for (int i = 0; i < 2; i++)
    {
        ///////////////////////////////////////////////////////////
        //life checks/poison checks also checks cant win or lose.//
        ///////////////////////////////////////////////////////////
        players[i]->DeadLifeState(true);//refactored
    }
    //////////////////////////////////////////////////////
    //-------------card based states effects------------//
    //////////////////////////////////////////////////////
    //ie:cantcast; extra land; extra turn;no max hand;--//
    //////////////////////////////////////////////////////

    for (int i = 0; i < 2; i++)
    {
        //checks if a player has a card which has the stated ability in play.
        Player * p = players[i];
        MTGGameZone * z = players[i]->game->inPlay;
        //------------------------------
        if(z->hasAbility(Constants::NOMAXHAND)||p->opponent()->inPlay()->hasAbility(Constants::OPPNOMAXHAND))
            p->nomaxhandsize = true;
        else
            p->nomaxhandsize = false;
        //////////////////////////////////
        //clear will attack player or pw//
        //////////////////////////////////
        if (mCurrentGamePhase == MTG_PHASE_COMBATBLOCKERS)
        {
            for (int l = z->nb_cards - 1; l >= 0; l--)
            {
                MTGCardInstance * c = z->cards[l];
                if(c)
                {
                    c->willattackplayer = 0;
                    c->willattackpw = 0;
                }
            }
        }
        ///provoke clear///
        if (mCurrentGamePhase == MTG_PHASE_COMBATEND)
        {
            for (int l = z->nb_cards - 1; l >= 0; l--)
            {
                MTGCardInstance * c = z->cards[l];
                if(c)
                {
                    c->isProvoked = false;
                    c->ProvokeTarget = NULL;
                    c->Provoker = NULL;
                }
            }
        }
        /////////////////////////////////////////////////
        //handle end of turn effects while we're at it.//
        /////////////////////////////////////////////////
        if (mCurrentGamePhase == MTG_PHASE_ENDOFTURN+1)
        {
            for (int j = z->nb_cards - 1; j >= 0; j--)
            {
                MTGCardInstance * c = z->cards[j];

                if(!c)break;
                while (c->flanked)
                {
                    /////////////////////////////////
                    //undoes the flanking on a card//
                    /////////////////////////////////
                    c->power += 1;
                    c->addToToughness(1);
                    c->flanked -= 1;
                }
                c->fresh = 0;
                if(c->wasDealtDamage > 0 && c->isInPlay(this) && !c->has(Constants::NODAMAGEREMOVED)) // Added to avoid damage is removed from a card (e.g. "Patient Zero").
                    c->wasDealtDamage = 0;
                c->damageToController = 0;
                c->damageToOpponent = 0;
                c->combatdamageToOpponent = 0;
                c->damageToCreature = 0;
                c->isAttacking = NULL;
                c->isProvoked = false;
                c->ProvokeTarget = NULL;
                c->Provoker = NULL;
            }

            MTGGameZone * fg = p->game->graveyard;
            for (int k = 0; k < fg->nb_cards; k++)
            {
                MTGCardInstance * card = fg->cards[k];
                card->fresh = 0; // Remove fresh attribute to cards put in graveyard last turn
                card->discarded = false; // Remove discarded attribute to cards put in graveyard last turn
            }
            MTGGameZone * fe = p->game->exile;
            for (int k = 0; k < fe->nb_cards; k++)
            {
                MTGCardInstance * card = fe->cards[k];
                card->fresh = 0; // Remove fresh attribute to cards put in exile last turn
            }
            MTGGameZone * fh = p->game->hand;
            for (int k = 0; k < fh->nb_cards; k++)
            {
                MTGCardInstance * card = fh->cards[k];
                card->fresh = 0; // Remove fresh attribute to cards put in hand last turn
            }
            MTGGameZone * fc = p->game->commandzone;
            for (int k = 0; k < fc->nb_cards; k++)
            {
                MTGCardInstance * card = fc->cards[k];
                card->fresh = 0; // Remove fresh attribute to cards put in commandzone last turn
            }
            MTGGameZone * fl = p->game->library;
            for (int k = 0; k < fl->nb_cards; k++)
            {
                MTGCardInstance * card = fl->cards[k];
                card->fresh = 0; // Remove fresh attribute to cards put in library last turn
            }
            MTGGameZone * fs = p->game->sideboard;
            for (int k = 0; k < fs->nb_cards; k++)
            {
                MTGCardInstance * card = fs->cards[k];
                card->fresh = 0; // Remove fresh attribute to cards put in sideboard last turn
            }
        }
        // An empty battlefield cannot grant NOMAXHAND from the player's own side,
        // but the opponent may still grant OPPNOMAXHAND from THEIRS - do not
        // clear the flag in that case (else a beneficiary controlling no
        // permanents loses the protection). Consult the actual granting side.
        if (z->nb_cards == 0 && !p->opponent()->inPlay()->hasAbility(Constants::OPPNOMAXHAND))
        {
            p->nomaxhandsize = false;
        }
        //////////////////////////
        // Check auras on a card//
        //////////////////////////
        enchantmentStatus();
        /////////////////////////////
        // Check affinity on a card//
        //    plus modify costs    //
        /////////////////////////////
        Affinity();
        /////////////////////////////////////
        // Check colored statuses on cards //
        /////////////////////////////////////
        for(int w = 0;w < z->nb_cards;w++)
        {  
            int colored = 0;
            for (int i = Constants::MTG_COLOR_GREEN; i <= Constants::MTG_COLOR_WHITE; ++i)
            {
                if (z->cards[w]->hasColor(i))
                    ++colored;
            }
            z->cards[w]->isMultiColored = (colored > 1) ? 1 : 0;
        }
    }
    ///////////////////////////////////
    //phase based state effects------//
    ///////////////////////////////////
    if (combatStep == TRIGGERS)
    {
        if (!mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED) && !targetChooser
            && !mLayers->actionLayer()->isWaitingForAnswer()) 
            mLayers->stackLayer()->AddNextCombatStep();
    }

    //Auto skip Phases
    //W53-DELVER (owner Vita report 2026-08-28, reproduced live in the desktop
    //GUI on b2ce13e89: Delver of Secrets never triggered at upkeep unless an
    //instant sat in hand). Phase triggers POLL the current phase once per tick
    //(GenericTriggeredAbility::Update), and the skips below used to fire in the
    //SAME tick the phase was entered (the stack resolves NextGamePhase into
    //Upkeep in mLayers->Update, this function then advanced to Draw) - so no
    //trigger ever saw the upkeep. An instant in hand merely kept the
    //no-legal-action skip from firing, which is why it "worked" then. Every
    //automation skip now waits for a phase that the PREVIOUS tick already saw.
    //mPhaseTicks counts the ticks this phase/step has been stable at the END of
    //a full Update: 0 = entered during this tick (no trigger has polled it yet).
    if (mSettledPhase == mCurrentGamePhase && mSettledTurn == turn && mSettledStep == (int) combatStep)
        mPhaseTicks++;
    else
        mPhaseTicks = 0;
    const bool phaseSettled = mPhaseTicks >= 1;
    mSettledPhase = mCurrentGamePhase;
    mSettledTurn = turn;
    mSettledStep = (int) combatStep;
    //A live-game transcript replay (post-pre-game dump) keeps the automation
    //the live game had; only suite/undo loads run with it off.
    //#W55-REPLAY: a WAGIC_REPLAY of a HUMAN game must not auto-advance either -
    //the live human seat advanced only by its own `next` presses, every one of
    //which is recorded; automation here skipped phases the record then could
    //not re-enter (the 2026-09-02 Vita transcript diverged at action 0).
    const bool loadingScripted = mLoading && (!mSnapshotPostPregame || getenv("WAGIC_REPLAY") != NULL);
    int skipLevel = (!phaseSettled || currentPlayer->playMode == Player::MODE_TEST_SUITE || mSuiteGame || loadingScripted) ? Constants::ASKIP_NONE
        : options[Options::ASPHASES].number;
    bool noattackers = currentPlayer->noPossibleAttackers();
    bool nodiaochan = (currentPlayer->game->battlefield->countByAlias(10544)<1)?true:false;
    if (skipLevel == Constants::ASKIP_SAFE || skipLevel == Constants::ASKIP_FULL)
    {
        if ((opponent()->isAI() && !(isInterrupting)) && ((mCurrentGamePhase == MTG_PHASE_UNTAP)
            || (mCurrentGamePhase == MTG_PHASE_DRAW) 
            || ((mCurrentGamePhase == MTG_PHASE_COMBATBEGIN) && (nodiaochan))
            || ((mCurrentGamePhase == MTG_PHASE_COMBATATTACKERS) && (noattackers))
            || (mCurrentGamePhase == MTG_PHASE_COMBATEND) 
            || (mCurrentGamePhase == MTG_PHASE_ENDOFTURN)
            || ((mCurrentGamePhase == MTG_PHASE_CLEANUP) && (currentPlayer->game->hand->nb_cards < 8))))
            userRequestNextGamePhase();
    }
    //W3b: the defending HUMAN's empty blockers step auto-advances - no
    //legal block exists, so there is no declaration to make. Gated on a
    //settled stack (attack triggers still resolving keep the step alive)
    //and same skip levels as the attacker-side skips above. The advance
    //goes through userRequestNextGamePhase, so a priority window still
    //opens when the defender holds an instant-speed response.
    if ((skipLevel == Constants::ASKIP_SAFE || skipLevel == Constants::ASKIP_FULL)
        && mCurrentGamePhase == MTG_PHASE_COMBATBLOCKERS && combatStep == BLOCKERS
        && currentPlayer->isAI() && !opponent()->isAI() && !isInterrupting
        && !mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED)
        && !mLayers->actionLayer()->menuObject && !targetChooser
        && currentPlayer->game->inPlay->getNextAttacker(NULL)
        && !LegalActionsOracle::hasLegalBlock(opponent()))
        userRequestNextGamePhase();

    if (skipLevel == Constants::ASKIP_FULL)
    {
        if ((opponent()->isAI() && !(isInterrupting)) && !humanDisplayOpen() && (mCurrentGamePhase == MTG_PHASE_UPKEEP
            || mCurrentGamePhase == MTG_PHASE_COMBATDAMAGE))
            userRequestNextGamePhase();
    }

    //A window where nothing is possible is not a window. If the human seat has
    //no legal action in this phase, advance REGARDLESS of the ASPHASES posture -
    //including ASKIP_NONE. The two mechanisms are deliberately orthogonal:
    //ASPHASES is a userland knob governing stops where the player COULD act,
    //and this rule removes only the stops where they could not. Folding this
    //into a skip level instead would make one level quietly mean another.
    //
    //Deliberately biased toward stopping: hasAnyLegalAction answers true when
    //unsure, because a wrongly-skipped window can lose a game while a spurious
    //stop costs one keypress.
    //
    //Automation is off entirely for suite and loading games - they encode exact
    //phase cadences that any skip would drift - which is the same reason
    //skipLevel is forced to ASKIP_NONE for them above.
    //Settled-stack guard as in the empty-blockers skip: a trigger still
    //resolving has to keep the window open.
    //mSuiteHumanSeat (the `realgame` fixture directive) deliberately
    //re-enables the automation for a scripted seat - that is the whole
    //point of the directive; it is NULL in every ordinary game.
    const bool automationAllowed = phaseSettled && (mSuiteHumanSeat
        || !(currentPlayer->playMode == Player::MODE_TEST_SUITE
             || mSuiteGame || (mLoading && (!mSnapshotPostPregame || getenv("WAGIC_REPLAY") != NULL))));
    Player * humanSeat = mSuiteHumanSeat ? mSuiteHumanSeat
        : (!players[0]->isAI() ? players[0] : (!players[1]->isAI() ? players[1] : NULL));
    //Only the human's OWN turn. userRequestNextGamePhase advances the phase
    //globally, so firing this on the opponent's turn would rip the phase out
    //from under the AI before its throttled Act got to play - every other skip
    //here is turn-gated for the same reason. The opponent's turn is already
    //covered by the reactive half: the priority window at userRequestNextGamePhase
    //only opens when the non-acting player can actually respond.
    //The turn gate above is not enough on its own: the AI's BLOCKER declaration
    //happens on the HUMAN's turn. During declare-blockers currentPlayer is the
    //attacking human, so with no instant in hand this skip would advance combat
    //before the defender's throttled Act ever declared a block - the AI was not
    //declining to block, its window was being skipped (live-observed on PSP,
    //2026-08-06: blocks present below this rule, absent above it). Hold whenever
    //the OTHER seat has a combat declaration due; pendingCombatDecision is the
    //engine's existing authority on exactly that question.
    Player * otherSeat = humanSeat ? ((humanSeat == players[0]) ? players[1] : players[0]) : NULL;
    //W53-DELVER (owner Vita report 2026-08-28: Delver "not checking top deck
    //multiple turns... works only once I have an instant castable at upkeep").
    //An upkeep trigger's reveal/look display (MTGRevealingCards -> OpenedDisplay)
    //opens AFTER its StackAbility has resolved, so the settled-stack guard is
    //already satisfied and this rule advanced the phase under the open display
    //whenever the hand held no instant. A display awaiting the player IS a
    //legal action pending: hold while any display is open.
    if (automationAllowed && humanSeat && currentPlayer == humanSeat && !isInterrupting
        && !mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED)
        && !mLayers->actionLayer()->menuObject && !targetChooser && !humanDisplayOpen()
        && (!otherSeat || pendingCombatDecision(otherSeat) == COMBAT_DECISION_NONE))
    {
        if (mNoActionTurn != turn || mNoActionPhase != mCurrentGamePhase
            || mNoActionStep != (int) combatStep)
        {
            mNoActionTurn = turn;
            mNoActionPhase = mCurrentGamePhase;
            mNoActionStep = (int) combatStep;
            mNoActionVerdict = LegalActionsOracle::hasAnyLegalAction(humanSeat);
        }
        if (!mNoActionVerdict)
            userRequestNextGamePhase();
    }

    this->LPWeffect = false;
    //WEventGameStateBasedChecked event checked
    receiveEvent(NEW WEventGameStateBasedChecked());
}

void GameObserver::enchantmentStatus()
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * zone = players[i]->game->inPlay;
        for (int k = zone->nb_cards - 1; k >= 0; k--)
        {
            MTGCardInstance * card = zone->cards[k];
            if (card && !card->hasType(Subtypes::TYPE_EQUIPMENT) && !card->hasSubtype(Subtypes::TYPE_AURA))
            {
                card->enchanted = false;
                card->auras = 0;
            }
        }
        for (int j = zone->nb_cards - 1; j >= 0; j--)
        {
            MTGCardInstance * card = zone->cards[j];
            if (card->target && isInPlay(card->target) && !card->hasType(Subtypes::TYPE_EQUIPMENT) && card->hasSubtype(Subtypes::TYPE_AURA))
            {
                card->target->enchanted = true;
                card->target->auras += 1;
                card->auraParent = card->target;
            }
        }
    }
}

void GameObserver::Affinity()
{
    for (int dd = 0; dd < 2; dd++)
    {
        MTGGameZone * dzones[] = { players[dd]->game->graveyard, players[dd]->game->hand, players[dd]->game->library, players[dd]->game->commandzone, players[dd]->game->exile };
        for (int kk = 0; kk < 5; kk++)
        { 
            MTGGameZone * zone = dzones[kk];
            for (int cc = zone->nb_cards - 1; cc >= 0; cc--)
            {//start
                MTGCardInstance * card = zone->cards[cc];
                if (!card || card->hasType(Subtypes::TYPE_DUNGEON) || card->hasType(Subtypes::TYPE_CONSPIRACY)) // Fix to avoid crash when the card is null or if is a Dungeon/Conspiracy in the commandzone.
                    continue;

                bool checkAuraP = false;
                ///////////////////////////
                //reset extracost shadows//
                ///////////////////////////
                card->isExtraCostTarget = false;
                if (mExtraPayment != NULL)
                {
                    for (unsigned int ec = 0; ec < mExtraPayment->costs.size(); ec++)
                    {

                        if (mExtraPayment->costs[ec]->tc)
                        {
                            vector<Targetable*>targetlist = mExtraPayment->costs[ec]->tc->getTargetsFrom();
                            for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
                            {
                                Targetable * cardMasked = *it;
                                dynamic_cast<MTGCardInstance*>(cardMasked)->isExtraCostTarget = true;
                            }

                        }
                    }
                }
                ///we handle trisnisphere seperately because its a desaster.
                if(card->getManaCost())//make sure we check, abiliy$!/token dont have a mancost object.
                {
                    if (card->controller()->AuraIncreased->getConvertedCost() || card->controller()->AuraReduced->getConvertedCost())
                        if(card->model->data->getManaCost()->getBestow())
                            checkAuraP = true;

                    //change cost to colorless for anytypeofmana ability (Obsolete code)
                    /*if(card->has(Constants::ANYTYPEOFMANA))
                    {
                        card->anymanareplacement = true;
                        int convertedC = card->getManaCost()->getConvertedCost();
                        card->getManaCost()->changeCostTo( NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, card)) );
                        for (int jj = 0; jj < convertedC; jj++)
                        {
                            card->getManaCost()->add(Constants::MTG_COLOR_ARTIFACT, 1);
                        }
                    }
                    else
                    {
                        if (card->anymanareplacement)
                        {
                            card->getManaCost()->changeCostTo( card->model->data->getManaCost() );
                            card->anymanareplacement = false;
                        }
                    }*/

                    if (card->has(Constants::TRINISPHERE))
                    {
                        for (int jj = card->getManaCost()->getConvertedCost(); jj < 3; jj++)
                        {
                            card->getManaCost()->add(Constants::MTG_COLOR_ARTIFACT, 1);
                            card->countTrini++;
                        }
                    }
                    else
                    {
                        if (card->countTrini)
                        {
                            card->getManaCost()->remove(Constants::MTG_COLOR_ARTIFACT, card->countTrini);
                            card->countTrini = 0;
                        }
                    }
                }
                ///////////////////////
                bool NewAffinityFound = false;
                //#W54-H (L15): the memo gates everything below; test it first.
                if (AffinityNeedsUpdate)
                for (unsigned int na = 0; na < card->cardsAbilities.size(); na++)
                {
                    if (!card->cardsAbilities[na])
                        break;
                    ANewAffinity * newAff = dynamic_cast<ANewAffinity*>(card->cardsAbilities[na]);
                    if (newAff)
                    {
                        NewAffinityFound = true;
                    }
                }
                bool DoReduceIncrease = false;
                if (
                    AffinityNeedsUpdate &&
                    (card->has(Constants::AFFINITYARTIFACTS) ||
                    card->has(Constants::AFFINITYENCHANTMENTS) ||
                    card->has(Constants::AFFINITYFOREST) ||
                    card->has(Constants::AFFINITYGREENCREATURES) ||
                    card->has(Constants::AFFINITYISLAND) ||
                    card->has(Constants::AFFINITYMOUNTAIN) ||
                    card->has(Constants::AFFINITYPLAINS) ||
                    card->has(Constants::AFFINITYSWAMP) ||
                    card->has(Constants::CONDUITED) ||
                    card->has(Constants::AFFINITYALLCREATURES) ||
                    card->has(Constants::AFFINITYCONTROLLERCREATURES) ||
                    card->has(Constants::AFFINITYOPPONENTCREATURES) ||
                    card->has(Constants::AFFINITYALLDEADCREATURES) ||
                    card->has(Constants::AFFINITYTWOALLDEADCREATURES) ||
                    card->has(Constants::AFFINITYPARTY) ||
                    card->has(Constants::AFFINITYBASICLANDTYPES) ||
                    card->has(Constants::AFFINITYTWOBASICLANDTYPES) ||
                    card->has(Constants::AFFINITYGRAVECREATURES) ||
                    card->has(Constants::AFFINITYATTACKINGCREATURES) ||
                    card->has(Constants::AFFINITYGRAVEINSTSORC) ||
                    card->getIncreasedManaCost()->getConvertedCost() ||
                    card->getReducedManaCost()->getConvertedCost() ||
                    NewAffinityFound || checkAuraP)
                    )
                    DoReduceIncrease = true;
                if (!DoReduceIncrease)
                    continue;

                if (mExtraPayment != NULL && card == mExtraPayment->source) // Fix to avoid crash when the card paying extracost has also a cost alteration (e.g. combo with "Pirate's Pillage" and "Ruby Medallion").
                    continue;

                //above we check if there are even any cards that effect cards manacost
                //only do any of the following if a card with the stated ability is in your hand.
                //kicker is an addon to normal cost, suspend is not casting. add cost as needed EXACTLY as seen below.
                card->getManaCost()->resetCosts();
                ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost(), card->model->data->getManaCost()));
                card->getManaCost()->changeCostTo(newCost);
                SAFE_DELETE(newCost);
                if (card->getManaCost()->getAlternative())
                {
                    card->getManaCost()->getAlternative()->resetCosts();
                    ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost()->getAlternative(), card->model->data->getManaCost()->getAlternative()));
                    card->getManaCost()->getAlternative()->changeCostTo(newCost);
                    SAFE_DELETE(newCost);
                }
                if (card->getManaCost()->getBestow())
                {
                    card->getManaCost()->getBestow()->resetCosts();
                    ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost()->getBestow(), card->model->data->getManaCost()->getBestow(),false,true));
                    card->getManaCost()->getBestow()->changeCostTo(newCost);
                    SAFE_DELETE(newCost);
                }
                if (card->getManaCost()->getBuyback())
                {
                    card->getManaCost()->getBuyback()->resetCosts();
                    ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost()->getBuyback(), card->model->data->getManaCost()->getBuyback()));
                    card->getManaCost()->getBuyback()->changeCostTo(newCost);
                    SAFE_DELETE(newCost);
                }
                if (card->getManaCost()->getFlashback())
                {
                    card->getManaCost()->getFlashback()->resetCosts();
                    ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost()->getFlashback(), card->model->data->getManaCost()->getFlashback()));
                    card->getManaCost()->getFlashback()->changeCostTo(newCost);
                    SAFE_DELETE(newCost);
                }
                if (card->getManaCost()->getMorph())
                {
                    card->getManaCost()->getMorph()->resetCosts();
                    ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost()->getMorph(), card->model->data->getManaCost()->getMorph()));
                    card->getManaCost()->getMorph()->changeCostTo(newCost);
                    SAFE_DELETE(newCost);
                }
                if (card->getManaCost()->getRetrace())
                {
                    card->getManaCost()->getRetrace()->resetCosts();
                    ManaCost *newCost = NEW ManaCost(card->computeNewCost(card, card->getManaCost()->getRetrace(), card->model->data->getManaCost()->getRetrace()));
                    card->getManaCost()->getRetrace()->changeCostTo(newCost);
                    SAFE_DELETE(newCost);
                }
                
            }//end
        }
    }
    AffinityNeedsUpdate = false;
}

void GameObserver::Render()
{
    if(mLayers)
        mLayers->Render();
    if (targetChooser || (mLayers && mLayers->actionLayer()->isWaitingForAnswer()))
        JRenderer::GetInstance()->DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ARGB(255,255,0,0));
    if (mExtraPayment) 
        mExtraPayment->Render();
    
    for (size_t i = 0; i < players.size(); ++i)
    {
        players[i]->Render();
    }
    if (mPregame)
        mPregame->Render();
}

void GameObserver::ButtonPressed(PlayGuiObject * target)
{
    DebugTrace("GAMEOBSERVER Click");
    if (CardView* cardview = dynamic_cast<CardView*>(target))
    {
        MTGCardInstance * card = cardview->getCard();
        cardClick(card, card);
    }
    else if (GuiLibrary* library = dynamic_cast<GuiLibrary*>(target))
    {
        if (library->showCards)
        {
            library->toggleDisplay();
            forceShuffleLibraries();
        }
        else
        {
            TargetChooser * _tc = this->getCurrentTargetChooser();
            if (_tc && _tc->targetsZone(library->zone))
            {
                library->toggleDisplay();
                library->zone->needShuffle = true;
            }
        }
    }
    else if (GuiGraveyard* graveyard = dynamic_cast<GuiGraveyard*>(target))
        graveyard->toggleDisplay();
    else if (GuiExile* exile = dynamic_cast<GuiExile*>(target))
        exile->toggleDisplay();
    else if (GuiCommandZone* commandzone = dynamic_cast<GuiCommandZone*>(target))
        commandzone->toggleDisplay();
    else if (GuiSideboard* sideboard = dynamic_cast<GuiSideboard*>(target))
        sideboard->toggleDisplay();
    //opponenthand
    else if (GuiOpponentHand* opponentHand = dynamic_cast<GuiOpponentHand*>(target))
        if (opponentHand->showCards)
        {
            opponentHand->toggleDisplay();
        }
        else
        {
            bool showopponenthand = (opponentHand->zone && opponentHand->zone->owner->opponent()->game->battlefield->nb_cards && opponentHand->zone->owner->opponent()->game->battlefield->hasAbility(Constants::SHOWOPPONENTHAND))?true:false;
#ifdef WITH_GPT_AI
            //Evaluation peek (config peek=1 / WAGIC_GPT_PEEK): open the AI's
            //hand on click without a Telepathy effect, so a human can judge
            //the choices the AI is picking from.
            if (gptPeekOpponentHand())
                showopponenthand = true;
#endif
            bool showcontrollerhand = (opponentHand->zone && opponentHand->zone->owner->game->battlefield->nb_cards && opponentHand->zone->owner->game->battlefield->hasAbility(Constants::SHOWCONTROLLERHAND))?true:false;
            TargetChooser * _tc = this->getCurrentTargetChooser();
            if ((_tc && _tc->targetsZone(opponentHand->zone)) || showopponenthand || showcontrollerhand)
            {
                opponentHand->toggleDisplay();
            }
        }
    //end opponenthand
    else if (GuiAvatar* avatar = dynamic_cast<GuiAvatar*>(target))
    {
        cardClick(NULL, avatar->player);
    }
    else if (dynamic_cast<GuiPhaseBar*>(target))
    {
        mLayers->getPhaseHandler()->NextGamePhase();
    }
}

void GameObserver::stackObjectClicked(Interruptible * action)
{
    stringstream stream;
    stream << "stack[" << mLayers->stackLayer()->getIndexOf(action) << "]";
    logAction(currentlyActing(), stream.str());

    if (targetChooser != NULL)
    {
        int result = targetChooser->toggleTarget(action);
        if (result == TARGET_OK_FULL)
        {
            cardClick(cardWaitingForTargets, 0, false);
        }
        else
        {
            return;
        }
    }
    else
    {
        int reaction = mLayers->actionLayer()->isReactingToTargetClick(action);
        if (reaction == -1) 
            mLayers->actionLayer()->reactToTargetClick(action);
    }
}

bool GameObserver::WaitForExtraPayment(MTGCardInstance * card)
{
    bool result = false;
    if (mExtraPayment)
    {
        if (card)
        {
            mExtraPayment->tryToSetPayment(card);
        }
        if (mExtraPayment->isPaymentSet())
        {
            mLayers->actionLayer()->reactToClick(mExtraPayment->action, mExtraPayment->source);
            mExtraPayment = NULL;
        }
        result = true;
        // Avoid game stucks on current phase till snow mana cost will be paid
        if(mExtraPayment && mExtraPayment->costs.size() == 1 && !strcmp(mExtraPayment->costs[0]->mCostRenderString.c_str(), "Snow Mana"))
            result = false;    
    }

    return result;
}

int GameObserver::cardClick(MTGCardInstance * card, MTGAbility *ability)
{
    MTGGameZone* zone = card->currentZone;
    size_t index  = 0;
    if(zone)
        index = zone->getIndex(card);
    int choice;
    bool logChoice = mLayers->actionLayer()->getMenuIdFromCardAbility(card, ability, choice);
    int result = ability->reactToClick(card);
    logAction(card, zone, index, result);
#ifdef WAGIC_TRANSCRIPT_ON
    if (getenv("WAGIC_TRANSCRIPT_TRACE") && zone)
    {
        std::stringstream tr;
        tr << "[transcript-trace] click " << (card->controller() == players[0] ? "p1." : "p2.") << zone->getName() << "[" << index << "] -> " << result << " " << card->getLCName() << " | zone now:";
        for (int i = 0; i < zone->nb_cards; i++) tr << " " << zone->cards[i]->getLCName();
        DebugTrace(tr.str());
    }
#endif

    if(logChoice) {
        stringstream stream;
        stream << "choice " << choice;
        logAction(currentActionPlayer, stream.str());
    }

    return result;
}

int GameObserver::cardClick(MTGCardInstance * card, int abilityType)
{
    int result = 0;
    MTGAbility * a = mLayers->actionLayer()->getAbility(abilityType);

    if(a)
    {
        result = cardClick(card, a);
    }

    return result;
}

int GameObserver::cardClickLog(bool log, Player* clickedPlayer, MTGGameZone* zone, MTGCardInstance*backup, size_t index, int toReturn)
{
    if(log)
    {
        if (clickedPlayer) {
            this->logAction(clickedPlayer);
        } else if(zone)  {
            this->logAction(backup, zone, index, toReturn);
#ifdef WAGIC_TRANSCRIPT_ON
            if (getenv("WAGIC_TRANSCRIPT_TRACE"))
            {
                std::stringstream tr;
                tr << "[transcript-trace] click " << (backup->controller() == players[0] ? "p1." : "p2.") << zone->getName() << "[" << index << "] -> " << toReturn << " " << backup->getLCName() << " | zone now:";
                for (int i = 0; i < zone->nb_cards; i++) tr << " " << zone->cards[i]->getLCName();
                DebugTrace(tr.str());
            }
#endif
        }
    }
    return toReturn;
}

int GameObserver::cardClick(MTGCardInstance * card, Targetable * object, bool log)
{
    Player * clickedPlayer = NULL;
    int toReturn = 0;
    int handmodified = 0;
    MTGGameZone* zone = NULL;
    size_t index = 0;
    MTGCardInstance* backup = NULL;

    if (!card) {
        clickedPlayer = ((Player *) object);
    } else {
        backup = card;
        zone = card->currentZone;
        if(zone)
        {
            index = zone->getIndex(card);
        }
    }

    do {
        if (targetChooser)
        {
            int result;
            if (card)
            {
                if (card == cardWaitingForTargets)
                {
                    int _result = targetChooser->ForceTargetListReady();
                    if(targetChooser->targetMin && int(targetChooser->getNbTargets()) < targetChooser->maxtargets)
                        _result = 0;

                    if (_result)
                    {
                        result = TARGET_OK_FULL;
                    }
                    else
                    {
                        result = targetChooser->targetsReadyCheck();
                    }
                }
                else
                {
                    result = targetChooser->toggleTarget(card);
                    WEvent * e = NEW WEventTarget(card,cardWaitingForTargets);
                    receiveEvent(e);
                }
            }
            else
            {
                result = targetChooser->toggleTarget(clickedPlayer);
                if(card)
                    card->playerTarget = clickedPlayer;
                else
                    targetChooser->source->playerTarget = clickedPlayer;
            }
            if (result == TARGET_OK_FULL)
                card = cardWaitingForTargets;
            else {
                toReturn = 1;
                break;
            }
        }
        ExtraManaCost * costType = NULL;
        if( mExtraPayment && mExtraPayment->costs.size())
            costType = dynamic_cast<ExtraManaCost*>(mExtraPayment->costs[0]);

        if (WaitForExtraPayment(card) && !costType) 
        {
            toReturn = 1;
            break;
        }

        int reaction = 0;

        if (ORDER == combatStep)
        {
            //Damage-assignment order is a turn-based action (CR 509.2-509.3):
            //no player has priority during the ordering interaction, so ALL
            //clicks are consumed here - clicks on a blocker reorder it,
            //anything else (hand cards included) is deliberately ignored.
            //The defenser guard fixes a NULL deref: this used to call
            //card->defenser->raiseBlockerRankOrder unconditionally, UB for
            //every non-blocker click that landed in this step.
            if (card && card->defenser)
                card->defenser->raiseBlockerRankOrder(card);
            toReturn = 1;
            break;
        }

        if (card)
        {
            //card played as normal, alternative cost, buyback, flashback, retrace.

            //the variable "paymenttype = int" only serves one purpose, to tell this bug fix what menu item you clicked on...
            // all alternative cost or play methods suffered from the fix because if the card contained "target="
            // it would automatically force the play method to putinplayrule...even charge you the original mana cost.

            /* Fix for Issue http://code.google.com/p/wagic/issues/detail?id=270
             put into play is hopefully the only ability causing that kind of trouble
             If the same kind of issue occurs with other abilities, let's think of a cleaner solution
             */
            if (targetChooser)
            {
                MTGAbility * a = mLayers->actionLayer()->getAbility(card->paymenttype);
                toReturn = a->reactToClick(card);
                return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
            }

            reaction = mLayers->actionLayer()->isReactingToClick(card);
            if (reaction == -1) {
                toReturn = mLayers->actionLayer()->reactToClick(card);
                return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
            }
        }
        else
        {//this handles abilities on a menu...not just when card is being played
            reaction = mLayers->actionLayer()->isReactingToTargetClick(object);
            if (reaction == -1) {
                toReturn = mLayers->actionLayer()->reactToTargetClick(object);
                return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
            }
        }

        if (!card) {
            toReturn = 0;
            return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
        }

        // CR 500.3 / 502.4 / 514.3: no player receives priority during the untap
        // step, and (simplified model) none during the cleanup step, so no spell
        // may be cast and no ability activated in either step. Suppress the
        // spell/ability reaction here. The mandatory cleanup hand-size discard
        // below is a turn-based action (not a priority action) and stays enabled;
        // the manual-untap fallthrough (reaction == 0) is unaffected.
        // SIMPLIFICATION: CR 514.3a would grant priority in the cleanup step when a
        // state-based action or a triggered ability is waiting; the engine has no
        // trigger-in-cleanup machinery to hang that exception on, so cleanup is
        // modeled as unconditionally priority-free.
        if (reaction && (mCurrentGamePhase == MTG_PHASE_UNTAP || mCurrentGamePhase == MTG_PHASE_CLEANUP))
            reaction = 0;

        //Current player's hand
        handmodified = currentPlayer->handsize+currentPlayer->handmodifier;
        if(handmodified < 0)
            handmodified = 0;
        if (currentPlayer->game->hand->hasCard(card) && mCurrentGamePhase == MTG_PHASE_CLEANUP
                    && currentPlayer->game->hand->nb_cards > handmodified && currentPlayer->nomaxhandsize == false)
        {
            //W50-W (D4): the event names the card actually discarded (it was
            //hand->cards[0], so a chosen discard narrated as the OLDEST card).
            WEvent * e = NEW WEventCardDiscard(card);
            receiveEvent(e);
            currentPlayer->game->putInGraveyard(card);
        }
        else if (reaction)
        {
            if (reaction == 1)
            {
                toReturn = mLayers->actionLayer()->reactToClick(card);
                return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
            }
            else
            {
                mLayers->actionLayer()->setMenuObject(object);
                toReturn = 1;
                return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
            }
        }
        else if (card->isTapped() && card->controller() == currentPlayer)
        {
            toReturn = untap(card);
            return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
        }
    } while(0);


    return cardClickLog(log, clickedPlayer, zone, backup, index, toReturn);
}

int GameObserver::untap(MTGCardInstance * card)
{
    if (!card->isUntapping())
    {
        return 0;
    }
    if (card->has(Constants::DOESNOTUNTAP))
        return 0;
    if (card->frozen > 0) 
        return 0;
    card->attemptUntap();
    return 1;
}

TargetChooser * GameObserver::getCurrentTargetChooser()
{
    if(mLayers)
    {
        TargetChooser * _tc = mLayers->actionLayer()->getCurrentTargetChooser();
        if (_tc)
            return _tc;
    }
    return targetChooser;
}

/* Returns true if the card is in one of the player's play zone */
int GameObserver::isInPlay(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        if (players[i]->game->isInPlay(card)) 
            return 1;
    }
    return 0;
}
int GameObserver::isInGrave(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * graveyard = players[i]->game->graveyard;
        if (players[i]->game->isInZone(card,graveyard)) 
            return 1;
    }
    return 0;
}
int GameObserver::isInExile(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * exile = players[i]->game->exile;
        if (players[i]->game->isInZone(card,exile)) 
            return 1;
    }
    return 0;
}
int GameObserver::isInCommandZone(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * commandzone = players[i]->game->commandzone;
        if (players[i]->game->isInZone(card,commandzone)) 
            return 1;
    }
    return 0;
}
int GameObserver::isInHand(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * hand = players[i]->game->hand;
        if (players[i]->game->isInZone(card, hand))
            return 1;
    }
    return 0;
}
int GameObserver::isInLibrary(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * library = players[i]->game->library;
        if (players[i]->game->isInZone(card, library))
            return 1;
    }
    return 0;
}
int GameObserver::isInStack(MTGCardInstance * card)
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * stack = players[i]->game->stack;
        if (players[i]->game->isInZone(card, stack))
            return 1;
    }
    return 0;
}
void GameObserver::cleanupPhase()
{
    currentPlayer->cleanupPhase();
    opponent()->cleanupPhase();
}

//Phasing flips exactly once per turn, synchronously as the untap step
//begins (CR 702.26) and BEFORE untapAll, so a card that phases in still
//untaps this turn. The old implementation polled mCurrentGamePhase ==
//MTG_PHASE_UNTAP from the per-tick state pass; the untap step can transit
//within a single tick, so the poll usually missed the window and a
//phased-out card never phased back in.
void GameObserver::phasingPhase()
{
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * zone = players[i]->game->inPlay;
        for (int j = zone->nb_cards - 1; j >= 0; j--)
        {
            MTGCardInstance * card = zone->cards[j];
            if (currentPlayer != card->controller() || card->phasedTurn == turn)
                continue;
            if (card->has(Constants::PHASING) && !card->isPhased)
            {
                card->isPhased = true;
                card->phasedTurn = turn;
                if (card->view)
                    card->view->alpha = 50;
                card->initAttackersDefensers();
                WEvent * evphaseout = NEW WEventCardPhasesOut(card, turn);
                receiveEvent(evphaseout);
            }
            else if (card->has(Constants::PHASING) || card->isPhased)
            {
                card->isPhased = false;
                card->phasedTurn = turn;
                if (card->view)
                    card->view->alpha = 255;
                WEvent * evphasein = NEW WEventCardPhasesIn(card);
                receiveEvent(evphasein);
            }
        }
    }
}

void GameObserver::untapPhase()
{
    currentPlayer->inPlay()->untapAll();
}

int GameObserver::receiveEvent(WEvent * e)
{
    if (!e) 
        return 0;
    //#W54-H (A6b/L15): every event but the per-tick GSB-checked marker is a
    //state change - bump the epoch (which also re-arms the Affinity memo).
    if (!dynamic_cast<WEventGameStateBasedChecked *>(e))
        bumpAbilityEpoch();
    eventsQueue.push(e);
    if (eventsQueue.size() > 1) 
        return -1; //resolving events can generate more events
    int result = 0;
    while (eventsQueue.size())
    {
        WEvent * ev = eventsQueue.front();
        result += mLayers->receiveEvent(ev);
        for (int i = 0; i < 2; ++i)
        {
            result += players[i]->receiveEvent(ev);
        }
        SAFE_DELETE(ev);
        eventsQueue.pop();
    }
    if (w54hLegacyBehavior())
        AffinityNeedsUpdate = true; //#W54-H (L15): the old every-event re-arm
    return result;
}

Player * GameObserver::currentlyActing()
{
    if (isInterrupting) 
        return isInterrupting;
    return currentActionPlayer;
}

//TODO CORRECT THIS MESS
int GameObserver::targetListIsSet(MTGCardInstance * card)
{
    if (targetChooser == NULL)
    {
        TargetChooserFactory tcf(this);
        targetChooser = tcf.createTargetChooser(card);
        if (targetChooser == NULL)
        {
            return 1;
        }
    }
    if(targetChooser && targetChooser->validTargetsExist())
    {
        cardWaitingForTargets = card;
        return (targetChooser->targetListSet());
    }
    else
        SAFE_DELETE(targetChooser);
    return 0;
    
}

ostream& operator<<(ostream& out, const GameObserver& g)
{
    if(g.startupGameSerialized == "")
    {
        out << "[init]" << endl;
        out << "player=" << g.currentPlayerId + 1 << endl;
        if(g.mCurrentGamePhase != MTG_PHASE_INVALID)
            out << "phase=" << g.phaseRing->phaseName(g.mCurrentGamePhase) << endl;
        out << "[player1]" << endl;
        out << *(g.players[0]) << endl;
        out << "[player2]" << endl;
        out << *(g.players[1]) << endl;
        return out;
    }
    else
    {
        out << "seed:";
        out << g.mSeed;
        out << endl;
        out << "rvalues:";
        g.randomGenerator.saveUsedRandValues(out);
        out << endl;
        if (g.mSnapshotPostPregame)
            out << "snapshot:postpregame" << endl;
        out << g.startupGameSerialized;
    }

    out << "[do]" << endl;
    list<string>::const_iterator it;

    for(it = (g.actionsList.begin()); it != (g.actionsList.end()); it++)
    {
        out << (*it) << endl;
    }

    out << "[end]" << endl;
    return out;
}

bool GameObserver::parseLine(const string& s)
{
    size_t limiter = s.find("=");
    if (limiter == string::npos) limiter = s.find(":");
    string areaS;
    if (limiter != string::npos)
    {
        areaS = s.substr(0, limiter);
        if (areaS.compare("player") == 0)
        {
            currentPlayerId = atoi(s.substr(limiter + 1).c_str()) - 1;
            return true;
        }
        else if (areaS.compare("phase") == 0)
        {
            mCurrentGamePhase = PhaseRing::phaseStrToInt(s.substr(limiter + 1).c_str());
            return true;
        }
    }
    return false;
}

bool GameObserver::load(const string& ss, bool undo, int controlledPlayerIndex
#ifdef TESTSUITE
                    , TestSuiteGame* testgame
#endif
                        )
{
    bool currentPlayerSet = false;
    int state = -1;
    string s;
    stringstream stream(ss);

    DebugTrace("Loading " + ss);
    randomGenerator.loadRandValues("");

    cleanup();
    mSnapshotPostPregame = false;

    while (std::getline(stream, s))
    {
        if (!s.size()) continue;
        if (s[s.size() - 1] == '\r') s.erase(s.size() - 1); //Handle DOS files
        if (!s.size()) continue;
        if (s[0] == '#') continue;
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s.find("seed ") == 0)
        {
            mSeed = atoi(s.substr(5).c_str());
            randomGenerator.setSeed(mSeed);
            continue;
        }
        if (s.find("snapshot:postpregame") == 0)
        {
            mSnapshotPostPregame = true;
            continue;
        }
        if (s.find("rvalues:") == 0)
        {
            randomGenerator.loadRandValues(s.substr(8).c_str());
            continue;
        }
        switch (state)
        {
        case -1:
            if (s.compare("[init]") == 0)
                state++;
            break;
        case 0:
            if (s.compare("[player1]") == 0)
            {
                state++;
            }
            else
            {
                currentPlayerSet  = parseLine(s);
            }
            break;
        case 1:
            if (s.compare("[player2]") == 0)
            {
                state++;
            }
            else
            {
                if(players.size() == 0 || !players[0])
                {
                    if (s.find("mode=") == 0)
                    {
                        createPlayer(s.substr(5)
             #ifdef TESTSUITE
                                     , testgame
             #endif //TESTSUITE
                                     );
                    }
                }
                players[0]->parseLine(s);
            }
            break;
        case 2:
            if (s.compare("[do]") == 0)
            {
                state++;
            }
            else
            {
                if(players.size() == 1 || !players[1])
                {
                    if (s.find("mode=") == 0)
                    {
                        createPlayer(s.substr(5)
#ifdef TESTSUITE
                                     , testgame
#endif //TESTSUITE
                                     );
                    }
                }
                players[1]->parseLine(s);
            }
            break;
        case 3:
            if (s.compare("[end]") == 0)
            {
                turn = 0;
                mLayers = NEW DuelLayers(this, controlledPlayerIndex);
                currentPlayer = players[currentPlayerId];
                phaseRing = NEW PhaseRing(this);
                startedAt = time(0);

                // take a snapshot before processing the actions
                resetStartupGame();

                if(mRules) mRules->initGame(this, currentPlayerSet);
                phaseRing->goToPhase(0, currentPlayer, false);
                phaseRing->goToPhase(mCurrentGamePhase, currentPlayer);

#ifdef TESTSUITE
                if(testgame)
                    testgame->initGame();
#endif //TESTSUITE

                processActions(undo
               #ifdef TESTSUITE
                               , testgame
               #endif //TESTSUITE
                               );
            }
            else
            {
                logAction(s);
            }
            break;
        }
    }

    return true;
}

//A transcript records a click as zone + INDEX + result + card name. The
//index is only valid for the exact zone ordering the live game had: a
//library shuffle whose rand draw the dump did not capture reorders a hand
//and every later index drifts, ending the replay on a card that is merely
//in a different slot (the 2026-09-02 Vita dump died at turn 6 that way).
//The NAME is the stable identity, so under WAGIC_REPLAY the index is a
//hint: resolve the recorded name inside the recorded zone (nearest slot to
//the recorded one wins) and compare accepted actions on zone+result+name.
static string transcriptActionName(const string& s)
{
    size_t close = s.find("] ");
    if (close == string::npos) return "";
    size_t p = close + 2;
    if (p < s.size() && s[p] == '-') p++;
    while (p < s.size() && s[p] >= '0' && s[p] <= '9') p++;
    return s.substr(p);
}

static string transcriptActionKey(const string& s)
{
    size_t open = s.find("[");
    size_t close = s.find("] ");
    if (open == string::npos || close == string::npos) return s;
    return s.substr(0, open) + s.substr(close + 1);
}

bool GameObserver::processAction(const string& s)
{
    Player* p = players[1];
    if (s.find("p1") != string::npos)
        p = players[0];

    MTGGameZone* zone = NULL;
    if(s.find(string(p->game->hand->getName())+"[") != string::npos)
        zone = p->game->hand;
    else if(s.find(string(p->game->battlefield->getName())+"[") != string::npos)
        zone = p->game->battlefield;
    else if(s.find(string(p->game->graveyard->getName())+"[") != string::npos)
        zone = p->game->graveyard;
    else if(s.find(string(p->game->library->getName())+"[") != string::npos)
        zone = p->game->library;

    if(zone) {
        size_t begin = s.find("[")+1;
        size_t size = s.find("]")-begin;
        size_t index = atoi(s.substr(begin, size).c_str());
        if (getenv("WAGIC_REPLAY"))
        {
            const string wanted = transcriptActionName(s);
            const bool drifted = !wanted.empty()
                && (index >= zone->cards.size() || zone->cards[index]->getLCName() != wanted);
            if (drifted)
            {
                size_t found = zone->cards.size();
                for (size_t i = 0; i < zone->cards.size(); i++)
                {
                    if (zone->cards[i]->getLCName() != wanted) continue;
                    const size_t di = (i > index) ? i - index : index - i;
                    const size_t df = (found > index) ? found - index : index - found;
                    if (found == zone->cards.size() || di < df) found = i;
                }
                if (found < zone->cards.size())
                {
                    DebugTrace("REPLAY: index drift on '" << s << "' - '" << wanted << "' resolved at slot " << found);
                    index = found;
                }
            }
        }
        if (index >= zone->cards.size())
        {
            {
                std::stringstream z;
                z << "REPLAY MISMATCH zone index out of range: " << s << " | zone has " << zone->nb_cards << ":";
                for (int i = 0; i < zone->nb_cards; i++) z << " " << zone->cards[i]->getLCName();
                z << " | other hand " << p->opponent()->game->hand->nb_cards << " library " << p->game->library->nb_cards << " current player " << (currentPlayer == players[0] ? "p1" : "p2");
                DebugTrace(z.str());
            }
            if (!getenv("WAGIC_REPLAY"))
                dumpAssert(false);
            return false;
        }
        cardClick(zone->cards[index], zone->cards[index]);
    } else if (s.find("stack") != string::npos) {
        size_t begin = s.find("[")+1;
        size_t size = s.find("]")-begin;
        size_t index = atoi(s.substr(begin, size).c_str());
        stackObjectClicked((Interruptible*)mLayers->stackLayer()->getByIndex(index));
    } else if (s.find("yes") != string::npos) {
        mLayers->stackLayer()->setIsInterrupting(p);
    } else if (s.find("no") != string::npos) {
        mLayers->stackLayer()->cancelInterruptOffer();
    } else if (s.find("endinterruption") != string::npos) {
        mLayers->stackLayer()->endOfInterruption();
    } else if (s.find("next") != string::npos) {
        userRequestNextGamePhase();
    } else if (s.find("combatok") != string::npos) {
        mLayers->combatLayer()->clickOK();
    } else if (s == "p1" || s == "p2") {
        cardClick(NULL, p);
    } else if (s.find("choice") != string::npos) {
        int choice = atoi(s.substr(s.find("choice ") + 7).c_str());
            mLayers->actionLayer()->doReactTo(choice);
    } else if (s == "p1" || s == "p2") {
        cardClick(NULL, p);
    } else if(s.find("mulligan") != string::npos) {
        Mulligan(p);
    } else if(s.find("shufflelib") != string::npos) {
        // This should probably be differently and be automatically part of the ability triggered
        // that would allow the AI to use it as well.
        shuffleLibrary(p);
    } else {
        DebugTrace("no clue about: " + s);
    }

    return true;
}

bool GameObserver::processActions(bool undo
                                  #ifdef TESTSUITE
                                  , TestSuiteGame* testgame
                                  #endif
                                  )
{
    bool result = false;
    size_t cmdIndex = 0;

    loadingList = actionsList;
    actionsList.clear();

    mLoading = true;
    float counter = 0.0f;

    // To handle undo, we'll remove the last P1 action and all P2 actions after.
    if(undo && loadingList.size()) {
        while(loadingList.back().find("p2") != string::npos)
            loadingList.pop_back();
        // we do not undo "next phase" action to avoid abuse by users
        if(loadingList.back().find("next") == string::npos)
            loadingList.pop_back();
    }

    // We fake here cause the initialization before caused mana pool reset events to be triggered
    // So, we need them flushed to be able to set the manapool to whatever we need
    GameObserver::Update(counter);
    counter += 1.000f;

#ifdef TESTSUITE
    if(testgame)
    {
        testgame->ResetManapools();
    }
#endif

    for(loadingite = loadingList.begin(); loadingite != loadingList.end(); loadingite++, cmdIndex++)
    {
        //A recorded action is an intent; the live game accepted it on some
        //tick, so re-issue it until the engine logs the same effective
        //action (a refused click logs "0<name>", a gated request logs
        //nothing) - bounded, then it is a real divergence.
        const bool lenient = getenv("WAGIC_REPLAY") != NULL;
        size_t nb = 0;
        bool accepted = false;
        //A recorded phase request for a phase this replay has already left
        //(the live game's automation ran between two recorded requests) is
        //already satisfied; a request for a later phase is driven there by
        //the retry loop one phase at a time.
        {
            size_t nx = loadingite->find("next ");
            if (nx != string::npos && mSnapshotPostPregame)
            {
                int recordedPhase = atoi(loadingite->substr(loadingite->rfind(' ') + 1).c_str());
                if (recordedPhase < mCurrentGamePhase)
                {
                    DebugTrace("REPLAY: already past '" << *loadingite << "' (phase " << mCurrentGamePhase << ")");
                    actionsList.push_back(*loadingite);
                    continue;
                }
            }
        }
        for (int attempt = 0; attempt < 60 && !accepted; attempt++)
        {
            size_t before = actionsList.size();
            processAction(*loadingite);
            nb = actionsList.size();
            for (int i = 0; i<6; i++)
            {
                // let's fake an update
                GameObserver::Update(counter);
                counter += 1.000f;
            }
            if (nb > before && nb == actionsList.size()
                && (actionsList.back() == *loadingite
                    || (lenient && transcriptActionKey(actionsList.back()) == transcriptActionKey(*loadingite))))
                accepted = true;
            else if (lenient && loadingite->find("next ") != string::npos && mSnapshotPostPregame
                     && atoi(loadingite->substr(loadingite->rfind(' ') + 1).c_str()) < mCurrentGamePhase)
            {
                //the retries' own updates carried the game past this phase
                actionsList.resize(before);
                actionsList.push_back(*loadingite);
                accepted = true;
            }
            else if (nb > before)
                actionsList.resize(before); //drop the refused/extra record and retry
        }
        if (!accepted)
        {
            std::stringstream why;
            why << "REPLAY DIVERGED at action " << cmdIndex << " expected '" << *loadingite << "' got '"
                << (actionsList.size() ? actionsList.back() : string("(nothing)")) << "' turn " << turn << " phase " << mCurrentGamePhase
                ;
            //currentActionCard / the chooser's source are known dangle classes
            //once the layer is no longer waiting (the 2026-09-02 desktop replay
            //SEGV'd reporting a divergence through a freed card): name them only
            //while the layer is waiting for an answer.
            const bool layerWaiting = mLayers && mLayers->actionLayer()->isWaitingForAnswer();
            why << " | chooser=" << (layerWaiting && getCurrentTargetChooser() ? (getCurrentTargetChooser()->source ? getCurrentTargetChooser()->source->getLCName() : string("(no source)")) : string("none"))
                << " menu=" << (layerWaiting && mLayers->actionLayer()->menuObject ? mLayers->actionLayer()->menuObjectName : string("none"))
                << " menuCard=" << (layerWaiting && mLayers->actionLayer()->currentActionCard ? mLayers->actionLayer()->currentActionCard->getLCName() : string("none"))
            ;
            if (layerWaiting && mLayers->actionLayer()->currentActionCard)
            {
                MTGCardInstance * mc = mLayers->actionLayer()->currentActionCard;
                why << " reacting:";
                ActionLayer * al = mLayers->actionLayer();
                for (size_t i = 1; i < al->mObjects.size(); i++)
                {
                    MTGAbility * ab = dynamic_cast<MTGAbility *>(al->mObjects[i]);
                    if (ab && ab->isReactingToClick(mc, NULL))
                        why << " [" << ab->getMenuText() << "]";
                }
            }
            why                << " extraPayment=" << (mExtraPayment ? "yes" : "no")
                << " stackUnresolved=" << (mLayers ? mLayers->stackLayer()->count(0, NOT_RESOLVED) : -1)
                << " isInterrupting=" << (isInterrupting ? (isInterrupting == players[0] ? "p1" : "p2") : "none");
            DebugTrace(why.str());
            if (lenient)
            {
                fprintf(stderr, "WAGIC_REPLAY: %s\n", why.str().c_str());
                break;
            }
            dumpAssert(false);
        }
        dumpAssert(cmdIndex == (actionsList.size()-1));
    }

    mLoading = false;
    return result;
}

void GameObserver::logAction(Player* player, const string& s) {
    if(player == players[0])
        if(s != "")
            logAction("p1." + s);
        else
            logAction("p1");
    else
        if(s != "")
            logAction("p2." + s);
        else
            logAction("p2");
}

void GameObserver::logAction(MTGCardInstance* card, MTGGameZone* zone, size_t index, int result) {
    stringstream stream;
    if(zone == NULL) zone = card->currentZone;
    string zoneName = (zone != NULL)?zone->getName():"UnknownZone"; // Fixed a crash when zone pointer was null.
    stream << "p" << ((card->controller()==players[0])?"1.":"2.")
           << zoneName << "[" << index << "] "
           << result << card->getLCName();
    logAction(stream.str());
}

void GameObserver::logAction(const string& s)
{
    if(mLoading)
    {
        string toCheck = *loadingite;
        if (toCheck != s && !(getenv("WAGIC_REPLAY") && transcriptActionKey(toCheck) == transcriptActionKey(s)))
        {
            //Replay drift: the load loop re-issues the intent or reports the
            //divergence; a transcript replay (WAGIC_REPLAY) must not abort.
            DebugTrace("REPLAY MISMATCH expected '" << toCheck << "' got '" << s << "'");
            if (!getenv("WAGIC_REPLAY"))
                dumpAssert(false);
        }
    }
    actionsList.push_back(s);
};

bool GameObserver::undo()
{
    stringstream stream;
    stream << *this;
    DebugTrace(stream.str());
    return load(stream.str(), true);
}

void GameObserver::Mulligan(Player* player)
{
    if(!player) player = currentPlayer;
    logAction(player, "mulligan");
    player->takeMulligan();
}

void GameObserver::serumMulligan(Player* player)
{
    if(!player) player = currentPlayer;
    logAction(player, "mulligan serum powder");
    player->serumMulligan();
}

Player* GameObserver::createPlayer(const string& playerMode
                                #ifdef TESTSUITE
                                , TestSuiteGame* testgame
                                #endif //TESTSUITE
                                )
{
    Player::Mode aMode = (Player::Mode)atoi(playerMode.c_str());
    Player* pPlayer = 0;

    switch(aMode)
    {
    case Player::MODE_AI:
        AIPlayerFactory playerCreator;
        if(players.size())
            pPlayer = playerCreator.createAIPlayer(this, MTGCollection(), players[0]);
        else
            pPlayer = playerCreator.createAIPlayer(this, MTGCollection(), 0);
        break;
    case Player::MODE_HUMAN:
        pPlayer = new HumanPlayer(this, "", "");
        break;
    case Player::MODE_TEST_SUITE:
#ifdef TESTSUITE
        if(players.size())
            pPlayer = new TestSuiteAI(testgame, 1);
        else
            pPlayer = new TestSuiteAI(testgame, 0);
#endif //TESTSUITE
        break;
    }

    if(pPlayer)
    {
        players.push_back(pPlayer);
    }

    return pPlayer;
}

#ifdef TESTSUITE
void GameObserver::loadTestSuitePlayer(int playerId, TestSuiteGame* testSuite)
{
    loadPlayer(playerId, new TestSuiteAI(testSuite, playerId));
}
#endif //TESTSUITE

void GameObserver::loadPlayer(int playerId, Player* player)
{
    //Because we're using a vector instead of an array (why?),
    // we have to prepare the vector in order to be the right size to accomodate the playerId variable
    // see http://code.google.com/p/wagic/issues/detail?id=772
    if (players.size() > (size_t) playerId) {
        SAFE_DELETE(players[playerId]);
        players[playerId] = NULL;
    } else {
        while (players.size() <= (size_t) playerId)
        {
            players.push_back(NULL);
        }
    }

    players[playerId] = player;
}

void GameObserver::loadPlayer(int playerId, PlayerType playerType, int decknb, bool premadeDeck)
{
    if (decknb)
    {
        if (playerType == PLAYER_TYPE_HUMAN)
        { //Human Player
            if(playerId == 0)
            {
                char deckFile[255];
                if (premadeDeck)
                    sprintf(deckFile, "player/premade/deck%i.txt", decknb);
                else
                    sprintf(deckFile, "%s/deck%i.txt", options.profileFile().c_str(), decknb);
                char deckFileSmall[255];
                sprintf(deckFileSmall, "player_deck%i", decknb);

                loadPlayer(playerId, NEW HumanPlayer(this, deckFile, deckFileSmall, premadeDeck));
            }
        }
        else
        { //AI Player, chooses deck
            AIPlayerFactory playerCreator;
            Player * opponent = NULL;
            if (playerId == 1) opponent = players[0];

            loadPlayer(playerId, playerCreator.createAIPlayer(this, MTGCollection(), opponent, decknb));
        }
    }
    else
    {
        //Random deck
        AIPlayerFactory playerCreator;
        Player * opponent = NULL;

        // Reset the random logging.
        randomGenerator.loadRandValues("");

        if (playerId == 1) opponent = players[0];
#ifdef AI_CHANGE_TESTING
        if (playerType == PLAYER_TYPE_CPU_TEST)
            loadPlayer(playerId, playerCreator.createAIPlayerTest(this, MTGCollection(), opponent, playerId == 0 ? "ai/bakaA/" : "ai/bakaB/"));
        else
#endif
        {
            loadPlayer(playerId, playerCreator.createAIPlayer(this, MTGCollection(), opponent));
        }

        if (playerType == PLAYER_TYPE_CPU_TEST)
            ((AIPlayer *) players[playerId])->setFastTimerMode();
    }
}

#ifdef NETWORK_SUPPORT
NetworkGameObserver::NetworkGameObserver(JNetwork* pNetwork, WResourceManager* output, JGE* input)
    : GameObserver(output, input), mpNetworkSession(pNetwork),     mSynchronized(false)
{
    mpNetworkSession->registerCommand("loadPlayer", this, loadPlayer, ignoreResponse);
    mpNetworkSession->registerCommand("synchronize", this, synchronize, checkSynchro);
    mpNetworkSession->registerCommand("sendAction", this, sendAction, checkSynchro);
    mpNetworkSession->registerCommand("disconnect", this, disconnect, ignoreResponse);
}

NetworkGameObserver::~NetworkGameObserver()
{
    mpNetworkSession->sendCommand("disconnect", "");
}

void NetworkGameObserver::disconnect(void*pxThis, stringstream&, stringstream&)
{
    NetworkGameObserver* pThis = (NetworkGameObserver*)pxThis;
    pThis->setLoser(pThis->getView()->getRenderedPlayerOpponent());
}

void NetworkGameObserver::Update(float dt)
{
    mpNetworkSession->Update();
    ::GameObserver::Update(dt);
}

void NetworkGameObserver::loadPlayer(int playerId, Player* player)
{
    GameObserver::loadPlayer(playerId, player);
    stringstream out;
    out << *player;
    mpNetworkSession->sendCommand("loadPlayer", out.str());
}

void NetworkGameObserver::loadPlayer(void*pxThis, stringstream& in, stringstream&)
{
    NetworkGameObserver* pThis = (NetworkGameObserver*)pxThis;
    Player* pPlayer = 0;
    string s;

    while(std::getline(in, s))
    {
        if (s.find("mode=") == 0)
        {
            pPlayer = pThis->createPlayer(s.substr(5)
    #ifdef TESTSUITE
                            , 0
    #endif //TESTSUITE
                            );
        }

        if(pPlayer && (!pPlayer->parseLine(s)))
        {
            break;
        }
    }
}

void NetworkGameObserver::synchronize()
{
    if(!mSynchronized && mpNetworkSession->isServer())
    {
        stringstream out;
        out << *this;
        mpNetworkSession->sendCommand("synchronize", out.str());
        mSynchronized = true;
    }
}

void NetworkGameObserver::synchronize(void*pxThis, stringstream& in, stringstream& out)
{
    NetworkGameObserver* pThis = (NetworkGameObserver*)pxThis;
    // now, we need to load the game from player 2's perspective
    pThis->load(in.str(), false, 1);
    out << *pThis;
}


void NetworkGameObserver::checkSynchro(void*pxThis, stringstream& in, stringstream&)
{
    NetworkGameObserver* pThis = (NetworkGameObserver*)pxThis;
    
    GameObserver aGame;
    aGame.mRules = pThis->mRules;
    aGame.load(in.str());

    assert(aGame == *pThis);
}

void NetworkGameObserver::sendAction(void*pxThis, stringstream& in, stringstream&)
{
    NetworkGameObserver* pThis = (NetworkGameObserver*)pxThis;

    pThis->mForwardAction = false;
    pThis->processAction(in.str());
    pThis->mForwardAction = true;
    //out << *pThis;
}

void NetworkGameObserver::logAction(const string& s)
{
    GameObserver::logAction(s);
    if(mForwardAction)
        mpNetworkSession->sendCommand("sendAction", s);
}

#endif
