#ifndef _GAMEOBSERVER_H_
#define _GAMEOBSERVER_H_

#include "Player.h"
#include "MTGAbility.h"
#include "DuelLayers.h"
#include "MTGCardInstance.h"
#include "PlayGuiObject.h"
#include "TargetChooser.h"
#include "PhaseRing.h"
#include "ReplacementEffects.h"
#include "GuiStatic.h"
#include <queue>
#include <time.h>
#ifdef NETWORK_SUPPORT
#include "JNetwork.h"
#endif //NETWORK_SUPPORT

class MTGGamePhase;
class MTGAbility;
class MTGCardInstance;
struct CardGui;
class Player;
class TargetChooser;
class Rules;
class TestSuiteGame;
class Trash;
class DeckManager;
class PreGamePhase;
using namespace std;

//Match transcript (development builds): every game rewrites a replayable
//dump of itself - the suite's own [init]/[player]/[do] format plus seed and
//rand values - at each untap and at game end, and the owner can classify the
//match from the victory screen. Desktop replays a dump with WAGIC_REPLAY=<file>
//(GameStateDuel selfplay boot). Compiled out of release builds.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS) || defined(WAGIC_TRANSCRIPT)
#define WAGIC_TRANSCRIPT_ON 1
#endif

class GameObserver{
 protected:
  unsigned int mSeed;
  GameType mGameType;
  MTGCardInstance * cardWaitingForTargets;
  queue<WEvent *> eventsQueue;
  // used when we're running to log actions
  list<string> actionsList;
  // used when we're loading to know what to load
  list<string> loadingList;
  list<string>::iterator loadingite;
  //#W56-E: clicks the ENGINE made during a WAGIC_REPLAY that the record
  //does not contain (equip/activation auto-tap). Parked, not appended to
  //actionsList; the loader consumes one when the record asks for it.
  list<string> mReplayEngineActions;
  RandomGenerator randomGenerator;
  //#W56-E (O8/A35): the heuristic AI's OWN stream. One shared stream fed
  //shuffles, dice AND the AI's own rolls, so a seat swap, an AI change or
  //any new AI-side draw shifted every later game value and a transcript
  //stopped replaying. Seeded from the game seed (derived, so the two
  //streams never coincide) - deterministic, and invisible to `rvalues:`.
  RandomGenerator aiRandomGenerator;
  WResourceManager* mResourceManager;
  JGE* mJGE;
  DeckManager* mDeckManager;
  Player * gameOver;
  GamePhase mCurrentGamePhase;

  int untap(MTGCardInstance * card);
  bool WaitForExtraPayment(MTGCardInstance* card);
  void cleanup();
  string startupGameSerialized;
  bool parseLine(const string& s);
  virtual void logAction(const string& s);
  bool processAction(const string& s);
  bool processActions(bool undo
                    #ifdef TESTSUITE
                    , TestSuiteGame* testgame
                    #endif
                      );
  friend ostream& operator<<(ostream&, const GameObserver&);
  bool mLoading;
  void nextGamePhase();
  void shuffleLibrary(Player* p);
  Player* createPlayer(const string& playerMode
                  #ifdef TESTSUITE
                  , TestSuiteGame* testgame
                  #endif //TESTSUITE
                    );

