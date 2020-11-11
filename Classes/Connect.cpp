#include "Connect.h"
#include "Ui_Prompt.h"
#include "UserRecord.h"
#include "startScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
const std::string HOSTURL = "http://202.104.151.83:8088"; 
#else
const std::string HOSTURL = "http://202.104.151.83:8082"; 
#endif
const int INTERVAL_TIME   = 300;		//自动连接时间 
const int MAX_INTERVAL_TIME = 600; //连接间隔时间 600秒
using namespace network;
static Connect* _instance = nullptr;
Connect::Connect()
{

}
Connect::~Connect()
{

}

Connect* Connect::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Connect();
		_instance->init();
	}
	return _instance;
}
void Connect::destroyInstance()
{
	CC_SAFE_DELETE(_instance);
};

void Connect::init()
{
	_chronon = 0;
	_loadEnabled = true;
	_autoConnect = false;
	_lastTime = 0;
	_deltaTime = 0;
	_autoTag = 0;
	_connectCount = 0;
	reLoginFlag = true;
	_reLoginTag = 0;
	_widget = nullptr;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(Connect::logicUpdate),this,1.0f,false);
}

void Connect::loadSend(std::string data,Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector)
{
	createLoadPanel();
	send(data,pTarget,pSelector);
}
void Connect::send(std::string data,Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector)
{
	
	setAutoConnect(false);
	//如果上次连接的时间超过了最大间隔时间，重新登录
	if (_lastTime+INTERVAL_TIME <= _deltaTime && reLoginFlag)
	{
		RequestMap* rmap = new RequestMap();
		rmap->queryStr = data;
		rmap->pTarget = pTarget;
		rmap->pSelector = pSelector;
		rmap->retain();
		_requests.insert(++_autoTag,rmap);
		reLogin(_autoTag);
		return;
	}

	HttpRequest* request = new HttpRequest();
	request->setUrl(HOSTURL.c_str());
	request->setRequestType(HttpRequest::Type::POST);

	RequestMap* rmap = new RequestMap();
	rmap->queryStr = data;
	rmap->pTarget = pTarget;
	rmap->pSelector = pSelector;
	rmap->retain();
	_requests.insert(++_autoTag,rmap);
	//增加SESSIONID
	if (!UserRecord::getInstance()->getSeid().empty())
	{
		data.append(__String::createWithFormat("&ssd=%s",UserRecord::getInstance()->getSeid().c_str())->getCString());
	}
	data  = encryptData(data);
	CCLog("%s",data.c_str());
	request->setRequestData(data.c_str(),data.length());
	request->setResponseCallback(this, httpresponse_selector(Connect::onHttpRequestCompleted));
	HttpClient::getInstance()->send(request);
	request->setTag(Value(_autoTag).asString().c_str());
	request->release();
}
std::string Connect::encryptData(std::string data)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	JniMethodInfo minfo;//定义Jni函数信息结构体 
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"test/Test","encrypt", "([B)Ljava/lang/String;");  
	if (!isHave) { 
		CCLog("jni:no"); 
	}else{ 
		CCLog("jni:yes"); 
		jstring jstr;
		jbyteArray bytes = minfo.env->NewByteArray((jsize)strlen(data.c_str()));
		minfo.env->SetByteArrayRegion(bytes, 0, (jsize)strlen(data.c_str()), (jbyte*)data.c_str()); 
		jstr = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID,bytes);
		std::string dd = "s=";
		dd.append(JniHelper::jstring2string(jstr));
		//data = dd;
	}
