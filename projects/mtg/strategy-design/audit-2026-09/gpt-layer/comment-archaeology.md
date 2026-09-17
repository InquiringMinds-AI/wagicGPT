# GPT-layer comment archaeology (#W82-P12)

Incident narrative moved out of the code by the P12 split: wave-by-wave measurements, seq/game citations and corpus counts that a comment block carried AFTER its lead paragraph. The lead paragraph (the WHY, the invariant, the rule) stays beside the code; each moved paragraph is replaced there by a one-line pointer to its anchor here. Nothing was rewritten - every paragraph is verbatim. Paragraphs carrying a rules citation, an owner ruling, an invariant or a doctrine rail were NOT moved.

### AIPlayerGPT-L305-4  — AIPlayerGPT.cpp, near `L305` (note)

```
//
//WAVE-35 PROTOCOL SURGERY (churn drivers #1 and #4 of the wave-34 trace audit).
//Two clauses here were the two largest measured consumers of reasoning tokens
//across ~740 traces, and BOTH were undecidable rather than merely wordy:
//  #1 "ONLY IF your plan has changed" (7-15% of ALL reasoning chars, present in
//     ~100% of traces). It has no answer at the FIRST decision of a game (there
//     is no prior plan to compare against - one trace spent 57% of itself on
//     exactly that), and no answer while EXECUTING a multi-step plan ("does
//     advancing a step count as changing it?"). The rewrite states the three
//     cases as a closed, mechanical set - first decision / something in the plan
//     is now done-or-false / otherwise - so the question is answerable in every
//     state a decision can occur in, and says outright that executing is not
//     changing. It also states that the PLAN is NOT checked against the option
//     list, which is the other half of the same defect: "write your plan FROM
//     the list" (system_prompt.txt) was read as "the plan line is validated
//     against the current options", costing 52% of one 26.8k trace.
//  #4 "<action name exactly as listed>" is UNSATISFIABLE against real option
//     labels: they carry {mana}, {right now: ...} and a {card text: "..."} blob
//     that is itself truncated mid-sentence with an ellipsis. The model either
//     litigated the scope (~6k chars a time) or complied literally and echoed
//     the whole blob - the mechanism behind every post_answer_overrun event
//     measured (42/90 at batch2, 53/146 at batch6). The contract is now NUMBER +
//     SHORT NAME, which is what the parser already wants: parseChoice matches
//     the echo's significant words as a SUBSET of the option text (and, in the
//     reverse pass, the option's words as a subset of the echo), so a short name
//     binds exactly and a copied blob only adds noise. The instruction is moved
//     to parser reality, not the parser to the instruction.
//  Also folded in (batch5 #10 / batch6 P6): the parenthesised-name mandate and
//  the ATTACK:/BLOCKS: worked examples contradicted each other, and 70/102
//  traces reasoned about the format. The label form ("ATTACK: A1, A3") is what
//  every combat example and every per-decision line already shows and what the
//  parsers prefer, so the mandate is scoped to CHOICE: and combat is exempted.
```

### AIPlayerGPT-L623-41  — AIPlayerGPT.cpp, near `L623` (#W81)

```
//#W81-DM (V16, wave-80 known-bugs V16 + deck125 MED B-1). THE ECHO HEADING
//STOPS NAMING A PLAN IN CAPS. The old heading opened `THE PLAN YOU LAST STATED`
//and the reply protocol's own label is `PLAN:`; all four of deck125's protocol
//deviations are that collision - three replies whose label slot reads
//`THE PLAN: ...` (`125v126` seqs 164/165, `125v152` seq 63) and, at `152` seq
//111, a reply whose entire plan line is the render's own sentence echoed back
//(`THE PLAN YOU LAST STATED reads "End turn."` + `CHOICE: 6`). #W73-CA already
//established the mechanism and the remedy: what the model copies into the label
//slot is the CAPS PHRASE that names a plan, not the punctuation. #W73-CA removed
//`YOUR PLAN` for exactly this and the carry heading kept a second one.
```

### AIPlayerGPT-w80TrimDanglingTail-56  — AIPlayerGPT.cpp, near `w80TrimDanglingTail` (#W80)

```
//#W80-DG (U12 second half, wave-79 deck126 MED): ONE marker per cut.
//Wave 79 printed the `...` AND the `(...more)` on the same cut, so
//Sigarda's line read "...look at the top five... (...more)" - two
//omission marks for one omission, which reads as two different cuts.
//`(...more)` already opens with the ellipsis and already says text
//was omitted, and with the dangling-connective trim above the cut now
//ends on a phrase that can end, so the separate `...` says nothing
//the marker does not. The marker itself is UNCHANGED, which is what
//keeps every "is this render marked?" pin binding on one literal.
```

### AIPlayerGPT-textCarriesSeatVoice-70  — AIPlayerGPT.cpp, near `textCarriesSeatVoice` (#W72)