 public:
  int currentPlayerId;
  CombatStep combatStep;
  int turn;
  int forceShuffleLibraries();
  int targetListIsSet(MTGCardInstance * card);
  PhaseRing * phaseRing;
  vector<list<Phase*> >gameTurn;
  int cancelCurrentAction();
  ExtraCosts * mExtraPayment;
  //TRUE for games driven by the test-suite harness. Scripted fixtures
  //encode exact phase cadences, so the ASPHASES auto-skips must stay off
  //even after the `ai` command flips both players to MODE_AI (the old
  //playMode==MODE_TEST_SUITE check stops seeing them at that point).
  bool mSuiteGame;
  //Transcript/undo baseline moved past the pre-game: the dump was taken
  //AFTER opening hands and mulligans, so load() must not re-run the rules'
  //[PLAYERS] init actions (shuffle, draw:7) nor the first-player roll.
  bool mSnapshotPostPregame;
  string mTranscriptPath;   //"" = no transcript for this game (suite, replay)
  string mTranscriptNotes;  //#result / #classification lines appended after [end]
  //W53-U: the game-over hooks below used to be guarded by file-static
  //`GameObserver *` pointers in GameStateDuel::Update ("have I already noted
  //THIS game?"). A GameObserver is deleted at End() and a new one allocated at
  //Start(), and the allocator hands back the SAME address more often than not -
  //so on the console the guard read "already done" for a brand new game and the
  //#result line (and the Vita memlog's gameend mark) went missing for 9 of the
  //16 vpk12 matches. Identity has to live on the object, not in a static.
  bool mGameEndNoted;      //#result written for this game
  bool mGameEndMemlogged;  //VITA memlog "gameend" mark written for this game
  void writeTranscript(const char * tag);
  //#W57-T: the softlock diagnostics dump written for THIS game (empty = none).
  //Kept on the observer, not in a static: a GameObserver is deleted at End()
  //and the allocator hands the same address back for the next game (the W53-U
  //lesson), so identity has to live on the object. Read by the post-match
  //classification menu so a "bug" verdict names the file that was captured.
  string mSoftlockDumpPath;
  //#W57-T half A - the in-thread hang guard. DEFAULT OFF, and off in the test
  //suite and the selfplay harness even when the env var is set for the shell,
  //unless WAGIC_HANG_GUARD=1 names it explicitly: a suite tick under load can
  //legitimately take seconds, and a guard that fires on a slow machine is a
  //flaky red, not a bug report. hangTickBegin re-arms the budget once per game
  //tick; hangCheck is the cheap per-iteration probe the loop sites call. NOTE
  //the budget is per TICK, not per decision: an AI seat waiting on a model
  //round trip is not inside this tick at all (the seam returns kChoicePending
  //and the frame completes), so a slow endpoint can never trip it.
  bool mHangGuardOn = false;
  bool mHangGuardResolved = false; //env consulted once per observer
  long mHangBudgetMs = 5000;
  long long mHangTickStartMs = 0;
  unsigned long mHangIterations = 0;
  void hangTickBegin();
  void hangCheck(const char * site);
  //Test-suite hook: the programmatic twin of WAGIC_HANG_GUARD=1 +
  //WAGIC_HANG_GUARD_MS=<budget>, so a fixture can arm the guard without an
  //environment variable (a registered fixture runs under the threaded suite
  //and must not depend on process-wide state). Never called outside a fixture.
  void hangGuardForTest(long budgetMs)
  {
      mHangGuardResolved = true;
      mHangGuardOn = true;
      mHangBudgetMs = budgetMs;
      mHangTickStartMs = 0;
      mHangIterations = 0;
  }
  //#W57-T: the last recorded actions, for the dump's transcript tail. The
  //list IS the transcript body (operator<< writes it), so no second buffer.
  const list<string> & getActionsList() const { return actionsList; }
  void setReplayRules(Rules * rules) { mRules = rules; }
  void appendTranscriptNote(const string & note);
  //`realgame` fixture directive only: names the seat the engine should treat
  //as the human one for the no-legal-action phase automation, so a fixture
  //can exercise the real-play skip path. NULL in every ordinary game.
  Player * mSuiteHumanSeat;
  //Memo for the "a window with no legal action is not a window" phase skip.
  //hasAnyLegalAction walks the hand, every mana producer and the whole action
  //layer; the skip is evaluated on EVERY state-based check, which is far too
  //often for that walk to run - it would be felt on the handhelds first.
  //The verdict only needs recomputing on phase entry: a false verdict
  //advances the phase immediately, and a true verdict is a stop the player is
  //already sitting in. Defaults mean "the player can act", so a stale memo can
  //only ever cost a stop, never skip a window.
  int mNoActionTurn = -1, mNoActionPhase = -1, mNoActionStep = -1;
  bool mNoActionVerdict = true;
  //Test-suite opt-in: a fixture's `interactivereveal` [DO] directive sets this
  //so an aicode= reveal/scry card drives its REAL interactive display (which
  //the scripted seat can operate) instead of the AI's headless aicode
  //substitute. Off by default, so every existing aicode fixture (Armillary
  //Sphere, Collected Conjuring, ...) keeps the aicode path unchanged; only a
  //fixture that must test the interactive selection (surveil binning) opts in.
  //In-class initialized so real games and unopted fixtures never see it true.
  bool mForceInteractiveReveal = false;
  //W50-W (D2) test-suite hook, per observer (safe under the threaded suite):
  //`revealasync <names>` makes the scripted seat present as an interactive AI
  //to the reveal driver and stands in for the model's decideReveal (picks = the
  //named cards); `revealasyncticks N` sets how many driver ticks the stub
  //reports "in flight" before deciding (default 2). Empty/-1 = off.
  std::string mRevealTestAsyncPicks;
  int mRevealTestAsyncTicks = -1;
  //#W54-F (D7a) test-suite hook: shrink the reveal driver's stall budget so a
  //fixture can reach the force-close inside a script. -1 = the shipped budget
  //(both a tick floor and a wall-clock floor); N > 0 = N no-progress ticks and
  //NO wall-clock floor. Never set outside a fixture.
  int mRevealStallTicks = -1;
  //The CR pre-game phase (opening hands + London mulligan + 103.6 actions),
  //run before turn 1 of real/selfplay/demo games. NULL in suite games and
  //once the phase has completed. While non-NULL, Update runs the phase and
  //gates the normal game loop.
  PreGamePhase * mPregame = NULL;
  //TRUE once the pre-game phase has finished. Closes the old in-game mulligan
  //menu entry (GameStateDuel) and the old leyline click path (MTGPutInPlayRule)
  //so there is no double mulligan / double leyline. Stays FALSE in suite games
  //(which skip the phase and rely on the old start), keeping those paths live.
  bool mPregameDone = false;
  int oldGamePhase;
  TargetChooser * targetChooser;
  CardDisplay * OpenedDisplay;
  GuiGameZone * guiOpenDisplay;
  DuelLayers * mLayers;
  ReplacementEffects *replacementEffects;
  vector<Player *> players; //created outside
  time_t startedAt;
  Rules * mRules;
  MTGCardInstance* ExtraRules;
  Trash* mTrash;

