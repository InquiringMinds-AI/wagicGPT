#include <SDL.h>
#if (defined ANDROID)
#include <SDL_opengles.h>
#else
#include <SDL_opengl.h>
#ifdef WIN32
#undef GL_VERSION_2_0
#endif
#endif

#include "../include/JGE.h"
#if !defined(WIN32) && !defined(PSP) && !defined(VITA) && !defined(IOS)
#include <sys/resource.h>
#endif
#include "../include/JTypes.h"
#include "../include/JApp.h"
#include "../include/JFileSystem.h"
#include "../include/JRenderer.h"
#include "../include/JGameLauncher.h"
#include "DebugRoutines.h"
#include <stdexcept>
#include <iostream>
#include <math.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#if (defined FORCE_GLES)
#undef GL_ES_VERSION_2_0
#undef GL_VERSION_2_0
#define GL_VERSION_ES_CM_1_1 1
#define glOrthof glOrtho
#define glClearDepthf glClearDepth
#endif

#define ACTUAL_SCREEN_WIDTH (SCREEN_WIDTH)
#define ACTUAL_SCREEN_HEIGHT (SCREEN_HEIGHT)
#define ACTUAL_RATIO ((GLfloat)ACTUAL_SCREEN_WIDTH / (GLfloat)ACTUAL_SCREEN_HEIGHT)

enum eDisplayMode
{
	DisplayMode_lowRes = 0,
	DisplayMode_hiRes,
	DisplayMode_fullscreen
};

unsigned int gDisplayMode = DisplayMode_lowRes;

const int kHitzonePliancy = 50;

// tick value equates to ms
const int kTapEventTimeout = 250;


uint64_t	lastTickCount;
JGE* g_engine = NULL;
JApp* g_app = NULL;
JGameLauncher* g_launcher = NULL;
#ifdef ANDROID
JNIEnv * mJNIEnv = NULL;
jclass * mJNIClass = NULL;
int SDL_ResumeSyncTime = 0;
#endif

class SdlApp;

SdlApp *g_SdlApp = NULL;


#ifdef ANDROID
// Pause
extern "C" void Java_org_libsdl_app_SDLActivity_nativePause(
                                    JNIEnv* env, jclass cls)
{    
	DebugTrace("Attempt pause");
	if (!g_engine)
		return;
   g_engine->Pause();
   DebugTrace("Pause done");
}

// Resume
extern "C" void Java_org_libsdl_app_SDLActivity_nativeResume(
                                    JNIEnv* env, jclass cls)
{    
	if (!g_engine)
		return;
    SDL_ResumeSyncTime = 100000;
    g_engine->Resume();
}

#include "Wagic_Version.h"
extern "C" jstring Java_org_libsdl_app_SDLActivity_getResourceName(
                                       JNIEnv* env, jclass cls)
{
    return env->NewStringUTF (WAGIC_RESOURCE_NAME);
}

extern "C" jstring Java_org_libsdl_app_SDLActivity_getResourceUrl(
                                       JNIEnv* env, jclass cls)
{
    return env->NewStringUTF (WAGIC_RESOURCE_URL);
}
#endif


//WAGIC_FASTCLOCK: decouple game time from wall time for bulk (headless)
//runs. The game paces itself in real seconds -- the AI acts at most every
//~0.07s of dt (AIPlayerBaka::Act) -- so a demo game costs ~20 wall minutes
//no matter the CPU. In fast-clock mode every tick feeds a FIXED dt (default
//0.1s, >= the AI throttle, so the AI may act every tick) and the event wait
//shrinks 10ms -> 1ms. Real waits (model inference) still dominate where they
//exist; the artificial padding is what disappears. Value = game-seconds per
//tick; anything unparseable or out of (0,10) falls back to the default.
static float fastClockDt()
{
    static float cached = -2.0f;
    if (cached == -2.0f)
    {
        const char * v = getenv("WAGIC_FASTCLOCK");
        if (!v || !*v)
            cached = -1.0f; //off
        else
        {
            float parsed = (float)atof(v);
            cached = (parsed > 0.0f && parsed < 10.0f) ? parsed : 0.1f;
        }
    }
    return cached;
}

