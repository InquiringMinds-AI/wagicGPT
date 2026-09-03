#include "PrecompiledHeader.h"

#include "CardSelector.h"
#include "CardSelector.h"
#include "GameApp.h"
#include "Trash.h"
#include "GuiHand.h"
#include <sstream>
#include "OptionItem.h"
#include "LegalActions.h"
#include "ManaEngine.h"
#ifdef WITH_GPT_AI
#include "GptConfig.h"
#endif

const float GuiHand::ClosedRowX = 459;
const float GuiHand::LeftRowX = 420;
const float GuiHand::RightRowX = 460;

const float GuiHand::OpenX = 394;
const float GuiHand::ClosedX = 494;
const float GuiHand::OpenY = SCREEN_HEIGHT - 50;
const float GuiHand::ClosedY = SCREEN_HEIGHT;

bool HandLimitor::select(Target* t)
{
    if (CardView* c = dynamic_cast<CardView*>(t))
        return hand->isInHand(c);
    else
        return false;
}
bool HandLimitor::greyout(Target*)
{
    return true;
}
HandLimitor::HandLimitor(GuiHand* hand) :
    hand(hand)
{
}

GuiHand::GuiHand(GameObserver* observer, MTGHand* hand) :
    GuiLayer(observer), hand(hand)
{
    if(observer->getResourceManager())
    {
        back = observer->getResourceManager()->RetrieveTempQuad("handback.png");
        if (back.get())
            back->SetTextureRect(1, 0, 100, 250);
        else
            GameApp::systemError = "Error loading hand texture : " __FILE__;
    }
}

GuiHand::~GuiHand()
{
        for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
            delete (*it);
}

void GuiHand::Update(float dt)
{
    for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
        (*it)->Update(dt);
}

bool GuiHand::isInHand(CardView* card)
{
    vector<CardView*>::iterator it;
    it = find(cards.begin(), cards.end(), card);
    return (it != cards.end());
}

GuiHandOpponent::GuiHandOpponent(GameObserver* observer, MTGHand* hand) :
    GuiHand(observer, hand)
{
    vector<MTGCardInstance *>::iterator ite;
    for(ite = hand->cards.begin(); ite != hand->cards.end(); ite++)
    {
        WEventZoneChange event(*ite, NULL, hand);
        receiveEventPlus(&event);
    }
}

void GuiHandOpponent::Render()
{
    JQuadPtr quad = WResourceManager::Instance()->GetQuad(kGenericCardThumbnailID);

    float x = 45;
    for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
    {
        (*it)->x = x;
        (*it)->y = 2;
        (*it)->zoom = 0.3f;
#ifdef WITH_GPT_AI
        //Evaluation peek (config peek=1 / WAGIC_GPT_PEEK): draw the AI's
        //hand face-up so a human can judge the choices it is picking from.
        if (gptPeekOpponentHand())
            (*it)->Render();
        else
#endif
        (*it)->Render(quad.get());
        if(cards.size() > 12)
            x += 240/cards.size();
        else
            x += 18;
    }
}

GuiHandSelf::GuiHandSelf(GameObserver* observer, MTGHand* hand) :
    GuiHand(observer, hand), state(Closed), backpos(ClosedX, SCREEN_HEIGHT - 250, 1.0, 0, 255), mCastableRefresh(0),
    mDisplayDirty(true)
{
    limitor = NEW HandLimitor(this);
    if (OptionHandDirection::HORIZONTAL == options[Options::HANDDIRECTION].number)
    {
        backpos.t = M_PI / 2;
        backpos.y = ClosedY;
        backpos.x = SCREEN_WIDTH - 30 * 7 - 14;
        backpos.UpdateNow();
    }

    vector<MTGCardInstance *>::iterator ite;
    for(ite = hand->cards.begin(); ite != hand->cards.end(); ite++)
    {
        WEventZoneChange event(*ite, NULL, hand);
        receiveEventPlus(&event);
    }
}

GuiHandSelf::~GuiHandSelf()
{
    SAFE_DELETE(limitor);
}

