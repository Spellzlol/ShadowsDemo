#include <jni.h>
#include "JniHelpers.h"
#include "utils.h"
#include "SimpleEngine.h"
#include "EventManager.h"
#include "AsyncFIFO.h"
 
jobject m_bitmapLoader = nullptr;

void cacheEnv()
{
    JNIEnv* env = nullptr;

    JavaVM *jvm = JniHelpers::get()->getJavaVM();
    jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

    switch (ret) 
    {
        case JNI_OK :
            break;
                
        case JNI_EDETACHED :  
            if (jvm->AttachCurrentThread(&env, NULL) < 0)
            {
                LOGE("Failed to get the environment using AttachCurrentThread()");
                return;
            } 
            else 
            {
                break;
            }
                
        case JNI_EVERSION :
            LOGE("JNI interface version 1.4 not supported");

        default:
            LOGE("Failed to get the environment using GetEnv()");
            return;
    }

    JniHelpers::get()->setEnv(env);    
}

extern "C"
{

jint JNI_OnLoad(JavaVM *jvm, void*)
{
	LOGI(__FUNCTION__);

	SimpleEngine::create();
    JniHelpers::get()->setJavaVM(jvm);

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeCreate (JNIEnv* env, jobject, jobject bitmapLoader)
{
    LOGI(__FUNCTION__);

    m_bitmapLoader = env->NewGlobalRef(bitmapLoader);
}

// Java OpenGL thread
// -----
JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeInit(JNIEnv*, jobject, jint w, jint h)
{
	LOGI(__FUNCTION__);
    LOGI("GLView size: %d %d", w, h);
    cacheEnv();
    
    SimpleEngine::get()->init(w,h);
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeResize (JNIEnv*, jobject, jint, jint)
{
	LOGI(__FUNCTION__);
    // unused
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeRender (JNIEnv*, jobject)
{
	SimpleEngine::get()->mainLoop();
}

// Java OpenGL thread end
// -----

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativePause (JNIEnv*, jobject)
{
	LOGI(__FUNCTION__);

    AsyncFIFO::get()->pushBack([] 
    {
        SimpleEngine::get()->pause();
    });
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeResume (JNIEnv*, jobject)
{
	LOGI(__FUNCTION__);
    
    AsyncFIFO::get()->pushBack([] 
    {
        SimpleEngine::get()->resume();
    });
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeOnTouchBegin (JNIEnv*, jobject, jint id, jfloat x, jfloat y)
{
    AsyncFIFO::get()->pushBack([id,x,y] 
    {
        EventManager::get()->touchBegin(id, x, y);
    });
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeOnTouchMove (JNIEnv*, jobject, jint id, jfloat x, jfloat y)
{
    AsyncFIFO::get()->pushBack([id,x,y]
    {
        EventManager::get()->touchMove(id, x, y);
    });
}

JNIEXPORT void JNICALL Java_com_gsom_simpleengine_SimpleNativeWrapper_nativeOnTouchEnded (JNIEnv*, jobject, jint id, jfloat x, jfloat y)
{
    AsyncFIFO::get()->pushBack([id,x,y] 
    {
	   EventManager::get()->touchEnd(id, x, y);
    });
}

} // extern C