class SdlApp
{
public: /* For easy interfacing with JGE static functions */
    bool            Running;
    SDL_Window*     window;
    SDL_GLContext   glContext;
    SDL_Rect        viewPort;
    Uint32          lastMouseUpTime;
    Uint32          lastFingerDownTime;
    int             windowed_w;
    int             windowed_h;
    int             windowed_pos_x;
    int             windowed_pos_y;

    int mMouseDownX;
    int mMouseDownY;

public:
    SdlApp() : glContext(NULL), window(NULL), lastMouseUpTime(0), lastFingerDownTime(0), Running(true), mMouseDownX(0), mMouseDownY(0)
    {
    }

    int OnExecute()
    {
        if (OnInit() == false)
        {
            return -1;
        }

        SDL_Event Event;

        while(Running)
        {
            if (g_engine)
            {
                for (int x = 0; x < 5 && SDL_WaitEventTimeout(&Event, fastClockDt() > 0 ? 1 : 10); ++x)
                {
                    if(!g_engine->IsPaused())
                        OnEvent(&Event);
                }
                if(!g_engine->IsPaused()){
                    #ifdef ANDROID
                    // Fix for SDL crash on resuming Android app from background.
                    if(SDL_ResumeSyncTime){
                        usleep(SDL_ResumeSyncTime);
                        SDL_ResumeSyncTime = 0;
                    }
                    #endif
                    OnUpdate();
                }
            }
        }

        OnCleanup();

        return 0;
    };

public:
    bool OnInit();

    void OnResize(int width, int height)
    {
        DebugTrace("OnResize Width " << width << " height " << height);

        if ((GLfloat)width / (GLfloat)height <= ACTUAL_RATIO)
        {
                viewPort.x = 0;
                viewPort.y = -(static_cast<int>(width / ACTUAL_RATIO) - height) / 2;
                viewPort.w = width;
                viewPort.h = static_cast<int>(width / ACTUAL_RATIO);
        }
        else
        {
                viewPort.x = -(static_cast<int>(height * ACTUAL_RATIO) - width) / 2;
                viewPort.y = 0;
                viewPort.w = static_cast<int>(height * ACTUAL_RATIO);
                viewPort.h = height;
        }

        glViewport(viewPort.x, viewPort.y, viewPort.w, viewPort.h);

        JRenderer::GetInstance()->SetActualWidth(static_cast<float>(viewPort.w));
        JRenderer::GetInstance()->SetActualHeight(static_cast<float>(viewPort.h));
        glScissor(0, 0, width, height);

#if (!defined GL_ES_VERSION_2_0) && (!defined GL_VERSION_2_0)

        glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
        glLoadIdentity ();													// Reset The Projection Matrix

#if (defined GL_VERSION_ES_CM_1_1)
        glOrthof(0.0f, (float) (viewPort.w)-1.0f, 0.0f, (float) (viewPort.h)-1.0f, -1.0f, 1.0f);
#else
        gluOrtho2D(0.0f, (float) (viewPort.w)-1.0f, 0.0f, (float) (viewPort.h)-1.0f);
#endif
        glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
        glLoadIdentity ();													// Reset The Modelview Matrix

        glDisable (GL_DEPTH_TEST);
#endif
    }

    void OnKeyPressed(const SDL_KeyboardEvent& event);
    void OnMouseDoubleClicked(const SDL_MouseButtonEvent& event);
    void OnMouseClicked(const SDL_MouseButtonEvent& event);
    void OnMouseMoved(const SDL_MouseMotionEvent& event);
    void OnMouseWheel(int x, int y);

    void OnTouchEvent(const SDL_TouchFingerEvent& event);

