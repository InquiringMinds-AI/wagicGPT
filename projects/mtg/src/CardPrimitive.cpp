#include "PrecompiledHeader.h"

#include "MTGDefinitions.h"
#include "CardPrimitive.h"

#include "MTGDeck.h"
#include "Subtypes.h"
#include "Translate.h"
#include "GameApp.h"
#include "ExtraCost.h"

using std::string;

namespace
{
    /**
    ** Count the number of set bits in a given integer 
    */
    int CountSetBits(int n)
    {
        int count = 0;
        while (n)
        {
            n &= (n-1);
            count++;
        }
        return count;
    }
}


SUPPORT_OBJECT_ANALYTICS(CardPrimitive)

CardPrimitive::CardPrimitive()
    : colors(0)
{
#if defined (PSP)
    mMagicMaterialized = false;
#endif
    init();
}

CardPrimitive::CardPrimitive(CardPrimitive * source)
{
#if defined (PSP)
    mMagicMaterialized = true;  // copies receive materialized content below
#endif
    if(!source)
        return;
#if defined (PSP)
    source->materializeMagicText();  // universal seam: every in-game card copies through here
#endif
    basicAbilities = source->basicAbilities;
    LKIbasicAbilities = source->basicAbilities;

    for (size_t i = 0; i < source->types.size(); ++i)
        types.push_back(source->types[i]);
    colors = source->colors;
    manaCost.copy(source->getManaCost());
    //reducedCost.copy(source->getReducedManaCost());
    //increasedCost.copy(source->getIncreasedManaCost());
    if(source->getManaCost()->getAlternative())
        manaCost.getAlternative()->alternativeName = source->getManaCost()->getAlternative()->alternativeName;
    if(source->getManaCost()->getKicker())
        manaCost.getKicker()->alternativeName = source->getManaCost()->getKicker()->alternativeName;
    if(source->getManaCost()->getRetrace())
        manaCost.getRetrace()->alternativeName = source->getManaCost()->getRetrace()->alternativeName;

    text = source->text;
    formattedText = source->formattedText;
    setName(source->name);

    setdoubleFaced(source->doubleFaced);
    setAICustomCode(source->AICustomCode);
    setCrewAbility(source->CrewAbility);
    setPhasedOutAbility(source->PhasedOutAbility);
    setModularValue(source->ModularValue);
    power = source->power;
    toughness = source->toughness;
    restrictions = source->restrictions ? source->restrictions->clone() : NULL;
    suspendedTime = source->suspendedTime;
    dredgeAmount = source->dredgeAmount;

    magicText = source->magicText;
    for (map<string, string>::const_iterator it = source->magicTexts.begin(); it != source->magicTexts.end(); ++it)
        magicTexts[it->first] = source->magicTexts[it->first];
    spellTargetType = source->spellTargetType;
    alias = source->alias;
}

CardPrimitive::~CardPrimitive()
{
    SAFE_DELETE(restrictions);
}

int CardPrimitive::init()
{
    basicAbilities.reset();

    types.clear();

    nameOrig = "";
    magicText = "";
    magicTexts.clear();
    spellTargetType = "";
    alias = 0;
    restrictions = NULL;
    dredgeAmount = 0;

    power = 0;
    toughness = 0;
    return 1;
}

bool CardPrimitive::isCreature()
{
    return hasSubtype(Subtypes::TYPE_CREATURE);
}

bool CardPrimitive::isLand()
{
    return hasSubtype(Subtypes::TYPE_LAND);
}

bool CardPrimitive::isSpell()
{
    return (!isCreature() && !isLand());
}

bool CardPrimitive::isPermanent()
{
    return (!isSorceryorInstant());
}

bool CardPrimitive::isSorceryorInstant()
{
    if(hasSubtype(Subtypes::TYPE_SORCERY)||hasSubtype(Subtypes::TYPE_INSTANT))
        return true;
    return false;
}

int CardPrimitive::dredge()
{
    return dredgeAmount;
}

void CardPrimitive::setRestrictions(string _restriction)
{
    if (!restrictions)
        restrictions = NEW CastRestrictions();

    restrictions->restriction = _restriction;
}

