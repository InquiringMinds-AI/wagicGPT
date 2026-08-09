/* JFileSystem centralizes all access to resources in JGE.
It allows to have files for the game split in two subfolders, a "system" subfolder (read only) and a "user" subfolder (read/write)
Additionally, these two subfolders can have some of their resources in zip file (see zfsystem.h).
Zip files can contain duplicates of the same file, the one that will eventually be used is the one is the latest zip file (by alphabetical order)

Read access priority:
User folder, real file
User folder, zip file
System folder, real file
System folder, zip file

Write access:
User folder, real file

User folder is the only one that is really needed to guarantee both read and write access, the system folder is not necessary but provides a nice way to distinguish
The content that users should not be touching.
*/

#if defined (ANDROID)
#include "PrecompiledHeader.h"
#endif //ANDROID

#if defined (LINUX)
#include "../../projects/mtg/include/PrecompiledHeader.h"
#endif //LINUX


#ifdef WIN32
#pragma warning(disable : 4786)
#include <direct.h>
#define MAKEDIR(name) _mkdir(name)
#elif defined(VITA)
#include <psp2/io/stat.h>
#define MAKEDIR(name) sceIoMkdir(name, 0777)
#else
#include <sys/stat.h>
#define MAKEDIR(name) mkdir(name, 0777)
#endif

#include "../include/JGE.h"
#include "../include/JFileSystem.h"
#include "../include/JLogger.h"
#include <set>
#include <cstdio>
#include <dirent.h>

#ifdef QT_CONFIG
#include <QDir>
#endif

/*
#ifdef IOS
#include <Foundation/Foundation.h>
#endif
*/

JFileSystem* JFileSystem::mInstance = NULL;

#if defined(PSP) && defined(WAGIC_MEMPROBE)
//Load-probe accumulators (read by JGfx.cpp's LoadJPG probe). Zip attach and
//central-directory parse costs land BEFORE LoadJPG entry (cardFile resolves
//the path first), so per-load attribution is done by delta-since-last-load.
#include <pspkernel.h>
unsigned int gLoadProbeAttachUs = 0;  //time spent switching zips in AttachZipFile
unsigned int gLoadProbeAttachN = 0;   //zip switches (same-zip early-outs not counted)
unsigned int gLoadProbeParseUs = 0;   //time spent parsing central dirs in preloadZip
unsigned int gLoadProbeParseN = 0;
unsigned int gLoadProbeWipeN = 0;     //clearZipCache() full-cache wipes
#endif

JZipCache::JZipCache()
    : lastUse(0)
{}

//LRU tick for the zip-directory cache; bumped on every cache touch.
static unsigned int gZipCacheTick = 0;

JZipCache::~JZipCache()
{
    dir.clear();
}

void JFileSystem::Pause() 
{
    zip_file_system::filesystem::closeTempFiles();
}

//Zip-path failure breadcrumbs (see the card-art no-show hunt, 2026-08-09):
//a lookup that fails inside the zip machinery is invisible to the caller
//beyond "404", and on the handhelds that has meant weeks of a byte-perfect
//pack rendering no-art frames with nothing to read. One deduped line per
//event, capped, straight into the user root so it survives without any
//game-layer plumbing.
static void zipDiagLog(JFileSystem * fs, const string& line)
{
    static std::set<string> logged;
    if (logged.size() >= 100 || !logged.insert(line).second)
        return;
    string path = fs->GetUserRoot();
    if (path.size() && path[path.size() - 1] != '/' && path[path.size() - 1] != '\\')
        path += '/';
    path += "wagic-ziplog.txt";
    FILE * f = fopen(path.c_str(), "a");
    if (!f)
        return;
    fprintf(f, "%s\n", line.c_str());
    fclose(f);
}

