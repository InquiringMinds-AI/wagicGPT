# split-parsetest — the PARSETEST corpus out of AIPlayerGPT.cpp

Mechanical lane, 2026-09-15. ONE job: move the `WAGIC_GPT_PARSETEST` self-test corpus out
of `projects/mtg/src/AIPlayerGPT.cpp` into its own translation unit, with zero behaviour
change. No other refactoring: no dead render removed, no comment touched, no production
logic moved, every `#if`/`#ifdef` gate left exactly as it was.

Driven by `strategy-design/audit-2026-09/gpt-layer-census.md`, which measured the corpus at
43,066 lines — 40.6% of the file — bracketed from L62,992 to EOF.

## Line counts

| file | before | after |
|---|---:|---:|
| `src/AIPlayerGPT.cpp` | 106,056 | **63,671** |
| `src/AIPlayerGPTSelfTest.cpp` (new) | — | **43,140** |
| `include/AIPlayerGPTSelfTestAccess.h` (new) | — | 1,147 |

AIPlayerGPT.cpp loses 42,385 lines (-40.0%). The moved region is the old L62,970-106,033:
the `answerSegmentStatic` PARSETEST helper, `AIPlayerGPT::runParseSelfTest` with its
`=== self-test` printer, the `CHECK` macro, every section and fixture, and the
`#if defined(WAGIC_GPT_PARSETEST_BUILD)` / `#else` stub pair that gates them. 892 lines come
back into AIPlayerGPT.cpp as the test-access seam (below), so the net change is -42,385.

Side effect worth noting: `make -B -f Makefile.sdl -j8` got faster — the two halves now
compile in parallel instead of one 106k-line TU serialising the build tail.

## The corpus body is byte-for-byte the same code

`AIPlayerGPTSelfTest.cpp` carries the corpus verbatim. Exactly three edits exist in the
moved text, all mechanical:

1. the body now sits in `AIPlayerGPTSelfTestAccess::run()` instead of
   `AIPlayerGPT::runParseSelfTest()`, which calls it;
2. `answerSegmentStatic` became a static member of the same struct (it was a PARSETEST-only
   file-static, and it calls the internal `gptAnswerSegment`);
3. ONE call site, `::w77DropUnaskedCastNote(promptNote, castNote, refSeq)`, became
   `AIPlayerGPTSelfTestAccess::w77DropUnaskedCastNote(...)`. Its explicit `::` existed to
   pick the free helper over the same-named `AIPlayerGPT` member; across a TU boundary the
   free helper is only reachable through the seam, so the qualification moved with it.

Nothing else in 43,065 lines changed.

## The test-access seam

The corpus used to reach AIPlayerGPT.cpp's internals by being in the same TU — the old
comment said so outright: "Same-TU access to the file-static parsers is the point." It now
reaches them through ONE struct, `AIPlayerGPTSelfTestAccess`, declared in
`include/AIPlayerGPTSelfTestAccess.h`:

* it is a **`friend` of `AIPlayerGPT`** (one line added to the class, replacing nothing), so
  the corpus keeps its access to AIPlayerGPT's private statics and **no member was made
  public**;
* it **derives from `AIPlayerGPT`**, purely so that the corpus's unqualified calls to those
  private statics still resolve by ordinary class-scope lookup. It is never instantiated;
* its static members are **one-line forwarders** onto AIPlayerGPT.cpp's anonymous-namespace
  and file-static helpers and constants. The forwarder *definitions* live in AIPlayerGPT.cpp
  (they must, to see those helpers) inside the same `#if
  defined(WAGIC_GPT_PARSETEST_BUILD)` guard the corpus is under. **Nothing in
  AIPlayerGPT.cpp changed linkage**: the anonymous namespaces are still anonymous, every
  `static` is still `static`.

Types cannot be forwarded through a thunk, so 18 type/typedef declarations **moved** from
AIPlayerGPT.cpp into that header, verbatim and unchanged, plus one prototype for a function
that lives in `AllAbilities.cpp`. That is the only production text that moved.

