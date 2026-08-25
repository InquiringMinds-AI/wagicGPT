#include "PrecompiledHeader.h"

#include "MTGDefinitions.h"
#include "Rules.h"
#include "ManaCost.h"
#include "Player.h"
#include "AIMomirPlayer.h"

#include "GameApp.h"
#include "MTGGameZones.h"
#include "MTGAbility.h"
#include "AllAbilities.h"
#include "DeckManager.h"
#include "AIPlayer.h"
#include <JLogger.h>

vector<Rules *> Rules::RulesList = vector<Rules *>();

#if defined(WAGIC_AUTODEMO) || defined(WAGIC_HWPROBE)
#include <stdarg.h>
static void rulesProbe(const char* fmt, ...)
{
    FILE* f = fopen("User/wagic-probe.log", "a");
    if (!f) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fprintf(f, "\n");
    fclose(f);
}
#else
static inline void rulesProbe(const char*, ...) {}
#endif

//#W44-1 selfplay fairness gates. Source of the finding: the wave-43 seat review
//at strategy-design/wave43/seats/seat-146-152-162.md ("RIGGED OPENING HANDS").
//See the AI-vs-AI branch in addExtraRules() for what these unrig, and for why
//the DEFAULT deliberately stays rigged for now.
//Semantics: unset or empty -> the default; "0" -> off; anything else -> on.
static bool rulesEnvFlag(const char * name, bool dflt)
{
    const char * v = getenv(name);
    if (!v || !*v)
        return dflt;
    return !(v[0] == '0' && v[1] == '\0');
}

//The hand gate: deal an AI-vs-AI seat a REAL opening hand instead of a stacked one.
static bool selfplayFairHand()
{
    return rulesEnvFlag("WAGIC_SELFPLAY_FAIRHAND", false);
}

//The seat gate: drop the agressivity/forceBestAbilityUse pins. Defaults to
//whatever the hand gate says (so one flag buys a fully fair seat), but can be
//pinned back on with WAGIC_SELFPLAY_FAIRAI=0 to isolate the hand variable alone.
static bool selfplayFairAI()
{
    return rulesEnvFlag("WAGIC_SELFPLAY_FAIRAI", selfplayFairHand());
}

//Sorting by displayName
struct RulesMenuCmp{
    bool operator()(const Rules * a,const Rules * b) const{
        return a->displayName < b->displayName;
    }
} RulesMenuCmp_;

Rules * Rules::getRulesByFilename(string _filename)
{
    //Case-insensitive: PSP memory-stick (FAT) directory scans return
    //8.3-compatible names UPPERCASED (CLASSIC.TXT), so the stored filename
    //may not byte-match a lowercase literal like "classic.txt".
    std::transform(_filename.begin(), _filename.end(), _filename.begin(), ::tolower);
    for (size_t i = 0; i < RulesList.size(); ++i)
    {
        string candidate = RulesList[i]->filename;
        std::transform(candidate.begin(), candidate.end(), candidate.begin(), ::tolower);
        if (candidate == _filename)
            return RulesList[i];
    }
    return NULL;
}

int Rules::loadAllRules()
{
    vector<string> rulesFiles = JFileSystem::GetInstance()->scanfolder("rules");
    for (size_t i = 0; i < rulesFiles.size(); ++i)
    {
        Rules * rules = NEW Rules();
        if (rules->load(rulesFiles[i]))
        {
            RulesList.push_back(rules);
        }
        else
        {
            SAFE_DELETE(rules);
        }
    }
    //Kind of a hack here, we sort Rules alphabetically because it turns out to be matching
    // The historical order of Game modes: Classic, Momir Basic, Random 1, Random 2, Story
    std::sort(RulesList.begin(),RulesList.end(),RulesMenuCmp_);
    return 1;
}

void Rules::unloadAllRules()
{
    for (size_t i = 0; i < RulesList.size(); ++i)
    {
        SAFE_DELETE(RulesList[i]);
    }
    RulesList.clear();
}

int Rules::getMTGId(string cardName)
{
    int cardnb = atoi(cardName.c_str());
    if (cardnb) return cardnb;
    if (cardName.compare("*") == 0) return -1; //Any card
    MTGCard * card = MTGCollection()->getCardByName(cardName);
    if (card) return card->getMTGId();
    DebugTrace("RULES: Can't find card:" << cardName.c_str());
    return 0;
}

MTGCardInstance * Rules::getCardByMTGId(GameObserver* g, int mtgid, Player * preferred)
{
    for (int i = 0; i < 2; i++)
    {
        //A caller loading one player's zones must find THAT player's copy:
        //the global first-match order (all of player 0's zones, then player
        //1's) otherwise returns an already-placed same-id card belonging to
        //the other player - e.g. a basic land both sides start with - and
        //the subsequent putInZone from this player's library returns NULL.
        Player * p = g->players[i];
        if (preferred)
            p = (i == 0) ? preferred : g->players[(g->players[0] == preferred) ? 1 : 0];
        MTGGameZone * zones[] = { p->game->library, p->game->hand, p->game->inPlay, p->game->graveyard, p->game->exile, p->game->commandzone, p->game->sideboard };
        for (int j = 0; j < 7; j++)
        {
            MTGGameZone * zone = zones[j];
            for (int k = 0; k < zone->nb_cards; k++)
            {
                MTGCardInstance * card = zone->cards[k];
                if (!card) return NULL;
                if (card->getMTGId() == mtgid) return card;
            }
        }
    }
    return NULL;
}

RulesPlayerData::RulesPlayerData()
    : player(0)
{
}

RulesPlayerData::~RulesPlayerData()
{
    SAFE_DELETE(player);
}

RulesState::RulesState()
{
    // CR 500.1/501.1: every turn has a beginning phase (untap, upkeep, draw). The
    // game formerly opened turn 1 at first main, which deleted the starting player's
    // entire beginning phase. Turn 1 now begins at the untap step so untap and upkeep
    // occur (e.g. an upkeep-triggered leyline fires on turn 1); the starting player's
    // one-time draw-step skip (CR 103.8a / 504.1) is enforced in
    // GameObserver::nextGamePhase (turn == 0). Test-suite fixtures set their own [INIT]
    // phase, so this default only affects real games.
    phase = MTG_PHASE_UNTAP;
    player = 0;
}