void JFileSystem::preloadZip(const string& filename)
{
    map<string,JZipCache *>::iterator it = mZipCache.find(filename);
    if (it != mZipCache.end()) return;

#if defined(PSP) && defined(WAGIC_MEMPROBE)
    unsigned int probeT0 = sceKernelGetSystemTimeLow();
#endif

    // Zip directory cache limit.  PSP original: 4500 entries (~200KB) with a
    // WIPE-EVERYTHING eviction — measured on hardware 2026-08-06: 31 full
    // wipes in one short session, each forcing central-directory re-parses
    // that cost up to 5 SECONDS per set on the memory stick. Now: bigger cap
    // + per-zip LRU eviction, so browsing evicts the stalest set instead of
    // nuking the cache.
#if defined(VITA)
    const unsigned int zipCacheLimit = 80000;   // ~3.6MB, covers all 75k+ card entries
#elif defined(PSP)
    const unsigned int zipCacheLimit = 12000;   // ~0.5-1MB of the diet's heap margin
#else
    const unsigned int zipCacheLimit = 12000;
#endif
    while (mZipCachedElementsCount > zipCacheLimit && mZipCache.size() > 1)
    {
        map<string,JZipCache *>::iterator victim = mZipCache.end();
        for (map<string,JZipCache *>::iterator lru = mZipCache.begin(); lru != mZipCache.end(); ++lru)
        {
            if (lru->first == filename) continue;
            if (victim == mZipCache.end() || lru->second->lastUse < victim->second->lastUse)
                victim = lru;
        }
        if (victim == mZipCache.end()) break;
        mZipCachedElementsCount -= victim->second->dir.size();
        delete victim->second;
        mZipCache.erase(victim);
#if defined(PSP) && defined(WAGIC_MEMPROBE)
        gLoadProbeWipeN++;   //now counts single-zip LRU evictions, not full wipes
#endif
    }

    JZipCache * cache = new JZipCache();
    cache->lastUse = ++gZipCacheTick;
    mZipCache[filename] = cache;

    //A failed attach or parse used to leave the just-inserted EMPTY cache
    //in the map forever: every later lookup found it, missed, and the whole
    //set silently rendered no-art frames until restart (the Vita's missing
    //card art, hunted 2026-08-09 - the parse fails transiently on-device).
    //Now the empty entry is dropped so the next request retries, with a
    //budget: a PERSISTENTLY failing zip must not re-parse per request
    //(a parse costs seconds on PSP memory stick), so after 5 strikes the
    //empty entry stays as a deliberate session tombstone.
    static map<string, int> zipFailCounts;

    if (!mZipAvailable || !mZipFile) {
		AttachZipFile(filename);
		if (!mZipAvailable || !mZipFile)
		{
		    int fails = ++zipFailCounts[filename];
		    char tail[48];
		    snprintf(tail, sizeof(tail), " strike=%d", fails);
		    zipDiagLog(this, "zip-attach-fail " + filename + tail);
		    if (fails < 5)
		    {
		        delete mZipCache[filename];
		        mZipCache.erase(filename);
		    }
		    return;
		}
	}

    //Parse through the ALREADY-OPEN attach handle (mZipFile) instead of
    //opening a second handle to the same file: the second open is what
    //fails on the Vita under fd pressure (ziplog reason=open, 2026-08-09),
    //and it was pure waste besides. Path-based parse stays as the fallback.
    if (mUserFS->PreloadZip(mZipFile, cache->dir)
        || mUserFS->PreloadZip(filename.c_str(), cache->dir)
        || (mSystemFS && mSystemFS->PreloadZip(filename.c_str(), cache->dir)))
    {
        mZipCachedElementsCount+= cache->dir.size();
        zipFailCounts.erase(filename); //a success clears the strike record
    }
    else
    {
        int fails = ++zipFailCounts[filename];
        char tail[96];
        snprintf(tail, sizeof(tail), " reason=%s strike=%d",
                 zip_file_system::filesystem::PreloadZipFailReason(), fails);
        zipDiagLog(this, "zip-parse-fail " + filename + tail);
        DetachZipFile();
        if (fails < 5)
        {
            delete mZipCache[filename];
            mZipCache.erase(filename);
        }
    }

#if defined(PSP) && defined(WAGIC_MEMPROBE)
    gLoadProbeParseUs += sceKernelGetSystemTimeLow() - probeT0;
    gLoadProbeParseN++;
#endif
}


