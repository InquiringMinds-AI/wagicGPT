// Wagic PS Vita native entry point
// Based on SDLmain.cpp, adapted for SDL2 + vitaGL on PS Vita
//
// Uses vitaGL for OpenGL rendering over the native GXM GPU API.
// Uses SDL2 for window/event/input abstraction.

#ifdef VITA

#include <psp2/kernel/processmgr.h>
#include <psp2/apputil.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>

#include <vitaGL.h>
#include <SDL2/SDL.h>

#include "../include/JGE.h"
#include "../include/JTypes.h"
#include "../include/JApp.h"
#include "../include/JFileSystem.h"
#include "../include/JRenderer.h"
#include "../include/JGameLauncher.h"
#include "DebugRoutines.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <dirent.h>

// --- Debug logging (writes to ux0:data/Wagic/debug.txt) ---
// This file is on the writable partition so it persists across launches.
// Check it with VitaShell to see where the app crashes.
static SceUID g_debugFd = -1;

static void debugLogInit() {
    sceIoMkdir("ux0:data", 0777);
    sceIoMkdir("ux0:data/Wagic", 0777);
    g_debugFd = sceIoOpen("ux0:data/Wagic/debug.txt",
                          SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0666);
}

static void debugLog(const char* msg) {
    if (g_debugFd >= 0) {
        sceIoWrite(g_debugFd, msg, strlen(msg));
        sceIoWrite(g_debugFd, "\n", 1);
    }
}

static void debugLogClose() {
    if (g_debugFd >= 0) {
        sceIoClose(g_debugFd);
        g_debugFd = -1;
    }
}

// Virtual resolution matches PSP (game logic expects 480x272)
// Actual Vita display is 960x544 = exactly 2x PSP
#define VITA_SCREEN_WIDTH   960
#define VITA_SCREEN_HEIGHT  544

static uint64_t lastTickCount;
static JGE*     g_engine   = NULL;
static JApp*    g_app      = NULL;
static JGameLauncher* g_launcher = NULL;

static bool g_running = true;

// Analog stick state (sceCtrl: 0-255, 128=center)
static u8 g_analogX = 128;
static u8 g_analogY = 128;
#define ANALOG_DEADZONE 30  // deadzone in 0-255 range (around center 128)

// Analog-to-dpad emulation state
// Game menus use ReadButton() which only sees d-pad button events, not raw analog.
// We convert analog stick position to d-pad press/release events.
static bool g_analogLeft  = false;
static bool g_analogRight = false;
static bool g_analogUp    = false;
static bool g_analogDown  = false;
#define ANALOG_DPAD_THRESHOLD 64  // distance from center (128) to trigger dpad button

// Previous button state for edge detection (press/release)
static unsigned int g_prevButtons = 0;

// Vita button -> JGE button mapping (native sceCtrl bitmask -> JGE button)
static const struct { unsigned int sceBtn; JButton jgeBtn; } gVitaButtonMap[] = {
    { SCE_CTRL_UP,       JGE_BTN_UP },
    { SCE_CTRL_DOWN,     JGE_BTN_DOWN },
    { SCE_CTRL_LEFT,     JGE_BTN_LEFT },
    { SCE_CTRL_RIGHT,    JGE_BTN_RIGHT },
    { SCE_CTRL_CROSS,    JGE_BTN_OK },       // Cross = confirm
    { SCE_CTRL_CIRCLE,   JGE_BTN_SEC },      // Circle = secondary action
    { SCE_CTRL_SQUARE,   JGE_BTN_PRI },      // Square = primary action
    { SCE_CTRL_TRIANGLE, JGE_BTN_CANCEL },   // Triangle = cancel/back
    { SCE_CTRL_LTRIGGER, JGE_BTN_PREV },     // L trigger
    { SCE_CTRL_RTRIGGER, JGE_BTN_NEXT },     // R trigger
    { SCE_CTRL_START,    JGE_BTN_MENU },     // Start
    { SCE_CTRL_SELECT,   JGE_BTN_CTRL },     // Select
};