const string CardPrimitive::getRestrictions()
{
    if (!restrictions)
        return "";

    return restrictions->restriction;
}

void CardPrimitive::setOtherRestrictions(string _restriction)
{
    if (!restrictions)
        restrictions = NEW CastRestrictions();

    restrictions->otherrestriction = _restriction;
}

const string CardPrimitive::getOtherRestrictions()
{
    if (!restrictions)
        return "";

    return restrictions->otherrestriction;
}

void CardPrimitive::setColor(const string& _color, int removeAllOthers)
{
    for( size_t i =0 ; i< Constants::MTGColorStrings.size(); i++)
    {
        if (_color.compare(Constants::MTGColorStrings[i]) == 0)
            return setColor(i, removeAllOthers);
    }
    //Keep artifact compare, to Remove this we need change all MTG.txt
    if (_color.compare("artifact") == 0)
        return setColor(Constants::MTG_COLOR_ARTIFACT, removeAllOthers);
}

void CardPrimitive::setColor(int _color, int removeAllOthers)
{
    if (removeAllOthers)
        colors = 0;

    colors |= ConvertColorToBitMask(_color);
}

void CardPrimitive::removeColor(int _color)
{
    uint8_t mask = ~ConvertColorToBitMask(_color);
    colors &= mask;
}

int CardPrimitive::getColor()
{
    if (colors)
    {
        for (int i = 1; i < Constants::NB_Colors; i++)
            if (hasColor(i))
                return i;
    }

    return 0;
}

bool CardPrimitive::hasColor(int inColor)
{
    return (colors & ConvertColorToBitMask(inColor)) > 0;
}

int CardPrimitive::countColors()
{
    uint8_t mask = kColorBitMask_Green | kColorBitMask_Blue | kColorBitMask_Red | kColorBitMask_Black | kColorBitMask_White;
    mask &= colors;

    return CountSetBits(mask);
}

void CardPrimitive::setManaCost(const string& s)
{
    ManaCost::parseManaCost(s, &manaCost);
    for (int i = Constants::MTG_COLOR_GREEN; i <= Constants::MTG_COLOR_WHITE; i++)
    {
        if (manaCost.hasColor(i))
        {
            setColor(i);
        }
    }
    //X + convoke composition (CR 702.51 + 601.2b). A convoke card models its
    //convoke as an ALTERNATIVE cost - a {0}+Convoke shell parsed from the
    //`other={convoke}` line, which is read BEFORE this printed cost. When the
    //printed cost carries {X}, that {X} must ride the alternative shell so
    //MTGAlternativeCostRule announces X when the "cast with convoke" variant is
    //chosen. Without it the alternative has no X, the announce block is skipped,
    //and the spell casts at X=0 (March of the Multitudes: 0/17 Soldier tokens,
    //corpus 20260725). The convoke extra still reduces the printed {G}{W}{W}
    //colored requirement from tapped creatures; the announced X is paid from the
    //pool - a legal payment (CR 601.2h) that yields X>0 tokens. Guarded on an
    //actual Convoke extra so no other alternative (offering/delve/flashback)
    //inherits a spurious X.
    if (manaCost.hasX() && manaCost.getAlternative() && manaCost.getAlternative()->extraCosts)
    {
        ExtraCosts * aec = manaCost.getAlternative()->extraCosts;
        for (size_t xi = 0; xi < aec->costs.size(); xi++)
        {
            if (dynamic_cast<Convoke*>(aec->costs[xi]))
            {
                manaCost.getAlternative()->x();
                break;
            }
        }
    }
}

void CardPrimitive::setType(const string& _type_text)
{
    setSubtype(_type_text);
}

void CardPrimitive::addType(char * _type_text)
{
    setSubtype(_type_text);
}

void CardPrimitive::setSubtype(const string& value)
{
    //find the parent type for this card
    int parentType = 0;
    for (size_t i = 0; i < types.size(); ++i)
    {
        if (MTGAllCards::isType(types[i]))
        {
            parentType = types[i];
            break;
        }
    }

    // "Artifact Creature" should have "Creature" as parent type
    if (parentType == Subtypes::TYPE_ARTIFACT && isCreature())
        parentType = Subtypes::TYPE_CREATURE;

    int id = MTGAllCards::add(value, parentType);
    addType(id);
}