  GameType gameType() const { return mGameType; };
  TargetChooser * getCurrentTargetChooser();
  void stackObjectClicked(Interruptible * action);

  int cardClickLog(bool log, Player* clickedPlayer, MTGGameZone* zone, MTGCardInstance*backup, size_t index, int toReturn);
  int cardClick(MTGCardInstance * card, MTGAbility *ability);
  int cardClick(MTGCardInstance * card, int abilityType);
  int cardClick(MTGCardInstance * card,Targetable * _object = NULL, bool log = true);
  GamePhase getCurrentGamePhase();
  void setCurrentGamePhase(GamePhase phase) { mCurrentGamePhase = phase; };
  const string& getCurrentGamePhaseName();
  const string& getNextGamePhaseName();
  void nextCombatStep();
  //Engine-issued combat decisions (W3b): the engine, not AI branch luck,
  //decides when a declaration is due. NONE while the stack is unsettled
  //(attack triggers resolve BEFORE blocks are declared) or any menu /
  //target chooser / extra payment is pending.
  enum CombatDecision
  {
      COMBAT_DECISION_NONE,
      COMBAT_DECISION_ATTACKERS, //p is the active player at declare-attackers with a legal attacker
      COMBAT_DECISION_BLOCKERS,  //p is the defender at the blockers step with a legal block
  };
  CombatDecision pendingCombatDecision(Player * p);
  void userRequestNextGamePhase(bool allowInterrupt = true, bool log = true);
  //W53-DELVER: a reveal/look/scry display owned by the HUMAN seat is open or
  //about to open (its ability resolved, the display not yet built). Phase
  //automation and phase-advance requests hold while it is.
  bool humanDisplayOpen();
  //W53-DELVER root cause: phase triggers (GenericTriggeredAbility::Update)
  //POLL getCurrentGamePhase once per tick; a phase entered and left inside one
  //Update is never seen by any "@each my upkeep" trigger. The automation skips
  //in gameStateBasedEffects therefore act only on a phase that has already
  //survived a full tick (mSettledPhase/Turn/Step = what the previous tick saw).
  int mSettledPhase;
  int mSettledTurn;
  int mSettledStep;
  int mPhaseTicks; //full ticks the current phase/step has survived (see gameStateBasedEffects)
  void cleanupPhase();
  void nextPlayer();

#ifdef TESTSUITE
  void loadTestSuitePlayer(int playerId, TestSuiteGame* testSuite);
#endif //TESTSUITE
  void loadPlayer(int playerId, PlayerType playerType = PLAYER_TYPE_HUMAN, int decknb=0, bool premadeDeck=false);
  virtual void loadPlayer(int playerId, Player* player);