void RulesState::parsePlayerState(int playerId, string s)
{
    if(playerData[playerId].player->parseLine(s))
      return;

    size_t limiter = s.find("=");
    if (limiter == string::npos) limiter = s.find(":");
    string areaS;
    if (limiter != string::npos)
    {
        areaS = s.substr(0, limiter);

        if (areaS.compare("auto") == 0)
        {
            playerData[playerId].extraRules.push_back(s.substr(limiter + 1));
            return;
        }
        else
        {
            return; // ERROR
        }
    }
    else
    {
        //ERROR
    }
}

void Rules::addExtraRules(GameObserver* g)
{
    int id = g->mLayers->actionLayer()->getMaxId();
    MTGAllCards::sortSubtypeList();
    for (int i = 0; i < 2; ++i)
    {
        Player * p = g->players[i];
        //Trick so that the abilities don't die;
        g->ExtraRules[i].currentZone = p->game->inPlay;
        g->ExtraRules[i].lastController = p;
        g->ExtraRules[i].owner = p;
        for (size_t j = 0; j < initState.playerData[i].extraRules.size(); ++j)
        {
            AbilityFactory af(g);
            MTGPlayerCards * hand = NULL;
            int handsize = 7;
            int difficultyRating = 0;
            int Optimizedhandcheat = options[Options::OPTIMIZE_HAND].number;
            MTGAbility * a = af.parseMagicLine(initState.playerData[i].extraRules[j], id++, NULL, &(g->ExtraRules[i]));

            if (p->isAI() && (p->playMode == Player::MODE_AI && p->opponent()->playMode== Player::MODE_AI))
                difficultyRating = 1;
            else if (g->players[1]->playMode == Player::MODE_HUMAN)
                difficultyRating = 0;
            else if (p->playMode == Player::MODE_TEST_SUITE)
                difficultyRating = 0;
            else if (g->mRules->gamemode == GAME_TYPE_MOMIR)
                difficultyRating = 0;
            else if(g->mRules->gamemode == GAME_TYPE_RANDOM1 || g->mRules->gamemode == GAME_TYPE_RANDOM2)
                difficultyRating = 0;
            else if(g->mRules->gamemode == GAME_TYPE_RANDOM3 || g->mRules->gamemode == GAME_TYPE_RANDOM5 || g->mRules->gamemode == GAME_TYPE_RANDOMCOMMANDER || g->mRules->gamemode == GAME_TYPE_RANDOMCOMMANDERFROMFILE)
                difficultyRating = 0;
            else if(g->mRules->gamemode == GAME_TYPE_HORDE || g->mRules->gamemode == GAME_TYPE_SET_LIMITED)
                difficultyRating = 0;
            else if (g->mRules->gamemode == GAME_TYPE_STORY)
                difficultyRating = 0;
            else if (a->aType == MTGAbility::STANDARD_DRAW)
                difficultyRating = g->getDeckManager()->getDifficultyRating(g->players[0], g->players[1]);

            if (a)
            {
                //We make those non interruptible, so that they don't appear on the player's stack
                a->canBeInterrupted = false;
                if (a->oneShot)
                {
                    if (a->aType != MTGAbility::STANDARD_DRAW)
                        a->resolve();
                    else if (p->isAI() && (p->playMode == Player::MODE_AI && p->opponent()->playMode== Player::MODE_AI))
                    {
                        //#W44-1 (docket item 1; evidence in strategy-design/
                        //wave43/seats/seat-146-152-162.md, "RIGGED OPENING
                        //HANDS"). Stock Wagic rigs BOTH seats of an AI-vs-AI
                        //game - the only shape the selfplay harness ever runs,
                        //and also the GUI's endless/attract demo:
                        //  - OptimizedHand(p, handsize, 3, 1, 3) stacks the
                        //    opening hand with 3 lands + 1 cheap creature + 3
                        //    cheap spells, "to insure a challanging match";
                        //  - agressivity += 100 pins the seat aggressive
                        //    (consumed at AIPlayerBaka.cpp's `agressivity > 80`
                        //    in chooseAttackers);
                        //  - forceBestAbilityUse bypasses ability-use randomness
                        //    (3 consumers in AIPlayerBaka.cpp).
                        //The wave-43 review measured the cost: 42/42 corpus hands
                        //held exactly 3 lands and 42/42 were kept. So every
                        //mulligan teach in every deck guide was UNTESTABLE by this
                        //harness (no seat ever saw a hand worth refusing), and
                        //every heuristic-fallback attack decision in every corpus
                        //ran pinned aggressive with nothing saying so.
                        //
                        //WAGIC_SELFPLAY_FAIRHAND=1 opts a run out of the hand rig:
                        //the seat takes the SAME a->resolve() deal a human seat
                        //takes, which also finally arms PreGamePhase's London
                        //mulligan for AI seats (the phase already runs for
                        //MODE_AI x2 - it just never had a hand worth refusing).
                        //WAGIC_SELFPLAY_FAIRAI follows the hand gate and drops the
                        //two seat pins; =0 forces them back on for isolation runs.
                        //
                        //THE DEFAULT (no env) IS DELIBERATELY UNCHANGED: the rig
                        //stays, so corpora from waves <=43 stay comparable.
                        //Flipping the default is an OWNER decision and NO SUCH
                        //RULING HAS BEEN MADE - do not flip it without one.
                        //
                        //SCOPE: this branch is MODE_AI vs MODE_AI only. Suite
                        //seats are MODE_TEST_SUITE (they seed hands via INIT) and
                        //never reach here; a human game leaves players[0]
                        //MODE_HUMAN, so the single-player difficulty ladder below
                        //is untouched either way.
                        handsize = ((AADrawer *)a)->getNumCards();
                        if (!selfplayFairAI())
                        {
                            ((AIPlayer *) p)->forceBestAbilityUse = true;
                            ((AIPlayer *) p)->agressivity += 100;
                        }
                        if (selfplayFairHand())
                            a->resolve();
                        else
                            hand->OptimizedHand(p,handsize, 3, 1, 3);
                    }
                    else if (!p->isAI() && !Optimizedhandcheat)
                        a->resolve();
                    else if (p->playMode == Player::MODE_TEST_SUITE)
                        a->resolve();
                    else if (g->mRules->gamemode == GAME_TYPE_MOMIR)
                        a->resolve();
                    else if(g->mRules->gamemode == GAME_TYPE_RANDOM1 || g->mRules->gamemode == GAME_TYPE_RANDOM2)
                        a->resolve();
                    else if(g->mRules->gamemode == GAME_TYPE_RANDOM3 || g->mRules->gamemode == GAME_TYPE_RANDOM5 || g->mRules->gamemode == GAME_TYPE_RANDOMCOMMANDER || g->mRules->gamemode == GAME_TYPE_RANDOMCOMMANDERFROMFILE) 
                        a->resolve();
                    else if (g->mRules->gamemode == GAME_TYPE_STORY)
                        a->resolve();
                    else//stupid protections to keep this out of momir and other game modes.
                    {
                        handsize = ((AADrawer *)a)->getNumCards();
                        if(difficultyRating == EASY)
                        {
                            ((AIPlayer *) p)->forceBestAbilityUse = true;
                            ((AIPlayer *) p)->agressivity += 100;
                            hand->OptimizedHand(p,handsize, 3, 1, 3);//easy decks get a major boost, open hand is 2lands,1 creature under 3 mana,3spells under 3 mana.
                        }
                        else if (difficultyRating == NORMAL)
                        {
                            hand->OptimizedHand(p,handsize, 2, 0, 2);//give the Ai deck a tiny boost by giving it 1 land and 2 spells under 3 manacost.
                        }
                        else if (difficultyRating == HARD)
                        {
                            hand->OptimizedHand(p,handsize, 2, 0, 0);//give the Ai deck a tiny boost by giving it 1 land and 2 spells under 3 manacost.
                        }
                        else
                        {
                            hand->OptimizedHand(p,handsize, 3, 1, 3);//no rating fall out case.
                        }
                    }
                    SAFE_DELETE(a);
                }
                else
                {
                    a->addToGame();
                }

            }
        }
    }

    rulesProbe("extraRules perplayer p0=%d p1=%d",
        (int) initState.playerData[0].extraRules.size(), (int) initState.playerData[1].extraRules.size());
    int parsedGlobal = 0, failedGlobal = 0;
    for (size_t j = 0; j < extraRules.size(); ++j)
    {
        AbilityFactory af(g);
        MTGAbility * a = af.parseMagicLine(extraRules[j], id++, NULL, &(g->ExtraRules[0]));
        if (a)
        {
            parsedGlobal++;
            if (a->oneShot)
            {
                a->resolve();
                delete (a);
            }
            else
            {
                a->addToGame();
            }
        }
        else
        {
            failedGlobal++;
            //NB: rule keywords (putinplayrule, attackrule, ...) install observers
            //and return NULL by design - a NULL here is not necessarily a failure.
            rulesProbe("extraRules parsed NULL: %.100s", extraRules[j].c_str());
        }
    }
    rulesProbe("extraRules global=%d parsed=%d failed=%d p0hand=%d p0handsize=%d p1handsize=%d",
        (int) extraRules.size(), parsedGlobal, failedGlobal,
        g->players[0]->game->hand->nb_cards, g->players[0]->handsize, g->players[1]->handsize);
}