### Exposed symbols — types moved into the header (19)

    struct WipeVictim, GangBlockerStat, XDamVictim, XVictimSurvey, OneShotDrawBranch,
           W75LegendBoardCard, CrackBackAttackerFact, W77RemovalVictim, W79BlkFact,
           W79AtkFact, GptModalMode, GptTokenPlan, CombatTradeStat
    enum   PaymentTapRestriction, HandCastVerdict, MdfcLandArrival,
           { kConverterClassDraw, kConverterClassDiscard } (anonymous),
           { kW76CloseFallThrough, kW76CloseArm, kW76CloseDefer } (anonymous)
    typedef HoldRowKeyFn
    prototype  bool revealChooserCanDecline(bool, int);   (definition untouched, AllAbilities.cpp)

### Exposed symbols — AIPlayerGPT private members

None individually. The single `friend struct AIPlayerGPTSelfTestAccess;` declaration covers
all of them; no member's access specifier changed. The corpus reaches 34 of them
(`parseChoice`, `isDecodeGarbage`, `rowSaysNoOp`, `choiceLineIsClean`, `transportPhaseFor`,
`deadlineTenthsPct`, `latchedRowMismatch`, …) exactly as it did before.

### Exposed symbols — file-scope constants forwarded (81)

    kReplyProtocol                  kAttackersTurnFacts             kBlockersTurnFacts
    kBlockTradeTrustNote            kSecondMainAheadFact            kMainPhasesAheadFact
    kPriorityAgainFact              kCastAnsweredFact               kNoPassRowFact
    kDeclineRowFact                 kRevealWindowScopeFact          kPlanTargetAbsentNote
    kStalePlanNote                  kPlanCarryMaxChars              kPlanCarryMaxSteps
    kEchoHeadWord                   kBoardEffectClauseFactor        kBoardEffectSnippetSlack
    kBoardCreatureEffectLen         kBoardCreatureEffectNameCap     kForeignEffectVoiceFrame
    kGhostformNotRemovalTail        kPotentialBlockersEnumerateMax  kPlanFirstLead
    kAnswerLockPrefix               kLandDropConsequence            kLandDropDeclineRow
    kBlockerRangeNote               kVictimRosterGroupCap           kOptionRangeNote
    kSharedCardTextMinLen           kEventRunCollapseFloor          kXNetNotSupplied
    kXMostKillsMarker               kXBestTradeMarker               kXNoKillsMarker
    kAnswerReserveTokens            kDefaultReplyCeilingTokens      kAnswerLockedRetryTokens
    kDefaultReasoningBudget         kW76CloseDeferMaxTicks          kLogWindowTrimMarkerHead
    kW81PromptByteTarget            kW81NarrationFloor              kAnnounceXPlanScopeNote
    kNarrationTrimTrigger           kNarrationTrimKeepBytes         kNoAttackThisCombatLine
    kHandCastableNowTagText         kHandCastAnsweredTagText        kRepeatActivationFloor
    kPassPriorityRowText            kHoldPriorityRowHead            kHoldPriorityRowText
    kHoldPriorityRowTextActivation  kHoldPriorityRowTextCast        kCastNoRowZeroFact
    kChainFeedTail                  kW80StarterNone                 kW80StarterActivated
    kW80StarterEntersTrigger        kW80StarterCycleTrigger         kW81StarterOpponentTrigger
    kW81StarterAttackTrigger        kRepeatRowFloor                 kRepeatRowMax
    kSelfTargetClause               kSelfOnlyWindowNote             kModeNoLifeNoDrawTag
    kBlockPairAnswerBytes           kAttackTargetScopeFacts         kLifeLoopAttackerRowTag
    kPreventNone                    kPreventFull                    kPreventPartial
    kRevealPlanScopeFact            kCardTextOpen                   kCardTextClose
    kSharedCardTextHeadLead         kCompletingCastNote             kLatchedLineKeep

