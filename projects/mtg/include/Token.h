#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "MTGCardInstance.h"

class Token: public MTGCardInstance
{
    MTGCardInstance * tokenSource;
public:
    Token(string _name, MTGCardInstance * source, int _power = 0, int _toughness = 0);
    Token(int id);
    Token(const Token& source);
    virtual MTGCardInstance* clone();
    //W35 narration: the card that CREATED this token, so the game log can
    //attribute it ("March from the Black Gate created a 0/0 Orc Army token")
    //instead of narrating a token that simply appeared.
    MTGCardInstance * getTokenSource() const { return tokenSource; }
};

#endif