void GuiHandSelf::Repos()
{
    float y = 48.0;
    if (Closed == state && OptionClosedHand::VISIBLE == options[Options::CLOSEDHAND].number)
    {
        float dist = 180.0f / cards.size();
        if (dist > 20)
            dist = 20.0;
        else
            y = 40.0;
        for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
        {
            (*it)->x = ClosedRowX;
            (*it)->y = y;
            y += dist;
        }
    }
    else
    {
        bool q = (Closed == state);
        if (OptionHandDirection::HORIZONTAL == options[Options::HANDDIRECTION].number)
        {
            y = SCREEN_WIDTH - 30;
            float dist = 240.0f / cards.size();
            if (dist > 30)
                dist = 30;
            else
                y = SCREEN_WIDTH - 15;
            for (vector<CardView*>::reverse_iterator it = cards.rbegin(); it != cards.rend(); ++it)
            {
                (*it)->x = y;
                (*it)->y = SCREEN_HEIGHT - 30;
                y -= dist;
                (*it)->alpha = static_cast<float> (q ? 0 : 255);
            }
            backpos.x = y + SCREEN_HEIGHT - 14;
        }
        else
        {
            float dist = 224.0f / ((cards.size() + 1) / 2);
            if (dist > 65)
                dist = 65;
            bool flip = false;
            for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
            {
                (*it)->x = flip ? RightRowX : LeftRowX;
                (*it)->y = y;
                if (flip)
                    y += dist;
                flip = !flip;
                (*it)->alpha = static_cast<float> (q ? 0 : 255);
            }
        }
    }
}

bool GuiHandSelf::CheckUserInput(JButton key)
{
    JButton trigger = (options[Options::REVERSETRIGGERS].number ? JGE_BTN_PREV : JGE_BTN_NEXT);
    if (trigger == key)
    {
        state = (Open == state ? Closed : Open);
        if (Open == state)
            observer->getCardSelector()->Push();
        observer->getCardSelector()->Limit(Open == state ? limitor : NULL, CardView::handZone);
        if (Closed == state)
            observer->getCardSelector()->Pop();
        if (OptionHandDirection::HORIZONTAL == options[Options::HANDDIRECTION].number)
            backpos.y = Open == state ? OpenY : ClosedY;
        else
            backpos.x = Open == state ? OpenX : ClosedX;
        if (Open == state && OptionClosedHand::INVISIBLE == options[Options::CLOSEDHAND].number)
        {
            if (OptionHandDirection::HORIZONTAL == options[Options::HANDDIRECTION].number)
                for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
                {
                    (*it)->y = SCREEN_HEIGHT + 30;
                    (*it)->UpdateNow();
                }
            else
                for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
                {
                    (*it)->x = SCREEN_WIDTH + 30;
                    (*it)->UpdateNow();
                }
        }
        Repos();
        return true;
    }
    return false;
}

