#pragma once
#include "GameScene.h"
#include "FreshManManager.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    scene->setTag(GAME_SCENE_TAG);
    // 'layer' is an autorelease objec*t
	GameScene *layer=GameScene::create(1,1);
    // add layer as a child to scene
	layer->setTag(1000);
    scene->addChild(layer);
	
    // return the scene
    return scene;
}
GameScene::GameScene(void)
{
	
}

GameScene::~GameScene(void)
{
	CCLOG("~gamescene");
	_gameLayer=nullptr;
	_hudLayer=nullptr;
}
// on "init" you need to initialize your instance

bool GameScene::init(int sectionNum,int sceneNum)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	_sectionNum = sectionNum;
	_sceneNum = sceneNum;
	_timeCount = 0;
	_disTime = 0;
	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	Value currentSection(UserRecord::getInstance()->getSectionNum());
	Value currentScene(UserRecord::getInstance()->getSceneNum());

	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	int monsterCount = monster->count();
	UserRecord::getInstance()->setMaxRound(monsterCount);
	UserRecord::getInstance()->resetGameData();
	UserRecord::getInstance()->setIsGameStart(false);


	_gameLayer = GameLayer::create();
    this->addChild(_gameLayer, 0);
    _hudLayer = HudLayer::create();
    this->addChild(_hudLayer, 1);
    _gameLayer->setHud(_hudLayer);
     visibleSize = Director::getInstance()->getVisibleSize();
     origin = Director::getInstance()->getVisibleOrigin();
     winSize=Director::getInstance()->getWinSize();

	 UserRecord::getInstance()->setIsGameOver(false);
	 UserRecord::getInstance()->setIsObjsNoCD(false);
	 UserRecord::getInstance()->setIsFreshMan(false);
	 UserRecord::getInstance()->setFreshManCurrentID(0);
	 scheduleOnce(schedule_selector(GameScene::addFreshman),0.3f);
	this->schedule(schedule_selector(GameScene::logicUpdate),1.0f);

	//对手机返回键的监听 
	auto listener = EventListenerKeyboard::create(); 
	//和回调函数绑定 
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased,this); 
	//添加到事件分发器中 
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this); 


    return true;
}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		__Dictionary* text = __Dictionary::createWithContentsOfFile("plist/text.plist");
		__String* connect = static_cast<__String*>(text->objectForKey("1014"));
		__String* title = static_cast<__String*>(text->objectForKey("1015"));
		MessageBox(connect->getCString(),title->getCString());
	}
}
void GameScene::addFreshman(float dt)
{
	//前三关加上教学部分
	if ( UserRecord::getInstance()->getSectionNum()==0 && UserRecord::getInstance()->getSceneNum()==1 ){

		NotificationCenter::sharedNotificationCenter()->postNotification(UInextPAUSEKEY, NULL);
		UserRecord::getInstance()->setIsFreshMan(true);
		FirstScene *freshman = FirstScene::create("100001");
		freshman->setPosition(Point::ZERO);
		freshman->setTag(99999);
		this->addChild(freshman,10);
	}else if ( UserRecord::getInstance()->getSectionNum()==0 && UserRecord::getInstance()->getSceneNum()==2 ){
		NotificationCenter::sharedNotificationCenter()->postNotification(UInextPAUSEKEY, NULL);
		UserRecord::getInstance()->setIsFreshMan(true);
		SecondScene *freshman = SecondScene::create("200001");
		freshman->setPosition(Point::ZERO);
		freshman->setTag(99999);
		this->addChild(freshman,10);
	}else if ( UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()==3 ){
		NotificationCenter::sharedNotificationCenter()->postNotification(UInextPAUSEKEY, NULL);
		UserRecord::getInstance()->setIsFreshMan(true);
		ThirdScene *freshman = ThirdScene::create("300001");
		freshman->setPosition(Point::ZERO);
		freshman->setTag(99999);
		this->addChild(freshman,10);
	}
}

void GameScene::logicUpdate(float dt)
{
}