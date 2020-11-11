#include "levelScene.h"
#include "GameScene.h"
#include "Ui_SceneSelect.h"
#include "UIUpgrades.h"
#include "Ui_LoginAward.h"
#include "FreshManManager.h"
#include "CachManager.h"
#include "LoadingScene.h"

#include <iostream>
#include <fstream>
#include <string>

Scene* levelScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	scene->setTag(2);

	// 'layer' is an autorelease objec*t
	levelScene *layer=levelScene::create();
	layer->setTag(101);
	// add layer as a child to scene
	scene->addChild(layer);


	// return the scene
	return scene;
}
levelScene::levelScene(void)
{
/* CCLOG("levelScene");*/
}

levelScene::~levelScene(void)
{
	/* CCLOG("~levelScene");*/
	
}



// on "init" you need to initialize your instance

bool levelScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	 CCLOG("level init");
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	winSize=Director::getInstance()->getWinSize();
	 this->setTag(2);

	 UserRecord::getInstance()->takeUnitsInit();
	 UserRecord::getInstance()->setIsFreshMan(false);

	Ui_SceneSelect *selectUi = Ui_SceneSelect::create();
	selectUi->setPosition(Point(0,0));
	this->addChild(selectUi);
	selectUi->setTag(1836);
	selectUi->setZOrder(5);

	auto Image_248 = Helper::seekWidgetByName(selectUi->_widget,"Image_248");
	Button* upGradeBtn = static_cast<Button*>(Image_248->getChildByTag(16183)); 
	Button* heroBtn = static_cast<Button*>(Image_248->getChildByTag(16185));
	upGradeBtn->setGrey();
	heroBtn   ->setGrey();

	Widget *btnsWidget = selectUi->_cWidget->getChildByName("Buttons");

//  移至ui_login，直接进入教学
// 	if ( UserRecord::getInstance()->getIndexGuide()<2 && UserRecord::getInstance()->getSectionNum() == 1 ){
// 		freshmanPrompt();
// 	}else{
		if (!UserRecord::getInstance()->getIsAward())
		{
			UserRecord::getInstance()->setIsAward(true);
			auto award  = Ui_LoginAward::create();
			this->addChild(award,16);
		}
/*	}*/


	if ( UserRecord::getInstance()->getIsFirstScene() && UserRecord::getInstance()->getSectionNum() == 1 ){
		//添加第一关手指
		Button* postBtn1 = static_cast<Button*>(btnsWidget->getChildByTag(19860));
		Point pos = postBtn1->getPosition();
		Size  sz  = postBtn1->getSize();

		Rect _rect = Rect(pos.x-sz.width/2,pos.y-sz.height/2,sz.width,sz.height);
		//addFinger(btnsWidget,_rect);

		Sprite *sp = Sprite::create("freshman/ZI_026.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width +80 , _rect.origin.y+_rect.size.height+30));
		btnsWidget->addChild(sp,10);

		UserRecord::getInstance()->setIsFirstScene(false);
// 		Button* nextBtn = static_cast<Button*>(selectUi->_widget->getChildByName("next"));
// 		nextBtn->setGrey();
// 		nextBtn->getChildByTag(1777)->setScaleX(-nextBtn->getChildByTag(1777)->getScaleX());

		
	}else if ( UserRecord::getInstance()->getIndexGuide()==3 && !UserRecord::getInstance()->getIsFourthScene() ){
		//添加第四关关手指
// 		Button* postBtn4 = static_cast<Button*>(btnsWidget->getChildByTag(19863));
// 		Point pos = postBtn4->getPosition();
// 		Size  sz  = postBtn4->getSize();
// 
// 		Rect _rect = Rect(pos.x-sz.width/2,pos.y-sz.height/2,sz.width,sz.height);
//      addFinger(btnsWidget,_rect);

// 		Sprite *sp = Sprite::create("freshman/ZI_026.png");
// 		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 , _rect.origin.y+_rect.size.height+30 ));
// 		btnsWidget->addChild(sp,10);

// 		Button* nextBtn = static_cast<Button*>(selectUi->_widget->getChildByName("next"));
// 		nextBtn->setGrey();
// 		nextBtn->getChildByTag(1777)->setScaleX(-nextBtn->getChildByTag(1777)->getScaleX());
	}

 	if ( UserRecord::getInstance()->getIndexPost()==4  && !UserRecord::getInstance()->getIsHeroGuide() ){
		selectUi->setTouchEnabled(false);

		UserRecord::getInstance()->setIsHeroGuide(true);
		UserRecord::getInstance()->flush();

		UserRecord::getInstance()->setIsFreshMan(true);
		HeroScene *freshman = HeroScene::create("400001");
		freshman->setPosition(Point::ZERO);
		this->addChild(freshman,10);
	}else if ( UserRecord::getInstance()->getIndexPost()==5 && !UserRecord::getInstance()->getIsGradeGuide()){
		selectUi->setTouchEnabled(false);

		UserRecord::getInstance()->setIsGradeGuide(true);
		UserRecord::getInstance()->flush();

		UserRecord::getInstance()->setIsFreshMan(true);
		UpgradeScene *freshman = UpgradeScene::create("500001");
		freshman->setPosition(Point::ZERO);
		this->addChild(freshman,10);
	}

	if (UserRecord::getInstance()->getIndexPost() < 4)
	{
		heroBtn->getChildByName("tips")->setVisible(false);
	}
	if (UserRecord::getInstance()->getIndexPost() < 5) {
		upGradeBtn->getChildByName("tips")->setVisible(false);
	}

	if (UserRecord::getInstance()->getIndexPost()>=4){  //解锁英雄
		heroBtn   ->removeGrey();
	}
	if (UserRecord::getInstance()->getIndexPost()>=5){  //解锁天赋
		upGradeBtn->removeGrey();
	}
	this->schedule(schedule_selector(levelScene::logicUpdate),1.0f);




	//对手机返回键的监听 
	auto listener = EventListenerKeyboard::create(); 
	//和回调函数绑定 
	listener->onKeyReleased = CC_CALLBACK_2(levelScene::onKeyReleased,this); 
	//添加到事件分发器中 
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this); 

	return true;
}
void levelScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		__Dictionary* text = __Dictionary::createWithContentsOfFile("plist/text.plist");
		__String* connect = static_cast<__String*>(text->objectForKey("1014"));
		__String* title = static_cast<__String*>(text->objectForKey("1015"));
		MessageBox(connect->getCString(),title->getCString());
	}
}
void levelScene::freshmanPrompt()
{
	Ui_Prompt *_prompt = Ui_Prompt::create();
	_prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	_prompt->setTag(251);
	auto Text = PlistManager::getInstance()->getPlistMap("Text");
	__String* TextStr = static_cast<__String*>(Text->objectForKey("1009"));
	_prompt->initWithTwoButtons(TextStr->getCString());
	_prompt->setRightSelector(this,comm_selector(levelScene::enterFreshMan));
	_prompt->setLeftSelector(this,comm_selector(levelScene::jumpFreshMan));
	this->addChild(_prompt,300);
}