void GuiHandSelf::Update(float dt)
{
    backpos.Update(dt);
    GuiHand::Update(dt);

    //Castability display: refresh which hand cards could legally be played
    //right now. Uses the PURE oracle - probing the rules layer's
    //isReactingToClick from here both mutates state (Leyline auto-resolve)
    //and drags non-reentrant parse paths into the threaded test suite.
    //The verdicts land on the CARD (castableNow) and render in
    //CardGui::Render, so they show no matter which layer draws the card
    //(CardSelector re-renders the focused card on top of this layer).
    mCastableRefresh -= dt;
    //#W54-K (A4): the flags this refresh writes (castableNow, canAttackNow,
    //canBlockNow, hasUsableAbilityNow, willPayForFocused) are read only by
    //CardGui::Render for a human at the controls. An AI-owned hand (self-
    //play, the demo, every suite seat) has nobody reading them - skip the
    //oracles entirely; the flags stay at their neutral 0.
    if (mCastableRefresh <= 0 && hand && hand->owner && hand->owner->isAI())
        mCastableRefresh = 0.25f;
    if (mCastableRefresh <= 0 && hand && hand->owner)
    {
        mCastableRefresh = 0.25f;
        Player * p = hand->owner;
        MTGGameZone * bf = p->game->inPlay;
        MTGCardInstance * focused = NULL;
        for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
            if ((*it)->card && (*it)->mHasFocus)
                focused = (*it)->card;
        //#W54-K (A4): change-driven recompute. On a static board (the player
        //reading the screen) every 0.25 s refresh produced the same verdicts;
        //re-run the oracles only when an event landed since the last one or
        //the cheap state signature moved. The signature is everything the
        //oracles read that can change without an event reaching this layer:
        //phase/turn/acting player, stack depth, the number of live ability
        //objects, the mana pool, and the hand/battlefield card identities
        //with their tap state.
        std::ostringstream state;
        state << observer->getCurrentGamePhase() << '|' << observer->turn << '|'
              << (const void *) observer->currentPlayer << '|' << (const void *) observer->currentlyActing() << '|'
              << observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) << '|'
              << observer->mLayers->actionLayer()->mObjects.size() << '|'
              << p->getManaPool()->toString() << '|';
        for (int i = 0; i < p->game->hand->nb_cards; i++)
            state << (const void *) p->game->hand->cards[i] << ',';
        state << '|';
        for (int i = 0; i < bf->nb_cards; i++)
            state << (const void *) bf->cards[i] << (bf->cards[i]->isTapped() ? 't' : 'u');
        std::string stateSig = state.str();
        static const bool refreshAlways = getenv("WAGIC_HAND_REFRESH_ALWAYS") != NULL;
        bool changed = refreshAlways || mDisplayDirty || stateSig != mDisplaySig;
        mDisplayDirty = false;
        mDisplaySig = stateSig;
        if (changed)
        {
            std::set<MTGCardInstance*> ok = LegalActionsOracle::castableForDisplay(p);
            for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
            {
                MTGCardInstance * c = (*it)->card;
                if (!c)
                    continue;
                c->castableNow = ok.count(c) ? 1 : -1;
            }
            //#W53-S perf (owner Vita report, vpk11: "it became almost frozen when
            //attempting to select cards in my hand"). The usable-ability verdict is
            //the expensive one - it prices every activation against the board's
            //potential mana - and asking it PER CARD rebuilt that potential and
            //re-walked every ability object once per permanent. One batch pass
            //answers it for the whole battlefield with identical semantics.
            std::set<MTGCardInstance*> usable = LegalActionsOracle::usableAbilityCards(p);
            for (int i = 0; i < bf->nb_cards; i++)
            {
                //Availability signals for the battlefield, refreshed on the same
                //tick as castability so the whole board tells the player the
                //same story at the same moment. Both are display-only: the
                //engine still decides what is legal when the button is pressed.
                bf->cards[i]->canAttackNow =
                    LegalActionsOracle::canDeclareAttacker(bf->cards[i]) ? 1 : 0;
                bf->cards[i]->hasUsableAbilityNow = usable.count(bf->cards[i]) ? 1 : 0;
                bf->cards[i]->canBlockNow =
                    LegalActionsOracle::canDeclareBlocker(bf->cards[i]) ? 1 : 0;
            }
        }
        //Tap preview: mark the producers the auto-tap plan would activate
        //for the focused hand card (rendered as a border on the
        //battlefield cards, cleared every refresh).
        for (int i = 0; i < bf->nb_cards; i++)
            bf->cards[i]->willPayForFocused = 0;
        if (focused && focused->castableNow == 1 && focused->getManaCost()
            && !p->getManaPool()->canAfford(focused->getManaCost(), focused->has(Constants::ANYTYPEOFMANA)))
        {
            //#W55-OPT perf: the option-preserving plan (refineForOptions) is
            //the costly part of this refresh - re-plan only when something it
            //reads changed (focused card, pool, hand, battlefield tap state);
            //the console re-ran it four times a second on a static board.
            std::ostringstream sig;
            sig << (const void *) focused << '|' << stateSig;
            if (sig.str() != mPreviewSig)
            {
                mPreviewSig = sig.str();
                mPreviewSources.clear();
                vector<MTGAbility*> picks = ManaEngine::selectAutoTapProducers(p, focused,
                    focused->getManaCost(), focused->has(Constants::ANYTYPEOFMANA));
                for (size_t i = 0; i < picks.size(); i++)
                    if (picks[i]->source)
                        mPreviewSources.push_back(picks[i]->source);
            }
            for (size_t i = 0; i < mPreviewSources.size(); i++)
                if (bf->hasCard(mPreviewSources[i]))
                    mPreviewSources[i]->willPayForFocused = 1;
        }
        else
            mPreviewSig.clear();
    }
}

