#ifndef _PLAYER_DATA_H_
#define _PLAYER_DATA_H_

#include "MTGDeck.h"
#include "Tasks.h"

class PlayerData
{
protected:
    void init();
public:
    int credits;
    map<string, string> storySaves;
    MTGDeck * collection;
    TaskList * taskList;
    //Fingerprint of the collection state at load. save() skips the
    //collection write (a .tmp + rename = two slow Memory Stick FAT ops)
    //when the state is unchanged - the common case on the victory screen,
    //where the collection is loaded ~100ms before being saved back.
    string mCollectionFP;
    PlayerData(); //This doesn't init the collection, do not use it to manipulate the player's collection
    PlayerData(MTGAllCards * allcards);
    ~PlayerData();
    int save();
};

#endif
