
#ifndef IAPJni
#define IAPJni

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

//使用预编译，对当前平台为Android时才会导入如下头文件
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include <jni.h>
	#include "2d/platform/android/jni/JniHelper.h"
	#include <android/log.h>
#endif

extern std::string checkSIM();
extern std::string getChannelID();

#ifdef __cplusplus
extern "C" {
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JNIEXPORT void JNICALL Java_IAP_IAPJni_orderBombSuccess(JNIEnv*  env, jobject thiz, jstring payCode,jstring orderID,jstring tradeID);
	JNIEXPORT void JNICALL Java_IAP_IAPJni_orderFaild(JNIEnv*  env, jobject thiz);
#endif

#ifdef __cplusplus
}
#endif

#endif