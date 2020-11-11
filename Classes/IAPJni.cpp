#include "IAPJni.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define  IAPLOG_TAG    "IAPJni.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,IAPLOG_TAG,__VA_ARGS__)
#endif


std::string checkSIM()
{
	std::string ret="0";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "IAP/IAPJni", "checkSIM", "()Ljava/lang/String;")) {
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		ret = JniHelper::jstring2string(str);
		t.env->DeleteLocalRef(str);
	}
#endif
	return ret;
}

std::string getChannelID()
{
	std::string ret="0";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "IAP/IAPJni", "getChannelID", "()Ljava/lang/String;")) {
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		ret = JniHelper::jstring2string(str);
		t.env->DeleteLocalRef(str);
	}
#endif
	return ret;
}


extern "C"
{

	//使用预编译，对当前平台为Android时才会导入如下头文件
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JNIEXPORT void JNICALL Java_IAP_IAPJni_orderBombSuccess(JNIEnv*  env, jobject thiz, jstring payCode,jstring orderID,jstring tradeID)
	{
		std::string _payCode = JniHelper::jstring2string(payCode); 
		std::string _orderID = JniHelper::jstring2string(orderID); 
		std::string _tradeID = JniHelper::jstring2string(tradeID);

		//LOGD("[%s],[%s],[%s]",_payCode.c_str(),_orderID.c_str(),_tradeID.c_str());
		if (_payCode=="30000844763701" || _payCode=="30000844763702" || _payCode=="30000844763703" || _payCode=="30000844763704" || _payCode=="30000844763705"){//购买英雄之魂成功
			NotificationCenter::getInstance()->postNotification("pay_soul_successful",String::createWithFormat("%s",_payCode.c_str()));
		}else if (_payCode=="30000844763706" || _payCode=="30000844763707"){//购买英雄成功
			NotificationCenter::getInstance()->postNotification("pay_hero_successful",String::createWithFormat("%s",_tradeID.c_str()));
		}
	}


	JNIEXPORT void JNICALL Java_IAP_IAPJni_orderFaild(JNIEnv*  env, jobject thiz)
	{

	}
#endif

};