Player * Rules::loadPlayerMomir(GameObserver* observer, int isAI)
{
    string deckFileSmall = "momir";
    char empty[] = "";

    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection()); //Autogenerate a momir deck. Leave the "momir.txt" bits below for stats.
    tempDeck->addRandomCards(12, 0, 0, Constants::RARITY_L, "Forest");
    tempDeck->addRandomCards(12, 0, 0, Constants::RARITY_L, "Plains");
    tempDeck->addRandomCards(12, 0, 0, Constants::RARITY_L, "Swamp");
    tempDeck->addRandomCards(12, 0, 0, Constants::RARITY_L, "Mountain");
    tempDeck->addRandomCards(12, 0, 0, Constants::RARITY_L, "Island");

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, options.profileFile("momir.txt", "", true).c_str(), deckFileSmall, false, tempDeck);
    else
        player = NEW AIMomirPlayer(observer, options.profileFile("momir.txt", "", true).c_str(), deckFileSmall, empty, tempDeck);

    return player;
}

Player * Rules::loadPlayerRandom(GameObserver* observer, int isAI, int mode)
{
    int color1 = 1 + observer->getRandomGenerator()->random() % 5;
    int color2 = 1 + observer->getRandomGenerator()->random() % 5;
    int color0 = Constants::MTG_COLOR_ARTIFACT;
    if (mode == GAME_TYPE_RANDOM1) color2 = color1;
    int colors[] = { color1, color2, color0 };
    int nbcolors = 3;

    string lands[] = { "forest", "forest", "island", "mountain", "swamp", "plains", "forest" };

    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection());
    tempDeck->addRandomCards(9, 0, 0, -1, lands[color1].c_str());
    tempDeck->addRandomCards(9, 0, 0, -1, lands[color2].c_str());
    tempDeck->addRandomCards(1, 0, 0, 'U', "land");
    tempDeck->addRandomCards(1, 0, 0, 'R', "land");
    tempDeck->addRandomCards(12, 0, 0, -1, "creature", colors, nbcolors);
    tempDeck->addRandomCards(2, 0, 0, -1, "sorcery", colors, nbcolors);
    tempDeck->addRandomCards(2, 0, 0, -1, "enchantment", colors, nbcolors);
    tempDeck->addRandomCards(2, 0, 0, -1, "instant", colors, nbcolors);
    tempDeck->addRandomCards(2, 0, 0, -1, "artifact", colors, nbcolors);

    string deckFile = "random";
    string deckFileSmall = "random";

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

