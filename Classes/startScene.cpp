#pragma once
#include "startScene.h"
#include "levelScene.h"
#include "json/rapidjson.h"
#include "UserRecord.h"
#include "LoadingScene.h"
#include "Ui_Login.h"
#include "Connect.h"
#include "Ui_Prompt.h"
#include "CachManager.h"
#include "audioPlayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 

#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 

#endif

using namespace network;
USING_NS_CC;
Scene* startScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    scene->setTag(START_SCENE_TAG);
    // 'layer' is an autorelease objec*t
	startScene *layer=startScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
	

    // return the scene
    return scene;
}
void startScene::onEnter()
{
	

	Layer::onEnter();
	if (! UserRecord::getInstance()->getIsLogin())
	{
		//char postDatas[100] = {'\0'};
		//snprintf(postDatas,sizeof(postDatas),"c=login&uuid=%s",  UserRecord::getInstance()->getUuid().c_str());
		////sprintf(postDatas, "c=login&uuid=%s",  UserRecord::getInstance()->getUuid().c_str());
		//Connect* handle = Connect::getInstance();
		//handle->login(this,httpresponse_selector(startScene::onHttpRequestCompleted));
	}
	
	
}
void  startScene::onExit(){
	Layer::onExit();
	/*CCLOG("startScene::onExit()");*/
}
startScene::startScene(void)
{
	/*CCLOG("startScene::startScene(void)");*/
}

startScene::~startScene(void)
{
	
}
// on "init" you need to initialize your instance

bool startScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	CCLOG("startScene::init()");
     visibleSize = Director::getInstance()->getVisibleSize();
     origin = Director::getInstance()->getVisibleOrigin();
     winSize=Director::getInstance()->getWinSize();
	 

	 float hscale = visibleSize.height/640;

	 //背景、、、、、
	 Sprite *bg=Sprite::create("image/BJ_005.png");//这个的资源再上面的Ui_Login创建时候会加载全部的了
	 bg->setPosition(winSize.width/2,winSize.height/2);
	 this->addChild(bg,-3);
	 bg->setScaleX(winSize.width/bg->getContentSize().width);
	 bg->setScaleY(winSize.height/bg->getContentSize().height);
	 
	 //人物动画
	 ArmatureDataManager::getInstance()->addArmatureFileInfo("kaishijiemian0.png","kaishijiemian0.plist","kaishijiemian.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	 Armature* bg_arm = Armature::create("kaishijiemian");   
	 bg_arm->setPosition(winSize.width/2,320);
	 bg_arm->getAnimation()->play("play",-1,1);
	 this->addChild(bg_arm,1);

	 //logo出现
	 ArmatureDataManager::getInstance()->addArmatureFileInfo("startlogo0.png","startlogo0.plist","startlogo.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	 Armature* bg_logo = Armature::create("startlogo");   
	 bg_logo->setPosition(winSize.width/2,winSize.height/2);
	 bg_logo->setScale(hscale);
	 bg_logo->getAnimation()->play("logo",-1,0);
	// bg_logo->getAnimation()->setAnimationScale(2.0);
	 this->addChild(bg_logo,2);

	 bg_logo->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(startScene::onFrameEvent));

	 //背后的光
	startguang1=Sprite::create("startguang1.png");
	startguang2=Sprite::create("startguang2.png");
	startguang1->setOpacity(0);
	startguang2->setVisible(false);
	startguang1->setPosition(winSize.width/2,winSize.height-startguang1->getContentSize().height/2*hscale);
	startguang2->setPosition(winSize.width/2,winSize.height-startguang2->getContentSize().height/2*hscale);
 	startguang1->setScaleY(hscale);
 	startguang2->setScaleY(hscale);
	this->addChild(startguang1,0);
	this->addChild(startguang2,0);

	_widget = Ui_Login::create();
	this->addChild(_widget,30);
	//对手机返回键的监听 
	auto listener = EventListenerKeyboard::create(); 
	//和回调函数绑定 
	listener->onKeyReleased = CC_CALLBACK_2(startScene::onKeyReleased,this); 
	//添加到事件分发器中 
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this); 
    return true;
}


void startScene::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	// CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);


	if (strcmp(evt.c_str(),"comeout")==0)//出现光
	{
		

		float hscale = visibleSize.height/640;

		startguang1->setScale(0.3);
		float t=0.4;
		auto fade=FadeIn::create(t);
		auto sca=ScaleTo::create(t,1.0,hscale);
		auto spa=Spawn::create(fade,sca,NULL);
		auto delay=DelayTime::create(0.1);
		auto callback=CallFuncN::create(CC_CALLBACK_1(startScene::guangCallback, this));
		auto seq=Sequence::create(spa,delay,callback,NULL);
		startguang1->runAction(seq);
	}


}

void startScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		__Dictionary* text = __Dictionary::createWithContentsOfFile("plist/text.plist");
		__String* connect = static_cast<__String*>(text->objectForKey("1014"));
		__String* title = static_cast<__String*>(text->objectForKey("1015"));
		MessageBox(connect->getCString(),title->getCString());
	}
}
void startScene::guangCallback(Ref *r){
	this->schedule(schedule_selector(startScene::myupdate),(float) 0.5);
}

void startScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	CCLOG("%s",&(*buffer->begin()));
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		CCLOG("get json data err!:%s",doc.GetParseError()); 
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		if ( v.HasMember("info")) {
			rapidjson::Value &val=v["info"];
			auto user = UserRecord::getInstance();
		    //基本信息
			user->setUid(Value(val["uid"].GetString()).asInt());
			user->setUuid(val["uuid"].GetString());
			user->setBirthday(Value(val["birthday"].GetString()).asInt());
			user->setLastLogin(Value(val["lastlogin"].GetString()).asInt());
			user->setModifyTime(Value(val["modify_time"].GetString()).asInt());
			user->setPlatform(Value(val["platform"].GetString()).asInt());
			user->setIsAward(Value(val["isaward"].GetString()).asBool());
			user->setLoginDayNum(Value(val["login_day_num"].GetString()).asInt());
			user->setSeid(val["ssd"].GetString());
			user->setGold(Value(val["gold"].GetString()).asInt());
			user->setHeroSoul(Value(val["hero_soul"].GetString()).asInt());
			user->setStarCount(Value(val["star_count"].GetString()).asInt());
			user->setSeatCount(Value(val["seat_count"].GetString()).asInt());
			user->setIsLogin(true);
			//设置设备与服务器时间差
			setDeltaT(getDeviceTime()-Value(val["modify_time"].GetString()).asDouble());
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
			//限制商品数据
			if ( val.HasMember("goods_limit")) {
				rapidjson::Value &vals = val["goods_limit"];
				if (vals.IsArray())
				{
					for (rapidjson::SizeType i=0; i <vals.Size();++i)
					{
						rapidjson::Value &temp = vals[i];
						user->setGoodsLimit(atoi(temp["goods_id"].GetString()) ,atoi(temp["limit_count"].GetString()));
					}
				}
			}

			//随机关
			if ( val.HasMember("random_scene")) {
				rapidjson::Value &vals = val["random_scene"];
				if (vals.IsArray())
				{
					for (rapidjson::SizeType i=0; i <vals.Size();++i)
					{
						rapidjson::Value &temp = vals[i];
						user->setRandomScene(atoi(temp["cid"].GetString()) ,atoi(temp["sid"].GetString()));
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
						user->setObjectsUnLock(temp["name"].GetString(),temp["id"].GetString());
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
						user->setHeroKeyValue( atoi(itr->name.GetString()),_HeroDataKey_Index_revivieTime, NO);
						/*	
						_HeroData* herodata = new _HeroData();
						herodata->id = atoi(itr->value["hero_id"].GetString());
						herodata->level = atoi(itr->value["level"].GetString());
						herodata->star = atoi(itr->value["star"].GetString());
						herodata->blessing = atoi(itr->value["blessing"].GetString());
						herodata->skill1_level = atoi(itr->value["skill_1_level"].GetString());
						herodata->skill2_level = atoi(itr->value["skill_2_level"].GetString());
						herodata->skill3_level = atoi(itr->value["skill_3_level"].GetString());
						herodata->successRate = atoi(itr->value["successRate"].GetString());
						herodata->deletTime = atoi(itr->value["deletTime"].GetString());
						herodata->unLock = true;*/	
					}
				}
			}
			user->flush();
		}
	}else{
	}
}



void startScene::myupdate(float dt){
	  
	 if (startguang1!=nullptr)
	 {
		 if (startguang1->isVisible()==false)
		 {
			 startguang1->setVisible(true);
		 }else{
			  startguang1->setVisible(false);
		 }
	 }
	 if (startguang2!=nullptr)
	 {
		 if (startguang2->isVisible()==false)
		 {
			 startguang2->setVisible(true);
		 }else{
			 startguang2->setVisible(false);
		 }
	 }


	
	
}