// Touch gesture tracking (mimics Android SDLmain.cpp behavior)
static bool     g_touchActive    = false;
static int      g_touchStartX    = 0;      // game coords at touch start
static int      g_touchStartY    = 0;
static int      g_touchLastX     = 0;      // game coords of last motion
static int      g_touchLastY     = 0;
static uint32_t g_touchStartTime = 0;      // SDL_GetTicks at touch start

// Gesture thresholds (matching Android SDLmain.cpp: kTapEventTimeout=250, kHitzonePliancy=50)
#define TAP_TIMEOUT_MS    250   // max duration for tap gesture (ms)
#define TAP_DISTANCE_SQ   2500  // max movement squared for tap (50*50 in game coords)

void JGECreateDefaultBindings()
{
    // Actual gameplay input bypasses keyBinds (PollInput dispatches sceCtrl
    // straight to HoldKey_NoRepeat(JButton)). But Options > Controls iterates
    // JGE::keyBinds to display rows AND validates that every button in
    // btnToCheck is bound — if keyBinds is empty, save & exit is trapped in a
    // confirmation loop. Mirror gVitaButtonMap here so the UI/validator have
    // entries to find.
    for (size_t i = 0; i < sizeof(gVitaButtonMap) / sizeof(gVitaButtonMap[0]); ++i)
        JGE::BindKey(gVitaButtonMap[i].sceBtn, gVitaButtonMap[i].jgeBtn);
}

int JGEGetTime()
{
    return (int)SDL_GetTicks();
}

bool JGEToggleFullscreen()
{
    // Vita is always fullscreen
    return true;
}

// sceCtrl analog: 0-255, 128=center. JGE expects same range.
// Apply deadzone around center.
u8 JGEGetAnalogX()
{
    int val = (int)g_analogX - 128;  // center at 0, range -128..127
    if (val > -ANALOG_DEADZONE && val < ANALOG_DEADZONE)
        return 0x80;
    return g_analogX;
}

u8 JGEGetAnalogY()
{
    int val = (int)g_analogY - 128;
    if (val > -ANALOG_DEADZONE && val < ANALOG_DEADZONE)
        return 0x80;
    return g_analogY;
}

static bool InitGame(void)
{
    g_engine = JGE::GetInstance();
    g_app = g_launcher->GetGameApp();
    g_app->Create();
    g_engine->SetApp(g_app);

    JRenderer::GetInstance()->Enable2D();
    lastTickCount = JGEGetTime();

    return true;
}

static void DestroyGame(void)
{
    g_engine->SetApp(NULL);
    if (g_app)
    {
        g_app->Destroy();
        delete g_app;
        g_app = NULL;
    }

    JGE::Destroy();
    g_engine = NULL;
}

// Create writable directories on first launch
static void EnsureUserDirs()
{
    sceIoMkdir("ux0:data/Wagic", 0777);
    sceIoMkdir("ux0:data/Wagic/player", 0777);
    sceIoMkdir("ux0:data/Wagic/settings", 0777);
}