    void OnEvent(SDL_Event* Event)
    {
        /* if(Event->type < SDL_USEREVENT)
        DebugTrace("Event received" << Event->type);*/
        switch(Event->type)
        {
        case SDL_QUIT:
            Running = false;
            break;

        /* On Android, this is triggered when the device orientation changed */
        case SDL_WINDOWEVENT:
            window = SDL_GetWindowFromID(Event->window.windowID);
            int h,w;
            SDL_GetWindowSize(window, &w, &h);
            OnResize(w, h);
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            OnKeyPressed(Event->key);
            break;

        case SDL_TEXTINPUT:
            //Typed characters for the text-entry capture (on-screen
            //keyboard). ASCII only: the game's bitmap fonts and config
            //values (URLs, model names, keys) have no wider needs.
            if (g_engine->TextInputActive())
            {
                for (const char * c = Event->text.text; *c; ++c)
                    if (*c >= 0x20 && *c <= 0x7E)
                        g_engine->PushTextChar(*c);
            }
            break;

        case SDL_MOUSEMOTION:
            OnMouseMoved(Event->motion);
            break;

        case SDL_MOUSEBUTTONDOWN:
            OnMouseClicked(Event->button);
            break;

        case SDL_MOUSEBUTTONUP:
            {
                Uint32 eventTime = SDL_GetTicks();
                if (eventTime - lastMouseUpTime <= 500)
                {
                    OnMouseDoubleClicked(Event->button);
                }
                else
                {
                    OnMouseClicked(Event->button);
                }
                lastMouseUpTime = eventTime;
            }
            break;

        case SDL_MOUSEWHEEL:
            OnMouseWheel(Event->wheel.x, Event->wheel.y);
            break;

        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
        	DebugTrace("Finger Up/Down/Motion detected:" );
            OnTouchEvent(Event->tfinger);
            break;

        case SDL_MULTIGESTURE:
            DebugTrace("Multigesture : touchId " << Event->mgesture.touchId
                    << ", x " << Event->mgesture.x
                    << ", y " << Event->mgesture.y
                    << ", dTheta " << Event->mgesture.dTheta
                    << ", dDist " << Event->mgesture.dDist
                    << ", numFingers " << Event->mgesture.numFingers);
            break;

        case SDL_JOYBALLMOTION:
            DebugTrace("Flick gesture detected, x: " << Event->jball.xrel << ", y: " << Event->jball.yrel);
            g_engine->Scroll(Event->jball.xrel, Event->jball.yrel);
            break;
        }
    }

    void OnUpdate();

