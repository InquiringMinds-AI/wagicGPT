#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "JGE.h"
#include "MTGGameZones.h"
#include "Damage.h"
#include "Targetable.h"

class MTGDeck;
class MTGPlayerCards;
class MTGInPlay;
class ManaPool;

class Player: public Damageable
{
protected:
    ManaPool * manaPool;
    JTexture * mAvatarTex;
    JQuadPtr mAvatar;
    bool loadAvatar(string file, string resName = "playerAvatar");
    bool premade;

public:
    enum Mode
    {
        MODE_TEST_SUITE,
        MODE_HUMAN,
        MODE_AI
    };

    int deckId;
    string mAvatarName;
    Mode playMode;
    bool nomaxhandsize;
    MTGPlayerCards * game;
    MTGDeck * mDeck;
    string deckFile;
    string deckFileSmall;
    string deckName;
    string phaseRing;
    int offerInterruptOnPhase;
    int skippingTurn;
    int extraTurn;
    int drawCounter;
    int energyCount;
    int experienceCount;
    int yidaroCount;
    int ringTemptations;
    int dungeonCompleted;
    int numOfCommandCast;
    int monarch;
    int initiative;
    int surveilOffset;
    int devotionOffset;
    int lastShuffleTurn;
    //Number of cards this player exiled via Serum Powder's redraw. The
    //opening-hand rules (Leylines, the Mulligan menu) detect the pre-game
    //window by "graveyard and exile are empty"; Serum Powder legitimately
    //fills exile before the game starts, so those checks compare against
    //this count instead of zero.
    int exiledBySerum;
    int epic;
    int forcefield;
    int dealsdamagebycombat;
    int initLife;
    int raidcount;
    int cycledCount;
    int handmodifier;
    int snowManaG;
    int snowManaR;
    int snowManaB;
    int snowManaU;
    int snowManaW;
    int snowManaC;
    string lastChosenName;
    vector<string> prowledTypes;
    Player(GameObserver *observer, string deckFile, string deckFileSmall, MTGDeck * deck = NULL);
    virtual ~Player();
    virtual void setObserver(GameObserver*g);
    virtual void End();
    virtual int displayStack()
    {
        return 1;
    }
    const string getDisplayName() const;

    int afterDamage();

    // Added source of life gain/loss in order to check later a possible exception.
    int gainLife(int value, MTGCardInstance* source);
    int loseLife(int value, MTGCardInstance* source);
    int gainOrLoseLife(int value, MTGCardInstance* source);

    bool isPoisoned() {return (poisonCount > 0);}
    int poisoned();
    int damaged();
    int prevented();
    void unTapPhase();
    MTGInPlay * inPlay();
    ManaPool * getManaPool();
    void takeMulligan();
    void serumMulligan();
    //London mulligan (CR 103.5): shuffle the WHOLE hand back into the library
    //and draw a fresh starting hand of full size. The London shrink is applied
    //later as bottoming at KEEP time (bottomCardToLibrary), not here.
    void takeLondonMulligan();
    //Put one card from hand onto the BOTTOM of the library (London bottoming /
    //any-order pre-game placement).
    void bottomCardToLibrary(MTGCardInstance * card);
    //Starting hand size (CR 103.5, default 7). Keyed here so draw count, redraw
    //count, and the mulligan-to-zero floor all read one value.
    int startingHandSize() { return 7; }

    //Sentinel returned by the pre-game AI decision hooks while an asynchronous
    //model call is still in flight (same value as AIPlayerGPT::kChoicePending):
    //PreGamePhase unwinds for the tick and re-polls next frame.
    static const int PREGAME_PENDING = -2;
    //Pre-game (opening-hand) decisions for an AI seat, consulted by PreGamePhase
    //ONLY for AI players (a human's choices come from the phase's own menus).
    //Base defaults (non-interactive seats never actually reach these): keep, no
    //bottom preference, start leylines on the battlefield.
    // returns 0 = keep, 1 = mulligan, PREGAME_PENDING = model call in flight
    virtual int pregameMulliganDecision(int mullsTaken) { (void) mullsTaken; return 0; }
    // choose ONE hand card to put on the bottom. Returns a card currently in
    // hand, or NULL for no preference. `status` set to PREGAME_PENDING while a
    // model call is in flight.
    virtual MTGCardInstance * pregameChooseBottom(int need, int chosenSoFar, int & status)
    { (void) need; (void) chosenSoFar; status = 0; return NULL; }
    // returns 1 = begin the game with this leyline on the battlefield,
    // 0 = keep it in hand, PREGAME_PENDING while a model call is in flight
    virtual int pregameLeylineDecision(MTGCardInstance * card) { (void) card; return 1; }
    bool hasPossibleAttackers();
    bool noPossibleAttackers();
    //Engine game-over hook, called once the duel's outcome is decided
    //(GameStateDuel's didWin transition). Idempotent by contract - the
    //transition frame can repeat. Base: nothing; AIPlayerGPT closes its
    //decision translog with a "gameend" record.
    virtual void gameEnded() {}
    bool DeadLifeState(bool check = false);
    ManaCost * doesntEmpty;
    ManaCost * poolDoesntEmpty;
    ManaCost * AuraIncreased;
    ManaCost * AuraReduced;
    void cleanupPhase();
    virtual int Act(float)
    {
        return 0;
    }

