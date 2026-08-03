#include "PrecompiledHeader.h"

#include "ExtraCost.h"
#include "TargetChooser.h"
#include "MTGCardInstance.h"
#include "Translate.h"
#include "Player.h"
#include "Counters.h"
#include "AllAbilities.h"

#if !defined(QT_CONFIG)
#include <boost/scoped_ptr.hpp>
typedef boost::scoped_ptr<ManaCost> ManaCostPtr;
#else
#include <QScopedPointer>
class ManaCostPtr : public QScopedPointer<ManaCost>
{
public:
    ManaCostPtr(ManaCost*m) : QScopedPointer(m){
    };
    ManaCost* get() const {return data();};

};
#endif

SUPPORT_OBJECT_ANALYTICS(ExtraCost)

ExtraCost::ExtraCost(const std::string& inCostRenderString, TargetChooser *_tc, ManaCost * _costToPay)
    : tc(_tc),costToPay(_costToPay), source(NULL), target(NULL), mCostRenderString(inCostRenderString)
{
    if (tc)
        tc->targetter = NULL;
}

ExtraCost::~ExtraCost()
{
    SAFE_DELETE(costToPay);
    SAFE_DELETE(tc);
}

int ExtraCost::setSource(MTGCardInstance * _source)
{
    source = _source;
    if (tc)
    {
        tc->source = _source;
        // this keeps the target chooser from being unable to select a creature with shroud/protections.
        tc->targetter = NULL;
        tc->Owner = source->controller();
    }
    else
    {
        target = _source;
    }
    return 1;
}

void ExtraCost::Render()
{
    if (!mCostRenderString.empty())
    {
        WFont * mFont = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        mFont->SetColor(ARGB(255,255,255,255));
        mFont->DrawString(mCostRenderString, 40, 20, JGETEXT_LEFT);
    }
}

int ExtraCost::setPayment(MTGCardInstance * card)
{
    int result = 0;
    if (tc)
    {
        result = tc->addTarget(card);
        //this is flawed logic, we need to fix. if there is a target in list
        //we return targetready instead, the card is not pushed back into list
        //how ever, it is made the target becuase the result is 1 even if we couldnt
        //target it with the targetchooser.
        if (result)
        {
            target = card;
        }
    }
    if (costToPay && source->controller()->getManaPool()->canAfford(costToPay,card->has(Constants::ANYTYPEOFMANAABILITY)))
    {
        result = 1;
    }
    return result;
}
//extra added manacost, or add a manacost as the cost of extra
ExtraManaCost * ExtraManaCost::clone() const
{
    ExtraManaCost * ec = NEW ExtraManaCost(*this);
    return ec;
}

ExtraManaCost::ExtraManaCost(ManaCost * costToPay)
    : ExtraCost("\nCost:",NULL, costToPay)
{
}

int ExtraManaCost::tryToSetPayment(MTGCardInstance * card)
{
    return 1;
}

int ExtraManaCost::isPaymentSet()
{
    if (!source->controller()->getManaPool()->canAfford(costToPay,source->has(Constants::ANYTYPEOFMANAABILITY)))
    {
        return 0;
    }
    return 1;
}

int ExtraManaCost::canPay()
{
    if(!source->controller()->getManaPool()->canAfford(costToPay,source->has(Constants::ANYTYPEOFMANAABILITY)))
    {
        return 0;
    }
    return 1;
}

int ExtraManaCost::doPay()
{
    if (!source->controller()->getManaPool()->canAfford(costToPay,source->has(Constants::ANYTYPEOFMANAABILITY)))
        return 0;

    source->controller()->getManaPool()->pay(costToPay);
    return 1;
}

//Snow cost
SnowCost * SnowCost::clone() const
{
    SnowCost * ec = NEW SnowCost(*this);
    return ec;
}

SnowCost::SnowCost() :
ExtraCost("Snow Mana")
{
}

int SnowCost::isPaymentSet()
{
    if (source->controller()->getManaPool()->getConvertedCost())
    {
        int result = 0;
        result += source->controller()->snowManaG;
        result += source->controller()->snowManaU;
        result += source->controller()->snowManaR;
        result += source->controller()->snowManaB;
        result += source->controller()->snowManaW;
        result += source->controller()->snowManaC;
        if (result)
        {
            if ((source->controller()->snowManaC && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{1}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))) ||
               (source->controller()->snowManaG && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{g}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))) ||
               (source->controller()->snowManaU && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{u}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))) ||
               (source->controller()->snowManaR && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{r}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))) ||
               (source->controller()->snowManaB && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{b}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))) ||
               (source->controller()->snowManaW && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{w}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))) ||
               (source->controller()->snowManaC && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{c}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY))))
                return 1;
            else
                return 0;
        }
    }
    return 0;
}

int SnowCost::canPay()
{
    return isPaymentSet();
}