    void OnCleanup()
    {
        if (glContext)
            SDL_GL_DeleteContext(glContext);
        if (window)
            SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

static const struct { LocalKeySym keysym; JButton keycode; } gDefaultBindings[] =
{  
	/* windows controls */
	{ SDLK_LCTRL,         JGE_BTN_CTRL },
	{ SDLK_RCTRL,         JGE_BTN_CTRL },
	{ SDLK_RETURN,        JGE_BTN_MENU },
	{ SDLK_KP_ENTER,      JGE_BTN_MENU },
	{ SDLK_ESCAPE,        JGE_BTN_MENU },
	{ SDLK_UP,            JGE_BTN_UP },
	{ SDLK_DOWN,          JGE_BTN_DOWN },
	{ SDLK_LEFT,          JGE_BTN_LEFT },
	{ SDLK_RIGHT,         JGE_BTN_RIGHT },
	{ SDLK_w,             JGE_BTN_UP },
	{ SDLK_s,             JGE_BTN_DOWN },
	{ SDLK_a,             JGE_BTN_LEFT },
	{ SDLK_d,             JGE_BTN_RIGHT },
	{ SDLK_q,             JGE_BTN_PREV },
	{ SDLK_e,             JGE_BTN_NEXT },
	{ SDLK_i,             JGE_BTN_CANCEL },
	{ SDLK_l,             JGE_BTN_OK },
	{ SDLK_SPACE,         JGE_BTN_OK },
	{ SDLK_k,             JGE_BTN_SEC },
	{ SDLK_j,             JGE_BTN_PRI },
	{ SDLK_b,             JGE_BTN_SOUND },
	{ SDLK_f,             JGE_BTN_FULLSCREEN },

	/* old Qt ones, basically modified to comply with the N900 keyboard
	{ SDLK_a,             JGE_BTN_NEXT },
	{ SDLK_TAB,           JGE_BTN_CANCEL },
	{ SDLK_q,             JGE_BTN_PREV },
	{ SDLK_BACKSPACE,     JGE_BTN_CTRL },
	*/

	/* Android customs */
	//Android Back = cancel / back one step (platform convention). The game
	//menu has its own gesture (swipe up -> KEYCODE_MENU) so the two do not
	//collide.
	{ SDLK_AC_BACK,       JGE_BTN_SEC },
	{ SDLK_MENU,          JGE_BTN_MENU },
	/* Android/maemo volume button mapping */
	{ SDLK_VOLUMEUP,      JGE_BTN_PREV },
	{ SDLK_VOLUMEDOWN,    JGE_BTN_SEC},
};

void JGECreateDefaultBindings()
{
	for (signed int i = sizeof(gDefaultBindings)/sizeof(gDefaultBindings[0]) - 1; i >= 0; --i)
		g_engine->BindKey(gDefaultBindings[i].keysym, gDefaultBindings[i].keycode);
}

int JGEGetTime()
{
	return (int)SDL_GetTicks();
}

bool JGEToggleFullscreen()
{
	//cycle between the display modes
	++gDisplayMode;
	if (gDisplayMode > DisplayMode_fullscreen)
		gDisplayMode = DisplayMode_lowRes;

	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);

	int width = 0;
	int height = 0;

	switch (gDisplayMode)
	{
	case DisplayMode_fullscreen:
		SDL_SetWindowSize(g_SdlApp->window, mode.w, mode.h);
		return (SDL_SetWindowFullscreen(g_SdlApp->window, SDL_TRUE) == 0);

		break;

	case DisplayMode_hiRes:
		width = SCREEN_WIDTH * 2;
		height = SCREEN_HEIGHT * 2;
		break;

	case DisplayMode_lowRes:
	default:
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		break;
	}

	if (SDL_SetWindowFullscreen(g_SdlApp->window, SDL_FALSE) < 0)
	{
		return false;
	}

	int x = (mode.w - width) / 2;
	int y = (mode.h - height) / 2;

	SDL_SetWindowPosition(g_SdlApp->window, x, y);
	SDL_SetWindowSize(g_SdlApp->window, width, height);

	return true;
}

bool InitGame(void)
{
	g_engine = JGE::GetInstance();
	g_app = g_launcher->GetGameApp();
	g_app->Create();
	g_engine->SetApp(g_app);
#ifdef ANDROID
    DebugTrace("Can I Set JNI Params ?");
     if (mJNIEnv)
        DebugTrace("mJNIEnv is ok");
    if (mJNIEnv && mJNIClass)
    {
        DebugTrace("Setting JNI Params");
        g_engine->SetJNIEnv(mJNIEnv, *mJNIClass);
    }
#endif	

	JRenderer::GetInstance()->Enable2D();
	lastTickCount = JGEGetTime();

	return true;
}

void DestroyGame(void)
{
	//Init can fail before InitGame ever runs (window/GL context refused);
	//tearing down a game that never started must not become a null deref.
	if (!g_engine)
		return;
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

void SdlApp::OnUpdate()
{
	static int tickCount = 0;
	tickCount = JGEGetTime();
	int64_t dt = (tickCount - lastTickCount);
	lastTickCount = tickCount;

	if(g_engine->IsDone())
	{
		SDL_Event event;
		event.user.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}

	//Fast clock: feed a fixed game-time step instead of wall time (see
	//fastClockDt above). Real elapsed time still drives IsDone/tick counts.
	float dtSec = (fastClockDt() > 0) ? fastClockDt() : (float)dt / 1000.0f;

	try
	{
		g_engine->SetDelta(dtSec);
		g_engine->Update(dtSec);
	}
	catch(out_of_range& oor)
	{
		cerr << oor.what();
	}

	//Keep SDL's text-input state in sync with the engine's capture flag
	//(SDL3 via sdl2-compat delivers SDL_TEXTINPUT only while started; on
	//platforms with a soft keyboard this also summons/dismisses it).
	{
		static bool sdlTextInputOn = false;
		bool want = g_engine && g_engine->TextInputActive();
		if (want != sdlTextInputOn)
		{
			if (want)
				SDL_StartTextInput();
			else
				SDL_StopTextInput();
			sdlTextInputOn = want;
		}
	}

	//Headless: nothing to draw and no window to swap (see OnInit).
	if (!window)
		return;

	if(g_engine)
		g_engine->Render();

#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
	//WAGIC_FRAMEDUMP=<dir>[:N]: write every Nth rendered frame (default 1)
	//as a PPM into <dir> - the capture side of visual-defect reproduction
	//(WAGIC_KEYSCRIPT in JGE::Update is the input side). Read BEFORE the
	//swap so the dump is the frame just composed, not the stale back buffer.
	{
		static const char * dumpSpec = getenv("WAGIC_FRAMEDUMP");
		if (dumpSpec && *dumpSpec)
		{
			static string dumpDir;
			static int every = 1;
			static unsigned int frameNo = 0;
			if (dumpDir.empty())
			{
				dumpDir = dumpSpec;
				size_t colon = dumpDir.rfind(':');
				if (colon != string::npos)
				{
					every = atoi(dumpDir.c_str() + colon + 1);
					if (every < 1) every = 1;
					dumpDir.erase(colon);
				}
			}
			frameNo++;
			//Draw-attribution runs: mark frame boundaries in the thin/draw
			//log so each dumped frame's draw list can be isolated.
			{
				static const char * tl = getenv("WAGIC_THINLOG");
				if (tl && getenv("WAGIC_DRAWLOG"))
				{
					FILE * mf = fopen(tl, "a");
					if (mf) { fprintf(mf, "FRAME %u\n", frameNo); fclose(mf); }
				}
			}
			if (frameNo % every == 0)
			{
				int w = (int) JRenderer::GetInstance()->GetActualWidth();
				int h = (int) JRenderer::GetInstance()->GetActualHeight();
				static std::vector<unsigned char> px;
				px.resize((size_t) w * h * 3);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, px.data());
				char name[512];
				snprintf(name, sizeof(name), "%s/frame-%06u.ppm", dumpDir.c_str(), frameNo);
				FILE * f = fopen(name, "wb");
				if (f)
				{
					fprintf(f, "P6\n%d %d\n255\n", w, h);
					//glReadPixels is bottom-up; write rows reversed.
					for (int row = h - 1; row >= 0; row--)
						fwrite(px.data() + (size_t) row * w * 3, 1, (size_t) w * 3, f);
					fclose(f);
				}
			}
		}
	}
#endif //_DEBUG || WAGIC_DEVLOGS


	SDL_GL_SwapWindow(window);
}

//WAGIC_PADLOG (Android: flag file User/padlog.on): platform half of the
//on-screen-keyboard input trace - what SDL actually delivers. Pairs with the
//SimplePad-side PADLOG to bisect "keyboard unresponsive" between layers.
//Dev builds only - diagnostics are compiled out of release builds.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
static FILE * jgePadlogFile()
{
    static FILE * out = NULL;
    static int state = 0;
    if (state == 0)
    {
        if (getenv("WAGIC_PADLOG"))
            { out = stderr; state = 1; }
#ifdef ANDROID
        else if (access("/sdcard/Wagic/User/padlog.on", F_OK) == 0)
            { out = fopen("/sdcard/Wagic/User/padlog.txt", "a"); state = out ? 1 : -1; }
#endif
        else state = -1;
    }
    return (state == 1) ? out : NULL;
}
#define JGEPADLOG(...) do { FILE * f_ = jgePadlogFile(); if (f_) { fprintf(f_, __VA_ARGS__); fflush(f_); } } while (0)
#else
#define JGEPADLOG(...) ((void)0)
#endif //_DEBUG || WAGIC_DEVLOGS

void SdlApp::OnKeyPressed(const SDL_KeyboardEvent& event)
{
	//Text-entry capture (the on-screen keyboard is open and a physical
	//keyboard is present): keys become typed characters instead of game
	//buttons, so text entry never fights the game's key bindings.
	//Printable characters arrive through SDL_TEXTINPUT; only the edit
	//controls are taken from the raw key events here.
	JGEPADLOG("sdl key type=%d sym=%d textinput=%d\n",
	          (int)event.type, (int)event.keysym.sym, g_engine->TextInputActive() ? 1 : 0);
	if (g_engine->TextInputActive())
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.keysym.sym == SDLK_BACKSPACE)
				g_engine->PushTextChar('\b');
			else if (event.keysym.sym == SDLK_RETURN || event.keysym.sym == SDLK_KP_ENTER)
				g_engine->PushTextChar('\n');
			//Android hardware back = SDLK_AC_BACK: cancel the edit, same as
			//Escape. Before this it fell through and was swallowed - back
			//"did nothing" while the on-screen keyboard was open.
			else if (event.keysym.sym == SDLK_ESCAPE || event.keysym.sym == SDLK_AC_BACK)
				g_engine->PushTextChar(0x1b);
		}
		return;
	}
	if (event.type == SDL_KEYDOWN)
	{
		g_engine->HoldKey_NoRepeat((LocalKeySym)event.keysym.sym);
	}
	else if(event.type == SDL_KEYUP)
	{
		g_engine->ReleaseKey((LocalKeySym)event.keysym.sym);
	}
}