void GuiHandSelf::Render()
{
    //Empty hand
    if (state == Open && cards.size() == 0)
    {
        WFont * mFont = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
        mFont->SetColor(ARGB(255,255,0,0));
        if (OptionHandDirection::HORIZONTAL == options[Options::HANDDIRECTION].number)
        {
            back->SetColor(ARGB(255,255,0,0));
            JRenderer::GetInstance()->RenderQuad(back.get(), backpos.actX, backpos.actY, backpos.actT, backpos.actZ, backpos.actZ);
            back->SetColor(ARGB(255,255,255,255));
            mFont->DrawString("0", SCREEN_WIDTH - 10, backpos.actY);
        }
        else
            backpos.Render(back.get());
        return;
    }

    backpos.Render(back.get());
    if (OptionClosedHand::VISIBLE == options[Options::CLOSEDHAND].number || state == Open)
        for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
            (*it)->Render();
    //castability signals render in CardGui::Render (castableNow), so they
    //survive card art AND the CardSelector's re-render of the focused card
}

float GuiHandSelf::LeftBoundary()
{
    float min = SCREEN_WIDTH + 10;
    if (OptionClosedHand::VISIBLE == options[Options::CLOSEDHAND].number || state == Open)
        for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
            if ((*it)->x - CardGui::Width / 2 < min)
                min = (*it)->x - CardGui::Width / 2;
    return min;
}

int GuiHandSelf::receiveEventPlus(WEvent* e)
{
    mDisplayDirty = true; //#W54-K (A4): any game event may change the availability display
    if (WEventZoneChange* ev = dynamic_cast<WEventZoneChange*>(e))
        if (hand == ev->to)
        {
            CardView* card;
            if (ev->card->view)
            {

                //fix for http://code.google.com/p/wagic/issues/detail?id=462.
                // We don't want a card in the hand to have an alpha of 0
                ev->card->view->alpha = 255;

                card = NEW CardView(CardView::handZone, ev->card, *(ev->card->view));
            }
            else
                card = NEW CardView(CardView::handZone, ev->card, ClosedRowX, 0);
            card->t = 6 * M_PI;
            cards.push_back(card);
            observer->getCardSelector()->Add(card);
            Repos();
            return 1;
        }
    return 0;
}
int GuiHandSelf::receiveEventMinus(WEvent* e)
{
    if (WEventZoneChange* event = dynamic_cast<WEventZoneChange*>(e))
    {
        if (hand == event->from)
            for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
                if (event->card->previous == (*it)->card)
                {
                    CardView* cv = *it;
                    observer->getCardSelector()->Remove(cv);
                    cards.erase(it);
                    Repos();
                    observer->mTrash->trash(cv);
                    return 1;
                }
        return 1;
    }
    return 0;
}

int GuiHandOpponent::receiveEventPlus(WEvent* e)
{
    if (WEventZoneChange* event = dynamic_cast<WEventZoneChange*>(e))
        if (hand == event->to)
        {
            CardView* card;
            if (event->card->view)
                card = NEW CardView(CardView::handZone, event->card, *(event->card->view));
            else
                card = NEW CardView(CardView::handZone, event->card, ClosedRowX, 0);
            card->alpha = 255;
            card->t = -4 * M_PI;
            cards.push_back(card);
            return 1;
        }
    return 0;
}
int GuiHandOpponent::receiveEventMinus(WEvent* e)
{
    if (WEventZoneChange* event = dynamic_cast<WEventZoneChange*>(e))
    {
        if (hand == event->from)
            for (vector<CardView*>::iterator it = cards.begin(); it != cards.end(); ++it)
                if (event->card->previous == (*it)->card)
                {
                    CardView* cv = *it;
                    cards.erase(it);
                    observer->mTrash->trash(cv);
                    return 1;
                }
        return 0;
    }
    return 0;
}

// I wanna write it like that. GCC doesn't want me to without -O.
// I'm submitting a bug report.
//      it->x = (it->x + (flip ? RightRowX : LeftRowX)) / 2;
