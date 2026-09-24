//AIPlayerGPTInternal.h - #W82-P12: the declarations shared between the
//AIPlayerGPT translation units (AIPlayerGPT.cpp = prompt facts + presentation,
//AIPlayerGPTTransport.cpp = the async request lifecycle and transport,
//AIPlayerGPTTranslog.cpp = telemetry records, AIPlayerGPTParse.cpp = answer
//selection and parsing, AIPlayerGPTSeams.cpp = the decision seams). Every
//symbol here was a file-static or anonymous-namespace helper of the one-file
//edition, referenced from a region that now compiles apart; the definitions
//are unchanged (the `static` dropped, default arguments stated here). Not a
//self-contained header: include it after AIPlayerGPT.h and the engine headers,
//as each of those TUs does.
#ifndef AIPLAYERGPT_INTERNAL_H
#define AIPLAYERGPT_INTERNAL_H

#ifdef _WIN32
#include <direct.h>
#define GPT_MKDIR(p) _mkdir(p) //Windows mkdir takes no mode argument
#else
#define GPT_MKDIR(p) mkdir((p), 0755)
#endif

extern bool gLastPutGlossStripped;
extern const char * kAnswerLockPrefix;
extern const long kAnswerLockedRetryTokens;
extern const long kAnswerReserveTokens;
extern const char * kAttackTargetScopeFacts;
extern const char * kAttackerRangeNote;
extern const char * kAttackersTurnFacts;
extern const size_t kBattlefieldCollapseFloor;
extern const long kBlockPairAnswerBytes;
extern const char * kBlockTradeTrustNote;
extern const char * kBlockerRangeNote;
extern const char * kBlockersTurnFacts;
extern const char * kCastAnsweredFact;
extern const char * kCastNoRowZeroFact;
extern const long kDefaultReasoningBudget;
extern const long kDefaultReplyCeilingTokens;
extern const char * kExampleFakeCardLc;
extern const char * kForceCloseTag;
extern const char * kGhostformNotRemovalTail;
extern const char * kHoldPriorityRowShortHead;
extern const char * kHoldPriorityRowText;
extern const char * kLandDropConsequence;
extern const char * kLandDropDeclineRow;
extern const char * kMainPhasesAheadFact;
extern const char * kNoPassRowFact;
extern const char * kPassPriorityRowText;
extern const int kPlanAnswerExtraLine;
extern const int kPlanAnswerNoteNone;
extern const size_t kPlanCarryMaxChars;
extern const size_t kPlanCarryMaxSteps;
extern const char * kPlanFirstLead;
extern const size_t kPotentialBlockersEnumerateMax;
extern const char * kPriorityAgainFact;
extern const long kPutSlotAnswerBytes;
extern const int kRepeatAbsentWindows;
extern const int kRepeatRowMax;
extern const char * kReplyProtocol;
extern const char * kRevealWindowScopeFact;
extern const char * kSecondMainAheadFact;
extern const char * kSelfOnlyWindowNote;
extern const char * kSelfTargetClause;
extern const char * kTimeoutRetryTag;
extern const size_t kLatchedLineKeep;
extern const int kPreventNone;
struct CastRowBoardAnswer;
struct LandTapGate;
struct CombatWindowCache;
string actionTakenNarration(const string& action, bool activationVerb = false);
MTGCardInstance * activeDungeon(MTGPlayerCards * g);
string allAssignmentsIllegalClass(const string& prunedPairs, bool exhausted);
string allCastRowsDeadNote(bool allDead, int castRows, int landPlaysLegal = 0, bool anyTagDead = false);
string animatedThisTurnNote(MTGCardInstance * c);
void appendParseNote(std::string * noteOut, const char * sig);
void appendPlainDeclineScope(std::vector<string>& rows, bool holdRowOffered);
void appendStackDeathToDeclineRows(std::vector<string>& rows, int stackLossToMe, int myLife);
void applyBoardSweepMark(std::vector<std::string>& rows, const std::vector<int>& theirs, const std::vector<int>& mine, const std::vector<std::string> * engines = NULL, const std::vector<std::string> * myNames = NULL, const std::vector<std::string> * myEngines = NULL);
void applyDuplicateEffectTags(std::vector<std::string>& rows, const std::vector<std::string>& names, const std::vector<int>& costs);
void applyMenuFitTags(std::vector<std::string>& rows, const std::vector<int>& uses, int untappedSources, int crackTotal = 0, int crackAttackers = 0);
ActivatedAbility * asActivatedForCount(MTGAbility * a);
AATurnSide * asTurnSide(MTGAbility * a);
string askExemplar(const vector<string>& options, int * usedRow = NULL);
int askWindowKindForAsk(const string& decision, const vector<string>& options);
int askWindowKindForPriority(const vector<string>& rows, bool stackRespondable);
const char * askWindowKindName(int kind);
bool asyncLandArm(const string& seamTail);
int attackDeclarationPunisherSize(Player * opp, int declaredCount, bool& sized);
string attackDeclarationPunishers(Player * opp);
string attackTotalLine(int attackers, int totalPower, int oppLife, int blockers, int guaranteed, int infectExcluded = 0, bool damageSuppressed = false, int blockGain = 0, const string& attackPunishers = "", bool oppLifeLoop = false, bool * outKillClaim = NULL, int blockLifelink = 0, int selfConverterLifelink = 0, int punisherPerAttacker = 0, int punisherKilled = 0, int punisherKilledPower = 0, const string& punisherVictims = "");
string attackerBlockerCountLine(int blockers, int evasive = 0, int attackers = 0);
string attackerBlockerEntry(const string& name, int power, int toughness, const string& outcome, const string& blockTrigger);
string attackerPoisonNote(MTGCardInstance * c);
string attackersHeldSickLine(const std::vector<std::string>& names, const std::vector<bool>& nextTurn = std::vector<bool>());
std::vector<long> attemptMsField(long firstMs, long secondMs, long latencyMs);
bool auditMOff();
string blockCountRequirementTag(int minBlockers);
int blockPairMaterialRank(MTGCardInstance * blocker, MTGCardInstance * attacker);
string blockPriceSumTag(int priced, int sureSum, int maySum);
const char * blockRestrictionReason(MTGCardInstance * mine, MTGCardInstance * theirs);
void blockTriggeredLifeFor(MTGCardInstance * blocker, int& sure, int& may);
int blockerLifelinkGain(int blkPower, int blkToughness, bool blkLifelink, bool blkFirstStrike, int atkPower, bool atkFirstStrike, bool atkDeathtouch, bool blkDoubleStrike = false);
int blockingLifeCeiling(const std::vector<int>& perBlockerLife, const std::vector<bool>& canBlockSomeOffered);
string buildForcedSacrificeAsk(const string& effectName, bool byOpponent, int gain, const string& engineRows = "");
int cardIdOf(MTGCardInstance * c);
string cardTextSnippet(MTGCardInstance * card, size_t maxLen);
string castAbandonedNarration(const string& card, int floating);
int castBodiesAdded(MTGCardInstance * card);
int castBodiesNetOfOwnText(int bodies, bool cardIsCreature, bool legendTwinControlled, bool selfLeavesOnResolution = false);
bool castBodyEntersTapped(MTGCardInstance * card);
string castDeclineRow(bool combatNext);
string castDrawPriceRowTag(int perCast, const string& castNames, int perDraw, const string& punishers, int life = -1, int priorCharge = 0);
string castKillSummaryTag(const std::vector<std::string>& killed, int creatureTargets, const string& magnitude, const string& playerTail = "", const std::vector<std::string>& killedMine = std::vector<std::string>());
string castKillVerdictNow(GameObserver * g, Player * me, MTGCardInstance * card, int oppLifeGain = 0, int oppGainTurns = 0);
string castModeCommitmentNote(bool castModeMenu, const string& ctxName, const string& committedName, bool sameTurn);
string castPlayerDamageTail(int dmg, bool oppTargetable, int oppLife, int myLife = -1, int lifeLossFirst = 0, int oppLifeGain = 0, int oppGainTurns = 0);
string castSetKeyOf(const std::vector<string>& castNames);
int castTriggerDrawCount(const string& magicText);
void castTriggerDrawScan(Player * opp, std::vector<std::string>& names, int& perCast);
string changelingAnnotation(MTGCardInstance * card);
string chooseANameHeaderText(const string& sourceName, const string& cardText);
bool ciStartsWith(const string& s, const string& name);
string cleanupDiscardHeaderText(int handN, int limit, int over, int perDiscard = 0, const string& punishers = "", int life = -1);
int codedAnswerCount(const string& reply);
string codedAnswerLineAt(const string& reply, int ordinal, size_t keep = kLatchedLineKeep, size_t * atOut = NULL);
size_t codedHeadEnd(const string& payload);
string codedLineTail(const string& payload, size_t headEnd, bool * terminal);
void collectMutatePile(MTGCardInstance * card, std::vector<MTGCardInstance *>& out);
string combatAttackOutcome(CombatWindowCache& cw, MTGCardInstance * attacker, MTGCardInstance * blocker, int blockerOnAttacker, bool foeLifeLoop = false, string * outBlockTrigger = NULL);
string combatDamageForecast(int life, int poison, int lifeIncoming, int poisonIncoming, int oppLife, bool oppLoopLive = false, int forcedCycleLoss = 0, const string& forcedCycleSource = "", bool figureOnFrame = false);
string combatTradePreviewStats(const CombatTradeStat& b, const CombatTradeStat& a, int preventAtoB = kPreventNone, int preventBtoA = kPreventNone, int preventAtoFace = kPreventNone, bool attackerSeat = false, int bRemaining = -1, bool bGainConverted = false, string * outBlockTrigger = NULL, bool * outBlockerDies = NULL, string * outBlockerLifelink = NULL, string * outAttackerLifelink = NULL, bool * outAttackerDies = NULL, bool foeLifeLoop = false);
void converterScan(Player * me, std::vector<std::string>& drawNames, std::vector<std::string>& discardNames);
string converterSituationLine(Player * me, Player * opp);
string copyOfTag(int rank, int total, const char * where);
string crackBackBlockerRowTag(int total, int myLife, int checkedBodies, int uncheckedBodies, const std::vector<CrackBackAttackerFact>& atk, bool totalIsFloor = false, int existingBodies = 0, int floorExtra = 0);
int crackBackBodyContribution(MTGCardInstance * ac);
bool crackBackBodyHoldsUpOthers(MTGCardInstance * dying, Player * opp, GameObserver * obs);
void crackBackCoverFacts(Player * opp, MTGCardInstance * card, int bodies, std::vector<CrackBackAttackerFact>& out, int& checkedBodies, int& uncheckedBodies, Player * me = NULL, int * existingOut = NULL, bool attacksSettled = false);
string crackBackFloorSources(Player * opp, int * sizeableOut = NULL, bool * unsizedOut = NULL, int * blockableOut = NULL, int * unblockableOut = NULL);
string crackBackKillRowTag(int total, int myLife, bool totalIsFloor, int removedPower, int removedBodies, int attackerBodies);
bool crackBackNextTurnDue(bool selfActive, int phase, int ableAttackers, int maxDamage);
string crackBackReliefClause(int total, int removed, int myLife, bool floorTotal, const string& staticSourceName = "", int sacrificeGiveBack = 0, int sacrificeBodies = 0);
string crackBackRemovalRowTag(int total, int myLife, bool totalIsFloor, int theirCreatures, int attackerBodies, int minAttackerPower);
bool crackBackScreenTotal(Player * me, Player * opp, GameObserver * obs, int& total, bool& isFloor);
int crackBackTotalOver(Player * opp, int * attackersOut);
int creatureCountOnBattlefield(Player * p);
bool damageKillsTarget(int dmg, int remaining, bool indestructible, bool deathtouch);
string damagePlaneswalkerVerdict(int dmg, int loyalty);
string damagePlayerVerdict(int dmg, int life, bool isMe, int myLife = -1, int lifeLossFirst = 0, bool myLifeLoop = false, bool poisonInstead = false, int poison = -1);
string damageTargetVerdict(int dmg, int toughness, int remaining, bool indestructible, bool deathtouch);
string declineFactForMenu(const vector<string>& options, bool callerSaysDecline);
int declineRowIndexOf(const vector<string>& options);
string declinedListNote(int n);
string describeTarget(Player * me, Targetable * t, bool decisionSurface = true);
string dfcOtherFaceName(const string& text);
string discardDeadTargetClause(int legalTargets);
void discardPunisherScan(Player * me, Player * opp, std::vector<std::string>& mine, int& minePerDiscard, std::vector<std::string>& theirs, int& theirsPerDiscard);
string drawPriceRowTag(int cards, int perDraw, const string& punishers, int life = -1, bool deferTotal = false, int discards = 0, int perDiscard = 0, const string& discardPunishers = "", bool discardIsCycleCost = false);
void drawPunisherScan(Player * me, Player * opp, std::vector<std::string>& mine, int& minePerDraw, std::vector<std::string>& theirs, int& theirsPerDraw);
string dungeonRoomBranchHeader(const string& dungeonName, int exploreCount, const std::vector<std::pair<string, string> >& rooms);
string dungeonRoomPositionTag(const string& dungeonName, size_t index, size_t total);
string dynamicMagnitudes(MTGCardInstance * card, CastRowBoardAnswer * ans = NULL);
void echoSignificantWords(const string& seg, vector<string>& out);
void edictFloorScan(Player * them, int& theirCreatures, int& minToughness, int& atFloor, MTGCardInstance ** onlyOut, int * maxOut = NULL);
string edictOnlyVictimOnStackClause(bool alreadyAimed);
const char * engineKindForScript(const string& magicText);
bool envFlag(const char * name);
string equipmentCastPriceTag(bool isEquipment, int ownCreatures, bool makesItsOwnBody);
bool everyCastRowDead(const std::vector<std::string>& rows, bool * anyTagDead = NULL);
string exileCastNote(Player * me, MTGCardInstance * card, const string& zoneLabel);
string exileHostageDescriptor(MTGCardInstance * held, Player * me);
string exileHostageRowTag(const vector<string> & held);
void exileHostagesOf(MTGCardInstance * card, vector<MTGCardInstance *> & out);
string feedsRowTag(int perTurn, bool variable, int perCast, const std::vector<std::string>& converters, const std::vector<std::string>& handConverters = std::vector<std::string>(), const std::vector<std::string>& discardConverters = std::vector<std::string>(), const std::vector<std::string>& handDiscardConverters = std::vector<std::string>(), int selfPerTurn = 0, int variableHandSize = -1);
string fetchLandColorsClause(const bool adds[5], const bool canMake[5], const int sourceCounts[5] = NULL);
string fetchMakesNoManaClause(int untapped, bool fetchedEntersTapped, const string& colorsClause);
bool findAnswerLabelLine(const string& text, const char * expectedLabel, size_t& segStart, size_t& segEnd, size_t& labelLineStart, int * extraAnswerLines = NULL, int * rejectedLines = NULL, size_t * lastHeadLineStart = NULL);
string firstLabelledLine(const string& replyIn, const char * labelLc);
size_t firstLineLeadingPlanPos(const string& text);
string foldManaBillClauses(const string& row);
void forcedCleanupInputs(MTGCardInstance * card, Player * me, Player * opp, int& handAfterCast, int& limit, int& perDiscard, string& discardPunishers, int * stackedDrawsOut = NULL);
string forcedSacrificeRowTag(int gain, int toughness, const char * engineKind = NULL, const string& convName = "", int convTakes = 0, bool gainReaches = true);
bool forcedSacrificeShape(const string& lowScript, bool allMineBattlefieldCreatures, bool chooserIsTargeted);
string gangBlockPriceTag(int need, int damage, bool anyOfThem, bool mine = false, bool resultsBelow = false, bool byDeathtouch = false, bool afterFirstStrike = false, int trampleSpill = -1, int trampleSoak = 0);
int gangKillNeed(int atkPower, int atkToughness, bool atkFirstStrike, bool atkDeathtouch, const vector<GangBlockerStat>& g, int minBlockers, int * outDamage, bool * outAnyOfThem, bool * outByDeathtouch, int * outSoak = NULL);
bool gateSideBranches(const string& script, const string& label, string& labelA, string& specA, string& labelB, string& specB);
string gateSideCensusTag(const string& labelA, int nA, const string& labelB, int nB);
long gptDeclarationAnswerFloorTokens(long items, long bytesPerItem);
bool gptForceCloseEarned(bool reasoningOnly, bool contentEmpty, bool finishLength, bool haveReasoning, int codedAnswers);
long w82RetryReasoningBudget(long phase1Budget); //#W82-EA (H6)
bool w82RetryKeepsThinking(bool thinking, bool legacyPrefill); //#W82-EA (H6)
int gptForceCloseOutstanding(bool liveArmed, bool parkArmed);
string gptForceClosePrefillBody(const string& prefill);
bool gptInlineChoiceOnPlanLine(const string& text, size_t& segStart, size_t& segEnd, size_t& lineStart);
long gptSeamMaxTokens(const char * seam, long ceilingTokens);
int gptSelectAnswerIndex(const std::vector<bool>& usable, const std::vector<bool>& clean);
int gptStackPendingDrawsFor(GameObserver * observer, Player * seat, MTGCardInstance * exclude);
void groupCombatCandidates(const vector<string>& names, const vector<string>& handles, vector<size_t>& order);
void groupNumberedRows(const vector<string>& rows, vector<size_t>& order);
void handConverterScan(Player * me, std::vector<std::string>& drawNames, std::vector<std::string>& discardNames);
bool hasCodedAnswerLine(const string& content);
string headParenthetical(const string& answer);
string heldBackBlockTag(const vector<string>& cannotBlock, int totalOpposing);
int heuristicRevealIndex(const std::vector<int>& cmc, const std::vector<bool>& eligible);
string holdContractParagraph();
string holdKeyRow(const string& row);
int holdRowIndexOf(const std::vector<string> * optionTexts);
string holdRowLine(bool castSeam = false, bool activationLive = false);
string instanceHandle(MTGCardInstance * card);
bool isAnimationRow(const string& line);
bool isExampleEchoLine(const string& line);
bool isFetchCrackLine(const string& line);
bool isLandBackedDisplayToggle(MTGAbility * a, MTGCardInstance * click);
bool isManaOnlyAction(MTGAbility * a);
bool isRenderVocabWord(const string& w);
bool isReservedHoldEcho(const string& echoLc);
bool isReservedPassEcho(const string& echoLc);
bool isTemplatePlaceholderLine(const string& line);
string joinBlockerRows(const vector<string>& names, const vector<string>& handles, const vector<string>& rests, bool * rangeUsed, const char * label = "B");
string joinNumberedRows(const vector<string>& rows, bool * rangeUsed, unsigned * formsUsed = NULL);
string joinTargetEntries(const vector<string>& names, const vector<string>& handles, const vector<string>& tails, const std::map<string, string>& notes);
string joinZoneEntries(const vector<string>& names, const vector<string>& handles, const vector<string>& tails, bool collapse);
string keywordList(MTGCardInstance * card);
void landColorFlags(MTGCardInstance * card, bool have[5]);
string landDropAskText(size_t landCount);
string landDropThreatTag(const string& rawScript);
string landTag(MTGCardInstance * card);
LandTapGate landTapGateScan(const string& script);
string landTapMana(const string& text);
string laterStepRouteClause(const string& offendingName, const std::vector<string> * rows);
string leavesFloatingTag(int poolTotal, int spent);
string leavesUntappedTag(int untappedSources, int sourcesUsed);
string legendRuleHeaderText(const string& name, int copies);
string legendRuleTargetClause(const string& name, int copies);
string legendTwinTag(const string& name, int loyalty = -1);
bool lifeLoopProvenWin(Player * me);
bool lifeLoopWinnable(bool loopClosed, bool theirLifeCanChange, bool iCanGainLife);
bool lifeLossMirrorScript(const string& magicText);
string lifeMenuLabelTag(int amount, bool gain, const string& sourceName, const string& sourceText);
bool lifeToDamageConverterScript(const string& magicText);
int lifeToDamageConverterTake(const string& magicText, int gain);
int listCopyRank(const std::vector<MTGCardInstance*>& cards, size_t index, int & outTotal);
size_t listKeyHash(const string& s);
string loopChainingNote(bool oppLoopProven, bool holdRowOffered, bool actingRowsPresent, bool actingRowTouchesLife);
bool makesCreatureToken(MTGAbility * a);
string manaCostToken(MTGCardInstance * card);
bool manaOnlyWindowSkippable(bool allManaOnly, bool midTransaction, int poolMana, const string& seatFloatStepKey, const string& nowStepKey);
string markBareBackFaces(const string& clause);
int mdfcLandArrivalClass(const string& script, int& lifeCost);
string mdfcLandArrivalTag(int arrival, int lifeCost);
string mdfcLandArrivalTagFrom(const string& script, const string& printedText);
string mdfcLandFaceRow(const string& menuLabel, const string& faceName, const string& faceMana, const string& arrivalTag);
string mdfcLandPlayRowTag(const string& backName, const string& backMana, const string& frontName, const string& arrivalTag = string(), bool frontIsLand = false);
bool mdfcOtherFaceIsLand(MTGCardInstance * c);
string mdfcRowHead(const string& printedName, const string& menuToken);
string menaceAttackRestrictionTag(int minBlockers, int eligibleUntapped);
string menaceBlockPairingTag(int minBlockers);
string menaceBlockerBudgetLine(const vector<int>& needs, int totalAttackers, int blockersAvailable);
string menuConsequenceNarration(const string& subject, const string& option);
bool menuHasReserveRow(const std::vector<string>& rows);
bool modalChoiceModes(const string& magicText, std::vector<GptModalMode>& out);
bool modalModeLiveness(GameObserver * observer, MTGCardInstance * card, const string& script, std::vector<std::string>& live, std::vector<std::string>& halfDead, std::vector<std::string>& dead, std::vector<int> * verdicts = NULL, std::vector<std::string> * liveObjects = NULL, Player * me = NULL);
string modalModesTag(const std::vector<std::string>& live, const std::vector<std::string>& halfDead, const std::vector<std::string>& dead, const std::vector<std::string> * liveObjects = NULL);
string modalResolutionOrderNote(const string& subject);
int modalSpecObjectCount(GameObserver * observer, MTGCardInstance * card, const string& spec);
MTGCardInstance * modalSubjectFromScript(Player * me, const string& script);
string modeEdictFloorTag(int theirCreatures, const string& onlyName, int minToughness, int atFloor);
string modeLivenessRowTag(int verdict);
string modeRowAnnotations(const string& script, const string& optionLabel, int myLife, int oppLife, int candidatesCanBlockTapped, int candidatesDoNotUntap, int theirDrawPunisherPerDraw = 0);
void modeTapCounts(Player * opp, int& canBlockTapped, int& doNotUntap);
string mulliganNarration(bool kept, int handSize);
bool nameEchoesRow(const string& nameIn, const vector<string>& rows);
int nameOrdinal(const string& seg);
string namedCardRemainingTag(int theirNotPublic, int myNotPublic);
string namedCardVisibilityTag(int theirBattlefield, int theirGraveyard, int myBattlefield, int myGraveyard);
int namedCastLifeSurcharge(Player * me, MTGCardInstance * card);
bool namedCastPenaltyScan(const string& magicText, int& lifeLoss, int& draws);
string namedCastPriceTag(const string& sourceName, int lifeLoss, int draws, int life);
bool naturalTextLess(const string& a, const string& b);
string noPotentialBlockersTag();
long offProtocolBytes(const string& replyIn, bool * actionBeforePlanOut = NULL, std::vector<string> * offLinesOut = NULL);
void oneShotDrawGrantScan(MTGCardInstance * card, std::vector<OneShotDrawBranch>& out);
string oneShotDrawGrantTag(const std::vector<OneShotDrawBranch>& branches, const std::vector<std::string>& converters, int minePerDraw, int theirsPerDraw, const string& theirPunishers, int life = -1);
int opponentExtraDrawPerTurn(const string& script, bool& variable);
string optionCardText(MTGCardInstance * c, size_t maxLen, const string& focusPrefix = "");
string optionCardTextCore(const string& raw, size_t maxLen, const string& focusPrefix = "");
string optionLabel(const string& row);
string optionRangeNote(unsigned forms);
string optionSetKeyOf(const std::vector<string>& rows);
string ownClockTagFor(Player * seat, Player * opp, int myLife = -1, int incomingDamage = 0, int stackDamage = 0, bool oppLoopLive = false, int crackBackDamage = 0);
int ownExtraDrawPerTurn(const string& script, bool& variable);
string ownTargetsOnlyClause(const string& names);
int parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out, const vector<string> * optionNames = NULL, bool echoBinds = false, int * repeatedOut = NULL);
void parseDungeonRooms(const string& t, std::vector<std::pair<string, string> >& rooms);
int parseRepeatCount(const string& replyIn);
string passRowCleanupPriceTag(bool cleanupThisTurn, int handNow, int limit, int perDiscard, const string& punishers, int life, int stackedDraws);
string paymentLifeCostClause(const std::vector<std::string>& names, const std::vector<int>& damage, int life);
string paymentNoLifeCostClause(bool boardHasHarmSource, int totalDamage, int used);
int paymentTapRestrictionOf(MTGCardInstance * ps, bool beforeAttack, bool blockStillMatters);
string paymentTapsClause(const std::vector<std::string>& names, const std::vector<int>& restrictions, const string& untapSource = "");
string paymentTapsClause(const std::vector<std::string>& names, bool cannotAttackThisTurn);
int pendingStackLifeLossToSeat(GameObserver * observer, Player * seat, MTGCardInstance * exclude = NULL);
string perTargetLifeCostNote(Targetable * t);
bool permanentCanAnimate(MTGCardInstance * c);
string pileAwareCardText(MTGCardInstance * c, size_t maxLen);
size_t planBlockEndOffset(const string& text, size_t planPos);
string planLineOnly(const string& replyIn);
string planMenuDiffClause(const string& absentName);
bool planNamesStrandedCard(const string& plan, const string& cardName);
string playerBranchLabel(const string& raw);
bool playerHasLifeLoop(Player * p);
string potentialBlockersTag(const vector<string>& entries, const string& biggest, const string& extraNote = string(), const string& gangNote = string(), const string& priciest = string(), const string& sumNote = string());
string printedFirstClause(const string& cardText);
string prunedPairsReaskClause(const string& prunedPairs);
bool ptDropKillsTarget(int n, int remaining);
string ptDropTargetVerdict(int n, int toughness, int remaining);
bool ptPumpModifierDelta(MTGAbility * a, int& dp, int& dt, bool& ueot, MTGCardInstance *& modTarget);
const char * punisherVerb(const string& punishers);
string removalVictimTag(const string& verb, const std::vector<std::string>& victims, const std::vector<std::string>& immune, const std::vector<std::string>& mine = std::vector<std::string>());
string renderAbilityLabel(const string& raw, bool * mapped = NULL);
string renderMayBlockList(const vector<int>& labels, const vector<string>& parens, bool * rangeUsed);
int repeatAnnotationCount(const string& row);
bool repeatAskAnswerStands(const string& heldKey, const string& nowKey, const string& heldPlan, const string& nowPlan, int heldTurn, int nowTurn, int heldChoice, int nOptions);
bool repeatPlanStopAndCurrent(const string& plan, int * stopOut, int * currentOut);
bool repeatPlanStopIsOwn(const string& plan);
bool repeatRowEligible(ActivatedAbility * aa);
string repeatRowFoldKey(const string& row);
string repeatRowLine(const string& shortName, int rowIndex, int creatureCount = -1);
string repeatRowShortName(const string& row);
string repeatRowStopClause(int creatureCount, int statedStop, int oppLifeAtStop = -1, int oppLifeNow = -1);
string repeatRunRangeLine(int fromIdx, int toIdx, int firstIdx, int baseFrom, int baseTo);
string repeatShortName(const string& line);
bool replyHasPlanLine(const string& replyIn);
string reserveDeclineCarryNote(bool declinedThisTurn);
string reserveStrandedCardName(const string& chosenRow);
string resolveOwningCardName(const MTGCardInstance * c, int depth = 0);
bool revealRefusalMustPick(bool pickExactlyOne, bool singlePickDeclineLegal);
string revealSummaryNarration(size_t revealedCount, const string& chosen, const string& optionLabel);
long revealWaitUnexplainedSecs(long waitSecs, long latencyMs);
bool riderHasLegalTarget(MTGCardInstance * card, const string& spec);
string rowVerdictClause(const string& row);
bool sameCardId(MTGCardInstance * live, int armedId);
int scanCodedAnswerLines(const string& content, size_t * firstLineEnd, string * firstPayload, string * lastPayload, size_t * firstLabelStart = NULL);
int scriptAbilityDrawCount(const string& script, const string& abilityName);
string scriptAllZones(MTGCardInstance * c);
string scriptLower(const string & s);
int scriptSelfDrawCount(const string& script);
string secondCopyTag(const string& name, const string& magicText = string());
string sentenceNaming(const string& text, const string& needle, size_t maxLen);
string sorceryReserveClause(int left, const string& name, const string& cost, int need, int landDropSources = 0);
bool sourceDealsPoisonInsteadOfDamage(MTGCardInstance * c);
int spellPTDropAmount(GameObserver * observer, MTGCardInstance * src);
string spellRemovalVerb(MTGCardInstance * src);
int spellSingleDamageAmount(MTGCardInstance * src, bool& deathtouch);
string stackHandReplacerFor(GameObserver * observer, Player * seat);
int stackLifeLossBefore(GameObserver * observer, Player * seat, MTGCardInstance * below);
int stackPendingDrawsFor(GameObserver * observer, Player * seat, MTGCardInstance * exclude, int * mayOut = NULL);
string stackTargetTextCore(const string& snippet);
string stackTargetTextNote(MTGCardInstance * c);
string stackTrapEffectGerund(const string& menuLabel);
string stackTrapNoteText(const string& gerund, bool declineOffered);
int statedCrackBackLossToSeat(Player * me, Player * opp, GameObserver * obs);
string strandsHandCardTag(int used, int untapped, const string& name, const string& cost, int need);
string stripAnswerLabelPrefix(const string& seg);
string stripNarrationDecoration(const string& in);
string stripRenderAnnotationsLc(const string& s);
bool tailOpensWith(const string& tail, const char * const * table, size_t count);
string targetChoiceNarration(const string& target, const string& source, const string& ability);
string targetCommitClause(int legalTargets);
string targetPreviewFacts(MTGCardInstance * c);
string theirConverterBodyTag(int toughness, const std::vector<std::string>& theirConverters, int myLife, bool theirLoopClosed = false);
void theirConverterScan(Player * opp, std::vector<std::string>& names);
string toLowerCopy(const string & s);
string transformDfcToggleNote(const string& curFace, const string& otherName, const string& otherCost);
string transportRecordField(const std::vector<std::string>& failures, const std::string& roundTripStamp, long latencyMs);
string transportStampPhase(const string& transport);
string tutorFindsTag(const string& type, const vector<string>& fresh, const vector<string>& copies);
string tutorSearchType(const string& magicText);
string typeTag(MTGCardInstance * card);
int uniqueNameMatch(const vector<string>& words, const vector<string>& names, size_t limit, const vector<int> * allowed = NULL, int ordinal = 0);
void unpermuteSelection(const vector<size_t>& order, size_t n, vector<bool>& send);
string unreachableAttackerTag(int canBlockCount, int totalBlockers, bool flyingExplains);
AADamager * unwrapDamagerAbility(MTGAbility * a, int depth);
string upkeepAnimationClause();
string ventureSourceDungeonTag(const string& sourceName, const string& sentence);
string ventureSourceSilentTag(const string& sourceName);
string verdictScopeOperative(const string& scope);
bool w72CastAnsweredFactApplies(bool stampMatchesThisWindow, int castDecisionsOpen);
bool w72LandDropIsManaSource(bool basicManaSubtype, const string& script);
int w72NameMenuEngineIndex(int pick, bool pickIsDisplayIndex, const std::vector<size_t>& nameOrder);
bool w72RowIsDeclineOrHold(const string& row);
bool w72StopReachedWindowCollapses(bool everyBaseRowIsStopReached, bool anyStopReachedRow, int carriedStop, int planTurn, int nowTurn);
bool w73RowTouchesLife(const string& row);
string w74MdfcLandBackFace(Player * p, MTGCardInstance * card, string& backMana);
bool w74RetryArmedHold(const string& armedPrompt, string& seen, long& sinceMs, long nowMs, long holdMs);
bool w74RowIsDecisionEmpty(const string& row);
string w74XAbandonTraceLine(const string& cardName, int floating);
void w75ApplyChainFeed(std::vector<string>& rows, bool oppLoopLive);
bool w75LegendTwinControlled(const std::vector<W75LegendBoardCard>& board, const std::string& castName);
string w75MdfcFaceRowTag(bool isBack, const string& frontName, const string& backName, const string& backMana);
string w75PlanNamesUncastableZoneCard(const string& plan, MTGPlayerCards * game);
const char * w75ProtocolDeviationClass(bool planLineMissing, int offProtocolBytes, bool forcedCloseEcho = false, bool answerLabelAbsent = false);
int w76BlockableCoveredPower(const std::vector<int>& powers, const std::vector<int>& blockersNeeded, int blockerCount);
bool w76BodyReturnsInsteadOfDying(MTGCardInstance * c);
int w76ClosingRowInsertAt(int declineRowIdx, size_t menuSize);
void w76HoldWindowAsked(W76HoldMemory& m, const char * seam, int windowSeq);
void w76HoldWindowNotAsked(W76HoldMemory& m, const char * seam);
bool w76OffProtocolIsPrefillEcho(const std::vector<string>& offLines, const string& prefill);
bool w76PlayerCanGainLife(Player * gainer);
void w77ApplyOwnLoopFeed(std::vector<string>& rows, bool ownLoopResolving);
void w77ApplyOwnLoopThreatFeed(std::vector<string>& rows, const string& tag);
int w77CoverBodyContribution(MTGCardInstance * c, int destroyKind);
string w77CoverClauseFor(bool sweep, int total, int myLife, bool totalIsFloor, int removedPower, int removedBodies, const std::vector<W77RemovalVictim>& victims, int attackerBodies);
void w77DropUnaskedCastNote(string& promptNote, string& castNote, int& refSeq);
string w77EdictPlayerTargetTagFor(Player * me, Player * tgt, MTGCardInstance * src);
string w77KeyTailOf(const string& tail);
string w77OwnLoopThreatTag(const string& theirSpell, const string& component);
string w77OwnLoopVerdictKey(int state, const string& theirSpell, const string& component);
string w77StayHomeCoverTag(int total, int myLife, bool totalIsFloor, int bodies, int covered, bool anyTrampler = false);
int w77StayHomeCoveredPower(const std::vector<CrackBackAttackerFact>& atk, int bodyCount);
string w77TargetedRemovalCoverTag(int total, int myLife, bool totalIsFloor, const std::vector<W77RemovalVictim>& victims, int attackerBodies);
bool w78AnswerLabelAbsentShape(const std::vector<string>& offLines, bool hadLabelledAnswer);
bool w78AnyRepeatRow(const vector<string>& rows);
bool w78ConditionalCounterScript(const string& magicText);
string w78CounterEscapeCost(const string& magicText);
std::vector<string> w78DeferOptionTexts(const std::vector<string>& rawRows);
bool w78DeferRecordMoves(bool haveMenuController, bool controllerIsSelf, bool controllerIsGptSeat, bool controllerHasTransLog);
string w78HoldRowShortName(const std::vector<string>& rows);
void w78HoldWindowRecordSeq(W76HoldMemory& m, const char * seam, int windowSeq, int recordSeq);
bool w78KeepCandidateStands(int payableSources, int untappedSources);
string w78KeepCostText(const string& printedCost, bool hasAlternative);
int w78PayableSourceCount(Player * p, MTGCardInstance * hc);
bool w78RePutIdentityStands(const string& recordedAtDecline, const string& nowIdentity);
string w78RedundantCounterTag(const string& myCounterName, bool conditional = false, const string& escapeCost = string());
bool w78RowsUnchangedSinceLastAsk(const string& holdCheckNote);
string w78ShortNameParenthetical(bool hasRepeatRow);
string w78StackDrainNote(int theirTriggers, bool rowsUnchangedSinceLastAsk, const string& holdRowShortName);
bool w78StagedNoteIsEmpty(W76HoldMemory& m, const char * seam);
void w79ApplyLoopFaceAtSend(bool sent, const string& pendingFace, string& faceOut, bool& countIt);
bool w79CounterspellScript(const string& magicText, bool castableAtInstantSpeed = true);
bool w79ForcePassNoProgress(bool keyUnchanged, int lastChoice, bool boardMoved);
bool w79LabelAbsentRead(const char * devClass, bool handedToHeuristic);
bool w79LabellessAnswerLine(const std::string& text, size_t planLineStart, bool hadCodedAnswerLine, size_t * segStart, size_t * segEnd);
string w79OwnLoopVerdictLineFor(int state, bool loopProven, const string& theirSpell, const string& component, const string& starterName);
string w79RePutCollapseIdentity(const std::vector<string>& actingRows, int carriedStop);
bool w79RecoveryLabelAbsent(const char * devClass);
bool w79StopReachedRePutCollapses(bool everyBaseRowStopPriced, bool anyStopReachedRow, int carriedStop, int declinedN, bool identityStands);
string w79UntapEngineSources(Player * me, Player * opp);
bool w80CastSelfLeavesOnResolution(MTGCardInstance * card);
bool w80CompletingCastMenu(const string& tail, bool inAnnounceXAsk);
string w80CompletingDeclineSpentClause(int sources, const string& card);
string w80CrackBackFaceOfLine(const string& line);
string w80LoopFaceOfLine(const string& line);
string w80OwnStackSpellTag(bool mine, const string& zoneName);
string w80SelfLeavesNoCoverClause(const string& cardName, int crackTotal);
int w80TwinLoyalty(MTGCardInstance * twin);
bool w80VariableDrawIsHandSize(const string& script, const char * zone);
bool w81AskCacheUsable(bool stateBasedActionWindow);
bool w81AttackCoverDue(bool crackBackLinePrinted, bool anyAttackerOffered);
bool w81CastAnsweredStampMatches(int stampTurn, int stampPhase, const string& stampStack, int nowTurn, int nowPhase, const string& nowStack);
int w81CompulsoryDrawStepLoss(Player * me, Player * opp, int * cardsOut, int * optionalCardsOut, std::vector<std::string> * theirsOut);
string w81CrackBackTotalFace(const string& line);
void w81FoldDuplicateCoverParagraphs(std::vector<std::string>& rows);
int w81SendDisposition(bool transportHandedOff, bool pollReturnedPending);
bool w81XSweepMarkerNamesOwnLosses(const string& row);
string w82WindowKey(const string& boardKey, const string& question);
string wallMissClassFor(const string& phase);
string xAnnounceLibraryNote(int capX, int drawPerX, int library, int reserve, const string& reserveWhy, int libX);
bool xAnnounceRowKills(MTGCardInstance * card, Player * me, int capX, std::vector<string>& out, int * markX = NULL, string * markerText = NULL);
string xCastRemainderScopeTag(int maxX, int baseCMC, bool colouredX, const string& keepName = string(), const string& keepCost = string(), int keepNeed = -1, int totalMana = -1, bool keepIsAnotherCopy = false, const string& instName = string(), const string& instCost = string(), int instNeed = -1, bool instIsCounter = false);
string xDeclineForfeitClause(int floating);
string xKillRowCore(const std::vector<XDamVictim>& victims, int atX, bool sweep, bool hitsMe, bool hitsOpp, int myLife, int oppLife);
int xLibraryCeilingX(int capX, int drawPerX, int library, int reserve);
void xLibraryReserve(Player * me, MTGCardInstance * exclude, int& library, int& reserve, string& why, int * stackDrawsOut = NULL);
bool xLifeDrawClauses(const string& magicText, int& lifePerX, int& drawPerX);
string xSpellPricing(MTGCardInstance * card, Player * me, CastRowBoardAnswer * ans = NULL);
bool xSurveyBoard(MTGCardInstance * card, Player * me, XVictimSurvey & sv);
string zeroPowerAttackerTag(int power);
string zeroPowerBlockerTag(int minP, int maxP, bool anyTrample, bool anyMenace = false);
string zoneDesc(MTGGameZone * z);
CombatTradeStat combatStatOf(MTGCardInstance * c);
bool playerHasLifeToDamageConverter(Player * p);