int SnowCost::doPay()
{
    if (source->controller()->getManaPool()->getConvertedCost())
    {
        int result = 0;
        result += source->controller()->snowManaG;
        result += source->controller()->snowManaU;
        result += source->controller()->snowManaR;
        result += source->controller()->snowManaB;
        result += source->controller()->snowManaW;
        result += source->controller()->snowManaC;
        if (result)
        {
            // Avoided double payments for Snow Mana cost
            if (source->controller()->snowManaC && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{1}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{1}",NULL,source));
                source->controller()->snowManaC -= 1;
            }
            else if (source->controller()->snowManaG && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{g}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{g}",NULL,source));
                source->controller()->snowManaG -= 1;
            }
            else if (source->controller()->snowManaU && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{u}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{u}",NULL,source));
                source->controller()->snowManaU -= 1;
            }
            else if (source->controller()->snowManaR && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{r}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{r}",NULL,source));
                source->controller()->snowManaR -= 1;
            }
            else if (source->controller()->snowManaB && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{b}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{b}",NULL,source));
                source->controller()->snowManaB -= 1;
            }
            else if (source->controller()->snowManaW && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{w}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{w}",NULL,source));
                source->controller()->snowManaW -= 1;
            }
            else if (source->controller()->snowManaC && source->controller()->getManaPool()->canAfford(ManaCost::parseManaCost("{c}",NULL,source),source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                //source->controller()->getManaPool()->pay(ManaCost::parseManaCost("{c}",NULL,source));
                source->controller()->snowManaC -= 1;
            }
            else
                return 0;
            return 1;
        }
    }
    return 0;
}

//Energy Cost
EnergyCost * EnergyCost::clone() const
{
    EnergyCost * ec = NEW EnergyCost(*this);
    return ec;
}

EnergyCost::EnergyCost(int enc) :
ExtraCost("Energy"),enc(enc)
{
}

int EnergyCost::canPay()
{
    if(source->controller()->energyCount >= enc)
    {
        return 1;
    }
    return 0;
}

int EnergyCost::doPay()
{
    if(source->controller()->energyCount)
    {
        source->controller()->energyCount -= enc;
        return 1;
    }
    return 0;
}

//life cost
LifeCost * LifeCost::clone() const
{
    LifeCost * ec = NEW LifeCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

LifeCost::LifeCost(TargetChooser *_tc)
    : ExtraCost("Life", _tc)
{
}

int LifeCost::canPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (!_target)
        return 0;
    if (_target->controller()->life <= 0 || _target->controller()->inPlay()->hasAbility(Constants::CANTCHANGELIFE) ||
        _target->controller()->opponent()->game->battlefield->hasAbility(Constants::CANTPAYLIFE) ||
        _target->controller()->game->battlefield->hasAbility(Constants::CANTPAYLIFE))
    {
        return 0;
    }
    return 1;
}

int LifeCost::doPay()
{
    if (!target)
        return 0;

    MTGCardInstance * _target = (MTGCardInstance *) target;

    _target->controller()->loseLife(1, source);
    target = NULL;
    if (tc)
        tc->initTargets();
    return 1;
}

//Specific life cost
SpecificLifeCost * SpecificLifeCost::clone() const
{
    SpecificLifeCost * ec = NEW SpecificLifeCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

SpecificLifeCost::SpecificLifeCost(TargetChooser *_tc, int slc)
    : ExtraCost("Life", _tc), slc(slc)
{
}

int SpecificLifeCost::canPay()
{
    if(source->controller()->life >= slc && !source->controller()->inPlay()->hasAbility(Constants::CANTCHANGELIFE) &&
       !source->controller()->opponent()->game->battlefield->hasAbility(Constants::CANTPAYLIFE) &&
       !source->controller()->game->battlefield->hasAbility(Constants::CANTPAYLIFE))
        {
            return 1;
        }
    /*MTGCardInstance * _target = (MTGCardInstance *) target;
    if(_target->controller()->life >= slc && !_target->controller()->inPlay()->hasAbility(Constants::CANTCHANGELIFE) &&
       !_target->controller()->opponent()->game->battlefield->hasAbility(Constants::CANTPAYLIFE) &&
       !_target->controller()->game->battlefield->hasAbility(Constants::CANTPAYLIFE))
    {
        return 1;
    }*/
    return 0;
}

int SpecificLifeCost::doPay()
{
    if (!target)
        return 0;

    MTGCardInstance * _target = (MTGCardInstance *) target;

    _target->controller()->loseLife(slc, source);
    target = NULL;
    if (tc)
        tc->initTargets();
    return 1;
}

//life or Mana cost
LifeorManaCost * LifeorManaCost::clone() const
{
    LifeorManaCost * ec = NEW LifeorManaCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

ManaCost * LifeorManaCost::getManaCost()
{
    return &manaCost;
}

LifeorManaCost::LifeorManaCost(TargetChooser *_tc, string manaType)
    : ExtraCost("Phyrexian Mana", _tc), manaType(manaType)
{
    string buildType ="{";
    buildType.append(manaType);
    buildType.append("}");
    ManaCostPtr cost(ManaCost::parseManaCost(buildType));
    manaCost.copy(cost.get());
}

int LifeorManaCost::canPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if ( _target->controller()->life <= 1 || _target->controller()->inPlay()->hasAbility(Constants::CANTCHANGELIFE) ||
       _target->controller()->opponent()->game->battlefield->hasAbility(Constants::CANTPAYLIFE) ||
       _target->controller()->game->battlefield->hasAbility(Constants::CANTPAYLIFE))
    {
        return _target->controller()->getManaPool()->canAfford(getManaCost(),_target->has(Constants::ANYTYPEOFMANAABILITY));
    }
    else if((_target->controller()->life > 1 || _target->controller()->getManaPool()->canAfford(getManaCost(),_target->has(Constants::ANYTYPEOFMANAABILITY))) && 
        (!_target->controller()->inPlay()->hasAbility(Constants::CANTCHANGELIFE) &&
        !_target->controller()->opponent()->game->battlefield->hasAbility(Constants::CANTPAYLIFE) &&
        !_target->controller()->game->battlefield->hasAbility(Constants::CANTPAYLIFE)))
    {
        return 1;
    }
    return 0;
}

int LifeorManaCost::doPay()
{
    if (!target)
        return 0;

    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (_target->controller()->getManaPool()->canAfford(&manaCost,_target->has(Constants::ANYTYPEOFMANAABILITY)))
    {
        _target->controller()->getManaPool()->pay(&manaCost);
    }
    else
    {
        _target->controller()->loseLife(2, source);
    }
    target = NULL;
    if (tc)
        tc->initTargets();
    return 1;
}

//discard a card at random as a cost
//DiscardRandom cost
DiscardRandomCost * DiscardRandomCost::clone() const
{
    DiscardRandomCost * ec = NEW DiscardRandomCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

DiscardRandomCost::DiscardRandomCost(TargetChooser *_tc)
    : ExtraCost("Discard Random", _tc)
{
}

int DiscardRandomCost::canPay()
{
    MTGGameZone * z = target->controller()->game->hand;
    int nbcards = z->nb_cards;
    if (nbcards < 1)
        return 0;
    if (nbcards == 1 && z->hasCard(source))
        return 0;
    return 1;
}

int DiscardRandomCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (target)
    {
        source->storedCard = target->createSnapShot();
        _target->controller()->game->discardRandom(_target->controller()->game->hand, source);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}
//a choosen discard

DiscardCost * DiscardCost::clone() const
{
    DiscardCost * ec = NEW DiscardCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

DiscardCost::DiscardCost(TargetChooser *_tc) :
ExtraCost("Discard", _tc)
{
}

int DiscardCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (target)
    {
        source->storedCard = target->createSnapShot();
        WEvent * e = NEW WEventCardDiscard(target);
        GameObserver * game = target->owner->getObserver();
        game->receiveEvent(e);
        _target->controller()->game->putInGraveyard(_target);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//cycling
CycleCost * CycleCost::clone() const
{
    CycleCost * ec = NEW CycleCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

CycleCost::CycleCost(TargetChooser *_tc) :
ExtraCost("Cycle", _tc)
{
}

int CycleCost::doPay()
{
    MTGCardInstance * _source = (MTGCardInstance *) source;
    if (_source)
    {
        WEvent * e = NEW WEventCardDiscard(target);//cycling sends 2 events one for the discard and one for the specific cycle trigger
        GameObserver * game = _source->owner->getObserver();
        game->receiveEvent(e);
        WEvent * e2 = NEW WEventCardCycle(_source);
        game->receiveEvent(e2);
        _source->controller()->game->putInGraveyard(_source);
        _source->controller()->cycledCount++;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//to library cost
ToLibraryCost * ToLibraryCost::clone() const
{
    ToLibraryCost * ec = NEW ToLibraryCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

ToLibraryCost::ToLibraryCost(TargetChooser *_tc)
    : ExtraCost("Move to Library", _tc)
{
}

int ToLibraryCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (target)
    {
        source->storedCard = target->createSnapShot();
        _target->controller()->game->putInLibrary(target);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//to graveyard cost
ToGraveCost * ToGraveCost::clone() const
{
    ToGraveCost * ec = NEW ToGraveCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

ToGraveCost::ToGraveCost(TargetChooser *_tc)
    : ExtraCost("Move to Graveyard", _tc)
{
}

int ToGraveCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (target)
    {
        source->storedCard = target->createSnapShot();
        _target->controller()->game->putInGraveyard(target);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//Mill yourself as a cost
MillCost * MillCost::clone() const
{
    MillCost * ec = NEW MillCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

MillCost::MillCost(TargetChooser *_tc)
    : ExtraCost("Deplete", _tc)
{
}

int MillCost::canPay()
{
    MTGGameZone * z = target?target->controller()->game->library:source->controller()->game->library;
    int nbcards = z->nb_cards;
    if (nbcards < 1)
        return 0;
    return 1;
}

int MillCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    if (target)
    {
        source->storedCard = (MTGCardInstance*)_target->controller()->game->library->cards[_target->controller()->game->library->nb_cards - 1]->createSnapShot();
        _target->controller()->game->putInZone(
            _target->controller()->game->library->cards[_target->controller()->game->library->nb_cards - 1],
            _target->controller()->game->library, _target->controller()->game->graveyard);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}


MillExileCost::MillExileCost(TargetChooser *_tc)
    : MillCost(_tc)
{
    // override the base string here
    mCostRenderString = "Deplete To Exile";
}

int MillExileCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    source->storedCard = (MTGCardInstance*)_target->controller()->game->library->cards[_target->controller()->game->library->nb_cards - 1]->createSnapShot();
    if (target)
    {
        _target->controller()->game->putInZone(
            _target->controller()->game->library->cards[_target->controller()->game->library->nb_cards - 1],
            _target->controller()->game->library, _target->controller()->game->exile);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//sac all lands cost
SacLandsCost * SacLandsCost::clone() const
{
    SacLandsCost * ec = NEW SacLandsCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

SacLandsCost::SacLandsCost(TargetChooser *_tc)
    : ExtraCost("Sacrifice All Lands", _tc)
{
}

int SacLandsCost::doPay()
{
    MTGGameZone * zone = source->controller()->game->inPlay;
    for (int j = zone->nb_cards - 1; j >= 0; j--)
    {
        MTGCardInstance * card = zone->cards[j];
        if(card->isLand() && !card->has(Constants::CANTBESACRIFIED))
        {        
            if (card)
            {
                MTGCardInstance * beforeCard = card;
                source->storedCard = card->createSnapShot();
                card->controller()->game->putInGraveyard(card);
                WEvent * e = NEW WEventCardSacrifice(beforeCard,card);
                GameObserver * game = card->owner->getObserver();
                game->receiveEvent(e);
            }
        }
    }
    return 1;
}

//unattach cost

UnattachCost * UnattachCost::clone() const
{
    UnattachCost * ec = NEW UnattachCost(*this);
    return ec;
}

UnattachCost::UnattachCost(MTGCardInstance * realSource)
    : ExtraCost("Unattach"),rSource(realSource)
{
}

int UnattachCost::isPaymentSet()
{
    if (rSource && !rSource->target)
    {
        return 0;
    }
    return 1;
}

int UnattachCost::canPay()
{
    return isPaymentSet();
}

int UnattachCost::doPay()
{
    MTGCardInstance * _source = (MTGCardInstance *) source;
    if(_source != rSource)
        _source = rSource;//for debugging purposes I let it set what it thinks is the source.
    if (_source)
    {
        GameObserver * game = _source->getObserver();
        for (size_t i = 1; i < game->mLayers->actionLayer()->mObjects.size(); i++)
        {
            MTGAbility * a = ((MTGAbility *) game->mLayers->actionLayer()->mObjects[i]);
            AEquip * eq = dynamic_cast<AEquip*> (a);
            if (eq && eq->source == _source)
            {
                ((AEquip*)a)->unequip();
            }
        }
        return 1;
    }
    return 0;
}

//Tap cost
TapCost * TapCost::clone() const
{
    TapCost * ec = NEW TapCost(*this);
    return ec;
}

TapCost::TapCost()
    : ExtraCost("Tap")
{
}

int TapCost::isPaymentSet()
{
    if (source && (source->isTapped() || source->hasSummoningSickness()))
    {
        return 0;
    }
    return 1;
}

int TapCost::canPay()
{
    return isPaymentSet();
}

int TapCost::doPay()
{
    MTGCardInstance * _source = (MTGCardInstance *) source;
    if (_source)
    {
        _source->tap();
        return 1;
    }
    return 0;
}
//unTap cost
UnTapCost * UnTapCost::clone() const
{
    UnTapCost * ec = NEW UnTapCost(*this);
    return ec;
}

UnTapCost::UnTapCost() :
ExtraCost("Untap")
{
}

int UnTapCost::isPaymentSet()
{
/*602.5a A creature's activated ability with the tap symbol ({T}) or the untap symbol ({Q})
 * in its activation cost can't be activated unless the creature has been under its
 * controller's control since the start of his or her most recent turn.
 * Ignore this rule for creatures with haste (see rule 702.10). As of 6/1/2014 Comprehensive Rules
 */
    if (source && (!source->isTapped() || source->hasSummoningSickness()))
    {
        return 0;
    }
    return 1;
}

int UnTapCost::canPay()
{
    return isPaymentSet();
}

int UnTapCost::doPay()
{
    MTGCardInstance * _source = (MTGCardInstance *) source;
    if (_source)
    {
        _source->untap();
        return 1;
    }
    return 0;
}

//Tap target cost
TapTargetCost * TapTargetCost::clone() const
{
    TapTargetCost * ec = NEW TapTargetCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

TapTargetCost::TapTargetCost(TargetChooser *_tc, bool crew)
    : ExtraCost("Tap Target", _tc), crew(crew)
{
}

int TapTargetCost::isPaymentSet()
{
    if (target && target->isTapped())
    {
        tc->removeTarget(target);
        target->isExtraCostTarget = false;
        target = NULL;
        return 0;
    }
    if (crew && target && target->has(Constants::CANTCREW))
    {
        tc->removeTarget(target);
        target->isExtraCostTarget = false;
        target = NULL;
        return 0;
    }
    if (target)
        return 1;
    return 0;
}

int TapTargetCost::canPay()
{
    //base ExtraCost::canPay() always says 1, which offers the activation as
    //legal even when no valid untapped candidate exists - the player (or AI)
    //then enters a target-selection dead-end that can never commit (CS-002
    //family). Payable = at least one untapped candidate the chooser accepts.
    if (!tc || !source)
        return 1;
    GameObserver * g = source->getObserver();
    if (!g)
        return 1;
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * z = g->players[i]->game->inPlay;
        for (int k = 0; k < z->nb_cards; k++)
        {
            MTGCardInstance * c = z->cards[k];
            if (c && !c->isTapped() && !(crew && c->has(Constants::CANTCREW)) && tc->canTarget(c))
                return 1;
        }
    }
    return 0;
}

int TapTargetCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    
    if (target)
    {
        source->storedCard = target->createSnapShot();
        _target->tap();
        //crew ability
        if(crew)
        {
            if(_target->getCrewAbility().size())
            {
                AbilityFactory af(_target->getObserver());
                MTGAbility * a = af.parseMagicLine(_target->getCrewAbility(), -1, NULL, source,false,true);
                MTGAbility * crewAbility = a->clone();
                SAFE_DELETE(a);
                crewAbility->oneShot = true;
                crewAbility->canBeInterrupted = false;
                crewAbility->target = source;
                crewAbility->resolve();
                SAFE_DELETE(crewAbility);
            }
        }
        //end
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//untap other as a cost
UnTapTargetCost * UnTapTargetCost::clone() const
{
    UnTapTargetCost * ec = NEW UnTapTargetCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

UnTapTargetCost::UnTapTargetCost(TargetChooser *_tc)
    : ExtraCost("Untap Target", _tc)
{
}

int UnTapTargetCost::isPaymentSet()
{
    if (target && !target->isTapped())
    {
        tc->removeTarget(target);
        target->isExtraCostTarget = false;
        target = NULL;
        return 0;
    }
    if (target)
        return 1;
    return 0;
}

int UnTapTargetCost::canPay()
{
    //mirror of TapTargetCost::canPay - payable only if a tapped candidate
    //the chooser accepts exists (CS-002: Crackleburr's untap costs were
    //offered with zero tapped blue creatures and silently never committed)
    if (!tc || !source)
        return 1;
    GameObserver * g = source->getObserver();
    if (!g)
        return 1;
    for (int i = 0; i < 2; i++)
    {
        MTGGameZone * z = g->players[i]->game->inPlay;
        for (int k = 0; k < z->nb_cards; k++)
        {
            MTGCardInstance * c = z->cards[k];
            if (c && c->isTapped() && tc->canTarget(c))
                return 1;
        }
    }
    return 0;
}

int UnTapTargetCost::doPay()
{
    MTGCardInstance * _target = (MTGCardInstance *) target;
    
    if (target)
    {
        source->storedCard = target->createSnapShot();
        _target->untap();
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//exile as cost
ExileTargetCost * ExileTargetCost::clone() const
{
    ExileTargetCost * ec = NEW ExileTargetCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

ExileTargetCost::ExileTargetCost(TargetChooser *_tc)
    : ExtraCost("Exile Target", _tc)
{
}

//How many cards this exile cost must collect. The 16 ordinary E(sel|zone) users
//exile exactly one (a card needing several stacks separate single costs); they
//are not X-spells, so source->setX == -1 -> 1. The lone variable-count user,
//Skeletal Scrying's E(<prex>...), exiles the announced X, and it is the ONLY
//X-spell with an E() cost today, so setX > -1 unambiguously means "exile X".
//We read setX directly rather than tc->maxtargets, which is captured at
//chooser-creation time before X is announced (the stale-X bug) and is unreliable.
int ExileTargetCost::requiredExiles()
{
    if (source && source->setX > -1)
        return source->setX;
    return 1;
}

//Sync the chooser's capacity to the announced X before it starts accepting
//clicks. Without this the <prex> chooser keeps its stale creation-time
//maxtargets (0 or a pool-derived guess): a 0-capacity chooser reports
//"complete from birth" and refuses every click, so the exile never happens.
//Only touched for X-spells (setX > -1); the 16 fixed single-target costs keep
//their maxtargets == 1 and the exact legacy path.
int ExileTargetCost::setPayment(MTGCardInstance * card)
{
    if (tc && source && source->setX > -1)
        tc->maxtargets = source->setX;
    return ExtraCost::setPayment(card);
}

int ExileTargetCost::isPaymentSet()
{
    int needed = requiredExiles();
    if (needed <= 0)
        return 1; //X=0: nothing to exile, cost trivially met
    if (needed == 1)
        return (target != NULL); //single-target: byte-identical to legacy
    return (tc && tc->getNbTargets() >= needed);
}

int ExileTargetCost::doPay()
{
    if (requiredExiles() >= 2)
    {
        //multi-target: exile every card the chooser accumulated, not just the
        //last one that landed in target.
        vector<Targetable*> targetlist = tc->getTargetsFrom();
        for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); ++it)
        {
            MTGCardInstance * tCard = dynamic_cast<MTGCardInstance*>(*it);
            if (!tCard)
                continue;
            source->storedCard = tCard->createSnapShot();
            tCard->controller()->game->putInExile(tCard);
        }
        target = NULL;
        tc->initTargets();
        return 1;
    }

    //single-target (needed == 1): legacy path, unchanged.
    if (target)
    {
        source->storedCard = target->createSnapShot();
        target->controller()->game->putInExile(target);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return (requiredExiles() <= 0) ? 1 : 0; //X=0 pays trivially, exiling nothing
}

//Bounce as cost
BounceTargetCost * BounceTargetCost::clone() const
{
    BounceTargetCost * ec = NEW BounceTargetCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

BounceTargetCost::BounceTargetCost(TargetChooser *_tc)
    : ExtraCost("Move to Hand", _tc)
{
}

int BounceTargetCost::doPay()
{

    if (target)
    {
        source->storedCard = target->createSnapShot();
        target->controller()->game->putInHand(target);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//Bounce as cost for ninja
Ninja * Ninja::clone() const
{
    Ninja * ec = NEW Ninja(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

Ninja::Ninja(TargetChooser *_tc) :
ExtraCost("Choose Unblocked Card", _tc)
{
}

int Ninja::canPay()
{
    if(source->getObserver()->getCurrentGamePhase() != MTG_PHASE_COMBATBLOCKERS)
        return 0;
    return 1;
}

int Ninja::isPaymentSet()
{
    if (target && ((target->isAttacker() && target->isBlocked()) ||
                   target->isAttacker() < 1 ||
                   target->getObserver()->getCurrentGamePhase() != MTG_PHASE_COMBATBLOCKERS))
    {
        tc->removeTarget(target);
        target = NULL;
        return 0;
    }
    if (target)
        return 1;
    return 0;
}

int Ninja::doPay()
{
    if (target)
    {
        target->controller()->game->putInHand(target);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//endbouncetargetcostforninja

//Convoke
Convoke * Convoke::clone() const
{
    Convoke * ec = NEW Convoke(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

Convoke::Convoke(TargetChooser *_tc) :
    ExtraCost("Choose Cards To Tap", _tc)
{
}

int Convoke::canPay()
{
    return isPaymentSet();
}

int Convoke::isPaymentSet()
{
    //A convoke creature already tapped cannot pay - drop it from the set.
    if (target && target->isTapped())
    {
        tc->removeTarget(target);
        target->isExtraCostTarget = false;
        target = NULL;
    }
    if (!tc || tc->getNbTargets() == 0)
        return 0; //no creature chosen to convoke yet
    //Payment is set once the chosen creatures' reduction plus the mana in the
    //pool cover the printed cost. This historically keyed off the transient
    //`target` field (the last-added creature), which NULLS ITSELF the first
    //time the pool is short - so a cast whose remaining mana is floated AFTER
    //the creatures were chosen (exactly how the AI pays: creatures set
    //synchronously, land taps queued for later ticks) could never re-satisfy
    //the gate and every not-fully-creature-covered convoke aborted (Venerated
    //Loxodon 2/23, March 0/17, corpus 20260725). Key it off the actual chosen
    //targets so the gate re-passes once the floated mana lands.
    ManaCost * toReduce = getReduction();
    int afford = source->controller()->getManaPool()->canAfford(toReduce, source->has(Constants::ANYTYPEOFMANAABILITY));
    SAFE_DELETE(toReduce);
    return afford ? 1 : 0;
}

//CR 702.51b: "each creature tapped this way pays for {1} or one mana of that
//creature's color". ONE convoker, applied to `toReduce` in place.
//
//N-137a: this scan used to live inline in three places (getReduction's forward
//pass, its backward retry, and convokeReduceByCreatures' offer probe) and every
//copy broke on the FIRST index - the else arm set `next = true` alongside the
//match arm, so the `if (next) break;` fired after i == MTG_COLOR_GREEN and
//colours 2..5 were dead code. A WHITE convoker therefore never paid a {W} pip;
//it silently reduced GENERIC instead, so `{X}{G}{W}{W}` (March of the
//Multitudes) was offerable only when the LANDS alone could make both {W} - the
//deck137 witness where 3-4 untapped Forests plus two untapped white Human
//tokens produced no March line at all (corpus 20260727 s20/t8, s24/t10).
//
//Now: scan every colour, reduce the first COLOURED pip this creature's colour
//matches, and fall back to one generic only if no colour matched. Reducing a
//coloured pip is strictly the better greedy - generic can be paid by any mana,
//a coloured pip cannot - which is why the original intended colour-first order.
//The generic fallback is guarded on there BEING generic left: a convoker that
//matches no remaining pip pays nothing, and removing from an empty component
//was the shape behind the `in GetCost Seems ManaCost was not properly
//initialized` trace (N-137f).
//
//SINGLE DEFINITION ON PURPOSE. The offer probe and the real payment MUST agree
//or the oracle offers casts doPay cannot complete (the wave-30 over-offer that
//convoke_unaffordable_not_offered guards); three hand-kept copies is how they
//drifted. Every caller now routes here.
static void convokeApplyOneCreature(ManaCost * toReduce, MTGCardInstance * c)
{
    if (!toReduce || !c)
        return;
    for (int i = Constants::MTG_COLOR_GREEN; i <= Constants::MTG_COLOR_WHITE; ++i)
    {
        if ((c->getManaCost() && c->getManaCost()->hasColor(i)) || c->hasColor(i))
        {
            if (toReduce->hasColor(i))
            {
                toReduce->remove(i, 1);
                return;
            }
        }
    }
    if (toReduce->getCost(Constants::MTG_COLOR_ARTIFACT) > 0)
        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, 1);
}

ManaCost * Convoke::getReduction()
{
    ManaCost * toReduce = NEW ManaCost(source->getManaCost());
    tc->maxtargets = source->getManaCost()->getConvertedCost();
    if (tc->getNbTargets())
    {
        vector<Targetable*>targetlist = tc->getTargetsFrom();
        for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
            convokeApplyOneCreature(toReduce, dynamic_cast<MTGCardInstance*>(*it));
        //if we didnt find it payable one way, lets try again backwards.
        if (!source->controller()->getManaPool()->canAfford(toReduce,source->has(Constants::ANYTYPEOFMANAABILITY)))
        {
            SAFE_DELETE(toReduce);
            toReduce = NEW ManaCost(source->getManaCost());
            for (vector<Targetable*>::reverse_iterator it = targetlist.rbegin(); it != targetlist.rend(); it++)
                convokeApplyOneCreature(toReduce, dynamic_cast<MTGCardInstance*>(*it));
        }
    }
    return toReduce;
}

//Reduce `toReduce` in place by tapping the given creatures. Shares
//convokeApplyOneCreature with getReduction's BOTH passes, so this probe agrees
//with what doPay can actually pay BY CONSTRUCTION rather than by hand-kept
//duplication (N-137a: the three copies had all drifted the same way, and the
//probe's agreement with the payment is the offer oracle's whole invariant).
//`reverse` walks the creature list backwards, matching getReduction's own
//second-pass retry.
static void convokeReduceByCreatures(ManaCost * toReduce,
                                     const vector<MTGCardInstance*> & creatures,
                                     bool reverse)
{
    size_t n = creatures.size();
    for (size_t idx = 0; idx < n; idx++)
        convokeApplyOneCreature(toReduce, reverse ? creatures[n - 1 - idx] : creatures[idx]);
}

bool Convoke::offerable(MTGCardInstance * source, ManaCost * floatable)
{
    if (!source || !floatable || !source->getManaCost())
        return false;
    Player * p = source->controller();
    if (!p || !p->game || !p->game->inPlay)
        return false;

    //The untapped creatures that could convoke (Convoke's tc is
    //"creature|mybattlefield"; isPaymentSet drops any already-tapped one).
    //Summoning-sick creatures CAN convoke, so no sickness gate.
    vector<MTGCardInstance*> creatures;
    MTGGameZone * z = p->game->inPlay;
    for (int i = 0; i < z->nb_cards; i++)
    {
        MTGCardInstance * c = z->cards[i];
        if (!c || !c->isCreature())
            continue;
        if (c->isTapped() || c->isPhased)
            continue;
        creatures.push_back(c);
    }

    //Build the cost the payment must actually reach. An X-spell must reach X>=1
    //(X=0 is a donothing), so strip the X flag and add one generic pip.
    ManaCost * target = NEW ManaCost(source->getManaCost());
    if (target->hasX())
    {
        target->remove(Constants::NB_Colors, 1); //drop the {X} marker
        target->add(Constants::MTG_COLOR_ARTIFACT, 1); //require X = 1
    }
    int anytype = source->has(Constants::ANYTYPEOFMANA);

    //Forward greedy, then getReduction's backward retry, before deciding
    //unpayable - matching the two passes doPay's getReduction uses.
    ManaCost * fwd = NEW ManaCost(target);
    convokeReduceByCreatures(fwd, creatures, false);
    bool ok = floatable->canAfford(fwd, anytype) != 0;
    SAFE_DELETE(fwd);
    if (!ok)
    {
        ManaCost * bwd = NEW ManaCost(target);
        convokeReduceByCreatures(bwd, creatures, true);
        ok = floatable->canAfford(bwd, anytype) != 0;
        SAFE_DELETE(bwd);
    }
    SAFE_DELETE(target);
    return ok;
}

int Convoke::doPay()
{
    //Guard on the CHOSEN TARGETS, not the transient `target` field - the
    //float-then-pay flow nulls `target` before payment lands (see
    //isPaymentSet), and a doPay that early-outs on it leaves tc holding raw
    //pointers to creatures that may later die and be freed. The next offer
    //probe (MTGAlternativeCostRule::isReactingToClick -> isPaymentSet ->
    //getReduction) then dynamic_casts freed memory: the probe-198 SIGSEGV
    //at ExtraCost.cpp:1231. doPay and isPaymentSet MUST key off the same
    //state, and tc must come back empty however payment ends.
    if (tc && tc->getNbTargets())
    {
        ManaCost * toReduce = getReduction();
        source->controller()->getManaPool()->pay(toReduce);
        SAFE_DELETE(toReduce);
        vector<Targetable*>targetlist = tc->getTargetsFrom();
        for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
        {
            MTGCardInstance * targetCard = dynamic_cast<MTGCardInstance*>(*it);
            if (!targetCard)
                continue;
            source->storedCard = targetCard->createSnapShot();
            targetCard->tap();
        }
        tc->initTargets();
        target = NULL;
        return 1;
    }
    return 0;
}

//DELVE
Delve * Delve::clone() const
{
    Delve * ec = NEW Delve(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

Delve::Delve(TargetChooser *_tc) :
    ExtraCost("Choose Cards To Exile", _tc)
{
}

int Delve::canPay()
{
    return isPaymentSet();
}

int Delve::isPaymentSet()
{
    ManaCost * toReduce = NEW ManaCost(source->getManaCost());
    tc->maxtargets = source->getManaCost()->getCost(Constants::MTG_COLOR_ARTIFACT);
    if (tc->getNbTargets())
    {
        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, tc->getNbTargets());
    }
    if (target && (!source->controller()->getManaPool()->canAfford(toReduce,source->has(Constants::ANYTYPEOFMANAABILITY))))
    {
        target = NULL;
        SAFE_DELETE(toReduce);
        return 0;
    }
    if (target && (source->controller()->getManaPool()->canAfford(toReduce,source->has(Constants::ANYTYPEOFMANAABILITY))))
    {
        if (target->getObserver()->guiOpenDisplay)
            target->getObserver()->ButtonPressed(target->getObserver()->guiOpenDisplay);
        SAFE_DELETE(toReduce);
        return 1;
    }
    SAFE_DELETE(toReduce);
    return 0;
}

int Delve::doPay()
{
    if (target && tc->getNbTargets())
    {
        ManaCost * toReduce = NEW ManaCost(source->getManaCost());

        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, tc->getNbTargets());

        target->controller()->getManaPool()->pay(toReduce);
        SAFE_DELETE(toReduce);
        vector<Targetable*>targetlist = tc->getTargetsFrom();
        for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
        {
            MTGCardInstance * targetCard = dynamic_cast<MTGCardInstance*>(*it);
            source->storedCard = targetCard->createSnapShot();
            targetCard->controller()->game->putInExile(targetCard);
        }
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//IMPROVISE
Improvise * Improvise::clone() const
{
    Improvise * ec = NEW Improvise(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

Improvise::Improvise(TargetChooser *_tc) :
    ExtraCost("Choose Artifacts to Tap", _tc)
{
}

int Improvise::canPay()
{
    return isPaymentSet();
}

int Improvise::isPaymentSet()
{
    ManaCost * toReduce = NEW ManaCost(source->getManaCost());
    tc->maxtargets = source->getManaCost()->getCost(Constants::MTG_COLOR_ARTIFACT);
    if (tc->getNbTargets())
    {
        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, tc->getNbTargets());
    }
    if (target && (!source->controller()->getManaPool()->canAfford(toReduce,target->has(Constants::ANYTYPEOFMANAABILITY))))
    {
        target = NULL;
        SAFE_DELETE(toReduce);
        return 0;
    }
    if (target && (source->controller()->getManaPool()->canAfford(toReduce,target->has(Constants::ANYTYPEOFMANAABILITY))))
    {
        /*if (target->getObserver()->guiOpenDisplay)
            target->getObserver()->ButtonPressed(target->getObserver()->guiOpenDisplay);*/
        SAFE_DELETE(toReduce);
        return 1;
    }
    SAFE_DELETE(toReduce);
    return 0;
}

int Improvise::doPay()
{
    if (target && tc->getNbTargets())
    {
        ManaCost * toReduce = NEW ManaCost(source->getManaCost());

        toReduce->remove(Constants::MTG_COLOR_ARTIFACT, tc->getNbTargets());

        target->controller()->getManaPool()->pay(toReduce);
        SAFE_DELETE(toReduce);
        vector<Targetable*>targetlist = tc->getTargetsFrom();
        for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
        {
            MTGCardInstance * targetCard = dynamic_cast<MTGCardInstance*>(*it);
            source->storedCard = targetCard->createSnapShot();
            targetCard->tap();
        }
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

///////////////
//Sacrifice target as cost for Offering
Offering * Offering::clone() const
{
    Offering * ec = NEW Offering(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

Offering::Offering(TargetChooser *_tc,bool emerge) :
ExtraCost("Choose Creature to Offer", _tc), emerge(emerge)
{
}

int Offering::canPay()
{
    if (target && target->has(Constants::CANTBESACRIFIED))
        return 0;
    if (emerge)
    {
        if (target)
        {
            ManaCost * reduced = NEW ManaCost(source->getManaCost()->getAlternative());
            reduced->extraCosts = NULL;
            reduced->remove(Constants::MTG_COLOR_ARTIFACT, target->getManaCost()->getConvertedCost());

            if (target && (!source->controller()->getManaPool()->canAfford(reduced,source->has(Constants::ANYTYPEOFMANAABILITY))))
            {
                tc->removeTarget(target);
                target = NULL;
                SAFE_DELETE(reduced);
                return 0;
            }
            if (target && source->controller()->getManaPool()->canAfford(reduced,source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                SAFE_DELETE(reduced);
                return 1;
            }
        }

    }
    else
    {
        if (target)
        {
            ManaCost * diff = source->getManaCost()->Diff(target->getManaCost());
            if (target && (!source->controller()->getManaPool()->canAfford(source->getManaCost()->Diff(target->getManaCost()),source->has(Constants::ANYTYPEOFMANAABILITY))))
            {
                SAFE_DELETE(diff);
                tc->removeTarget(target);
                target = NULL;
                return 0;
            }
            if (target && (source->controller()->getManaPool()->canAfford(source->getManaCost()->Diff(target->getManaCost()),source->has(Constants::ANYTYPEOFMANAABILITY))))
            {
                SAFE_DELETE(diff);
                return 1;
            }
        }
    }
    return 0;
}

int Offering::isPaymentSet()
{
    if (emerge)
    {
        if (target)
        {
            ManaCost * reduced = NEW ManaCost(source->getManaCost()->getAlternative());
            reduced->extraCosts = NULL;
            reduced->remove(Constants::MTG_COLOR_ARTIFACT, target->getManaCost()->getConvertedCost());

            if (target && (!source->controller()->getManaPool()->canAfford(reduced,source->has(Constants::ANYTYPEOFMANAABILITY))))
            {
                tc->removeTarget(target);
                target = NULL;
                SAFE_DELETE(reduced);
                return 0;
            }
            if (target && source->controller()->getManaPool()->canAfford(reduced,source->has(Constants::ANYTYPEOFMANAABILITY)))
            {
                SAFE_DELETE(reduced);
                return 1;
            }
        }

    }
    else
    {
        if (target)
        {
            ManaCost * diff = source->getManaCost()->Diff(target->getManaCost());
            if (target && (!source->controller()->getManaPool()->canAfford(diff,source->has(Constants::ANYTYPEOFMANAABILITY))))
            {
                SAFE_DELETE(diff);
                tc->removeTarget(target);
                target = NULL;
                return 0;
            }
            if (target && (source->controller()->getManaPool()->canAfford(diff,source->has(Constants::ANYTYPEOFMANAABILITY))))
            {
                SAFE_DELETE(diff);
                return 1;
            }
        }
    }
    return 0;
}

int Offering::doPay()
{
    if (target)
    {
        if (emerge)
        {
            ManaCost * reduced = NEW ManaCost(source->getManaCost()->getAlternative());
            reduced->extraCosts = NULL;
            reduced->remove(Constants::MTG_COLOR_ARTIFACT, target->getManaCost()->getConvertedCost());
            target->controller()->getManaPool()->pay(reduced);
            SAFE_DELETE(reduced);
        }
        else
        {
            ManaCost * diff = source->getManaCost()->Diff(target->getManaCost());
            target->controller()->getManaPool()->pay(diff);
            SAFE_DELETE(diff);
        }
        MTGCardInstance * beforeCard = target;
        source->storedCard = target->createSnapShot();
        target->controller()->game->putInGraveyard(target);
        WEvent * e = NEW WEventCardSacrifice(beforeCard,target);
        GameObserver * game = target->owner->getObserver();
        game->receiveEvent(e);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}
//------------------------------------------------------------

SacrificeCost * SacrificeCost::clone() const
{
    SacrificeCost * ec = NEW SacrificeCost(*this);
    if (tc)
        ec->tc = tc->clone();
    return ec;
}

SacrificeCost::SacrificeCost(TargetChooser *_tc)
    : ExtraCost("Sacrifice", _tc)
{
}

int SacrificeCost::canPay()
{
    if (target && target->has(Constants::CANTBESACRIFIED))
        return 0;
    //base ExtraCost::canPay() always says 1, which offers the activation as
    //legal even when the sacrifice filter (e.g. "Sacrifice another creature")
    //matches no sacrificeable permanent - the player (or AI) then enters a
    //target-selection dead-end that can never commit (CS-002 family; the
    //Yawgmoth-with-only-itself unpayable offer). Payable = at least one
    //candidate the chooser accepts (and that may be sacrificed) exists.
    //No tc means "sacrifice this card itself" (setSource set target=source),
    //handled by the CANTBESACRIFIED check above.
    if (tc && source)
    {
        GameObserver * g = source->getObserver();
        if (!g)
            return 1;
        for (int i = 0; i < 2; i++)
        {
            MTGGameZone * z = g->players[i]->game->inPlay;
            for (int k = 0; k < z->nb_cards; k++)
            {
                MTGCardInstance * c = z->cards[k];
                if (c && !c->has(Constants::CANTBESACRIFIED) && tc->canTarget(c))
                    return 1;
            }
        }
        return 0;
    }
    return 1;
}

int SacrificeCost::doPay()
{
    if (target)
    {
        MTGCardInstance * beforeCard = target;
        source->storedCard = target->createSnapShot();
        target->controller()->game->putInGraveyard(target);
        WEvent * e = NEW WEventCardSacrifice(beforeCard,target);
        GameObserver * game = target->owner->getObserver();
        game->receiveEvent(e);
        target = NULL;
        if (tc)
            tc->initTargets();
        return 1;
    }
    return 0;
}

//Counter costs

CounterCost * CounterCost::clone() const
{
    CounterCost * ec = NEW CounterCost(*this);
    if (tc)
        ec->tc = tc->clone();
    
    if (counter)
        ec->counter = NEW Counter(counter->target, counter->name.c_str(), counter->power, counter->toughness);

    //TODO: counter can be NULL at this point, what do we set ec->counter->nb to if it is?
    if (ec->counter != NULL)
        ec->counter->nb = counter->nb;
    
    return ec;
}

CounterCost::CounterCost(Counter * _counter, TargetChooser *_tc) :
ExtraCost("Counters", _tc)
{
    counter = _counter;
    hasCounters = 0;
}

int CounterCost::setPayment(MTGCardInstance *card)
{
    if (tc)
    {
        int result = tc->addTarget(card);
        if (result)
        {
            if (counter->nb >= 0)
                return 1; //add counters always possible
            target = card;
            Counter * targetCounter = target->counters->hasCounter(counter->name.c_str(), counter->power, counter->toughness);
            if (targetCounter && targetCounter->nb >= -counter->nb)
            {
                hasCounters = 1;
                return result;
            }
        }
    }
    return 0;
}

int CounterCost::isPaymentSet()
{
    if (!target)
        return 0;
    if (counter->nb >= 0)
        return 1; //add counters always possible
    Counter * targetCounter = target->counters->hasCounter(counter->name.c_str(), counter->power, counter->toughness);
    if (targetCounter && targetCounter->nb >= -counter->nb)
    {
        hasCounters = 1;
    }
    if (target && hasCounters)
        return 1;
    return 0;
}

int CounterCost::canPay()
{
    // if target needs to be chosen, then move on.
    if (tc)
        return 1;
    if (counter->nb >= 0)
        return 1; //add counters always possible
    // otherwise, move on only if target has enough counters
    Counter * targetCounter = NULL;
    if(target)
    {
        targetCounter = target->counters->hasCounter(counter->name.c_str(), counter->power, counter->toughness);

    }
    else
    {
        targetCounter = source->counters->hasCounter(counter->name.c_str(), counter->power, counter->toughness);
    }
    if (targetCounter && targetCounter->nb >= -counter->nb)
    {
        return 1;
    }
    return 0;
}

int CounterCost::doPay()
{
    if (!target)
        return 0;

    //Add counters as a cost
    if (counter->nb >= 0)
    {
        int totalcounters = 0;
        for (int i = 0; i < counter->nb; i++)
        {
            target->counters->addCounter(counter->name.c_str(), counter->power, counter->toughness);
            totalcounters++;
        }
        WEvent * w = NEW WEventTotalCounters(target->counters, counter->name.c_str(), counter->power, counter->toughness, true, false, totalcounters, true, source);
        dynamic_cast<WEventTotalCounters*>(w)->targetCard = target->counters->target;
        target->getObserver()->receiveEvent(w);
        if (tc)
            tc->initTargets();
        target = NULL;
        return 1;
    }

    //remove counters as a cost
    if (hasCounters)
    {
        int totalcounters = 0;
        for (int i = 0; i < -counter->nb; i++)
        {
            target->counters->removeCounter(counter->name.c_str(), counter->power, counter->toughness);
            totalcounters++;
        }
        WEvent * w = NEW WEventTotalCounters(target->counters, counter->name.c_str(), counter->power, counter->toughness, false, true, totalcounters, true, source);
        dynamic_cast<WEventTotalCounters*>(w)->targetCard = target->counters->target;
        target->getObserver()->receiveEvent(w);
        hasCounters = 0;
        if (tc)
            tc->initTargets();
        target = NULL;
        return 1;
    }
    if (tc)
        tc->initTargets();
    target = NULL;
    return 0;
}

CounterCost::~CounterCost()
{
    SAFE_DELETE(counter);
}

//
//Container
//
ExtraCosts::ExtraCosts()
{
    action = NULL;
    source = NULL;
}

ExtraCosts * ExtraCosts::clone() const
{
    ExtraCosts * ec = NEW ExtraCosts(*this);
    ec->costs.clear();
    for (size_t i = 0; i < costs.size(); i++)
    {
        ec->costs.push_back(costs[i]->clone());
    }
    return ec;
}

void ExtraCosts::Render()
{
    //TODO cool window and stuff...
    for (size_t i = 0; i < costs.size(); i++)
    {
        costs[i]->Render();
    }
}

int ExtraCosts::setAction(MTGAbility * _action, MTGCardInstance * _card)
{
    action = _action;
    source = _card;
    source->storedCard = NULL;
    for (size_t i = 0; i < costs.size(); i++)
    {
        costs[i]->setSource(_card);
    }
    return 1;
}

int ExtraCosts::reset()
{
    action = NULL;
    if (source)
        source->storedCard = NULL;
    source = NULL;
    //Set all payments to "unset" (the long-standing TODO): stored payment
    //targets are raw pointers that dangle once the creatures leave play, and
    //a stale set makes later offer probes walk freed memory (probe-198
    //SIGSEGV). Whatever path abandons or completes a cast, the costs come
    //back target-free.
    for (size_t i = 0; i < costs.size(); i++)
    {
        costs[i]->target = NULL;
        if (costs[i]->tc)
            costs[i]->tc->initTargets();
    }
    return 1;
}

int ExtraCosts::tryToSetPayment(MTGCardInstance * card)
{
    for (size_t i = 0; i < costs.size(); i++)
    {
        if(!costs[i]->isPaymentSet())
        {
            for(size_t k = 0; k < costs.size(); k++)
            {
                if (card == costs[k]->target)
                {
                    //tapping or sacrificing a target to pay for its own cost
                    //is allowed, unless the source is already being tapped and contains a tap target
                    //or sacced and contains a sactarget
                    //cost like {t}{s(creature)} the source is allowed to be targeted for this
                    //if it is a creature. these cases below should be added whenever we a need
                    //for extra cost that have both a source and target version used on cards.
                    if (dynamic_cast<TapCost*>(costs[k]))
                    {
                        for (size_t tapCheck = 0; tapCheck < costs.size(); tapCheck++)
                        {
                            if (dynamic_cast<TapTargetCost*>(costs[tapCheck]))
                            {
                                return 0;//{t}{t(creature)}
                            }
                        }

                    }
                    else if (SacrificeCost * checking = dynamic_cast<SacrificeCost*>(costs[k]))
                    {
                        for (size_t sacCheck = 0; sacCheck < costs.size(); sacCheck++)
                        {
                            SacrificeCost * checking2 = dynamic_cast<SacrificeCost*>(costs[sacCheck]);
                            if (checking2)
                            if ((checking->tc != NULL && checking2->tc == NULL)
                                || (checking->tc == NULL && checking2->tc != NULL))
                            {
                                return 0; //{s}{s(creature)}
                            }
                        }
                    }
                    else
                    return 0;
                }
            }
            if (int result = costs[i]->setPayment(card))
            {
                //card->isExtraCostTarget = true;//moved to gameobserver, flawed logic was setting this to true even when it wasnt really a target
                return result;
            }
        }
    }
    return 0;
}

int ExtraCosts::isPaymentSet()
{
    for (size_t i = 0; i < costs.size(); i++)
    {
        if (!costs[i]->isPaymentSet())
            return 0;
    }
    return 1;
}

int ExtraCosts::canPay()
{
    for (size_t i = 0; i < costs.size(); i++)
    {
        if (!costs[i]->canPay())
        {
            if(costs[i]->target)
                costs[i]->target->isExtraCostTarget = false;
            return 0;
        }
    }
    return 1;
}

int ExtraCosts::doPay()
{
    int result = 0;
    for (size_t i = 0; i < costs.size(); i++)
    {
        if(costs[i]->target)//todo deprecate this let gameobserver control this.
        {
            costs[i]->target->isExtraCostTarget = false;
        }
        if (costs[i]->tc)
        {
            vector<Targetable*>targetlist = costs[i]->tc->getTargetsFrom();
            for (vector<Targetable*>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
            {
                costs[i]->target = dynamic_cast<MTGCardInstance*>(*it);
                costs[i]->doPay();
            }
        }
        else
        result += costs[i]->doPay();
    }
    return result;
}

ExtraCosts::~ExtraCosts()
{
    for (size_t i = 0; i < costs.size(); i++)
    {
        SAFE_DELETE(costs[i]);
    }
}

void ExtraCosts::Dump()
{
    DebugTrace("=====\nDumping ExtraCosts=====\n");
    DebugTrace("NbElements: " << costs.size());
}