void SdlApp::OnMouseMoved(const SDL_MouseMotionEvent& event)
{
	int actualWidth = (int) JRenderer::GetInstance()->GetActualWidth();
	int actualHeight = (int) JRenderer::GetInstance()->GetActualHeight();

	if (event.y >= viewPort.y &&
		event.y <= viewPort.y + viewPort.h &&
		event.x >= viewPort.x &&
		event.x <= viewPort.x + viewPort.w)
	{
		g_engine->LeftClicked(
			((event.x-viewPort.x)*SCREEN_WIDTH)/actualWidth,
			((event.y-viewPort.y)*SCREEN_HEIGHT)/actualHeight);
	}
}

void SdlApp::OnMouseDoubleClicked(const SDL_MouseButtonEvent& event)
{
#if (defined ANDROID) || (defined IOS)
	if(event.button == SDL_BUTTON_LEFT) /* Left button */
	{
		g_engine->HoldKey_NoRepeat(JGE_BTN_OK);
	}
#endif
}

void SdlApp::OnMouseWheel(int x, int y)
{
  if(!x && y)
  { // Vertical wheel
    if(y > 0)
    {
      g_engine->HoldKey_NoRepeat(JGE_BTN_UP);
    }
    else
    {
      g_engine->HoldKey_NoRepeat(JGE_BTN_DOWN);
    }
  }
  else if(x && !y)
  { // Horizontal wheel
    g_engine->HoldKey_NoRepeat(JGE_BTN_LEFT);
  }
  else
  {
    g_engine->HoldKey_NoRepeat(JGE_BTN_RIGHT);
  }
}

