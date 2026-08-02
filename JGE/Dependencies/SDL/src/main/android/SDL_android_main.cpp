
/* Include the SDL main definition header */
#include "SDL_main.h"
#include "JGE.h"

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>

#if defined(WITH_GPT_AI) && defined(WAGIC_HTTP_JNI)
#include "GptConfig.h"
#endif

// Called before SDL_main() to initialize JNI bindings in SDL library
extern "C" void SDL_Android_Init(JNIEnv* env, jclass cls);

// Library init
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JGE *mEngine = JGE::GetInstance();
    mEngine->setJVM(vm);
#if defined(WITH_GPT_AI) && defined(WAGIC_HTTP_JNI)
    //Load time is the only moment the GPT transport can resolve SDLActivity:
    //it runs under the app class loader, while the model-call worker later
    //attaches from native and would see only the system loader.
    JNIEnv * env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) == JNI_OK)
        gptAndroidCacheClass(env);
#endif
    return JNI_VERSION_1_4;
}

// Start up the SDL app
extern "C" void Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject obj)
{
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    /* Run the application code! */
    int status;
    char *argv[4];
    argv[0] = strdup("SDL_app");
	argv[1] = (char *)env;
    argv[2] = (char *)&cls;
    argv[3] = NULL;
    status = SDL_main(3, argv);

    /* We exit here for consistency with other platforms. */
    exit(status);
}

/* vi: set ts=4 sw=4 expandtab: */
