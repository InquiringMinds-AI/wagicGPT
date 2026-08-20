#ifndef _AI_PLAYER_BAKA_H_
#define _AI_PLAYER_BAKA_H_

#include "AIPlayer.h"
#include "AllAbilities.h"

class AIStats;
class AIHints;
class AIHint;


//Would love to define those classes as private nested classes inside of AIPlayerBaka, but they are used by AIHints (which itself should be known only by AIPlayerBaka anyways)
// Any way to clean this up and make all the AIPlayerBaka universe (AIHints, AIPlayerBaka, OrderedAIAction) "closed" ?
class OrderedAIAction: public AIAction
{
protected:
    Player * getPlayerTarget();

public:
    int efficiency;
    
    OrderedAIAction(AIPlayer * owner, MTGAbility * a, MTGCardInstance * c, MTGCardInstance * t = NULL)
        : AIAction(owner, a, c, t), efficiency(-1)
    {
    };

    OrderedAIAction(AIPlayer * owner, MTGCardInstance * c, MTGCardInstance * t = NULL);

    OrderedAIAction(AIPlayer * owner, Player * p)//player targeting through spells
        : AIAction(owner,p), efficiency(-1)
    {
    };

    OrderedAIAction(AIPlayer * owner, MTGAbility * a, MTGCardInstance * c, vector<Targetable*>targetCards)
        : AIAction(owner,a,  c, targetCards), efficiency(-1)
    {
    };

    OrderedAIAction(AIPlayer * owner, MTGAbility * a, Player * p, MTGCardInstance * c)//player targeting through abilities.
        : AIAction(owner, a, p, c), efficiency(-1)
    {
    };
    int getEfficiency();

    // Functions depending on the type of Ability
    int getEfficiency(AADamager * aad);

    int getRevealedEfficiency(MTGAbility * ability);

};

// compares Abilities efficiency
class CmpAbilities
{ 
public:
    bool operator()(const OrderedAIAction& a1, const OrderedAIAction& a2) const
    {
        OrderedAIAction* a1Ptr = const_cast<OrderedAIAction*>(&a1);
        OrderedAIAction* a2Ptr = const_cast<OrderedAIAction*>(&a2);
        int e1 = a1Ptr->getEfficiency();
        int e2 = a2Ptr->getEfficiency();
        if (e1 != e2) return (e1 > e2);
        if (a1Ptr->id != a2Ptr->id) return a1Ptr->id < a2Ptr->id;
        //Same ability, same efficiency: tie-break on the actual action
        //identity. Without this, equally-rated targets of one ability
        //collapse into a single map key and all but one are silently
        //dropped from the ranking - a major cause of 'the AI never
        //plays/aims this card right' reports (upstream issue #1079).
        if (a1Ptr->target != a2Ptr->target) return a1Ptr->target < a2Ptr->target;
        if (a1Ptr->playerAbilityTarget != a2Ptr->playerAbilityTarget) return a1Ptr->playerAbilityTarget < a2Ptr->playerAbilityTarget;
        if (a1Ptr->player != a2Ptr->player) return a1Ptr->player < a2Ptr->player;
        return a1Ptr->click < a2Ptr->click;
    }
};

typedef std::map<OrderedAIAction, int, CmpAbilities> RankingContainer;


struct AttackerDamaged;

class AIPlayerBaka: public AIPlayer{
 protected:
    virtual int orderBlockers();
    //Permute atk->blockers so autoaffectDamage's lethal-first walk kills
    //as many blockers as this attacker's damage affords. Shared by the
    //heuristic ORDER step and AIPlayerGPT's fallback path.
    void heuristicDamageOrder(AttackerDamaged * atk);
    virtual int combatDamages();
    virtual int interruptIfICan();
    virtual int chooseAttackers();
    virtual int chooseBlockers();
    virtual int canFirstStrikeKill(MTGCardInstance * card, MTGCardInstance *ennemy);
    virtual int effectBadOrGood(MTGCardInstance * card, int mode = MODE_PUTINTOPLAY, TargetChooser * tc = NULL);
    virtual bool shouldAIForceAttack(MTGCardInstance* card, bool globalAttack);

    // returns 1 if the AI algorithm supports a given cost (ex:simple mana cost), 0 otherwise (ex: cost involves Sacrificing a target)
    virtual int CanHandleCost(ManaCost * cost, MTGCardInstance * card = NULL); 

    //Tries to play an ability recommended by the deck creator
    virtual int selectHintAbility();

    virtual vector<MTGAbility*> canPayMana(MTGCardInstance * card, ManaCost * mCost, int anytypeofmana);
    virtual vector<MTGAbility*> canPayMana(MTGCardInstance * card, ManaCost * mCost, int anytypeofmana, map<MTGCardInstance*, bool> &usedCards, bool searchingAgain = false);
    virtual vector<MTGAbility*> canPaySunBurst(ManaCost * mCost = NULL);
    //N-116g: the specific-mana plan for ONE (ability, source) pair - see the definition.
    vector<MTGAbility*> planPaymentForAction(MTGAbility * ability, MTGCardInstance * click,
                                             ManaCost * totalPotentialMana);

