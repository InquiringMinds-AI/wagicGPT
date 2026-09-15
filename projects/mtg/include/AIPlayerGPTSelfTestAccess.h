#ifndef _AIPLAYERGPTSELFTESTACCESS_H_
#define _AIPLAYERGPTSELFTESTACCESS_H_

//Test-access seam for the PARSETEST self-test corpus (src/AIPlayerGPTSelfTest.cpp).
//The corpus used to live inside AIPlayerGPT.cpp so that it could reach that file's
//internal-linkage helpers directly ("same-TU access to the file-static parsers is
//the point"). Split out, it reaches them through this ONE struct instead:
//  * AIPlayerGPTSelfTestAccess is a friend of AIPlayerGPT, so the corpus keeps its
//    access to AIPlayerGPT's private statics with no member made public;
//  * it derives from AIPlayerGPT purely so that UNQUALIFIED calls in the corpus body
//    still find those private statics by ordinary class-scope lookup (it is never
//    instantiated);
//  * the static members below are one-line forwarders onto AIPlayerGPT.cpp's
//    anonymous-namespace / file-static helpers and constants, defined in that same
//    file so they can still see them. Nothing in AIPlayerGPT.cpp changed linkage.
//The struct-type and typedef declarations below moved here from AIPlayerGPT.cpp
//unchanged: a type cannot be forwarded through a thunk.

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"

//moved verbatim from AIPlayerGPT.cpp (was line 3236) so the self-test TU can name it
struct WipeVictim
{
    string name;
    int power;
    int toughness;
    bool indestructible;
    bool protectedFromSource; //#W61-V (R5): the damage never arrives
    bool canRegenerate;       //#W61-V (R5): destroyed, but its controller may pay
    WipeVictim() : power(0), toughness(0), indestructible(false),
                   protectedFromSource(false), canRegenerate(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 7883) so the self-test TU can name it
struct GangBlockerStat
{
    int power;
    int toughness;
    bool deathtouch;
    bool firstStrike;
};

//moved verbatim from AIPlayerGPT.cpp (was line 8659) so the self-test TU can name it
enum PaymentTapRestriction
{
    TAP_RESTRICT_NONE = 0,
    TAP_RESTRICT_NO_ATTACK = 1,
    TAP_RESTRICT_NO_BLOCK = 2
};

//moved verbatim from AIPlayerGPT.cpp (was line 14129) so the self-test TU can name it
struct XDamVictim
{
    string name;     //display name WITH its instance handle ("Llanowar Elves #3")
    string baseName; //the same name WITHOUT the handle, for run-grouping
    int lethalX;     //smallest X that kills it; <=0 = damage cannot kill it
    bool mine;
    bool pluralVerb; //"you NEED X>=6", not "you needs" - the caster's own row
    //#W45-5: a PLAYER row ("you" / "the opponent"), only ever produced by the
    //TARGETED class, where a player is a legal target. It is not a creature and
    //belongs on neither side's kill list, so xVictimList skips it and the
    //player-lethality clause states it in its own words instead.
    bool isPlayer;
    //#W76-CO (Q5): the instance this victim IS, so a row that kills it can be
    //priced against the crack-back walk with no second identification pass (a
    //name match would be a second walk, and two walks drift). NULL on every
    //non-board row (the player rows) and on any branch that does not set it.
    MTGCardInstance * inst;
    XDamVictim() : lethalX(0), mine(false), pluralVerb(false), isPlayer(false), inst(NULL) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 14476) so the self-test TU can name it
struct XVictimSurvey
{
    int maxX;      //the engine's own affordability answer at survey time
    int baseCMC;
    bool priceable; //a damage clause of the SPELL'S OWN was found and priced
    bool sweep;     //sweep class: every listed victim dies at the same X
    bool hitsMe, hitsOpp; //sweep class only: the spell also damages that player
    int myLife, oppLife;
    std::vector<XDamVictim> victims;
    XVictimSurvey()
        : maxX(0), baseCMC(0), priceable(false), sweep(false), hitsMe(false), hitsOpp(false),
          myLife(0), oppLife(0)
    {
    }
};

//moved verbatim from AIPlayerGPT.cpp (was line 15151) so the self-test TU can name it
struct OneShotDrawBranch
{
    string label;    //"" on a non-modal card
    int oppDraws;    //cards this branch hands the OPPONENT, now
    int selfDraws;   //cards this branch draws for the PILOT, now (evaluated only)
    bool variable;   //an amount this render will not claim
    OneShotDrawBranch() : oppDraws(0), selfDraws(0), variable(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 15759) so the self-test TU can name it
enum { kConverterClassDraw = 1, kConverterClassDiscard = 2 };

//moved verbatim from AIPlayerGPT.cpp (was line 20333) so the self-test TU can name it
enum
{
    kW76CloseFallThrough = 0, //not earned: the ordinary retry ladder below owns it
    kW76CloseArm = 1,         //buy phase 2 now
    kW76CloseDefer = 2        //earned, park busy: hold the decision PENDING
};

//moved verbatim from AIPlayerGPT.cpp (was line 29399) so the self-test TU can name it
struct W75LegendBoardCard
{
    std::string name;
    bool isToken;
    bool isCastCard;
    W75LegendBoardCard() : isToken(false), isCastCard(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 29583) so the self-test TU can name it
struct CrackBackAttackerFact
{
    int power;          //what this body adds to the crack-back total
    int blockersNeeded; //CR 509.1c: menace 2, "three or more" 3, else 1
    bool coverable;     //a CHECKED body this row adds may legally block it
    //#W69-BH (K6a): which of the seat's ALREADY-UNTAPPED creatures may legally
    //block this attacker, by index into the caller's own list of them. Empty on
    //every pre-wave-69 call, and then every figure below is byte-identical.
    std::vector<int> existingBlockers;
    //#W80-DH (F8): does this attacker TRAMPLE? The cover's whole arithmetic is
    //"a blocker removes its attacker's full power from the total", and CR 702.19b
    //says that is false for a trampler: only the blocker's lethal cut is absorbed
    //and the rest reaches the player. The cover model does not carry blocker
    //toughness, so it cannot price the spill - it must not claim survival over one.
    bool trample;
    CrackBackAttackerFact() : power(0), blockersNeeded(1), coverable(false),
                              trample(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 30108) so the self-test TU can name it
struct W77RemovalVictim
{
    std::string name;
    int takes;   //this body's contribution to the crack-back total
    W77RemovalVictim() : takes(0) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 30714) so the self-test TU can name it
struct W79BlkFact //#W79-DC (F3)
{
    int soak;        //remaining toughness, the lethal cut CR 702.19b prices
    int power;
    bool firstStrike;
    bool doubleStrike;
    bool deathtouch;
    W79BlkFact() : soak(0), power(0), firstStrike(false), doubleStrike(false),
                   deathtouch(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 30725) so the self-test TU can name it
struct W79AtkFact //#W79-DA (T5)
{
    int power;
    int minBlockers;
    bool trample;
    bool deathtouch;
    int toughness;     //#W79-DC (F3): 0 = not modelled, the attacker cannot die here
    bool firstStrike;  //#W79-DC (F3)
    bool doubleStrike; //#W79-DC (F3)
    W79AtkFact() : power(0), minBlockers(1), trample(false), deathtouch(false),
                   toughness(0), firstStrike(false), doubleStrike(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 31332) so the self-test TU can name it
enum HandCastVerdict
{
    kHandCastableNow = 0,
    kHandNeedsMana,      //converted cost exceeds the untapped sources
    kHandNeedsColours,   //enough sources by count, the payment still does not assemble
    kHandSorcerySpeed,   //timing, not resources
    kHandNoLegalTarget,  //CR 601.2c: a mandatory target with none on the board
    kHandRestricted,     //a play restriction forbids the cast
    //#W72-BT (M13, wave-71 deck146 MED-4): castable this turn, but this window
    //is not where it is offered - the phase's Casting decision was already put
    //to the model and answered.
    kHandCastableAnswered
};

//moved verbatim from AIPlayerGPT.cpp (was line 33054) so the self-test TU can name it
enum MdfcLandArrival
{
    kMdfcArrivalPlain = 0,   //nothing to say (the Pathway class)
    kMdfcArrivalPayOrTap,    //a follow-up menu: pay N life, or enter tapped
    kMdfcArrivalTapped       //no menu, and it arrives tapped
};

//moved verbatim from AIPlayerGPT.cpp (was line 41935) so the self-test TU can name it
struct GptModalMode
{
    string label;
    string outerSpec;
    string subSpec;
    //#W66-AQ (H9): does each half of the pair exist at all, and does it carry
    //payload that happens with no target? A `choice` line with two halves of
    //which ONE has no legal object is not a dead mode - it is a HALF-dead pair
    //that still does the other half, and the wave-65 census called it dead.
    bool outerPayload;
    bool subPresent;
    //#W68-BD (J8): is the granted half an EDICT handed to the opponent - a
    //`sacrifice` over a spec on THEIR battlefield that they, not the pilot,
    //choose from? Recorded here so the mode row can carry the same floor the
    //standalone edict row carries.
    bool subOpponentEdict;
    GptModalMode() : outerPayload(false), subPresent(false), subOpponentEdict(false) {}
};

//moved verbatim from AIPlayerGPT.cpp (was line 44269) so the self-test TU can name it
struct GptTokenPlan
{
    long answer;    //tokens allowed for the PLAN line + the action line
    long reasoning; //tokens allowed for the native thinking window (0 when off)
    long total;     //what actually goes in the request as max_tokens
};

//moved verbatim from AIPlayerGPT.cpp (was line 57086) so the self-test TU can name it
struct CombatTradeStat
{
    int power;
    int toughness;
    bool deathtouch;
    bool wither;       //wither or infect: damage as -1/-1 counters
    bool infectLabel;  //message word: true=infect, false=wither
    bool firststrike;  //first strike or double strike
    bool indestructible;
    bool trample;
    bool persist;      //has persist AND no -1/-1 counter yet -> returns once if it dies
    //#W44-LOW (wave-43 seat-162 E-3): lifelink is a LIFE SWING the verdict
    //hides. "(both die)" was the whole preview for an Unraveler trading with a
    //lifelink blocker - true about the bodies, silent about the +5 that decided
    //the race. 'doublestrike' is carried separately from 'firststrike' only to
    //keep this clause honest: a double striker gains twice and this preview
    //does not model the two damage steps, so it states the fact without a
    //number rather than printing half of one.
    bool lifelink;
    bool doublestrike;
    //#W45-3: life the BLOCKING side gains just for blocking, from triggers that
    //sit on a third permanent (Perimeter Captain, Pride Guardian, Noble Stand).
    //Only ever read off the 'b' (blocking) side - every call site puts the
    //blocker there, in both seats. 'blockLifeMay' is the script's own "may",
    //kept apart from the certain total rather than folded into it.
    int blockLife;
    int blockLifeMay;
};

//moved verbatim from AIPlayerGPT.cpp (was line 18104) so the self-test TU can name it
bool revealChooserCanDecline(bool targetMin, int maxtargets);

//moved verbatim from AIPlayerGPT.cpp (was line 35054) so the self-test TU can name it
typedef string (*HoldRowKeyFn)(const string&); //#W74-CH / #W75-CI (P1 b)

#if defined(WAGIC_GPT_PARSETEST_BUILD)
struct AIPlayerGPTSelfTestAccess : public AIPlayerGPT
{
    //the self-test corpus itself
    static void run();
    //PARSETEST-only helper that moved with the corpus
    static string answerSegmentStatic(const string& content, const char * label, int * run, int * rej,
                                      bool * labellessRead = NULL);