### Exposed symbols — internal helpers forwarded (784 names, 788 overloads)

    abilityActivationNarration         actionTakenNarration               activationActorIsMine
    activationChairOverLadder          activationManaCost                 activationSourceIsGrantedDummy
    activationVerbPhrase               afterCombatBlockerCostText         allAssignmentsIllegalClass
    allCastRowsDeadNote                altCostPrintedNote                 amassCountersFromScript
    animatableNotCountedTail           animateDeathPriceClause            animateRungCeilingClause
    annihilatorTag                     annotateEtbPayOrTapMenu            announceXHeader
    answerReplaced                     answerTailFromReasoning            appendParseNote
    appendPlainDeclineScope            appendStackDeathToDeclineRows      applyBoardSweepMark
    applyDuplicateEffectTags           applyMenuFitTags                   askExemplar
    askWindowKindForAsk                askWindowKindForPriority           assignableMatchedAttackers
    assignableRemainderDamage          asyncDropOutcome                   asyncDropStamp
    asyncDropTraceLine                 asyncLandArm                       asyncSlotDriftKind
    asyncSlotKeyOf                     attackPunisherClause               attackRestrictionFree
    attackTotalLine                    attackerBlockerCountLine           attackerBlockerEntry
    attackerDestinationTag             attackersHeldSickLine              attemptMsField
    battlefieldHeaderText              becomesBlockedSelfPump             betterBlockerPair
    blockAssignmentClause              blockCountRequirementTag           blockKeepAlternativeClause
    blockPriceSumTag                   blockerLifelinkGain                blockingLifeCeiling
    blockingLifeTriggerClause          blockingTriggerTotalLine           boardCreatureEffectEligible
    boardCreatureEffectResidueTail     boardCreatureEffectSnippetLen      boardCreatureEffectText
    boardEffectSnippet                 boardEffectSnippetCeiling          boardEffectSnippetFocus
    boardEffectSnippetLen              boardEffectTag                     boardEffectTextEligible
    boardEntryIsPermanent              boardSweepMarker                   buildForcedSacrificeAsk
    buildHandGainAsk                   buildHandRemovalAsk                buildMayObjectAsk
    buildRevealAskText                 bulkZoneMoveNarration              cannotPayNowClause
    cardTransformNarration             carriedPlanHeaderText              castAbandonedNarration
    castBodiesNetOfOwnText             castDeclineRow                     castDrawPriceRowTag
    castKillSummaryTag                 castModeAltPriceTag                castModeCastPriceTag
    castModeCommitmentNote             castModeRowTag                     castPlayerDamageTail
    castSetKeyOf                       castTriggerDrawCount               ceasedToExistNarration
    choiceBranchLabel                  chooseANameHeaderText              chosenNameNarration
    chosenNameTag                      ciStartsWith                       clauseLeadTrim
    clauseOccurrences                  cleanupDiscardHeaderText           cleanupDiscardPriceClause
    closedLoopHeaderText               codedAnswerCount                   codedAnswerLineAt
    codedAnswerLineInPlanBlock         codedAnswerLinePlanSpan            collapsedRunNarration
    collectLabeledLines                collectXDamageClauses              combatDamageForecast
    combatLineIsClean                  combatTradePreviewStats            composeRowOrder
    compoundModeTargetNote             converterClassOfScript             converterSummaryText
    copiesLeftBehindTag                copyOfTag                          copyRangeTag
    countLegalAssignments              counterAddPlusFromScript           counterAppliedTag
    counterEventNarration              crackBackAnimatedPower             crackBackAnimatorEntry
    crackBackBestAnimateRung           crackBackBlockerRowTag             crackBackBodyUntaps
    crackBackColorsFit                 crackBackCostAffordable            crackBackDamageIsDirect
    crackBackExileReturnClause         crackBackFlatDamageAmount          crackBackKillRowTag
    crackBackNextTurnDue               crackBackNextTurnLine              crackBackReliefClause
    crackBackRemovalRowTag             crackBackVerdictKey                damageKillsTarget
    damageNarration                    damagePlaneswalkerVerdict          damagePlayerVerdict
    damageTargetVerdict                dayNightChangeNarration            dayNightStateLine
    declineBoardScope                  declineFactForMenu                 declineRowIndexOf
    declineRowReaskTag                 declineRowText                     declinedListNote
    dfcOtherFaceName                   discardAlreadyControlClause        discardBoardVerdictTag
    discardDeadTargetClause            discardDisposabilityClass          discardDisposabilityOrder
    discardNotSpareTag                 discardPunisherClause              discardPunisherSummaryText
    discardSpareDefaultLine            discardSpareLandClause             discardVerdictLegend
    drawFeedsStepForecast              drawForecastBlock                  drawPriceRowTag
    drawPunisherClause                 drawPunisherSummaryText            drawStepForecastText
    drawsResolvedClause                drawsStillAhead                    drawsUnattributedClause
    dualRoleHandNote                   dungeonRoomBranchHeader            dungeonRoomPositionTag
    dungeonRowTag                      dungeonVenturesToCompletion        dungeonsCompletedLine
    duplicateEffectTag                 duplicateVerdictTag                edictAlreadyOnStackClause
    edictClause                        edictOnlyVictimOnStackClause       edictSelfClause
    engineKindForScript                equipHolderNote                    equipmentCastPriceTag
    etbUsableClause                    everyCastRowDead                   exampleCoreTrim
    exemplarSentence                   exileHostageRowTag                 feedsRowTag
    fetchLandColorsClause              fetchLineKey                       fetchMakesNoManaClause
    findAnswerLabelLine                findPlanMarker                     firstLabelledLine
    firstLineLeadingPlanPos            firstLoyaltyClausePrefix           foldDuplicateTail
    foldManaBillClauses                forcedSacrificeRowTag              forcedSacrificeShape
    fromExileClause                    gangBlockPriceTag                  gangKillNeed
    gangKillsAttacker                  gateSideBranches                   gateSideCensusTag
    gptAnswerSegment                   gptDeadlineMissed                  gptDeclarationAnswerFloorTokens
    gptForceCloseEarned                gptForceClosePrefillBody           gptHoldCoversSiblingWindow
    gptInlineChoiceOnPlanLine          gptLineAtOffset                    gptResolveMaxTokens
    gptSeamMaxTokens                   gptTextHash                        graveyardRecursionScript
    graveyardRefillRowClause           graveyardZoneLine                  groupCombatCandidates
    groupNumberedRows                  handAboutToBeReplacedLine          handCastabilityTag
    handRemovalVerb                    handleRank                         hasCodedAnswerLine
    headParenthetical                  headerCountsAsMoreAbleToAttack     heldBackBlockTag
    heuristicRevealIndex               hiddenOriginMoveNarration          highestMvEdictClause
    holdActionKeyRow                   holdContractParagraph              holdKeyRow
    holdNoteSameWindow                 holdReopenNoteText                 holdRowBenefitClause
    holdRowIndexOf                     holdRowLine                        holdStillStands
    hybridPipNoteText                  identicalInterchangeableRows       improveAssignmentMaterial
    incomingCombatForecastLine         incomingCombatForm                 incomingCombatLine
    incomingCombatSettledLine          insideRecurringTrigger             isAddNCountersOption
    isAnimationRow                     isDayNightMarkerName               isEngineTokenText
    isFetchCrackLine                   isPublicOriginZone                 isRemovalDestination
    isRenderVocabWord                  isReservedHoldEcho                 joinBlockerRows
    joinNumberedRows                   joinTargetEntries                  joinVictimRoster
    joinZoneEntries                    landDropAskText                    landDropStatusLine
    landDropThreatTag                  landEntersTappedTag                landTapMana
    landTapTagFor                      lastOfferClause                    laterStepRouteClause
    leavesFloatingTag                  leavesUntappedTag                  legendRuleHeaderText
    legendRuleTargetClause             legendTwinTag                      legibleCounterName
    legibleCounterNameForGrant         legibleKeywordName                 librarySearchNarration
    lifeAfterPendingStack              lifeChangeNarration                lifeLoopAttackerRowTag
    lifeLoopDrainClause                lifeLoopWinTail                    lifeLoopWinnable
    lifeLossMirrorScript               lifeMenuLabelTag                   lifePaymentVerdict
    lifeToDamageConverterRate          lifeToDamageConverterScript        lifeToDamageConverterTake
    lineIsActivatedCost                lineIsCodedAnswerLine              listCopyRank
    listKeyHash                        logWindowInertRow                  logWindowKindBudgeted
    logWindowLogHeader                 logWindowOpeningDigest             logWindowSettingLabel
    logWindowSplit                     logWindowSummaryLine               loopCautionClause
    loopChainEntrySentence             loopChainingNote                   loopHalfAffordabilityClause
    loopNonChainingClause              loyaltyClausePrefix                loyaltyRowConsequenceTag
    magnitudeExprIsResolutionTimeOnly  manaAvailableLine                  manaColourSetSymbols
    manaColourSetText                  manaCostTextCmc                    manaCostTokenText
    manaOnlyWindowSkippable            manaPoolClause                     manaSourceColoursText
    manlandBoardTag                    markBareBackFaces                  mdfcHandLandFaceTag
    mdfcLandArrivalClass               mdfcLandArrivalTag                 mdfcLandArrivalTagFrom
    mdfcLandFaceRow                    mdfcLandPlayRowTag                 mdfcRowHead
    mdfcSpellLandBackNote              mdfcSpellToggleNote                menaceAttackRestrictionTag
    menaceBlockPairingTag              menaceBlockerBudgetLine            menuConsequenceNarration
    menuFitTag                         menuHasReserveRow                  menuIsBareXAnnounce
    menuRowProseName                   menuRowShortName                   modalActorSwap
    modalChoiceModes                   modalMirrorModeLabel               modalModeVerdict
    modalModesTag                      modalResolutionOrderNote           modeEdictFloorTag
    modeEffectPriceTag                 modeLivenessRowTag                 modeRowAnnotations
    monotoneXRangeRow                  morbidMagnitudeClause              morbidPTBranches
    moverGainDestination               mulliganKeepClause                 mulliganNarration
    mulliganNoCoverCause               mulliganRowLabel                   mutateAltCostLabel
    mutateHostAsk                      mutateOverUnderHeader              mutatePlacementFromLabel
    mutatedPileTag                     mutatedPileTextCore                nameEchoesRow
    nameOrdinal                        namedCardRemainingTag              namedCardVisibilityTag
    namedCastPenaltyScan               namedCastPriceTag                  narrationAppend
    narrationBucketRuns                narrationFoldPaidSources           narrationShapeKey
    narrationTrimKeep                  narrationTrimNear                  narrationTrimTrigger
    narrationTrimV1                    naturalTextLess                    noPotentialBlockersTag
    noopReaskLine                      noopRowEarnsReask                  offProtocolBytes
    oneShotDrawGrantTag                oppLifeRaceClause                  opponentExtraDrawPerTurn
    opponentLifeTrendLine              opponentOpenManaLine               opponentZoneCountsLine
    optionCardTextCore                 optionLabel                        optionRowMentions
    optionSetKeyLine                   optionSetKeyOf                     ownClockTag
    ownExtraDrawPerTurn                ownTargetsOnlyClause               ownerTag
    parseAttackerSet                   parseAttackerTargets               parseBlockAssignments
    parseDungeonRooms                  parseLogWindowSetting              parseRepeatCount
    passRowCleanupPriceTag             payLifeAmountFromOption            payRepeatBandRowTag
    payRepeatCollapse                  payRepeatCollapseTag               payRepeatModeNote
    payRepeatPerCounterCost            payRepeatRowCostTag                payRepeatTapsClause
    paymentLifeCostClause              paymentNoLifeCostClause            paymentReceiptLine
    paymentTapsClause                  pendingLoopWarningText             pendingStackDamageLine
    perColorSourceCountClause          plainDeclineScopeClause            planAgeClauseText
    planAssertedAbsentBlock            planBlockEndOffset                 planContradictedBlock
    planHardAged                       planLibraryOnlyNote                planLineOnly
    planMenuDiffClause                 planNamesStrandedCard              planRetractionServedAlone
    planWithdrawnBlock                 playerBranchLabel                  poisonGainLine
    poisonStatusLine                   postAnswerOverrun                  potentialBlockersTag
    pregameBottomNarration             pregameHandHeaderText              printedClauseFor
    printedFirstClause                 printedPTTag                       protocolLinesOnly
    prunedPairsReaskClause             ptDropKillsTarget                  ptDropTargetVerdict
    reasoningHiddenShape               reasoningNgramRepeatRatio          reasoningRepetitionRatio
    recordChoiceIsRowIndex             refusedChosenText                  removalVictimTag
    renderAbilityLabel                 renderMayBlockList                 repeatAnnotationCount
    repeatAskAnswerStands              repeatAskKey                       repeatMechanismClause
    repeatPlanScanNumber               repeatPlanStopAndCurrent           repeatPlanStopIsOwn
    repeatRowFoldKey                   repeatRowLine                      repeatRowShortName
    repeatRowStopClause                repeatRunRangeLine                 repeatShortName
    repeatStopClampApplies             repeatStopClampCount               repeatStopClampReceipt
    repeatStopExecutesNothing          repeatedClauseBody                 replyActionBeforePlan
    replyHasPlanLine                   replyTerminatedNaturally           reserveDeclineCarryNote
    reserveDeclineStillStands          reserveStrandedCardName            restoreScriptTokenCase
    revealCostTag                      revealDestinationClause            revealDestinationZone
    revealDuplicateTag                 revealEligMarker                   revealRefusalMustPick
    revealSinglePickDeclineLegal       revealSubjectIsMe                  revealSummaryNarration
    revealWaitUnexplainedSecs          revealedCardNarration              riderTargetSpec
    rowIsDeclineRow                    rowIsRepeatBase                    rowVerdictClause
    salvageLoopedBlocks                salvageLoopedSubset                scriptAbilityCost
    scriptAbilityDrawCount             scriptActivatedAnimateCost         scriptCostGloss
    scriptHasActivatedAnimate          scriptHasWord                      scriptPTDrop
    scriptSelfDrawCount                secondCopyTag                      secondCopyVerdict
    sentenceNaming                     sorceryReserveClause               sourceBlockLine
    spareReachCap                      splitCopyRowHandle                 splitMonotoneXRow
    splitNoOpBandRow                   splitReasoningBlock                splitRowHandle
    splitSourceOrdinal                 stackAbilityLine                   stackAbilityName
    stackAnswerYesRowClause            stackDamagePhrase                  stackDeathRowClause
    stackDeathVerdictKey               stackDrawRecipient                 stackFactsCore
    stackLifePhrase                    stackObjectIsRespondable           stackTargetTextCore
    stackTargetTextNote                stackTrapEffectGerund              stackTrapNoteText
    strandsHandCardTag                 stripAnnotationBrackets            stripDeclineReaskTags
    stripNarrationDecoration           stripRenderAnnotationsLc           stripRepeatAnnotation
    stripTrailingListGloss             stripTrailingListGlossLines        summoningSickRowApplies
    summoningSickTag                   sweeperClause                      sweeperRosterTail
    sweeperSurvivorTail                tapCostBeforeCombatClause          tapOutCrackBackClause
    tapUntapBranchTag                  tappedAnimateNeedsVerdict          tappedCreatureTag
    tappedSourceAnimateClause          tappedSourceAnimateVerdict         targetChoiceNarration
    targetCommitClause                 targetPreviewFacts                 targetZoneTag
    textCarriesSeatVoice               textSnippetCore                    theirConverterBodyTag
    theirDrawStepForecastText          toLowerCopy                        tokenCreatedNarration
    transformDfcToggleNote             transportRecordField               transportStampPhase
    trimMarkerLine                     tutorFindsTag                      tutorSearchType
    uniqueNamedIn                      unpermuteSelection                 unreachableAttackerTag
    upkeepAnimationClause              ventureSourceDungeonTag            ventureSourceSilentTag
    ventureStepLine                    w42Capitalize                      w72CastAnsweredFactApplies
    w72LandDropIsManaSource            w72NameMenuEngineIndex             w72RowIsDeclineOrHold
    w72StopReachedWindowCollapses      w73HoldExpiredByUntap              w73RowTouchesLife
    w73SiblingBoardKey                 w74ClockSourceRecurs               w74HoldUnseenRows
    w74RetryArmedHold                  w74RowIsDecisionEmpty              w74ScriptGrantsHasteToMine
    w74TapOnlyDamagePerActivation      w74XAbandonTraceLine               w74XPermuteToClimbing
    w74XPickToContractIndex            w75ChainFeedRow                    w75LegendTwinControlled
    w75MdfcFaceRowTag                  w75OwnClockSuppressed              w75ProtocolDeviationClass
    w76BlockableCoveredPower           w76CastPermissionKeyword           w76CastableFromDeadZone
    w76ClosingRowInsertAt              w76CrackVictimTrulyRemoved         w76CrossPhaseListKey
    w76CrossPhaseRePutNote             w76ForceCloseArmAllowed            w76ForceCloseDecision
    w76HoldReopenNote                  w76HoldWindowAsked                 w76HoldWindowNotAsked
    w76LifeGainReaches                 w76LoyaltyRowTargetForm            w76OffProtocolIsPrefillEcho
    w76PhaseFreeBoardKey               w76SelfRecursiveZoneScript         w76StatedLethalOnScreen
    w76StripBalancedAnnotationGroups   w76UpToNTargetClause               w77ApplyOwnLoopFeed
    w77ApplyOwnLoopThreatFeed          w77ConsumeWindowSeq                w77ConverterTakeClause
    w77CoverBodyContribution           w77CoverClauseFor                  w77DropUnaskedCastNote
    w77EdictPlayerTargetTag            w77GateMeteredGains                w77KeyTailOf
    w77LoopVerdictFrom                 w77OwnLoopRow                      w77OwnLoopThreatTag
    w77OwnLoopVerdictKey               w77PlanNamesAsCastIntent           w77StayHomeCoverTag
    w77StayHomeCoveredPower            w77TargetedRemovalCoverTag         w78AnswerLabelAbsentShape
    w78AnyRepeatRow                    w78ClauseCutAtOrBefore             w78ConditionalCounterScript
    w78CounterCostBody                 w78CounterEscapeCost               w78DeferOptionTexts
    w78DeferRecordMoves                w78FirstClauseCut                  w78ForceCloseBusy
    w78ForceCloseSameArmBusy           w78HoistSharedCardText             w78HoldLatchKeys
    w78HoldRowDelta                    w78HoldRowKeys                     w78HoldRowShortName
    w78HoldStillStands                 w78HoldWindowRecordSeq             w78IsDrainLinkType
    w78KeepCandidateStands             w78KeepCostText                    w78LoopVerdictFrom
    w78MassRemovalOfTypeScript         w78OwnLoopVerdictLine              w78PayableSourceCount
    w78RePutCollapseIdentity           w78RePutIdentityStands             w78RedundantCounterTag
    w78RepeatSurfacesAgree             w78RowNumberList                   w78RowsUnchangedSinceLastAsk
    w78ShortNameParenthetical          w78StackDrainNote                  w78StagedNoteIsEmpty
    w78StopReachedRePutCollapses       w78StripHandleOrdinal              w78StripSharedCardTextHeader
    w78TrimTail                        w78UnitDepths                      w78UnitSafeCut
    w79ApplyLoopFaceAtSend             w79AskScopeKey                     w79BestBlockDamage
    w79ClauseNeedsAntecedent           w79ClausePrioritySnippet           w79CloseOpenUnits
    w79ContinuationDigestOf            w79CounterspellScript              w79CoveredByBodies
    w79DecisionBearingClause           w79ForcePassNoProgress             w79HoldVerdictForCompare
    w79LabelAbsentRead                 w79LabellessAnswerLine             w79NumberedRowBody
    w79OwnLoopVerdictLineFor           w79PunisherLineAmount              w79RePutCollapseIdentity
    w79RecoveryLabelAbsent             w79ResidualFor                     w79SharedCardTextPays
    w79StopReachedRePutCollapses       w79UntapEngineLineQualifies        w79VerdictDangerRank
    w80AnswerPrecedesPlan              w80CarriedPlanSteps                w80CompletingCastMenu
    w80CompletingDeclineSpentClause    w80CountRenderedAtSend             w80CoverMechanismSentence
    w80CrackBackFaceOfLine             w80CrackBackVerdictLine            w80CrossPhaseReplayable
    w80DrawExtraIsOptional             w80EchoStepIndex                   w80EtbSelfLeavesLine
    w80HoldRowOnMenu                   w80LoopFaceOfLine                  w80LoopStarterScript
    w80LoyaltyClauseLabel              w80NewLethalThreat                 w80OwnStackSpellTag
    w80ProvenWinLoopLine               w80SacrificeSpendsBlockerClause    w80ScriptLineHasActivationCost
    w80SelfLeavesNoCoverClause         w80SingleOutcomeRepeatMenu         w80StackDeathVerdictLine
    w80StackNotEmptyReason             w80StarterIsLive                   w80StarterLineKind
    w80TrimDanglingTail                w80VariableDrawIsHandSize          w81AttackCoverDue
    w81CachedReplayMustReask           w81CounterspellsSeenLine           w81CrackBackTotalFace
    w81CrackBackVerdictLine            w81EngineBodyTail                  w81FoldDuplicateCoverParagraphs
    w81NarrationBudget                 w81StarterEventNeedsOpponent       w81StepNameCandidates
    w81TrimNarrationToBudget           w81XSweepMarkerNamesOwnLosses      walkerOnlyExclusionLine
    wallMissAbandoned                  wallMissClassFor                   winFoldBlockedTail
    wipeSurvivorTail                   xAffordabilityCore                 xAnnounceLibraryNote
    xBestFreeKillX                     xBestTradeX                        xCastIsOnlyXDamage
    xCastRemainderScopeTag             xCastRowBestXMarker                xCastRowMarkerFrom
    xDamageSweepCore                   xDamageTargetedCore                xDeclineForfeitClause
    xDrawPunishClause                  xKillRowAnnotations                xKillRowCore
    xLibraryCeilingClause              xLibraryCeilingX                   xLibraryReserveCount
    xLibraryReserveWhy                 xLibraryRowClause                  xLifeDrawClauses
    xLifeDrawEffectClause              xLifeDrawRowAnnotations            xLifeDrawRowCore
    xMarkerRestate                     xMenuMarkX                         xMonotoneMarker
    xNetLifeForX                       xReserveUpkeepAhead                xScriptDrawsRng
    xTradeMarker                       xVictimList                        yourHandDisplacedClause
    yourLibraryLine                    zeroPowerAttackerTag               zeroPowerBlockerTag
    zoneTagText