    virtual int isAI()
    {
        return 0;
    }

    //An AI that can answer interactive prompts (menus, target choosers)
    //like a human. Card data gates decisions on "ishuman" to route AI
    //players onto dice-roll heuristic lines instead of interactive ones -
    //an LLM player must take the interactive path (those gates encode the
    //heuristic AI's limits, not the game's rules).
    virtual bool isInteractiveAI() const
    {
        return false;
    }

    bool isHuman()
    {
        return (playMode == MODE_HUMAN);
    }

    //Interactive reveal/scry decision for an interactive AI (the LLM
    //opponent). When such a player controls a reveal/surveil/scry display,
    //the display delegates the whole look-and-choose to this seam instead of
    //the headless aicode substitute (a moverandom heuristic that cannot
    //look-and-choose). ONE bundled decision: the model is shown EVERY revealed
    //card and picks which ones go to "option one" (surveil: the graveyard);
    //the rest default to option two (surveil: top of the library). Returns
    //1 = decided (selForOptionOne holds indices into 'revealed'),
    //0 = the model call is in flight (wait, act on no card this tick),
    //-1 = not handled / model failed (the display uses its safe default:
    //     select nothing, so every revealed card takes option two). The base
    //returns -1; only an interactive AI (AIPlayerGPT) overrides it.
    //eligibleForOptionOne (optional; same size/order as revealed) carries the
    //engine's per-card canTarget verdict for option one's filter, so a filtered
    //tutor/dig can surface which revealed cards actually qualify. The base
    //ignores it (headless AIs never reach this override).
    //revealSource: 0 = top of library (surveil/dig), 1 = the OPPONENT's hand
    //(Thoughtseize/Duress-class targeted discard, from this decider's seat),
    //2 = the decider's OWN hand. pickExactlyOne: option one's chooser is a
    //fixed pick-EXACTLY-ONE (<1>, targetMin) rather than a subset (<upto:N> /
    //<anyamount>) - drives choose-ONE framing instead of choose-a-subset.
    virtual int decideReveal(const vector<MTGCardInstance*>& revealed,
                             const string& optOneLabel, const string& optTwoLabel,
                             const string& optOneEffect,
                             vector<int>& selForOptionOne,
                             const vector<bool>& eligibleForOptionOne = vector<bool>(),
                             int revealSource = 0, bool pickExactlyOne = false)
    {
        (void) revealed; (void) optOneLabel; (void) optTwoLabel;
        (void) optOneEffect; (void) selForOptionOne; (void) eligibleForOptionOne;
        (void) revealSource; (void) pickExactlyOne;
        return -1;
    }

    Player * opponent();
    int getId();
    JQuadPtr getIcon();

    virtual int receiveEvent(WEvent *)
    {
        return 0;
    }

    virtual void Render()
    {
    }

    /**
    ** Returns the path to the stats file of currently selected deck. 
    */
    std::string GetCurrentDeckStatsFile();
    virtual bool parseLine(const string& s);
    friend ostream& operator<<(ostream&, const Player&);
    friend istream& operator>>(istream&, Player&);
    bool operator<(Player& aPlayer);
    bool isDead();
};

class HumanPlayer: public Player
{
public:
    HumanPlayer(GameObserver *observer, string deckFile, string deckFileSmall, bool premade = false, MTGDeck * deck = NULL);
    void End();
    friend ostream& operator<<(ostream&, const HumanPlayer&);
};

#endif