//#W61-U (C10): what a cast row does to the creature board, in the two numbers
//the menu pass ranks on. Filled ONLY by the emitters that actually price a
//board sweep on this row, from the same walk that renders the clause, so the
//marker and the clause the pilot reads underneath it cannot disagree.
//theirs < 0 = this row prices no board sweep and takes no part in the ranking.
struct CastRowBoardAnswer
{
    int theirs;
    int mine;
    string engines; //#W66-AQ (H10): the ones of THEIRS that are not bodies
    //#W80-DF (U7): the sweep's OWN-SIDE roster - every creature of the seat's
    //that this row destroys, by name, and the engine kind of any of them that
    //is not merely a body. Same walk, same predicate as `engines`.
    string mineNames;
    string mineEngines;
    //#W68-BD (J8): 1 = this row is a plain edict whose victim the OPPONENT
    //picks. It takes no part in the sweep ranking; it is read by the
    //crack-back cover block only.
    int edictKind;
    //#W76-CO (Q5): the crack-back total this row REMOVES, and from how many
    //bodies - summed with crackBackBodyContribution, the same walk the
    //CRACK-BACK NEXT TURN line itself is built from, over the victims this row
    //already names. 0 = this row removes nothing that was going to attack.
    int crackRemoved;
    int crackRemovedBodies;
    CastRowBoardAnswer() : theirs(-1), mine(-1), edictKind(0),
                           crackRemoved(0), crackRemovedBodies(0) {}
};