void CardPrimitive::addType(int id)
{
    types.push_back(id);
}

//TODO Definitely move some of these functions to CardInstance. There is no reason to remove a type from an CardPrimitive since they represent the Database
//Removes a type from the types of a given card
//If removeAll is true, removes all occurences of this type, otherwise only removes the first occurence
int CardPrimitive::removeType(string value, int removeAll)
{

    int id = MTGAllCards::findType(value);
    return removeType(id, removeAll);
}

int CardPrimitive::removeType(int id, int removeAll)
{
    int result = 0;
    for (int i = types.size() - 1; i >= 0; i--)
    {
        if (types[i] == id)
        {
            types.erase(types.begin() + i);
            result++;
            if (!removeAll)
                return result;
        }
    }
    return result;
}

#if defined (PSP)
// ---- card-data sidecars (see CardPrimitive.h) ----
// Two deploy-time file pairs at the Res root (NOT sets/primitives/ — that dir is
// scanned and every file in it is parsed as card data):
//   cardtext.{idx,dat} — display text (text= lines), fetched at render time.
//   cardauto.{idx,dat} — raw auto*/anyzone lines, replayed through addMagicText
//                        when a primitive is first used in a game.
// idx: u32 count, then count * {u32 fnv1a(lowercased name), u32 offset, u32 len},
//      sorted by hash (little-endian).
// dat: entry at offset = lowercased name, '\n', payload; len covers the whole entry.
// Hash collisions resolve by comparing the stored name.
namespace
{
    struct SidecarEnt { unsigned int hash, offset, len; };

    unsigned int sidecarFnv1a(const string& s)
    {
        unsigned int h = 2166136261u;
        for (size_t i = 0; i < s.size(); ++i)
        {
            h ^= (unsigned char) s[i];
            h *= 16777619u;
        }
        return h;
    }

    struct Sidecar
    {
        std::vector<SidecarEnt> idx;
        bool tried;
        FILE * dat;

        Sidecar() : tried(false), dat(NULL) {}

        void load(const char * idxPath, const char * datPath, const char * label)
        {
            tried = true;
            FILE * f = fopen(idxPath, "rb");
            unsigned int count = 0;
            if (f)
            {
                if (fread(&count, 4, 1, f) == 1 && count && count < 200000)
                {
                    idx.resize(count);
                    if (fread(&idx[0], sizeof(SidecarEnt), count, f) != count)
                        idx.clear();
                }
                fclose(f);
            }
            if (idx.size())
                dat = fopen(datPath, "rb");
            if (!dat)
                idx.clear();
#if defined(WAGIC_AUTODEMO) || defined(WAGIC_HWPROBE)
            FILE * p = fopen("User/wagic-probe.log", "a");
            if (p)
            {
                fprintf(p, "%s sidecar: idx=%s count=%u dat=%s -> %s\n", label,
                    f ? "open" : "MISSING", count, dat ? "open" : "MISSING",
                    idx.empty() ? "OFFLOAD OFF" : "OFFLOAD ON");
                fclose(p);
            }
#else
            (void) label;
#endif
        }

        bool fetch(const string& cardName, string& out)
        {
            if (idx.empty()) return false;
            string lower = cardName;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            unsigned int h = sidecarFnv1a(lower);
            size_t lo = 0, hi = idx.size();
            while (lo < hi)
            {
                size_t mid = (lo + hi) / 2;
                if (idx[mid].hash < h) lo = mid + 1;
                else hi = mid;
            }
            for (; lo < idx.size() && idx[lo].hash == h; ++lo)
            {
                string buf;
                buf.resize(idx[lo].len);
                if (fseek(dat, idx[lo].offset, SEEK_SET)) return false;
                if (fread(&buf[0], 1, buf.size(), dat) != buf.size()) return false;
                size_t nl = buf.find('\n');
                if (nl == string::npos) continue;
                if (buf.compare(0, nl, lower)) continue;  // hash collision — try next
                out = buf.substr(nl + 1);
                return true;
            }
            return false;
        }
    };