void JFileSystem::init(const string & userPath, const string & systemPath)
{
    Destroy();
    mInstance = new JFileSystem(userPath, systemPath);
}

JFileSystem* JFileSystem::GetInstance()
{
    if (!mInstance)
    {
#ifdef RESPATH
    init( RESPATH"/");
#else
    init("Res/");
#endif
    }
    return mInstance;
}

// Tries to set the system and user paths.
// On some OSes, the parameters get overriden by hardcoded values
JFileSystem::JFileSystem(const string & _userPath, const string & _systemPath)

{
    string systemPath = _systemPath;
    string userPath = _userPath;

#ifdef VITA
    // Vita filesystem:
    // app0: = read-only application data (bundled in VPK, includes card art)
    // ux0:data/Wagic/ = writable user data (saves, settings, decks)
    systemPath = "app0:Res/";
    userPath = "ux0:data/Wagic/";

#elif defined (IOS)
    NSArray *paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];

    userPath = [[documentsDirectory  stringByAppendingString: @"/User/"] cStringUsingEncoding:1];
    systemPath = [[documentsDirectory  stringByAppendingString: @"/Res/"] cStringUsingEncoding:1];

#elif defined (ANDROID)
    userPath = JGE::GetInstance()->getFileUserFolderPath();
    systemPath = JGE::GetInstance()->getFileSystemLocation();

    DebugTrace("User path " << userPath);
    DebugTrace("System path " << systemPath);
#elif defined (QT_CONFIG)

    QDir sysDir(RESDIR);
    QDir dir(QDir::homePath());
    dir.mkdir(USERDIR);
    dir.cd(USERDIR);

    userPath = QDir::toNativeSeparators(dir.absolutePath()).toStdString();
    systemPath = QDir::toNativeSeparators(sysDir.absolutePath()).toStdString();

    DebugTrace("User path " << userPath);
    DebugTrace("System path " << systemPath);
    DebugTrace("Current path " << QDir::currentPath().toStdString());
#else
    //Find the Res.txt file and matching Res folders for backwards compatibility
    ifstream mfile("Res.txt");
    string resPath;
    if (mfile)
    {
        bool found = false;
        while (!found && std::getline(mfile, resPath))
        {
            if (!resPath.size())
                continue;

            if (resPath[resPath.size() - 1] == '\r')
                resPath.erase(resPath.size() - 1); //Handle DOS files
            string testfile = resPath;
            testfile.append("graphics/simon.dat");
            ifstream file(testfile.c_str());
            if (file)
            {
                userPath = resPath;
                systemPath = "";
                found = true;
                file.close();
            }
        }
        mfile.close();
    }
#endif

	// Make sure the base paths finish with a '/' or a '\'
	if (! userPath.empty()) {
                string::iterator c = userPath.end();//userPath.at(userPath.size()-1);
                c--;
		if ((*c != '/') && (*c != '\\'))
			userPath += '/';
	}

	if (! systemPath.empty()) {
                string::iterator c = systemPath.end();//systemPath.at(systemPath.size()-1);
                c--;
                if ((*c != '/') && (*c != '\\'))
			systemPath += '/';
	}

	mUserFSPath = userPath;
    MAKEDIR(userPath.c_str());

    mSystemFSPath = systemPath;
   
    mUserFS = new zip_file_system::filesystem(userPath.c_str());
    mSystemFS = (mSystemFSPath.size() && (mSystemFSPath.compare(mUserFSPath) != 0)) ? new zip_file_system::filesystem(systemPath.c_str()) : NULL;

    mZipAvailable = false;
    mZipCachedElementsCount = 0;
    mPassword = NULL;
    mFileSize = 0;
    mCurrentFileInZip = NULL;

};

void JFileSystem::Destroy()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

bool JFileSystem::DirExists(const string& strDirname)
{ 
    return (mSystemFS && mSystemFS->DirExists(strDirname)) || mUserFS->DirExists(strDirname);
}