    //---- forwarders onto AIPlayerGPT.cpp internal helpers ----
    static string abilityActivationNarration(bool mine, const string& abilityText, const string& cardName, const string& targets= string());
    static string actionTakenNarration(const string& action, bool activationVerb= false);
    static bool activationActorIsMine(bool activatorIsMe, bool cardControllerKnown, bool cardControllerIsMine);
    static bool activationChairOverLadder(bool activatorIsMe, const vector<std::pair<bool, int> >& rungs);
    static int activationManaCost(const string& head);
    static bool activationSourceIsGrantedDummy(const string& sourceDisplayName, const string& sourceRawName);
    static string activationVerbPhrase(bool mine);
    static string afterCombatBlockerCostText(int availableBlockers);
    static string allAssignmentsIllegalClass(const string& prunedPairs, bool exhausted);
    static string allCastRowsDeadNote(bool allDead, int castRows, int landPlaysLegal= 0, bool anyTagDead= false);
    static string altCostPrintedNote(const string& liveCost, const string& printedCost);
    static int amassCountersFromScript(const string& magicText);
    static string animatableNotCountedTail(int n);
    static string animateDeathPriceClause(const string& name);
    static string animateRungCeilingClause(const string& name);
    static string annihilatorTag(const string& magicText);
    static bool annotateEtbPayOrTapMenu(vector<string>& opts, const string& landName, bool alreadyTapped= false, bool canTapForMana= true, bool canAttackEver= true);
    static string announceXHeader(const string& spell, int capX, bool canDecline= true, const string& libraryNote= "");
    static bool answerReplaced(const string& reply);
    static string answerTailFromReasoning(const string& reasoning);
    static void appendParseNote(std::string * noteOut, const char * sig);
    static void appendPlainDeclineScope(std::vector<string>& rows, bool holdRowOffered);
    static void appendStackDeathToDeclineRows(std::vector<string>& rows, int stackLossToMe, int myLife);
    static void applyBoardSweepMark(std::vector<std::string>& rows, const std::vector<int>& theirs, const std::vector<int>& mine, const std::vector<std::string> * engines= NULL, const std::vector<std::string> * myNames= NULL, const std::vector<std::string> * myEngines= NULL);
    static void applyDuplicateEffectTags(std::vector<std::string>& rows, const std::vector<std::string>& names, const std::vector<int>& costs);
    static void applyMenuFitTags(std::vector<std::string>& rows, const std::vector<int>& uses, int untappedSources, int crackTotal= 0, int crackAttackers= 0);
    static string askExemplar(const vector<string>& options, int * usedRow= NULL);
    static int askWindowKindForAsk(const string& decision, const vector<string>& options);
    static int askWindowKindForPriority(const vector<string>& rows, bool stackRespondable);
    static int assignableMatchedAttackers(int attackerCount, const vector<vector<char> >& can);
    static int assignableRemainderDamage(const vector<int>& damage, const vector<vector<char> >& can, const vector<char> * preventable= NULL, vector<int> * matchOut= NULL);
    static const char * asyncDropOutcome(bool gaveUpToHeuristic);
    static string asyncDropStamp(const char * arm, const string& driftKind, const char * outcome);
    static string asyncDropTraceLine(const char * arm, const string& driftKind, const char * outcome);
    static bool asyncLandArm(const string& seamTail);
    static string asyncSlotDriftKind(const string& oldKey, const string& newKey);
    static string asyncSlotKeyOf(bool forceClose, int turn, int phase, const string& seamTail, const string& board);
    static string attackPunisherClause(int theirsAbleToAttack, const std::vector<WipeVictim>& hit= std::vector<WipeVictim>(), int animatable= 0);
    static bool attackRestrictionFree(bool cantAttack, bool defenderWithoutOverride, bool flyersOnlyWithoutFlying, bool isBattle, bool tappedAndWillNotUntap= false);
    static string attackTotalLine(int attackers, int totalPower, int oppLife, int blockers, int guaranteed, int infectExcluded= 0, bool damageSuppressed= false, int blockGain= 0, const string& attackPunishers= "", bool oppLifeLoop= false, bool * outKillClaim= NULL, int blockLifelink= 0, int selfConverterLifelink= 0, int punisherPerAttacker= 0, int punisherKilled= 0, int punisherKilledPower= 0, const string& punisherVictims= "");
    static string attackerBlockerCountLine(int blockers, int evasive= 0, int attackers= 0);
    static string attackerBlockerEntry(const string& name, int power, int toughness, const string& outcome, const string& blockTrigger);
    static string attackerDestinationTag(bool mayAttackPlayer, bool mayAttackTarget);
    static string attackersHeldSickLine(const std::vector<std::string>& names, const std::vector<bool>& nextTurn= std::vector<bool>());
    static std::vector<long> attemptMsField(long firstMs, long secondMs, long latencyMs);
    static string battlefieldHeaderText(bool mine, int permanents, int creatures, int ableToAttack= -1, bool liveScope= true, int attacking= 0, int lands= -1, int animatable= -1);
    static int becomesBlockedSelfPump(const string& text, int& dp, int& dt);
    static bool betterBlockerPair(int a1, int a2, int b1, int b2);
    static string blockAssignmentClause(const vector<string>& blockerNames, const vector<string>& attackerNames, const vector<int>& match, int blockersDying= -1, bool leastDamageOnly= false, const vector<int>& matchRank= vector<int>(), int declineLife= -1);
    static string blockCountRequirementTag(int minBlockers);
    static string blockKeepAlternativeClause(const string& pairings, int keepDamage, int bestCase, int lifeAfterGains= -1);
    static string blockPriceSumTag(int priced, int sureSum, int maySum);
    static int blockerLifelinkGain(int blkPower, int blkToughness, bool blkLifelink, bool blkFirstStrike, int atkPower, bool atkFirstStrike, bool atkDeathtouch, bool blkDoubleStrike= false);
    static int blockingLifeCeiling(const std::vector<int>& perBlockerLife, const std::vector<bool>& canBlockSomeOffered);
    static bool blockingLifeTriggerClause(const string& lowLine, string& sourceSpec, string& amountExpr, bool& optional);
    static string blockingTriggerTotalLine(int triggered, int sure, int may, bool uniform, int totalGain, bool converter, int oppLife);
    static bool boardCreatureEffectEligible(bool isCreature, bool isLand, const string& magicText, const string& text, const string& keywordsPrinted);
    static string boardCreatureEffectResidueTail(size_t unlisted);
    static size_t boardCreatureEffectSnippetLen(size_t distinctNames);
    static string boardCreatureEffectText(const string& raw, const string& keywordsPrinted);
    static string boardEffectSnippet(const string& raw, size_t maxLen);
    static size_t boardEffectSnippetCeiling(size_t maxLen);
    static string boardEffectSnippetFocus(const string& raw, size_t maxLen, const string& focusPrefix);
    static size_t boardEffectSnippetLen(size_t distinctNames);
    static string boardEffectTag(const string& snippet, bool moreCopies, bool opponentsCard= false);
    static bool boardEffectTextEligible(bool isCreature, bool isLand, const string& magicText, const string& text);
    static bool boardEntryIsPermanent(bool isDesignationMarker);
    static string boardSweepMarker(int theirs, int mine, int measuredRows, const string& theirEngines= "", const string& myNames= "", const string& myEngines= "");
    static string buildForcedSacrificeAsk(const string& effectName, bool byOpponent, int gain, const string& engineRows= "");
    static string buildHandGainAsk(const string& where, const string& effectName, bool multi, bool unlimited, bool targetMin, int maxtargets, size_t pickIndex);
    static string buildHandRemovalAsk(const string& verb, bool byOpponent, bool relocate, const string& effectName, bool multi, bool unlimited, bool targetMin, int maxtargets, size_t pickIndex);
    static string buildMayObjectAsk(const string & srcName, const string & objName, const string & objOrigin, vector<string> & opts);
    static string buildRevealAskText(const vector<MTGCardInstance*>& revealed, const string& optOneLabel, const string& optTwoLabel, const string& optOneEffect, const vector<bool>& eligibleForOptionOne, int revealSource, bool pickExactlyOne, bool singlePickBare, bool singlePickDeclineLegal, bool wholeLibrary, vector<size_t> * outOrder= NULL, Player * seat= NULL);
    static string bulkZoneMoveNarration(bool mine, int count, const string& from, const string& to, const string& sourceName);
    static string cannotPayNowClause(int reach);
    static string cardTransformNarration(bool mine, const string& fromName, const string& toName, bool isCreature, int power, int toughness, const string& keywords);
    static string carriedPlanHeaderText(const string& ageClause, const string& absentClause, const string& plan);
    static string castAbandonedNarration(const string& card, int floating);
    static int castBodiesNetOfOwnText(int bodies, bool cardIsCreature, bool legendTwinControlled, bool selfLeavesOnResolution= false);
    static string castDeclineRow(bool combatNext);
    static string castDrawPriceRowTag(int perCast, const string& castNames, int perDraw, const string& punishers, int life= -1, int priorCharge= 0);
    static string castKillSummaryTag(const std::vector<std::string>& killed, int creatureTargets, const string& magnitude, const string& playerTail= "", const std::vector<std::string>& killedMine= std::vector<std::string>());
    static string castModeAltPriceTag(const std::vector<std::string>& labels, const std::vector<std::string>& costs, const std::vector<int>& draws);
    static string castModeCastPriceTag(const string& castCost, const string& castClause, int legalTargets);
    static string castModeCommitmentNote(bool castModeMenu, const string& ctxName, const string& committedName, bool sameTurn);
    static string castModeRowTag(const string& costBody, const string& clause);
    static string castPlayerDamageTail(int dmg, bool oppTargetable, int oppLife, int myLife= -1, int lifeLossFirst= 0, int oppLifeGain= 0, int oppGainTurns= 0);
    static string castSetKeyOf(const std::vector<string>& castNames);
    static int castTriggerDrawCount(const string& magicText);
    static string ceasedToExistNarration(bool mine, const string& cardName, bool isTokenCard, const string& from);
    static string choiceBranchLabel(const string& rawLine);
    static string chooseANameHeaderText(const string& sourceName, const string& cardText);
    static string chosenNameNarration(bool mine, const string& cardName, const string& chosen);
    static string chosenNameTag(const string& chosen);
    static bool ciStartsWith(const string& s, const string& name);
    static string clauseLeadTrim(const string& s);
    static int clauseOccurrences(const string& row, const char * literal);
    static string cleanupDiscardHeaderText(int handN, int limit, int over, int perDiscard= 0, const string& punishers= "", int life= -1);
    static string cleanupDiscardPriceClause(int handAfterDraw, int limit, int perDiscard, const string& punishers, int life= -1, const string& label= "CLEANUP PRICE", int stackedDraws= 0, int castLifeDelta= 0);
    static string closedLoopHeaderText(const string& converter, const string& mirror, bool theirs, bool loopCanOperate);
    static int codedAnswerCount(const string& reply);
    static string codedAnswerLineAt(const string& reply, int ordinal, size_t keep= kLatchedLineKeep, size_t * atOut= NULL);
    static bool codedAnswerLineInPlanBlock(const string& reply, int ordinal);
    static bool codedAnswerLinePlanSpan(const string& reply, int ordinal, size_t * atOut, size_t * startOut, size_t * endOut);
    static string collapsedRunNarration(const string& line, int count, int total);
    static void collectLabeledLines(const string& content, const char * label, vector<string>& out, vector<string> * prevOut= NULL, vector<vector<string> > * windowOut= NULL);
    static void collectXDamageClauses(const string& magicText, std::vector<string>& sweepSpecs, std::vector<string>& targetSpecs, bool& implicitTarget);
    static string combatDamageForecast(int life, int poison, int lifeIncoming, int poisonIncoming, int oppLife, bool oppLoopLive= false, int forcedCycleLoss= 0, const string& forcedCycleSource= "");
    static bool combatLineIsClean(const string& line, const vector<string> * rosterA, const vector<string> * rosterB);
    static string combatTradePreviewStats(const CombatTradeStat& b, const CombatTradeStat& a, int preventAtoB= kPreventNone, int preventBtoA= kPreventNone, int preventAtoFace= kPreventNone, bool attackerSeat= false, int bRemaining= -1, bool bGainConverted= false, string * outBlockTrigger= NULL, bool * outBlockerDies= NULL, string * outBlockerLifelink= NULL, string * outAttackerLifelink= NULL, bool * outAttackerDies= NULL, bool foeLifeLoop= false);
    static void composeRowOrder(const std::vector<size_t>& outer, const std::vector<size_t>& inner, std::vector<size_t>& out);
    static string compoundModeTargetNote(const string& modeName);
    static int converterClassOfScript(const string& script);
    static string converterSummaryText(const vector<string>& mine, const vector<string>& theirs, const vector<string>& mineMirror, const vector<string>& theirsMirror);
    static string converterSummaryText(const vector<string>& mine, const vector<string>& theirs);
    static string copiesLeftBehindTag(bool mineZone, const string& zone, const string& cardName, int remaining);
    static string copyOfTag(int rank, int total, const char * where);
    static string copyRangeTag(int first, int last, int total, const string& scope);
    static int countLegalAssignments(const vector<int>& pick, size_t nAttackers, const vector<vector<int> >& legalPerBlocker);
    static int counterAddPlusFromScript(const string& magicText);
    static string counterAppliedTag(bool targetIsCreature, bool stateCaptured, int settledP, int settledT, int liveP, int liveT);
    static string counterEventNarration(bool mine, const string& cardName, const string& handle, bool added, const string& counterName, int power, int toughness, const string& appliedTag, const string& sourceName);
    static int crackBackAnimatedPower(const string& line, size_t becomesPos);
    static string crackBackAnimatorEntry(const string& name, int bestPower, const string& bestCost);
    static bool crackBackBestAnimateRung(const string& mt, bool sourceStaysTapped, int oppReach, int reachColorMask, int& bestPower, string& bestCost);
    static string crackBackBlockerRowTag(int total, int myLife, int checkedBodies, int uncheckedBodies, const std::vector<CrackBackAttackerFact>& atk, bool totalIsFloor= false, int existingBodies= 0, int floorExtra= 0);
    static bool crackBackBodyUntaps(bool tapped, bool doesNotUntap, bool shackler, bool frozen);
    static bool crackBackColorsFit(const string& head, int reachColorMask);
    static bool crackBackCostAffordable(const string& costHead, bool sourceStaysTapped, int oppReach, int reachColorMask= -1);
    static bool crackBackDamageIsDirect(const string& line, size_t costColon, size_t damagePos);
    static string crackBackExileReturnClause(int count, int power);
    static int crackBackFlatDamageAmount(const string& line, size_t damagePos);
    static string crackBackKillRowTag(int total, int myLife, bool totalIsFloor, int removedPower, int removedBodies, int attackerBodies);
    static bool crackBackNextTurnDue(bool selfActive, int phase, int ableAttackers, int maxDamage);
    static string crackBackNextTurnLine(int ableAttackers, int maxDamage, int myLife, int exileReturnCount= 0, int exileReturnPower= 0, int evasiveAttackers= 0, int evasiveDamage= 0, bool haveBodies= false, const string& floorSources= "", int floorExtra= 0, bool floorUnsized= false, const string& blockerMakers= "");
    static string crackBackReliefClause(int total, int removed, int myLife, bool floorTotal, const string& staticSourceName= "", int sacrificeGiveBack= 0, int sacrificeBodies= 0);
    static string crackBackRemovalRowTag(int total, int myLife, bool totalIsFloor, int theirCreatures, int attackerBodies, int minAttackerPower);
    static string crackBackVerdictKey(int ableAttackers, int maxDamage, int myLife);
    static bool damageKillsTarget(int dmg, int remaining, bool indestructible, bool deathtouch);
    static string damageNarration(bool sourceMine, const string& sourceName, int amount, const string& targetName, bool haveResult= false, int settledLife= 0);
    static string damagePlaneswalkerVerdict(int dmg, int loyalty);
    static string damagePlayerVerdict(int dmg, int life, bool isMe, int myLife= -1, int lifeLossFirst= 0, bool myLifeLoop= false, bool poisonInstead= false, int poison= -1);
    static string damageTargetVerdict(int dmg, int toughness, int remaining, bool indestructible, bool deathtouch);
    static string dayNightChangeNarration(const string& designation);
    static string dayNightStateLine(const string& designation);
    static string declineFactForMenu(const vector<string>& options, bool callerSaysDecline);
    static int declineRowIndexOf(const vector<string>& options);
    static string declineRowReaskTag(int n);
    static bool declineRowText(const string& row);
    static string declinedListNote(int n);
    static string dfcOtherFaceName(const string& text);
    static string discardAlreadyControlClause(const string& onBattlefield);
    static string discardBoardVerdictTag(const string& rightNowClause);
    static string discardDeadTargetClause(int legalTargets);
    static int discardDisposabilityClass(bool isLand, const string& row);
    static void discardDisposabilityOrder(const std::vector<int>& classes, std::vector<size_t>& order);
    static string discardNotSpareTag(bool anySpare, bool isSpare);
    static bool discardPunisherClause(const string& script, int& perDiscard, bool& conditional);
    static string discardPunisherSummaryText(const vector<string>& mine, int minePerDiscard, const vector<string>& theirs, int theirsPerDiscard);
    static string discardSpareDefaultLine(int spareRows, int over);
    static string discardSpareLandClause(int myLands, int highestHandCost, bool haveHandCost, const string& beyondName= "", int beyondCost= 0, int reachCap= -1, const string& blockColour= "", int blockNeed= 0, int blockAfter= 0);
    static string discardVerdictLegend(bool anyVerdict);
    static bool drawFeedsStepForecast(const string& sourceMagicText, bool sourceOnBattlefield, bool sourceIsTurnBasedRule);
    static string drawForecastBlock(bool theirs, int base, const std::vector<std::pair<std::string, int> >& extras, int perDraw, const string& loopClause, bool stepIsNow, int holderLife, int resolvedInStep= 0, int unattributedInStep= 0);
    static string drawPriceRowTag(int cards, int perDraw, const string& punishers, int life= -1, bool deferTotal= false, int discards= 0, int perDiscard= 0, const string& discardPunishers= "", bool discardIsCycleCost= false);
    static bool drawPunisherClause(const string& magicText, int& perDraw, bool& conditional);
    static string drawPunisherSummaryText(const vector<string>& mine, int minePerDraw, const vector<string>& theirs, int theirsPerDraw, const string& yourLossLoopClause= "", const string& theirLossLoopClause= "");
    static string drawStepForecastText(int base, const std::vector<std::pair<std::string, int> >& extras, int perDraw, const string& loopCaution= "", bool stepIsNow= false, int holderLife= -1, int resolvedInStep= 0, int unattributedInStep= 0);
    static string drawsResolvedClause(int stepSize, int remaining);
    static int drawsStillAhead(int stepSize, int resolvedInStep, bool stepIsNow);
    static string drawsUnattributedClause(int unattributed);
    static string dualRoleHandNote(int handSize, int lands, int spells, const vector<std::pair<string, string> >& dual);
    static string dungeonRoomBranchHeader(const string& dungeonName, int exploreCount, const std::vector<std::pair<string, string> >& rooms);
    static string dungeonRoomPositionTag(const string& dungeonName, size_t index, size_t total);
    static string dungeonRowTag(int rooms, int ventures, int shortestVentures, const string& finalRoom, const string& finalReward);
    static int dungeonVenturesToCompletion(const string& commandZoneScript, int * shortest);
    static string dungeonsCompletedLine(int mine, int opp);
    static string duplicateEffectTag(int cheaperRow, int extraMana);
    static string duplicateVerdictTag(int cheaperRow, const string& cheaperName, int lessMana);
    static string edictAlreadyOnStackClause(bool alreadyAimed);
    static string edictClause(int theirCreatures, const string& onlyName, int onlyToughness, bool gainsToughness, bool targetGains= false, const string& onlyFacts= "", const string& converterName= "", int converterTakes= 0, int myLife= -1, bool myLifeLoop= false, int minToughness= 0, int atFloor= 0, int maxToughness= 0);
    static string edictOnlyVictimOnStackClause(bool alreadyAimed);
    static string edictSelfClause(int myCreatures, const string& onlyName, int onlyToughness, bool targetGains, int myMaxToughness= 0, const string& myConvName= "", int myConvTakes= 0, const string& engineTail= "");
    static const char * engineKindForScript(const string& magicText);
    static string equipHolderNote(const string& equipName, const string& hostName, const string& targetName, bool hostIsTheTarget);
    static string equipmentCastPriceTag(bool isEquipment, int ownCreatures, bool makesItsOwnBody);
    static string etbUsableClause(bool canTapForMana, bool canAttackEver);
    static bool everyCastRowDead(const std::vector<std::string>& rows, bool * anyTagDead= NULL);
    static string exampleCoreTrim(const string& coreIn, size_t maxLen);
    static string exemplarSentence(const string& exemplarText, int exemplarRow);
    static string exileHostageRowTag(const vector<string> & held);
    static string feedsRowTag(int perTurn, bool variable, int perCast, const std::vector<std::string>& converters, const std::vector<std::string>& handConverters= std::vector<std::string>(), const std::vector<std::string>& discardConverters= std::vector<std::string>(), const std::vector<std::string>& handDiscardConverters= std::vector<std::string>(), int selfPerTurn= 0, int variableHandSize= -1);
    static string fetchLandColorsClause(const bool adds[5], const bool canMake[5], const int sourceCounts[5]= NULL);
    static string fetchMakesNoManaClause(int untapped, bool fetchedEntersTapped, const string& colorsClause);
    static bool findAnswerLabelLine(const string& text, const char * expectedLabel, size_t& segStart, size_t& segEnd, size_t& labelLineStart, int * extraAnswerLines= NULL, int * rejectedLines= NULL, size_t * lastHeadLineStart= NULL);
    static size_t findPlanMarker(const string& text, size_t labelLineStart, size_t * firstOut);
    static string firstLabelledLine(const string& replyIn, const char * labelLc);
    static size_t firstLineLeadingPlanPos(const string& text);
    static string firstLoyaltyClausePrefix(const string& raw);
    static bool foldDuplicateTail(string& buf, const string& lastRendered, const string& folded);
    static string foldManaBillClauses(const string& row);
    static string forcedSacrificeRowTag(int gain, int toughness, const char * engineKind= NULL, const string& convName= "", int convTakes= 0, bool gainReaches= true);
    static bool forcedSacrificeShape(const string& lowScript, bool allMineBattlefieldCreatures, bool chooserIsTargeted);
    static string fromExileClause(bool yours, bool adventure, const string& causeName, bool causeTheirs, int tax);
    static string gangBlockPriceTag(int need, int damage, bool anyOfThem, bool mine= false, bool resultsBelow= false, bool byDeathtouch= false, bool afterFirstStrike= false, int trampleSpill= -1, int trampleSoak= 0);
    static int gangKillNeed(int atkPower, int atkToughness, bool atkFirstStrike, bool atkDeathtouch, const vector<GangBlockerStat>& g, int minBlockers, int * outDamage, bool * outAnyOfThem, bool * outByDeathtouch, int * outSoak= NULL);
    static bool gangKillsAttacker(int atkPower, int atkToughness, bool atkFirstStrike, bool atkDeathtouch, const vector<GangBlockerStat>& g, int * outDamage= NULL, bool * outByDeathtouch= NULL);
    static bool gateSideBranches(const string& script, const string& label, string& labelA, string& specA, string& labelB, string& specB);
    static string gateSideCensusTag(const string& labelA, int nA, const string& labelB, int nB);
    static bool gptAnswerSegment(const string& text, const char * expectedLabel, size_t& segStart, size_t& segEnd, size_t& labelLineStart, int * extraAnswerLines, int * rejectedLines, size_t * lastHeadLineStart, bool * labellessRead);
    static bool gptDeadlineMissed(bool emptyBody, long elapsedMs, long timeoutMs, long httpStatus, long curlCode);
    static long gptDeclarationAnswerFloorTokens(long items, long bytesPerItem);
    static bool gptForceCloseEarned(bool reasoningOnly, bool contentEmpty, bool finishLength, bool haveReasoning, int codedAnswers);
    static string gptForceClosePrefillBody(const string& prefill);
    static bool gptHoldCoversSiblingWindow(const string& heldSeam, int heldTurn, int heldPhase, const string& heldBoard, const std::set<string>& heldRowKeys, const char * nowSeam, int nowTurn, int nowPhase, const string& nowBoard, const std::vector<string>& nowRowKeys);
    static bool gptInlineChoiceOnPlanLine(const string& text, size_t& segStart, size_t& segEnd, size_t& lineStart);
    static string gptLineAtOffset(const string& text, size_t off);
    static GptTokenPlan gptResolveMaxTokens(bool thinking, bool forceClose, long reasoningBudget, long configuredCeiling, const char * seam, bool seamCapsDisabled, bool answerLockedRetry, long legalAnswerFloor= 0);
    static long gptSeamMaxTokens(const char * seam, long ceilingTokens);
    static string gptTextHash(const string& s);
    static bool graveyardRecursionScript(const string& magicText, const string& targetSpec);
    static string graveyardRefillRowClause(int graveyard, int library, bool selfToLibrary= true);
    static string graveyardZoneLine(bool mine, const vector<string>& entries);
    static void groupCombatCandidates(const vector<string>& names, const vector<string>& handles, vector<size_t>& order);
    static void groupNumberedRows(const vector<string>& rows, vector<size_t>& order);
    static string handAboutToBeReplacedLine(const string& sourceName);
    static string handCastabilityTag(int verdict, int need, int sources, const string& cost, const string& timingWhy= "", int floating= 0, bool instantSpeed= false);
    static string handRemovalVerb(const string& lc, bool& relocate, const string& gainDest= "");
    static int handleRank(const string& handle);
    static bool hasCodedAnswerLine(const string& content);
    static string headParenthetical(const string& answer);
    static bool headerCountsAsMoreAbleToAttack(bool canAttackNow, bool attacking);
    static string heldBackBlockTag(const vector<string>& cannotBlock, int totalOpposing);
    static int heuristicRevealIndex(const std::vector<int>& cmc, const std::vector<bool>& eligible);
    static string hiddenOriginMoveNarration(bool mine, const string& from, const string& to);
    static string highestMvEdictClause(const vector<std::pair<string, int> >& theirs);
    static string holdActionKeyRow(const string& row);
    static string holdContractParagraph();
    static string holdKeyRow(const string& row);
    static bool holdNoteSameWindow(bool first, int unseenRows, int measuredSeq, int nowSeq, int goneRows= 0);
    static string holdReopenNoteText(int unseenRows, int repeats, bool first= false, int goneRows= 0, bool contractBelow= false);
    static string holdRowBenefitClause();
    static int holdRowIndexOf(const std::vector<string> * optionTexts);
    static string holdRowLine(bool castSeam= false, bool activationLive= false);
    static bool holdStillStands(const std::set<string>& heldRows, const std::vector<string>& nowRows, const char ** whyOut, HoldRowKeyFn keyOf= holdKeyRow);
    static string hybridPipNoteText(const vector<string>& pips, int totalMana);
    static void improveAssignmentMaterial(const vector<vector<char> >& can, const vector<vector<int> >& rank, vector<int>& match);
    static string incomingCombatForecastLine(int ableAttackers, int maxDamage, int myLife);
    static int incomingCombatForm(bool oppActive, int phase, int liveAttackers, int latchTurn, int turn, int ableAttackers);
    static string incomingCombatLine(int attackers, int unblockedDamage, int myLife, bool haveBodies, int unblockableAttackers, int unblockableDamage, int bestCaseDamage= -1, bool bestCaseOptimal= true, int matchedAttackers= -1, int assignableAttackers= -1, const string& bestCaseAssignment= "", bool oppLifeLoopClosed= false, int blockTriggerGain= 0, int blockLifelinkGain= 0, int blockMayGain= 0);
    static string incomingCombatSettledLine(int attackers, int unblockedDamage);
    static bool insideRecurringTrigger(const string& lowText, size_t pos);
    static bool isAddNCountersOption(const string & optRaw);
    static bool isAnimationRow(const string& line);
    static bool isDayNightMarkerName(const string& name);
    static bool isEngineTokenText(const string& t);
    static bool isFetchCrackLine(const string& line);
    static bool isPublicOriginZone(const string& zone);
    static bool isRemovalDestination(const string& destination);
    static bool isRenderVocabWord(const string& w);
    static bool isReservedHoldEcho(const string& echoLc);
    static string joinBlockerRows(const vector<string>& names, const vector<string>& handles, const vector<string>& rests, bool * rangeUsed, const char * label= "B");
    static string joinNumberedRows(const vector<string>& rows, bool * rangeUsed);
    static string joinTargetEntries(const vector<string>& names, const vector<string>& handles, const vector<string>& tails, const std::map<string, string>& notes);
    static string joinVictimRoster(const std::vector<std::string>& entries);
    static string joinZoneEntries(const vector<string>& names, const vector<string>& handles, const vector<string>& tails, bool collapse);
    static string landDropAskText(size_t landCount);
    static string landDropStatusLine(bool myTurn, bool playable, bool haveLand);
    static string landDropThreatTag(const string& rawScript);
    static string landEntersTappedTag(const string& script, const string& printedText);
    static string landTapMana(const string& text);
    static string landTapTagFor(const string& script, const string& printedText, int myLands, const vector<string>& witness, const vector<char> * witnessPrinted= NULL);
    static string laterStepRouteClause(const string& offendingName, const std::vector<string> * rows);
    static string leavesFloatingTag(int poolTotal, int spent);
    static string leavesUntappedTag(int untappedSources, int sourcesUsed);
    static string legendRuleHeaderText(const string& name, int copies);
    static string legendRuleTargetClause(const string& name, int copies);
    static string legendTwinTag(const string& name, int loyalty= -1);
    static string legibleCounterName(const string& engineName);
    static string legibleCounterNameForGrant(const string& engineName, bool grantedTriggerLive);
    static string legibleKeywordName(const string& engineName);
    static string librarySearchNarration(bool mine, const string& sourceName);
    static int lifeAfterPendingStack(int life, int stackLossToMe);
    static string lifeChangeNarration(bool mine, int amount, int settled);
    static string lifeLoopAttackerRowTag(bool reachable, bool attackProvenLethal);
    static string lifeLoopDrainClause(const string& converterName, int takes, int life);
    static string lifeLoopWinTail(bool myLoopClosed, bool amountKnownPositive);
    static bool lifeLoopWinnable(bool loopClosed, bool theirLifeCanChange, bool iCanGainLife);
    static bool lifeLossMirrorScript(const string& magicText);
    static string lifeMenuLabelTag(int amount, bool gain, const string& sourceName, const string& sourceText);
    static string lifePaymentVerdict(int life, int cost);
    static int lifeToDamageConverterRate(const string& magicText, bool& proportional);
    static bool lifeToDamageConverterScript(const string& magicText);
    static int lifeToDamageConverterTake(const string& magicText, int gain);
    static bool lineIsActivatedCost(const string& low);
    static bool lineIsCodedAnswerLine(const string& line);
    static int listCopyRank(const std::vector<MTGCardInstance*>& cards, size_t index, int & outTotal);
    static size_t listKeyHash(const string& s);
    static bool logWindowInertRow(const string& row);
    static bool logWindowKindBudgeted(int mode, int kind);
    static string logWindowLogHeader(bool windowed, int keptTurns);
    static string logWindowOpeningDigest(const string& elidedPrefix);
    static string logWindowSettingLabel(int mode, int turns);
    static string logWindowSplit(const string& narration, int keepTurns, string& elidedPrefix, int& elidedTurns, bool& earlierTrimmed);
    static string logWindowSummaryLine(int elidedTurns, bool earlierTrimmed, const string& myGrave, const string& oppGrave, const string& myExile, const string& oppExile, const string& openingDigest);
    static string loopCautionClause(const string& converter, const string& mirror, bool theirs);
    static string loopChainEntrySentence(bool theirs);
    static string loopChainingNote(bool oppLoopProven, bool holdRowOffered, bool actingRowsPresent, bool actingRowTouchesLife);
    static string loopHalfAffordabilityClause(const string& half, const string& cost, bool castableNow, int untappedSources, const string& colours);
    static string loopNonChainingClause(const string& converter, const string& mirror, bool theirs);
    static string loyaltyClausePrefix(const string& menuText);
    static string loyaltyRowConsequenceTag(const string& label, bool hasteText, int emblemsLive);
    static bool magnitudeExprIsResolutionTimeOnly(const string& expr);
    static string manaAvailableLine(int sources, const string& colors, const string& sourceList, const std::vector<std::string> * perSourceColors= NULL);
    static string manaColourSetSymbols(const vector<int>& slots);
    static string manaColourSetText(ManaCost * potential);
    static int manaCostTextCmc(const string& costText);
    static string manaCostTokenText(const string& costString, bool castForMana, bool hasPrintedCost);
    static bool manaOnlyWindowSkippable(bool allManaOnly, bool midTransaction, int poolMana, const string& seatFloatStepKey, const string& nowStepKey);
    static string manaPoolClause(const string& pool, int poolAmount, int sources);
    static string manaSourceColoursText(const string& raw);
    static string manlandBoardTag(bool canAnimate, bool tapsForMana, const string& cost= "");
    static string markBareBackFaces(const string& clause);
    static string mdfcHandLandFaceTag(const string& backName, const string& tapMana);
    static int mdfcLandArrivalClass(const string& script, int& lifeCost);
    static string mdfcLandArrivalTag(int arrival, int lifeCost);
    static string mdfcLandArrivalTagFrom(const string& script, const string& printedText);
    static string mdfcLandFaceRow(const string& menuLabel, const string& faceName, const string& faceMana, const string& arrivalTag);
    static string mdfcLandPlayRowTag(const string& backName, const string& backMana, const string& frontName, const string& arrivalTag= string(), bool frontIsLand= false);
    static string mdfcRowHead(const string& printedName, const string& menuToken);
    static string mdfcSpellLandBackNote(const string& curFace, const string& otherName, const string& otherMana);
    static string mdfcSpellToggleNote(const string& curFace, const string& otherName, const string& otherCost);
    static string menaceAttackRestrictionTag(int minBlockers, int eligibleUntapped);
    static string menaceBlockPairingTag(int minBlockers);
    static string menaceBlockerBudgetLine(const vector<int>& needs, int totalAttackers, int blockersAvailable);
    static string menuConsequenceNarration(const string& subject, const string& option);
    static string menuFitTag(int leftAfter, const std::vector<int>& lostRows, int otherPricedRows);
    static bool menuHasReserveRow(const std::vector<string>& rows);
    static bool menuIsBareXAnnounce(const std::vector<string> * options);
    static string menuRowProseName(const string& row);
    static string menuRowShortName(const string& row);
    static string modalActorSwap(const string& seg);
    static bool modalChoiceModes(const string& magicText, std::vector<GptModalMode>& out);
    static string modalMirrorModeLabel(const string& script, const string& label);
    static int modalModeVerdict(bool outerIsSpec, int outerCount, bool outerPayload, bool subIsSpec, int subCount, bool subPresent);
    static string modalModesTag(const std::vector<std::string>& live, const std::vector<std::string>& halfDead, const std::vector<std::string>& dead, const std::vector<std::string> * liveObjects= NULL);
    static string modalResolutionOrderNote(const string& subject);
    static string modeEdictFloorTag(int theirCreatures, const string& onlyName, int minToughness, int atFloor);
    static string modeEffectPriceTag(const string& script, const string& optionLabel, int myLife, int oppLife, int theirDrawPunisherPerDraw= 0);
    static string modeLivenessRowTag(int verdict);
    static string modeRowAnnotations(const string& script, const string& optionLabel, int myLife, int oppLife, int candidatesCanBlockTapped, int candidatesDoNotUntap, int theirDrawPunisherPerDraw= 0);
    static string monotoneXRangeRow(size_t firstLabel, size_t lastLabel, int xFirst, int xLast, const string& firstRow, const string& lastRow);
    static string morbidMagnitudeClause(const string& ifMorbid, const string& ifNotMorbid, bool morbidLive);
    static bool morbidPTBranches(const string& lowText, string& ifMorbid, string& ifNotMorbid);
    static string moverGainDestination(const string& dest);
    static string mulliganKeepClause(int mullsTaken, int keepSize);
    static string mulliganNarration(bool kept, int handSize);
    static string mulliganNoCoverCause(int lands, const int sources[5], int cheapestCmc);
    static string mulliganRowLabel(int next);
    static string mutateAltCostLabel(bool hasMutate, const string& alternativeName);
    static string mutateHostAsk(const string& effectName, int placement);
    static string mutateOverUnderHeader(const string& ctxName);
    static int mutatePlacementFromLabel(const string& s);
    static string mutatedPileTag(const std::vector<string>& underNames);
    static string mutatedPileTextCore(const std::vector<std::pair<string, string> >& members, size_t maxLenPerCard);
    static bool nameEchoesRow(const string& nameIn, const vector<string>& rows);
    static int nameOrdinal(const string& seg);
    static string namedCardRemainingTag(int theirNotPublic, int myNotPublic);
    static string namedCardVisibilityTag(int theirBattlefield, int theirGraveyard, int myBattlefield, int myGraveyard);
    static bool namedCastPenaltyScan(const string& magicText, int& lifeLoss, int& draws);
    static string namedCastPriceTag(const string& sourceName, int lifeLoss, int draws, int life);
    static void narrationAppend(string& narration, string& pendingPhase, const string& line, const string& trimMarker, string * delta= NULL);
    static string narrationBucketRuns(const string& body);
    static string narrationFoldPaidSources(const string& body);
    static string narrationShapeKey(const string& line);
    static size_t narrationTrimKeep(size_t markerLen);
    static bool narrationTrimNear(size_t logSize, size_t lineSize, size_t pendingPhaseSize);
    static size_t narrationTrimTrigger();
    static bool narrationTrimV1();
    static bool naturalTextLess(const string& a, const string& b);
    static string noPotentialBlockersTag();
    static long offProtocolBytes(const string& replyIn, bool * actionBeforePlanOut= NULL, std::vector<string> * offLinesOut= NULL);
    static string oneShotDrawGrantTag(const std::vector<OneShotDrawBranch>& branches, const std::vector<std::string>& converters, int minePerDraw, int theirsPerDraw, const string& theirPunishers, int life= -1);
    static string oppLifeRaceClause(int dmg, int gain, int turns);
    static int opponentExtraDrawPerTurn(const string& script, bool& variable);
    static string opponentLifeTrendLine(const int lifeByTurn[3], const int turnNo[3], int samples, int nowLife, int eventGained= -1, int eventLost= -1);
    static string opponentOpenManaLine(int sources, const string& colours);
    static string opponentZoneCountsLine(int oppHandCards, int oppHandInReveal, int oppLibraryCards, bool deckOutBlocked= false);
    static string optionCardTextCore(const string& raw, size_t maxLen, const string& focusPrefix= "");
    static string optionLabel(const string& row);
    static bool optionRowMentions(const string& optionText, const string& name);
    static string optionSetKeyLine(const string& row);
    static string optionSetKeyOf(const std::vector<string>& rows);
    static string ownClockTag(const string& name, int copies, int perTurn, int oppLife);
    static int ownExtraDrawPerTurn(const string& script, bool& variable);
    static string ownTargetsOnlyClause(const string& names);
    static string ownerTag(bool mine);
    static int parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out, const vector<string> * optionNames= NULL, bool echoBinds= false, int * repeatedOut= NULL);
    static int parseAttackerTargets(const string& line, size_t nAttackers, size_t nTargets, vector<int>& out);
    static int parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out, const vector<string> * blockerNames= NULL, const vector<string> * attackerNames= NULL, const vector<vector<int> > * legalPerBlocker= NULL, int * dropped= NULL, bool * gangConflict= NULL, bool * blockerInAttackerSlot= NULL);
    static void parseDungeonRooms(const string& t, std::vector<std::pair<string, string> >& rooms);
    static bool parseLogWindowSetting(const char * raw, int& mode, int& turns);
    static int parseRepeatCount(const string& replyIn);
    static string passRowCleanupPriceTag(bool cleanupThisTurn, int handNow, int limit, int perDiscard, const string& punishers, int life, int stackedDraws);
    static bool payLifeAmountFromOption(const string & opt, int & amount);
    static string payRepeatBandRowTag(int bandPaid);
    static int payRepeatCollapse(const vector<string>& optionTexts, const vector<int>& paid, vector<bool>& bandRows, int& bandLow, int& bandHigh, int& bandPaid);
    static string payRepeatCollapseTag(int bandLow, int bandHigh, int bandPaid);
    static string payRepeatModeNote(const vector<string>& opts);
    static string payRepeatPerCounterCost(const string& magicText, const string& optionLabel);
    static string payRepeatRowCostTag(int counters, const string& perCost, int perCmc, int available, int paidCopies= -1);
    static string payRepeatTapsClause(const std::vector<std::string>& names, const std::vector<int>& restrictions, int paid, int counters, const string& untapSource= "");
    static string paymentLifeCostClause(const std::vector<std::string>& names, const std::vector<int>& damage, int life);
    static string paymentNoLifeCostClause(bool boardHasHarmSource, int totalDamage, int used);
    static string paymentReceiptLine(const string& cost, const string& target, const vector<string>& sources, bool fromPool);
    static string paymentTapsClause(const std::vector<std::string>& names, const std::vector<int>& restrictions, const string& untapSource= "");
    static string paymentTapsClause(const std::vector<std::string>& names, bool cannotAttackThisTurn);
    static string pendingLoopWarningText(const string& inPlayHalf, const string& seenHalf, const string& seenWhere, bool theirs, const string& affordClause= "", bool halfCanReturn= true, bool blockedByExile= true);
    static string pendingStackDamageLine(int dmgToMe, int myLife, int queuedDraws= 0, int perDraw= 0, const string& punisherNames= "");
    static string perColorSourceCountClause(const std::vector<std::string>& perSourceColors);
    static string plainDeclineScopeClause(bool holdRowOffered);
    static string planAgeClauseText(int windows, int turn);
    static string planAssertedAbsentBlock(const string& assertedName);
    static size_t planBlockEndOffset(const string& text, size_t planPos);
    static string planContradictedBlock(const string& deniedName);
    static bool planHardAged(int windows);
    static string planLibraryOnlyNote(const string& cardName);
    static string planLineOnly(const string& replyIn);
    static string planMenuDiffClause(const string& absentName);
    static bool planNamesStrandedCard(const string& plan, const string& cardName);
    static bool planRetractionServedAlone(int windows, bool retractionFires);
    static string planWithdrawnBlock(const string& ageClause, const string& absentClause);
    static string playerBranchLabel(const string& raw);
    static string poisonGainLine(bool mine, int delta, int total);
    static string poisonStatusLine(int mine, int opp, bool poisonLive);
    static long postAnswerOverrun(const string& reply);
    static string potentialBlockersTag(const vector<string>& entries, const string& biggest, const string& extraNote= string(), const string& gangNote= string(), const string& priciest= string(), const string& sumNote= string());
    static string pregameBottomNarration(int count, const string& names);
    static string pregameHandHeaderText(int handSize, int lands, int spells, const int sources[5], const string& cheapestLabel, int cheapestCmc, const vector<string>& reachable, const vector<std::pair<string, string> >& dual= vector<std::pair<string, string> >());
    static string printedClauseFor(const string& cardText, const string& label);
    static string printedFirstClause(const string& cardText);
    static string printedPTTag(int power, int toughness, int basepower, int basetoughness, int origpower, int origtoughness, int showsOtherFace);
    static string protocolLinesOnly(const string& replyIn);
    static string prunedPairsReaskClause(const string& prunedPairs);
    static bool ptDropKillsTarget(int n, int remaining);
    static string ptDropTargetVerdict(int n, int toughness, int remaining);
    static bool reasoningHiddenShape(bool requested, const string& content, const string& reasoning);
    static double reasoningNgramRepeatRatio(const string& s);
    static double reasoningRepetitionRatio(const string& s);
    static bool recordChoiceIsRowIndex(const char * kind);
    static string refusedChosenText(int choice, const char * fallback, const std::vector<string> * optionTexts);
    static string removalVictimTag(const string& verb, const std::vector<std::string>& victims, const std::vector<std::string>& immune, const std::vector<std::string>& mine= std::vector<std::string>());
    static string renderAbilityLabel(const string& raw, bool * mapped= NULL);
    static string renderMayBlockList(const vector<int>& labels, const vector<string>& parens, bool * rangeUsed);
    static int repeatAnnotationCount(const string& row);
    static bool repeatAskAnswerStands(const string& heldKey, const string& nowKey, const string& heldPlan, const string& nowPlan, int heldTurn, int nowTurn, int heldChoice, int nOptions);
    static string repeatAskKey(int turn, int phase, const string& decision, const vector<string>& rows);
    static string repeatMechanismClause(int loopCount);
    static int repeatPlanScanNumber(const string& plan, const char * label);
    static bool repeatPlanStopAndCurrent(const string& plan, int * stopOut, int * currentOut);
    static bool repeatPlanStopIsOwn(const string& plan);
    static string repeatRowFoldKey(const string& row);
    static string repeatRowLine(const string& shortName, int rowIndex, int creatureCount= -1);
    static string repeatRowShortName(const string& row);
    static string repeatRowStopClause(int creatureCount, int statedStop, int oppLifeAtStop= -1, int oppLifeNow= -1);
    static string repeatRunRangeLine(int fromIdx, int toIdx, int firstIdx, int baseFrom, int baseTo);
    static string repeatShortName(const string& line);
    static bool repeatStopClampApplies(bool repeatRowTaken, int stopTestCount);
    static int repeatStopClampCount(int namedCount, int statedStop, int statedCurrent);
    static string repeatStopClampReceipt(int named, int statedCurrent, int statedStop, int allowed);
    static bool repeatStopExecutesNothing(int allowed);
    static bool repeatedClauseBody(const string& row, const char * literal);
    static bool replyActionBeforePlan(const string& replyIn);
    static bool replyHasPlanLine(const string& replyIn);
    static bool replyTerminatedNaturally(const string& content, bool finishLength);
    static string reserveDeclineCarryNote(bool declinedThisTurn);
    static bool reserveDeclineStillStands(const string& heldBoard, const string& nowBoard, const string& heldKey, const string& nowKey, int heldSources, int nowSources, const char ** whyOut);
    static string reserveStrandedCardName(const string& chosenRow);
    static string restoreScriptTokenCase(const string& label, const string& printed);
    static string revealCostTag(const string& printedCost, int manaValue);
    static string revealDestinationClause(const string& optOneLabel, const string& zone);
    static string revealDestinationZone(const string& effect);
    static string revealDuplicateTag(bool legendary, int inMyHand, int onMyBattlefield);
    static string revealEligMarker(const string & optOneLabel);
    static bool revealRefusalMustPick(bool pickExactlyOne, bool singlePickDeclineLegal);
    static bool revealSinglePickDeclineLegal(const string& targetSpec);
    static bool revealSubjectIsMe(bool originOwnerIsMe, bool revealZoneOwnerIsMe);
    static string revealSummaryNarration(size_t revealedCount, const string& chosen, const string& optionLabel);
    static long revealWaitUnexplainedSecs(long waitSecs, long latencyMs);
    static string revealedCardNarration(bool mine, const string& cardName, const string& fromZone);
    static string riderTargetSpec(const string& script, size_t pos);
    static bool rowIsDeclineRow(const string& row);
    static bool rowIsRepeatBase(const std::vector<int>& repeatBaseRow, int choice);
    static string rowVerdictClause(const string& row);
    static int salvageLoopedBlocks(const string& content, size_t nBlockers, size_t nAttackers, const vector<string>& blockerNames, const vector<string>& attackerNames, const vector<vector<int> >& legalIdx, vector<int>& out);
    static int salvageLoopedSubset(const string& content, const char * label, size_t n, const vector<string>& names, vector<bool>& out);
    static string scriptAbilityCost(const string& script, const string& abilityName);
    static int scriptAbilityDrawCount(const string& script, const string& abilityName);
    static string scriptActivatedAnimateCost(const string& lowScript);
    static string scriptCostGloss(const string& costTokens);
    static bool scriptHasActivatedAnimate(const string& lowScript);
    static bool scriptHasWord(const string& low, const char * word);
    static int scriptPTDrop(const string& lowText);
    static int scriptSelfDrawCount(const string& script);
    static string secondCopyTag(const string& name, const string& magicText= string());
    static int secondCopyVerdict(const string& magicText);
    static string sentenceNaming(const string& text, const string& needle, size_t maxLen);
    static string sorceryReserveClause(int left, const string& name, const string& cost, int need, int landDropSources= 0);
    static string sourceBlockLine(int first, int last, int refFirst, int refLast, const string& sourceName, int rank, int refRank);
    static int spareReachCap(int myLands, int handLandCards);
    static bool splitCopyRowHandle(const string& row, string& head, string& tail, int& rank, int& total, string& scope);
    static bool splitMonotoneXRow(const string& row, string& erased, int& xval);
    static bool splitNoOpBandRow(const string& row, string& erased, int& nval);
    static bool splitReasoningBlock(string& content, string& reasoning);
    static bool splitRowHandle(const string& row, string& head, string& tail, int& rank);
    static bool splitSourceOrdinal(const string& row, string& masked, int& rank, string& sourceName);
    static string stackAbilityLine(const string& grantorName, const string& menuText, const string& victimName, bool grantorMine, const string& genericKind, const string& effectNote= "");
    static string stackAbilityName(const string& sourceName, const string& menuText);
    static string stackAnswerYesRowClause(int gain, int stackLossToMe, int myLife);
    static string stackDamagePhrase(int amount, bool toMe);
    static string stackDeathRowClause(int stackLossToMe, int myLife);
    static string stackDeathVerdictKey(int stackLossToMe, int myLife);
    static Player * stackDrawRecipient(Targetable * t);
    static string stackFactsCore(const string& cost, bool isCreature, bool isArtifact, const string& typeName, int power, int toughness);
    static string stackLifePhrase(int delta, bool toMe);
    static bool stackObjectIsRespondable(bool hasSource, bool sourceIsEmblemMarker);
    static string stackTargetTextCore(const string& snippet);
    static string stackTargetTextNote(MTGCardInstance * c);
    static string stackTrapEffectGerund(const string& menuLabel);
    static string stackTrapNoteText(const string& gerund, bool declineOffered);
    static string strandsHandCardTag(int used, int untapped, const string& name, const string& cost, int need);
    static string stripAnnotationBrackets(const string& s);
    static string stripDeclineReaskTags(const string& s);
    static string stripNarrationDecoration(const string& in);
    static string stripRenderAnnotationsLc(const string& s);
    static string stripRepeatAnnotation(const string& line);
    static string stripTrailingListGloss(const string& line, vector<string> * namesOut);
    static string stripTrailingListGlossLines(const string& reply, vector<string> * namesOut);
    static bool summoningSickRowApplies(bool sick, bool inPlay, bool controllerIsActivePlayer);
    static string summoningSickTag(bool canBlock);
    static string sweeperClause(const char * verb, int theirs, int theirsAbleToAttack, int mine, bool liveScope= false, const std::vector<std::string>& theirNames= std::vector<std::string>(), const std::vector<std::string>& myNames= std::vector<std::string>(), const std::vector<std::string>& theirSurvivors= std::vector<std::string>(), const std::vector<std::string>& mySurvivors= std::vector<std::string>(), int animatable= 0);
    static string sweeperRosterTail(const std::vector<std::string>& theirNames, const std::vector<std::string>& myNames);
    static string sweeperSurvivorTail(const std::vector<std::string>& theirSurvivors, const std::vector<std::string>& mySurvivors);
    static string tapCostBeforeCombatClause(const string& name);
    static string tapOutCrackBackClause(int leftAfter, int crackTotal, int crackAttackers);
    static string tapUntapBranchTag(const string& script, const string& optionLabel, int candidatesCanBlockTapped= 0, int candidatesDoNotUntap= 0);
    static bool tappedAnimateNeedsVerdict(const string& row);
    static string tappedCreatureTag(bool canBlockTapped, bool attacking, const string& blockedName, bool activeSide= true);
    static string tappedSourceAnimateClause();
    static string tappedSourceAnimateVerdict();
    static string targetChoiceNarration(const string& target, const string& source, const string& ability);
    static string targetCommitClause(int legalTargets);
    static string targetPreviewFacts(MTGCardInstance * c);
    static string targetZoneTag(Player * me, MTGCardInstance * c);
    static bool textCarriesSeatVoice(const string& snippet);
    static string textSnippetCore(const string& raw, size_t maxLen, bool completeClause= true);
    static string theirConverterBodyTag(int toughness, const std::vector<std::string>& theirConverters, int myLife, bool theirLoopClosed= false);
    static string theirDrawStepForecastText(int base, const std::vector<std::pair<std::string, int> >& extras, int perDraw, const string& loopCaution= "", bool stepIsNow= false, int holderLife= -1, int resolvedInStep= 0, int unattributedInStep= 0);
    static string toLowerCopy(const string & s);
    static string tokenCreatedNarration(bool mine, const string& tokenName, const string& creator, bool isCreature, int power, int toughness, const string& to);
    static string transformDfcToggleNote(const string& curFace, const string& otherName, const string& otherCost);
    static string transportRecordField(const std::vector<std::string>& failures, const std::string& roundTripStamp, long latencyMs);
    static string transportStampPhase(const string& transport);
    static string trimMarkerLine(const string& myGrave, const string& oppGrave, const string& myExile, const string& oppExile);
    static string tutorFindsTag(const string& type, const vector<string>& fresh, const vector<string>& copies);
    static string tutorSearchType(const string& magicText);
    static string uniqueNamedIn(const string& text, const vector<string>& names);
    static void unpermuteSelection(const vector<size_t>& order, size_t n, vector<bool>& send);
    static string unreachableAttackerTag(int canBlockCount, int totalBlockers, bool flyingExplains);
    static string upkeepAnimationClause();
    static string ventureSourceDungeonTag(const string& sourceName, const string& sentence);
    static string ventureSourceSilentTag(const string& sourceName);
    static string ventureStepLine(bool mine, const string& dungeonName, int step);
    static string w42Capitalize(const string& s);
    static bool w72CastAnsweredFactApplies(bool stampMatchesThisWindow, int castDecisionsOpen);
    static bool w72LandDropIsManaSource(bool basicManaSubtype, const string& script);
    static int w72NameMenuEngineIndex(int pick, bool pickIsDisplayIndex, const std::vector<size_t>& nameOrder);
    static bool w72RowIsDeclineOrHold(const string& row);
    static bool w72StopReachedWindowCollapses(bool everyBaseRowIsStopReached, bool anyStopReachedRow, int carriedStop, int planTurn, int nowTurn);
    static bool w73HoldExpiredByUntap(int heldTurn, bool ownTurnAtHold, int nowTurn);
    static bool w73RowTouchesLife(const string& row);
    static string w73SiblingBoardKey(const string& board);
    static bool w74ClockSourceRecurs(bool cannotUntap, bool creature, bool sick, bool haste);
    static bool w74ClockSourceRecurs(MTGCardInstance * c);
    static int w74HoldUnseenRows(const std::set<string>& lastKeys, const std::vector<string>& rows, HoldRowKeyFn keyOf= holdActionKeyRow);
    static bool w74RetryArmedHold(const string& armedPrompt, string& seen, long& sinceMs, long nowMs, long holdMs);
    static bool w74RowIsDecisionEmpty(const string& row);
    static bool w74ScriptGrantsHasteToMine(const string& scriptIn);
    static int w74TapOnlyDamagePerActivation(const string& scriptIn);
    static string w74XAbandonTraceLine(const string& cardName, int floating);
    static bool w74XPermuteToClimbing(vector<string>& shown, int capX);
    static int w74XPickToContractIndex(int pick, int capX, int rowCount, bool climbing, int optionCount);
    static string w75ChainFeedRow(const string& row, bool oppLoopLive);
    static bool w75LegendTwinControlled(const std::vector<W75LegendBoardCard>& board, const std::string& castName);
    static string w75MdfcFaceRowTag(bool isBack, const string& frontName, const string& backName, const string& backMana);
    static bool w75OwnClockSuppressed(int myLife, int incomingDamage, int stackDamage, bool oppLoopLive);
    static const char * w75ProtocolDeviationClass(bool planLineMissing, int offProtocolBytes, bool forcedCloseEcho= false, bool answerLabelAbsent= false);
    static int w76BlockableCoveredPower(const std::vector<int>& powers, const std::vector<int>& blockersNeeded, int blockerCount);
    static bool w76CastPermissionKeyword(const string& textIn);
    static bool w76CastableFromDeadZone(MTGCardInstance * c, bool inExile);
    static int w76ClosingRowInsertAt(int declineRowIdx, size_t menuSize);
    static bool w76CrackVictimTrulyRemoved(MTGCardInstance * c, int destroyKind);
    static string w76CrossPhaseListKey(int turn, const string& decision, const vector<string>& rows);
    static string w76CrossPhaseRePutNote(int windowsAgo, const string& phaseName, bool boardUnchanged);
    static bool w76ForceCloseArmAllowed(bool parkArmed);
    static int w76ForceCloseDecision(bool earned, bool parkArmed, int deferTicks);
    static string w76HoldReopenNote(W76HoldMemory& m, const char * seam, const std::vector<string>& rows, int windowSeq, bool commitAtBuild, HoldRowKeyFn keyOf);
    static void w76HoldWindowAsked(W76HoldMemory& m, const char * seam, int windowSeq);
    static void w76HoldWindowNotAsked(W76HoldMemory& m, const char * seam);
    static bool w76LifeGainReaches(bool gainerCantChangeLife, bool gainerNoLifeGain, bool foeNoLifeGainOpponent);
    static int w76LoyaltyRowTargetForm(const string& row, int& upTo);
    static bool w76OffProtocolIsPrefillEcho(const std::vector<string>& offLines, const string& prefill);
    static string w76PhaseFreeBoardKey(const string& board);
    static bool w76SelfRecursiveZoneScript(const string& scriptIn);
    static bool w76StatedLethalOnScreen(int myLife, int incomingCombat, int stackDamage, int crackBackDamage, bool oppLoopLive);
    static string w76StripBalancedAnnotationGroups(const string& row);
    static string w76UpToNTargetClause(int n);
    static void w77ApplyOwnLoopFeed(std::vector<string>& rows, bool ownLoopResolving);
    static void w77ApplyOwnLoopThreatFeed(std::vector<string>& rows, const string& tag);
    static int w77ConsumeWindowSeq(int& windowSeq, const char * kind);
    static string w77ConverterTakeClause(bool theyGain, const string& convName, int convTakes, bool upTo= false);
    static int w77CoverBodyContribution(MTGCardInstance * c, int destroyKind);
    static string w77CoverClauseFor(bool sweep, int total, int myLife, bool totalIsFloor, int removedPower, int removedBodies, const std::vector<W77RemovalVictim>& victims, int attackerBodies);
    static void w77DropUnaskedCastNote(string& promptNote, string& castNote, int& refSeq);
    static string w77EdictPlayerTargetTag(bool targetIsOpponent, int creatures, const string& onlyName, int onlyToughness, int minT, int maxT, const string& convName, int convTakes, const string& engineTail= "");
    static void w77GateMeteredGains(bool canGainLife, int& sureGain, int& mayGain, int& lifelinkGain);
    static string w77KeyTailOf(const string& tail);
    static int w77LoopVerdictFrom(bool provenWin, bool ownLoopEventPending, bool theirStackTargetsComponent);
    static string w77OwnLoopRow(const string& row, bool ownLoopResolving);
    static string w77OwnLoopThreatTag(const string& theirSpell, const string& component);
    static string w77OwnLoopVerdictKey(int state, const string& theirSpell, const string& component);
    static bool w77PlanNamesAsCastIntent(const string& planIn, const string& cardName);
    static string w77StayHomeCoverTag(int total, int myLife, bool totalIsFloor, int bodies, int covered, bool anyTrampler= false);
    static int w77StayHomeCoveredPower(const std::vector<CrackBackAttackerFact>& atk, int bodyCount);
    static string w77TargetedRemovalCoverTag(int total, int myLife, bool totalIsFloor, const std::vector<W77RemovalVictim>& victims, int attackerBodies);
    static bool w78AnswerLabelAbsentShape(const std::vector<string>& offLines, bool hadLabelledAnswer);
    static bool w78AnyRepeatRow(const vector<string>& rows);
    static size_t w78ClauseCutAtOrBefore(const string& text, size_t limit);
    static bool w78ConditionalCounterScript(const string& magicText);
    static string w78CounterCostBody(const string& counterName, int nb, int have, const string& sourceName);
    static string w78CounterEscapeCost(const string& magicText);
    static std::vector<string> w78DeferOptionTexts(const std::vector<string>& rawRows);
    static bool w78DeferRecordMoves(bool haveMenuController, bool controllerIsSelf, bool controllerIsGptSeat, bool controllerHasTransLog);
    static size_t w78FirstClauseCut(const string& text);
    static bool w78ForceCloseBusy(bool parkArmed, bool thisArmArmed);
    static bool w78ForceCloseSameArmBusy(bool parkArmed, bool thisArmArmed);
    static string w78HoistSharedCardText(vector<string>& rows);
    static void w78HoldLatchKeys(const std::vector<string>& rows, HoldRowKeyFn keyOf, std::vector<string>& out);
    static void w78HoldRowDelta(const std::vector<string>& lastRowKeys, const std::vector<string>& nowRowKeys, int& unseen, int& gone);
    static void w78HoldRowKeys(const std::vector<string>& rowKeys, std::vector<string>& out);
    static string w78HoldRowShortName(const std::vector<string>& rows);
    static bool w78HoldStillStands(const std::vector<string>& heldLatchKeys, const std::vector<string>& nowRows, const char ** whyOut, HoldRowKeyFn keyOf);
    static void w78HoldWindowRecordSeq(W76HoldMemory& m, const char * seam, int windowSeq, int recordSeq);
    static bool w78IsDrainLinkType(int interruptibleType);
    static bool w78KeepCandidateStands(int payableSources, int untappedSources);
    static string w78KeepCostText(const string& printedCost, bool hasAlternative);
    static int w78LoopVerdictFrom(bool provenWin, bool ownLoopEventPending, bool theirStackTargetsComponent, bool theirStackSweepsComponentType, bool theirObjectAboveOwnTrigger);
    static bool w78MassRemovalOfTypeScript(const string& magicText, const string& typeWordLc);
    static string w78OwnLoopVerdictLine(int state, const string& theirSpell, const string& component);
    static int w78PayableSourceCount(Player * p, MTGCardInstance * hc);
    static string w78RePutCollapseIdentity(const std::vector<string>& actingRows, const string& boardScope, int carriedStop, int stopTurn, int stopCountAtStatement, const char * stopSource);
    static bool w78RePutIdentityStands(const string& recordedAtDecline, const string& nowIdentity);
    static string w78RedundantCounterTag(const string& myCounterName, bool conditional= false, const string& escapeCost= string());
    static bool w78RepeatSurfacesAgree(const vector<string>& rows, const string& tail);
    static string w78RowNumberList(const vector<size_t>& idx);
    static bool w78RowsUnchangedSinceLastAsk(const string& holdCheckNote);
    static string w78ShortNameParenthetical(bool hasRepeatRow);
    static string w78StackDrainNote(int theirTriggers, bool rowsUnchangedSinceLastAsk, const string& holdRowShortName);
    static bool w78StagedNoteIsEmpty(W76HoldMemory& m, const char * seam);
    static bool w78StopReachedRePutCollapses(bool everyBaseRowStopPriced, bool anyStopReachedRow, int carriedStop, int planTurn, int nowTurn, int declinedN, bool identityStands);
    static string w78StripHandleOrdinal(const string& key);
    static string w78StripSharedCardTextHeader(const string& s);
    static string w78TrimTail(const string& s);
    static void w78UnitDepths(const string& text, std::vector<int>& depth);
    static size_t w78UnitSafeCut(const string& text, size_t at);
    static void w79ApplyLoopFaceAtSend(bool sent, const string& pendingFace, string& faceOut, bool& countIt);
    static string w79AskScopeKey(int turn, int phase, const string& continuation= string());
    static int w79BestBlockDamage(const vector<W79AtkFact>& atk, const vector<W79BlkFact>& soak, const vector<vector<char> >& can, vector<int> * matchOut);
    static int w79BestBlockDamage(const vector<W79AtkFact>& atk, const vector<int>& soak, const vector<vector<char> >& can, vector<int> * matchOut);
    static bool w79ClauseNeedsAntecedent(const string& clause);
    static string w79ClausePrioritySnippet(const string& text, size_t maxLen);
    static string w79CloseOpenUnits(const string& kept);
    static bool w79CounterspellScript(const string& magicText, bool castableAtInstantSpeed= true);
    static int w79CoveredByBodies(std::vector<CrackBackAttackerFact> sorted, int bodies);
    static bool w79DecisionBearingClause(const string& clause);
    static bool w79ForcePassNoProgress(bool keyUnchanged, int lastChoice, bool boardMoved);
    static string w79HoldVerdictForCompare(const string& heldMarker, const string& nowMarker);
    static bool w79LabelAbsentRead(const char * devClass, bool handedToHeuristic);
    static bool w79LabellessAnswerLine(const std::string& text, size_t planLineStart, bool hadCodedAnswerLine, size_t * segStart, size_t * segEnd);
    static bool w79NumberedRowBody(const string& line, size_t * bodyAt);
    static string w79OwnLoopVerdictLineFor(int state, bool loopProven, const string& theirSpell, const string& component, const string& starterName);
    static int w79PunisherLineAmount(const string& lineLc, int declaredCount, bool& readable);
    static string w79RePutCollapseIdentity(const std::vector<string>& actingRows, int carriedStop);
    static bool w79RecoveryLabelAbsent(const char * devClass);
    static int w79ResidualFor(const W79AtkFact& a, const vector<W79BlkFact>& blk, unsigned mask);
    static bool w79SharedCardTextPays(size_t rows, size_t bodyLen, size_t rowListLen);
    static bool w79StopReachedRePutCollapses(bool everyBaseRowStopPriced, bool anyStopReachedRow, int carriedStop, int declinedN, bool identityStands);
    static bool w79UntapEngineLineQualifies(const string& lineLc, bool sideIsSeat);
    static int w79VerdictDangerRank(const string& marker);
    static bool w80AnswerPrecedesPlan(size_t labelLineStart, size_t planMarkerPos);
    static string w80CarriedPlanSteps(const string& plan, int stepsDone, bool castPendingCompletion, const string& tail= string());
    static bool w80CompletingCastMenu(const string& tail, bool inAnnounceXAsk);
    static string w80CompletingDeclineSpentClause(int sources, const string& card);
    static bool w80CountRenderedAtSend(bool sent, bool rendered, int windowSeq, int& countedSeq, int& counter);
    static const char * w80CoverMechanismSentence(bool anyTrampler);
    static string w80CrackBackFaceOfLine(const string& line);
    static string w80CrackBackVerdictLine(const string& face, int bestBlockFloor= -1, int myLife= 0);
    static string w82WindowKey(const string& boardKey, const string& question); //#W82-A (L2)
    static bool w80DrawExtraIsOptional(const string& payloadLow);
    static size_t w80EchoStepIndex(int stepsDone, bool castPendingCompletion);
    static bool w80EtbSelfLeavesLine(const string& low);
    static bool w80HoldRowOnMenu(const std::vector<string>& rows);
    static string w80LoopFaceOfLine(const string& line);
    static bool w80LoopStarterScript(const string& magicText);
    static string w80LoyaltyClauseLabel(const string& clause);
    static bool w80NewLethalThreat(const string& liveMarker, const string& heldIdentity, const string& liveIdentity);
    static string w80OwnStackSpellTag(bool mine, const string& zoneName);
    static string w80ProvenWinLoopLine(const string& starterName);
    static string w80SacrificeSpendsBlockerClause(int total, int myLife, bool floorTotal, int give, int bodies);
    static string w80SelfLeavesNoCoverClause(const string& cardName, int crackTotal);
    static string w80StackDeathVerdictLine(const string& face);
    static string w80StackNotEmptyReason(bool stackBlockRendered);
    static bool w80StarterIsLive(int kind, bool abilityUsableNow, bool aCreatureCanEnter, bool thisBodyCanAttackNow= false);
    static int w80StarterLineKind(const string& low);
    static string w80TrimDanglingTail(const string& kept);
    static bool w80VariableDrawIsHandSize(const string& script, const char * zone);
    static bool w81AttackCoverDue(bool crackBackLinePrinted, bool anyAttackerOffered);
    static string w81CounterspellsSeenLine(int timesCountered, const vector<string>& names, int theirUntapped);
    static string w81CrackBackTotalFace(const string& line);
    static string w81CrackBackVerdictLine(const string& face, int rawCombat, int addBlockable, int addUnblockable, int compulsoryDraw, int bestBlockFloor, int myLife, bool addUnsized= false);
    static int crackBackRungConvertedCost(const string& rung); //#W82-A (L5)
    static string w81EngineBodyTail(MTGCardInstance * c);
    static void w81FoldDuplicateCoverParagraphs(std::vector<std::string>& rows);
    static size_t w81NarrationBudget(size_t otherBytes);
    static bool w81StarterEventNeedsOpponent(const string& ev);
    static void w81StepNameCandidates(const string& step, vector<string>& out);
    static string w81TrimNarrationToBudget(const string& body, size_t keep, const string& marker);
    static bool w81XSweepMarkerNamesOwnLosses(const string& row);
    static string walkerOnlyExclusionLine(int walkerOnlyRows);
    static bool wallMissAbandoned(bool pending, const string& missedBase, const string& nowBase);
    static string wallMissClassFor(const string& phase);
    static string winFoldBlockedTail(int myLife, int lifeLossFirst);
    static string wipeSurvivorTail(int damage, const std::vector<WipeVictim>& hit);
    static string xAffordabilityCore(int maxX, int baseCMC);
    static string xAnnounceLibraryNote(int capX, int drawPerX, int library, int reserve, const string& reserveWhy, int libX);
    static int xBestFreeKillX(const std::vector<XDamVictim>& victims, int capX);
    static int xBestTradeX(const std::vector<XDamVictim>& victims, int capX, int * theirsOut= NULL, int * mineOut= NULL, bool * lopsidedOut= NULL);
    static bool xCastIsOnlyXDamage(const string& magicText);
    static string xCastRemainderScopeTag(int maxX, int baseCMC, bool colouredX, const string& keepName= string(), const string& keepCost= string(), int keepNeed= -1, int totalMana= -1, bool keepIsAnotherCopy= false, const string& instName= string(), const string& instCost= string(), int instNeed= -1, bool instIsCounter= false);
    static string xCastRowBestXMarker(const XVictimSurvey& sv, int lifePerX, int drawPerX, int netAtCap= kXNetNotSupplied, int life= -1, int safeX= -1, int netAtSafeX= kXNetNotSupplied, const string& libraryClause= "", int libX= -1, int stackLossToMe= 0);
    static string xCastRowMarkerFrom(const string& menuMarker, int bestX, bool namesBestX);
    static string xDamageSweepCore(int maxX, int baseCMC, const std::vector<XDamVictim>& victims, bool hitsMe, bool hitsOpp, int myLife, int oppLife);
    static string xDamageTargetedCore(int maxX, int baseCMC, const std::vector<XDamVictim>& victims);
    static string xDeclineForfeitClause(int floating);
    static string xDrawPunishClause(int maxX, int drawPerX, int perDraw, const string& punishers);
    static void xKillRowAnnotations(const std::vector<XDamVictim>& victims, int capX, bool sweep, bool hitsMe, bool hitsOpp, int myLife, int oppLife, bool castIsOnlyXDamage, std::vector<string>& out);
    static string xKillRowCore(const std::vector<XDamVictim>& victims, int atX, bool sweep, bool hitsMe, bool hitsOpp, int myLife, int oppLife);
    static string xLibraryCeilingClause(int capX, int drawPerX, int library, int reserve, const string& reserveWhy, int libX);
    static int xLibraryCeilingX(int capX, int drawPerX, int library, int reserve);
    static int xLibraryReserveCount(int drawStepSize, int stackDraws);
    static string xLibraryReserveWhy(int drawStepSize, int stackDraws, int mayDraws= 0);
    static string xLibraryRowClause(int cards, int library, int owedDraws);
    static bool xLifeDrawClauses(const string& magicText, int& lifePerX, int& drawPerX);
    static string xLifeDrawEffectClause(int lifePerX, int drawPerX);
    static void xLifeDrawRowAnnotations(int capX, int lifePerX, int drawPerX, int punisherPerDraw, const string& punishers, std::vector<string>& out, int handAfterCast= -1, int handLimit= -1, int perDiscard= 0, const string& discardPunishers= "", int stackedDraws= 0, int library= -1, int owedDraws= 0);
    static string xLifeDrawRowCore(int x, int lifePerX, int drawPerX, int punisherPerDraw, const string& punishers, int handAfterCast= -1, int handLimit= -1, int perDiscard= 0, const string& discardPunishers= "", int stackedDraws= 0);
    static string xMarkerRestate(const std::vector<XDamVictim>& victims, int atX);
    static int xMenuMarkX(const std::vector<XDamVictim>& victims, int capX, string& markerOut);
    static string xMonotoneMarker(int capX, int lifePerX, int drawPerX, int netAtCap= kXNetNotSupplied, int life= -1, int safeX= -1, int netAtSafeX= kXNetNotSupplied, const string& libraryClause= "", int stackLossToMe= 0);
    static int xNetLifeForX(int x, int lifePerX, int drawPerX, int punisherPerDraw, const string& punishers, int handAfterCast, int handLimit, int perDiscard, const string& discardPunishers, bool * priced= NULL);
    static bool xReserveUpkeepAhead(bool myTurn, int phase);
    static bool xScriptDrawsRng(const string& magicText);
    static string xTradeMarker(int theirs, int mine, bool lopsided);
    static string xVictimList(const std::vector<XDamVictim>& victims, int atX, bool mine);
    static string yourHandDisplacedClause(int myHandInReveal);
    static string yourLibraryLine(int libraryCards, int myLibraryInReveal);
    static string zeroPowerAttackerTag(int power);
    static string zeroPowerBlockerTag(int minP, int maxP, bool anyTrample, bool anyMenace= false);
    static string zoneTagText(bool isDungeon, bool mine, const string& zoneName);