## PARSETEST diff statement

`WAGIC_GPT_PARSETEST=1 ./wagic` was captured on the pre-split binary built from this lane's
base commit, and again on the post-split `make -B` binary:

    diff ~/.gatelogs/split-parsetest-before.log ~/.gatelogs/split-parsetest-after.log   -> no output
    cmp  ~/.gatelogs/split-parsetest-before.log ~/.gatelogs/split-parsetest-after.log   -> identical bytes

**The two logs are byte-identical — nothing differs at all.** No timing, address or path
appears anywhere in PARSETEST output, so there is no excepted class to state. Both logs are
9,097 lines and both end `=== self-test: 7275 passed, 0 failed ===`.

## Gate results

| gate | result |
|---|---|
| `make -B -f Makefile.sdl -j8` | clean, 0 errors, links |
| PARSETEST | **7275 passed, 0 failed** — byte-identical to the pre-split log |
| suite, `WAGIC_TESTSUITE_THREADS=1`, detached unit | **1309 tests (0 failed), 80 AI tests (0 failed)** — baseline exactly |
| `check-ctor-init.py src` | OK, 119 file(s) (was 118); the 55 pre-existing scalars unchanged |
| `check-reply-instructions.py src bin/Res/ai/baka` | OK (1 source file, 37 guides) |
| heuristic A/B vs `archives/baka-ab/badbba7d3` | **PASS** — 210 games, **0 unfinished**, no per-deck abs(z) >= 2 (largest +1.68, deck162) |
| U+FFFD in the diff and in both new files | 0 |