bool JFileSystem::FileExists(const string& strFilename)
{ 
    if (strFilename.length() < 1 ) return false;
    
    return (mSystemFS && mSystemFS->FileExists(strFilename)) || mUserFS->FileExists(strFilename);
}

bool JFileSystem::MakeDir(const string & dir)
{
    string fullDir = mUserFSPath + dir;
    MAKEDIR(fullDir.c_str());
    return true;
}

JFileSystem::~JFileSystem()
{
    clearZipCache();
    zip_file_system::filesystem::closeTempFiles();
    SAFE_DELETE(mUserFS);
    SAFE_DELETE(mSystemFS);
}


void JFileSystem::clearZipCache()
{
    DetachZipFile();

    map<string,JZipCache *>::iterator it;
    for (it = mZipCache.begin(); it != mZipCache.end(); ++it){
        delete(it->second);
    }
    mZipCache.clear();
    mZipCachedElementsCount = 0;
}

bool JFileSystem::AttachZipFile(const string &zipfile, char *password /* = NULL */)
{
#ifndef WIN32
    if (mZipAvailable && mZipFile.is_open())
    {
        if (mZipFileName != zipfile)
            DetachZipFile();		// close the previous zip file
        else
            return true;
    }
#endif
#if defined(PSP) && defined(WAGIC_MEMPROBE)
    unsigned int probeT0 = sceKernelGetSystemTimeLow();
#endif
    mZipFileName = zipfile;
    mPassword = password;

    openForRead(mZipFile, mZipFileName);

    if (!mZipFile)
        return false;


    //A hack for a zip inside a zip: instead we open the zip containing it
    if (mZipFile.Zipped())
    {
        mZipFile.close();
        if (!zip_file_system::filesystem::getCurrentFS())
            return false;
        mZipFile.open(zip_file_system::filesystem::getCurrentZipName().c_str(), zip_file_system::filesystem::getCurrentFS());
        if (!mZipFile)
            return false;
    }
    mZipAvailable = true;
#if defined(PSP) && defined(WAGIC_MEMPROBE)
    gLoadProbeAttachUs += sceKernelGetSystemTimeLow() - probeT0;
    gLoadProbeAttachN++;
#endif
    return true;

}


void JFileSystem::DetachZipFile()
{
    if (mZipFile)
    {
        mZipFile.close();
    }
    mCurrentFileInZip = NULL;
    mZipAvailable = false;
}

bool JFileSystem::openForRead(izfstream & File, const string & FilePath) {

    File.open(FilePath.c_str(), mUserFS);
    if (File)
        return true;
    
    if(!mSystemFS)
        return false;

    File.open(FilePath.c_str(), mSystemFS);
    if (File)
        return true;

    return false;
}

bool JFileSystem::readIntoString(const string & FilePath, string & target)
{
    izfstream file;
    if (!openForRead(file, FilePath))
        return false;

    int fileSize = GetFileSize(file);

    target.resize((std::string::size_type) fileSize);


    if (fileSize)
        file.read(&target[0], fileSize);

    file.close();
    return true;
}

bool JFileSystem::openForWrite(ofstream & File, const string & FilePath, ios_base::openmode mode)
{
    string filename = mUserFSPath;
    filename.append(FilePath);

    #if defined(ANDROID)
    DebugTrace("ANDROID");
    std::vector<string> dirs;
    string path = filename.substr( 0, filename.find_last_of( '/' ) + 1 );

    // put it into list
    dirs.push_back(path);

    //make list of directories that need to be created
    do
    {
        path = path.substr( 0, path.find_last_of( '/', path.size() - 2 ) + 1 );
        dirs.push_back(path);
    
    } while (path.compare(mUserFSPath) != 0);
    
    // remove mUserFSPath from list
    dirs.pop_back();

    // create missing directories
    for (std::vector<string>::reverse_iterator it = dirs.rbegin(); it != dirs.rend(); ++it)
    {
        if(!DirExists(*it))
        {
            MAKEDIR((*it).c_str());
        }
    }
    #endif

    File.open(filename.c_str(), mode);

    if (File)
    {
        return true;
    }
    return false;
}