    Sidecar gTextSidecar;
    Sidecar gAutoSidecar;
    bool gMaterializing = false;  // addMagicText bypass during replay
}

bool CardPrimitive::textOffloadActive()
{
    if (!gTextSidecar.tried)
        gTextSidecar.load("Res/cardtext.idx", "Res/cardtext.dat", "cardtext");
    return !gTextSidecar.idx.empty();
}

bool CardPrimitive::fetchOffloadedText(const string& cardName, string& out)
{
    if (!textOffloadActive()) return false;
    return gTextSidecar.fetch(cardName, out);
}

bool CardPrimitive::autoOffloadActive()
{
    if (!gAutoSidecar.tried)
        gAutoSidecar.load("Res/cardauto.idx", "Res/cardauto.dat", "cardauto");
    return !gAutoSidecar.idx.empty();
}

// Replay this primitive's auto*/anyzone lines from the sidecar, mirroring
// MTGAllCards::processConfLine's three magic-text branches exactly.
void CardPrimitive::materializeMagicText()
{
    if (mMagicMaterialized) return;
    mMagicMaterialized = true;
    if (!autoOffloadActive()) return;
    string payload;
    if (!gAutoSidecar.fetch(name, payload)) return;
    gMaterializing = true;
    size_t pos = 0;
    while (pos < payload.size())
    {
        size_t end = payload.find('\n', pos);
        if (end == string::npos) end = payload.size();
        string line = payload.substr(pos, end - pos);
        pos = end + 1;
        size_t eq = line.find('=');
        if (eq == string::npos || !eq) continue;
        string key = line.substr(0, eq);
        string val = line.substr(eq + 1);
        if (key == "auto")
            addMagicText(val);
        else if (key.compare(0, 4, "auto") == 0)
            addMagicText(val, key.substr(4));
        else if (key == "anyzone")
        {
            addMagicText(val, "hand");
            addMagicText(val, "library");
            addMagicText(val, "graveyard");
            addMagicText(val, "stack");
            addMagicText(val, "exile");
            addMagicText(val, "commandzone");
            addMagicText(val, "reveal");
            addMagicText(val, "sideboard");
            addMagicText(val);
        }
    }
    gMaterializing = false;
}
#endif

void CardPrimitive::setText(const string& value)
{
#if defined (PSP)
    if (textOffloadActive()) return;  // served from the sidecar at render time
#endif
    text = value;
}

/* This alters the card structure, but this is intentional for performance and
*  space purpose: The only time we get the card text is to render it
*  on the screen, in a formatted way.
* Formatting the string every frame is not efficient, especially since we always display it the same way
* Formatting all strings at startup is inefficient too.
* Instead, we format when requested, but only once, and cache the result.
* To avoid memory to blow up, in exchange of the cached result, we erase the original string
*/
const vector<string>& CardPrimitive::getFormattedText(bool noremove)
{
#if defined (PSP)
    if (!text.size() && formattedText.empty())
        fetchOffloadedText(name, text);  // miss (textless card) leaves text empty
#endif
    if (!text.size())
        return formattedText;

    std::string::size_type found = text.find_first_of("{}");
    if(!noremove)
    {
        while (found != string::npos)
        {
            text[found] = '/';
            found = text.find_first_of("{}", found + 1);
        }
    }
    int defL = noremove?44:0;
    WFont * mFont = WResourceManager::Instance()->GetWFont(Fonts::MAGIC_FONT);
    mFont->FormatText(text, formattedText, defL);

    text = "";

    return formattedText;
};

void CardPrimitive::addMagicText(string value)
{
#if defined (PSP)
    if (!gMaterializing && autoOffloadActive()) return;  // replayed from sidecar on first game use
#endif
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    if (magicText.size())
        magicText.append("\n");
    magicText.append(value);
}

void CardPrimitive::addMagicText(string value, string key)
{
#if defined (PSP)
    if (!gMaterializing && autoOffloadActive()) return;  // replayed from sidecar on first game use
#endif
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    if (magicTexts[key].size())
        magicTexts[key].append("\n");
    magicTexts[key].append(value);
}