`check-reply-instructions.py` still finds what it needs. It selects files by the name
`AIPlayerGPT.cpp` and cuts its scan at the `#if defined(WAGIC_GPT_PARSETEST_BUILD)` line.
That line still exists in AIPlayerGPT.cpp — it now opens the forwarder block at the end of
the file — so the tool scans MORE production text than before, not less, and the corpus it
was always meant to skip is now in a file it never opens.

## Build lists

Every platform that compiles `AIPlayerGPT.cpp` now also compiles `AIPlayerGPTSelfTest.cpp`:

| build | file | change |
|---|---|---|
| desktop / SDL (qmake) | `projects/mtg/wagic.pri` | the source, plus the new header in HEADERS |
| PSP | `projects/mtg/Makefile` | `objs/AIPlayerGPTSelfTest.o` |
| Vita (CMake) | `CMakeLists.txt` | `projects/mtg/src/AIPlayerGPTSelfTest.cpp` |
| Android (NDK) | `projects/mtg/Android/jni/Android.mk` | `$(MTG_PATH)/src/AIPlayerGPTSelfTest.cpp` |
| Windows | `projects/mtg/Makefile.win` | the object in the list, plus its compile rule |

`Makefile.sdl` was regenerated with `qmake6 wagic-SDL.pro CONFIG+=debug -o Makefile.sdl`
(never plain `qmake6` — that overwrites the tracked PSP dispatch `Makefile`).

Only `wagic-SDL.pro` defines `WAGIC_GPT_PARSETEST_BUILD`, so on PSP/Vita/Android the new TU
compiles to nothing but the stub `AIPlayerGPT::runParseSelfTest()` — the same gating, and
the same near-zero cost, those platforms had before.