void levelScene::enterFreshMan()
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
	Scene* pTScene =LoadingScene::createScene();
	LoadingScene*  cl = static_cast<LoadingScene*> (pTScene->getChildByTag(6));
	cl->loadScene(GAME_SCENE_TAG);
	TransitionFade *transitionfadeScene=TransitionFade::create(0.6f,pTScene);
	Director::getInstance()->replaceScene(transitionfadeScene);
}

void levelScene::jumpFreshMan()
{
	if (!UserRecord::getInstance()->getIsAward())
	{
		UserRecord::getInstance()->setIsAward(true);
		auto award  = Ui_LoginAward::create();
		this->addChild(award,16);
	}
	if (UserRecord::getInstance()->getIndexPost()<2){
		UserRecord::getInstance()->setIndexGuide(2);
		UserRecord::getInstance()->flush();
	}
}

void levelScene::selectUiMoveOut(){
	Widget*selectUi=static_cast<Widget*>(this->getChildByTag(1836));
	//selectUi->setPosition(Point(-10000,0));
}
void levelScene::selectUiComeIn(){
	Widget*selectUi=static_cast<Widget*>(this->getChildByTag(1836));
	selectUi->setTouchEnabled(true);
	//selectUi->setPosition(Point(0,0));
}
void levelScene::upgradesUiMoveOut(){
	Widget*upgradesUi=static_cast<Widget*>(this->getChildByTag(1835));
	upgradesUi->removeFromParentAndCleanup(true);
}
void levelScene::upgradesUiComeIn(){
	//新手引导
	if (UserRecord::getInstance()->getIsFreshMan()){
		UserRecord::getInstance()->setFreshManMutex(false);
	}

	UIUpgrades*upgradesUi=UIUpgrades::create();
	this->addChild(upgradesUi, 8);
	upgradesUi->setTag(1835);
}

void levelScene::addFinger(Widget* wg,Rect _rect)
{
	//添加手指动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("freshman/zhujiemiananniu0.png","freshman/zhujiemiananniu0.plist","freshman/zhujiemiananniu.ExportJson");
	Armature* _finger = Armature::create("zhujiemiananniu");
	wg->addChild(_finger,100);
	_finger->getAnimation()->play("Animation1");

	//调整手指位置
	Point originPoint = Director::getInstance()->getVisibleOrigin();
	if (_finger){
		_finger->setPosition(Point(_rect.origin.x + _rect.size.width/2 + _finger->getContentSize().width -85 
								 , _rect.origin.y + _rect.size.height/2 - _finger->getContentSize().height +20 ));
	}

}

void levelScene::logicUpdate(float dt)
{
	
}