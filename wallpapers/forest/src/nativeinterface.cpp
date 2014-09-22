#include "nativeinterface.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "gameplay.h"

using namespace gameplay;

JNIEXPORT void JNICALL
Java_com_faustgames_forest_NativeInterface_initAssetManager( JNIEnv* env, jobject thiz, jobject assetManager)
{
	Game* game = Game::getInstance();
    Platform* platform = Platform::create(game);
    GP_ASSERT(platform);
	AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
	Platform::setAssetManager(mgr);
}

JNIEXPORT void JNICALL 
Java_com_faustgames_forest_NativeInterface_onSurfaceCreated(JNIEnv* env, jobject thiz)
{
}

JNIEXPORT void JNICALL 
Java_com_faustgames_forest_NativeInterface_onSurfaceChanged(JNIEnv* env, jobject thiz, jint width, jint height)
{
	Platform::setViewportSize(width, height);
	if (Game::getInstance()->getState() == Game::UNINITIALIZED)
    {
        Game::getInstance()->run();
    }
    else
    {
        Game::getInstance()->restoreDeviceObjects();
		Mesh::RestoreDeviceObjects();
		Effect::RestoreDeviceObjects();
    }	
}

JNIEXPORT void JNICALL 
Java_com_faustgames_forest_NativeInterface_onDrawFrame(JNIEnv* env, jobject thiz)
{
	Game::getInstance()->frame();
}