static bool VitaInit()
{
    // No clock overrides. Wagic is a 2D card game with a 30 fps target —
    // default Vita clocks are plenty. Boosting clocks just costs battery.

    debugLog("[3b] Configuring vitaGL");
    // Initialize vitaGL directly — this owns the GPU/display.
    // vitaGL manages all rendering via GXM. We do NOT use SDL for video.
    //
    // Pattern: vitaGL for rendering, SDL for input/audio only.
    // This matches vitaQuake, vitaRTCW, and other working Vita homebrew.

    // Pre-init configuration (MUST be before vglInit*)
    vglSetParamBufferSize(4 * 1024 * 1024);  // larger param buffer for many draw calls
    vglUseTripleBuffering(GL_TRUE);           // smoother frame pacing
    vglUseCachedMem(GL_TRUE);                 // faster CPU access to internal pools

    debugLog("[3b] Calling vglInitExtended");
    // 0x40000 = 256KB legacy pool for glBegin/glEnd immediate mode (UI rendering)
    // 24MB RAM threshold — leaves plenty for game logic
    // No MSAA — 2D card game doesn't need antialiasing
    vglInitExtended(0x40000, VITA_SCREEN_WIDTH, VITA_SCREEN_HEIGHT,
                    24 * 1024 * 1024, SCE_GXM_MULTISAMPLE_NONE);
    // Note: vglInitExtended aborts on failure (calls sceKernelExitProcess internally)

    // Post-init runtime configuration
    vglUseVram(GL_TRUE);             // prefer VRAM for textures (faster GPU access)
    vglWaitVblankStart(GL_TRUE);     // vsync on — prevents tearing

    debugLog("[3b] vglInitExtended OK");

    debugLog("[3c] Initializing native input (sceCtrl + sceTouch)");
    // Use native Vita APIs for input — more reliable than SDL without video subsystem.
    // sceCtrl: buttons + analog sticks
    // sceTouch: front touchscreen
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    debugLog("[3c] Native input OK");

    debugLog("[3d] Calling SDL_Init (audio only)");
    // SDL only for audio — input is native, video is vitaGL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "[3d] SDL_Init(AUDIO) FAILED: %s", SDL_GetError());
        debugLog(buf);
        // Audio failure is non-fatal — game can run without sound
    }
    else
    {
        debugLog("[3d] SDL_Init(AUDIO) OK");
    }

    // Setup OpenGL state (matches SDLmain.cpp initialization)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_SCISSOR_TEST);

    // Setup viewport — full Vita screen
    glViewport(0, 0, VITA_SCREEN_WIDTH, VITA_SCREEN_HEIGHT);
    glScissor(0, 0, VITA_SCREEN_WIDTH, VITA_SCREEN_HEIGHT);

    // Initial projection — Enable2D() will reset this to virtual 480x272,
    // and BeginScene() applies glScalef(2x) from mActualWidth/Height each frame.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0f, SCREEN_WIDTH_F, 0.0f, SCREEN_HEIGHT_F, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    JRenderer::GetInstance()->SetActualWidth((float)VITA_SCREEN_WIDTH);
    JRenderer::GetInstance()->SetActualHeight((float)VITA_SCREEN_HEIGHT);

    return true;
}