bool JFileSystem::OpenFile(const string &filename)
{
    mCurrentFileInZip = NULL;

    if (!mZipAvailable || !mZipFile)
        return openForRead(mFile, filename);

    preloadZip(mZipFileName);
    map<string,JZipCache *>::iterator it = mZipCache.find(mZipFileName);
    if (it == mZipCache.end())
    {
        //DetachZipFile();
        //return OpenFile(filename); 
        return openForRead(mFile, filename);
    }
    JZipCache * zc = it->second;
    zc->lastUse = ++gZipCacheTick;
    map<string,  zip_file_system::filesystem::limited_file_info>::iterator it2 = zc->dir.find(filename);
    if (it2 == zc->dir.end())
    {
        //dir=0 means the attach/parse failed earlier and left an empty
        //cache (see preloadZip); dir>0 means the parse ran but dropped
        //this entry. The two point at entirely different defects. Scoped
        //to card-art keys inside set zips: every open probes the attached
        //zip first, so routine lookups (zone icons, mana symbols, sounds)
        //miss here by design and would drown the breadcrumb cap. Card art
        //keys are "<numeric id>.jpg" and "thumbnails/<id>.jpg".
        if (mZipFileName.compare(0, 5, "sets/") == 0
            && filename.size()
            && (isdigit((unsigned char) filename[0])
                || filename.compare(0, 11, "thumbnails/") == 0))
        {
            char n[64];
            snprintf(n, sizeof(n), " dir=%u", (unsigned) zc->dir.size());
            zipDiagLog(this, "zip-entry-miss " + mZipFileName + " " + filename + n);
        }
        /*DetachZipFile();
        return OpenFile(filename); */
        return openForRead(mFile, filename);
    }

    mCurrentFileInZip = &(it2->second);
    mFileSize = it2->second.m_Size;
    return true;

}


void JFileSystem::CloseFile()
{
    if (mZipAvailable && mZipFile)
    {
        mCurrentFileInZip = NULL;
    }

    if (mFile)
        mFile.close();
}

//returns 0 if less than "size" bits were read
int JFileSystem::ReadFile(void *buffer, int size)
{
    if (mCurrentFileInZip)
    {
        assert(mZipFile);
        if((size_t)size > mCurrentFileInZip->m_Size) //only support "store" method for zip inside zips
            return 0;
        //Each call seeks to the member start, so reads here are independent of
        //each other - but the stream's error state is NOT. One short read sets
        //eofbit, and every later seekg/read on the same stream then fails,
        //turning a single bad read into "no resource ever loads again". Clear
        //before seeking so a failure stays local to the file that caused it.
        mZipFile.clear();
        std::streamoff offset = zip_file_system::filesystem::SkipLFHdr(mZipFile, mCurrentFileInZip->m_Offset);
        if (!mZipFile.seekg(offset))
        {
            mZipFile.clear();
            return 0;
        }
        mZipFile.read((char *) buffer, size);
        //TODO what if can't read
        return size;
    }

    if (!mFile)
        return 0;

    assert(!mFile.Zipped() || (size_t)size <= mFile.getUncompSize());
	mFile.read((char *)buffer, size);
    if (mFile.eof())
        return 0;
    return size;
}

std::vector<std::string>& JFileSystem::scanRealFolder(const std::string& folderName, std::vector<std::string>& results)
{
    DIR *dip = opendir(folderName.c_str());
    if (!dip)
        return results;

    while (struct dirent * dit = readdir(dip))
    {
        results.push_back(dit->d_name);
    }

    closedir(dip);

    return results;
}

