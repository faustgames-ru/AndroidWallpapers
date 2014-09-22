#include <jni.h>

#ifndef _Included_com_FaustGames_blurengine_GLESRendererNative
#define _Included_com_FaustGames_blurengine_GLESRendererNative

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_faustgames_forest_NativeInterface_initAssetManager(JNIEnv* env, jobject thiz, jobject assetManager);
JNIEXPORT void JNICALL Java_com_faustgames_forest_NativeInterface_onSurfaceCreated(JNIEnv* env, jobject thiz);
JNIEXPORT void JNICALL Java_com_faustgames_forest_NativeInterface_onSurfaceChanged(JNIEnv* env, jobject thiz, jint width, jint height);
JNIEXPORT void JNICALL Java_com_faustgames_forest_NativeInterface_onDrawFrame(JNIEnv* env, jobject thiz);

#ifdef __cplusplus
}
#endif

#endif