void SdlApp::OnMouseClicked(const SDL_MouseButtonEvent& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button == SDL_BUTTON_LEFT) /* Left button */
		{
			// this is intended to convert window coordinate into game coordinate.
			// this is correct only if the game and window have the same aspect ratio, otherwise, it's just wrong
			int actualWidth = (int) JRenderer::GetInstance()->GetActualWidth();
			int actualHeight = (int) JRenderer::GetInstance()->GetActualHeight();

			if (event.y >= viewPort.y &&
				event.y <= viewPort.y + viewPort.h &&
				event.x >= viewPort.x &&
				event.x <= viewPort.x + viewPort.w)
			{
				g_engine->LeftClicked(
					((event.x-viewPort.x)*SCREEN_WIDTH)/actualWidth,
					((event.y-viewPort.y)*SCREEN_HEIGHT)/actualHeight);
#if (!defined ANDROID) && (!defined IOS)
				g_engine->HoldKey_NoRepeat(JGE_BTN_OK);
#endif
			}
			else if(event.y < viewPort.y)
			{
				g_engine->HoldKey_NoRepeat(JGE_BTN_MENU);
			}
			else if(event.y > viewPort.y + viewPort.h)
			{
				g_engine->HoldKey_NoRepeat(JGE_BTN_NEXT);
			}
		}
		else if(event.button == SDL_BUTTON_RIGHT) /* Right button */
		{ /* next phase please */
			g_engine->HoldKey_NoRepeat(JGE_BTN_PREV);
		}
		else if(event.button == SDL_BUTTON_MIDDLE) /* Middle button */
		{ /* interrupt please */
			g_engine->HoldKey_NoRepeat(JGE_BTN_SEC);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		if(event.button == SDL_BUTTON_LEFT)
		{
			if (event.y >= viewPort.y &&
				event.y <= viewPort.y + viewPort.h &&
				event.x >= viewPort.x &&
				event.x <= viewPort.x + viewPort.w)
			{
#if (!defined ANDROID) && (!defined IOS)
				g_engine->ReleaseKey(JGE_BTN_OK);
#endif
			}
			else if(event.y < viewPort.y)
			{
				g_engine->ReleaseKey(JGE_BTN_MENU);
			}
			else if(event.y > viewPort.y + viewPort.h)
			{
				g_engine->ReleaseKey(JGE_BTN_NEXT);
			}
		}
		else if(event.button == SDL_BUTTON_RIGHT)
		{ /* next phase please */
			g_engine->ReleaseKey(JGE_BTN_PREV);
		}
		else if(event.button == SDL_BUTTON_MIDDLE)
		{ /* interrupt please */
			g_engine->ReleaseKey(JGE_BTN_SEC);
		}
	}
}