//State shared between the game thread and the HTTP worker. The worker owns
//a shared_ptr copy: if the game (and this player) is destroyed while a
//request is still running, the worker finishes writing into memory that
//only it still references, then frees it.
struct AIPlayerGPT::AsyncState
{
    //GptMutex, not std::mutex: on Vita std::mutex operations are NO-OPS
    //(inactive gthreads layer), which was harmless while no worker could
    //start but is a real race now that gptSpawnWorker supplies one.
    GptMutex mtx;
    int status;      //0 idle, 1 in flight, 2 done (answer not yet consumed)
    string prompt;   //the userMsg the in-flight/done request was built for
    //#W56-A (D18): the SLOT this request belongs to - (seam, turn, phase,
    //boardKey), where the seam is the question-and-options tail the caller
    //assembled. The prompt string carries a GAME LOG header and a plan block
    //that grow under the same question, so `prompt != userMsg` was answering
    //two different facts at once: "a different question" and "the same question
    //re-rendered". The slot separates them.
    string slotKey;
    string response; //raw HTTP body once status == 2
    std::chrono::steady_clock::time_point started; //request launch time
    //#W53-Q (D10): this round trip came back EMPTY at the wall. The transports
    //(curl / the Android JNI bridge / the Codex SSE reader) all report a
    //failure the same way - an empty body - so the deadline is told from a
    //refused connection by the only fact that separates them and that every
    //platform has: how long it took. A timeout consumes the whole window; a
    //refusal returns in milliseconds. Written by the worker under the mutex
    //alongside the body, read once on consume.
    bool timedOut;
    //audit-L (A24): the HTTP status of the round trip (0 = no status: a
    //transport-level failure, a refused connection, or the Codex path). A
    //4xx/5xx used to come back as an empty body indistinguishable from an
    //unreachable server; the code is what tells a wrong key from a dead host.
    long httpStatus;
    //#W59-H (K1): libcurl result for the same attempt. -1 means this platform
    //used another transport; 0 is CURLE_OK.
    long curlResult;
    //#W57-U: the request GENERATION. A request the GAME THREAD abandons (in
    //flight past its deadline plus a grace - see reapWedgedRequests) bumps
    //this, and the worker publishes only under the generation it launched in.
    //A late answer from a round trip nobody is waiting for is therefore
    //DISCARDED at the worker, never consumed against a board that has moved
    //on. The worker holds its own shared_ptr to this state, so that late write
    //is always into live memory: abandoning frees nothing, which is the whole
    //requirement on Vita, where the worker is a native sceKernel thread the
    //game cannot join or cancel.
    unsigned int gen;
    AsyncState() : status(0), timedOut(false), httpStatus(0), curlResult(-1), gen(0) {}
};


