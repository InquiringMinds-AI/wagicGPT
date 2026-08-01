#include "PrecompiledHeader.h"

#include "PreGamePhase.h"
#include "GameObserver.h"
#include "Player.h"
#include "MTGCardInstance.h"
#include "MTGGameZones.h"
#include "ActionStack.h"
#include "SimpleMenu.h"
#include "WFont.h"
#include "WResourceManager.h"
#include "MTGDefinitions.h"
#include "CardGui.h"
#include "GameOptions.h"

#include <algorithm>
#include <sstream>

using std::string;
using std::vector;

//A menu id distinct from the duel's own menu ids; only this listener reads it.
static const int kPregameMenuId = 9500;

PreGamePhase::PreGamePhase(GameObserver * obs)
    : observer(obs), mState(PG_DECLARE), mIdx(0), mBottomCount(0),
      mLeylineIdx(0), mLeylineBuilt(false), mDone(false),
      mMenu(NULL), mMenuResult(-1), mMenuHasResult(false), mDt(0)
{
    //Turn order: the starting player (currentPlayer, set by Rules::initGame)
    //declares first, then the opponent. mMulls/mKept/... are indexed to match.
    mOrder[0] = observer->currentPlayer;
    mOrder[1] = observer->currentPlayer->opponent();
    for (int i = 0; i < 2; i++)
    {
        mMulls[i] = 0;
        mKept[i] = false;
        mRoundDecl[i] = -1;
        mNeedBottom[i] = false;
    }
    DebugTrace("PREGAME: starting pre-game phase; starting player = seat "
               << mOrder[0]->getId() << ", opponent = seat " << mOrder[1]->getId());
}

PreGamePhase::~PreGamePhase()
{
    closeMenu();
}

void PreGamePhase::ButtonPressed(int /*controllerId*/, int controlId)
{
    //Latch only: the menu is freed in Update, never from inside this callback
    //(deleting a JGuiController from its own listener is a use-after-free).
    mMenuResult = controlId;
    mMenuHasResult = true;
}

void PreGamePhase::closeMenu()
{
    if (mMenu)
    {
        mMenu->Close();
        SAFE_DELETE(mMenu);
    }
    mMenuHasResult = false;
}

int PreGamePhase::humanChoose(const char * title, const vector<string> & items)
{
    if (!mMenu)
    {
        mMenu = NEW SimpleMenu(JGE::GetInstance(), WResourceManager::Instance(),
                               kPregameMenuId, this, Fonts::MENU_FONT,
                               SCREEN_WIDTH / 2 - 100, 25, title);
        for (size_t i = 0; i < items.size(); i++)
            mMenu->Add((int) i, items[i]);
        mMenuHasResult = false;
    }
    mMenu->Update(mDt);
    if (mMenuHasResult)
    {
        int r = mMenuResult;
        closeMenu();
        return r;
    }
    return Player::PREGAME_PENDING;
}

MTGCardInstance * PreGamePhase::findSerumPowder(Player * p)
{
    MTGGameZone * h = p->game->hand;
    for (int i = 0; i < h->nb_cards; i++)
    {
        string n = h->cards[i]->name;
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if (n == "serum powder")
            return h->cards[i];
    }
    return NULL;
}

void PreGamePhase::buildLeylines(Player * p)
{
    mLeylines.clear();
    MTGGameZone * h = p->game->hand;
    for (int i = 0; i < h->nb_cards; i++)
    {
        MTGCardInstance * c = h->cards[i];
        if (c->basicAbilities[(int) Constants::LEYLINE])
            mLeylines.push_back(c);
    }
}

void PreGamePhase::placeOnBattlefield(Player * p, MTGCardInstance * card)
{
    if (!card || !p->game->hand->hasCard(card))
        return;
    //Mirror MTGPutInPlayRule's leyline path: move to temp, resolve a Spell so
    //the permanent enters the battlefield with its static abilities live.
    MTGCardInstance * copy = p->game->putInZone(card, card->currentZone, p->game->temp);
    if (!copy)
        return;
    Spell * spell = NEW Spell(observer, copy);
    spell->resolve();
    delete spell;
}

