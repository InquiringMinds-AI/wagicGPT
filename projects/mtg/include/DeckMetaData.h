#ifndef _DECKMETADATA_H_
#define _DECKMETADATA_H_

#include <string>
#include <vector>
#include <map>
#include "DeckStats.h"

using namespace std;
enum DECK_DIFFICULTY
{
    HARD = -1,
    NORMAL = 0,
    EASY = 1
};

class DeckMetaData
{
private:
    string mFilename;
    string mDescription;
    string mName;
    vector<int> mUnlockRequirements;
    int mDeckId;
    string mAvatarFilename;
    string mColorIndex;
    bool mCommanderDeck; //Added to read the command tag in deck's metafile.

    // statistical information
    int mGamesPlayed, mVictories, mPercentVictories, mDifficulty;
    int getAvatarId();

    DeckMetaData();

public:

    DeckMetaData(const string& filename, bool isAI = false);
    void LoadDeck();
    void LoadStats();

    // Reads a deck file's comment-line metadata with MTGDeck's exact semantics, so listing
    // decks never constructs an MTGDeck. Card lines are skipped, not resolved.
    // Returns false if the file could not be read; the outputs still hold their defaults.
    static bool ReadFileMetaData(const string& filename, string& name, string& description,
                                 string& unlockRequirements, bool& isCommanderDeck);

    // Accessors
    bool isCommanderDeck(); //Added to read the command tag in deck's metafile.
    string getFilename();
    string getDescription();
    string getName();
    string getAvatarFilename();
    // Existence of the avatar image, checked with file stats (not a texture
    // load) and cached for the object's lifetime. DeckMenuItem used to answer
    // this question with RetrieveTexture - a full JPEG decode per menu item
    // per menu construction, measured at ~1.4s for the 19-deck opponent menu.
    bool avatarExists();
    string getColorIndex();
    string getStatsSummary();
    vector<int> getUnlockRequirements();

    int getDeckId();
    int getGamesPlayed();
    int getVictories();
    int getVictoryPercentage();
    int getDifficulty();
    string getDifficultyString();

    // setters
    void setColorIndex(const string& colorIndex);
    void setDeckName( const string& newDeckTitle );
    void Invalidate();

    string mStatsFilename;
    string mPlayerDeck;
    bool mDeckLoaded;
    bool mStatsLoaded;
    bool mIsAI;
    bool mAvatarChecked;
    bool mAvatarFound;
};


#endif