#endif
	return data;
}
void Connect::reSend(int tag)
{
	Map<int,RequestMap*>::iterator it;
	it = _requests.find(tag);
	if (it == _requests.end())
	{
		return;
	}
	++_connectCount;
	this->send(it->second->queryStr,it->second->pTarget,it->second->pSelector);
	_requests.erase(it);
}
void Connect::reLogin(int tag)
{
	CCLOG("reconnect...");
	reLoginFlag = false;
	_reLoginTag = tag;
	login(this,httpresponse_selector(Connect::reLoginCallBack));
}
void Connect::reLoginCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() <= 0)
	{
		//报错
	}
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		//报错
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		reLoginFlag = true;
		if ( v.HasMember("timestamp")) {
			UserRecord::getInstance()->setModifyTime(v["timestamp"].GetInt());
			_deltaTime = _lastTime= UserRecord::getInstance()->getModifyTime();
		}
		
		if ( v.HasMember("info")) {
			rapidjson::Value &val=v["info"];
			auto user = UserRecord::getInstance();
			//基本信息
			user->setUid(Value(val["uid"].GetString()).asInt());
			user->setModifyTime(Value(val["modify_time"].GetString()).asInt());
			user->setPlatform(Value(val["platform"].GetString()).asInt());
			user->setIsAward(Value(val["isaward"].GetString()).asBool());
			user->setLoginDayNum(Value(val["login_day_num"].GetString()).asInt());
			user->setSeid(val["ssd"].GetString());
			user->setGold(Value(val["gold"].GetString()).asInt());
			user->setHeroSoul(Value(val["hero_soul"].GetString()).asInt());
			user->setStarCount(Value(val["star_count"].GetString()).asInt());
			user->setIsLogin(true);
			user->setClothesItemId(Value(val["clothesType"].GetString()).asInt());
			//道具物品数据
			if ( val.HasMember("props")) {
				if (!val.IsNull()){
					rapidjson::Value &vals = val["props"];
					for (rapidjson::Value::ConstMemberIterator itr = vals.MemberonBegin(); itr != vals.MemberonEnd(); ++itr) 
					{
						user->setGoodsCount(itr->name.GetString(),itr->value.GetString());
					}
				}
			}
			//已经解锁的单位数据
			if ( val.HasMember("object")) {
				rapidjson::Value &vals = val["object"];
				if (vals.IsArray())
				{
					for (rapidjson::SizeType i=0; i <vals.Size();++i)
					{
						rapidjson::Value &temp = vals[i];
						user->_objects.insert(make_pair(temp["name"].GetString(),temp["id"].GetString()));
					}
				}
			}
			//天赋
			if ( val.HasMember("upgrades")) {
				rapidjson::Value &vals = val["upgrades"];
				if (!vals.IsNull())
				{
					for (rapidjson::Value::ConstMemberIterator itr = vals.MemberonBegin(); itr != vals.MemberonEnd(); ++itr) 
					{
						if (itr->value.IsObject())
						{
							for (rapidjson::Value::ConstMemberIterator itrr = itr->value.MemberonBegin();
								itrr != itr->value.MemberonEnd(); ++itrr) 
							{
								user->setUpgradesItemLevel(itr->name.GetString(),itrr->name.GetString(),atoi(itrr->value.GetString()));
							}
						}
					}
				}
			}
			//过关的数据
			if (val.HasMember("chapter"))
			{
				rapidjson::Value &vals = val["chapter"];
				if (!vals.IsNull())
				{
					user->chapter.clear();
					for (rapidjson::Value::ConstMemberIterator itr = vals.MemberonBegin(); itr != vals.MemberonEnd(); ++itr) 
					{
						if (itr->value.IsObject())
						{
							for (rapidjson::Value::ConstMemberIterator itrr = itr->value.MemberonBegin();
								itrr != itr->value.MemberonEnd(); ++itrr) 
							{

								user->setChapterLog(
									atoi(itr->name.GetString()),
									atoi(itrr->name.GetString()),
									atoi(itrr->value.GetString())
									);
							}
						}
					}
				}
			}
			//过关的数据
			if (val.HasMember("hero"))
			{
				rapidjson::Value &vals = val["hero"];
				if (!vals.IsNull())
				{
					for (rapidjson::Value::ConstMemberIterator itr = vals.MemberonBegin(); itr != vals.MemberonEnd(); ++itr) 
					{
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_id,atoi(itr->value["hero_id"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_level,atoi(itr->value["level"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_star,atoi(itr->value["star"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_blessing,atoi(itr->value["blessing"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_skill1_level,atoi(itr->value["skill_1_level"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_skill2_level,atoi(itr->value["skill_2_level"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_skill3_level,atoi(itr->value["skill_3_level"].GetString()));
						user->setHeroKeyValue_Float( atoi(itr->name.GetString()),_HeroDataKey_Index_successRate,atol(itr->value["successRate"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_deletTime,atoi(itr->value["deletTime"].GetString()));
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_unLock,1);
					}
				}
			}
			user->flush();
		}
		//发送上次请求
		reSend(_reLoginTag);
	}  else {
		//报错
			


	}
}
//登录游戏 初始化用户数据
void Connect::login(Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector)
{
	auto user = UserRecord::getInstance();
	auto postDatas = __String::createWithFormat("c=login&uuid=%s",user->getUuid().c_str())->getCString();
	this->send(postDatas,pTarget,pSelector);
}

void Connect::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	closeLoadPanel();
	if (!response || !response->isSucceed() || response->getResponseCode() != 200)
	{
		setAutoConnect(false);
		closeLoadPanel();
		int code = response->getResponseCode();
		std::string temp;
		temp.append("ConnectError:");
		temp.append(response->getHttpRequest()->getRequestData());
		temp.append("code="+Value(code).asString());
		temp.append("errorBuff=");
		temp.append(response->getErrorBuffer());
		//CCLOG("%s",temp.c_str());
		CCLOG("%s",response->getHttpRequest()->getRequestData());
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		auto Text = PlistManager::getInstance()->getPlistMap("Text");
		__String* TextStr;
		if (_connectCount > 0)
		{
			TextStr  =  static_cast<__String*>(Text->objectForKey("1002"));
		} else {
			TextStr  =  static_cast<__String*>(Text->objectForKey("1001"));
		}
		string str = TextStr->getCString();
		while ( str.find_first_of("_n")!=string::npos )
		{
			size_t pos1 = str.find_first_of("_n"); 
			str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
		}
		prompt->initWithRetry(str.c_str());
		prompt->setRightConnectSelector(this,connect_selector(Connect::reSend),Value(response->getHttpRequest()->getTag()).asInt());
		prompt->setLeftSelector(this,comm_selector(Connect::cancelConnect));
		auto scene = Director::getInstance()->getRunningScene();
		if (!UserRecord::getInstance()->getIsGamePause())
		{
			NotificationCenter::getInstance()->postNotification(PAUSEKEY, (Ref*)1);
			setPauseGame(true);
		}
		scene->addChild(prompt,300);
		return;
	} 

	setAutoConnect(true);
	if (getPauseGame())
	{
		setPauseGame(false);
		NotificationCenter::getInstance()->postNotification(RESUMEKEY, NULL);
	}
	_connectCount = 0;
	auto curRequest = _requests.at(Value(response->getHttpRequest()->getTag()).asInt());


	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() <= 0)
	{
		//报错
	}
	buffer->push_back('\0');
	CCLOG("%s",&(*buffer->begin()));
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		//报错
		return ;  
	}  
	rapidjson::Value &v=doc;
	if ( v.HasMember("timestamp")) {
		UserRecord::getInstance()->setModifyTime(v["timestamp"].GetInt());
		_deltaTime = _lastTime = UserRecord::getInstance()->getModifyTime();
	}

	if (v["status"].GetInt() == 3) //登录超时
	{
		reLogin(Value(response->getHttpRequest()->getTag()).asInt());
		return;
	}
	if (curRequest->pTarget && curRequest->pSelector) 
	{
		(curRequest->pTarget->*curRequest->pSelector)(sender, response);
	}
	_requests.erase(Value(response->getHttpRequest()->getTag()).asInt());
}
void Connect::cancelConnect()
{
	auto  cScene = Director::getInstance()->getRunningScene();
	_connectCount = 0;
	_autoConnect = true;
	int tag = cScene->getTag();
	if (tag == 1)
	{


	} else {
		auto pTScene = startScene::createScene();
		TransitionFade* pTranslateScene = TransitionFade::create(0.6f,pTScene);;
		Director::getInstance()->replaceScene(pTranslateScene);
	}
}
void Connect::logicUpdate(float dt)
{
	++_deltaTime;
	if (!_autoConnect)
	{
		return;
	}

	++_chronon;
	if (_chronon == INTERVAL_TIME)
	{
		_chronon=0;
		doLoop();
	}
}
void Connect::doLoop()
{
	//login()
	reLogin(0);
}


void Connect::createLoadPanel()
{
	if (_widget == nullptr)
	{
		_widget = Ui_Loading::create();
	}
	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(_widget,500);
}
void Connect::closeLoadPanel(){
	if (_widget != nullptr)
	{
		_widget->removeFromParentAndCleanup(true);
		_widget = nullptr;
	}
}
void Connect::setAutoConnect(bool enabled){
	if (enabled == _autoConnect)
	{
		return;
	}
	_autoConnect = enabled;
	_chronon = 0;
}