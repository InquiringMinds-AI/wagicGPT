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
#include <memory>

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
    std::string providerOnly; //OpenRouter provider pinning (config key
                              //provider_only, comma-separated provider names):
                              //sent as provider:{only:[...],allow_fallbacks:
                              //false} so routing NEVER falls back to a
                              //provider the user did not name. Empty = off.
    std::string reasoningEffort; //Codex-backend reasoning tier (config key
                              //reasoning_effort): one of the server's own
                              //set - none/low/medium/high/xhigh/max. Empty =
                              //the built-in default (low). Only sent on the
                              //"OpenAI subscription" preset; the thinking
                              //flag covers every other provider family.
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

//Root of the writable per-user config tree (".Wagic" component included);
//"" = nowhere writable. Platform-aware: $HOME/.Wagic on desktop,
//ux0:data/Wagic on Vita, the engine's resolved root on Android. EVERY
//writable GPT path must come from here - a raw getenv("HOME") is silently
//empty on the consoles (the translog was lost on Vita to exactly that).
std::string gptUserRoot();

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

//--- ChatGPT-subscription backend (Codex Responses API) --------------------
//The "OpenAI subscription" preset: OAuth tokens from the ChatGPT device-code
//flow instead of key=, request path /responses instead of /v1/chat/completions,
//replies always SSE (the backend rejects stream:false). See the implementation
//block in GptConfig.cpp for the verified endpoint/header/shape facts.

//True when url points at the Codex backend (chatgpt.com/backend-api).
bool gptCodexEndpoint(const std::string& url);
//Model used when the config names none.
extern const char * const kGptCodexDefaultModel;
//The models a ChatGPT account may use on this backend (no /v1/models exists;
//this list was established by probing - the backend 400s anything else with
//"not supported when using Codex with a ChatGPT account").
const char * const * gptCodexModels(size_t& count);
//True when s is one of the backend's reasoning tiers (none/low/medium/high/
//xhigh/max - the server's own enumeration; there is no "minimal").
bool gptCodexEffortValid(const std::string& s);
//True when usable subscription auth material was found; whyNot names the gap.
bool gptCodexAuthPresent(std::string& whyNot);
//Blocking round trip (worker thread): POST a Responses-shaped body to the full
//url, refreshing the access token when needed. Returns the assistant text, or
//"" with errOut set. Thread-safe across concurrent AI seats.
std::string gptCodexComplete(const std::string& url, const std::string& requestBody,
                             long timeoutMs, std::string& errOut);
//Assistant text out of a Codex SSE stream (exposed for the parse self-test).
std::string gptCodexExtractText(const std::string& sse);
//Last plan-usage percent the backend reported ("" until a reply carried one).
std::string gptCodexUsedPercent();

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

//--- in-client ChatGPT sign-in (device-code flow) ---------------------------
//The subscription preset's login, run entirely from the game: request a user
//code, show it (plus a QR of the entry page) on screen, poll until the user
//approves from their phone browser, then persist the tokens where
//gptCodexComplete reads them. Endpoints match openai/codex and were proven
//live 2026-08-09; the entry page is a modern SPA no console browser renders,
//which is exactly why the flow needs no browser on THIS device.

struct GptOaiSignIn
{
    GptMutex mtx;
    int status;    //0 starting, 1 code ready (userCode/verifyUrl set),
                   //2 signed in (plan set), 3 failed (error set)
    bool cancel;   //set by the UI; the worker exits at the next poll slice
    std::string userCode;
    std::string verifyUrl;
    std::string plan;
    std::string error;
    GptOaiSignIn() : status(0), cancel(false) {}
};

//Launch the sign-in worker. False (state already marked failed, with the
//reason) when this platform has no TLS transport or refuses a thread - the
//flow cannot run synchronously, it waits on a human for up to 15 minutes.
bool gptOaiSignInStart(std::shared_ptr<GptOaiSignIn> state);

//Probe url + "/v1/models". True when the endpoint answers with a usable
//model list; modelOut receives the first advertised model id. On the Codex
//backend (no /v1/models) the probe is a minimal real completion instead,
//run against modelHint when given - so "Test connection" exercises the model
//the game will actually use, not a default that can mask a bad config.
bool gptProbeEndpoint(const std::string& url, const std::string& key, std::string& modelOut, long timeoutMs = 20000, const std::string& modelHint = "");

//Every model id the endpoint advertises (/v1/models; the Codex backend has
//no listing, so its verified static roster is returned there). Blocking -
//call from a worker thread. Order preserved, duplicates dropped. False on
//transport failure or an answer with no usable ids.
bool gptListModels(const std::string& url, const std::string& key, std::vector<std::string>& out, long timeoutMs = 20000);

#ifdef WAGIC_HTTP_JNI
//Capture the app's SDLActivity class while the app class loader is still the
//one in effect. MUST be called from JNI_OnLoad (or any Java-originated call);
//the model-call worker is a native thread and cannot resolve app classes.
#include <jni.h>
void gptAndroidCacheClass(JNIEnv * env);
#endif

#endif //WITH_GPT_AI

#endif //_GPTCONFIG_H_