void PreGamePhase::Update(float dt)
{
    mDt = dt;
    if (mDone)
        return;

    switch (mState)
    {
    case PG_DECLARE:
    {
        while (mIdx < 2 && mKept[mIdx])
            mIdx++;
        if (mIdx >= 2)
        {
            mState = PG_EXECUTE;
            return;
        }
        Player * p = mOrder[mIdx];
        //Mulligan-to-zero floor: a player who has already mulliganed down to a
        //zero-card keep may not mulligan again (CR 103.5) -> forced keep.
        if (mMulls[mIdx] >= p->startingHandSize())
        {
            mRoundDecl[mIdx] = 0;
            mIdx++;
            return;
        }
        int decision;
        if (!p->isAI())
        {
            MTGCardInstance * serum = findSerumPowder(p);
            vector<string> items;
            items.push_back("Keep this hand");
            {
                std::ostringstream m;
                m << "Mulligan (draw " << p->startingHandSize() << ", bottom "
                  << (mMulls[mIdx] + 1) << " on keep)";
                items.push_back(m.str());
            }
            if (serum)
                items.push_back("Use Serum Powder (exile hand, redraw)");
            int r = humanChoose("Opening hand: keep or mulligan?", items);
            if (r == Player::PREGAME_PENDING)
                return;
            if (serum && r == 2)
            {
                //Serum Powder (CR 103.5b): exile hand, redraw the same size, in
                //addition to mulligans (does not count as one). Stay in DECLARE
                //and re-ask this player.
                p->serumMulligan();
                return;
            }
            decision = r;
        }
        else
        {
            decision = p->pregameMulliganDecision(mMulls[mIdx]);
            if (decision == Player::PREGAME_PENDING)
                return;
            if (decision < 0)
                decision = 0;
        }
        mRoundDecl[mIdx] = decision;
        mIdx++;
        return;
    }

    case PG_EXECUTE:
    {
        for (int i = 0; i < 2; i++)
        {
            if (mKept[i])
                continue;
            if (mRoundDecl[i] == 1)
            {
                mOrder[i]->takeLondonMulligan();
                mMulls[i]++;
                DebugTrace("PREGAME: player " << i << " (seat " << mOrder[i]->getId()
                           << ") takes mulligan #" << mMulls[i]
                           << ", redrew " << mOrder[i]->game->hand->nb_cards << " cards");
            }
            else
            {
                mKept[i] = true;
                mNeedBottom[i] = (mMulls[i] > 0);
                DebugTrace("PREGAME: player " << i << " (seat " << mOrder[i]->getId()
                           << ") KEEPS after " << mMulls[i] << " mulligan(s); will bottom "
                           << mMulls[i] << " card(s)");
            }
            mRoundDecl[i] = -1;
        }
        if (mNeedBottom[0] || mNeedBottom[1])
        {
            mState = PG_BOTTOM;
            mIdx = 0;
            mBottomCount = 0;
        }
        else if (mKept[0] && mKept[1])
        {
            mState = PG_LEYLINE;
            mIdx = 0;
            mLeylineBuilt = false;
            mLeylineIdx = 0;
        }
        else
        {
            mState = PG_DECLARE;
            mIdx = 0;
        }
        return;
    }

    case PG_BOTTOM:
    {
        while (mIdx < 2 && !mNeedBottom[mIdx])
            mIdx++;
        if (mIdx >= 2)
        {
            if (mKept[0] && mKept[1])
            {
                mState = PG_LEYLINE;
                mIdx = 0;
                mLeylineBuilt = false;
                mLeylineIdx = 0;
            }
            else
            {
                mState = PG_DECLARE;
                mIdx = 0;
            }
            return;
        }
        Player * p = mOrder[mIdx];
        //N-139i (wave-31): the clamp must be against the hand size at the START
        //of bottoming, not the CURRENT one - the hand shrinks by one with every
        //card bottomed, so clamping live walked the target down alongside the
        //progress counter and the loop met itself in the middle. With 7
        //mulligans the seat bottomed 4 of 7 and kept a 3-card hand ("1 of 7",
        //"2 of 6", "3 of 5", "4 of 4", stop) - a CR 103.5 violation, and it fed
        //the AI seam a DIFFERENT N on every ask. Cards already bottomed count
        //toward what was available, so the target stays put.
        int need = bottomTarget(mMulls[mIdx], mBottomCount, p->game->hand->nb_cards);
        if (mBottomCount >= need)
        {
            mNeedBottom[mIdx] = false;
            mBottomCount = 0;
            mIdx++;
            return;
        }
        MTGCardInstance * card = NULL;
        if (!p->isAI())
        {
            vector<string> items;
            for (int i = 0; i < p->game->hand->nb_cards; i++)
                items.push_back(p->game->hand->cards[i]->name);
            std::ostringstream t;
            t << "Bottom a card (" << (mBottomCount + 1) << " of " << need << ")";
            int r = humanChoose(t.str().c_str(), items);
            if (r == Player::PREGAME_PENDING)
                return;
            if (r >= 0 && r < p->game->hand->nb_cards)
                card = p->game->hand->cards[r];
        }
        else
        {
            int status = 0;
            card = p->pregameChooseBottom(need, mBottomCount, status);
            if (status == Player::PREGAME_PENDING)
                return;
            if (!card && p->game->hand->nb_cards)
                card = p->game->hand->cards[p->game->hand->nb_cards - 1];
        }
        if (card)
        {
            DebugTrace("PREGAME: player " << mIdx << " (seat " << p->getId()
                       << ") bottoms '" << card->name << "' (" << (mBottomCount + 1)
                       << " of " << need << "); library bottom card now '"
                       << card->name << "'");
            p->bottomCardToLibrary(card);
            mBottomCount++;
        }
        return;
    }

    case PG_LEYLINE:
    {
        if (mIdx >= 2)
        {
            mState = PG_DONE;
            mDone = true;
            DebugTrace("PREGAME: pre-game phase complete; turn 1 begins (game opens at"
                       " the untap step; the starting player skips only its first draw)");
            return;
        }
        Player * p = mOrder[mIdx];
        if (!mLeylineBuilt)
        {
            buildLeylines(p);
            mLeylineBuilt = true;
            mLeylineIdx = 0;
        }
        while (mLeylineIdx < (int) mLeylines.size()
               && !p->game->hand->hasCard(mLeylines[mLeylineIdx]))
            mLeylineIdx++;
        if (mLeylineIdx >= (int) mLeylines.size())
        {
            mIdx++;
            mLeylineBuilt = false;
            mLeylineIdx = 0;
            return;
        }
        MTGCardInstance * c = mLeylines[mLeylineIdx];
        int yes;
        if (!p->isAI())
        {
            vector<string> items;
            items.push_back(string("Yes, begin with ") + c->name + " on the battlefield");
            items.push_back("No, keep it in hand");
            int r = humanChoose("Pre-game action: put a Leyline into play?", items);
            if (r == Player::PREGAME_PENDING)
                return;
            yes = (r == 0) ? 1 : 0;
        }
        else
        {
            int d = p->pregameLeylineDecision(c);
            if (d == Player::PREGAME_PENDING)
                return;
            yes = (d == 1) ? 1 : 0;
        }
        if (yes)
        {
            DebugTrace("PREGAME: player " << mIdx << " (seat " << p->getId()
                       << ") begins the game with leyline '" << c->name
                       << "' on the battlefield (103.6a)");
            placeOnBattlefield(p, c);
        }
        mLeylineIdx++;
        return;
    }

    case PG_DONE:
    default:
        mDone = true;
        return;
    }
}

