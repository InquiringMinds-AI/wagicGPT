#ifndef _COUNTERS_H_
#define _COUNTERS_H_
#include <string>

using std::string;
class MTGCardInstance;

/* One family of counters. Ex : +1/+1 */
class Counter
{
public:
    string name;
    int nb;
    int maxNb;
    int power, toughness;
    MTGCardInstance * target;
    Counter(MTGCardInstance * _target, int _power, int _toughness);
    Counter(MTGCardInstance * _target, const char * _name, int _power = 0, int _toughness = 0);
    int init(MTGCardInstance * _target, const char * _name, int _power, int _toughness);
    bool sameAs(const char * _name, int _power, int _toughness);
    bool cancels(int _power, int _toughness);
    int cancelCounter(int power, int toughness, MTGCardInstance * _source);
    int added();
    int removed();
};

/* Various families of counters attached to an instance of a card */
class Counters
{
public:
    int mCount;
    vector<Counter *>counters;
    MTGCardInstance * target;
    Counters(MTGCardInstance * _target);
    ~Counters();
    //_batchManaged: caller adds/removes several counters as ONE game event and
    //emits its own WEventTotalCounters batch afterwards (AACounter, CounterCost,
    //the Damage.cpp loops) - suppresses the per-call batch event below so
    //plus-riders (Winding Constrictor et al.) see one event per batch, not one
    //per counter. Callers that add a single counter leave it false and the
    //event is emitted here - before 2026-08-06 the ~23 raw call sites (evolve,
    //wither, persist...) emitted no batch event at all, so every plus-rider
    //card silently missed them.
    int addCounter(const char * _name, int _power = 0, int _toughness = 0, bool _noevent = false, bool duplicated = false, MTGCardInstance * _source = NULL, bool _batchManaged = false);
    int addCounter(int _power, int _toughness);
    int addCounter(int _power, int _toughness, bool _noevent);
    int removeCounter(const char * _name, int _power = 0, int _toughness = 0, bool _noevent = false, bool duplicated = false, MTGCardInstance * _source = NULL, bool _batchManaged = false);
    int removeCounter(int _power, int _toughness);
    //One WEventTotalCounters for a batch a caller managed with _batchManaged.
    //No-op when totalamount is 0.
    void emitTotalCountersEvent(const char * _name, int _power, int _toughness, bool added, int totalamount, MTGCardInstance * _source);
    Counter * hasCounter(const char * _name, int _power = 0, int _toughness = 0);
    Counter * hasCounter(int _power, int _toughness);
    Counter * getNext(Counter * previous = NULL);
    int init();
};

#endif