```
//#W72-BV (M8, deck152 HIGH-1 - it cost the game). A card's printed text is
//written from ITS CONTROLLER's seat: on the OPPONENT's battlefield line
//`Exquisite Blood {4}{b} [enchantment] {effect: "Whenever an opponent loses
//life, you gain that much life."}` says "you" about THEM and "an opponent"
//about the reader, and nothing on the line said so. `152v126` seq 35 inverted
//it in its own reasoning ("if I deal damage to them, they gain life from
//Exquisite Blood ... Net life change: 0"), alpha-struck into a printed
//`so blocking can leave them as high as 12`, and lost the game to the eight
//points it thought were refunded. The engine and the primitive are both right
//(seq 40 shows the opponent going 12 -> 4 with no gain); the render is the only
//wrong surface. The prompt already normalises perspective for the CONVERTER
//paragraph - this is that machinery applied to the row the model actually
//quotes.
//The frame STATES THE MAPPING rather than rewriting the sentence: a rewrite of
//arbitrary rules text is a surface that can be false, and a false surface is
//the defect this fixes. It is emitted only where the text carries a seat
//pronoun at all, so a card whose text names no seat says nothing extra.
//Pure over the snippet, so both branches are provable in PARSETEST.
```

### AIPlayerGPT-scriptMemoEnabled-93  — AIPlayerGPT.cpp, near `scriptMemoEnabled` (N-1)

```
//N-158a/N-158c (wave-31 deck158): AMASS. The amass macro puts N +1/+1 counters
//on an Army you control, creating a 0/0 Orc Army token first if you control
//none - and everything the card then does keys off the POST-amass Army. Two
//defects fell out of not modelling that: the cast option carried no Army-size
//preview at all (the pilot re-derived it every single time, and this seat's
//top-2 reply-length spikes are those re-derivations), and "damage:power" was
//evaluated against the SOURCE - a sorcery, power 0 - so Foray of Orcs rendered
//"{right now: damage 0}" in 100% of cases while resolving for 2 and for 9. The
//pilot argued with the annotation out loud at 13.1k and 9.8k characters.
//Detect the macro from the script and report the count so both surfaces can use
//the real post-amass number. Returns 0 when the card does not amass.
//Pure over the script text so the real primitives can be replayed in PARSETEST.
```

### AIPlayerGPT-lifeToDamageConverterTake-110  — AIPlayerGPT.cpp, near `lifeToDamageConverterTake` (#W50)

```
//#W50-X D3 (wave-49 ledger HIGH, the corpus's one render falsehood): the
//gain RECIPIENT is read off the script's dynamicability target token, not
//assumed. Devour Flesh is `toughnesslifegain targetcontroller` - the TARGETED
//player (the one sacrificing) gains - and the row printed "you gain 5" while
//the opponent went 20 -> 25 (deck123 vs162 seq 8; 10 rows, believed twice).
//Tribute to Hunger (`targetopponent`) and Consuming Vapors (`abilitycontroller`)
//keep "you gain": their recipient is the caster.
```

### AIPlayerGPT-lifeToDamageConverterTake-122  — AIPlayerGPT.cpp, near `lifeToDamageConverterTake` (#W53)

```
//#W53-O (D13, wave-52 ledger MED): the LIFE-TO-DAMAGE CONVERTER block sits in
//the frame and the row it prices sits twenty lines below it. `123v126` seq 66
//(14 life, opponent 26) printed "they gain 4" beside a printed Sanguine Bond and
//the consequence - 4 off the pilot - appeared nowhere; 14 -> 10 and 26 -> 30 in
//one window. The gain amount is already on the row, the converter is already
//scanned, so the row states the whole trade. Same construction lane K uses for
//pain-source payments (a life number, then the lethal case named outright).
//How much a converter of THEIRS takes off the seat when its controller gains
//`gain` life: `:life:-thatmuch` mirrors the gain, `:life:-N` is that fixed N,
//anything else is not knowable here (0 - print nothing rather than a guess).
```

### AIPlayerGPT-w73GhostformTriggerLive-137  — AIPlayerGPT.cpp, near `w73GhostformTriggerLive` (#W60)

```
//#W60-O (B7, wave-59 deck125 HIGH-2): the count was the whole clause. 180
//`{right now: destroys/exiles N of their creature ...}` renders across the
//wave-59 corpus, 0 of them naming a creature, while every Path to Exile row
//already carries the `{removes: <name>}` roster removalVictimTag builds. The
//deciding fact is the IDENTITY: deck125's own guide branches on "exactly 1
//creature, and that creature is tagged [defender], or shows printed power 0 or
//1", which needs a name and a printed P/T carried from the board list down to
//the option row - and the seat failed to do that 5 times in one corpus (twice
//consecutively in a game it lost by 4 life). So the sweeper row names what it
//would take, on BOTH sides, in the same facts shape the board snapshot and the
//edict clause use (display name + copy handle + live P/T + the `(printed X/Y)`
//tail when they differ + the live keyword set). Same count, same scope wording,
//same order: with both rosters empty the string is BYTE-IDENTICAL to before.
```

### AIPlayerGPT-edictSelfClause-155  — AIPlayerGPT.cpp, near `edictSelfClause` (#W77)

```
//#W77-CS (R3 c): and the seat's own converter on the seat's own gain - the
//other half of the one clause shape. `126v123` deck126 seq 23's sibling: a
//gain OF THE SEAT'S under the seat's OWN Sanguine Bond is damage to THEM, and
//no surface said so.
```

### AIPlayerGPT-mulliganNoCoverCause-164  — AIPlayerGPT.cpp, near `mulliganNoCoverCause` (#W54)

```
//#W54-E (D16, wave-53 ledger MED = R176). "Playing every land in this hand
//would not cover any spell in it" was printed for TWO different hands: too few
//lands, and enough lands of the wrong colour. The one wrong mulligan on the
//wave-53 reviewers' seats (`152v162` s1: 2 lands making {G}{G} against five
//white spells) was the COLOUR case and the reply's own stated reason was the
//colour, so the two cases are not interchangeable to the pilot. Both facts are
//already inputs here, so the line names WHICH it is. Strictly hand-derived like
//everything else on this surface (the pregame ask carries no board state), and
//pure so every branch is provable in PARSETEST.
```

### AIPlayerGPT-revealRefusalMustPick-178  — AIPlayerGPT.cpp, near `revealRefusalMustPick` (note)

```
//
//A reveal whose option one is a MANDATORY single pick has no decline the engine
//can finalize: `MTGRevealingCards::CheckUserInput` only ends a chooser on
//BTN_NEXT when `targetMin == false || maxtargets == UNLIMITED`, so a zero-target
//answer on a `target(<1>...)` chooser presses nothing, the driver re-enters
//phase 3 with nothing changed, and 600 s later the stall guard force-closes the
//reveal and puts every revealed card back. That is not a fallback: it VOIDS a
//resolved spell. `126v162` seq 10 (`ANSWER: PUT: 44`, row 44 eligible) -> seq 12
//"the engine returned the 52 revealed cards to your library"; deck 126's only
//Idyllic Tutor, turn 5, game lost. Same pair at `126v123` seq 51 -> 53. Both of
//the corpus's `reveal_stall_forced` records, and both of its refused reveals -
//the class is 2 of 2, not a coincidence.
```

### AIPlayerGPT-w42Collapse-195  — AIPlayerGPT.cpp, near `w42Collapse` (W42-)

```
//W42-D4: RENDER-NAME MAPPING for engine ability labels.
//`MTGAbility::getMenuText()` returns the INTERNAL menu tokens the 480x272 d-pad
//UI was built around, and every one of them reached the pilot VERBATIM in the
//wave-41 corpus: "ToughLife" (137 lines), "Put in Play"/"Put in Hand"/"Put in
//Library" (2,416), " 1/1 Counter" (821 - with the engine's own leading space,
//rendered as the literal double space in "used:  1/1 Counter"), "level Counter"
//(502), "Deal 1 Damage" (6,216). The engine strings belong to the HUMAN UI and
//are NOT touched; this is the render boundary for the model's surface only.
```

### AIPlayerGPT-stackDrawRecipient-208  — AIPlayerGPT.cpp, near `stackDrawRecipient` (#W69)

```
//#W69-BH (K4a, deck125 HIGH; wave-68 lane BB's fold still under-counted):
//WHO the pending draw is FOR. The scan below asked `dynamic_cast<Player *>` of
//the AADrawer's target and counted nothing else, while `AADrawer::resolve`
//asks `MTGAbility::getPlayerFromTarget` - which maps a CARD target to its
//controller. `@each my upkeep:draw:1` with no target keyword (Staff of Nin;
//`who == UNSET`, target = the source card) therefore scanned as ZERO draws for
//anybody, while `draw:1 controller` (Howling Mine) set `who = CONTROLLER` and
//getTarget() handed back a Player - which is why the wave-63 evidence looked
//like the walk worked. Corpus proof (matchups-20260906-134120, 2,282 prompts):
//the reserve's stack term rendered `plus the N draws already on the stack`
//exactly ZERO times, and both of the corpus's own-upkeep ANNOUNCE_X windows
//with a pending Staff trigger (`125v126` s91 and s110) printed a reserve of 1
//where the truth was 2 - s110 read a ceiling of X=7 against a true X=6, the
//seat announced 7 and decked itself at 65 life. Same mapping the resolution
//uses, so the count and the resolution cannot disagree. Pure over the target.
```

### AIPlayerGPT-attackRestrictionFree-228  — AIPlayerGPT.cpp, near `attackRestrictionFree` (#W74)

```
//#W74-CC (O12, wave-73 deck125 A-4): a fifth restriction, and the one the
//static count could not see. `125v123` seq 399 printed "103 of them without a
//restriction against attacking" over `Human #1-#102 (1/1) [doesn't untap during
//its controller's untap step] [tapped - cannot attack or block this turn]` -
//two Intruder Alarms on the board, so those 102 bodies are tapped and have no
//untap step to come back from. Tappedness alone is NOT a restriction on the
//non-live board (it clears at their untap), which is why this predicate ignores
//it; tapped AND no-untap is permanent and belongs here. The deck125 guide has
//carried a compensating sentence since wave 70 and this corpus falsified half
//of it.
```

### AIPlayerGPT-battlefieldHeaderText-243  — AIPlayerGPT.cpp, near `battlefieldHeaderText` (#W53)

```
//#W53-P (D14, wave-52 ledger MED): the header counted creatures and stopped
//there, so a board whose land half is COLLAPSED into four printed rows
//("Island #1-#5 x5; Plains #1-#5 x5; ...") gave the pilot no number for it -
//deck130 pushed eight land-destruction casts past its own four-land gate over
//one corpus (130v125 seq 61 and five more). The land count is exactly as
//computable as the creature count at this call site, and the collapse is
//precisely what makes it uncountable by eye, so state it. `lands` < 0 omits
//the clause entirely and renders byte-identical to the pre-fix header, so
//every existing window and its PARSETEST cases are untouched.
```

### AIPlayerGPT-zeroPowerBlockerTag-257  — AIPlayerGPT.cpp, near `zeroPowerBlockerTag` (#W47)

```
//#W47-R12 (wave-46 ledger, MED): the STOPS quantity carried no DURATION and was
//read as a standing property of the wall - a 0/4 Perimeter Captain traded itself
//for ONE point of stopped damage against a 1/1 first-strike deathtoucher, and a
//0/3 Pride Guardian took the 3-damage attacker that kills it over the 1-damage
//attacker it survives (deck126 vs146 seq 5 and 8, both blocks lost the game).
//The quantity is true for THIS combat and for no combat after it, so it says
//so. Scoped on the two positive non-menace rungs, which are the ones that make
//an unqualified STOP claim; the menace rungs already defer per-A-line, and the
//trample rung already qualifies. What is NOT added: a verdict. "Do not block"
//is strategy and belongs in the guide - this asks only that the quantity stop
//overstating its reach.
```

### AIPlayerGPT-L7422-273  — AIPlayerGPT.cpp, near `L7422` (#W45)

```
//#W45-2 (wave-44 ledger, HIGH): THE COLLAPSE THRESHOLD, named and in the open.
//It used to be a bare `> 4` literal inside the builder, invisible to every
//reviewer reading the corpus - and the collapsed form it selects is a different
//surface from the enumerated one: it shows exactly ONE outcome, the biggest
//defender's, which on a wide board is the FAVOURABLE one. deck162 vs123 attackers
//seq 13: "they have 6 untapped creatures that could block this one, biggest
//Bloodline Keeper #1 (3/3) (you kill it, your attacker lives)" - true, and three
//2/2 Vampires then gang-blocked and killed the 5/5. All seven strategy guides are
//written against the ENUMERATED form; none mentions this one. Raising this number
//widens the enumerated (honest-by-construction) form at a token cost that grows
//with board width; the collapse is the token-budget compromise, so the price of
//the gang block travels with it (`gangNote`, computed by the caller where the
//candidates' powers are in hand).
```

### AIPlayerGPT-attackerBlockerCountLine-291  — AIPlayerGPT.cpp, near `attackerBlockerCountLine` (#W64)

```
//#W64-AJ (deck162 MED, vs123 seq 16). THE AGGREGATE CONTRADICTED THE ROWS
//UNDER IT. `canBlock()` is the SOLO gate - untapped, not forbidden to block -
//and says nothing about whether that body may legally block any attacker on
//OFFER, so the header counted walls that every per-attacker tag in the same
//prompt had already ruled out with `[no creature they control can block this
//attacker]`. Two true statements, one of them in the wrong scope, and the
//header is the one the model reads first. `evasive` is the count of attackers
//in THIS bundle that no untapped creature of theirs may legally block - the
//same `rowNoLegalBlock` predicate the guaranteed-damage floor below is built
//from, so the header, the floor and the rows cannot disagree. Nothing is
//deleted and no number is replaced: the count stands and its SCOPE is named.
//Pure over the three counts (`evasive <= 0` renders the wave-54 line byte for
//byte, which is every window where no attacker has evasion).
```

### AIPlayerGPT-L8046-309  — AIPlayerGPT.cpp, near `L8046` (#W74)

```
//#W74-CC (O5, wave-73 deck126 HIGH-1): the seat's OWN life-to-damage
//converter. `126v130` seat seq 43 printed "at 3 ... That is NOT lethal"
//over one 2-power LIFELINK attacker while Sanguine Bond sat on the seat's
//own battlefield: the lifelink gain of 2 is life the opponent ALSO loses,
//so the true unblocked figure was 1, not 3, and the categorical verdict was
//computed from a number the same prompt's own CONVERTER paragraph
//contradicts. Lifelink damage is counted ONCE MORE here, before the figure
//and before the verdict, so the two surfaces cannot disagree. Scoped to a
//converter the ACTING seat controls (a converter of theirs converts THEIR
//gains, which this line does not compute), and to the same
//none-of-them-blocked assumption the sentence already states.
```

### AIPlayerGPT-L8091-325  — AIPlayerGPT.cpp, near `L8091` (#W76)

```
//#W76-CO (Q4b, wave-75 deck123 MED-1). THE FLOOR WAS ASSERTED OVER
//A DECLARATION TRIGGER. `125v123` deck123 seq 1208: 100 attackers,
//"At least 401 damage lands whatever they block - they would be at
//-304", with TWO Lightmine Fields on their battlefield. The seat
//declared all, the trigger killed the whole board before damage,
//and their life did not move from 97. C1b's tail sentence already
//named the punisher - AFTER the claim - and #W64-AK settled that a
//true footnote does not repair a false verdict. The floor is a
//floor over their BLOCKS ONLY, and the sentence now says so where
//a punisher is on the board, in the same clause as the number.
```

### AIPlayerGPT-gangBlockPriceTag-340  — AIPlayerGPT.cpp, near `gangBlockPriceTag` (#W46)

```
//#W46-5 (wave-45 ledger, MED): the SAME clause is owed to the BLOCKERS window,
//where the seat is the one fielding the group - deck123 vs152 seq 47 handed the
//pilot thirteen rows all reading "(your blocker dies, attacker lives)" against a
//7/7 with no group price anywhere on the screen, and it spent ~250 words deriving
//"I need exactly 7 blockers" against the prompt's own "do not re-derive these
//outcomes; use them". One builder, two voicings ('mine' flips the possessives and
//points at the B-lines, which sit BELOW the attacker line rather than above it):
//forking it would let the two windows drift apart on the same arithmetic.
```

### AIPlayerGPT-printedPTTag-353  — AIPlayerGPT.cpp, near `printedPTTag` (N-1)

```
//N-152d LAYER 2 (wave-32; MECHANISM CORRECTED at fix time): the docket recorded
//this as "fixed on the attacker/target enumerators, still false on the board
//snapshot". That is not what the source says - the board snapshot
//(describeZoneCards) is the ONLY emitter of a "(printed X/Y)" tail in the whole
//file, and it was ALREADY routed through this helper on the corpus binary
//7fabd9bd0. The real defect is this helper's DISCRIMINATOR: `isFlipped` is not a
//reliable "which face is showing" flag. AAFlip::testDestroy (AllAbilities.cpp:
//5811-5817) RESETS isFlipped back to 0 immediately after a transform resolves,
//while the card keeps its back-face name and back-face origpower/origtoughness
//and its FRONT-face basepower/basetoughness - so a day/night werewolf renders
//with isFlipped == 0, basepower 2 (Brutal Cathar) and power 3 (Moonrage Brute),
//producing exactly the observed false "(3/3) (printed 2/2)". (TestSuiteAI.cpp:
//898 already carries the same finding: "isFlipped is NOT usable as the
//discriminator - the day/night transform path leaves it 0".) AATurnSide, the
//modal-DFC display toggle, DOES keep isFlipped coherent, so the flag stays in
//the test as one of two signals rather than being replaced by it.
//  The reliable signal is the NAME: both face-swapping paths stash the original
//name in nameOrig and rename the instance (AAFlip 5618, AATurnSide 5474), and
//both restore name == nameOrig when the card goes back. `showsOtherFace` is the
//OR of the two signals, so neither path can go unnoticed again.
```

### AIPlayerGPT-perColorSourceCountClause-378  — AIPlayerGPT.cpp, near `perColorSourceCountClause` (#W46)

```
//#W46-1 (wave-45 R1, HIGH, cost a game at 6 life): the colour SET answers
//MEMBERSHIP ("can I make black at all?"), and membership is not the question a
//coloured cost asks. deck126 read `colours you can make: {g}{u}{r}{b}{w}` over a
//sub-list holding exactly ONE {b}-capable card, against `Cast Sanguine Bond
//{3}{B}{B}`; it tapped Overgrown Battlements for green twice, floated ten green,
//could not cast, and the two tapped walls then could not block (6 -> 1, lost).
//pregameHandHeaderText already ships the AGGREGATE form ("Mana sources among
//those lands, counted by the engine: {W} 2, {U} 2") and stops at the opening
//hand, so in-game the pilot had to re-derive the aggregate by parsing the
//sub-list one card at a time - the exact re-derivation N-139n's comment block
//argues only an engine-computed aggregate can close.
//  Two counting conventions are STATED IN THE CLAUSE rather than assumed,
//because an unstated convention makes the number a new false surface: a source
//that makes more than one colour is counted under EACH of them (so the counts do
//not sum to the total), and a variable-output source counts as ONE, exactly as
//the total already counts it. Deliberately NOT a mana count: three Battlements
//are "{G} 3" and can make far more than three green.
//Pure (takes the per-source colour strings, not a game), so the shape and both
//conventions are provable in PARSETEST.
```

### AIPlayerGPT-animatedThisTurnNote-402  — AIPlayerGPT.cpp, near `animatedThisTurnNote` (#W52)

```
//#W52-L (D13 b): the name of a tapped source that is a land animated THIS
//turn says so - the seat paid to animate Lair of the Hydra at Upkeep and the
//Main-1 cast row named it as a plain tap (deck152 vs125 seq 36->39).
```

### AIPlayerGPT-sorceryReserveClause-410  — AIPlayerGPT.cpp, near `sorceryReserveClause` (#W66)

```
//#W66-AT (deck130 MED): the NEGATIVE half of the same fact. The board line
//says "CAUTION - some usable mana sources DAMAGE YOU when tapped for mana
//(auto-tap when you cast may spend them)"; the per-row clause below prints only
//when the plan DOES spend one, so a row whose plan spends none says nothing at
//all - and "may" plus silence is not a verdict. `130v126` seq 32: two Talisman
//of Impulse (1 damage each) on a board carrying BOTH halves of the opponent's
//life LOOP, so any nonzero payment was fatal; six castable cards, every row's
//auto-tap plan spending four Mountains and no Talisman, no row saying so, and
//the seat answered "Cast nothing right now" with the plan "no safe play
//exists" and held for the rest of the game. The row already asked
//`selectAutoTapProducers` which sources it would tap - this states its answer
//when the answer is "none of the ones that hurt you".
```

### AIPlayerGPT-sorceryReserveClause-427  — AIPlayerGPT.cpp, near `sorceryReserveClause` (#W66)

```
//#W66-AT (deck162 MED): the mirror of #W52-L's strand clause, for the window
//that clause has no opinion about. An INSTANT-SPEED row offered in the seat's
//OWN upkeep or draw step can wait - its own last window this turn is the end of
//the OPPONENT's turn - while a SORCERY-SPEED card in hand has exactly one
//window left, this turn's main phase, and the mana this row spends comes out of
//it first. `162v123` seq 11 and 14 took Dictate of Kruphix (Flash) in the seat's
//own DRAW step with Ob Nixilis, the Hate-Twisted stranded in hand; the row named
//what it stranded at instant speed and said nothing about the sorcery-speed half.
//Pure over the numbers the row already has: `left` is what the row leaves, and
//`need` is the stranded card's converted cost.
```

### AIPlayerGPT-sorceryReserveClause-442  — AIPlayerGPT.cpp, near `sorceryReserveClause` (#W72)

```
//#W72-BV (M9, deck162 HIGH): the verdict IGNORED AN UNUSED LAND DROP and was
//provably false with one available. `162v146` seq 10 printed
//`Land drop: NOT yet used this turn - you can still play a land`, an Island in
//the hand, and `VERDICT: taking this row STRANDS Howling Mine this turn - 1
//source cannot pay 2` in the same prompt; seq 11 played a Swamp and seq 12 cast
//the Mine. The model re-derived the true count, overrode the verdict and was
//right - which is the worst possible training signal on a screen whose whole
//contract is that printed verdicts are true, and the same seat then overrode a
//TRUE verdict at `162v152` seq 17 and lost the game to it. `landDropSources` is
//the count of lands the seat can still play this turn that produce mana on
//arrival; folded into the remainder, it can only make the clause claim LESS.
```

### AIPlayerGPT-sorceryReserveClause-458  — AIPlayerGPT.cpp, near `sorceryReserveClause` (#W72)

```
//#W72-BV (M9, second half; deck162 HIGH): the consequence led from the BACK.
//The clause ended "...so your main phase this turn is its last window. VERDICT:
//taking this row STRANDS X ...", and `162v152` seq 17's reasoning quotes the
//last-window sentence as a promise - "That means I will cast it in Main Phase
//1" - then takes the row and loses. The verdict now LEADS with what the row
//does to the reader, in the second person, before any clause about windows.
```

### AIPlayerGPT-w78AnswerLabelAbsentShape-469  — AIPlayerGPT.cpp, near `w78AnswerLabelAbsentShape` (#W76)

```
//#W76-CP (Q15, wave-75 engine-seat LOW-1): A FORCED CLOSE'S OWN PREFILL IS NOT
//THE MODEL'S PROSE. `126v146` deck126 seq 22 is a `reasoning_forced_close`
//record whose 823 off-protocol bytes are the phase-2 prefill's trace echoed
//back ahead of a correctly-formed PLAN/ATTACK pair, and the answer was consumed
//correctly. Counting it as `prose_outside_two_lines` made the class mean 3 of 3
//where the honest reading is 2 of 3. The record is NOT dropped and the two
//underlying fields are untouched - the class simply names what this one was, so
//a reviewer can separate the rescue's echo from a reply that wrote prose of its
//own. A forced close that ALSO lost its plan line is still reported as the plan
//defect it is; only the bytes-only shape is re-named.
```

### AIPlayerGPT-tapCostBeforeCombatClause-484  — AIPlayerGPT.cpp, near `tapCostBeforeCombatClause` (#W69)

```
//#W69-BI (K7, deck152 LOW): ONE LITERAL FOR ONE FACT. This clause and
//`paymentTapsClause` state the same thing - this action taps that body, so it
//cannot attack this turn - in two different spellings, and `152v162` s25 printed
//BOTH on one row (23 prompts carry `{tapping `, 19 carry `{paying this taps: `).
//A guide rule could not key on the fact without quoting two strings, and the one
//guide that quotes it (deck152) quotes `{paying this taps:`. So this emitter now
//composes the SAME clause, with the same restriction words, from the same
//builder: nothing is deleted, one spelling less to teach. `{tapping ` stays on
//stripNarrationDecoration's list - older narration still carries it.
```

### AIPlayerGPT-markBareBackFaces-498  — AIPlayerGPT.cpp, near `markBareBackFaces` (#W51)

```
//#W51-F D16 (wave-50 ledger LOW; 15 bare `// <name>"}` endings on deck130's
//land-destruction target clauses): the THIRD emitter of a printed face after
//optionCardTextCore (#W49-U D12) and printedFirstClause (#W50-X D17). Same face
//rule, same helper, applied to the snippet the target note quotes - the target
//clause and the STACK note share this core, so both paths close at once.
```

### AIPlayerGPT-L11648-508  — AIPlayerGPT.cpp, near `L11648` (#W75)

```
//#W75-CL (P15, deck125 A-3): ...IN BOTH DIRECTIONS. `#W74-CC` (O19) made the
//X ladder ASCENDING, and the ranged row now says so ("X = 2 up to X = 8 -
//one option per X in that range, smallest X first"), but this decode still
//taught the descending walk only: 14 of the wave-74 corpus's 28 X menus
//carried a paragraph that counted DOWN over a list that counts UP
//(`125v162` seq 148). Two statements on one screen, one of them false - and
//this is the one a pilot decoding an option NUMBER out of a range reads. Both
//walks are now stated, keyed on the words the row itself prints, so the
//paragraph can only be read against the row it is about. The descending
//sentence is byte-identical to wave 56's, so every existing pin holds.
```

### AIPlayerGPT-L12639-523  — AIPlayerGPT.cpp, near `L12639` (#W67)

```
//#W67-AY (MED, deck123 / 126 s115): THE LOG DOES NOT BUCKET WHAT THE BOARD LINE
//BUCKETS. 123v126 s115 (t15) carries a 208-line contiguous death batch - 96
//byte-identical "Your Human (token) ceased to exist and left your graveyard"
//lines and 94 "Your Human died (that Human was N of 96 copies...; the other K
//are still there)" lines - about 14 KB of a 25 KB prompt, while the same 96
//tokens occupy ONE collapsed entry on the battlefield line. Neither shipped
//collapser can reach it: `collapseAdjacentDuplicate` (#W57-D) needs the
//repeated line to be ADJACENT and the pair alternates, and the cycle holder
//(#W48-D11) needs the repeating block to be byte-identical while the death
//line's two ordinals change on every copy. So the bucketing happens where the
//board line's does - at RENDER, over the composed body, after logWindowApply -
//and it is a pure transform of the text about to be printed: mNarration, the
//translog's `events` delta, the trim marker and every key are untouched.
//The rule is joinZoneEntries': within one contiguous run of event lines, a
//SHAPE (the line with its digit runs normalised) that occurs at or above the
//floor is printed ONCE, verbatim, at the position of its first occurrence,
//with an exact count beside it; every other line keeps its place and its
//order. Nothing is dropped that the count does not state, and a batch with no
//repetition renders byte-identically to before.
```

### AIPlayerGPT-activationSourceIsGrantedDummy-547  — AIPlayerGPT.cpp, near `activationSourceIsGrantedDummy` (note)

```
//
//Wave 64 moved the activation line's actor onto `e->source->controller()` - but
//on a `targetedplayer` grant `e->source` IS the nameless dummy, and the dummy's
//controller is the GRANTEE, the very player the event's own `controller` already
//named. The two facts were identical, so the fix changed nothing at the seam the
//corpus exercises: 126v130 seq 16 still prints, verbatim,
//  - You cast Tribute to Hunger
//  - Your Tribute to Hunger resolved and went to your graveyard
//  - Opponent used: Gain life equal to its toughness with Tribute to Hunger ...
//  - You gained 1 life (now 21)
//10 lines over 5 events and 3 games, unchanged from wave 63. The NAME half was
//already right because resolveOwningCardName walks `storedSourceCard` back to
//the granting card; the CHAIR has to walk the SAME link, or the two halves of
//one sentence name two different cards.
```

### AIPlayerGPT-poisonIsLiveInGame-566  — AIPlayerGPT.cpp, near `poisonIsLiveInGame` (N-1)

```
//N-166j (wave-34 audit b3's 41-trace litigation + b6 F7, 9 decisions blind):
//poisonStatusLine prints nothing while BOTH counts are zero, so an Infect deck
//saw no poison line at all until its first connect - "there is NO poison
//counter line shown. This is unusual given the strategy guide instructions".
//A zero IS the state, and for an infect seat it is the state its whole plan is
//measured against. But rendering "0 of 10" in every game would tax ~all games
//for the benefit of the few, so gate on poison being LIVE: either count
//nonzero, or some permanent on either battlefield can actually produce poison
//(infect, toxic N, or damage-becomes-poison). Cheap scan, battlefields only.
```

### AIPlayerGPT-combatDamageForecast-580  — AIPlayerGPT.cpp, near `combatDamageForecast` (#W76)

```
//#W76-CO (Q3 a, wave-75 deck146 HIGH-1 + MED-2): AND THE VERDICT'S OWN
//PREMISE. `146v126` deck146 seq 70 (T22 Blockers, 20 life): this line read
//"Unblocked, these attackers deal up to 1 - you would be at 19 - NOT lethal:
//block only where the trade favors you", six lines under an INCOMING block
//that had just refused to make ANY survival claim - both halves of the
//opponent's life LOOP were on their battlefield, so the 1 unblocked damage is
//an entry into a chain that does not stop until the seat is at 0, and every
//block gains them lifelink life that enters the same chain from the other end.
//Two statements on one screen and one of them false; the seat blocked nothing
//and lost the game. `oppLoopLive` is `lifeLoopProvenWin(opponent())`, the same
//predicate the INCOMING block is rendered from, so the two cannot disagree.
//Nothing is deleted - the subtraction still prints, in full - and no verdict is
//invented: the SURVIVAL CLAIM is withdrawn, and the withdrawal is said, which
//is the same shape #W63-AB (E1) uses on the INCOMING line itself. The
//take-the-damage-while-ahead hint goes with it (it is the inverse of correct
//play against a chain; #W54-E D21's own reasoning, applied to the loop).
```

### AIPlayerGPT-combatDamageForecast-601  — AIPlayerGPT.cpp, near `combatDamageForecast` (#W80)

```
//#W80-DF (U14, wave-79 deck146 MED). "NOT LETHAL" WAS TRUE OF THE COMBAT AND
//FALSE OF THE TURN CYCLE. `146v162` seq 29, at 4 life: this line read `Unblocked,
//these attackers deal up to 3 - you would be at 1 - NOT lethal: block only where
//the trade favors you`, eleven lines under the same screen's `DRAW FORECAST:
//your next draw step draws 1 card = 1 x 3 = 3 life LOST BY YOU to their punishers
//above ... This draw step is COMPULSORY - no row on any menu declines it`. The
//two numbers are on one screen, they add, and the sum is death - but the verdict
//the model is instructed to believe is computed from one of them. The draw-step
//loss is the one non-combat damage the seat provably CANNOT decline this cycle
//(the forecast line says so in its own words), so it belongs inside the lethality
//test at the seam that states the verdict. Nothing is deleted: the combat
//subtraction still prints in full, the survival CLAIM is withdrawn and the
//withdrawal is said, and the take-the-damage-while-ahead hint goes with it
//(#W54-E D21's own reasoning - it is the inverse of correct play when the rest of
//the cycle kills you). `forcedCycleLoss` 0 leaves every wave-79 byte in place.
```

### AIPlayerGPT-drawPunisherSummaryText-621  — AIPlayerGPT.cpp, near `drawPunisherSummaryText` (#W63)

```
//#W63-AC (E13, engine MED-8): lane Y's D3 fixed the two forecast LINES and
//said it left this paragraph alone; the paragraph then stood on the same screen
//(`162v126` seq 13) carrying the chaining caution over BOTH of its numbers,
//eight lines above a forecast that correctly said the same board does not chain
//that way. The paragraph states TWO numbers with opposite owners - "every card
//YOU draw costs you N" is life the pilot loses, "every card the OPPONENT draws
//costs them M" is life they lose - and a loop held by side S is entered only by
//life S's OPPONENT loses (or life S gains). So each sentence carries the clause
//that is true for ITS number, exactly as D3 bound the forecast lines. Nothing
//the model relies on is deleted: where both numbers are on the page both
//clauses print; the only string that disappears is a false one.
```

### AIPlayerGPT-drawPriceRowTag-637  — AIPlayerGPT.cpp, near `drawPriceRowTag` (#W61)

```
//#W61-S (C6, wave-60 deck126 HIGH-1, the row the seat took): when a row carries
//BOTH draw-price clauses they folded off the SAME base life, so `126v162` seq 16
//(t13, life 23) printed "you would be at 22" and "you would be at 16" on one row
//whose true price was 8. Neither figure was the answer. `deferTotal` hands the
//resulting life to the clause that follows, which folds both charges once; the
//cost this clause states is unchanged, and every single-clause row is
//byte-identical.
```

### AIPlayerGPT-drawPriceRowTag-649  — AIPlayerGPT.cpp, near `drawPriceRowTag` (#W80)

```
//#W80-DF (U6, wave-79 deck130 HIGH-2 - a lost game). A CYCLE IS A DISCARD AND A
//DRAW, AND THIS TAG PRICED ONE HALF. `130v162` seqs 35 and 37 printed `cycling
//with Starstorm [cost: {3}, Cycle] ... [DRAW PRICE: this draws 1 card, and the
//opponent's Underworld Dreams punishes every draw, so taking it costs you 1 life
//right now - you would be at 18]` on a screen whose own DISCARD PUNISHERS
//paragraph read `theirs - Liliana's Caress. Every card YOU discard costs you 2
//life`; the seat went 19 -> 13 across two cycles and lost at exactly 0. The
//engine agrees with the rules text: `CycleCost::doPay` sends a
//`WEventCardDiscard` before the draw (ExtraCost.cpp), which is exactly what
//Liliana's Caress's `@discarded(*|opponenthand)` fires on. 33 rows in the corpus
//carry this shape. The bracket keeps its one literal and its one resulting life
//- the row's whole price, stated once - and a row with no discard in its cost is
//byte-identical to wave 79.
```

### AIPlayerGPT-castDrawPriceRowTag-667  — AIPlayerGPT.cpp, near `castDrawPriceRowTag` (#W53)

```
//#W53-O (D3, wave-52 ledger HIGH): the tag stated a damage total and stopped
//there. deck125 vs162 turns 18-20 carried 41 rows across 22 windows reading
//"deals you 7" at 7 life or less; seq 97 (3 life) took row 1 and the seat was
//dead two records later. Lane K's mana-cost path already prints the same
//arithmetic's consequence ("- you would be at K - this KILLS you") - so the
//draw-price total gets the same tail, with the seat's life as the only new
//input. life < 0 means "not supplied" and prints nothing.
```

### AIPlayerGPT-discardPunisherScan-679  — AIPlayerGPT.cpp, near `discardPunisherScan` (#W60)

```
//#W60-N (B5, wave-59 deck125 HIGH-1, a lost game): the DISCARD mirror of
//drawPunisherScan. `theirsPerDiscard` is what THIS seat pays for every card IT
//discards - Liliana's Caress x2 on the opponent's battlefield is 4 a card, and
//an 11-card cleanup discard is 44 life, which is exactly the 13 -> -31 the
//`125v162` seat took off a row that told it the cast was NET 0.
```

### AIPlayerGPT-w80VariableDrawIsHandSize-689  — AIPlayerGPT.cpp, near `w80VariableDrawIsHandSize` (#W80)

```
//#W80-DG (U12, wave-79 deck162 MED-1): "AN AMOUNT THAT IS NOT FIXED" WHEN THE
//AMOUNT IS ON THE SCREEN. Teferi's Puzzle Box is
//`count(type:*:opponenthand) && bottomoflibrary all(*|opponenthand) &&
//draw:countedamount opponent` - the variable IS the hand size, the same hand
//size the board block prints one paragraph above (`162v152` seq 18: 14 cards
//against a 6-card hand, unpriced). The script names its own counter, so the row
//can too. Pure over the script text; the LIVE number is read at the call site.
```

### AIPlayerGPT-drawFeedsStepForecast-701  — AIPlayerGPT.cpp, near `drawFeedsStepForecast` (#W62)

```
//#W62-X (D8, deck126 engine HIGH-1): "next" and no verdict. `126v162` seq 37
//rendered "your NEXT draw step draws 13 cards ... = 26 life" in phase Draw with
//all four draw triggers ALREADY on the stack, two lines under "Your life: 20" -
//a step the seat read as still ahead of it, and a cost above its life that
//carried none of the KILLS-you verdict every other lethal figure on that screen
//carries. `stepIsNow` drops the tense; `holderLife` >= 0 prints the resulting
//life and the verdict. The verdict is deliberately CONDITIONAL on the forecast
//resolving as printed ("if it resolves as forecast") - the step's own card may
//already be drawn, so the count is a forecast and is not re-derived here.
```

### AIPlayerGPT-drawFeedsStepForecast-715  — AIPlayerGPT.cpp, near `drawFeedsStepForecast` (#W66)

```
//#W66-AQ (H1, engine HIGH-1 / deck152 HIGH-2). The forecast charged the WHOLE
//step on every window served INSIDE it. `123v162` seq 138-153 print the
//identical `resolving NOW ... 6 x 1 = 6 life LOST BY YOU ... you would be at 1`
//for 16 consecutive windows while the seat's life falls 7 -> 3, and at seq 152
//(life 4, one point still owed) the same line reads `you would be at -2; that
//KILLS you` - a death claim over a cost of which 1 point remained. The seat
//lived at 3 and won. `152` seqs 41-49 read `draws 4 cards = 4 x 4 = 16` beside
//`ON THE STACK: 7 damage`.
```

### AIPlayerGPT-xNetLifeForX-728  — AIPlayerGPT.cpp, near `xNetLifeForX` (#W60)

```
//#W60-N (B5, wave-59 deck125 HIGH-1, a lost game): the NET was true about the
//DRAW punishers and silent about everything else, and "NET 0 life for this
//cast" is read as a verdict on the cast. `125v162` seq 111 took X=9 off that
//row at 13 life with two Liliana's Caress on the opposing board; the draw put
//18 cards in a 7-card hand, the cleanup step forced 11 discards, and 11 x 4 was
//exactly the 13 -> -31 the game ended on. `handAfterCast` is the hand this row
//leaves BEFORE its own draws (the spell itself has left it); `handLimit` is the
//maximum hand size. Both < 0 means "not supplied", and every branch of the old
//shape then renders byte-identical.
```

### AIPlayerGPT-xNetLifeForX-742  — AIPlayerGPT.cpp, near `xNetLifeForX` (#W61)

```
//#W61-S (C10, wave-60 deck125 HIGH-2): the ladder marker was computed from
//AFFORDABILITY alone and endorsed X=2 on a row whose own text one clause
//earlier read `NET -2 life for this cast` at my_life 2 (`125v162` seq 41/42,
//t15 - the seat took row 1 and died; X=1 is NET -1 and survives at 1). This is
//the row's NET arithmetic pulled out as ONE function so the badge and the row
//text cannot disagree; PARSETEST pins the two against each other over a matrix
//rather than trusting two copies of the same sum. `priced` is false when the
//row prints no NET at all (the cast has no life cost), in which case nothing is
//folded into the marker.
```

### AIPlayerGPT-xLibraryRowClause-756  — AIPlayerGPT.cpp, near `xLibraryRowClause` (#W68)

```
//#W68-BB (J4, deck125 HIGH-1/2; lane AW's "the row and the header cannot name
//different values" FALSIFIED by the corpus). `125v130` s90 printed the header
//"3 draws are already owed ... X=1 is the largest listed X the library pays
//for" over option 6 `{library: this draws 4 of your 4 library cards - 0 left}`,
//and the seat took option 6 and emptied its library with 3 undeclinable draws
//owed. Both surfaces were TRUE and they answered different questions: the
//header folded the whole reserve (draw step + stack + upkeep draws), the row
//folded only the STACK term (`stackPendingDrawsFor`), which was 0 on that
//board - the two Staff of Nin upkeep draws live on the battlefield, not the
//stack, so `; M more pending` rendered 0 times in the whole corpus. The row
//now takes the SAME `owedDraws` reserve `xLibraryCeilingClause` and
//`xAnnounceLibraryNote` are built from, so the largest drawing row that does
//NOT carry the deck warning is exactly `xLibraryCeilingX` - one arithmetic,
//two surfaces (pinned as an identity in PARSETEST). Pure over three ints.
```

### AIPlayerGPT-lifeAfterPendingStack-775  — AIPlayerGPT.cpp, near `lifeAfterPendingStack` (#W66)

```
//#W66-AQ (H4, deck125 HIGH-1): the ladder starts from the life the stack
//LEAVES, not from the life total printed above it. `125v162` seq 36 read
//`ON THE STACK: 13 damage to you - you would be at -8; that would KILL you`
//in the same prompt as `X=4 ... leaves you alive, at 1`. With no stack
//damage `stackLossToMe` is 0 and every byte below is as wave 65 wrote it.
```

### AIPlayerGPT-xTradeCountsAt-785  — AIPlayerGPT.cpp, near `xTradeCountsAt` (#W58)

```
//#W58-B (D5, wave-57 ledger MED-HIGH): the selection rule counted only THEIRS.
//`130v152` s33 marked `X = 4 {kills THEIRS: Sigarda; YOURS: Dwarven Blastminer,
//Siege-Gang Commander, Goblin x3}` - a 1-for-5 - as "the most of THEIRS at the
//smallest cost to YOURS", and the marker is the most reliably followed
//annotation this render produces (marked-row takes 6 of 9, 7 of 9, 4 of 7).
//The words already name both sides; the arithmetic now uses both. Counts at an
//X, over the same survey the rows print from. Pure.
```

### AIPlayerGPT-string-797  — AIPlayerGPT.cpp, near `string` (#W81)

```
//#W81-DL (V8, wave-80 known-bugs V8 / deck130). A REFUSAL DRESSED AS A
//RECOMMENDATION. `162v130` seq 75 rendered `[<- best X for this cast: X=6 -
//the only X that kills anything of THEIRS costs you more of YOURS than it
//takes - THEIRS 4 / YOURS 5]`: the BODY of that marker is the X menu's
//LOPSIDED verdict, which exists precisely to refuse the trade, and the
//`best X for this cast:` prefix reopened it as the menu's answer one screen
//early. The seat took the prefix and swept its own Siege-Gang Commander,
//three Goblins and a Blastminer off a board it had exact lethal on. The
//no-kill verdict was already carried across bare for exactly this reason;
//the lopsided one is the same class of statement - a refusal - and is now
//carried the same way. Nothing is removed: the row still names every number
//it named, and the X menu one screen later is untouched.
```

### AIPlayerGPT-L20358-814  — AIPlayerGPT.cpp, near `L20358` (#W56)

```
//#W56-A (D11): the carried plan's two ages, both pure so the corpus holds
//them without a game. plan_echo_count went 37 -> 143 between waves and 328 of
//1,511 records served a plan more than 40 windows old, so (a) a plan is
//WITHDRAWN outright at a hard age - the reply rules' own "no plan shown yet"
//clause then asks for a fresh one - and (b) when the menu-diff note fires on a
//plan already more than four windows old, the note is served ALONE: 152v125
//seq 101 served the whole of a turn-39 plan and, in the same breath, the
//sentence retracting the card it names, at a seat whose prompts reach 30,031
//chars. A withdrawal is a TRUE token, not a deletion: the pilot is told the
//plan is gone and asked for a new one.
```

### AIPlayerGPT-L20547-829  — AIPlayerGPT.cpp, near `L20547` (#W62)

```
//#W62-fix (wave-61 corpus, 15,900 stale drops in 11 h): a caller that
//splices prompt-only notes into the tail hands the note-free tail as
//keyTail. The declined count and the hold check are by design outside the
//ask key; they were inside THIS key, and the hold check's counter moved on
//every rebuild, so every in-flight answer was stale and the livelock
//breaker gave 6 round trips per decision to the heuristic.
```

### AIPlayerGPT-L20574-840  — AIPlayerGPT.cpp, near `L20574` (N-1)

```
//N-158r: gate the hand-only pregame frame on the ASK, not on the clock.
//`observer->turn == 0` is a PROXY for "this is a pregame decision" that is
//correct on the draw and wrong on the play: the player on the play takes
//their whole first turn while turn is still 0, so an ordinary first land
//drop was served serializePregameState() - no battlefield, no opponent
//board, no "Mana available:" line - under a hand-wide satisfiability header
//that is true across the game and false for that turn. The model spent
//214.5 s and 14,570 chars trying to reconcile the contradiction on a
//three-option land drop (deck158 vs116 seq 2, 3/3 on-the-play games).
//mInPregameAsk is set by the three pregame entry points themselves.
```

### AIPlayerGPT-if-855  — AIPlayerGPT.cpp, near `if` (#W49)

```
//#W49-U D7 (wave-48 ledger MED = R34): the carried plan was IMMORTAL when
//it named no action - deck130 vs125 seq 84-131 re-read "The game is lost
//... Passing is the only legal action" for 48 windows, and under it aimed
//Hammer of Bogardan at itself and cast Starstorm at X=12 into an empty
//board. R11's principle (pregame + land drop) generalised: a plan that
//names no card of the pilot's and no verb from this action menu is not a
//plan about this game. The block is DROPPED and the carry cleared, so the
//reply rules' own "no plan shown yet" clause asks for a fresh one.
```

### AIPlayerGPT-if-868  — AIPlayerGPT.cpp, near `if` (#W50)

```
//#W50-Y D10 (wave-49 ledger MED): expiry is keyed on CONTENT only. (i) A
//plan that OPENS with a verdict ("The game is lost. ...") expires even
//when its later sentences name cards (deck123 vs125 seq 53-56, deck130
//vs125 seq 125 carried exactly that). (iii) The wave-49 "5 verbatim
//echoes" trigger is RETIRED: deck125 vs126 seq 69-96 carried a CORRECT
//action-naming plan 28 windows and deck152 vs126 s31-41 the loop-lockout
//"ATTACK: none" plan 8x - a count would have expired both; the count is
//now a translog report field (plan_echo_count) and nothing else.
```

### AIPlayerGPT-if-881  — AIPlayerGPT.cpp, near `if` (W41-)

```
//W41-3(a) shipped it observer-only, on the reasoning that the acting seat
//already writes its own activations as consumed decisions. That closed the
//165/0 gap by INVERTING it (236 "You used" / 419 "Opponent used"; 17 of 20
//game pairs disagreed about the same activations, worst 0 vs 19): the
//consumed-decision channel only fires when the MODEL answered a priority
//ask, so every heuristic-answered, auto-resolved and trigger-internal
//activation was visible to the opponent and invisible to the seat that
//performed it. The engine event has no such blind spot - everything
//reaching ActivatedAbility::activateAbility raises it - so the actor reads
//it too, and only the genuine consumed-decision duplicates are suppressed.
```

### AIPlayerGPT-loopChainEntrySentence-896  — AIPlayerGPT.cpp, near `loopChainEntrySentence` (note)

```
//
//What was false was the caution's BINDING. A loop held by player H is entered
//from exactly two places: H GAINS life, or H's OPPONENT LOSES life. On
//`162v126` seq 52 the caution ("any life you pay is fatal") stood on the
//THEIR-draw-step forecast, whose number is life the OPPONENT loses to the
//pilot's own punishers - a number that enters THEIR loop nowhere, because
//their mirror reads "whenever an OPPONENT loses life". The seat that believed
//it would have stopped punishing and passed forever; the review records that
//this seat only won by disbelieving the sentence.
```

### AIPlayerGPT-loopChainEntrySentence-910  — AIPlayerGPT.cpp, near `loopChainEntrySentence` (#W73)

```
//#W73-BY (N5, wave-72 deck162 HIGH). ONE SCOPE SENTENCE, SHARED. The
//`LOOP COMPLETE` header and this `LOOP SCOPE` clause stated OPPOSITE things
//about the same number in the same prompt: the header's tail read "a number
//that reads as life to them is lethal to you" while the scope clause read
//"this number is life THEY lose ... does not enter it". `162v126` seq 29 obeyed
//the header, held its only removal and lost 0-28. The two surfaces now compose
//the SAME sentence from this one function, so they cannot disagree - and the
//PARSETEST pin proves it byte for byte. Pure over the side.
```

### AIPlayerGPT-theirConverterBodyTag-923  — AIPlayerGPT.cpp, near `theirConverterBodyTag` (#W65)

```
//#W65-AN (G6, deck162 HIGH-1). `162v126` seq 13 printed this row's finished
//subtraction - `life 21 -> 17` - six lines under the window's own LOOP SCOPE
//paragraph, which said that ANY nonzero payment on a tag above is fatal. The
//seat cast the body and died from 21. A converter plus a mirror is not a
//SIZED price at all: the first point they gain re-enters the chain, so the
//arithmetic this tag performs is the arithmetic of a board that does not
//exist. With the loop PROVEN closed (the #W62-AA R6 gate, the same predicate
//the LOOP SCOPE paragraph is built from, so the two cannot disagree) the row
//prints the loop verdict and NO resulting-life figure - the D2/E1 shape.
```

### AIPlayerGPT-incomingCombatLine-937  — AIPlayerGPT.cpp, near `incomingCombatLine` (#W57)

```
//#W57-B (D24, wave-56 ledger MED): the header raised the question "can any
//assignment of my blockers get me above 0" and made the seat do the
//subtraction. `123v152` seq 23: 8 life, "4 attackers, 16 unblocked damage -
//you would be at -8; this KILLS you (of that, 10 from 2 attackers none of your
//creatures can block)", one blocker whose only legal assignment removes 3. The
//seat answered BLOCKS: none and was RIGHT (16 - 3 = 13 > 8) while its guide's
//LETHAL rule ordered a block. `bestCaseDamage` is the damage that still lands
//after the best legal assignment (< 0 = not computed, nothing printed);
//`bestCaseOptimal` says whether that number is a proven maximum - a trample
//attacker in the total makes it merely ACHIEVABLE, and the wording drops from
//"best case" to "one legal assignment" rather than over-claiming.
```

### AIPlayerGPT-incomingCombatLine-953  — AIPlayerGPT.cpp, near `incomingCombatLine` (#W61)

```
//#W61-R (C2, wave-60 engine-seat HIGH-1 + deck126 MED-2): the parenthetical
//`(your creatures may legally block every attacker in that total)` is a
//per-attacker legality statement printed as an aggregate promise, and it fired
//over ONE blocker against 47 attackers (`162v123` s18) and at 1-vs-2, 1-vs-3,
//1-vs-6 (`126`'s `146` s9/s17/s22, where it sat in the same sentence as
//`no block saves you`). 32 such renders corpus-wide. This is the exact
//counterpart of what B11 fixed on the attackers side, and the fix is the same
//kind of number: `matchedAttackers` is the size of the maximum legal blocker-
//to-attacker matching over the ENGINE's own pairwise map (the assignment the
//`best case` clause below already solves), so the aggregate claim is made only
//where a full assignment provably exists, and where it does not the line says
//how many of them can be blocked AT ONCE instead of implying all of them.
//`assignableAttackers` is the denominator that claim is about - the attackers
//still unblocked, which is what the matching is built over. Both < 0 means the
//matching was not computed and the old wording is kept.
```

### AIPlayerGPT-if-973  — AIPlayerGPT.cpp, near `if` (#W59)

```
//#W59-I (K5, wave-58 deck162 HIGH-1): and then it inverted the floor. A
//floor on the DAMAGE is a CEILING on the LIFE, so `myLife - bestCaseDamage`
//printed with "or better" asserted the opposite of what it proves.
//`162v152` seq 19, turn 13, 18 life vs 5 attackers for 20: "at least 15 of
//that lands ... you would be at 3 or better" - the seat's actual legal
//block landed it at 0 and it died that combat, on a line that positively
//told it the floor was survivable. Two corrections, both pessimistic:
//the value is worded as the ceiling it is, and a non-positive ceiling is
//PROVEN death, so it carries the same `no block saves you` badge the exact
//branch has (a floor of D at life <= D means no assignment survives).
```

### AIPlayerGPT-L23706-988  — AIPlayerGPT.cpp, near `L23706` (#W63)

```
//#W63-AB (E3b): it is printed on the LETHAL sub-branch too. The
//caller labels it there as the least-damage line, which is what it
//is; withdrawing it left `146v152` seq 28's seat unguided and it
//picked the damage-MAXIMISING block.
```

### AIPlayerGPT-crackBackNextTurnLine-997  — AIPlayerGPT.cpp, near `crackBackNextTurnLine` (#W61)

```
//#W61-R (C3): two additions, both from the same board the count is read off.
//`floorSources` non-empty turns the CEILING into a FLOOR and says so in the
//head clause rather than leaving "for up to" to carry a claim it cannot make;
//the kill clause survives that change because a floor at or past the seat's
//life is still a kill. `evasive*` is the sub-total deck126 MED-1 asked for -
//51 of 51 renders at that seat priced a 20-creature board of which 19 were
//fliers against an all-ground defence and netted nothing. It is stated only
//where the seat HAS bodies, for the reason the INCOMING split uses: with none,
//"nothing you control can block it" is true of every attacker and says nothing.
```

### AIPlayerGPT-crackBackNextTurnDue-1011  — AIPlayerGPT.cpp, near `crackBackNextTurnDue` (#W75)

```
//#W75-CL (P6, deck125 A-2 HIGH). THE PHASE GATE WAS BUILT FOR A DECK THAT
//SPENDS ITS MANA IN A MAIN PHASE. Measured over the wave-74 corpus the line
//rendered on 40 of 40 own-turn Main 1 / Main 2 windows and on 0 of 173 own-turn
//Upkeep windows and 0 of 38 own-turn Draw windows - and deck125 is a draw-go
//control deck that commits its mana in its OWN upkeep, holding instants for the
//window the gate keeps silent. `125v123` seqs 68-73 are six turn-23 Upkeep asks
//at 37 life with FORTY able attackers on the other side of the table and not one
//word about them; seq 79, the Main 1 window six asks later, prints
//`CRACK-BACK NEXT TURN: 40 of their creatures will be able to attack ... for up
//to 41 - you would be at -1; that would KILL you`. The seat is owed that figure
//at every window where it can still answer it, and on its own turn every window
//is such a window (instants, activations and the mana that pays for them are
//live in upkeep, draw, combat and the end step alike). D9's "a line on every
//window is a line nobody reads" discipline is kept where it earns its keep: on
//the OPPONENT's turn the line is still silent, because the crack-back it prices
//is a whole turn cycle away and the seat's own untap comes first.
```

### AIPlayerGPT-w79CoveredByBodies-1032  — AIPlayerGPT.cpp, near `w79CoveredByBodies` (#W69)

```
//#W69-BH (K6a, deck162 MED-1): THE COVER COUNTED ONLY THE ROW'S OWN NEW BODY.
//`162v152` s15 printed "Counting only the checked bodies you cover 5 of 13 ...
//you would be at 4" beside a battlefield holding an untapped Master of the Feast
//(5/5) and an untapped Fate Unraveler (3/4) - two legal blockers the same
//opponent's attack has to get through, and the only survival arithmetic on the
//screen left both of them out; s19 read "at 0" the same way. The bodies already
//standing are the same fact the blockers screen declares from two turns later,
//and the render already knows them. They are folded FIRST (they are certain -
//no cast is required to have them), each body is assigned to at most one
//attacker, and the row's own bodies then cover what is left.
```

### AIPlayerGPT-w81AttackCoverDue-1047  — AIPlayerGPT.cpp, near `w81AttackCoverDue` (#W81)

```
//#W81-DL (V10): WHEN THE COVER PARAGRAPH IS DUE. The wave-64 gate also demanded
//`life - total <= 0`, so the paragraph reached the seat only on the windows where
//the crack-back had already won. The fact it states is true of every attackers
//window that PRINTS the line, so the gate is now exactly the line's own: one
//figure for the cover-clause count and the crack-back-header count on attackers
//prompts. Pure over the two facts.
```

### AIPlayerGPT-blockKeepAlternativeClause-1058  — AIPlayerGPT.cpp, near `blockKeepAlternativeClause` (#W63)

```
//#W63-AB (E3, deck152 HIGH-1/2). Two defects, both about what this clause is
//allowed to leave out. (a) The matching it renders maximises PREVENTED DAMAGE
//and nothing else, so among the many assignments that reach the same number it
//prints whichever the augmenting walk happened to build - `146v152` seq 25
//named `Wolf blocks Barrowin`, a chump, while the same Wolf line offered two
//"(you kill it, your blocker lives)" results, and the seat copied the header
//verbatim and lost the Wolf. `improveAssignmentMaterial` below now hands this
//function a matching that is material-improved at the SAME life figure, and the
//clause states the material fact so the ranking is checkable rather than
//implicit: survive AND keep material reads differently from survive. (b) On a
//lethal screen the clause withdrew entirely and the unguided seat picked the
//damage-MAXIMISING block (seq 28). A least-damage line is still a line: it is
//printed, labelled as what it is, and claims no survival.
//`blockersDying` < 0 means the material was not computed and nothing is claimed.
```

### AIPlayerGPT-blockKeepAlternativeClause-1077  — AIPlayerGPT.cpp, near `blockKeepAlternativeClause` (#W64)

```
//#W64-AG (F8, deck123 HIGH-1 / deck162 HIGH-1 / deck152 MED-1 / deck126 MED).
//Three separate falsehoods in one clause, all of the same class - the label
//claiming more than the computation performed.
//(a) "chosen for your blockers' material as well as for the life" was printed
//UNCONDITIONALLY. Material only breaks ties INSIDE the maximum-life set, so on
//`123v146` s15 it named the pairing that loses the blocker for nothing over the
//one that kills the Goblin and keeps it, and on `152v146` seq 18 it claimed a
//ranking over a single pure chump where there was nothing to rank. The claim is
//now made only where the assignment actually PRESERVES the material (no matched
//blocker dies); where it does not, the clause says the life is what chose it and
//NAMES the bodies it spends, which is the fact `123v146` s11/s15 needed (both
//spent the deck's only token-maker under a live Intruder Alarm).
//(b) it was copied 3 of 3 times as an INSTRUCTION on headers whose own next
//line reads "NOT lethal: block only where the trade favors you". Where the seat
//is not dead if it declines, the clause says so and prints the life declining
//leaves, so the two authorities in the window agree.
//`matchRank` is blockPairMaterialRank's 4/3/2/1 per matched blocker (empty or 0
//= not ranked, and then no material claim is made at all). `declineLife` >= 0
//means the header is NOT lethal and carries the life declining leaves.
//Pure over its inputs, so every wording is provable without a board.
```

### AIPlayerGPT-handCastabilityTag-1102  — AIPlayerGPT.cpp, near `handCastabilityTag` (#W68)

```
//#W68-BB (J5, deck130 HIGH-2): the tag counted UNTAPPED SOURCES and said
//nothing about mana already floating, so `130v126` s29 read `Spark Spray {r}
//[instant] [cannot pay now: needs 1 mana, you have 0 untapped sources]` two
//lines under `Already in pool: {r}{r}{r}{r}{r}{r}{c} (7 mana ALREADY produced
//and floating right now)` - and one line above its own offered `1. Cast Spark
//Spray` row. Measured on the corpus: 74 of 110 floating-pool prompts carried a
//cannot-pay tag (264 tags), 9 of them naming a card the SAME prompt offered a
//Cast row for. The pool is now folded into the affordability test AND named in
//the sentence; with an empty pool (`floating == 0`) every byte is wave 67's.
```

### AIPlayerGPT-getDisplayName-1116  — AIPlayerGPT.cpp, near `getDisplayName` (#W48)

```
//#W48-D7 (wave-47 ledger, MED): plus the LIVE KEYWORD SET, via
//the helper the target previews already use. The battlefield
//line for the same card reads "Perimeter Captain #1 {w} (0/4)
//[defender]"; this line read "(creature 0/4) [spell]" and the
//counter row beneath it the same, so every pool rule keyed to a
//keyword ("do not counter a wall", "kill the flier") was
//unsatisfiable exactly where the decision is made - deck125 spent
//a Cancel on a Perimeter Captain accordingly (125-vs-126 seq 66).
//Same helper as the battlefield render, so the two surfaces
//cannot disagree about what the card is.
```

### AIPlayerGPT-w72CastAnsweredFactApplies-1131  — AIPlayerGPT.cpp, near `w72CastAnsweredFactApplies` (#W81)

```
//#W81-DK (V4): ...and the stack the answer was given over. `152v125` seq 101
//printed this tag on an INSTANT while the opponent's counterspell sat on the
//stack; a stack that has moved since the answer is a different window.
```

### AIPlayerGPT-L29277-1139  — AIPlayerGPT.cpp, near `L29277` (#W47)

```
//#W47 (wave-46 engine seat): floor 1, not 2 - with 2 the FIRST repeat's option
//row was byte-identical to the previous tick's, and a one-option Doomsayer +
//Intruder Alarm loop spun the model for the full 900 s timeout (deck123 vs130
//seq 13); latency collapsed 20 s -> 7 s the moment the tag appeared at seq 14.
```

### AIPlayerGPT-paymentReceiptLine-1148  — AIPlayerGPT.cpp, near `paymentReceiptLine` (#W51)

```
//#W51-D (D17): sources are joined with "; " - a card name may itself carry a
//comma ("Katilda, Dawnhart Prime"), so "A, B, C" did not split back into
//names (10 receipts in corpus 20260827-115759, deck152 vs123 seq 13). The
//semicolon is already the prompt's own list separator ("Those sources, one
//per untapped card: ... ; ...") and no card name contains one.
```

### AIPlayerGPT-priority-1158  — AIPlayerGPT.cpp, near `priority` (note)

```
//
//3,818 of the corpus's 4,128 priority windows offered exactly ONE option, and
//in every one of them the only way to decline was to invent a number that was
//not in the list: "pass" existed only inside the reply-format sentence as
//"0 = pass priority". The house ordering rule (declines go LAST, where the
//model's option-1 bias cannot land on them) was therefore not honoured on any
//single-option window, and the turn-10 measurement is the consequence -
//1,871 windows, 1,869 takes, ZERO passes.
//This adds the row the rule always assumed was there. It removes nothing,
//changes no number the parser already accepts (0 has always been the pass
//index), and states the decline in the same shape as every other row so it can
//be chosen the same way. Scoped to single-option windows: that is where the
//absence was measured and where the row is the whole menu's other half.
```

### AIPlayerGPT-L29715-1176  — AIPlayerGPT.cpp, near `L29715` (#W61)

```
//#W61-U (C14, deck146 HIGH-3): the row's promise was scoped to the TURN, and
//so was the latch behind it - `146v125` seqs 92-101 are ten consecutive asks at
//20.4-21.6 KB each (~210 KB of inference) across Blockers -> Main 2 -> Upkeep ->
//Draw -> Main 1 -> Main 1 on a menu whose only two cast rows were annotated
//`at 0 this does nothing` and `the only legal targets are YOUR OWN right now`.
//Nothing about that menu changed; the TURN did, and the turn boundary retired a
//hold the model had already taken over the identical rows. The latch is now
//retired by the ONLY thing that ever made it unsafe - a row the model has not
//seen - so the row says that instead of the turn. This is not a blind cache:
//holdStillStands re-checks the rendered rows, byte for byte, at every window,
//and a single changed price re-opens the question.
```

### AIPlayerGPT-L29715-1192  — AIPlayerGPT.cpp, near `L29715` (#W71)

```
//#W71-BR (L17, deck123 MED-2): the tail is TRUE at the priority seam and false
//in effect on a CASTING menu, where the row sits beside "Cast nothing right
//now" and the two read as the same decline while one closes the turn's
//remaining casting windows. deck123's own gameend counters:
//`hold_windows_skipped_cast` 343 / 76 / 60 / 44 against
//`hold_windows_skipped_priority` 30 / 0 / 21 / 19 - the skipped windows are
//overwhelmingly CAST windows, and `123v146` seq 17 took the hold row over a
//live Idyllic Tutor row. The row is NOT removed (a decline the pilot may want
//is not the engine's to withhold, and the latch is what stops the identical-run
//loops the row exists for): it says what it costs where it costs it. The HEAD
//is shared by both spellings, so every consumer that identifies the row by its
//head - holdRowIndexOf, isReservedHoldEcho - binds either.
```

### AIPlayerGPT-L29715-1209  — AIPlayerGPT.cpp, near `L29715` (#W73)

```
//#W73-BY (N2 c, N9): the HEADLINE, rewritten. Two facts were true of this row
//and printed nowhere a reader lands: (a) the latch outlives THIS window, so a
//pilot that means "pass once" cannot come back for one of the rows above later
//in the same turn - the clause that said so sat ~600 chars deep in the benefit
//brace (`125` seq 36/101 held over their own +1); (b) with the wave-73 untap
//release the latch now has a hard END, and the row may as well name it. Both
//lead. Nothing is withheld and no row is removed: this is the same row, priced.
```

### AIPlayerGPT-string-1221  — AIPlayerGPT.cpp, near `string` (#W75)

```
//#W75-CI (P19, wave-74 deck146 MED 4 - AND A TRUTH FIX). The row had grown
//to 1,074-1,287 bytes on a 3-row menu, holds were taken in 11 of 322
//decisions, and CC refused a blind cut. The evidence: over the 1,619 corpus
//windows that render this row, the share of `reasoning` bodies quoting each
//clause is - head/"pass now, do not ask me again" 32.7%, "CANNOT COME BACK"
//15.3%, "stands until your next turn begins" 8.6%, "second main phase"
//10.6%; every clause of THIS brace sits at 3.5-5.3%, and 61 records (3.8%)
//echo the WHOLE row, which is the floor those numbers are measured against.
//So exactly one sentence of the brace earns its place on the row: the
//main-1/main-2 span (#W63-AF R2). The rest moves to `holdContractParagraph`
//below the list, once per prompt, where it is also REWRITTEN TRUE: since
```

### AIPlayerGPT-loopChainingNote-1237  — AIPlayerGPT.cpp, near `loopChainingNote` (#W73)

```
//#W73-BY (N5 second half, wave-72 deck162 HIGH). THE BRACKET ONLY WHERE ITS
//ADVICE IS TRUE. Its one actionable sentence points at the HOLD row, and on
//`162v126` seq 26/29 it was printed over a menu whose rows 1-4 were Ob Nixilis
//loyalty activations that do not enter the chain at all - so the seat read
//"the HOLD row answers all of them at once" and surrendered four live plays to
//a chain none of them was about. The bracket is not deleted (the chain fact is
//true and worth its bytes) and no row is removed: when the menu carries an
//ACTING row, the hold-answers-all sentence is replaced by the fact that those
//rows stand on their own. `actingRowsPresent` is read off the same rows the
//menu is about to print, so the sentence and the list cannot disagree.
```

### AIPlayerGPT-L30276-1252  — AIPlayerGPT.cpp, near `L30276` (#W79)

```
//#W79-CZ (T13, wave-78 known bugs T13 / S4 residual). "HOLD covers every link"
//was read as a promise with no end, and `123v125` seqs 379 -> 382 and 395 -> 398
//falsify it: the two windows are byte-identical menus one TURN apart (T51 -> T52,
//T53 -> T54), and the hold between them was retired by the seat's own untap
//release, which is the latch's own rule. The promise is true of THIS stack and of
//nothing beyond it, so it says which stack and names the release.
```

### AIPlayerGPT-w77OwnLoopVerdictKey-1263  — AIPlayerGPT.cpp, near `w77OwnLoopVerdictKey` (#W79)

```
//#W79-CZ's 194 corpus re-opens lived; an unnamed THREATENED (the record stamp's
//own call, which is prompt-side telemetry and enters no latch) is the bare face,
//byte-identical to wave 79's.
```

### AIPlayerGPT-holdNoteSameWindow-1271  — AIPlayerGPT.cpp, near `holdNoteSameWindow` (#W62)

```
//#W62-fix: is this rebuild the SAME window the note was last measured at? The
//prompt is rebuilt every tick while an answer is in flight; a window closes by
//writing a translog record (seq moves). Same seq + no unseen row = same window,
//and measuring it again would count rebuilds as windows (the wave-61 corpus
//printed "132 windows in a row" for one Cleanup window) and move the slot key.
```

### AIPlayerGPT-holdNoteSameWindow-1281  — AIPlayerGPT.cpp, near `holdNoteSameWindow` (#W78)

```
//#W78-CY (F5 a, Astra MED 5): ...AND NO ROW HAS LEFT. The wave-78 delta computes
//`gone` beside `unseen`, and this guard read only `unseen` - so the very sequence
//S9 b was built for (commit `{A,B}`, build an unchanged held window at seq N and
//suppress it, then rebuild `{A}` at the SAME N after B disappears) returned the
//CACHED "no row ... is gone" note over a window where one row had gone. `goneRows`
//is the fifth argument and defaults to 0, so every existing caller and pin reads
//byte-identically.
```

### AIPlayerGPT-w78StripHandleOrdinal-1293  — AIPlayerGPT.cpp, near `w78StripHandleOrdinal` (#W79)

```
//#W79-CZ (T12, wave-78 engine-seat MED-1). THE STRIP OVER-MERGES. It dropped
//EVERY ordinal, so `125v123` seq 263 - whose two rows went from
//`deal 1 damage with Staff of Nin targeting ...` to
//`... with Staff of Nin #2 targeting ...` because a SECOND Staff had entered and
//the first one's rows were no longer offered - normalised to the same key as seq
//260's and the bracket printed `every row above was also on the menu ... and no
//row that was on it is gone`. The rows address a DIFFERENT PERMANENT; the
//bracket's own contract says a row "naming a different card" re-opens a hold.
//Only `#1` is the renumber artefact: Wagic prints no ordinal on a lone copy and
//appends `#1` to it the moment `#2` arrives, so ` #1` and no-ordinal are the SAME
//instance and every higher ordinal is a different one. Stripping ` #1` alone
//keeps wave-78's fix for the renumber-in-place case (one row -> `#1` + `#2` reads
//as exactly one new row) and restores the identity wave 78 lost.
```

### AIPlayerGPT-L30774-1311  — AIPlayerGPT.cpp, near `L30774` (#W76)

```
//#W76-CQ (F1, Astra wave-76 finding 1): but STAGING is not measuring, and
//this return used to skip it. The sequence the review names is real:
//{A,B} is asked and committed; an identical held window is built at the
//next seq, measured, then SUPPRESSED (w76HoldWindowNotAsked erases the
//pending entry while measuredSeq stays put); B then leaves the board and
//`{A}` is built at that same seq - zero unseen rows, same seq, so this
//return fired and staged nothing. The changed menu earns a record, and
//w76HoldWindowAsked finds no pending entry to commit. `last` still holds
//{A,B}, so when B comes back the bracket calls it "also on the menu at
//the last window I asked you at this seam" - a window that contained only
//A. COMMIT ON RECORD, NOT ON NOTE REUSE: the rows being built right now
//are what a record would commit, so they are staged here too, under the
//run the reused note actually claims.
```

### AIPlayerGPT-L31365-1329  — AIPlayerGPT.cpp, near `L31365` (#W64)

```
//#W64-AJ (deck130 MED-4; 125 seqs 72-74). THE SECOND CLAUSE THAT IS NOT A ROW
//CHANGE. `[this cannot target the spell on the stack - battlefield permanents
//only]` is appended to a target row only while an OPPONENT SPELL IS ON THE
//STACK, so the same window rebuilt after that spell resolves prints the same
//rows minus one bracket - and the latch retired on a menu that had not moved,
//re-asking a question the seat had already answered "hold" to (125 seqs 72, 73,
//74, byte-identical rows). The clause states what the row CANNOT reach, which
//is a fact about the stack and not about the row's content or price: the row
//offers exactly the same objects with or without it, so removing it from the
//latch's identity key forgives nothing the model relies on. It is the same
//shape as #W63-AD E10's phase clause and is bounded the same way - two named
//clauses, nothing else, every price and every other bracket still compared byte
//for byte. Shared choke point checked: `holdKeyRow` is read by `takeHold` and
//`holdStillStands` and by nothing else, and `optionSetKeyLine` (the
//decline/deadlock key) already drops every bracket through
//`stripRenderAnnotationsLc`, so no ask key and no async slot key changes here.
```

### AIPlayerGPT-w79SplitOrdinals-1350  — AIPlayerGPT.cpp, near `w79SplitOrdinals` (#W79)

```
//#W79-CZ (T12) kept every ordinal above one as identity, so that window read as a
//new row and a gone row and re-opened a hold with no new legal action.
//The two cases the ordinals have to separate are: an earlier copy LEAVING (every
//rank slides DOWN, the row count for that shape is unchanged) and a new copy
//ARRIVING or a different instance taking over (a rank goes UP, or the count moves).
//That is decidable from the two row lists alone: for a row SHAPE - the key with
//every ordinal blanked - carrying the same number of rows in both windows, a
//pure DECREASE of its ordinals is a renumber, and the now-rows are realigned onto
//the held ones. Anything else is left exactly as #W79-CZ wrote it, so `125v123`
//seq 263 (no ordinal -> ` #2`: a different SHAPE, an ordinal going UP) still reads
//2 new / 2 gone, and the renumber-in-place case still reads 1 new / 0 gone.
```

### AIPlayerGPT-w79NumberedRowBody-1366  — AIPlayerGPT.cpp, near `w79NumberedRowBody` (#W79)

```
//#W79-CZ (T3, wave-78 engine-seat HIGH-2 - THE CG SHAPE, A FOURTH WAVE). The
//comment above ends "`[...]` notes were already outside the key". THEY ARE NOT.
//`w76StripBalancedAnnotationGroups` takes `{...}` groups only, and the corpus's
//own re-asked pairs name the byte: `123v126` seqs 83->86 (cast seam, turn 13) are
//byte-identical apart from `[this cannot target the spell on the stack -
//battlefield permanents only] ` appearing on row 1 - a RENDER note about the
//board, in the ask key and the async slot key, minting a fresh question for a
//menu whose actions had not moved. 186 of the corpus's 1,454 same-seam same-turn
//re-asks are exactly this shape.
//The fix is not another named strip. The KEY of a window is its ACTION-KEY SET,
//and the seat already owns the builder that says what an action is:
//`holdActionKeyRow` - every annotation group out (`[...]` and `{...}`, balanced),
//the target preview's numbers out, life projections normalised, and the
//`[cost: ...]` group kept, so a cost change is still a different question. So
//every NUMBERED ROW of the tail goes through it, and nothing else in the tail is
//touched (the question head - `Land drop: ` - and the closing instruction are
//left byte-exact, because `asyncLandArm` reads the head and the closing sentence
//is the protocol's).
```

### AIPlayerGPT-w73HoldExpiredByUntap-1389  — AIPlayerGPT.cpp, near `w73HoldExpiredByUntap` (#W73)

```
//#W73-BY (N2 a, THE HOLD FAMILY - wave-72 deck152 HIGH-1, deck123 HIGH-2,
//deck125 HIGH A-2). Four seats lost turns or games to ONE mechanism: the
//latch's only release is "a printed row changed", and a ONCE-PER-TURN row is
//byte-identical every turn it is offered. A loyalty ability, a {T}: create
//token, a repeat activation - each renders the same string on turn 13 and on
//turn 25, so the release condition cannot occur while the maker sits there and
//"pass this window" silently became "concede this permanent for the game"
//(`152v162` seq 34 silenced Teferi for 6 turns and the seat died at 0;
//`123v130` seq 31 held a Create row for 15 turns; `125v130` seq 46 skipped
//turns 27-30 at 18 life and the seat died at -1).
//The release is now a CLOCK the rows cannot stop: a hold ends at the start of
//the holder's next turn. Nothing is removed by this - a released hold means the
//model is ASKED again, which is the permissive direction - and the model may
//simply take the row again over the same board. Pure over (the turn the hold
//was taken, the turn now, whether this seat is the active player now).
```

### AIPlayerGPT-reserveDeclineStillStands-1409  — AIPlayerGPT.cpp, near `reserveDeclineStillStands` (note)

```
//
//`162v130` seq 16 declined a Dictate of Kruphix row whose own `{reserve:}`
//clause said the mana it spends is the last window Underworld Dreams {b}{b}{b}
//has this turn; seq 17 put the identical question back inside the SAME draw step
//(one count inside a `{feeds:}` clause had moved), and the model cast the row it
//had just reserved against. seq 18 -> 19 repeated it at 1 life with an exactly
//lethal second punisher in hand, and that is the game.
```

### AIPlayerGPT-reserveDeclineStillStands-1421  — AIPlayerGPT.cpp, near `reserveDeclineStillStands` (note)

```
//
//WHY NOT `holdStillStands`. The wave-66 review read `162v130` s16 -> s17 as "one
//count inside a {feeds:} clause had moved" - it is NOT. Measured on the corpus's
//own `options_text`: s16 -> s17 the row gains `in your hand: 1 - Liliana's Caress`
//where s16 read `in your hand: 0` (a card NAME, a word), and s18 -> s19 gains a
//whole `{spends 3 of your 4 untapped mana sources this turn; ...}` clause. The
//hold key re-opens on both, correctly - so a hold-shaped latch would not have
//held either pair, and shipping one would have been a fix that fires on nothing.
```

### AIPlayerGPT-reserveDeclineStillStands-1434  — AIPlayerGPT.cpp, near `reserveDeclineStillStands` (note)

```
//
//MEASURED on the corpus this was shipped for (162v130 s16->s17, s18->s19):
//under the full key NEITHER pair latches - s16->s17 the seat's hand went 5 -> 6
//cards, its library 46 -> 45 and a Howling Mine trigger left the stack; s18->s19
//the hand went 7 -> 9, the library 43 -> 41 and two of three stack triggers
//resolved. The board moved in both, so both windows are re-asked, which is what
//an unchanged-board latch means. The wave-67 latch held them by not looking.
```

### AIPlayerGPT-L34355-1446  — AIPlayerGPT.cpp, near `L34355` (#W71)

```
//#W71-BR (L17, deck123 MED-3): `kRepeatActivationFloor` is 1, so the annotation
//"[repeat: you control 5 creatures; activated this turn 1 times already. This
//turn will not advance while you keep taking this option.]" printed on the
//plain row at `123v130` seq 18 while the batch row this floor gates did not
//exist until seq 23/24. The seat spent seq 18-22 - five windows, ~9 minutes of
//wall clock - tapping one at a time under a note telling it the turn would not
//advance, with no row that could end the loop. One threshold for one fact: the
//window that says "you have done this before" is the window that offers the
//batch. The ceiling stated in the row is real - N is clamped to it at parse.
```

### AIPlayerGPT-menuRowShortName-1460  — AIPlayerGPT.cpp, near `menuRowShortName` (#W69)

```
//#W69-BF (K3): the row's SHORT NAME - the option text up to the first "{", "["
//or "(", with the leading verb dropped. This is exactly the string the protocol
//tells the model to copy into its CHOICE parenthetical, and it is what the model
//writes when it talks about the row in prose. It is NOT menuRowProseName: that
//helper strips annotations first, and on a row carrying a NESTED brace group
//("{spends 4 ... Devour Flesh {1}{b} in your hand needs 2}") the strip stops at
//the inner close and leaves annotation fragments glued to the name - measured on
//123v162 s34's row 5, where it yields "damnation in your hand needs 2}" and can
//match nothing. Cutting at the first bracket cannot nest.
```

### AIPlayerGPT-duplicateVerdictTag-1474  — AIPlayerGPT.cpp, near `duplicateVerdictTag` (#W57)

```
//#W57-C (D30, wave-56 ledger LOW): the same window one card apart. `123v125`
//seq 45 listed `Cast Devour Flesh {1}{b}` and `Cast Tribute to Hunger {2}{b}`
//reading the byte-identical verdict `they control 0 creatures - at 0 this does
//nothing`; D15's marker keys on the card NAME, so it saw two different cards
//and said nothing (0 renders all corpus). The claim this tag makes is narrower
//than D15's and is exactly what is provable: the two rows carry the SAME
//rendered verdict on THIS board at two prices. It does not say the cards are
//interchangeable. Pure.
```

### AIPlayerGPT-boardSweepMarker-1487  — AIPlayerGPT.cpp, near `boardSweepMarker` (#W66)

```
//#W66-AQ (H10, deck130 HIGH-3): the count is a count of BODIES, and
//`130v123` seq 44's THEIRS-1 was Bloodline Keeper under Intruder Alarm - one
//body by the count, an unbounded token loop on the board. The tally is not
//re-weighted (no invented value ranking): the ENGINES it removes are NAMED, and
//the "more of YOURS than of THEIRS" comparison says out loud that it is
//comparing bodies. With no engine among THEIRS the string is byte-identical to
//wave 65. Pure over its four inputs.
```

### AIPlayerGPT-boardSweepMarker-1499  — AIPlayerGPT.cpp, near `boardSweepMarker` (#W80)

```
//#W80-DF (U7, wave-79 deck123 HIGH-1). THE MARKER ARGUED ONE SIDE. `123v162`
//seq 42: `[<- board sweep: THEIRS 2 (including Fate Unraveler - a DRAW PUNISHER
//...)) / YOURS 1 ...]` - the row's own clause named the seat's loss (Thraben
//Doomsayer, a token engine) three brackets earlier, and the marker, the one
//surface the corpus shows the pilot obeys, reduced it to the integer 1. seq 74
//is the same shape with Bloodline Keeper. The marker is a comparison, and a
//comparison that names one side's contents and counts the other's is an
//argument, not a fact: the seat cast Damnation into its own engine and died two
//turns short. Same walk, same predicate, same wording - the YOURS half now
//names its bodies and flags any engine among them, and the BODIES-NOT-VALUE
//sentence fires when EITHER side loses an engine (it is the same caution in
//both directions). With no engine and no roster on either side every byte is
//wave 66's.
```

### AIPlayerGPT-L36976-1517  — AIPlayerGPT.cpp, near `L36976` (#W81)

```
//#W81-DM (V13, wave-80 deck126 MED-3). THE THIRD DEAD SHAPE. `everyCastRowDead`
//asks each row for a `{right now: ...}` verdict, so a cast row that this engine
//already marked dead with a BRACKET TAG instead of a verdict made the whole
//header go silent. `126v130` seqs 15, 21, 22, 32 and 38 are the repro: seq 32's
//entire menu is `1. Cast Idyllic Tutor {2}{w} [finds only an enchantment card -
//every enchantment left in your library is a copy of one you already control or
//hold: Sanguine Bond]`, a hold row and a decline row, and seqs 22/38 add
//`2. Cast Exquisite Blood {4}{b} [second copy: ... a second instance of an
//effect you already have]`. The marker printed on NONE of the five while the
//same marker printed 25 times at that seat on menus dead in exactly the same
//way; the seat had to classify them itself and got 3 of 5 right.
```

### AIPlayerGPT-allCastRowsDeadNote-1533  — AIPlayerGPT.cpp, near `allCastRowsDeadNote` (#W81)

```
//#W81-DM (V13): `anyTagDead` widens ONLY the opening clause, and only on a menu
//that actually carries a tag-dead row. A menu whose every row is verdict-dead -
//the 25 renders that already worked - is byte-identical to wave 80.
```

### AIPlayerGPT-removalVictimTag-1541  — AIPlayerGPT.cpp, near `removalVictimTag` (#W55)

```
//#W55-C (D15): the list is a CONSEQUENCE label, and it named the caster's own
//permanents with no ownership mark - `130v146` s23 rendered
//`Cast Stone Rain {2}{r} {kills: Plains #1, Plains #2, Mountain #1 ... #5}` on
//a seat whose whole plan is land destruction; five of the seven were its own
//Mountains and the spell destroys exactly one. The target menu one screen later
//marks every row `[your battlefield]`; this row did not. When nothing of the
//caster's is on the list the string is BYTE-IDENTICAL to before, so the common
//case does not move. The cardinality is deliberately NOT asserted ("whichever
//you target", not "whichever ONE"): the single-payload rail this tag rides does
//not by itself prove a one-target `target=` line.
```

### AIPlayerGPT-forcedSacrificeRowTag-1556  — AIPlayerGPT.cpp, near `forcedSacrificeRowTag` (#W76)

```
//#W76-CO (Q6, wave-75 deck162 HIGH). THE ROW STATED HALF THE PRICE.
//`162v126` deck162 seq 10 (t11, 18 life vs 21) offered two Shield Spheres
//("they gain 6 life (its toughness)") and a Fog Bank ("they gain 2") under a
//header whose tie-break is "the one that pays the least" - with Sanguine Bond
//on THEIR battlefield, and the prompt's own LIFE-TO-DAMAGE CONVERTER paragraph
//twenty lines above saying so. The 6 life they gain is 6 off the SEAT's total,
//and no row said it: the seat was ranking a price whose larger half was not
//printed. Same construction #W53-O (D13) already uses on the edict row - the
//converter is already scanned, the gain is already on the row, so the row
//states the whole trade rather than the half it can compute alone. `convTakes`
//is `lifeToDamageConverterTake`'s answer for THIS toughness (0 = no converter
//of theirs, or one whose amount is not knowable here), so nothing is guessed.
```

### AIPlayerGPT-modeRowAnnotations-1573  — AIPlayerGPT.cpp, near `modeRowAnnotations` (note)

```
//
//What the wave-64 corpus measured: 3 real modal windows (Silverquill Command),
//23 rows, ZERO annotations of any kind - not F6's price, not the wave-60
//liveness tag, not the wave-60 tap/untap tag - while the header named the card.
//The engine seat located the silence at the `ctx`-gated family and could not
//run the binary; the code says why, twice over.
```

### AIPlayerGPT-modeRowAnnotations-1584  — AIPlayerGPT.cpp, near `modeRowAnnotations` (note)

```
//
//(1) `ctx` is `req.contextCard`, and DecisionManager::buildMenuChoice sets it
//from `nameableCardPointer(validatedCardPointer(currentActionCard))`. An
//`auto=choice name(...)` list arms its menu while the SPELL RESOLVES, so its
//subject sits in no game zone and the dangle rail refuses the pointer - which
//is exactly why #W48 D6 added the arm-time NAME. So on the one card class these
//annotations were written for, `ctx` is NULL by construction.
//(2) That same class does not even arrive as CHOOSE_MODE - MEASURED, not
//inferred. DecisionManager::buildMenuChoice enters its multiple-choice branch
//only `if (object->abilitiesMenu->isMultipleChoice && object->currentActionCard)`.
//A probe run of a scripted Silverquill Command cast (temporary DebugTrace at
//both ActionLayer arm sites, removed after the reading) reports the menu arming
//through `setMenuObject` with **isMultipleChoice = 0**, six times over the two
//modes. So the list falls through to the REGULAR menu path and arrives as
//CHOOSE_MENU under the header "Choose an option for <card>:" - which is the
//header the wave-64 records print, byte for byte, while "Choose one mode for
//<card>:" appears in none of them. The wave-64 tags rode the CHOOSE_MODE branch
//alone and could not have reached those windows with any pointer at all.
//The same probe reports menuObjectText populated at that site (1717 bytes of
//Silverquill Command's script), so the snapshot below is not a hypothesis.
```

### AIPlayerGPT-w76StatedLethalOnScreen-1609  — AIPlayerGPT.cpp, near `w76StatedLethalOnScreen` (#W76)

```
//#W76-CO (Q3, wave-75 deck146 HIGH-1 + MED-2, deck125 B-2). ONE PREDICATE FOR
//"THIS SCREEN ALREADY STATES A LETHAL", because two computations of the same
//question are two chances to disagree with each other on one prompt - which is
//exactly what happened twice in the wave-75 corpus:
// (a) the blockers header printed `NOT lethal: block only where the trade
//     favors you` on three windows (`146v126` seqs 64 and 70, `162v126` seq
//     13) whose own INCOMING block, six lines above, refused to make ANY
//     survival claim because both halves of the opponent's life LOOP were in
//     play. Under that loop the seat's own unblocked damage is an entry into a
//     chain that does not stop at 0, so "NOT lethal" is false and the
//     take-the-damage-while-ahead hint that rides it is the inverse of correct
//     play. deck146 lost that game from 20 life.
// (b) the own-clock tag promised `the opponent reaches 0 in N more turns` on
//     eight windows (`125v123` seqs 381-395, t77) six lines under a
//     `CRACK-BACK NEXT TURN: ... you would be at -84; that would KILL you`.
```

### AIPlayerGPT-modalModesTag-1629  — AIPlayerGPT.cpp, near `modalModesTag` (#W66)

```
//#W66-AQ (H9): the census gained a THIRD bucket. A `choice` line whose two
//halves are one live and one dead is not a dead mode - it still does the live
//half - and calling it dead is what let `146v162` seq 24 read the mode ask as
//"whole modes" and take one. The bucket is OMITTED when empty, so a board with
//no half-dead pair renders exactly as wave 65 wrote it.
```

### AIPlayerGPT-L39917-1639  — AIPlayerGPT.cpp, near `L39917` (#W70)

```
//#W70-BK (C4): RE-MEASURED for the two-line reply, and NOTHING MOVED -
//the brief expected these to collapse under 200 tokens and the corpus
//says otherwise, so the numbers stand and the expectation is what was
//wrong. Over matchups-20260906-134120 (2,282 replies), the bytes of the
//PLAN line PLUS the action line alone - everything else in the reply
//discarded - are p99.5 ask 2,286 B, priority 1,943 B, attackers 2,416 B,
//blockers 579 B, discard 492 B, reveal 384 B, bottom 623 B. At the
//worst-case 3.15 B/token that is 726 / 617 / 767 / 184 / 156 / 122 / 198
//tokens: the three big seams need ESSENTIALLY THE WHOLE 768 they already
//have, because today's PLAN line is itself a 2 KB number ledger. Wave 70
//lane BL rewrites the PLAN into a short action SEQUENCE, which should
//move this distribution hard - but that corpus does not exist yet, and
//sizing a cap on a rewrite's PREDICTED effect is exactly what went wrong
//in wave 68 (lane BA sized on the previous corpus while the same wave
//moved the distribution). Re-fit these from the FIRST two-line corpus,
//not from this comment. What DID change this wave is what the number
//means: it is now an ANSWER ceiling (gptResolveMaxTokens) and is added
//to the reasoning budget, so no value here can bound a thinking window.
```

### AIPlayerGPT-L39917-1662  — AIPlayerGPT.cpp, near `L39917` (#W71)

```
//#W71-BO (R3 / wave-70 LOW-3, the OWED RE-FIT). Measured on the corpus these
//caps produced under reasoning-on - matchups-20260906-224849, 2,118 replies
//with a body, every one re-parsed for the bytes of its PLAN line plus its
//action line (which is the WHOLE reply now: 94.6% of replies are exactly those
//two lines, and the full-reply p99.9 equals the two-line p99.9 at every seam
//but discard, where it is 34 B larger). Wave 68 sized these on a PREDICTION of
//what the two-line rewrite would do; this is the measurement it was owed.
```

### AIPlayerGPTTransport-gptPyStripFrom-1674  — AIPlayerGPTTransport.cpp, near `gptPyStripFrom` (#W75)

```
//#W75-CM (F6, Astra wave-75 review finding 6): and str.strip() is UNICODE-
//aware. A trace ending in U+00A0 (or U+3000, U+2028, U+0085 ...) survived
//this ASCII-only trim, the template removed it, and the rendered prompt no
//longer contained the final message byte for byte - the same 400 this helper
//exists to prevent. The set below is exactly Python's str.isspace().
```

### AIPlayerGPTTransport-asyncSlotDriftKind-1684  — AIPlayerGPTTransport.cpp, near `asyncSlotDriftKind` (#W57)

```
//#W57-A (D5): the drop names its ARM and which half of the slot key
//moved. The wave-56 residual (21 drops whose next ask was a
//`Casting decision`) had to be classified by reading the NEXT line
//of a 40 KB stderr; this states it on the drop itself, so the next
//corpus can separate a genuinely moved board from a second
//cross-arm displacement without an archaeology pass. The leading
//literal is UNCHANGED - every existing census keys on it.
```

### AIPlayerGPTTransport-L1694-1696  — AIPlayerGPTTransport.cpp, near `L1694` (#W52)

```
//#W52-G (D-1): the streak is a property of ONE board state. The
//corpus livelock (deck123 vs deck126 seq 48) was six drops across
//six DIFFERENT boards - a repeat plan adding a token per tick - and
//the breaker handed a moving, healthy loop to the heuristic (Baka
//cast Devour Flesh mid-loop; the model then targeted itself). A drop
//on a moved board restarts the count; only drops on the SAME board
//accumulate, which is exactly the Kaya-menu shape this was built for.
```

### AIPlayerGPTTranslog-AIPlayerGPT-w81InvalidateHeldAnswer-1708  — AIPlayerGPTTranslog.cpp, near `AIPlayerGPT::w81InvalidateHeldAnswer` (#W81)

```
//#W81-DK (V3, wave-80 known-bugs V3 / engine-seat HIGH-1). A RE-OPEN THAT THE SAME
//LATCH IMMEDIATELY RE-CLAMPS IS NOT A RE-OPEN. `125v162` seqs 18/20/22/24: four
//`reopen_new_lethal` events on one `window_seq` 14, no prompt-bearing record after
//seq 17, seat dead at 0. The re-open DID retire the latch (`mHoldRows.erase`), the
//window WAS re-put - and `askModel` then found the window's own key in the ask
//cache and re-served the HOLD ROW with no round trip and no `-> chose` line, so
//`takeHold` ran again on the seat's own retained answer. DH F3 shipped the event
//and no behaviour. So a re-open invalidates the RETAINED ANSWER for that window's
//key before the hold check can read it again: the re-opened window goes to the
//model. Nothing is removed from any menu and nothing is auto-answered.
```

### AIPlayerGPTTranslog-L1093-1723  — AIPlayerGPTTranslog.cpp, near `L1093` (#W78)

```
//#W78-CV (S7, wave-77 engine-seat M3). A RECORD THAT SHOWED NO BRACKET IS NOT
//THE STAGED WINDOW'S RECORD. `146v126` records 46 and 55 are DUNGEON-ROOM menus
//(`kind: ask`, window kind `unclassified` - the same class every casting window
//carries, so the record's words cannot separate them): each staged a casting
//note, COMMITTED it, and rendered no hold-check line, so records 47 and 56 named
//as "the last window I asked you at this seam" a window that carried no casting
//question at all. The prompt about to be logged is the only authority on what
//reached the model: if the staged window measured a bracket and this prompt does
//not carry one, this record is not that window's - the stage is DROPPED (the
```

### AIPlayerGPTTranslog-if-1737  — AIPlayerGPTTranslog.cpp, near `if` (#W56)

```
//#W56-C (D12): the wave-55 triple was stamped on 12 of 12 reveal records
//with `reveal_stall_secs` equal to `latency_ms / 1000` on all twelve and
//`reveal_stall_phase` constant 0 - a duration duplicating one the record
//already carried, under a name that claimed a park nothing had selected.
//Split it. The WAIT is what every reveal has (ticks the driver spent with
//no structural progress while the model was answering) and is named as a
//wait; `reveal_stall` is now a BOOLEAN present only when the driver's own
//structural threshold - the same numbers its force-close uses, evaluated by
//the driver - had actually been crossed, and the phase rides that case
//alone, where it is the diagnostic it was built to be.
```

### AIPlayerGPTTranslog-L2049-1752  — AIPlayerGPTTranslog.cpp, near `L2049` (#W71)

```
//#W71-BP (L1/L2, engine-seat HIGH-0/HIGH-1): the livelock instrumentation.
//A hung seat used to be INVISIBLE here - the wave-70 hang's seat log simply
//stopped at seq 55. These four say how much of this game was answered
//without asking, how often that had to be refused, how often a stalled menu
//had to be broken by the pass floor's no-progress arm, and how many face
//declines were made sticky. All four are expected 0 in a healthy game.
```

### AIPlayerGPTTranslog-L2181-1763  — AIPlayerGPTTranslog.cpp, near `L2181` (#W82)

```
//#W82-A R1 (LEDGER v2): restored, all three - `hold_events` was 180 and
//`hold_reopened_new_lethal` 7 in the wave-80 corpus, so the "zero
//corpus-wide" half of the deletion rule was simply wrong about them.
```

### AIPlayerGPTParse-L190-1771  — AIPlayerGPTParse.cpp, near `L190` (note)

```
//
//The expectedLabel filter is stale-echo family B (wave-22). deck49 vs35 s18: a
//5.7k-char CHOICE deliberation contained the combat-math line "Attack: Regent
//(6/6) vs Striking Sliver (2/2, First Strike)..." AFTER its answer-first
//"CHOICE: 1 (...)". The unfiltered scan kept that later "Attack:" line as the
//"answer", handing parseChoice reasoning prose ("Regent (6/6) vs ... 2/2 ...")
//whose trailing 2/2 parsed to option 2, which then matched no echoed option and
//was dropped as stale_echo - discarding a valid, in-range CHOICE. A CHOICE ask
//can only be answered by a CHOICE: line; combat labels in its body are CoT.
```

### AIPlayerGPTParse-L190-1785  — AIPlayerGPTParse.cpp, near `L190` (W36)

```
//W36 lane-B item 3 (deck36 F2 s39): last-wins is the SELF-CORRECTION rule (a
//later answer after intervening reasoning supersedes), but a CONSECUTIVE block
//of CHOICE: lines is the other disease entirely - the model listing SEVERAL
//picks line-by-line for a single-pick ask ("CHOICE: 9 (Pest #3)\nCHOICE: 7
//(Island)\nCHOICE: 6 (Seat)"). There the FIRST line is the committed intent
//(rung 1 of its own ladder) and taking the last executed a pick the model
//ranked third. So: within an unbroken run of adjacent CHOICE lines the run's
//FIRST line is the answer; a CHOICE line separated by any non-blank content
//still supersedes (self-correction keeps line precedence). Scoped to CHOICE:
//only - combat labels keep pure line precedence (W32-N122d relies on it).
//choiceRunLen (out, optional) reports the winning run's length so the caller
//can stamp the multi-answer shape into the translog.
```

### AIPlayerGPTParse-L190-1802  — AIPlayerGPTParse.cpp, near `L190` (#W63)

```
//#W63-AD (E6b, deck146 HIGH-1 / deck126 HIGH-1 / engine HIGH-7). THE BOUND THE
//PROTOCOL ALREADY CLAIMS. kReplyProtocol has said since #W62-Z that "an answer
//written inside your PLAN sentence is part of the plan, not your answer, and is
//NOT read as one", and #W62-Z D10 put that bound on choiceRetractedNoReplacement
//- but THIS function is the seam that actually selects the answer, and it had
//no bound at all. Its walk is last-wins over the whole reply, so a coded line
//emitted inside a multi-KB PLAN/CoT paragraph became the answer directly, before
//any retraction machinery ran: 146v162 seq 41 (`latched_coded_line: 2`,
//`post_plan_overrun: 2803`) and the deck126 seat's seven latches are that walk,
//not the retraction path (which returns false outright when the last coded line
//sits past the PLAN marker). The protocol was true about the engine's intent and
//false about the engine.
//[planStart, planEnd) is the PLAN BLOCK as planBlockEndOffset defines it - the
//same span mCurrentPlan carries - and a label line that STARTS inside it is
//skipped. The exclusion only ever DEMOTES: the caller runs the bounded walk
//first and falls back to the unbounded one when the plan block holds the reply's
//only coded line, so no reply that used to parse stops parsing and no window is
//lost. npos/npos = the unbounded walk, byte for byte.
```

### AIPlayerGPTParse-L190-1825  — AIPlayerGPTParse.cpp, near `L190` (#W65)

```
//#W65-AO (G8, deck146 HIGH-2 / deck130 HIGH-1,2 / deck123 MED). THE ANSWER RULE,
//WRITTEN ONCE. This walk was LAST-WINS, and kReplyProtocol said so ("The LAST
//answer line you write is the one that runs"). What that buys on a model that
//keeps writing after its answer is in the wave-64 corpus: 146v130 seq 93
//answered `CHOICE: 2 (Cast nothing right now)` on line 1 and the engine ran a
//`CHOICE: 1 (Cast Acererak the Archlich)` written 6,544 bytes later inside the
//deliberation (`answer_replaced: true`, `plan_block_end: 735`,
//`latched_line_in_plan: false`); seq 101 and 130v162 seq 57 are the same shape -
//3 of 3 replacements flipped the executed row, two of them on a final turn.
```

### AIPlayerGPTParse-gptcaveat-planCarryComposeSteps-1839  — AIPlayerGPTParse.cpp, near `gptcaveat::planCarryComposeSteps` (#W60)

```
//#W60-M (B13a): then the LENGTH bound, with a stated marker where it
//cut. The old ceiling was 1,600 characters, which is not a bound on a
//deliberation stream: 148 of one wave-59 seat's 410 echoes were over
//400 characters and the 1,236-character one produced an off-menu
//CHOICE. kPlanCarryMaxChars is the plan the NEXT decision is asked to
//act on, not a transcript of how this one was reached.
```

### AIPlayerGPTParse-rowNamesALiveKill-1850  — AIPlayerGPTParse.cpp, near `rowNamesALiveKill` (#W71)

```
//#W71-BQ (L3, wave-70 deck123 HIGH-1): the two halves of the fix, both pure.
//(a) A NAMED KILL is a live verdict. `{kills: Elite Spellbinder, Intrepid
//Adversary}` is the engine's own conclusion that this row removes those bodies;
//no phrase anywhere else on the row can outrank it. The zero-count forms carry
//a DIFFERENT tag (`{kills 0 of the 2 CREATURE targets at ...}`, `{kills
//nothing: ...}`), so only the `{kills: ` opener with a non-empty payload
//counts, and `- INDESTRUCTIBLE, destroy does nothing: <names>` INSIDE that tag
//is a note about the bodies it does NOT name, not about the ones it does.
```

### AIPlayerGPTParse-appendParseNote-1863  — AIPlayerGPTParse.cpp, near `appendParseNote` (#W66)

```
//#W66-AS (deck123 MED): ...but NOT when the named row is UNIQUE on
//this menu, which the HOLD row is by construction (holdRowIndexOf
//finds the one row carrying its reserved text). A unique name IS an
//answer: it names exactly one row and the engine can take no other.
//All THREE `index_name_conflict` re-asks in the wave-65 corpus were
//this shape (`123v162` seqs 29, 68, 111, every one `CHOICE: 2 (Hold
//priority)` over a three-row menu) and all three recovered to the
//hold row - three full round trips to be told again what the reply
//had already said unambiguously. The divergence is still STAMPED, so
//it is not the silent name-wins of wave 64; only the round trip goes.
```

### AIPlayerGPTParse-isExampleEchoLine-1878  — AIPlayerGPTParse.cpp, near `isExampleEchoLine` (note)

```
//
//Belt-and-suspenders for the protocol-example leak (deck133 vs131 s21 class,
//wave-24 HARNESS/PARSER). The wave-25 de-fang made the example card name
//(kExampleFakeCardLc) NON-LIVE, so such a line already resolves stale TODAY
//(its foreign echo name matches no offered option -> echoStaleForIndex). But
//that closure is trigger-removal, not a parser guarantee: were a FUTURE example
//text to again collide with a live option name (exactly the wave-24 shape, where
//the old example "Cast Fatal Push" equalled a real option), a line-anchored
//parrot of it would be TAKEN. Excluding it structurally, in the same line-start
//coded-index walk that drops template placeholders, makes the primary resolution
//independent of the de-fang. Scoped to a parenthetical that names the example
//card so an incidental "for example card advantage" in prose cannot trip it, and
//a false drop only routes to the (safe) heuristic.
```

### AIPlayerGPTParse-fetchLandColorsClause-1896  — AIPlayerGPTParse.cpp, near `fetchLandColorsClause` (#W57)

```
//#W57-E (D19, wave-56 ledger MED): the clause had only a NEGATIVE form, so a
//guide rule keyed to the parenthesis is silent exactly where both colours are
//available - `123v126` seq 6 had 5 fetch windows, 4 tagged, and the untagged
//one cracked Marsh Flats for a Swamp over a Scrubland that adds {W} or {B}. The
//positive half states the SOURCE COUNT the seat already has of each colour the
//row adds, which is what separates a redundant crack from a colour-fixing one.
//`sourceCounts` may be NULL (the count is then simply not stated); the negative
//form is BYTE-IDENTICAL to before, because guides key on it.
```

### AIPlayerGPTSeams-L379-1909  — AIPlayerGPTSeams.cpp, near `L379` (#W67)

```
//#W67-AY (I6, deck123 HIGH-4): read from the PERSISTED store, not from the
//carry. The clause was built for the 20 windows that took a count past a
//stated stop and reached exactly ONE of them, because mCurrentPlan is
//cleared by a refusal and by the caveat gates (5 of 31 carried after a
//refusal, vs 288 of 337 otherwise) - so the window that most needs the
//verdict is the window least likely to carry it. The store is fed from the
//same reply PLANs the refusal reads, so both halves now state one number.
//The carry stays as the fallback for a plan folded before the store existed.
```

### AIPlayerGPTSeams-L566-1922  — AIPlayerGPTSeams.cpp, near `L566` (#W55)

```
//#W55-A (D4): offered on EVERY window, the seat's OWN turn included. The
//wave-53 exclusion reasoned that "on the seat's own turn the board changes
//with every land drop and cast, so a hold there would be taken and retired
//in the same breath"; 126v125 seq 72-121 refutes it - 50 byte-identical
//own-turn Blockers windows, every reply a decline, no board change at all,
//and no row to close them with. #W56-A (D1): the hold retires the moment a
//printed row moves, so offering the row everywhere weakens no guarantee:
//the model still opts in, one row at a time.
```

### AIPlayerGPTSeams-L610-1935  — AIPlayerGPTSeams.cpp, near `L610` (#W73)

```
//#W73-BY (N2 d, wave-72 deck146 HIGH-1): ON EVERY PRIORITY WINDOW. The
//answer sentence below has ALWAYS said "0 = pass priority" unconditionally,
//and the row it names was printed only where baseIndex == 1: 36 of deck146's
//66 priority windows advertised a pass with no row for it, and the seat then
//bought a durable HOLD 12 times while its own PLAN line said "pass". The
//model's own reasoning at `146v125` seq 180 names the gap ("there is no
//'Pass priority' option except the hold row"). Row 0 is an index the parser
//has always accepted and the engine has always been able to execute, so this
//adds nothing to the grammar and removes nothing from the menu - it prints
//the option the sentence already promised. It stays OUT of shownLines for
//the same reason it always was: the hold latch's row set and the option-set
//key are about the ACTING rows, and a decline is not a play.
```

### AIPlayerGPTSeams-L1262-1952  — AIPlayerGPTSeams.cpp, near `L1262` (#W69)

```
//#W69-BH (K4c, deck125 HIGH-3): OFF THE ROW'S OWN ZERO. The conjunction
//required the PLAN to argue against the row as well, so a take of a row
//whose own clause already reads zero went through in silence whenever
//the reply said nothing about it. Corpus count (wave-68,
//matchups-20260906-134120): 14 K=0 sweeper takes over 12 distinct
//(game, turn) - `146` s11/s20, `152` s39, `123` s10/s40/s41/s66/s104,
//`130` s28/s33/s56/s58, `162` s8/s61 - against ONE fire of the plan
//form (`130` s20, where the re-ask worked and s21 declined). The row's
//own zero is the engine's own statement about the row; it earns the
//same ONE re-ask on its own, and the second answer executes as given.
//The decline rows are exempt: a pass verdict is not a contradiction.
```

### AIPlayerGPTSeams-rowSaysHalfDead-1968  — AIPlayerGPTSeams.cpp, near `rowSaysHalfDead` (#W67)

```
//#W67-AY (MED, engine MED / deck146 s22): a row tagged `{HALF DEAD right now:
//one half of this mode has NO legal object...}` is half an answer, and AU R5's
//rule - a worked example may not put a row the engine has already priced down
//into the answer slot - reaches it for the same reason it reaches a dead one.
//It is a PREFERENCE, not a withholding: a menu whose only non-dead rows are
//half-dead still exemplifies one, because stating the FORMAT is worth more than
//the attraction costs (the same trade AU R5 made for the all-dead menu).
```

### AIPlayerGPTSeams-L2100-1980  — AIPlayerGPTSeams.cpp, near `L2100` (#W50)

```
//#W50-Y D9: every ask through this seam is MANDATORY - there is no pass
//row, and a coded 0 here has only ever been a fallback. Say so on the
//format line (deck123 vs130 seq 31 answered a damage-order ask "CHOICE: 0
//(Pass)" and the heuristic ordered the damage).
```

### AIPlayerGPTSeams-L3129-1989  — AIPlayerGPTSeams.cpp, near `L3129` (#W63)

```
//#W63-AE (E8): the granted-type disclosure. A land can carry a gate subtype it
//does NOT print - Urborg, Tomb of Yawgmoth makes every land a Swamp, and the
//wave-62 engine seat read two such rows (`you control Isolated Chapel, a Swamp`
//with Urborg out, `you control Savannah, a Swamp` likewise) as false witnesses.
//They are legal witnesses; what was missing is that the type is on the
//BATTLEFIELD, not on the card, so a pilot checking the reason against the card
//found nothing and had to distrust the verdict. Two changes, no verdict moves:
//a witness whose PRINTED types carry the subtype is preferred over one that
//only has it now, and a granted-only witness says so.
```

### AIPlayerGPTSeams-if-2003  — AIPlayerGPTSeams.cpp, near `if` (note)

```
//
//BROAD over narrow, deliberately. The narrow option - annotate only
//permanents whose ETB bounces them - is itself an instance of THIS wave's
//dominant defect shape (a fix landing on one shape of one class): it
//closes Acererak and misses every other card whose deciding text is not
//on the line. The asymmetry with ability lines is the actual defect, so
//remove the asymmetry. Cost measured against the corpus: card text= runs
//mean 157 chars, 143 capped at 220, so a 3-8 option casting ask grows by
//~0.4-1.1k against prompts already at 10-13k - single-digit percent, and
//this seat's largest replies are exactly the re-derivations it removes.
//Skipped where it would add nothing or duplicate: no text, lands (landTag
//already names what they tap for), and adventure alt-casts (the
//{adventure spell: ...} clause above is the same text, better scoped).
```

### AIPlayerGPTSeams-L4919-2021  — AIPlayerGPTSeams.cpp, near `L4919` (#W82)

```
//#W82-A (L1, audit-2026-09): the exhausted case used to RETIRE the rejected
//line for the turn (`mStuckCastLines`), which is a withhold. The re-poll
//churn that retirement was written for (corpus 20260823 deck125 vs139 t5
//seq11-15: five exhaustion records over one board, zero model calls) is now
//answered by the window-level no-progress deferral at the top of this
//function: the conceded window returns NULL once over that exact board, and
//every row is back on the menu at the next window.
```

### AIPlayerGPTSeams-w76ClosingRowInsertAt-2033  — AIPlayerGPTSeams.cpp, near `w76ClosingRowInsertAt` (#W66)

```
//#W66-AS (H7 second half; deck123 MED, deck125 MED): WHICH DECLINE IS
//THE DEFAULT. The two decline rows are not equivalent - the plain
//decline closes one window, the hold closes the run - and the corpus
//answered with the first one it read: 182 plain declines to 4 holds on
//deck123, 213 to 29 on deck125, while the same seats sat through runs
//of byte-identical windows. Where the seat holds an instant-speed
//answer the engine WILL put this question again, so the hold is the
//decline that fits the window and it is listed first. Nothing is
//removed and nothing is renumbered away: both rows are on the menu,
//the decline is still the last row, and every consumer below reads the
//index this block just assigned rather than a position it assumed.
```

### AIPlayerGPTSeams-w76ClosingRowInsertAt-2049  — AIPlayerGPTSeams.cpp, near `w76ClosingRowInsertAt` (#W76)

```
//#W76-CP (Q12, wave-75 deck126 MED-3): THE CLOSING ROWS NOW HAVE ONE
//ORDER. The hold was listed before the decline only where the seat held
//an instant-speed answer, so 127 of this seat's menus printed
//`Hold priority` then `Cast nothing right now` and 32 printed the
//reverse - while every one of them ended with the same sentence,
//"the LAST row of this menu declines". On those 32 that sentence named
//the HOLD row, which is not the plain decline and says so in its own
//text. The order is now unconditional: the plain decline is LAST and
//the hold sits immediately before it. The option SET is untouched -
//both rows are on every menu that had them, nothing is renumbered away,
//and every consumer below reads the index assigned here.
```

### AIPlayerGPTSeams-xAnnounceLibraryNote-2065  — AIPlayerGPTSeams.cpp, near `xAnnounceLibraryNote` (#W63)

```
//#W63-AD (E7, engine HIGH-5; D5/R1 FAIL). WHY THE DECLINE ROW IS ABSENT.
//17 of 18 ANNOUNCE_X asks in the wave-62 corpus carry the Decline row; 125v146
//seq 12 (Sphinx's Revelation, turn 12) does not, and its format line says only
//"(this ask has no pass row)". Both lanes had predicted the row on EVERY X menu.
//The engine is right and the render was silent: that menu armed with the mana
//already spent (`Paid {u}{u}{w}{x} for Sphinx's Revelation with Plains #1;
//Glacial Fortress #1; ...`), so `object->checkCantCancel()` is true and there
//genuinely is no way back to the hand - the SAME seat's seq 7 rendered the row
//because nothing was committed yet. Making the row unconditional would be a
//false surface (a Cancel the engine will not honour). So the ask states the fact
//the engine's own predicate proved, on the one screen where the pilot would
//otherwise read the absence as an engine bug or as an option it failed to see.
//`canDecline` is the contract's own flag, not a second guess at the state.
```

### AIPlayerGPTSeams-xAnnounceLibraryNote-2083  — AIPlayerGPTSeams.cpp, near `xAnnounceLibraryNote` (#W67)

```
//#W67-AW (I4, deck125 HIGH-3): the cast window got 48 s and 2.7 kB of correct
//library arithmetic; the X window that commits the irreversible number got
//3.3 s and one line (`125v126` s272/s273). The ceiling is a fact of the SECOND
//window, so the second window carries it - in the header, which is the one
//string on this screen the model reads before it answers. Pure over six values,
//and empty whenever the library does not bind the ladder, in which case the
//header is byte-identical to wave 66.
```

### AIPlayerGPTSeams-etbUsableClause-2095  — AIPlayerGPTSeams.cpp, near `etbUsableClause` (#W76)

```
//#W76-CP (Q14, wave-75 deck146 MED-3): THE USABLE TAG NAMES WHAT THE PERMANENT
//CAN ACTUALLY DO. All 8 `pay 3 life` rows in the wave-75 corpus printed
//`[usable (tap for mana / attack) this turn]` over Emeria, Shattered Skyclave
//and Agadeem, the Undercrypt - ordinary lands with no animation ability and
//nothing that can ever attack - and one of them is the screen where the seat
//paid 3 life for nothing (`146v162` seq 22). The bracket is a benefit claim and
//it has to be true of THIS permanent: `attack` is emitted only for a permanent
//that is a creature or can animate into one, `tap for mana` only for one with a
//mana ability. A permanent with neither gets the arrival fact and no usability
//claim at all (a silent omission would be worse - the sentence still says what
//paying buys, which is that it enters untapped). Pure over the two board facts.
```

### AIPlayerGPTSeams-castModeCastPriceTag-2111  — AIPlayerGPTSeams.cpp, near `castModeCastPriceTag` (#W57)

```
//#W57-C (D21, wave-56 ledger MED): the cast-mode menu offers two MUTUALLY
//EXCLUSIVE uses of one card and neither row said what the other one was worth.
//`cycling with Lay Waste [cost: {2}, Cycle]` said nothing about the land kill it
//spends; `130v125` seqs 52-73 sat the opponent at 2 life for seven turns with
//four lands while the seat held 12-14 untapped sources and its whole menu was
//`Cast Lay Waste` / `cycling with Lay Waste` / `Cast nothing` / hold - it took
//the cycle once. 137 cycle windows at that seat, 16 taken; at `Opponent life:`
//<= 6, 17 offered and 3 taken. Both directions are priced from facts the SAME
//pass already computes for the other row, so no new evaluation and no new claim
//is introduced - only the opportunity cost each row silently carried.
//The tag on an ALTERNATIVE row: what casting the card normally would do instead.
```

### AIPlayerGPTSeams-L7351-2127  — AIPlayerGPTSeams.cpp, near `L7351` (#W54)

```
//#W54-C (D5): the sub-menu's own rows. Nine corpus records rendered
//this header over bare labels with no annotation of any kind. Same
//computation as the cast row's clause, matched by label so a row the
//parse cannot account for is left exactly as the engine wrote it.
//Presentation only: req.optionTexts (the staleness key) and the option
//ORDER are untouched, so act.choice still means what applyMenuChoice
//thinks it means.
```

### AIPlayerGPTSeams-L8145-2139  — AIPlayerGPTSeams.cpp, near `L8145` (N-1)

```
//N-139q (wave-36): when the entering permanent is ALREADY tapped (put onto
//the battlefield tapped by another effect - the Arboreal Grazer shape),
//paying is a strictly-dominated no-op: the engine keeps the land tapped
//(verified 139v116 s8) and the 2 life buys nothing. With only one outcome
//that matters, decline WITHOUT a model call. When ctx is unrecoverable or
//reads untapped, the ask proceeds, annotated with whichever consequence
//text is true and the land named.
```

### AIPlayerGPTSeams-castModeCommitmentNote-2151  — AIPlayerGPTSeams.cpp, near `castModeCommitmentNote` (#W71)

```
//#W71-BQ (L5, wave-70 deck130 HIGH): this menu is the SECOND HALF of a cast
//the model already answered. Nothing is withheld and nothing is answered for
//it - every row stays exactly where it was, in the order the engine built -
//but the header stops reading as a fresh offer between two equal uses of the
//card. Gated on the engine's own "Cast Card Normally" label, on the menu's
//own subject name, and on the turn the commitment was made, so a menu armed
//by the heuristic's pick or in a later turn carries no claim about what the
//model said.
```

### AIPlayerGPTSeams-L9121-2164  — AIPlayerGPTSeams.cpp, near `L9121` (#W60)

```
//#W60-L (B1, wave-59 engine-seat HIGH-1): the life this seat loses BEFORE the
//object being targeted resolves. Two disjoint cases, decided by where the
//object is: already on the stack -> everything ABOVE it resolves first; not
//on the stack (a spell whose targets are chosen at cast time, which is the
//`130v146` seq 44 shape) -> nothing already on the stack precedes it, but its
//unpaid cast surcharge does. Handed to the player rows' win fold below.
```

### AIPlayerGPTSeams-L9386-2175  — AIPlayerGPTSeams.cpp, near `L9386` (N-1)

```
//N-139b (wave-29 deck139/deck93, PRIORITY): a loot / draw-then-discard (and
//any remove-from-your-own-hand cost/effect) reaches this same seam - the
//legal set is entirely the deciding player's OWN cards - but its chooser
//SOURCE is the CARD name (e.g. "Pollywog Symbiote"), so the old
//source-name-only verb check MISSED it and rendered the generic "TARGET
//CHOICE ... pick the ONE target it will affect": the model read its own hand
//as spell targets and discarded a land it needed (deck93 s20, a 14,181-char
//reply one class short of the truncation cliff). Fix: derive the VERB the
//engine will apply from the ACTING ABILITY too - AADiscardCard::getMenuText
//is "Discard", AASacrificeCard "Sacrifice"/"Exploit", AAMover's zone-aware
//menu names the destination ("Put in Library"/"Exile"). Build the detection
//string from the source name AND that ability verb, and split the framing:
//self-inflicted (your own loot) vs opponent-forced (the validated deck140
//path, kept byte-identical) vs a keep-the-card relocation (put back).
```

### AIPlayerGPTSeams-stripAnswerLabelPrefix-2194  — AIPlayerGPTSeams.cpp, near `stripAnswerLabelPrefix` (note)

```
//
//WAVE-33 N-152j/N-158n. The name->label reconciler splits the reply into
//segments and takes the FIRST ':' as the blocker/attacker separator. On the
//segment that carries the head label that colon is the LABEL'S OWN, so
//"BLOCKS: Orc army: Sigarda" split into leftSeg="BLOCKS" (no significant
//words -> no blocker match) and the whole assignment was dropped. Every
//name-form BLOCKS reply in the wave-32 corpus died here - both of that
//corpus's unparsed_reply fallbacks - even though the name pass has shipped
//since wave-19. Removing the label makes the FIRST colon the real separator.
```

### AIPlayerGPTSeams-combatTradePreviewStats-2208  — AIPlayerGPTSeams.cpp, near `combatTradePreviewStats` (#W47)

```
//#W47-R3 (wave-46 ledger, HIGH). 'bGainConverted' says the BLOCKING side's
//controller has a life-to-damage converter (Sanguine Bond and its class) on the
//battlefield, so every point of life this pairing gives that side ALSO comes
//off the other player's total. The converter paragraph in CURRENT SITUATION and
//the priced tail on these lines were both true and both present in the deck152
//and deck162 prompts, and the multiplication between them was left to the
//reader: deck152 attacked into gaining walls three combats running (20 -> 11 ->
//5 while the opponent went 28 -> 37) and lost at -1; deck162 sent a 3/4 into
//three 1/1 lifelinkers and went 19 -> 0 in one combat. So the doubling is bound
//to the number it doubles, exactly as #W46-9 bound the lifelink gain to the
//branch it belongs to. SCOPE, deliberately one-sided: only the blocking side's
//gains are priced this way, because that is the side whose gain is a COST to
//the reader in both windows; a converter on the reader's own side pricing the
//reader's own gain is a benefit, and the CURRENT SITUATION paragraph already
//states it. Defaulted false so every existing caller and PARSETEST case is
//untouched.
```

### AIPlayerGPTSeams-L10738-2229  — AIPlayerGPTSeams.cpp, near `L10738` (#W63)

```
//#W63-AB (E1, deck123 HIGH-1): with both halves of their life LOOP on the
//board, a price that names life THEY gain is not a price at all - it is the
//first link of a chain that runs until this seat is at 0. `123v126` seq 76
//printed "they gain 1 from this block only" beside "(you kill it, your
//blocker lives)" and the block ran 16 -> 0. The tail rides exactly the
//clauses whose gainer is the READING seat's opponent, and states the
//mechanism rather than a prediction, as D2's header clause does.
```

### AIPlayerGPTSeams-stripTrailingListGloss-2241  — AIPlayerGPTSeams.cpp, near `stripTrailingListGloss` (#W59)

```
//#W59-J (K9, wave-58 deck125 HIGH-2): the reply protocol REQUIRES a name gloss
//on a CHOICE: line ("CHOICE: 1 (Cast Supreme Verdict)"), and a model that has
//learned that habit writes it on a PUT: line too - once, at the end, over the
//whole list: `125v130` s83 answered `PUT: 9, 1 (Supreme Verdict, Fall of the
//Gavel)`, the exact required form plus the gloss, and it scored unparsed_reply
//while the heuristic picked the discards. The #W52-G echo reconciler reads a
//parenthetical as the echo of the ONE index in front of it, so a two-name gloss
//matched no single row and the index it followed was dropped as a stale echo.
//Split the shape off before any pass reads it: a trailing parenthetical whose
//comma-separated parts number EXACTLY the bare indices on the line is the
//list's gloss, so it leaves the line the numbers are read from and is handed
//back to the caller as names. Nothing is silently trusted: the caller
//reconciles those names against the rows, and the CHOICE rule still decides a
//disagreement (names win over indices). A gloss that does not pair 1:1 - a
//different count, a nested parenthetical, a prose tail, a numeric part - is
//left exactly where the model wrote it and the wave-51 rules judge it. Pure, so
//PARSETEST walks the whole grammar without a reply.
```

### AIPlayerGPTSeams-L12312-2263  — AIPlayerGPTSeams.cpp, near `L12312` (#W64)

```
//#W64-AG (F7): first strike, double strike and the candidates'
//deathtouch/wither/infect are MODELLED now (gangKillNeed), not
//grounds for silence - that guard is what hid `146v152` seq 20's
//verdict. What still abandons the claim is what changes the meaning
//of "kill": indestructible, persist/undying, a becomes-blocked pump,
//and (below) any candidate whose damage to this attacker is
//prevented.
```

### AIPlayerGPTSeams-parseBlockAssignments-2275  — AIPlayerGPTSeams.cpp, near `parseBlockAssignments` (note)

```
//
//When blockerNames/attackerNames are supplied, a SECOND pass resolves a
//block written with creature NAMES ("BLOCKS: Memnite: Boggart Ram-Gang",
//"Ornithopter blocks Dragonmaster Outcast") back to its B#/A# labels - the
//exact analogue of the shipped ATTACK name->index reconcile and the CHOICE:
//name-echo. A legal, correctly-shaped name-form block was previously dropped
//WHOLE -> unparsed -> the heuristic made a different, sometimes game-losing
//block (deck110 vs21 s18: a 9/1 chumped into a 1/1 wither, game lost by 2).
//Each comma/newline segment splits at a ':'/'blocks' separator into a
//blocker phrase (left) and an attacker phrase (right); each resolves to the
//UNIQUE matching name (attacker restricted to that blocker's legal set when
//given). Ambiguous/no-match drops THAT assignment only; already-coded
//assignments and the first-wins rule are respected.
//blockerInAttackerSlot (out, optional; #W53-M D19): set true when a well-formed
//"B<n>:" names ANOTHER B-handle where an attacker belongs ("B3:B1" - `126v162`
//seq 21 sent five, three of them this shape). Those pairs were already counted
//as dropped; nothing said WHY, so the record read as a clean parse that quietly
//discarded three lifelink Vampires. Representation only: the pair still drops.
//gangConflict (out, optional; W36 item 1): set true when ONE blocker is
//assigned to SEVERAL DIFFERENT attackers - the illegal shape whose first pair
//used to be taken silently (116-fp8 vs105 seq25: "B1:A1, B1:A2, B1:A3" left
//A2+A3 unblocked for lethal poison). A repeated IDENTICAL pair, or a legal
//gang-block (several blockers on one attacker), is NOT a conflict.
```

### AIPlayerGPTSeams-buildRevealAskText-2303  — AIPlayerGPTSeams.cpp, near `buildRevealAskText` (note)

```
//
//deck102 wave-20 E1: a HAND reveal (Thoughtseize/Duress-class targeted discard)
//was mislabelled "the top N cards of your library" and used choose-a-SUBSET
//framing on a pick-EXACTLY-ONE discard. Render the true source, and frame a
//fixed <1> chooser as choose-ONE. Library/multi-pick reveals (Glacial
//Revelation, Into the North, surveil - wave-20 ENGINE-R1) hit revealSource 0 +
//pickExactlyOne=false and keep byte-identical prior text.
```

### AIPlayerGPTSeams-discardBoardVerdictTag-2315  — AIPlayerGPTSeams.cpp, near `discardBoardVerdictTag` (#W59)

```
//#W59-I (K4, wave-58 engine-seat HIGH-1): the rebadge folded the zero-predicate
//over the WHOLE verdict string, which is not one statement but a LIST of
//per-scope verdicts joined by `;`, each carrying its own parenthetical
//qualifier. 11 of the corpus's 139 dead badges named a live kill:
//`123v126` s12 badged Devour Flesh `{dead right now: they control 1 creature -
//Overgrown Battlement (0/4) [defender] is sacrificed, they gain 4 - the
//sacrificing player gains, not you; YOU control 0 creatures - targeting
//yourself does nothing}` (the SELF scope's "does nothing" matched while the
//OPPONENT scope named a body dying), and `123v130` s17/s18 badged Tragic Slip
//whose operative `-1/-1` stands OUTSIDE the Morbid qualifier the predicate
//matched. Both cards were verified against their primitives - the scripts are
//right and the badge was the liar, at a discard where the seat is told the
//removal it is about to pitch does nothing. Lane C's `verdictReadsZero` is the
//per-scope fold the cast-menu header already uses; the two surfaces now apply
//the same predicate to the same units. `rowSaysNoOp` is unchanged and nothing
//is removed: a verdict that is not all-zero simply prints as `{right now: ...}`
//instead of `{dead right now: ...}`. The clause carries a LEADING SPACE here
//(it is appended to a row), which verdictReadsZero's anchor does not, so the
//space is dropped before the fold.
```

