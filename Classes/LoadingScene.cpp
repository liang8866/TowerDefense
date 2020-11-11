#pragma once
#include "LoadingScene.h"
#include "GameScene.h"
#include "CachManager.h"
#include "startScene.h"
USING_NS_CC;

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    scene->setTag(LOAD_SCENE_TAG);

    // 'layer' is an autorelease objec*t
	LoadingScene *layer=LoadingScene::create();
	layer->setTag(6);
    // add layer as a child to scene
    scene->addChild(layer);
	

    // return the scene
    return scene;
}
LoadingScene::LoadingScene(void)
{

	
}

LoadingScene::~LoadingScene(void)
{
}
// on "init" you need to initialize your instance
void LoadingScene::onEnter(){
	 Layer::onEnter();
}
 void LoadingScene::onExit(){
	 Layer::onExit();
 	 SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("loading0.plist");
 	 TextureCache::getInstance()->removeTextureForKey("loading0.png");
}
bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//取得屏幕大小  
	  Size size = CCDirector::getInstance()->getWinSize();  
	  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loading0.plist","loading0.png");
	  currentNum=0;
	  totalNum=0;
	  _sceneTag = 0; 
	  ArmatureDataManager::getInstance()->addArmatureFileInfo("loading0.png","loading0.plist","loading.ExportJson");
	  ArmatureDataManager::getInstance()->addArmatureFileInfo("td_boss_020.png","td_boss_020.plist","td_boss_02.ExportJson");

	  _loading  = Armature::create("loading");
	  _hero = Armature::create("td_boss_02");
	  this->addChild(_loading);
	  this->addChild(_hero);

	  _loading->getAnimation()->play("run");
	  _hero->getAnimation()->play("run");
	  _hero->getAnimation()->setSpeedScale(1.6f);
	  _hero->setScale(1.2f);
	   //_hero->getAnimation()->setan
	  Size visibleSize = Director::getInstance()->getVisibleSize();
	  Point origin = Director::getInstance()->getVisibleOrigin();
	  _loading->setPosition(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y);
	  _hero->setPosition(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y );
  
	 
	 /* (1)陆地上的白云移动 
      (2)防御塔打斗AI小BUG
	  (3)祭坛滑动条出现的数值和滑动的
	  (4)添加一个loadingScene 过渡场景（临时）*/
	//CCTextureCache::sharedTextureCache()->addImageAsync("",CallFuncN::create(CC_CALLBACK_1(LoadingScene::loadingCallBack, this)));  
     
	  //load plist
	
	  this->showTips();

    return true;
}
void LoadingScene::loadScene(int SceneTag)
{
	if (SceneTag == LEVEL_SCENE_TAG) //levelScene
	{
		 this->scheduleOnce(schedule_selector(LoadingScene::loadingCallback2),0.5f);
	} else if(SceneTag == GAME_SCENE_TAG) {//GameScene
		 this->scheduleOnce(schedule_selector(LoadingScene::loadingCallback3),2.0f);	
	} 
	_sceneTag = SceneTag;
}

