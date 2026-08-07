/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *
 *  Shared configuration for the LLM ("GPT") opponent: the settings struct,
 *  its load/save against the runtime config file, the small HTTP helpers,
 *  the endpoint probe, and the provider preset table. Used by both
 *  AIPlayerGPT (the player) and the GPT options tab (the GUI).
 *
 *  The single source of truth is the user config file
 *  ($HOME/.Wagic/ai/gpt/endpoints.txt, falling back to the bundled
 *  Res/ai/gpt/endpoints.txt for reads). The options GUI edits that same
 *  file, so hand editing ("config as code") and the GUI stay coherent.
 *  Environment variables (WAGIC_AI, WAGIC_GPT_*) override file values.
 */

#ifndef _GPTCONFIG_H_
#define _GPTCONFIG_H_

#ifdef WITH_GPT_AI

#include <string>
#include <vector>

class GptSettings
{
public:
    int enabled;              //0/1: the GUI master switch (env WAGIC_AI overrides)
    std::vector<std::string> urls; //candidate endpoints, probed in order; [0] is GUI-edited
    std::string model;        //empty = auto-detect (first id from /v1/models)
    std::string key;          //bearer token; empty = none
    int thinking;             //-1 unset, else 0/1
    long maxTokens;           //-1/0 unset = built-in default; caps the reply
                              //length (config key max_reply_tokens, legacy
                              //alias maxtokens) - a decode-side truncation
                              //guard against 12-16k-char spirals
    double repetitionPenalty; //vLLM repetition_penalty; 1.0 = OFF (the field
                              //is only sent when != 1.0). Sampling change,
                              //corpus-validate before defaulting on
    int timeoutSecs;          //per-call HTTP timeout for model completions
    int patienceSecs;         //seconds a call may be in flight before the duel
                              //offers "keep waiting / play without the LLM".
                              //NOT the same job as timeoutSecs: that bounds a
                              //dead connection, this bounds how long a person
                              //sits watching one. 0 = never ask
    int translog;             //0/1: dump every decision (prompt+reply) to
                              //~/.Wagic/ai/gpt/logs/*.jsonl - prompt-tuning
                              //raw material, and future training data for a
                              //small shippable policy model
    int telemetry;            //-1 never asked, 0 declined, 1 consented:
                              //contribute anonymized game decisions. Asked
                              //once, when a new endpoint is set up. Consent
                              //also enables the local decision log so the
                              //data exists to contribute.
    int peek;                 //0/1: evaluation aid - the opponent-hand zone
                              //widget opens face-up on click without needing
                              //a Telepathy-style effect, so a human can see
                              //what the AI is choosing from. Config/env only
                              //(like translog); not surfaced in the GUI.

    GptSettings();

    //Read the config file (user copy first, bundled copy as fallback).
    static GptSettings load();
    //Write $HOME/.Wagic/ai/gpt/endpoints.txt (directories created as
    //needed). Returns false when the file cannot be written.
    bool save() const;

    bool operator==(const GptSettings& o) const;
    bool operator!=(const GptSettings& o) const { return !(*this == o); }

    //First URL, for display/editing; "" when none configured.
    std::string primaryUrl() const;
    void setPrimaryUrl(const std::string& u);
};

//Read a GPT runtime asset by name ("endpoints.txt", "system_prompt.txt"):
//user copy first, bundled Res copy as fallback. "" when neither exists.
std::string gptReadAsset(const char * filename);

//Append one line to <user root>/ai/gpt/gpt-log.txt.
//DebugTrace is compiled out of release builds and, where it survives, writes to
//stderr - which is invisible on a console. This is the channel that reaches a
//user's bug report, so it is for the handful of events worth explaining after
//the fact (the transport refusing to start, and the like), not chatter.
void gptLogLine(const std::string& line);

//Evaluation peek (config peek=1, or WAGIC_GPT_PEEK env override): reveal
//the opponent's hand on click. Cached after the first call - flipping it
//takes a restart, which is fine for a debugging aid.
bool gptPeekOpponentHand();

//Provider presets: stable OpenAI-compatible base URLs. Index 0 is "Custom".
struct GptPreset
{
    const char * name;
    const char * url;
};
const GptPreset * gptPresets(size_t& count);
//Preset index whose url matches (prefix-insensitively) the given url;
//0 ("Custom") when none does.
size_t gptPresetForUrl(const std::string& url);

//Blocking HTTP round trips (call from a worker thread for UI paths).
//Returns the body, or "" on transport error / non-200.
std::string gptHttpGet(const std::string& url, long timeoutMs, const std::string& bearer);
std::string gptHttpPost(const std::string& url, const std::string& body, long timeoutMs, const std::string& bearer);

//--- Platform threading seam ---------------------------------------------
//Vita's libstdc++ has no active gthreads layer: std::thread construction
//THROWS and std::mutex lock/unlock are NO-OPS. The model-call worker
//therefore needs BOTH primitives supplied natively there - a real thread
//without a real mutex is a data race on AsyncState, worse than the Baka
//fallback it replaces. PSP task #6 (WiFi transport) will ride this same
//seam. Elsewhere both types delegate to the std library.

//Run fn(ctx) on a detached background thread. False when the platform
//refuses (caller keeps ownership of ctx and must fall back synchronously).
//Honors WAGIC_GPT_NOTHREAD (desktop emulation of a threadless platform).
bool gptSpawnWorker(void (*fn)(void *), void * ctx);

#if defined (VITA)
//BasicLockable over a kernel mutex; std::lock_guard works with it as-is.
class GptMutex
{
public:
    GptMutex();
    ~GptMutex();
    void lock();
    void unlock();
private:
    int mId; //SceUID
    GptMutex(const GptMutex&);
    GptMutex& operator=(const GptMutex&);
};
#else
#include <mutex>
typedef std::mutex GptMutex;
#endif

//Probe url + "/v1/models". True when the endpoint answers with a usable
//model list; modelOut receives the first advertised model id.
bool gptProbeEndpoint(const std::string& url, const std::string& key, std::string& modelOut, long timeoutMs = 20000);

#ifdef WAGIC_HTTP_JNI
//Capture the app's SDLActivity class while the app class loader is still the
//one in effect. MUST be called from JNI_OnLoad (or any Java-originated call);
//the model-call worker is a native thread and cannot resolve app classes.
#include <jni.h>
void gptAndroidCacheClass(JNIEnv * env);
#endif

#endif //WITH_GPT_AI

#endif //_GPTCONFIG_H_
