#ifndef _CACHE_ENGINE_H_
#define _CACHE_ENGINE_H_

#include "PrecompiledHeader.h"

#include "Threading.h"
#include <queue>
#include <deque>
#include <set>


struct CacheRequest
{
    CacheRequest()
    {
    }

    CacheRequest(std::string inFilename, int inSubmode, int inCacheID)
        : filename(inFilename), submode(inSubmode), cacheID(inCacheID)
    {
    }

    std::string filename;
    int submode;
    int cacheID;
};

const boost::posix_time::milliseconds kIdleTime(100);


class CardRetrieverBase
{
public:

    CardRetrieverBase(WCache<WCachedTexture,JTexture>& inCache)
        : mTextureCache(inCache)
    {
    }

    virtual ~CardRetrieverBase()
    {
    }

    virtual void QueueRequest(const std::string& inFilePath, int inSubmode, int inCacheID) = 0;

    // Called once per frame from the main loop. Threaded retrievers decode on
    // their worker thread and ignore this; unthreaded ones drain here.
    virtual void Service(int /*inMaxDecodes*/)
    {
    }

protected:

    WCache<WCachedTexture,JTexture>& mTextureCache;
};

/*
**
*/
class UnthreadedCardRetriever : public CardRetrieverBase
{
public:

    UnthreadedCardRetriever(WCache<WCachedTexture,JTexture>& inCache)
        : CardRetrieverBase(inCache)
    {
        DebugTrace("Unthreaded version");
    }

    virtual ~UnthreadedCardRetriever()
    {
    }

    /*
    **  No worker thread, but decoding inline stalls the render pass for the full
    **  JPEG decode. Instead, queue the request and drain a bounded number per
    **  frame from Service() (called by the main loop). RetrieveCard re-requests a
    **  missing card every frame it stays visible, so refreshing a duplicate to the
    **  back of the deque makes the back end "what is on screen right now" — Service
    **  drains newest-first.
    */
    void QueueRequest(const std::string& inFilePath, int inSubmode, int inCacheID)
    {
        if (mRequestLookup.find(inCacheID) != mRequestLookup.end())
        {
            // Already queued: refresh to the back (newest) so visible cards win.
            for (std::deque<CacheRequest>::iterator it = mRequestQueue.begin(); it != mRequestQueue.end(); ++it)
            {
                if (it->cacheID == inCacheID)
                {
                    CacheRequest request = *it;
                    mRequestQueue.erase(it);
                    mRequestQueue.push_back(request);
                    break;
                }
            }
            return;
        }

        mRequestLookup.insert(inCacheID);
        mRequestQueue.push_back(CacheRequest(inFilePath, inSubmode, inCacheID));

        // Drop the oldest (least recently visible) requests beyond the cap; they
        // re-queue themselves if still on screen.
        while (mRequestQueue.size() > kMaxQueuedRequests)
        {
            mRequestLookup.erase(mRequestQueue.front().cacheID);
            mRequestQueue.pop_front();
        }
    }

    void Service(int inMaxDecodes)
    {
        while (inMaxDecodes-- > 0 && !mRequestQueue.empty())
        {
            CacheRequest request = mRequestQueue.back();
            mRequestQueue.pop_back();
            mRequestLookup.erase(request.cacheID);
            mTextureCache.LoadIntoCache(request.cacheID, request.filename, request.submode);
        }
    }

protected:

    static const size_t kMaxQueuedRequests = 16;

    std::deque<CacheRequest> mRequestQueue;
    std::set<int> mRequestLookup;
};

/**
** Threaded implementation. 
*/
class ThreadedCardRetriever : public CardRetrieverBase
{
public:

    ThreadedCardRetriever(WCache<WCachedTexture,JTexture>& inCache)
        : CardRetrieverBase(inCache), mProcessing(true)
    {
        DebugTrace("Threaded Version");
        mWorkerThread = boost::thread(ThreadProc, this);
    }

    virtual ~ThreadedCardRetriever()
    {
        LOG("Tearing down ThreadedCardRetriever");
        mProcessing = false;
        mWorkerThread.join();
    }

    void QueueRequest(const std::string& inFilePath, int inSubmode, int inCacheID)
    {
        boost::mutex::scoped_lock lock(mMutex);
        // mRequestLookup is used to prevent duplicate requests for the same id
        if (mRequestLookup.find(inCacheID) == mRequestLookup.end() && mTextureCache.cache.find(inCacheID) == mTextureCache.cache.end())
        {
#ifdef DOLOG
            std::ostringstream stream;
            stream << "Queueing request: " << inFilePath;
            LOG(stream.str().c_str());
#endif
            mRequestLookup.insert(inCacheID);
            mRequestQueue.push(CacheRequest(inFilePath, inSubmode, inCacheID));

            // capping the number of queued decodes to 7, as this is 
            // the maximum # of cards we display concurrently in the deck editor.
            if (mRequestQueue.size() > 7)
            {
                int cacheIDToRemove;
                while (mRequestQueue.size() > 7)
                {
                    // pop the older requests out of the queue
                    cacheIDToRemove = mRequestQueue.front().cacheID;
                    mRequestQueue.pop();
                    mRequestLookup.erase(cacheIDToRemove);

                    assert(mRequestLookup.size() - mRequestQueue.size() < 2);
                }
            }
        }
    }

protected:
    ThreadedCardRetriever();

    static void ThreadProc(void* inParam)
    {
        LOG("Entering ThreadedCardRetriever::ThreadProc");
        ThreadedCardRetriever* instance = reinterpret_cast<ThreadedCardRetriever*>(inParam);
        if (instance)
        {
            while (instance->mProcessing)
            {
                while (!instance->mRequestQueue.empty())
                {
                    CacheRequest request;
                    {
                        boost::mutex::scoped_lock lock(instance->mMutex);
                        request = instance->mRequestQueue.front();
                        instance->mRequestQueue.pop();
                    }

                    instance->mTextureCache.LoadIntoCache(request.cacheID, request.filename, request.submode);

                    {
                        boost::mutex::scoped_lock lock(instance->mMutex);
                        instance->mRequestLookup.erase(request.cacheID);
                    }

                    // not sure this is necessary, adding it to potentially prevent SIGHUP on the psp
                    // rumour has it that if a worker thread doesn't allow the main thread a chance to run, it can hang the unit
#ifdef PSP
                    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
#endif
                }

                boost::this_thread::sleep(kIdleTime);
            }
        }
    }

    boost::thread mWorkerThread;

    std::queue<CacheRequest> mRequestQueue;
    std::set<int> mRequestLookup;
    boost::mutex mMutex;
    volatile bool mProcessing;

};




class CacheEngine
{
public:
    template <class T>
    static void Create(WCache<WCachedTexture,JTexture>& inCache)
    {
        LOG("Creating Card Retriever instance");
        sInstance = NEW T(inCache);
        ThreadedCardRetriever* test = dynamic_cast<ThreadedCardRetriever*>(sInstance);
        sIsThreaded = (test != NULL);
    }
    
    static CardRetrieverBase* Instance()
    {
        return sInstance;
    }
    
    static void Terminate()
    {
        SAFE_DELETE(sInstance);
    }
    
    static bool IsThreaded()
    {
        return sIsThreaded;
    }
    
    
    static CardRetrieverBase* sInstance;
    static bool sIsThreaded;
};

CardRetrieverBase* CacheEngine::sInstance = NULL;
bool CacheEngine::sIsThreaded = false;

#endif //_CACHE_ENGINE_H_