// Poll native Vita input: buttons, analog sticks, and touchscreen.
// This replaces SDL event-based input since we don't use SDL_INIT_VIDEO.
//
// Touch gesture behavior matches Android (SDLmain.cpp OnTouchEvent):
//   - Finger motion → LeftClicked() for hover/drag (card selection)
//   - Short tap (< 250ms, < 50px movement) → JGE_BTN_OK (confirm)
//   - Long drag/flick → Scroll(xVel, yVel) (scroll card lists)
static void PollInput()
{
    if (!g_engine)
        return;

    // --- Buttons + Analog via sceCtrl ---
    SceCtrlData ctrl;
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    // Analog sticks (0-255, 128=center)
    g_analogX = ctrl.lx;
    g_analogY = ctrl.ly;

    // Button edge detection: compare current vs previous state
    unsigned int pressed  = ctrl.buttons & ~g_prevButtons;  // newly pressed
    unsigned int released = g_prevButtons & ~ctrl.buttons;  // newly released
    g_prevButtons = ctrl.buttons;

    for (int i = 0; i < (int)(sizeof(gVitaButtonMap) / sizeof(gVitaButtonMap[0])); i++)
    {
        if (pressed & gVitaButtonMap[i].sceBtn)
            g_engine->HoldKey_NoRepeat(gVitaButtonMap[i].jgeBtn);
        if (released & gVitaButtonMap[i].sceBtn)
            g_engine->ReleaseKey(gVitaButtonMap[i].jgeBtn);
    }

    // --- Analog-to-DPad Emulation ---
    // Game menus call ReadButton() which only sees d-pad button events.
    // Convert analog stick movement into d-pad press/release events.
    {
        int ax = (int)g_analogX - 128;  // -128..+127, 0=center
        int ay = (int)g_analogY - 128;

        bool wantLeft  = (ax < -ANALOG_DPAD_THRESHOLD);
        bool wantRight = (ax >  ANALOG_DPAD_THRESHOLD);
        bool wantUp    = (ay < -ANALOG_DPAD_THRESHOLD);
        bool wantDown  = (ay >  ANALOG_DPAD_THRESHOLD);

        if (wantLeft && !g_analogLeft)
            g_engine->HoldKey_NoRepeat(JGE_BTN_LEFT);
        else if (!wantLeft && g_analogLeft)
            g_engine->ReleaseKey(JGE_BTN_LEFT);
        g_analogLeft = wantLeft;

        if (wantRight && !g_analogRight)
            g_engine->HoldKey_NoRepeat(JGE_BTN_RIGHT);
        else if (!wantRight && g_analogRight)
            g_engine->ReleaseKey(JGE_BTN_RIGHT);
        g_analogRight = wantRight;

        if (wantUp && !g_analogUp)
            g_engine->HoldKey_NoRepeat(JGE_BTN_UP);
        else if (!wantUp && g_analogUp)
            g_engine->ReleaseKey(JGE_BTN_UP);
        g_analogUp = wantUp;

        if (wantDown && !g_analogDown)
            g_engine->HoldKey_NoRepeat(JGE_BTN_DOWN);
        else if (!wantDown && g_analogDown)
            g_engine->ReleaseKey(JGE_BTN_DOWN);
        g_analogDown = wantDown;
    }

    // --- Touchscreen via sceTouch (Android-style gestures) ---
    // Mimics SDLmain.cpp OnTouchEvent:
    //   Down:   record start position + timestamp
    //   Motion: call LeftClicked() for drag/hover (game uses GetLeftClickCoordinates)
    //   Up:     short tap → JGE_BTN_OK, long drag → Scroll() for flick
    SceTouchData touch;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);

    if (touch.reportNum > 0)
    {
        // Convert Vita touch coords to game coords
        // Front panel: x 0-1919 (960px display), y 0-1087 (544px display)
        // Game expects virtual PSP resolution (480x272)
        int touchX = (int)(touch.report[0].x * SCREEN_WIDTH / 1920);
        int touchY = (int)(touch.report[0].y * SCREEN_HEIGHT / 1088);

        if (!g_touchActive)
        {
            // Finger down — record start position and time
            g_touchStartX    = touchX;
            g_touchStartY    = touchY;
            g_touchLastX     = touchX;
            g_touchLastY     = touchY;
            g_touchStartTime = SDL_GetTicks();
            g_touchActive    = true;
        }
        else
        {
            // Finger motion — update hover/drag position
            g_touchLastX = touchX;
            g_touchLastY = touchY;
            g_engine->LeftClicked(touchX, touchY);
        }
    }
    else if (g_touchActive)
    {
        // Finger up — determine gesture type
        uint32_t duration = SDL_GetTicks() - g_touchStartTime;
        int dx = g_touchLastX - g_touchStartX;
        int dy = g_touchLastY - g_touchStartY;
        int distSq = dx * dx + dy * dy;

        if (duration <= TAP_TIMEOUT_MS && distSq < TAP_DISTANCE_SQ)
        {
            // Short tap within small area → confirm action (matches Android kTapEventTimeout + kHitzonePliancy)
            g_engine->LeftClicked(g_touchStartX, g_touchStartY);
            g_engine->HoldKey_NoRepeat(JGE_BTN_OK);
        }
        else if (distSq >= TAP_DISTANCE_SQ && duration > 0)
        {
            // Drag/flick — calculate velocity in game-coord pixels/sec and send scroll
            // Game's OnScroll handlers use thresholds around 300+ for meaningful flicks
            int xVel = (int)((float)dx * 1000.0f / (float)duration);
            int yVel = (int)((float)dy * 1000.0f / (float)duration);
            g_engine->Scroll(xVel, yVel);
        }

        g_touchActive = false;
    }
}

// Target 30fps = 33.33ms per frame
#define TARGET_FRAME_MS 33