void SdlApp::OnTouchEvent(const SDL_TouchFingerEvent& event)
{
    JGEPADLOG("sdl touch type=%d finger=%d x=%.1f y=%.1f\n",
              (int)event.type, (int)event.fingerId, (float)event.x, (float)event.y);
    // Track ONE active finger, latest-down wins. The old filter honored only
    // fingerId 0, so with a second finger resting on the screen (holding
    // thumb, palm edge) every real tap arrived as finger 1 and was discarded
    // wholesale - the UI looked completely unresponsive. Adopting the most
    // recent FINGERDOWN as the pointer keeps single-finger behavior identical
    // and makes taps work regardless of what else is touching the glass.
    static int sActiveFinger = -1;
    if (event.type == SDL_FINGERDOWN)
        sActiveFinger = (int)event.fingerId;
    if ((int)event.fingerId == (sActiveFinger < 0 ? 0 : sActiveFinger))
    {
        if (event.type == SDL_FINGERUP)
            sActiveFinger = -1;
        if (event.y >= viewPort.y &&
            event.y <= viewPort.y + viewPort.h &&
            event.x >= viewPort.x &&
            event.x <= viewPort.x + viewPort.w)
        {
            int actualWidth = (int) JRenderer::GetInstance()->GetActualWidth();
            int actualHeight = (int) JRenderer::GetInstance()->GetActualHeight();

            Uint32 eventTime = SDL_GetTicks();
            if (event.type == SDL_FINGERDOWN)
            {
                mMouseDownX = event.x;
                mMouseDownY = event.y;

                lastFingerDownTime = eventTime;
            }


#if (defined ANDROID) || (defined IOS)
            if (event.type == SDL_FINGERUP)
            {
                if (eventTime - lastFingerDownTime <= kTapEventTimeout)
                {
                    // treat an up finger within 50 pixels of the down finger coords as a double click event
                    if (abs(mMouseDownX - event.x) < kHitzonePliancy && abs(mMouseDownY - event.y) < kHitzonePliancy)
                    {
                    	DebugTrace("Pressing OK BUtton");
                        JGEPADLOG("sdl tap -> JGE_BTN_OK\n");
                        g_engine->HoldKey_NoRepeat(JGE_BTN_OK);
                    }
                }
            }
      		else      
#endif
			g_engine->LeftClicked(
                ((event.x - viewPort.x) * SCREEN_WIDTH) / actualWidth,
                ((event.y - viewPort.y) * SCREEN_HEIGHT) / actualHeight);
	
        }
    }
}

bool SdlApp::OnInit()
{
	int window_w, window_h;

#if !defined(WIN32) && !defined(PSP) && !defined(VITA) && !defined(IOS)
	//Art loading opens set zips in bursts (330+ sets); under a 1024-fd soft
	//limit the burst exhausts descriptors and the affected cards silently
	//render art-less (proven: same game, art-less under LimitNOFILE=1024,
	//complete under 16384 - 2026-08-10). Raise the soft limit to the hard
	//limit; the steady-state usage stays tiny either way.
	{
		struct rlimit rl;
		if (getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur < rl.rlim_max)
		{
			rl.rlim_cur = rl.rlim_max;
			setrlimit(RLIMIT_NOFILE, &rl);
		}
	}
#endif

	//WAGIC_HEADLESS: no window, no GL context. SDL's dummy video driver
	//satisfies init on a machine with no display; every GL touch is skipped
	//inside JGfx.cpp under the same flag, and the game loop (Update) is
	//renderer-free, so logic - test suite, self-play - runs unchanged.
	if (getenv("WAGIC_HEADLESS"))
	{
		SDL_setenv("SDL_VIDEODRIVER", "dummy", 1); //SDL_setenv: setenv is POSIX-only
		SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			return false;
		}
		JGECreateDefaultBindings();
		if (!InitGame())
		{
			cerr << "Could not init the game\n";
			return false;
		}
		return true;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	DebugTrace("Video Display : h " << displayMode.h << ", w " << displayMode.w);

#if (defined ANDROID) || (defined IOS)
	window_w = displayMode.w;
	window_h = displayMode.h;
#else
	window_w = ACTUAL_SCREEN_WIDTH;
	window_h = ACTUAL_SCREEN_HEIGHT;
#endif

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,    	    8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,  	    8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,   	    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  	    8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  	    16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,		    32);

	//No accumulation buffer: nothing in the renderer uses one, and requesting
	//accum bits makes pixel-format selection FAIL outright on strict WGL
	//drivers and under Proton/Wine - the window never opens and the game
	//dies before its first frame (Windows-alpha triage, 2026-08-19).

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