    //---- AIPlayerGPT.cpp file-scope constants the corpus reads ----
    static const char * kReplyProtocol;
    static const char * kAttackersTurnFacts;
    static const char * kBlockersTurnFacts;
    static const char * kBlockTradeTrustNote;
    static const char * kSecondMainAheadFact;
    static const char * kMainPhasesAheadFact;
    static const char * kPriorityAgainFact;
    static const char * kCastAnsweredFact;
    static const char * kNoPassRowFact;
    static const char * kDeclineRowFact;
    static const char * kRevealWindowScopeFact;
    static const char * kPlanTargetAbsentNote;
    static const char * kStalePlanNote;
    static const size_t kPlanCarryMaxChars;
    static const size_t kPlanCarryMaxSteps;
    static const char * const kEchoHeadWord;
    static const size_t kBoardEffectClauseFactor;
    static const size_t kBoardEffectSnippetSlack;
    static const size_t kBoardCreatureEffectLen;
    static const size_t kBoardCreatureEffectNameCap;
    static const char * kForeignEffectVoiceFrame;
    static const char * kGhostformNotRemovalTail;
    static const size_t kPotentialBlockersEnumerateMax;
    static const char * kPlanFirstLead;
    static const char * kAnswerLockPrefix;
    static const char * kLandDropConsequence;
    static const char * kLandDropDeclineRow;
    static const char * kBlockerRangeNote;
    static const size_t kVictimRosterGroupCap;
    static const char * kOptionRangeNote;
    static const size_t kSharedCardTextMinLen;
    static const int kEventRunCollapseFloor;
    static const int kXNetNotSupplied;
    static const char * kXMostKillsMarker;
    static const char * kXBestTradeMarker;
    static const char * kXNoKillsMarker;
    static const long kAnswerReserveTokens;
    static const long kDefaultReplyCeilingTokens;
    static const long kAnswerLockedRetryTokens;
    static const long kDefaultReasoningBudget;
    static const int kW76CloseDeferMaxTicks;
    static const char * const kLogWindowTrimMarkerHead;
    static const size_t kW81PromptByteTarget;
    static const size_t kW81NarrationFloor;
    static const char * const kAnnounceXPlanScopeNote;
    static const size_t kNarrationTrimTrigger;
    static const size_t kNarrationTrimKeepBytes;
    static const char * const kNoAttackThisCombatLine;
    static const char * kHandCastableNowTagText;
    static const char * kHandCastAnsweredTagText;
    static const int kRepeatActivationFloor;
    static const char * kPassPriorityRowText;
    static const char * kHoldPriorityRowHead;
    static const char * kHoldPriorityRowText;
    static const char * kHoldPriorityRowTextActivation;
    static const char * kHoldPriorityRowTextCast;
    static const char * kCastNoRowZeroFact;
    static const char * kChainFeedTail;
    static const int kW80StarterNone;
    static const int kW80StarterActivated;
    static const int kW80StarterEntersTrigger;
    static const int kW80StarterCycleTrigger;
    static const int kW81StarterOpponentTrigger;
    static const int kW81StarterAttackTrigger;
    static const int kRepeatRowFloor;
    static const int kRepeatRowMax;
    static const char * kSelfTargetClause;
    static const char * kSelfOnlyWindowNote;
    static const char * const kModeNoLifeNoDrawTag;
    static const long kBlockPairAnswerBytes;
    static const char * kAttackTargetScopeFacts;
    static const char * kLifeLoopAttackerRowTag;
    static const int kPreventNone;
    static const int kPreventFull;
    static const int kPreventPartial;
    static const char * kRevealPlanScopeFact;
    static const char * const kCardTextOpen;
    static const char * const kCardTextClose;
    static const char * const kSharedCardTextHeadLead;
    static const char * const kCompletingCastNote;
    static const size_t kLatchedLineKeep;
};
#endif //WAGIC_GPT_PARSETEST_BUILD

#endif //WITH_GPT_AI
#endif //_AIPLAYERGPTSELFTESTACCESS_H_
