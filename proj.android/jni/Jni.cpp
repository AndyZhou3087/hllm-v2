#include <jni.h>
#include "ShopLayer.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "LoadingScene.h"
extern "C"
{
	JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_sendMessage(JNIEnv *env, jobject obj,jint msg);


	JNIEXPORT jint JNICALL Java_com_kuxx_hllm_JNI_doGamePause(JNIEnv *env, jobject obj);

	JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_exitGame(JNIEnv *env, jobject obj);

	JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_setMyHeadImg(JNIEnv *env, jobject obj, jstring url);

	JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_getUserData(JNIEnv *env, jobject obj, jstring localid, jstring nickname, jstring headurl, jstring channelid, jstring pkgname);

	JNIEXPORT jstring JNICALL Java_com_kuxx_hllm_JNI_getPostData(JNIEnv *env, jobject obj);

	JNIEXPORT jstring JNICALL Java_com_kuxx_hllm_JNI_getShareUrl(JNIEnv *env, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_kuxx_hllm_JNI_getIsBuyGift(JNIEnv *env, jobject obj,jint index);

	JNIEXPORT jstring JNICALL Java_com_kuxx_hllm_JNI_getShareImgPath(JNIEnv *env, jobject obj);

	JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_setBtnMode(JNIEnv *env, jobject obj, jint value);

	JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_cancelThirdLogin(JNIEnv *env, jobject obj);
}

JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_sendMessage(JNIEnv *env, jobject obj,jint msg)
{
	ShopLayer::setMessage(msg);
}

JNIEXPORT jint JNICALL Java_com_kuxx_hllm_JNI_doGamePause(JNIEnv *env, jobject obj)
{
	return GameScene::doGamePause();
}

JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_exitGame(JNIEnv *env, jobject obj)
{
	GameScene::exitGame();
}

JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_setMyHeadImg(JNIEnv *env, jobject obj, jstring url)
{
	GlobalData::setHeadImg(env->GetStringUTFChars(url, NULL));
}

JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_getUserData(JNIEnv *env, jobject obj, jstring localid, jstring nickname, jstring headurl, jstring channelid, jstring pkgname)
{
	LoadingScene::getUserData(env->GetStringUTFChars(localid, NULL), env->GetStringUTFChars(nickname, NULL), env->GetStringUTFChars(headurl, NULL), env->GetStringUTFChars(channelid, NULL), env->GetStringUTFChars(pkgname, NULL));
}

JNIEXPORT jstring JNICALL Java_com_kuxx_hllm_JNI_getPostData(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(GlobalData::getPostData());
}

JNIEXPORT jstring JNICALL Java_com_kuxx_hllm_JNI_getShareUrl(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(GlobalData::getShareUrl());
}

JNIEXPORT jboolean JNICALL Java_com_kuxx_hllm_JNI_getIsBuyGift(JNIEnv *env, jobject obj,jint index)
{
	return GlobalData::getIsBuyGift(index);
}

JNIEXPORT jstring JNICALL Java_com_kuxx_hllm_JNI_getShareImgPath(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(GlobalData::getShareImgPath());
}

JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_setBtnMode(JNIEnv *env, jobject obj, jint value)
{
	GlobalData::setBuyTextType(value);
}

JNIEXPORT void JNICALL Java_com_kuxx_hllm_JNI_cancelThirdLogin(JNIEnv *env, jobject obj)
{
	LoadingScene::cancelThirdLogin();
}