//Heap-allocated capture set for the HTTP worker. The worker owns and frees
//it; the shared_ptr inside keeps AsyncState alive if the player is destroyed
//mid-request.
struct AIPlayerGPT::WorkerCtx
{
    std::shared_ptr<AsyncState> state;
    string url;
    string requestBody;
    string key;
    long timeoutMs;
    bool codex; //ChatGPT-subscription preset: /responses + SSE, not chat completions
    unsigned int gen; //#W57-U: the AsyncState generation this round trip launched under
    WorkerCtx() : timeoutMs(0), codex(false), gen(0) {}
};

    //Willingness policy for the oracle: same as the inherited heuristic's
    //(canHandleCost may pre-choose extra-cost payments).
    class GptManaPolicy : public ManaEngine::ManaPolicy
    {
    public:
        GptManaPolicy(AIPlayerBaka * _ai) : ai(_ai) {}
        int canHandle(MTGAbility * producer) { return ai->canHandleCost(producer); }
    private:
        AIPlayerBaka * ai;
    };


//#W62-W (D1): the parsed enters-tapped gate. `kind` says whether this code can
//decide the gate from the pilot's own battlefield; the two evaluable shapes are
//a basic-land-type list and an other-land count.
enum
{
    kLandGateOpaque = 0,   //conditional, but not decidable from the battlefield
    kLandGateSubtype,      //taps unless one of `subtypes` is on my battlefield
    kLandGateLandCount     //taps when my other-land count passes a threshold
};


