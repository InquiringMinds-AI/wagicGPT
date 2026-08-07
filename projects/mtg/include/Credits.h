#ifndef _CREDITS_H_
#define _CREDITS_H_

#include <vector>
#include <string>
#include <JGE.h>
#include "WFont.h"
#include <time.h>
#include "Player.h"
#include <Threading.h>

class GameApp;
class DeckStats;
class PlayerData;

using namespace std;

class Unlockable
{
private:
    map <string, string>mValues;
public:
    Unlockable();
    void setValue(string, string);
    string getValue(string);
    bool isUnlocked();
    //deferSave: see GameOptionAward::giveAward - the award is granted but the
    //caller owns persisting options.
    bool tryToUnlock(GameObserver * game, bool deferSave = false);
    static void load();
    static map <string, Unlockable *> unlockables;
    static void Destroy();
};


class CreditBonus
{
public:
    int value;
    string text;
    CreditBonus(int _value, string _text);
    void Render(float x, float y, WFont * font);
};

class Credits
{
private:
    time_t gameLength;
    int isDifficultyUnlocked(DeckStats * stats);
    int isEvilTwinUnlocked();
    int isCommanderUnlocked();
    int isRandomDeckUnlocked();
    int IsMoreAIDecksUnlocked(DeckStats * stats);
    string unlockedTextureName;
    JQuadPtr GetUnlockedQuad(string texturename);
    bool mTournament;
    bool mMatch;
    bool mPlayerWin;
    int mGamesWon;
    int mGamesPlayed;
    int mMatchesWon;
    int mMatchesPlayed;
    //Deferred end-of-match persistence: compute() does all the (cheap) credit
    //math and unlock checks synchronously but stashes the (expensive, Memory
    //Stick-bound) writes here; Render() flushes them after the victory screen
    //has actually been presented, so the player sees the result screen instead
    //of a multi-second freeze on the last frame of the duel. The destructor is
    //the safety net - the save always happens.
    PlayerData * mPendingPlayerData;
    bool mPendingOptionsSave;
    int mRenderCount;
    //The flush runs on a WORKER thread (startAsyncFlush) so the victory screen
    //keeps rendering and taking input during the multi-second Memory Stick
    //writes - a confirm pressed mid-save is registered immediately and acted on
    //as soon as flushInProgress() clears (GameStateDuel gates the transition).
    //mFlushDone is the worker->main completion signal; mFlushStarted is
    //main-thread-only. The PSP thread wrapper's join() is terminate-delete, so
    //join is only ever called after mFlushDone (see ensureFlushed).
    boost::thread mFlushThread;
    bool mFlushStarted;
    volatile bool mFlushDone;
    static void FlushProc(void * inParam);
public:
    int value;
    Player * p1, *p2;
    GameObserver* observer;
    GameApp * app;
    int showMsg;
    int unlocked;
    string unlockedString;
    vector<CreditBonus *> bonus;
    Credits();
    ~Credits();
    void compute(GameObserver* observer, GameApp * _app);
    void computeTournament(GameObserver* g, GameApp * _app,bool tournament,bool match, bool playerWin,int gamesWon,int gamesPlayed,int matchesWon,int matchesPlayed);
    void Render();
    void flushPendingSave();
    void startAsyncFlush();
    void ensureFlushed();
    //True once deferred writes exist AND the victory screen has been presented
    //for at least two frames - the Update-side signal to call startAsyncFlush().
    bool readyToFlush() const { return (mPendingPlayerData || mPendingOptionsSave) && mRenderCount >= 2 && !mFlushStarted; }
    //True while the worker thread is still writing - transitions away from the
    //victory screen must wait for this to clear (the pending press stays
    //latched, no second press needed).
    bool flushInProgress() const { return mFlushStarted && !mFlushDone; }
    static int unlockRandomSet(bool force = false, bool deferSave = false);
    static int unlockSetByName(string name, bool deferSave = false);
    static int addCreditBonus(int value);
    static int addCardToCollection(int cardId, MTGDeck * collection);
    static int addCardToCollection(int cardId);
};

#endif