static void MainLoop()
{
    while (g_running)
    {
        if (!g_engine)
            break;

        int frameStart = JGEGetTime();

        // Poll native input (buttons, analog, touch)
        PollInput();

        if (!g_engine->IsPaused())
        {
            // Calculate delta time (cap to 100ms to prevent physics/animation jumps)
            int tickCount = JGEGetTime();
            int64_t dt = tickCount - lastTickCount;
            lastTickCount = tickCount;
            if (dt > 100) dt = 100;
            if (dt < 0) dt = 0;

            if (g_engine->IsDone())
            {
                g_running = false;
                break;
            }

            // Update game
            g_engine->SetDelta((float)dt / 1000.0f);
            g_engine->Update((float)dt / 1000.0f);

            // Render
            if (g_engine)
                g_engine->Render();

            vglSwapBuffers(GL_TRUE);
        }

        // Cap to 30fps — sleep if frame finished early
        int frameTime = JGEGetTime() - frameStart;
        if (frameTime < TARGET_FRAME_MS)
        {
            sceKernelDelayThread((TARGET_FRAME_MS - frameTime) * 1000);  // microseconds
        }
    }
}

int main(int argc, char* argv[])
{
    EnsureUserDirs();
    debugLogInit();
    debugLog("=== Wagic Vita debug log ===");

    debugLog("[1] EnsureUserDirs done");

    // Check if app0:Res/ is accessible
    {
        DIR* d = opendir("app0:Res");
        if (d) {
            debugLog("[1a] opendir(app0:Res) OK");
            struct dirent* ent;
            while ((ent = readdir(d)) != NULL) {
                char buf[256];
                snprintf(buf, sizeof(buf), "  entry: %s", ent->d_name);
                debugLog(buf);
            }
            closedir(d);
        } else {
            debugLog("[1a] opendir(app0:Res) FAILED!");
        }

        // Also test direct file open
        SceUID fd = sceIoOpen("app0:Res/core.zip", SCE_O_RDONLY, 0);
        if (fd >= 0) {
            SceOff size = sceIoLseek(fd, 0, SCE_SEEK_END);
            char buf[128];
            snprintf(buf, sizeof(buf), "[1b] core.zip direct open OK, size=%lld", (long long)size);
            debugLog(buf);
            sceIoClose(fd);
        } else {
            debugLog("[1b] core.zip direct open FAILED!");
        }
    }

    debugLog("[2] Creating JGameLauncher");
    g_launcher = new JGameLauncher();

    u32 flags = g_launcher->GetInitFlags();
    if ((flags & JINIT_FLAG_ENABLE3D) != 0)
    {
        JRenderer::Set3DFlag(true);
    }

    debugLog("[3] Calling VitaInit");
    if (!VitaInit())
    {
        debugLog("[3] VitaInit FAILED!");
        debugLogClose();
        sceKernelExitProcess(0);
        return -1;
    }
    debugLog("[3] VitaInit OK");

    JGECreateDefaultBindings();

    debugLog("[4] Calling InitGame (JFileSystem init happens here)");
    if (!InitGame())
    {
        debugLog("[4] InitGame FAILED!");
        debugLogClose();
        sceKernelExitProcess(0);
        return -1;
    }
    debugLog("[4] InitGame OK");

    // InitGame loads options.txt; if a prior build (with empty keyBinds)
    // wrote a `controls=` line, GameOptionKeyBindings::read clears keyBinds
    // and re-loads the (broken) saved values. Force defaults here so the
    // controls UI is always populated. Rebinding doesn't actually work on
    // Vita anyway — sceCtrl→JButton is hard-wired in PollInput.
    JGE::GetInstance()->ResetBindings();
    debugLog("[4a] Forced default keyBinds for Vita");

    debugLog("[5] Entering MainLoop");
    debugLogClose();

    MainLoop();

    // Exit immediately — matches PSP behavior (sceKernelExitGame).
    // vitaGL has no vglEnd() in this SDK version, so attempting to delete
    // GL resources (DestroyGame → JRenderer::Destroy) while GXM is still
    // active crashes.  The OS reclaims all memory, GPU resources, and file
    // handles on process exit, so explicit cleanup is unnecessary.
    sceKernelExitProcess(0);
    return 0;
}

#endif // VITA