struct LandTapGate
{
    bool found;        //the card has an own-ETB tap(noevent)
    bool conditional;  //that tap is gated
    int  kind;
    vector<string> subtypes; //lowercase, e.g. "plains","island"
    int  cmp;          //-1: taps when otherLands < threshold; +1: taps when >
    int  threshold;
    LandTapGate() : found(false), conditional(false), kind(kLandGateOpaque),
                    cmp(0), threshold(0) {}
};


//#W54-M (A22): the per-creature combat facts (CombatTradeStat incl. the
//blocking-trigger life walk) and the per-player converter scan do not depend
//on the PAIRING, yet the attackers/blockers windows recomputed them per pair
//(A x B x battlefield walks + script re-parses; 22-row blocker menus, 1,539-
//creature boards in the corpus). One memo per window, filled on first use,
//so every pairing reads the identical struct the pair-wise call produced.
struct CombatWindowCache
{
    std::map<MTGCardInstance *, CombatTradeStat> stats;
    std::map<Player *, bool> converter;
    CombatTradeStat statOf(MTGCardInstance * c)
    {
        if (auditMOff())
            return combatStatOf(c);
        std::map<MTGCardInstance *, CombatTradeStat>::iterator it = stats.find(c);
        if (it == stats.end())
            it = stats.insert(std::make_pair(c, combatStatOf(c))).first;
        return it->second;
    }
    bool converterOf(Player * p)
    {
        if (auditMOff())
            return playerHasLifeToDamageConverter(p);
        std::map<Player *, bool>::iterator it = converter.find(p);
        if (it == converter.end())
            it = converter.insert(std::make_pair(p, playerHasLifeToDamageConverter(p))).first;
        return it->second;
    }
};


#endif //AIPLAYERGPT_INTERNAL_H