std::vector<std::string>& JFileSystem::scanfolder(const std::string& _folderName, std::vector<std::string>& results)
{
    if (!_folderName.size())
        return results;

    map<string, bool> seen;


    string folderName = _folderName;
    if (folderName[folderName.length() - 1] != '/')
        folderName.append("/");

    //we scan zips first, then normal folders.
    // This is to avoid duplicate folders coming from the real folder 
    // (a folder "foo" in the zip comes out as "foo/", while on the real FS it comes out as "foo")

    //user zips
    {    
        //Scan the zip filesystem
        std::vector<std::string> userZips;
        mUserFS->scanfolder(folderName, userZips);

        for (size_t i = 0; i < userZips.size(); ++i)
            seen[userZips[i]] = true;
    }

    //system zips
    if (mSystemFS)
    {
        //Scan the zip filesystem
        std::vector<std::string> systemZips;
        mSystemFS->scanfolder(folderName, systemZips);

        for (size_t i = 0; i < systemZips.size(); ++i)
            seen[systemZips[i]] = true;

    }

    //user real files
    {
        //scan the real files
        std::vector<std::string> userReal;
        string realFolderName = mUserFSPath;
        realFolderName.append(folderName);
        scanRealFolder(realFolderName, userReal);

        for (size_t i = 0; i < userReal.size(); ++i)
        {
            string asFolder = userReal[i] + "/";
            if (seen.find(asFolder) == seen.end())
                seen[userReal[i]] = true;
        }
    }

    //system real files
    if (mSystemFS)
    {
        //scan the real files 
        std::vector<std::string> systemReal;
        string realFolderName = mSystemFSPath;
        realFolderName.append(folderName);
        scanRealFolder(realFolderName, systemReal);

    
        for (size_t i = 0; i < systemReal.size(); ++i)
        {
            string asFolder = systemReal[i] + "/";
            if (seen.find(asFolder) == seen.end())
                seen[systemReal[i]] = true;
        }
    }

    for(map<string,bool>::iterator it = seen.begin(); it != seen.end(); ++it) 
    {
      results.push_back(it->first);
    }

    return results;
}

std::vector<std::string> JFileSystem::scanfolder(const std::string& folderName)
{
    std::vector<std::string> result;
    return scanfolder(folderName, result);
}

int JFileSystem::GetFileSize()
{
    if (mCurrentFileInZip)
        return mFileSize;

    return GetFileSize(mFile);
}

bool JFileSystem::Rename(string _from, string _to)
{
    string from = mUserFSPath + _from;
    string to = mUserFSPath + _to;
    std::remove(to.c_str());
    return (rename(from.c_str(), to.c_str()) == 0);
}

bool JFileSystem::Remove(string aFile)
{
    string toRemove = mUserFSPath + aFile;
    return (std::remove(toRemove.c_str()) == 0);
}

int JFileSystem::GetFileSize(izfstream & file)
{
    if (!file)
        return 0;

    if (file.Zipped())
    {
        //There's a bug in zipped version that prevents from sending a correct filesize with the "standard" seek method
        //The hack below only works for the "stored" version I think...
        return file.getUncompSize();
    }

    file.seekg (0, ios::end);
    int length = (int) file.tellg();
    file.seekg (0, ios::beg);
    return length;

}

// AVOID Using This function!!!
/*
This function is deprecated, but some code is still using it
It used to give a pathname to a file in the file system.
Now with the support of zip resources, a pathname does not make sense anymore
However some of our code still relies on "physical" files not being in zip.
So this call is now super heavy: it checks where the file is, and if it's in a zip, it extracts
it to the user Filesystem, assuming that whoever called this needs to access the file through its pathname later on.

As a result, this function isvery inefficient and shouldn't be used in the general case.
*/
string JFileSystem::GetResourceFile(string filename) 
{
    izfstream temp;
    bool result = openForRead(temp, filename);

    if (!temp || !result)
        return "";

    if (!temp.Zipped())
    {
        string result = temp.FullFilePath();
        temp.close();
        return result;
    }

    // File is inside a zip archive,
    //we copy it to the user FS
    string destFile = mUserFSPath + filename;
    ofstream dest;
    if (openForWrite(dest, filename, ios_base::binary))
    {
        // allocate memory:
        size_t length = temp.getUncompSize();
        char * buffer = new char [length];

        // read data as a block:
        temp.read(buffer, length);
        temp.close();

        dest.write (buffer,length);
        delete[] buffer;
        dest.close();
        return destFile;
    }
    return "";
}