void LoadingScene::loadingCallback2(float dt)
{
	requestLogin();
}
void LoadingScene::requestLogin()
{
	Connect* handle = Connect::getInstance();
	handle->login(this,httpresponse_selector(LoadingScene::onHttpRequestCompleted));
}
void LoadingScene::requestGameStar()
{
	auto postDatas = __String::createWithFormat("c=game&a=start&cid=%d&sid=%d",
		UserRecord::getInstance()->getSectionNum(),
		UserRecord::getInstance()->getSceneNum()
		)->getCString();
	Connect* handle = Connect::getInstance();
	handle->send(postDatas,this,httpresponse_selector(LoadingScene::onHttpRequestCompleted));
}
void LoadingScene::showError()
{
	Ui_Prompt *prompt = Ui_Prompt::create();
	prompt->setTag(251);
	prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	auto Text = PlistManager::getInstance()->getPlistMap("Text");
	__String* TextStr  =  static_cast<__String*>(Text->objectForKey("1001"));
	prompt->initWithRetry(TextStr->getCString());
	if(_sceneTag == GAME_SCENE_TAG) {
		prompt->setRightSelector(this,comm_selector(LoadingScene::requestGameStar));
	} else if(_sceneTag == LEVEL_SCENE_TAG) {
		prompt->setRightSelector(this,comm_selector(LoadingScene::requestLogin));
	}
	prompt->setLeftSelector(this,comm_selector(LoadingScene::backToStarScene));
	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(prompt,300);
}
void LoadingScene::backToStarScene()
{
	auto pTScene = startScene::createScene();
	TransitionFade* pTranslateScene = TransitionFade::create(0.6f,pTScene);;
	Director::getInstance()->replaceScene(pTranslateScene);
}
void LoadingScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	CCLOG("%s",&(*buffer->begin()));
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		showError();
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		if (_sceneTag == LEVEL_SCENE_TAG)
		{
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
				user->setSeatCount(Value(val["seat_count"].GetString()).asInt());
				user->setIsLogin(true);
				user->setClothesItemId(Value(val["clothesType"].GetString()).asInt());
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
						}
					}
				}
				if ( UserRecord::getInstance()->getIndexGuide()<2 && UserRecord::getInstance()->getSectionNum() == 1 ){
					EnterFreshMan();
				}else{
					Scene* pTScene = levelScene::createScene();
					TransitionFade *transitionfadeScene=TransitionFade::create(0.5,pTScene);
					Director::getInstance()->replaceScene(transitionfadeScene);
				}
			}
			

		} else if(_sceneTag == GAME_SCENE_TAG) {
			Scene* pTScene = GameScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(0.5,pTScene);
			Director::getInstance()->replaceScene(transitionfadeScene);
		}
	} else {

		showError();
		
	}
}
void LoadingScene::loadingCallback3(float dt)
{
	if (UserRecord::getInstance()->getSectionNum() == 1 && UserRecord::getInstance()->getSceneNum() <=3 ) 
	{
		Scene* pTScene = GameScene::createScene();
		TransitionFade *transitionfadeScene=TransitionFade::create(0.8,pTScene);
		Director::getInstance()->replaceScene(transitionfadeScene);
	} else {
		//通知服务器游戏开始
		requestGameStar();

	}

}

void LoadingScene::EnterFreshMan()
{
		UserRecord::getInstance()->setSectionNum(0);
		UserRecord::getInstance()->setSceneNum(1);
		UserRecord::getInstance()->setRound(0);
		//战斗准备场景,前二关教学场景
		UserRecord::getInstance()->takeUnitsInit();
		std::map<int,std::string> _map;
		_map.insert(make_pair(1,"war1"));
		_map.insert(make_pair(2,"war2"));
		_map.insert(make_pair(3,"tower1"));
		_map.insert(make_pair(4,"trap1"));
		_map.insert(make_pair(5,"trap2"));
		UserRecord::getInstance()->setTakeUnits(_map);
		//load plist
		CachManager::getInstance()->freshCach();
		
		Scene* pTScene = GameScene::createScene();
		TransitionFade *transitionfadeScene=TransitionFade::create(0.5,pTScene);
		Director::getInstance()->replaceScene(transitionfadeScene);
}


void LoadingScene::showTips(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto tipsbg  = Sprite::create("image/JiaZai.png");
	this->addChild(tipsbg,2);
	tipsbg->setPosition(visibleSize.width/2+origin.x,origin.y+50);
	__Dictionary* tipsPlist = __Dictionary::createWithContentsOfFile("plist/tips.plist");
	__String* _TipsValue = dynamic_cast<__String*>(tipsPlist->randomObject());
	auto *label = Label::createWithBMFont("word.fnt",_TipsValue->getCString());
	label->setPosition(visibleSize.width/2+origin.x,origin.y+50);
	label->setScale(0.7f);
	label->setColor(Color3B(228,189,144));
	this->addChild(label,3);
	
}


//  void LoadingScene::loadingCallBack(Ref *obj)//回调函数，每加载一张纹理就调用一次
 //  {
 // 	 ++currentNum;  
 // 	 char tmp[10];  
 // 	    //求得百分比  
 //      sprintf(tmp, "%d%%", (int)(((float)currentNum / totalNum) * 100));  
 // 	    //设置标签显示的内容  
 // 	 m_pLabelPercent->setString(tmp);  
 // 	  
 // 		  //如果当前加载的张数跟总数相等则进行场景跳转  
 // 		   if (currentNum == totalNum)  
 // 	    {  
 // 		 
 // 			this->removeChild(m_pLabelPercent, true);         
 // 			this->removeChild(m_pLabelLoading, true);  
 // 			         //场景跳转  
 // 			Scene* pTScene = GameScene::createScene();
 // 			TransitionFade *transitionfadeScene=TransitionFade::create(0.8,pTScene);
 // 			Director::getInstance()->replaceScene(transitionfadeScene);
 // 			  
 //   }  
 // 
 //  }