void PreGamePhase::Render()
{
    if (mDone)
        return;
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
    if (font)
    {
        font->SetScale(DEFAULT_MAIN_FONT_SCALE);
        font->SetColor(ARGB(230, 255, 255, 200));
        font->DrawString("-- Pre-game: opening hands --", SCREEN_WIDTH / 2, 2, JGETEXT_CENTER);
    }
    //The keep/mulligan/bottoming/leyline menus are modal, so the hand-reveal key
    //never reaches the hand layer and the choice would be blind. Render the
    //deciding (human) player's hand under the menu.
    if (mMenu && mIdx < 2)
    {
        Player * p = mOrder[mIdx];
        if (p && p->isHuman() && p->game->hand->nb_cards)
        {
            MTGGameZone * h = p->game->hand;
            const float scale = 0.32f;
            const float cardW = scale * 200 + 2;
            float spacing = cardW;
            float total = spacing * h->nb_cards;
            if (total > SCREEN_WIDTH - 20 && h->nb_cards > 1)
            {
                spacing = (SCREEN_WIDTH - 20 - cardW) / (h->nb_cards - 1);
                total = spacing * (h->nb_cards - 1) + cardW;
            }
            float x = (SCREEN_WIDTH - total) / 2 + cardW / 2;
            int mode = options[Options::DISABLECARDS].number ? DrawMode::kText : DrawMode::kNormal;
            for (int i = 0; i < h->nb_cards; i++)
            {
                Pos pos(x + i * spacing, 212, scale * 285 / 250, 0.0, 255);
                CardGui::DrawCard(h->cards[i], pos, mode, true);
            }
        }
    }
    if (mMenu)
        mMenu->Render();
}