Player * Rules::loadRandomSetLimited(GameObserver* observer, int isAI)
{
    //Random sets
    int setId[] = { observer->getRandomGenerator()->random() % setlist.size(), observer->getRandomGenerator()->random() % setlist.size() };
    int nbSet = sizeof(setId)/sizeof(setId[0]);

    int color1 = 1 + observer->getRandomGenerator()->random() % 5;
    int color2 = 1 + observer->getRandomGenerator()->random() % 5;
    int colors[] = { color1, color2 };
    int nbcolors = 2;
    string lands[] = { "", "forest", "island", "mountain", "swamp", "plains" };

    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection());    
    // Try to add basic lands from that set
    tempDeck->addRandomCards(8, setId, nbSet, -1, lands[color1].c_str());
    tempDeck->addRandomCards(8, setId, nbSet, -1, lands[color2].c_str());
    tempDeck->addRandomCards(2, setId, nbSet, -1, "land");
    // If lands < 18 add from any set
    int missingLands = 18 - tempDeck->totalCards();
    if (missingLands > 0)
    {
        tempDeck->addRandomCards(missingLands/2, 0, 0, -1, lands[color1].c_str());
        tempDeck->addRandomCards(missingLands/2, 0, 0, -1, lands[color2].c_str());
    }
    // Lone artifact and creatures.
    tempDeck->addRandomCards(1, setId, nbSet, -1, "artifact");
    tempDeck->addRandomCards(12, setId, nbSet, -1, "creature", colors, nbcolors);
    // Want the deck to be 40 cards, take any card from the set in the colors
    int missingCards = 40 - tempDeck->totalCards();
    if (missingCards > 0)
        tempDeck->addRandomCards(missingCards, setId, nbSet, -1, "", colors, nbcolors);

    string deckFile = "random";
    string deckFileSmall = "random";

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

Player * Rules::loadPlayerRandomThree(GameObserver* observer, int isAI)
{
    int color1 = 1 + observer->getRandomGenerator()->random() % 5;
    int color2 = 1 + observer->getRandomGenerator()->random() % 5;
    int color3 = 1 + observer->getRandomGenerator()->random() % 5;
    int color0 = Constants::MTG_COLOR_ARTIFACT;
    
    int colors[] = { color1, color2, color3, color0 };
    int nbcolors = 4;

    string lands[] = { "", "forest", "island", "mountain", "swamp", "plains" };

    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection());
    tempDeck->addRandomCards(5, 0, 0, -1, lands[color1].c_str());
    tempDeck->addRandomCards(5, 0, 0, -1, lands[color2].c_str());
    tempDeck->addRandomCards(5, 0, 0, -1, lands[color3].c_str());
    tempDeck->addRandomCards(3, 0, 0, 'R', lands[color1].c_str());
    tempDeck->addRandomCards(3, 0, 0, 'R', lands[color2].c_str());
    tempDeck->addRandomCards(3, 0, 0, 'R', lands[color3].c_str());
    tempDeck->addRandomCards(1, 0, 0, 'U', "land");
    tempDeck->addRandomCards(1, 0, 0, 'R', "land");
    tempDeck->addRandomCards(18, 0, 0, -1, "creature", colors, nbcolors);
    tempDeck->addRandomCards(1, 0, 0, 'R', "creature", colors, nbcolors);
    tempDeck->addRandomCards(1, 0, 0, 'M', "creature", colors, nbcolors);
    tempDeck->addRandomCards(3, 0, 0, -1, "enchantment", colors, nbcolors);
    tempDeck->addRandomCards(3, 0, 0, -1, "instant", colors, nbcolors);
    tempDeck->addRandomCards(3, 0, 0, -1, "sorcery", colors, nbcolors);
    tempDeck->addRandomCards(3, 0, 0, -1, "artifact", colors, nbcolors);
    tempDeck->addRandomCards(1, 0, 0, -1, "battle", colors, nbcolors);
    tempDeck->addRandomCards(1, 0, 0, -1, "planeswalker", colors, nbcolors);

    string deckFile = "random";
    string deckFileSmall = "random";

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

Player * Rules::loadPlayerRandomFive(GameObserver* observer, int isAI)
{ 
    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection());

    tempDeck->addRandomCards(24, 0, 0, -1, "land");
    tempDeck->addRandomCards(36, 0, 0, -1, "");
    
    string deckFile = "random";
    string deckFileSmall = "random";

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

Player * Rules::loadPlayerRandomCommander(GameObserver* observer, int isAI)
{
    MTGDeck * cmdTempDeck = NEW MTGDeck(MTGCollection());
    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection());
    tempDeck->meta_commander = true;

    string lands[] = { "", "forest", "island", "mountain", "swamp", "plains", "basic", "basic" };

    cmdTempDeck->addRandomCards(1, 0, 0, -1, "legendary");
    DeckDataWrapper * myCommandZone = NEW DeckDataWrapper(cmdTempDeck);
    MTGCard * commander = myCommandZone->getCard(0, true);

    while(!commander->data->isCreature())
    {
        cmdTempDeck->addRandomCards(1, 0, 0, -1, "legendary");
        myCommandZone = NEW DeckDataWrapper(cmdTempDeck);
        commander = myCommandZone->getCard(0, true);
        delete myCommandZone; // Clean up to avoid memory leaks
    }

    stringstream cid;
    cid << commander->getMTGId();
    vector<string> newCMD;
    newCMD.push_back(cid.str());
    tempDeck->replaceCMD(newCMD);

    std::vector< int > colors;

    for (int i = 0; i < Constants::NB_Colors; i++)
    {
        if (commander->data->getManaCost()->hasColor(i))
            colors.push_back(i);
    }

    if(colors.data()[0] != 0) { colors.insert(colors.begin(),0); }

    // Add lands
    int numLands = colors.size() > 2 ? 40 / (colors.size() - 1) : 40;
    if(colors.size() > 1)
    {
        for (unsigned int i = 1; i < colors.size(); i++)
        {
            tempDeck->addRandomCards(numLands, 0, 0, -1, lands[colors.data()[i]].c_str());
        }
    }
    else { tempDeck->addRandomCards(numLands, 0, 0, Constants::RARITY_L); }

    tempDeck->addRandomCards(59, 0, 0, -1, "", colors.data(), colors.size());

    string deckFile = "random";
    string deckFileSmall = "random";

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

