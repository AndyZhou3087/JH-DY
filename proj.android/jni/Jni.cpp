#include <jni.h>
#include "ShopLayer.h"
#include "GameScene.h"
extern "C"
{
	JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_sendMessage(JNIEnv *env, jobject obj,jint msg);
	JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_changeGameStates(JNIEnv *env, jobject obj,jint status);
	JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_saveData(JNIEnv *env, jobject obj);
}

JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_sendMessage(JNIEnv *env, jobject obj,jint msg)
{
	ShopLayer::setMessage((PYARET)msg);
}

JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_saveData(JNIEnv *env, jobject obj)
{
	GameScene::saveMyData();
}

JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_changeGameStates(JNIEnv *env, jobject obj,jint status)
{
	GameScene::changeGameStates(status);
}

