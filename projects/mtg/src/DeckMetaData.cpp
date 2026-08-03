#include "PrecompiledHeader.h"

#include "DeckMetaData.h"
#include "DeckStats.h"
#include "MTGDeck.h"
#include "utils.h"
#include "Translate.h"

//Possible improvements:
//Merge this with DeckStats
//Have this class handle all the Meta Data rather than relying on MTGDeck. Then MTGDeck would have a MetaData object...

DeckMetaData::DeckMetaData(const string& filename, bool isAI)
    : mFilename(filename), mGamesPlayed(0), mVictories(0), mPercentVictories(0), mDifficulty(0),
      mDeckLoaded(false), mStatsLoaded(false), mIsAI(isAI)
{
    // Eager load, not deferrable as-is: the deck list is sorted alphabetically, so the deck file
    // must be opened here just to get its name. For the opponent list that means cracking open
    // 106 files only to read deck names — the bulk of the ~4 second delay the first time an
    // opponent is picked on the first match.
    LoadDeck();
}

void DeckMetaData::LoadDeck()
{
    if (!mDeckLoaded)
    {
        MTGDeck deck(mFilename.c_str(), NULL, 1);
        mName = trim(deck.meta_name);
        mDescription = trim(deck.meta_desc);
        mDeckId = atoi((mFilename.substr(mFilename.find("deck") + 4, mFilename.find(".txt"))).c_str());
        mCommanderDeck = deck.meta_commander; //Added to read the command tag in deck's metafile.

        vector<string> requirements = split(deck.meta_unlockRequirements, ',');
        for(size_t i = 0; i < requirements.size(); ++i)
        {
            mUnlockRequirements.push_back(Options::getID(requirements[i]));
        }

        mDeckLoaded = true;
        if (!mIsAI)
            mAvatarFilename = "avatar.jpg";
        else
        {
            ostringstream avatarFilename;
            avatarFilename << "avatar" << getAvatarId() << ".jpg";
            mAvatarFilename = avatarFilename.str();
        }        
    }

}

void DeckMetaData::LoadStats()
{
    if (!mStatsLoaded)
    {
        DeckStats * stats = DeckStats::GetInstance();
        if (mIsAI)
        {
            mPercentVictories = 0;
            mVictories = 0;
            mGamesPlayed = 0;
            mColorIndex = "";
            mDifficulty = 0;

            stats->load(mPlayerDeck);
            DeckStat * opponentDeckStats = stats->getDeckStat(mStatsFilename);
            if (opponentDeckStats)
            {
                mPercentVictories = opponentDeckStats->percentVictories();
                mVictories = opponentDeckStats->victories;
                mGamesPlayed = opponentDeckStats->nbgames;
                mColorIndex = opponentDeckStats->manaColorIndex;
 
                if (mPercentVictories < 34)
                {
                    mDifficulty = HARD;
                }
                else if (mPercentVictories < 55)
                {
                    mDifficulty = NORMAL;
                }
                else
                {
                    mDifficulty = EASY;
                }
                mStatsLoaded = true;
            }
        }
        else
        {
            if (FileExists(mStatsFilename))
            {
                stats->load(mStatsFilename);
                mGamesPlayed = stats->nbGames();               
                mPercentVictories = stats->percentVictories();
                mVictories = static_cast<int>(mGamesPlayed * (mPercentVictories / 100.0f));
                mStatsLoaded = true;
            }
        }

    }

}

// Removed the previous limit of 99 images, if "avatarXX.jpg" image is not present, for AI it will be used "baka.jpg" image instead.
int DeckMetaData::getAvatarId()
{
    return mDeckId;
}

//Accessors

bool DeckMetaData::isCommanderDeck()
{
    return mCommanderDeck;
}

string DeckMetaData::getFilename()
{
    return mFilename;
}

string DeckMetaData::getName()
{
    return mName;
}

int DeckMetaData::getDeckId()
{
    return mDeckId;
}

vector<int> DeckMetaData::getUnlockRequirements()
{
    return mUnlockRequirements;
}

string DeckMetaData::getAvatarFilename()
{
    return mAvatarFilename;
}

string DeckMetaData::getColorIndex()
{
    return mColorIndex;
}

int DeckMetaData::getGamesPlayed()
{
    return mGamesPlayed;
}

int DeckMetaData::getVictories()
{
    return mVictories;
}

int DeckMetaData::getVictoryPercentage()
{
    return mPercentVictories;
}

int DeckMetaData::getDifficulty()
{
    return mDifficulty;
}

string DeckMetaData::getDifficultyString()
{
    string difficultyString = _("Normal").c_str();
    switch (mDifficulty)
    {
    case HARD:
        difficultyString = _("Hard").c_str();
        break;
    case EASY:
        difficultyString = _("Easy").c_str();
        break;
    }

    return difficultyString;
}

string DeckMetaData::getDescription()
{
    return mDescription;
}

string DeckMetaData::getStatsSummary()
{
    LoadStats();

    ostringstream statsSummary;
    statsSummary << _("Difficulty: ").c_str() << _(getDifficultyString()) << endl
        << _("Victory %: ").c_str() << getVictoryPercentage() << endl
                    << _("Games Played: ").c_str() << getGamesPlayed() << endl;

    return statsSummary.str();
}

void DeckMetaData::setColorIndex(const string& colorIndex)
{
    mColorIndex = colorIndex;
}

void DeckMetaData::setDeckName(const string& newDeckTitle)
{
    mName = newDeckTitle;
}

void DeckMetaData::Invalidate()
{
    mStatsLoaded = false;
}