/**
 * @brief Loads a player with a randomly selected commander from a specified file.
 * 
 * This function reads a file ('User/commander/commander.txt') that contains a list of potential commander IDs or names. 
 * It selects one randomly to be used as the player's commander. If the file doesn't exist, or no valid commanders 
 * are found, a default commander ID (670972) is used.
 * 
 * @param observer A pointer to the GameObserver, used to track game events.
 * @param isAI An integer indicating if the player is AI (non-zero) or human (zero).
 * 
 * @return A pointer to the Player object created, either a human or AI player with a deck constructed 
 *         around the selected commander. Returns 'nullptr' if a player could not be created due to errors.
 */
Player* Rules::loadPlayerRandomCommanderFromFile(GameObserver* observer, int isAI)
{
    std::ifstream file("User/commander/commander.txt");

    // Check if the file exists
    if (!file.is_open())
    {
        std::cerr << "File not found: User/commander/commander.txt. Creating file with default value 670972." << std::endl;

        // Create the file with a default commander ID
        std::ofstream outFile("User/commander/commander.txt");
        if (!outFile.is_open())
        {
            std::cerr << "Failed to create file: User/commander/commander.txt" << std::endl;
            return NULL;
        }
        outFile << "670972" << std::endl;
        outFile.close();

        // Re-open the file to continue processing
        file.open("User/commander/commander.txt");
        if (!file.is_open())
        {
            std::cerr << "Failed to open file after creation: User/commander/commander.txt" << std::endl;
            return NULL;
        }
    }

    std::vector<std::string> commanderIds;
    std::string line;
    while (std::getline(file, line))
    {
        // Ignore lines that start with '#'
        if (!line.empty() && line[0] == '#')
        {
            continue;
        }

        // Validate and add the line if it's a valid numeric ID
        char* end;
        long id = std::strtol(line.c_str(), &end, 10);
        if (*end == '\0' && id > 0)
        {
            commanderIds.push_back(line);
        }
        else
        {
            // Attempt to convert name to ID if it's not a valid numeric ID
            int cardId = getMTGId(line); // Assumes getMTGId is defined and returns -1 if not found
            if (cardId != -1)
            {
                std::stringstream ss;
                ss << cardId;
                commanderIds.push_back(ss.str());
            }
        }
    }
    file.close();

    // If no valid commander IDs found, default to 670972
    if (commanderIds.empty())
    {
        std::cerr << "No valid commander IDs found in file. Using default commander ID 670972." << std::endl;
        commanderIds.push_back("670972");
    }

    // Filter out invalid commander IDs
    std::vector<std::string> validCommanderIds;
    for (size_t i = 0; i < commanderIds.size(); ++i)
    {
        int commanderIntId = std::atoi(commanderIds[i].c_str());
        MTGCard* card = MTGCollection()->getCardById(commanderIntId);
        if (card != NULL)
        {
            validCommanderIds.push_back(commanderIds[i]);
        }
        else
        {
            std::cerr << "Commander with ID " << commanderIds[i] << " does not exist in primitives." << std::endl;
        }
    }

    // If no valid commander IDs after filtering, default to 670972
    if (validCommanderIds.empty())
    {
        std::cerr << "No valid commanders available after filtering. Using default commander ID 670972." << std::endl;
        validCommanderIds.push_back("670972");
    }

    // Randomly select a commander ID
    std::srand(static_cast<unsigned int>(time(NULL)));
    int randomIndex = std::rand() % validCommanderIds.size();
    std::string commanderId = validCommanderIds[randomIndex];

    MTGDeck* cmdTempDeck = NEW MTGDeck(MTGCollection());
    int commanderIntId = std::atoi(commanderId.c_str());
    if (!cmdTempDeck->add(commanderIntId))
    {
        std::cerr << "Failed to add commander card with ID: " << commanderId << std::endl;
        return NULL;
    }

    MTGDeck* tempDeck = NEW MTGDeck(MTGCollection());
    tempDeck->meta_commander = true;

    std::string lands[] = { "", "forest", "island", "mountain", "swamp", "plains", "basic", "basic" };

    DeckDataWrapper* myCommandZone = NEW DeckDataWrapper(cmdTempDeck);
    MTGCard* commander = myCommandZone->getCard(0, true);

    if (commander == NULL)
    {
        std::cerr << "Invalid commander card retrieved from the command zone." << std::endl;
        return NULL;
    }

    std::stringstream cid;
    cid << commanderId;
    std::vector<std::string> newCMD;
    newCMD.push_back(cid.str());
    tempDeck->replaceCMD(newCMD);

    std::vector<int> colors;

    for (int i = 0; i < Constants::NB_Colors; i++)
    {
        if (commander->data->getManaCost()->hasColor(i))
            colors.push_back(i);
    }

    if (!colors.empty() && colors[0] != 0)
    {
        colors.insert(colors.begin(), 0);
    }

    // Add lands
    int numLands = colors.size() > 2 ? 40 / (colors.size() - 1) : 40;
    if (colors.size() > 1)
    {
        for (unsigned int i = 1; i < colors.size(); i++)
        {
            tempDeck->addRandomCards(numLands, 0, 0, -1, lands[colors[i]].c_str());
        }
    }
    else
    {
        tempDeck->addRandomCards(numLands, 0, 0, Constants::RARITY_L);
    }

    tempDeck->addRandomCards(59, 0, 0, -1, "", colors.data(), colors.size());

    std::string deckFile = "random";
    std::string deckFileSmall = "random";

    Player* player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

Player * Rules::loadPlayerHorde(GameObserver* observer, int isAI)
{    
    int nbColors = 1;
    string randomTribe = "";
    int tribeColor[] = { observer->getRandomGenerator()->random() % 6 };

    string lands[] = { "land", "forest", "island", "mountain", "swamp", "plains" };

    const char* const multicolorTribes[] = { "Ally", "Construct", "Drone", "Eldrazi", "Elemental", "Golem", "Human", "Myr",
          "Sliver", "Spellshaper", "Spirit", "Wizard" };
    const char* const whiteTribes[] = { "Angel", "Archer", "Bird", "Cat", "Cleric", "Griffin", "Kithkin", "Knight", "Kor", "Monk", "Rebel", "Samurai", "Scout", "Soldier", "Spirit" };
    const char* const blueTribes[] = { "Artificer", "Bird", "Drake", "Faerie", "Illusion", "Merfolk", "Mutant", "Nightmare", "Pirate", "Shapeshifter", "Sphinx", "Spirit", "Vedalken", "Wizard" };
    const char* const blackTribes[] = { "Assassin", "Cleric", "Demon", "Faerie", "Horror", "Insect", "Knight", "Nightmare", "Orc", "Phyrexian", "Pirate", "Rat", "Rogue", "Shade", "Skeleton", "Spirit", "Vampire", "Wizard", "Zombie" };
    const char* const redTribes[] = { "Artificer", "Beast", "Berserker", "Devil", "Dinosaur", "Dragon", "Dwarf", "Goblin", "Kavu", "Lizard", "Minotaur", "Ogre", "Orc", "Shaman", "Warrior", "Werewolf" };
    const char* const greenTribes[] = { "Archer", "Beast", "Cat", "Centaur", "Dinosaur", "Druid", "Dryad", "Elf", "Fungus", "Insect", "Kavu", "Lizard", "Mutant", "Plant", "Ranger", "Scout", "Shaman", "Snake", "Spider", "Treefolk", "Warrior", "Werewolf", "Wurm" };

    int multicolorTribesSize = sizeof(multicolorTribes)/sizeof(multicolorTribes[0]);
    int whiteTribesSize = sizeof(whiteTribes)/sizeof(whiteTribes[0]);
    int blueTribesSize = sizeof(blueTribes)/sizeof(blueTribes[0]);
    int blackTribesSize = sizeof(blackTribes)/sizeof(blackTribes[0]);
    int redTribesSize = sizeof(redTribes)/sizeof(redTribes[0]);
    int greenTribesSize = sizeof(greenTribes)/sizeof(greenTribes[0]);

    switch (tribeColor[0])
    {
        case Constants::MTG_COLOR_ARTIFACT :
            randomTribe = multicolorTribes[observer->getRandomGenerator()->random() % multicolorTribesSize];
            nbColors = 0;
            break;
        case Constants::MTG_COLOR_WHITE :
            randomTribe = whiteTribes[observer->getRandomGenerator()->random() % whiteTribesSize];
            break;
        case Constants::MTG_COLOR_BLUE :
            randomTribe = blueTribes[observer->getRandomGenerator()->random() % blueTribesSize];
            break;
        case Constants::MTG_COLOR_BLACK :
            randomTribe = blackTribes[observer->getRandomGenerator()->random() % blackTribesSize];
            break;
        case Constants::MTG_COLOR_RED :
            randomTribe = redTribes[observer->getRandomGenerator()->random() % redTribesSize];
            break;
        case Constants::MTG_COLOR_GREEN :
            randomTribe = greenTribes[observer->getRandomGenerator()->random() % greenTribesSize];
            break;
    }

    MTGDeck * tempDeck = NEW MTGDeck(MTGCollection());
    tempDeck->addRandomCards(14, 0, 0, -1, lands[tribeColor[0]].c_str());
    tempDeck->addRandomCards(6, 0, 0, 'R', lands[tribeColor[0]].c_str());
    tempDeck->addRandomCards(4, 0, 0, -1, "land");
    tempDeck->addRandomCards(20, 0, 0, -1, randomTribe);    
    tempDeck->addRandomCards(5, 0, 0, -1, "enchantment", tribeColor, nbColors);
    tempDeck->addRandomCards(5, 0, 0, -1, "instant", tribeColor, nbColors);
    tempDeck->addRandomCards(5, 0, 0, -1, "sorcery", tribeColor, nbColors);
    tempDeck->addRandomCards(1, 0, 0, -1, "battle", tribeColor, nbColors);

    string deckFile = "random";
    string deckFileSmall = "random";

    Player *player = NULL;
    if (!isAI) // Human Player
        player = NEW HumanPlayer(observer, deckFile, deckFileSmall, false, tempDeck);
    else
        player = NEW AIPlayerBaka(observer, deckFile, deckFileSmall, "", tempDeck);

    return player;
}

Player * Rules::initPlayer(GameObserver *g, int playerId)
{
    Player * p = g->players.size() > 1?g->players[playerId]:NULL;
    if (!p)
    {
        int isAI = 1;
        if (GameApp::players[playerId] == PLAYER_TYPE_HUMAN) isAI = 0;
        switch (gamemode)
        {
        case GAME_TYPE_MOMIR:
            return loadPlayerMomir(g, isAI);
        case GAME_TYPE_CLASSIC:
            return NULL; //Error for the time being
        case GAME_TYPE_COMMANDER:
            return NULL; //Error for the time being
        case GAME_TYPE_RANDOM1:
            return loadPlayerRandom(g, isAI, GAME_TYPE_RANDOM1);
        case GAME_TYPE_RANDOM2:
            return loadPlayerRandom(g, isAI, GAME_TYPE_RANDOM2);
         case GAME_TYPE_RANDOM3:
            return loadPlayerRandomThree(g, isAI);
         case GAME_TYPE_RANDOM5:
            return loadPlayerRandomFive(g, isAI);
         case GAME_TYPE_RANDOMCOMMANDER:
            return loadPlayerRandomCommander(g, isAI);
         case GAME_TYPE_RANDOMCOMMANDERFROMFILE:
            return loadPlayerRandomCommanderFromFile(g, isAI);
         case GAME_TYPE_HORDE:
            return loadPlayerHorde(g, isAI);
         case GAME_TYPE_SET_LIMITED:
             return loadRandomSetLimited(g, isAI);
        default:
            return NULL;
        }
    }
    //TODO p may still be NULL, what do we do to handle this? Above switch has no default case to handle the case where p is NULL 
    p->phaseRing = initState.playerData[playerId].player->phaseRing;
    p->offerInterruptOnPhase = initState.playerData[playerId].player->offerInterruptOnPhase;
    return p;
}

MTGDeck * Rules::buildDeck(int playerId)
{
    int nbcards = 0;
    MTGDeck * deck = NEW MTGDeck(MTGCollection());

    MTGGameZone * loadedPlayerZones[] = { initState.playerData[playerId].player->game->graveyard,
                                          initState.playerData[playerId].player->game->library,
                                          initState.playerData[playerId].player->game->hand,
                                          initState.playerData[playerId].player->game->inPlay,
                                          initState.playerData[playerId].player->game->exile,
                                          initState.playerData[playerId].player->game->commandzone,
                                          initState.playerData[playerId].player->game->sideboard };

    for (int j = 0; j < 7; j++)
    {
        for (size_t k = 0; k < loadedPlayerZones[j]->cards.size(); k++)
        {
            int cardid = loadedPlayerZones[j]->cards[k]->getId();
            deck->add(cardid);
            nbcards++;
        }
    }
    if (!nbcards)
    {
        delete (deck);
        return NULL;
    }
    return deck;
}

void Rules::initPlayers(GameObserver *g)
{
    for (int i = 0; i < 2; i++)
    {
        Player * p = initPlayer(g, i);
        if(p && g->getPlayersNumber() < 2)
            g->players.push_back(p);
        MTGDeck * deck = buildDeck(i);
        
        if (deck)
        {
            // TODO: p may be NULL, initPlayer(g, i) may return NULL, what do we do in this case?
            p->game->initDeck(deck);
            SAFE_DELETE(deck);
            p->game->setOwner(p);
        }
    }
}

#if defined(WAGIC_MEMPROBE) && defined(PSP)
//startGame stage timer (defined in GameObserver.cpp) — the initGame bracket
//measured a flat ~1064ms/match (2026-08-07); these sub-marks attribute it.
extern void sgMark(const char * tag);
#else
#define sgMark(x) ((void)0)
#endif

void Rules::initGame(GameObserver *g, bool currentPlayerSet)
{
    DebugTrace("RULES Init Game\n");

    //Set the current player/phase
    if (g->currentPlayer->playMode!= Player::MODE_TEST_SUITE &&  g->mRules->gamemode!= GAME_TYPE_STORY)
    {
        if(OptionWhosFirst::WHO_R == options[Options::FIRSTPLAYER].number)
            initState.player = g->getRandomGenerator()->random() % 2;
        if(OptionWhosFirst::WHO_O == options[Options::FIRSTPLAYER].number)
            initState.player = 1;
    }
    if(!currentPlayerSet)
    {
        g->currentPlayerId = initState.player;
    }
    g->currentPlayer =  g->players[g->currentPlayerId];
    g->currentActionPlayer = g->currentPlayer;
    g->phaseRing->goToPhase(0, g->currentPlayer, false);
    g->phaseRing->goToPhase(initState.phase, g->currentPlayer);
    g->setCurrentGamePhase(initState.phase);
    sgMark("ig goToPhase");

    for (int i = 0; i < 2; i++)
    {
        Player * p = g->players[i];
        p->life = initState.playerData[i].player->life;
        p->initLife = initState.playerData[i].player->life;
        p->poisonCount = initState.playerData[i].player->poisonCount;
        p->damageCount = initState.playerData[i].player->damageCount;
        p->preventable = initState.playerData[i].player->preventable;
        p->energyCount = initState.playerData[i].player->energyCount;
        p->experienceCount = initState.playerData[i].player->experienceCount;
        p->yidaroCount = initState.playerData[i].player->yidaroCount;
        p->ringTemptations = initState.playerData[i].player->ringTemptations;
        p->dungeonCompleted = initState.playerData[i].player->dungeonCompleted;
        p->numOfCommandCast = initState.playerData[i].player->numOfCommandCast;
        p->monarch = initState.playerData[i].player->monarch;
        p->initiative = initState.playerData[i].player->initiative;
        p->surveilOffset = initState.playerData[i].player->surveilOffset;
        p->devotionOffset = initState.playerData[i].player->devotionOffset;
        p->lastChosenName = initState.playerData[i].player->lastChosenName;
        if (initState.playerData[i].player->mAvatarName.size())
        {
            p->mAvatarName = initState.playerData[i].player->mAvatarName;
        }
        MTGGameZone * playerZones[] = { p->game->graveyard, p->game->library, p->game->hand, p->game->inPlay, p->game->exile , p->game->commandzone, p->game->sideboard };
        MTGGameZone * loadedPlayerZones[] = { initState.playerData[i].player->game->graveyard,
                                              initState.playerData[i].player->game->library,
                                              initState.playerData[i].player->game->hand,
                                              initState.playerData[i].player->game->inPlay,
                                              initState.playerData[i].player->game->exile,
                                              initState.playerData[i].player->game->commandzone,
                                              initState.playerData[i].player->game->sideboard };
        for (int j = 0; j < 7; j++)
        {
            MTGGameZone * zone = playerZones[j];
            for (size_t k = 0; k < loadedPlayerZones[j]->cards.size(); k++)
            {
                MTGCardInstance * card = getCardByMTGId(g, loadedPlayerZones[j]->cards[k]->getId(), p);
                if (card && zone != p->game->library)
                {
                    if (zone == p->game->inPlay)
                    {
                        //MTGCardInstance * copy = p->game->putInZone(card, p->game->library, p->game->stack);
                        MTGCardInstance * copy = zone->owner->game->putInZone(card, p->game->library, p->game->stack);
                        if (!copy)
                        {
                            LOG ("RULES ERROR, could not move card to stack\n");
                            continue;
                        }
                        Spell * spell = NEW Spell(g, copy);
                        spell->resolve();
                        delete spell;
                    }
                    else
                    {
                        if (!p->game->library->hasCard(card))
                        {
                            LOG ("RULES ERROR, CARD NOT FOUND IN LIBRARY\n");
                        }
                        //p->game->putInZone(card, p->game->library, zone);
                        zone->owner->game->putInZone(card, p->game->library, zone);
                    }
                }
                else
                {
                    if (!card)
                    {
                        LOG ("RULES ERROR, card is NULL\n");
                    }
                }
            }
        }
    }
    sgMark("ig zones");
    addExtraRules(g);
    sgMark("ig addExtraRules");

    postUpdateInitDone = false;
DebugTrace("RULES Init Game Done !\n");
}

//This function has all iitialization that can't be done in the "real" init function,
// because the first update call messes things up.
//It's a hack, ideally, the first update call shouldn't mess the init parameters...
void Rules::postUpdateInit(GameObserver* observer)
{
    if (postUpdateInitDone)
        return;
    for (int i = 0; i < 2; ++ i)
        observer->players[i]->getManaPool()->add(initState.playerData[i].player->getManaPool());
       // GameObserver::GetInstance()->players[i]->getManaPool()->copy(initState.playerData[i].manapool);
    postUpdateInitDone = true;
}

void RulesPlayerData::cleanup()
{
    SAFE_DELETE(player);
    player = new Player(NULL, "", "");
}

void RulesState::cleanup()
{
    for (int i = 0; i < 2; i++)
    {
        playerData[i].cleanup();
    }
}

void Rules::cleanup()
{
    initState.cleanup();
}

Rules::Rules(string _bg)
{
    bg = _bg;
    unlockOption = INVALID_OPTION;
    hidden = false;
    filename = "";
    postUpdateInitDone = false;
}

bool Rules::canChooseDeck() 
{
    return (gamemode == GAME_TYPE_CLASSIC || gamemode == GAME_TYPE_STONEHEWER || gamemode == GAME_TYPE_HERMIT || gamemode == GAME_TYPE_COMMANDER); 
}

int Rules::load(string _filename)
{
    //avoid non .txt files
    if (_filename.size() < 5 || (_filename.find(".txt") == string::npos && _filename.find(".TXT") == string::npos))
        return 0;

    if (!filename.size()) //this check is necessary because of the recursive calls (a file loads other files)
        filename = _filename;
    char c_filename[4096];
    if (fileExists(_filename.c_str()))
    {
        sprintf(c_filename, "%s", _filename.c_str());
    }
    else
    {
        sprintf(c_filename, "rules/%s", _filename.c_str());
    }
    std::string contents;
    if (!JFileSystem::GetInstance()->readIntoString(c_filename, contents))
        return 0;
    std::stringstream stream(contents);
    std::string s;

    int state = PARSE_UNDEFINED;

    cleanup();

    while (std::getline(stream, s))
    {
        if (!s.size()) continue;
        if (s[s.size() - 1] == '\r') s.erase(s.size() - 1); //Handle DOS files
        if (!s.size()) continue;
        if (s[0] == '#') continue;
        string scopy = s;
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s.find("include ") == 0)
        {
            load(s.substr(8));
            hidden = false; //To avoid transmitting the hidden param to children
            continue;
        }
        if (s.compare("[init]") == 0)
        {
            state = PARSE_INIT;
            continue;
        }
        if (s.compare("[players]") == 0)
        {
            state = PARSE_PLAYERS;
            continue;
        }
        if (s.compare("[player1]") == 0)
        {
            state = PARSE_PLAYER1;
            continue;
        }
        if (s.compare("[player2]") == 0)
        {
            state = PARSE_PLAYER2;
            continue;
        }

        switch (state)
        {
        case PARSE_UNDEFINED:
            if (s.find("name=") == 0)
            {
                displayName = scopy.substr(5);
            }
            else if (s.find("unlock=") == 0)
            {
                mUnlockOptionString = s.substr(7);
                unlockOption = Options::getID(mUnlockOptionString);
            }
            else if (s.find("hidden") == 0)
            {
                hidden = true;
            }
            break;
        case PARSE_INIT:
            if (s.find("auto=") == 0)
            {
                extraRules.push_back(s.substr(5));
            }
            else if (s.find("mode=") == 0)
            {
                gamemode = strToGameMode(s.substr(5));
            }
            else if (s.find("player=") == 0)
            {
                initState.player = atoi(s.substr(7).c_str()) - 1;
            }
            else
            {
                initState.phase = PhaseRing::phaseStrToInt(s);
            }
            break;
        case PARSE_PLAYER1:
            initState.parsePlayerState(0, s);
            break;
        case PARSE_PLAYER2:
            initState.parsePlayerState(1, s);
            break;
        case PARSE_PLAYERS:
            initState.parsePlayerState(0, s);
            initState.parsePlayerState(1, s);
            break;
        }
    }
    return 1;
}

GameType Rules::strToGameMode(string s)
{
    if (s.compare("momir") == 0) return GAME_TYPE_MOMIR;
    if (s.compare("random1") == 0) return GAME_TYPE_RANDOM1;
    if (s.compare("random2") == 0) return GAME_TYPE_RANDOM2;
    if (s.compare("random3") == 0) return GAME_TYPE_RANDOM3;
    if (s.compare("random5") == 0) return GAME_TYPE_RANDOM5;
    if (s.compare("random_commander") == 0) return GAME_TYPE_RANDOMCOMMANDER;
    if (s.compare("random_commander_from_file") == 0) return GAME_TYPE_RANDOMCOMMANDERFROMFILE;
    if (s.compare("horde") == 0) return GAME_TYPE_HORDE;
    if (s.compare("set_limited") == 0) return GAME_TYPE_SET_LIMITED;
    if (s.compare("story") == 0) return GAME_TYPE_STORY;
    if (s.compare("stonehewer") == 0) return GAME_TYPE_STONEHEWER;
    if (s.compare("hermit") == 0) return GAME_TYPE_HERMIT;
    if (s.compare("commander") == 0) return GAME_TYPE_COMMANDER;
    return GAME_TYPE_CLASSIC;
}