  Player * currentPlayer;
  Player * currentActionPlayer;
  Player * isInterrupting;
  Player * opponent();
  Player * nextTurnsPlayer();
  Player * currentlyActing();
  GameObserver(WResourceManager* output = 0, JGE* input = 0);
  virtual ~GameObserver();
  void gameStateBasedEffects();
  void enchantmentStatus();
  void Affinity();
  bool AffinityNeedsUpdate;
  //#W54-H (A6b): the change epoch that gates the per-tick condition
  //machinery (lord/foreach/aslongas list maintainers, this(...) /
  //thisforeach(...) wrappers). Bumped by every game event except the
  //per-tick WEventGameStateBasedChecked marker, by every ability add/remove
  //in the action layer, by every stack push/resolve, by a phase change, by
  //ActionLayer::stuffHappened (clicks, harness pokes) and by the card
  //type-change sites that raise mPropertiesChangedSinceLastUpdate. An
  //ability whose recorded epoch equals this one has nothing new to look at
  //and skips its re-evaluation (MTGAbility::conditionEpochDue). It also
  //re-arms the Affinity cost memo (L15), which the GSB-checked event used to
  //defeat every tick. WAGIC_W54H_LEGACY=1 restores the per-tick polling.
  unsigned int mAbilityEpoch;
  void bumpAbilityEpoch()
  {
      ++mAbilityEpoch;
      AffinityNeedsUpdate = true;
  }
  void addObserver(MTGAbility * observer);
  bool removeObserver(ActionElement * observer);
  //Validate a possibly-stale MTGCardInstance pointer by POINTER COMPARISON
  //(no deref) against every zone the game still tracks, including the
  //garbage zones (normal zone moves park instances there - valid memory).
  //Returns the pointer when the game still knows it, NULL otherwise.
  //Really-deleted instances (dead tokens) come back NULL - the raw-pointer
  //dangle class (currentActionCard core 3266478, ATransformer target core
  //3151670).
  MTGCardInstance * validateCardPointer(MTGCardInstance * card);
  //TRUE once the CR pre-game phase (opening hands + London mulligan + 103.6)
  //has completed. Read by the old mulligan menu / leyline click path to close
  //themselves so there is no double action. FALSE in suite games (no phase).
  bool pregameDone() const { return mPregameDone; }
  void startGame(GameType, Rules * rules);
  void phasingPhase();
  void untapPhase();
  MTGCardInstance * isCardWaiting(){ return cardWaitingForTargets; }
  int isInPlay(MTGCardInstance *  card);
  int isInGrave(MTGCardInstance *  card);
  int isInExile(MTGCardInstance *  card);
  int isInCommandZone(MTGCardInstance *  card);
  int isInHand(MTGCardInstance *  card);
  int isInLibrary(MTGCardInstance *  card);
  int isInStack(MTGCardInstance *  card);
  virtual void Update(float dt);
  void Render();
  void ButtonPressed(PlayGuiObject*);
  int getPlayersNumber() {return players.size();};

  int receiveEvent(WEvent * event);
  bool connectRule;
  bool LPWeffect;

  void logAction(Player* player, const string& s="");
  void logAction(int playerId, const string& s="") {
      logAction(players[playerId], s);
  };
  void logAction(MTGCardInstance* card, MTGGameZone* zone, size_t index, int result);
  bool load(const string& s, bool undo = false, int controlledPlayerIndex = 0
#ifdef TESTSUITE
            , TestSuiteGame* testgame = 0
#endif
          );
  bool undo();
  bool isLoading(){ return mLoading; };
  void Mulligan(Player* player = NULL);
  void serumMulligan(Player* player = NULL);
  Player* getPlayer(size_t index) { return players[index];};
  bool isStarted() { return (mLayers!=NULL);};
  RandomGenerator* getRandomGenerator() { return &randomGenerator; };
  //#W56-E: every AIPlayer*/AIHints draw goes here, never through the game
  //stream the transcript records. WAGIC_SINGLE_RNG=1 restores the single
  //pre-change stream (the disable flag for this change).
  RandomGenerator* getAIRandomGenerator()
  {
      static const bool singleStream = (getenv("WAGIC_SINGLE_RNG") != NULL);
      return singleStream ? &randomGenerator : &aiRandomGenerator;
  };
  //#W56-E: the AI stream's seed is derived from the game seed so one seed
  //directive still pins the whole run.
  static unsigned int aiSeedFrom(unsigned int seed) { return seed ^ 0x9E3779B9u; };
  //Reseed this game's randomness (test suite "seed" directive). The ctor
  //seeds from time(0), so a test's fixed seed must be re-applied after
  //construction to make the run deterministic.
  void resetSeed(unsigned int seed) { mSeed = seed; randomGenerator.setSeed(seed); aiRandomGenerator.setSeed(aiSeedFrom(seed)); };
  WResourceManager* getResourceManager() { if(this) return mResourceManager;else return 0;};
  CardSelectorBase* getCardSelector() { return mLayers->getCardSelector();};
  bool operator==(const GameObserver& aGame);
  JGE* getInput(){return mJGE;};
  DeckManager* getDeckManager(){ return mDeckManager; };
  void dumpAssert(bool val);
  void resetStartupGame();
  void setLoser(Player* aPlayer) {
      gameOver = aPlayer;
  };

