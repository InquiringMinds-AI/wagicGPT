#ifndef _PREGAMEPHASE_H_
#define _PREGAMEPHASE_H_

/*
 *  PreGamePhase: the Comprehensive Rules pre-game procedure (CR 103.5 + 103.6),
 *  run BEFORE turn 1 of a real/selfplay/demo duel. Suite games seed their hands
 *  via INIT and assume the old start, so they never build this phase.
 *
 *  Sequence (see projects/mtg/strategy-design/pregame-mulligan/rules-spec.md):
 *   - Opening hands are already drawn by the rules' draw:7 (GameObserver::
 *     startGame -> Rules::initGame). This phase then runs the London mulligan
 *     loop and the 103.6 pre-game actions.
 *   - 103.5 London: declarations are TURN-ORDERED (starting player first);
 *     executions are simultaneous per round; a KEEP is terminal per player.
 *     Every mulligan reshuffles the whole hand and redraws the FULL starting
 *     hand (7); at KEEP a player bottoms N cards (N = mulligans taken), any
 *     order. Mulligan-to-zero is legal.
 *   - 103.6: AFTER all keeps, the starting player takes all their pre-game
 *     actions, then the opponent. 103.6a `leyline`: a hand card with the
 *     leyline keyword MAY begin the game on the battlefield. The window is
 *     built generically so 103.6b (Chancellor reveals) can be added later.
 *
 *  Decision surfaces (all three consumers):
 *   - Human: SimpleMenu prompts (keep/mulligan, per-card bottoming, per-leyline
 *     may), keyboard/d-pad operable like every other duel menu.
 *   - Baka: the AIPlayerBaka pregame* heuristics (deterministic, suite-safe).
 *   - AIPlayerGPT: the pregame* model hooks (async; fall back to Baka on any
 *     failure so an AI that never answers cannot deadlock the phase).
 *
 *  While this phase is active GameObserver::Update is gated (the normal game
 *  loop does not run); GameObserver::Render draws the board plus this phase's
 *  menu overlay. On completion GameObserver::mPregameDone is set, which closes
 *  the old in-game mulligan menu entry and the old leyline click path.
 */

#include "JGui.h"
#include <vector>

class GameObserver;
class Player;
class MTGCardInstance;
class SimpleMenu;

class PreGamePhase : public JGuiListener
{
public:
    PreGamePhase(GameObserver * observer);
    virtual ~PreGamePhase();

    void Update(float dt);
    void Render();
    bool isDone() const { return mDone; }

    //SimpleMenu callback: record the pressed item; never delete the menu here
    //(the codebase's use-after-free trap) - it is freed in Update.
    virtual void ButtonPressed(int controllerId, int controlId);

private:
    enum State
    {
        PG_DECLARE,  //turn-ordered keep/mulligan declarations, one round
        PG_EXECUTE,  //apply this round's mulligans simultaneously; finalize keeps
        PG_BOTTOM,   //players who just kept bottom N cards (turn order)
        PG_LEYLINE,  //103.6 pre-game actions: leylines, starting player first
        PG_DONE
    };

    GameObserver * observer;
    State mState;
    Player * mOrder[2];   //[0] = starting player, [1] = opponent
    int  mMulls[2];       //mulligans taken per player (index matches mOrder)
    bool mKept[2];        //player has kept (terminal)
    int  mRoundDecl[2];   //this round's declaration: -1 none, 0 keep, 1 mulligan
    int  mIdx;            //per-state iterator (declarer / bottom / leyline player)
    int  mBottomCount;    //cards bottomed so far for the current bottom player
    bool mNeedBottom[2];  //player kept this round and still owes bottoming
    std::vector<MTGCardInstance*> mLeylines; //current leyline player's candidates
    int  mLeylineIdx;
    bool mLeylineBuilt;   //mLeylines built for the current player
    bool mDone;

    //Human-menu plumbing: one live menu at a time; its result is latched by
    //ButtonPressed and consumed in Update.
    SimpleMenu * mMenu;
    int  mMenuResult;
    bool mMenuHasResult;
    float mDt;

    //Drive a human choice through a SimpleMenu. Returns the 0-based chosen item,
    //or Player::PREGAME_PENDING while the menu is still open.
    int humanChoose(const char * title, const std::vector<std::string> & items);
    void closeMenu();

    //Does this hand hold a Serum Powder (autohand serumpowder)? Detected by
    //card name; used only to offer the human its declaration-window redraw.
    MTGCardInstance * findSerumPowder(Player * p);

    //Put a leyline (or any hand card) directly onto the battlefield, mirroring
    //MTGPutInPlayRule's leyline path (temp zone + Spell::resolve), so static
    //abilities come online exactly as the in-game path did.
    void placeOnBattlefield(Player * p, MTGCardInstance * card);

    //Build the leyline candidate list for a player (103.6a; generic enough that
    //103.6b reveal cards can be folded in here later).
    void buildLeylines(Player * p);
};

#endif
