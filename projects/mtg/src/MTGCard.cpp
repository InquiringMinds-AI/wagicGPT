//------------------------------------------------------
//MTGCard Class
//-------------------------------------------------
//TODO Fill BasicAbilities

#include "PrecompiledHeader.h"

#include "MTGDeck.h"
#include "MTGCard.h"
#include "CardPrimitive.h"
#include "Subtypes.h"
#include "Translate.h"

using std::string;

SUPPORT_OBJECT_ANALYTICS(MTGCard)

MTGCard::MTGCard()
{
    init();
}

MTGCard::MTGCard(int set_id)
{
    init();
    setId = set_id;
}

MTGCard::MTGCard(MTGCard * source)
{
    rarity = source->rarity;
    mtgid = source->mtgid;
    setId = source->setId;
    data = source->data;
}

MTGCard::~MTGCard()
{
}

int MTGCard::init()
{
    setId = 0;
    mtgid = 0;
    data = NULL;
    rarity = Constants::RARITY_C;
    return 1;
}

void MTGCard::setMTGId(int id)
{
    mtgid = id;
}

int MTGCard::getMTGId() const
{
    return mtgid;
}
int MTGCard::getId() const
{
    return mtgid;
}
char MTGCard::getRarity() const
{
    return rarity;
}

void MTGCard::setRarity(char _rarity)
{
    rarity = _rarity;
}

const string MTGCard::getImageName()
{
    //#W54-J (A5): this runs once per visible card per frame (RetrieveCard);
    //a stack snprintf instead of a std::stringstream (locale + heap buffer).
    //Tokens (negative id) name their image "<abs id>t.jpg".
    char buf[32];
    if (mtgid < 0)
        snprintf(buf, sizeof(buf), "%dt.jpg", -mtgid);
    else
        snprintf(buf, sizeof(buf), "%d.jpg", mtgid);
    return buf;
}

void MTGCard::setPrimitive(CardPrimitive * cp)
{
    data = cp;
}