  bool didWin(Player* aPlayer = 0) const {
      if(!gameOver) {
          // nobody won
          return false;
      } else if(!aPlayer) {
          // somebody won and we don't care who
          return true;
      } else if(gameOver == aPlayer) {
          // aPlayer lost
          return false;
      } else {
          // aPlayer won
          return true;
      }
  };

  DuelLayers *getView() { return mLayers; };

};

//#W57-T (softlock escape), half A: the in-thread hang guard's unwind signal.
//Thrown by GameObserver::hangCheck when one game tick has burned its whole
//wall-clock budget inside a loop, and caught at the duel screen's per-frame
//game->Update call - the outermost per-frame call that can unwind without
//leaving a half-updated frame behind. C++ exceptions are on everywhere this
//builds (the Vita toolchain compiles with -fexceptions).
struct SoftlockAbort
{
    const char * site;
    unsigned long iterations;
    long budgetMs;
    SoftlockAbort(const char * s, unsigned long it, long ms) : site(s), iterations(it), budgetMs(ms) {}
};

//#W57-T (softlock escape). Writes a diagnostics snapshot of the live game to
//ux0:data/Wagic/softlock-<epoch>.txt (Vita) / User/softlock-<epoch>.txt
//(desktop, next to User/transcripts) and returns the path it wrote, or "" on
//failure. `trigger` names what asked for it ("menu", "testsuite", a hang-guard
//site). Also stamps the game's transcript `#classification=softlock` and
//`#softlock file=...`. Reads only live state - no game mutation - so it is
//safe to call from a menu callback, from a fixture, or from a hang-guard
//unwind. Defined in GameStateDuel.cpp (the duel screen owns this feature);
//it is not its own translation unit on purpose - a new source would mean
//regenerating Makefile.sdl and editing the Vita CMakeLists and Android.mk
//mid-wave, for a function with exactly one owner.
class GameObserver;
std::string wagicSoftlockDump(GameObserver * game, const char * trigger);
//#W57-T: per-frame heartbeat, published by the duel screen's Update (see
//GameStateDuel::Update). Two clock reads a frame; the dump renders it as a
//memlog-style `frames` line so a capture says what the frame rate was doing.
void wagicSoftlockFrameTick(float dt);
void wagicSoftlockFrameReset();

#ifdef NETWORK_SUPPORT
class NetworkGameObserver : public GameObserver
{
protected:
    JNetwork* mpNetworkSession;
    bool mSynchronized;
    bool mForwardAction;
    virtual void logAction(const string& s);
public:
    // no serverIp means a server is being instantiated, otherwise a client
    NetworkGameObserver(JNetwork* pNetwork, WResourceManager* output = 0, JGE* input = 0);
    virtual ~NetworkGameObserver();
    virtual void loadPlayer(int playerId, Player* player);
    virtual void Update(float dt);
    void synchronize();
    static void loadPlayer(void*pThis, stringstream& in, stringstream& out);
    static void sendAction(void*pThis, stringstream& in, stringstream& out);
    static void synchronize(void*pThis, stringstream& in, stringstream& out);
    static void checkSynchro(void*pxThis, stringstream& in, stringstream& out);
    static void ignoreResponse(void*, stringstream&, stringstream&){};
    static void disconnect(void*pxThis, stringstream& in, stringstream& out);
};
#endif


#endif
