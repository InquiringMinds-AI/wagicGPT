#ifndef _REPLACEMENT_EFFECTS_H_
#define _REPLACEMENT_EFFECTS_H_

#include <list>
using namespace std;
#include "Damage.h"
#include "WEvent.h"
#include "Counters.h"

class TargetChooser;
class MTGAbility;

class ReplacementEffect
{
public:
    virtual WEvent * replace(WEvent * e)
    {
        return e;
    }
    ;
    //NON-DESTRUCTIVE PROBE (W41-5). replace() is the live path: it mutates the
    //Damage and deletes the event, so it can never be used to ASK "would this
    //damage be prevented?" - which is exactly what the block-outcome
    //annotation needs before combat happens. This mirrors the same predicate
    //without touching state. Return 0 = this effect does not apply; 1 = it
    //prevents the damage ENTIRELY (a claim the annotation may lean on);
    //2 = it applies but the residue is not exactly computable here (a finite
    //shield), which the caller must render as an honest not-included flag
    //rather than a guess.
    virtual int preventionKindFor(Targetable * /*src*/, Targetable * /*tgt*/, int /*damageType*/)
    {
        return 0;
    }
    virtual ~ReplacementEffect() {}
};

class REDamagePrevention: public ReplacementEffect
{
protected:
    MTGAbility * source;
    TargetChooser * tcSource;
    TargetChooser * tcTarget;
    int damage;
    bool oneShot;
    Damage::DamageType typeOfDamage;
public:
    REDamagePrevention(MTGAbility * _source, TargetChooser *_tcSource = NULL, TargetChooser *_tcTarget = NULL,
                       int _damage = -1, bool _oneShot = true, Damage::DamageType typeOfDamage = Damage::DAMAGE_ALL_TYPES);
    WEvent * replace(WEvent *e);
    int preventionKindFor(Targetable * src, Targetable * tgt, int damageType);
    ~REDamagePrevention();
};

class RECountersPrevention: public ReplacementEffect
{
protected:
    MTGAbility * source;
    MTGCardInstance * cardSource;
    MTGCardInstance * cardTarget;
    TargetChooser * tc;
    Counter * counter;
public:
    RECountersPrevention(MTGAbility * _source, MTGCardInstance * cardSource = NULL,
        MTGCardInstance * cardTarget = NULL, TargetChooser * tc = NULL, Counter * counter = NULL);
    WEvent * replace(WEvent *e);
    ~RECountersPrevention();
};

class REDrawReplacement: public ReplacementEffect
{
protected:
    MTGAbility * source;

public:
    Player * DrawerOfCard;
    MTGAbility * replacementAbility;
    REDrawReplacement(MTGAbility * _source, Player * Drawer = NULL, MTGAbility * replaceWith = NULL);
    WEvent * replace(WEvent *e);
    ~REDrawReplacement();
};
class ReplacementEffects
{
public:
    list<ReplacementEffect *> modifiers;
    ReplacementEffects();
    WEvent * replace(WEvent *e);
    //W41-5: the strongest verdict across every registered effect - 1 (fully
    //prevented) dominates 2 (applies, not exactly computable) dominates 0.
    int preventionKindFor(Targetable * src, Targetable * tgt, int damageType);
    int add(ReplacementEffect * re);
    int remove(ReplacementEffect * re);
    ~ReplacementEffects();
};

#endif
