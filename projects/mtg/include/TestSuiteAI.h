#ifndef _TESTSUITE_AI_H_
#define _TESTSUITE_AI_H_

#ifdef TESTSUITE

#define MAX_TESTSUITE_ACTIONS 100
#define MAX_TESTUITE_CARDS 100

#include "Threading.h"
#include <atomic>
#include "AIPlayerBaka.h"


class TestSuiteActions
{
public:
    int nbitems;
    string actions[MAX_TESTSUITE_ACTIONS];
    void add(string action);
    TestSuiteActions();
    void cleanup();
};


class TestSuiteGame;
class TestSuite;
class TestSuiteAI;
class TestSuiteState
{
public:
    GamePhase phase;
    // -1 means no terminal-state assertion was requested; 0 means the game
    // must still be live; 1/2 means player 1/2 must be the loser.
    int expectedGameOver;
    void parsePlayerState(int playerId, string s);
    TestSuiteState();
    ~TestSuiteState();

    vector<TestSuiteAI*> players;
    void cleanup(TestSuiteGame* tsGame);
};

class TestSuiteGame
{
    friend class TestSuiteAI;
    friend class TestSuite;
protected:
    string filename;
    int summoningSickness;
    bool forceAbility;
    GameType gameType;
    unsigned int seed;
    int aiMaxCalls;
    TestSuiteState endState;
    TestSuiteState initState;
    TestSuiteActions actions;
    float timerLimit;
    bool isOK;
    int currentAction;
    GameObserver* observer;
    // TestSuiteAI clears GameObserver::gameOver each tick so a test can keep
    // executing scripted actions. Preserve the observed terminal result for
    // an explicit [ASSERT] gameover: assertion.
    int observedGameOver;

    static boost::mutex mMutex;
    virtual void handleResults(bool wasAI, int error);
    TestSuite* testsuite;
    bool load();

public:
    virtual ~TestSuiteGame();
    TestSuiteGame(TestSuite* testsuite);
    TestSuiteGame(TestSuite* testsuite, string _filename);
    void ResetManapools();
    void initGame();
    void assertGame();
    //set once assertGame has run - lets the runner auto-assert when the
    //game ends by itself (lethal trigger loop) before the script's
    //end-of-actions assert is reached
    bool mAsserted;
    MTGPlayerCards * buildDeck(Player* player, int playerId);
    GameType getGameType() { return gameType; };
    string getNextAction();
    Interruptible * getActionByMTGId(int mtgid);
    static int Log(const char * text);
    void setObserver(GameObserver* anObserver) {observer = anObserver; };
};

class TestSuite : public TestSuiteGame
{
private:
    int currentfile;
    int nbfiles;
    vector<string> files;

    void cleanup();
    vector<boost::thread*> mWorkerThread;
    Rules* mRules;

    bool mProcessing;
    int startTime, endTime;
    static void ThreadProc(void* inParam);
    string getNextFile() {
        boost::mutex::scoped_lock lock(mMutex);
        if (currentfile >= (int)files.size()) return "";
        currentfile++;
        return files[currentfile - 1];
    };
    void pregameTests();

public:
    int getElapsedTime() {return endTime-startTime;};
    //Wait for all worker threads to finish their in-flight tests. MUST be
    //called before reading the result counters or tearing down: the file
    //list running dry on the main thread does not mean the suite is done,
    //and exiting with live workers used to discard their results (or
    //corrupt teardown - random "end of suite" segfaults).
    void joinWorkers();
    //This used to be a second `unsigned int seed` SHADOWING the inherited
    //TestSuiteGame::seed. TestSuiteGame::load() parses the test file's
    //"seed" directive into the base member, while GameStateDuel reads it
    //through a TestSuite* - the shadow meant the directive never worked
    //and AI chance gates rolled real rand() per run (flaky AI tests).
    using TestSuiteGame::seed;
    //atomic: incremented from every worker thread (handleResults runs
    //unlocked on the could-not-load paths); plain ints lost updates.
    std::atomic<int> nbFailed, nbTests, nbAIFailed, nbAITests;
    TestSuite(const char * filename);
    ~TestSuite();
    void initGame(GameObserver* g);
    int loadNext();
    void setRules(Rules* rules) {
      mRules = rules;
    };
    void handleResults(bool wasAI, int error);
    // run the test suite in turbo mode without UI, 
    // returns the amount of failed tests (AI or not), so 0 if everything went fine.
    int run();
};

class TestSuiteAI:public AIPlayerBaka
{
private:
    MTGCardInstance * getCard(string action);
    float timer;
    float aiActCounter; //dt fed to AIPlayerBaka::Act in [AI] tests (was a thread-shared function static)
    bool mAssertPhaseArmed; //[AI] tests: game has left the [ASSERT] phase at least once
    TestSuiteGame * suite;

public:
    //[ASSERT]-only: expected number of tapped battlefield cards (-1 = don't check)
    int expectedTappedInPlay;
    //[ASSERT]-only: cards expected to carry the hidden necroed flag.
    vector<string> expectedNecroed;
    //[ASSERT]-only: card|basic-ability pairs expected on the live card.
    vector<string> expectedBasicAbilities;
    //[ASSERT]-only: card|display-text pairs expected on the live card.
    vector<string> expectedCardText;

    TestSuiteAI(TestSuiteGame *tsGame, int playerId);
    virtual int Act(float dt);
    virtual int displayStack();
    bool parseLine(const string& s);
    bool summoningSickness() {return (suite->summoningSickness == 1); }
};

// Card-script validator (WAGIC_VALIDATE=1): drives every collection card's ability
// lines through the real AbilityFactory parse path and reports each line that fails
// to produce an ability. Returns the failure count (0 = all clean). Does not return
// to the caller in the intended flow — the caller exit()s on its result.
int runCardScriptValidation();

#endif
#endif