#ifdef ANDROID
	const Uint32 kWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
#else
	const Uint32 kWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
#endif
	window = SDL_CreateWindow(g_launcher->GetName(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_w, window_h, kWindowFlags);
	if (window)
		glContext = SDL_GL_CreateContext(window);
	if (!window || !glContext)
	{
		//Multisample is a nice-to-have; on drivers whose pixel-format search
		//rejects it the game must still open. Retry once without it.
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		if (window)
			SDL_DestroyWindow(window);
		window = SDL_CreateWindow(g_launcher->GetName(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			window_w, window_h, kWindowFlags);
		glContext = window ? SDL_GL_CreateContext(window) : NULL;
	}
	if (!window || !glContext)
	{
		//A silent exit here reads as "nothing happened" to the player; name
		//the failing stage and the driver's own reason.
		fprintf(stderr, "wagic: could not create a window/GL context: %s\n", SDL_GetError());
#ifndef ANDROID //the Android port's trimmed SDL has no messagebox; logcat carries the line above
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Wagic",
			"Could not create an OpenGL window. Please update your graphics drivers.", NULL);
#endif
		return false;
	}
	SDL_GL_SetSwapInterval(1);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Black Background (yes that's the way fuckers)
#if (defined GL_ES_VERSION_2_0) || (defined GL_VERSION_2_0)
#if (defined GL_ES_VERSION_2_0)
	glClearDepthf(1.0f);					// Depth Buffer Setup
#else
	glClearDepth(1.0f);					// Depth Buffer Setup
#endif// (defined GL_ES_VERSION_2_0)

	glDepthFunc(GL_LEQUAL);				// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);				// Enable Depth Testing

#else
#if (defined GL_VERSION_ES_CM_1_1)
	glClearDepthf(1.0f);					// Depth Buffer Setup
#else
	glClearDepth(1.0f);					// Depth Buffer Setup
#endif
	glDepthFunc(GL_LEQUAL);				// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);				// Enable Depth Testing
	glShadeModel(GL_SMOOTH);				// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Set Perspective Calculations To Most Accurate

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Set Line Antialiasing
	glEnable(GL_LINE_SMOOTH);				// Enable it!
	glEnable(GL_TEXTURE_2D);

#endif

	glEnable(GL_CULL_FACE);				// do not calculate inside of poly's
	glFrontFace(GL_CCW);					// counter clock-wise polygons are out

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);				// Enable Clipping

	JGECreateDefaultBindings();

	if (!InitGame())
	{
		cerr << "Could not init the game\n";
		return false;
	}

	OnResize(window_w, window_h);

	return true;
};

#if (defined ANDROID) || (defined WIN32)
int SDL_main(int argc, char * argv[])
#else
int main(int argc, char* argv[])
#endif //ANDROID
{

#if (defined ANDROID)
	if (argc > 2)
    {
		mJNIEnv = (JNIEnv * )argv[1];
        mJNIClass = (jclass * )argv[2];
    }
#endif		

	DebugTrace("I R in da native");

#ifdef WITH_GPT_AI
	//Reply-parser self-test: run the wave-20 parse/salvage checks against the
	//real static parsers and exit, no game/display needed (WAGIC_GPT_PARSETEST=1).
	//Declared extern to keep the heavy mtg AIPlayerGPT header out of the JGE layer.
	extern void wagicGptParseSelfTest();
	if (getenv("WAGIC_GPT_PARSETEST"))
	{
		wagicGptParseSelfTest();
		return 0;
	}
#endif

	g_launcher = new JGameLauncher();

	u32 flags = g_launcher->GetInitFlags();

	if ((flags&JINIT_FLAG_ENABLE3D)!=0)
	{
		JRenderer::Set3DFlag(true);
	}

	g_SdlApp = new SdlApp();

	int result = g_SdlApp->OnExecute();

	if (g_launcher)
		delete g_launcher;

	if(g_SdlApp)
		delete g_SdlApp;

	// Shutdown
	DestroyGame();

	return result;
}