void CardPrimitive::setdoubleFaced(const string& value)
{
    doubleFaced = value;
    std::transform(doubleFaced.begin(), doubleFaced.end(), doubleFaced.begin(), ::tolower);
}

const string& CardPrimitive::getdoubleFaced() const
{
    return doubleFaced;
}

void CardPrimitive::setAICustomCode(const string& value)
{
    AICustomCode = value;
    std::transform(AICustomCode.begin(), AICustomCode.end(), AICustomCode.begin(), ::tolower);
}

const string& CardPrimitive::getAICustomCode() const
{
    return AICustomCode;
}

void CardPrimitive::setCrewAbility(const string& value)
{
    CrewAbility = value;
    std::transform(CrewAbility.begin(), CrewAbility.end(), CrewAbility.begin(), ::tolower);
}

const string& CardPrimitive::getCrewAbility() const
{
    return CrewAbility;
}

void CardPrimitive::setPhasedOutAbility(const string& value)
{
    PhasedOutAbility = value;
    std::transform(PhasedOutAbility.begin(), PhasedOutAbility.end(), PhasedOutAbility.begin(), ::tolower);
}

const string& CardPrimitive::getPhasedOutAbility() const
{
    return PhasedOutAbility;
}

void CardPrimitive::setModularValue(const string& value)
{
    ModularValue = value;
    std::transform(ModularValue.begin(), ModularValue.end(), ModularValue.begin(), ::tolower);
}

const string& CardPrimitive::getModularValue() const
{
    return ModularValue;
}

void CardPrimitive::setName(const string& value)
{
    name = value;
    lcname = value;
    std::transform(lcname.begin(), lcname.end(), lcname.begin(), ::tolower);
}

const string& CardPrimitive::getName() const
{
    return name;
}

const string& CardPrimitive::getLCName() const
{
    return lcname;
}

ManaCost* CardPrimitive::getManaCost()
{
    return &manaCost;
}

bool CardPrimitive::hasType(int _type)
{
    if (types.size() > 400) {return false;} // Null pointer?
    for (size_t i = 0; i < types.size(); i++)
        if (types[i] == _type)
            return true;            
    return false;
}

bool CardPrimitive::hasSubtype(int _subtype)
{
    return hasType(_subtype);
}

bool CardPrimitive::hasType(const string& _type)
{
    int id = MTGAllCards::findType(_type);
    return hasType(id);
}

bool CardPrimitive::hasSubtype(const string& _subtype)
{
    int id = MTGAllCards::findType(_subtype);
    return hasType(id);
}

int CardPrimitive::has(int basicAbility)
{
    return basicAbilities[basicAbility];
}

//---------------------------------------------
// Creature specific
//---------------------------------------------
void CardPrimitive::setPower(int _power)
{
    power = _power;
}

int CardPrimitive::getPower()
{
    return power;
}

void CardPrimitive::setToughness(int _toughness)
{
    toughness = _toughness;
}

int CardPrimitive::getToughness()
{
    return toughness;
}

uint8_t CardPrimitive::ConvertColorToBitMask(int inColor)
{
    uint8_t value = 0;
    switch (inColor)
    {
    case Constants::MTG_COLOR_ARTIFACT:
        value = kColorBitMask_Artifact;
        break;

    case Constants::MTG_COLOR_GREEN:
        value = kColorBitMask_Green;
        break;

    case Constants::MTG_COLOR_BLUE:
        value = kColorBitMask_Blue;
        break;

    case Constants::MTG_COLOR_RED:
        value = kColorBitMask_Red;
        break;

    case Constants::MTG_COLOR_BLACK:
        value = kColorBitMask_Black;
        break;

    case Constants::MTG_COLOR_WHITE:
        value = kColorBitMask_White;
        break;

    case Constants::MTG_COLOR_LAND:
        value = kColorBitMask_Land;
        break;
    case Constants::MTG_COLOR_WASTE://the true colorless mana shares the kbitmask of land. kbitmask dictates the color of the quad(no image boarder), and the symbol. nothing more.
        value = kColorBitMask_Land;
        break;

    default:
        break;
    }

    return value;
}