    virtual MTGCardInstance * chooseCard(TargetChooser * tc, MTGCardInstance * source, int random = 0);
    //The COMMIT choice for an extra-cost target (what to sacrifice/tap/
    //discard to pay a cost) - split from chooseCard because chooseCard is
    //also a cheap existence PROBE (CanHandleCost, hot on every potential-
    //mana enumeration) that a decision-making override must never turn
    //into a model call. Returning NULL means "no choice yet"; the payment
    //attempt aborts for this tick (the async decision pattern).
    virtual MTGCardInstance * chooseCostTarget(TargetChooser * tc, MTGCardInstance * source);
    //A cost payment may legally BE the ability's own source ("{1}, Sacrifice a
    //Scarecrow" on a Scarecrow). chooseCard refuses the source outright - right
    //for its normal job (an effect must not target the spell producing it),
    //fatal for a cost whose only payment is the permanent itself: the payment
    //never gets made, the activation is silently dropped, and canPay() keeps
    //re-offering it forever (wave-34 b6 F1, Scarecrone). Legality here is the
    //TargetChooser's call. Restricted to a source ON THE BATTLEFIELD so a spell
    //being cast from hand can still never pay with itself.
    MTGCardInstance * selfAsCostPayment(TargetChooser * tc, MTGCardInstance * source);
    virtual int selectMenuOption();

    virtual AIStats * getStats();

    int payAlternative;
    MTGCardInstance * nextCardToPlay;
    //When set, FindCardToPlay examines ONLY this card: every other candidate
    //is skipped and the play-chance roll always passes. This lets a subclass
    //pick the card itself (e.g. an LLM choice) while the full legality /
    //restriction / payment machinery still validates and prices the pick.
    MTGCardInstance * aiForcedCandidate;
    //With aiForcedCandidate set: cast it with its ALTERNATIVE cost even when
    //the normal cost is payable (the chooser picked the mode explicitly,
    //e.g. evoke over hardcast).
    bool aiForcedAlternative;
    MTGCardInstance * activateCombo();
    TargetChooser * GetComboTc(GameObserver * observer, TargetChooser * tc = NULL);
    AIHints * hints;
    AIStats * stats;
    int oldGamePhase;
    float timer;
    virtual MTGCardInstance * FindCardToPlay(ManaCost * potentialMana, const char * type);
    vector<MTGCardInstance*>comboCards;

    //used by MomirPlayer, hence protected instead of private
    virtual int getEfficiency(OrderedAIAction * action);
    virtual int getEfficiency(MTGAbility * ability);
    virtual bool payTheManaCost(ManaCost * cost, int anytypeofmana, MTGCardInstance * card = NULL,vector<MTGAbility*> gotPayment = vector<MTGAbility*>());
    virtual int getCreaturesInfo(Player * player, int neededInfo = INFO_NBCREATURES , int untapMode = 0, int canAttack = 0);
    virtual ManaCost * getPotentialMana(MTGCardInstance * card = NULL);
    virtual int selectAbility();
    //Decision seam: given all legal ranked actions, pick one (or NULL to pass).
    //Default = heuristic top pick behind the historical efficiency/chance gate.
    //Subclasses (e.g. an LLM-backed player) can override to choose differently.
    virtual const OrderedAIAction * chooseOrderedAction(RankingContainer& ranking);
    virtual int doAbility(MTGAbility * Specific = NULL, MTGCardInstance * withCard = NULL);

 public:
    enum {
        INFO_NBCREATURES,
        INFO_CREATURESPOWER,
        INFO_CREATURESRANK,
        INFO_CREATURESTOUGHNESS,
        INFO_CREATURESATTACKINGPOWER
    };

    enum {
        NONE,
        OTHER,
        MORPH
    }; // Possbile alternative costs to be used for AI.

    vector<MTGAbility*>gotPayments;

    AIPlayerBaka(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck = NULL);
    AIHint * comboHint;
    virtual int Act(float dt);
    //W3c/c5d: TRUE while a decision is still being made out-of-band (an
    //async model call in flight). While it holds, Act neither acts nor
    //commits the empty-clickstream pass/decline - it is consulted at Act
    //entry (with the frame dt) and again after computeActions (dt 0, a
    //call may have just started). Baka decides synchronously: never holds.
    virtual bool decisionPending(float dt) { (void) dt; return false; }
    void initTimer();
    virtual int computeActions();
    virtual void Render();
    virtual int receiveEvent(WEvent * event);
    virtual ~AIPlayerBaka();
    virtual int affectCombatDamages(CombatStep step);
    virtual int canHandleCost(MTGAbility * ability);
    virtual int chooseTarget(TargetChooser * tc = NULL, Player * forceTarget = NULL,MTGCardInstance * Chosencard = NULL,bool checkonly = false);
    virtual vector<MTGAbility*> canPayManaCost(MTGCardInstance * card = NULL, ManaCost * mCost = NULL, int anytypeofmana = 0){ return canPayMana(card, mCost, anytypeofmana);};

    //used by AIHInts, therefore public instead of private :/
    virtual int createAbilityTargets(MTGAbility * a, MTGCardInstance * c, RankingContainer& ranking);

    //Pre-game (opening-hand) heuristics for the headless AI: keep hands with a
    //reasonable land count, floor mulligans at a 5-card hand, bottom the most
    //expensive cards, always start leylines on the battlefield. AIPlayerGPT
    //overrides these with model calls and falls back to these on any failure.
    virtual int pregameMulliganDecision(int mullsTaken);
    virtual MTGCardInstance * pregameChooseBottom(int need, int chosenSoFar, int & status);
    virtual int pregameLeylineDecision(MTGCardInstance * card);
};

#endif
