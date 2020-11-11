#include "FreshManManager.h"
#include "PlistManager.h"
#include "UserRecord.h"
#include "WindowManager.h"
#include "UIUpgrades.h"
#include "Ui_SceneSelect.h"
#include "magicObjects.h"
#include "Ui_SwallowAllTouchEvents.h"


FreshManManager::FreshManManager():
_widget(nullptr),
sp1(nullptr),
sp2(nullptr),
sp3(nullptr),
sp4(nullptr),
_touchedEnable(true),
_hasFinger(false),
_ispause(false),
_pauseFreshMan(false),
_direction("up"),
_rect(Rect::ZERO),
_content(""),
_contentTitle(""),
_finger(nullptr),
_currentID("")
{

}

FreshManManager::~FreshManManager()
{
	_currentID="";

	sp1 = nullptr;
	sp2 = nullptr;
	sp3 = nullptr;
	sp4 = nullptr;
}

FreshManManager* FreshManManager::create(const char* id)
{
	FreshManManager* pRet = new FreshManManager();
	if ( pRet && pRet->initWithID(id) )
	{
		pRet->initView(id);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void FreshManManager::onExit()
{
	_currentID="";
	Widget::onExit();
}

void FreshManManager::onEnter()
{
	Widget::onEnter();
}

bool FreshManManager::initWithID(const char* id)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point originPt   = Director::getInstance()->getVisibleOrigin();

	_widget = GUIReader::getInstance()->widgetFromJsonFile("Ui_FreshMan/Ui_FreshMan.ExportJson");

	Widget *panel_titile   = _widget->getChildByName("Panel_title");
	ImageView *imagePhoto  = static_cast<ImageView*>(panel_titile->getChildByName("Image_photo")); 
	Button    *btnNext     = static_cast<Button*>(panel_titile->getChildByName("Button_Next"));
	btnNext        -> addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));

	_widget->setTouchEnabled(false);
	this->addChild(_widget);

	sp1 = static_cast<ImageView*>(_widget->getChildByName("mask1"));
	sp2 = static_cast<ImageView*>(_widget->getChildByName("mask2"));
	sp3 = static_cast<ImageView*>(_widget->getChildByName("mask3"));
	sp4 = static_cast<ImageView*>(_widget->getChildByName("mask4"));


	__Dictionary *dic = PlistManager::getInstance()->addPlistMap("FreshMan","plist/freshMan.plist");
	__Dictionary *currData = static_cast<__Dictionary*>(dic->objectForKey(id));

	_touchedEnable = strcmp(static_cast<__String*>(currData->objectForKey("touchEnable"))->getCString(), "true")==0? true:false; 
	_hasFinger     = static_cast<__String*>(currData->objectForKey("isFinger"))->intValue() ==0? false:true;
	_direction	   = static_cast<__String*>(currData->objectForKey("derection"))->getCString();
	auto originX   = static_cast<__String*>(currData->objectForKey("originX"))->floatValue();
	auto originY   = static_cast<__String*>(currData->objectForKey("originY"))->floatValue();
	auto width     = static_cast<__String*>(currData->objectForKey("width"))->floatValue();
	auto height    = static_cast<__String*>(currData->objectForKey("height"))->floatValue();


	_rect          = Rect(originX,originY,width,height);  
	_content       = static_cast<__String*>(currData->objectForKey("content"))->getCString();
	_contentTitle  = static_cast<__String*>(currData->objectForKey("contentTitle"))->getCString();
	_ispause       = strcmp(static_cast<__String*>(currData->objectForKey("ispause"))->getCString(),"true")==0? true:false;
	
	if (strcmp(static_cast<__String*>(currData->objectForKey("photo"))->getCString(),"0")==0){
		panel_titile   ->setVisible(false);
		UserRecord::getInstance()->setTitlePos(0);
	}else{
		imagePhoto     ->loadTexture(StringUtils::format("freshman/NPC_%s.png",static_cast<__String*>(currData->objectForKey("photo"))->getCString()),UI_TEX_TYPE_LOCAL);
	}
	_currentID     = id;
	UserRecord::getInstance()->setFreshManCurrentID(atoi(id));

	setHasFinger(_hasFinger);
	setContent(_content.c_str());
	setContentTitle(_contentTitle.c_str());
/*	setPenetrateZone(_rect);*/

	fitScreen();

	_time = 0;
	return true;
}

void FreshManManager::setMainLayerBtnGrey()
{
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));

	if (!mainLayer->m_AddMagicBtn->_isgrey)
		mainLayer->m_AddMagicBtn->setGrey();

	if (!mainLayer->m_PauseBtn->_isgrey)
		mainLayer->m_PauseBtn->setGrey();

	if (!mainLayer->m_ShopBtn->_isgrey)
		mainLayer->m_ShopBtn->setGrey();

 	if (!mainLayer->HeroBtn->_isgrey)
 		mainLayer->HeroBtn->setGrey();

	Button *btn;
	for ( auto iter=mainLayer->m_itemInfo.begin();iter!=mainLayer->m_itemInfo.end();++iter )
	{
		btn  = static_cast<Button*>(mainLayer->currPanel->getChildByName(String::createWithFormat("item_index_btn_%d",iter->first)->getCString()));
		if (!btn->_isgrey){
			btn -> setGrey();
		}
	}

	if (!mainLayer->m_MagicBtn_1->_isgrey)
		mainLayer->m_MagicBtn_1->setGrey();

	if (!mainLayer->m_MagicBtn_2->_isgrey)
		mainLayer->m_MagicBtn_2->setGrey();

	if (!mainLayer->m_MagicBtn_3->_isgrey)
		mainLayer->m_MagicBtn_3->setGrey();

	if (!mainLayer->m_MagicBtn_4->_isgrey)
		mainLayer->m_MagicBtn_4->setGrey();
}

void FreshManManager::setHasFinger(bool bvar)
{
	if (bvar){
		setFingerDirection(_direction);

		scheduleOnce(schedule_selector(FreshManManager::addFingerCallfunc),0.3f);
	}

	//有手指限制点击操作则加上界面锁
	UserRecord::getInstance()->setFreshManMutex(bvar);	
}

void FreshManManager::addFingerCallfunc(float dt)
{
	//添加手指动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("freshman/zhujiemiananniu0.png","freshman/zhujiemiananniu0.plist","freshman/zhujiemiananniu.ExportJson");
	_finger = Armature::create("zhujiemiananniu");
	this->addChild(_finger);


	//调整手指位置
	Point originPoint = Director::getInstance()->getVisibleOrigin();
	Widget *panel_titile = _widget->getChildByName("Panel_title");
	Point benganPos   = Point(panel_titile->getPositionX()+panel_titile->getSize().width/2 , panel_titile->getPositionY()+panel_titile->getSize().height/2);
	Point endPos = Point(_rect.origin.x + _rect.size.width/2 + _finger->getContentSize().width - 85 + originPoint.x
					   , _rect.origin.y + _rect.size.height/2 - _finger->getContentSize().height + 20 + originPoint.y);

	Rect rect = Rect(originPoint.x,originPoint.y,Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height);
	if (rect.containsPoint(Point(_rect.origin.x + _rect.size.width/2 + originPoint.x ,_rect.origin.y + _rect.size.height/2 + originPoint.y))==false){
		_finger->setVisible(false);
	}
	//_finger->setPosition(endPos);
	//_finger->getAnimation()->play("Animation1");
	_finger->setPosition(benganPos);
	_finger->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.8f,endPos),CallFunc::create([=](){
																													_finger->getAnimation()->play("Animation1");
																												}),
																  DelayTime::create(4.0f),CallFunc::create([=](){
																													_finger->setPosition(benganPos);
																													_finger->getAnimation()->gotoAndPause(1);
																												}),NULL)));


}

void FreshManManager::setFingerDirection(string direct)
{
	if (strcmp(direct.c_str(),"up")==0){

	}else if (strcmp(direct.c_str(),"down")==0){

	}else if (strcmp(direct.c_str(),"right")==0){

	}else if (strcmp(direct.c_str(),"left")==0){

	}
}

void FreshManManager::setTitlePos(int type)
{
	Size visibleSize  = Director::getInstance()->getVisibleSize();

	Widget *panel_title = _widget->getChildByName("Panel_title");
	panel_title->stopAllActions();
	if ( type==1 ) {
		if (UserRecord::getInstance()->getTitlePos()!=1 && panel_title->isVisible()){
			UserRecord::getInstance()->setTitlePos(1);
			panel_title->setPosition(Point(0,-240));
			panel_title->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.3f,Point(0,20))),MoveTo::create(0.2f,Point(0,0)),NULL));
		}else{
			panel_title->setPosition(Point::ZERO);
		}	
	}else{
		if (UserRecord::getInstance()->getTitlePos()!=2 && panel_title->isVisible()){
			UserRecord::getInstance()->setTitlePos(2);
			panel_title->setPosition(Point(0 , visibleSize.height-panel_title->getSize().height+300 ));
			panel_title->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(0.3f,Point(0,-320))),MoveBy::create(0.2f,Point(0,20)),NULL));
		}else{
			panel_title->setPosition(Point(0 , visibleSize.height-panel_title->getSize().height));
		}	
	}
}

void FreshManManager::setContent(const char* text)
{
	Widget *panel_title = _widget->getChildByName("Panel_title");
	TextBMFont   *content       = static_cast<TextBMFont*>(Helper::seekWidgetByName(panel_title,"Label_content"));

	string str = text;
	while ( str.find_first_of("_n")!=string::npos )
	{
		size_t pos1 = str.find_first_of("_n"); 
		str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
	}

	content->setText(str.c_str());
}

void FreshManManager::setContentTitle(const char* title)
{
	Widget *panel_title   = _widget->getChildByName("Panel_title");
	TextBMFont   *contentTitle  = static_cast<TextBMFont*>(Helper::seekWidgetByName(panel_title,"Label_name"));
	contentTitle->setText(title);
}

void FreshManManager::setPenetrateZone(Rect rect)
{

	sp1->setTouchEnabled(true);
	sp2->setTouchEnabled(true);
	sp3->setTouchEnabled(true);
	sp4->setTouchEnabled(true);
	sp1->setOpacity(0);
	sp2->setOpacity(0);
	sp3->setOpacity(0);
	sp4->setOpacity(0);
	sp1->setTouchPriority(TOUCH_UI_PRIORITY*3);
	sp2->setTouchPriority(TOUCH_UI_PRIORITY*3);
	sp3->setTouchPriority(TOUCH_UI_PRIORITY*3);
	sp4->setTouchPriority(TOUCH_UI_PRIORITY*3);
	sp1->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));
	sp2->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));
	sp3->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));
	sp4->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	sp1->setSize(Size(rect.origin.x,visibleSize.height-rect.origin.y));
	sp2->setSize(Size(rect.origin.x+rect.size.width,rect.origin.y));
	sp3->setSize(Size(visibleSize.width - rect.origin.x,visibleSize.height - rect.origin.y - rect.size.height));
	sp4->setSize(Size(visibleSize.width - rect.origin.x - rect.size.width, rect.origin.y + rect.size.height));

	if (sp1->getSize().width ==0 || sp1->getSize().height==0){
		sp1->setVisible(false);
	}else{
		sp1->setVisible(true);
	}

	if (sp2->getSize().width ==0 || sp2->getSize().height==0){
		sp2->setVisible(false);
	}else{
		sp2->setVisible(true);
	}

	if (sp3->getSize().width ==0 || sp3->getSize().height==0){
		sp3->setVisible(false);
	}else{
		sp3->setVisible(true);
	}

	if (sp4->getSize().width ==0 || sp4->getSize().height==0){
		sp4->setVisible(false);
	}else{
		sp4->setVisible(true);
	}

}

void FreshManManager::spTouchEventCallback(Ref *pSender, TouchEventType type)
{
	if (type==TOUCH_EVENT_ENDED && !_hasFinger){
		sp1->setTouchEnabled(false);
		sp2->setTouchEnabled(false);
		sp3->setTouchEnabled(false);
		sp4->setTouchEnabled(false);

		nextFreshMan(_currentID.c_str());
	}
}

void FreshManManager::update(float dt)
{
	_time += dt;
	if (_hasFinger && _finger && !UserRecord::getInstance()->getFreshManMutex()){
		nextFreshMan(_currentID.c_str());
	}

	if (UserRecord::getInstance()->getFreshManCurrentID()!=0){
		int current_id = UserRecord::getInstance()->getFreshManCurrentID();
        //移动英雄教程，当英雄移动到箭头位置时，跳到下一教学
		if (current_id==200006 || current_id==200007){
			StoreyData *topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
			GameObjects *hero     = topStorey->getHero();

			Widget* thisCreateRectCtrl2 = topStorey->mItem->getChildByName("Image_CreateView2");
			Widget* thisCreateRectCtrl4 = topStorey->mItem->getChildByName("Image_CreateView3");
			float leftPosX   = thisCreateRectCtrl2->getPositionX();
			float rightPosX  = thisCreateRectCtrl4->getPositionX();

			if (current_id==200006){
				if (hero->getPositionX()<=leftPosX)
					UserRecord::getInstance()->setFreshManMutex(false);
			}
			if (current_id==200007){
				if (hero->getPositionX()>=rightPosX)
					UserRecord::getInstance()->setFreshManMutex(false);
			}
		}
	}

	if ( (
		  UserRecord::getInstance()->getFreshManCurrentID() == 100013 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 100016 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 200010 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 300003 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 300010 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 300019 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 300020 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 300021 ||
		  UserRecord::getInstance()->getFreshManCurrentID() == 300022
		)&& !_pauseFreshMan){ //判断士兵的位置
		if (UserRecord::getInstance()->getFreshManCurrentID() == 300019){
			if (_time<12){
				do 
				{
					CC_BREAK_IF(!sp1);
					sp1->removeTouchEventListener();
					CC_BREAK_IF(!sp2);
					sp2->removeTouchEventListener();
					CC_BREAK_IF(!sp3);
					sp3->removeTouchEventListener();
					CC_BREAK_IF(!sp4);
					sp4->removeTouchEventListener();

					Widget *panel_title = _widget->getChildByName("Panel_title");
					CC_BREAK_IF(!panel_title);
					if (panel_title->isVisible()){
						panel_title->setVisible(false);
						UserRecord::getInstance()->setTitlePos(0);
					}
				} while (0);
				
				return;
			}else{
				do 
				{
					CC_BREAK_IF(!sp1);
					sp1->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));
					CC_BREAK_IF(!sp2);
					sp2->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));
					CC_BREAK_IF(!sp3);
					sp3->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));
					CC_BREAK_IF(!sp4);
					sp4->addTouchEventListener(this,toucheventselector(FreshManManager::spTouchEventCallback));

					Widget *panel_title = _widget->getChildByName("Panel_title");
					CC_BREAK_IF(!panel_title);
					if (!panel_title->isVisible()){
						panel_title->setPositionY(panel_title->getPositionY()+300);
						panel_title->setVisible(true);
						setTitlePos(2);
					}
				} while (0);
			}
		}

		StoreyData *topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
		if (UserRecord::getInstance()->getFreshManCurrentID() == 100016){
			topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP-1);
		}
		if (topStorey==nullptr){
			return; 
		}

		Widget* thisCreateRectCtrl1 = topStorey->mItem->getChildByName("Image_CreateView1");
		Widget* thisCreateRectCtrl4 = topStorey->mItem->getChildByName("Image_CreateView4");
		
		float leftPosX   = thisCreateRectCtrl1->getPositionX();
		float rightPosX  = thisCreateRectCtrl4->getPositionX();

		float targetX = leftPosX;
		if (UserRecord::getInstance()->getFreshManCurrentID() == 100013){
			targetX = rightPosX;
		}
		 
		for (GameObjects *sp:topStorey->mObjectData){
			if (sp){
				if (sp->getobjectType() == TYPE_ENEMY && sp->getPositionX()>=targetX ){
					//暂停游戏
					_pauseFreshMan = true;
					NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY,  (Ref*)0);
					if (   UserRecord::getInstance()->getFreshManCurrentID() != 300021 && UserRecord::getInstance()->getFreshManCurrentID() != 300022 
						&& UserRecord::getInstance()->getFreshManCurrentID() != 300020 && UserRecord::getInstance()->getFreshManCurrentID() != 300019
						&& UserRecord::getInstance()->getFreshManCurrentID() != 200010){
						UserRecord::getInstance()->setFreshManMutex(false);
					}	
					break;
				}
			}
		}
	}

	//屏蔽listView移动
	if (UserRecord::getInstance()->getFreshManCurrentID() == 200005){
		auto child = WindowManager::getInstance()->mGameLayer->getChildByTag(126);
		if (child){
			Ui_HeroSelect* heroSelect = static_cast<Ui_HeroSelect*>(child);
			ListView* _listView = heroSelect->mListViewHeroSelect;
			_listView->getInnerContainer()->setPositionX(0);
		}
	}
}

void FreshManManager::nextFreshMan(const char* currentID)
{

	string nextID = StringUtils::format("%d",atoi(currentID)+1);
	__Dictionary *dic = PlistManager::getInstance()->addPlistMap("FreshMan","plist/freshMan.plist");
	__Dictionary *currData = static_cast<__Dictionary*>(dic->objectForKey(nextID));

	this->unscheduleAllSelectors();
	auto parent = getParent();
	this->removeFromParentAndCleanup(true);
	
	if (currData && nextID.find("100022")==string::npos && nextID.find("200013")==string::npos){
		FreshManManager *child=nullptr;
		if (nextID.find("1000")!=string::npos){
			child = FirstScene::create(nextID.c_str());
		}else if (nextID.find("2000")!=string::npos){
			child = SecondScene::create(nextID.c_str());
		}else if (nextID.find("3000")!=string::npos){
			child = ThirdScene::create(nextID.c_str());
		}else if (nextID.find("4000")!=string::npos){
			child = HeroScene::create(nextID.c_str());
		}else if (nextID.find("5000")!=string::npos){
			child = UpgradeScene::create(nextID.c_str());
		}
		if (child){
			child->setTag(99999);
			parent->addChild(child,10);
		}
		//CCLOG("============================================nextID.c_str()=%s",nextID.c_str());
	}else{
		if (nextID.find("100022")!=string::npos || nextID.find("200013")!=string::npos || nextID.find("300027")!=string::npos){
			WindowManager::getInstance()->createGameOverFrame(true,0);
		}
		if (nextID.find("400022")!=string::npos){
			//关闭英雄界面
			if (parent->getChildByTag(1836)){
				Ui_SceneSelect *sceneSelect = static_cast<Ui_SceneSelect*>(parent->getChildByTag(1836));
				if (sceneSelect->getChildByTag(1818)){
					Ui_HeroAltar   *heroAltar   = static_cast<Ui_HeroAltar*>(sceneSelect->getChildByTag(1818));
					heroAltar->removeFromParentAndCleanup(true);

					NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
				}
			}
		}
		if (nextID.find("500013")!=string::npos){
			if (parent->getChildByTag(1835)){
				levelScene *levelsc = static_cast<levelScene*>(parent);
				levelsc->upgradesUiMoveOut();
				levelsc->selectUiComeIn();
			}
		}

		UserRecord::getInstance()->setTitlePos(0);
	}
}

void FreshManManager::fitScreen()
{
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_widget->setPosition(originPt);

	sp1->setPosition(Point(0,visibleSize.height));
	sp2->setPosition(Point(0,0));
	sp3->setPosition(Point(visibleSize.width,visibleSize.height));
	sp4->setPosition(Point(visibleSize.width,0));
}

void FreshManManager::initView(const char* id)
{

}



//firstScene
FirstScene* FirstScene::create(const char* id)
{
	FirstScene* pRet = new FirstScene();
	if ( pRet && pRet->initWithID(id) )
	{
		pRet->initView(id);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void FirstScene::initView(const char* id)
{
	Ui_MainBack *mainback = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
	mainback->mScrollViewWidget->jumpToBottom();

	if (atoi(id)==100001){
		UserRecord::getInstance()->setSweet(1200);
	}

	Widget *homeStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->mItem;
	if (atoi(id)>=100012 && atoi(id)<=100021){
		mainback->mScrollViewWidget->getInnerContainer()->setPositionY(homeStorey->getSize().height*(-1)+30);
		if (atoi(id)<=100015){
			mainback->mScrollViewWidget->getInnerContainer()->setPositionY(homeStorey->getSize().height*(-1)+50);
		}
	}
	mainback->freshmanArrowPrompt(id);

	setMainLayerBtnGrey();
	fitZone(id);
	scheduleUpdate();
}

void FirstScene::fitZone(const char* id)
{
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));

	Size visibleSize  = Director::getInstance()->getVisibleSize();
	Point originPoint = Director::getInstance()->getVisibleOrigin();
	float originX = _rect.origin.x;
	float originY = _rect.origin.y;
	
	if (strcmp(id,"100008")==0 || strcmp(id,"100009")==0 ){
			Widget *homeStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->mItem;
			Widget *warp       = homeStorey->getChildByName("sweet_warp");

			if (strcmp(id,"100008")==0){
				originX  = warp->getWorldPosition().x - originPoint.x;
// 				Layout *lay = static_cast<Layout*>(warp);
// 				lay->setBackGroundColorType(LAYOUT_COLOR_SOLID);
// 				lay->setBackGroundColor(Color3B::GREEN);
				_rect.size.width = warp->getSize().width*homeStorey->getScale();
			}
			if (strcmp(id,"100009")==0){
				originX = warp->getWorldPosition().x  - originPoint.x + 55*homeStorey->getScale() ;
				_rect.size.width *= homeStorey->getScale();
			}
			originY       -= homeStorey->getSize().height*(1-homeStorey->getScale());

			_rect.origin.x = originX;
			_rect.origin.y = originY;
	}else if ( strcmp(id,"100015")==0 ){
		Widget *homeStorey   = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->mItem;
		Button *addStoreyBtn = static_cast<Button*>(homeStorey->getChildByName("Button_AddStorey")); 

		originX = addStoreyBtn->getWorldPosition().x - addStoreyBtn->getSize().width/2 - originPoint.x;
		originY = addStoreyBtn->getWorldPosition().y - addStoreyBtn->getSize().height/2 - originPoint.y;

		_rect.origin.x    = originX;
		_rect.origin.y    = originY;
		_rect.size.width  = addStoreyBtn->getSize().width;
		_rect.size.height = addStoreyBtn->getSize().height; 
	}
	
	if (atoi(id)==100003){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(210+originPoint.x,0+originPoint.y);
		sprite ->setPosition(pos);
		sprite->setContentSize(Size(540,120));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL)));
	}else if (atoi(id)==100004){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(visibleSize.width - 275 + originPoint.x,visibleSize.height - 60 + originPoint.y);
		sprite ->setPosition(pos);
		sprite->setContentSize(Size(85,60));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL)));
	}else if (atoi(id)==100005){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(visibleSize.width - 195 + originPoint.x,visibleSize.height - 60 + originPoint.y);
		sprite ->setPosition(pos);
		sprite->setContentSize(Size(80,60));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL)));
	}else if (atoi(id)==100006){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(visibleSize.width - 125 + originPoint.x,visibleSize.height - 60 + originPoint.y);
		sprite ->setPosition(pos);
		sprite->setContentSize(Size(120,60));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL)));
	}else if (atoi(id)==100008 || atoi(id)==100009 || atoi(id)==100015 ){
		float _a = 0;
		float _b = 0;  
		if (atoi(id)==100008 || atoi(id)==100009){
			_a = -_rect.size.width/2;
			_b = 20;
		}else if ( atoi(id)==100015 ){
			_a = 60;
			_b = -_rect.size.height/2;
		}

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPoint.x + _a , _rect.origin.y+_rect.size.height + originPoint.y + _b));
		addChild(sp,10);
	}else if ( atoi(id)==100012 || atoi(id)==100017 ){
		Sprite *sp = Sprite::create("freshman/ZI_023.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height-20 + originPoint.y));
		addChild(sp,10);

		if ( atoi(id)==100012 ){
			Button* btn  = static_cast<Button*>(mainLayer->currPanel->getChildByName("item_index_btn_3"));
			btn -> removeGrey();
		}else if ( atoi(id)==100017 ){
			Button* btn  = static_cast<Button*>(mainLayer->currPanel->getChildByName("item_index_btn_1"));
			btn -> removeGrey();
		}
	}else if ( atoi(id)==100018 ){
		StoreyData* storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(4);
		if (storey){
			for (GameObjects *obj:storey->mObjectData){
				if (obj && obj->getobjectType()==TYPE_WARRIOR ){
					warriorObjects *warObj = static_cast<warriorObjects*>(obj);
					warObj->load();
					warObj->unschedule(schedule_selector(warriorObjects::warriorupdate));
				}
			}
		}
		Widget* thisCreateRectCtrl2 = storey->mItem->getChildByName("Image_CreateView2");
		originX = thisCreateRectCtrl2->getWorldPosition().x - originPoint.x - thisCreateRectCtrl2->getSize().width/2;
		originY = thisCreateRectCtrl2->getWorldPosition().y - originPoint.y - thisCreateRectCtrl2->getSize().height/2;
		_rect.size.width  = thisCreateRectCtrl2->getSize().width;
		_rect.size.height = thisCreateRectCtrl2->getSize().height/3;

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(originX+_rect.size.width/2 +originPoint.x, originY+_rect.size.height + 30 + originPoint.y));
		addChild(sp,10);
		setIsTouchScene(false);
	}else if ( atoi(id)==100019 ){
		StoreyData* storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(4);
		Widget* thisCreateRectCtrl2 = storey->mItem->getChildByName("Image_CreateView2");
		originX = thisCreateRectCtrl2->getWorldPosition().x - originPoint.x;
		originY = thisCreateRectCtrl2->getWorldPosition().y - originPoint.y - thisCreateRectCtrl2->getSize().height/2 + thisCreateRectCtrl2->getSize().height/3 + 18;
		_rect.size.width  = 70;
		_rect.size.height = 45;

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(originX+_rect.size.width/2 +originPoint.x, originY+_rect.size.height+30 + originPoint.y-12));
		addChild(sp,10);
		setIsTouchScene(false);
	}else if ( atoi(id)==100020 ){
		StoreyData* storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(4);
		if (storey){
			for (GameObjects *obj:storey->mObjectData){
				if (obj && obj->getobjectType()==TYPE_WARRIOR ){
					warriorObjects *warObj = static_cast<warriorObjects*>(obj);
					warObj->setatk(150);
					warObj->schedule(schedule_selector(warriorObjects::warriorupdate),1.0/60);
				}
			}
		}
		NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
	}
	//setIsTouchScene(true);
	_rect.origin.x = originX;
	_rect.origin.y = originY;
	setPenetrateZone(_rect);

	if ( (_rect.origin.y>visibleSize.height/3 || !_hasFinger) && atoi(id)!=100003 &&  atoi(id)!=100014 &&  atoi(id)!=100015  &&  atoi(id)!=100019 &&  atoi(id)!=100020 &&  atoi(id)!=100021)
		setTitlePos(1);
	else
		setTitlePos(2);
}


//SecondScene
SecondScene* SecondScene::create(const char* id)
{
	SecondScene* pRet = new SecondScene();
	if ( pRet && pRet->initWithID(id) )
	{
		pRet->initView(id);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void SecondScene::initView(const char* id)
{
	Ui_MainBack *mainback = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
	if (atoi(id)<200006)
		mainback->mScrollViewWidget->jumpToBottom();

	mainback->freshmanArrowPrompt(id);

	setMainLayerBtnGrey();
	fitZone(id);
	scheduleUpdate();
}

void SecondScene::fitZone(const char* id)
{
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point originPt   = Director::getInstance()->getVisibleOrigin();
	Widget *topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP)->mItem;
	if (atoi(id)==200002){
		Widget *heroBtn = topStorey->getChildByName("Button_HeroBoss");
		Point  heroBtnPos = heroBtn->getWorldPosition();
		Size   sz     = Size(heroBtn->getSize().width,heroBtn->getSize().height);
		Point  origin = Point(heroBtnPos.x - heroBtn->getSize().width/2 - originPt.x , heroBtnPos.y - heroBtn->getSize().height/2 - originPt.y);
		
		_rect = Rect(origin.x,origin.y,sz.width,sz.height);

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width  + originPt.x + 70, _rect.origin.y+_rect.size.height-30 + originPt.y));
		addChild(sp,10);
	}else if ( atoi(id)==200004 ){
		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPt.x , _rect.origin.y+_rect.size.height + originPt.y));
		addChild(sp,10);
	}else if ( atoi(id)==200005 ){
		NotificationCenter::getInstance()->postNotification(UInextPAUSEKEY, NULL);

		GameObjects *hero = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP)->getHero();
		Point heroPos     = hero->convertToWorldSpace(hero->getPosition());
		Size  sz          = hero->getArmature()->getContentSize();
		Point origin      = Point(hero->getPositionX() - sz.width/2 , heroPos.y);

		_rect = Rect(origin.x,origin.y,sz.width,sz.height);

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPt.x , _rect.origin.y+_rect.size.height + originPt.y));
		addChild(sp,10);

	}else if (atoi(id)==200006){
		Button *leftBtn   = static_cast<Button*>(topStorey->getChildByName("Button_MoveLeft"));
		ImageView *leftIcon = static_cast<ImageView*>(leftBtn->getChildByName("leftBtn_icon"));
		Point leftBtnPos  = leftIcon->getWorldPosition();
		leftBtnPos.y      = visibleSize.height/2;
		Size  sz          = leftIcon->getSize();

		Point origin = Point(leftBtnPos.x - sz.width/2 - originPt.x,leftBtnPos.y - sz.height/2);
		_rect = Rect(origin.x,origin.y,sz.width,sz.height);

		Sprite *sp = Sprite::create("freshman/ZI_023.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPt.x , _rect.origin.y+_rect.size.height-70 + originPt.y));
		addChild(sp,10);
	}else if (atoi(id)==200007){
		Button *rightBtn     = static_cast<Button*>(topStorey->getChildByName("Button_MoveRight"));
		ImageView *rightIcon = static_cast<ImageView*>(rightBtn->getChildByName("rightBtn_icon"));
		Point rightBtnPos  = rightIcon->getWorldPosition();
		rightBtnPos.y     = visibleSize.height/2; 
		Size  sz          = rightIcon->getSize();

		Point origin      =  Point(rightBtnPos.x  - sz.width/2  - originPt.x , rightBtnPos.y - sz.height/2 - originPt.y);
		_rect = Rect(origin.x,origin.y,sz.width,sz.height);

		Sprite *sp = Sprite::create("freshman/ZI_023.png");
		sp->setPosition(Point( _rect.origin.x+_rect.size.width/2 + originPt.x , _rect.origin.y+_rect.size.height-70 + originPt.y ));
		addChild(sp,10);

	}else if (atoi(id)==200009){ //空白层，进入敌人
		StoreyData *topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
		GameObjects *hero     = topStorey->getHero();

		Widget* thisCreateRectCtrl = topStorey->mItem->getChildByName("Image_CreateView3");

		float posx   = thisCreateRectCtrl->getPositionX();
		hero->setPositionX(posx);

		NotificationCenter::getInstance()->postNotification(FORCESTART, NULL);
	}else if (atoi(id)==200010){ //进入敌人

		NotificationCenter::getInstance()->postNotification(FORCESTART, NULL);

		_rect = Rect(10  , visibleSize.height - 100 ,110,100);
		mainLayer->HeroBtn->removeGrey();

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPt.x +60 , _rect.origin.y+_rect.size.height-50 + originPt.y));
		addChild(sp,10);
	}else if (atoi(id)==200011){ //解除暂停 
		NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
// 
// 		Widget *topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP)->mItem;
// 		Button *leftBtn   = static_cast<Button*>(topStorey->getChildByName("Button_MoveLeft"));
// 		Button *rightBtn  = static_cast<Button*>(topStorey->getChildByName("Button_MoveRight"));
// 		leftBtn ->setTouchPriority(TOUCH_UI_PRIORITY*4);
// 		rightBtn->setTouchPriority(TOUCH_UI_PRIORITY*4);
// 
// 		Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
// 		Widget *heroPhotoBtn  = mainLayer->mMainWidget->getChildByName("Button_Hero");
// 		Point heroPhotoPos    = heroPhotoBtn->getWorldPosition();
// 		Size sz               = heroPhotoBtn->getSize();
// 		Point origin          = Point(heroPhotoPos.x - sz.width/2 - originPt.x , heroPhotoPos.y - sz.height/2 - originPt.y);
// 
// 		_rect = Rect(origin.x,origin.y,sz.width,sz.height);

	}
	
	setPenetrateZone(_rect);

	if (atoi(id)==200003){
		setTitlePos(2);
	}else{
		if (_rect.origin.y>visibleSize.height/3 || !_hasFinger)
			setTitlePos(1);
		else
			setTitlePos(2);
	}
}

//ThirdScene
ThirdScene* ThirdScene::create(const char* id)
{
	ThirdScene* pRet = new ThirdScene();
	if ( pRet && pRet->initWithID(id) )
	{
		pRet->initView(id);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void ThirdScene::initView(const char* id)
{
	Ui_MainBack *mainback = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
	Widget *homeStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->mItem;
	mainback->mScrollViewWidget->getInnerContainer()->setPositionY(homeStorey->getSize().height*(-1)+30);

	if (atoi(id)==300005){
		mainback->mScrollViewWidget->getInnerContainer()->setPositionY(homeStorey->getSize().height*(-1)+100);
	}

	setMainLayerBtnGrey();
	fitZone(id);
	scheduleUpdate();
}

void ThirdScene::fitZone(const char* id)
{
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point originPt   = Director::getInstance()->getVisibleOrigin();
	

	Button *heroCreateBtn = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP)->mButtonCreateHero;

	if ( atoi(id)==300003 || atoi(id)==300010 ){ //进入敌人
		NotificationCenter::sharedNotificationCenter()->postNotification(FORCESTART, NULL);
	}else if ( atoi(id)==300002 ){
		mainLayer->m_AddMagicBtn->removeGrey();

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPt.x +10 , _rect.origin.y+_rect.size.height + originPt.y));
		addChild(sp,10);
	
	}else if ( atoi(id)==300005 ){
		mainLayer->m_MagicBtn_1->removeGrey();

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPt.x +30 , _rect.origin.y+_rect.size.height-50 + originPt.y));
		addChild(sp,10);
		setIsTouchScene(false);
	}else if ( atoi(id)==300008 ){
		Widget *topStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP)->mItem;
		if (topStorey->getChildByTag(88888)){
				magicObjects *_obj=static_cast<magicObjects*>(topStorey->getChildByTag(88888));
				if (_obj && _obj->getobjectType()==TYPE_MAGIC && _obj->getSelfType()==MAGIC_ATK){
					_obj->delayKeepHereCallback(NULL);
				}
				SimpleAudioEngine::getInstance()->stopAllEffects();
		}
		setIsTouchScene(false);
		NotificationCenter::sharedNotificationCenter()->postNotification(UInextPAUSEKEY, NULL);
	}else if ( atoi(id)==300012 ){
		mainLayer->m_MagicBtn_2->removeGrey();

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPt.x +30, _rect.origin.y+_rect.size.height-50 + originPt.y));
		addChild(sp,10);
		setIsTouchScene(false);
	}else if ( atoi(id)==300017 ){
		mainLayer->m_MagicBtn_3->removeGrey();

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPt.x +30 , _rect.origin.y+_rect.size.height-50 + originPt.y));
		addChild(sp,10);
		setIsTouchScene(false);
	}else if (atoi(id)==300019){  //还原变羊效果
// 		StoreyData *topStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
// 		for (GameObjects* sp:topStoreyData->mObjectData){
// 			if (sp && sp->getobjectType()==TYPE_ENEMY){
// 				enemyObjects *enemy = static_cast<enemyObjects*>(sp);
// 				enemy->backEnemyEffect();
// 			}
// 		}
	}else if ( atoi(id)==300021 ){
		mainLayer->m_MagicBtn_4->removeGrey();

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width + originPt.x +30 , _rect.origin.y+_rect.size.height-50 + originPt.y));
		addChild(sp,10);
		setIsTouchScene(false);
	}else if ( atoi(id)==300024 ){
		StoreyData *topStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
		for (GameObjects *obj:topStoreyData->mObjectData){
			if (obj && obj->getobjectType() == TYPE_EVENT106){
				//Point pos = obj->convertToWorldSpace(Point(obj->getAnchorPoint().x * obj->getContentSize().width, obj->getAnchorPoint().y * obj->getContentSize().height));
				Point pos = obj->convertToWorldSpace(Point(0,0));
				_rect.origin.x    = pos.x - 50;
				_rect.origin.y    = pos.y;
				_rect.size.width  = obj->getContentSize().width;
				_rect.size.height = obj->getContentSize().height;

				Sprite *sp = Sprite::create("freshman/ZI_021.png");
				sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 +originPt.x, _rect.origin.y+_rect.size.height+30 + originPt.y));
				addChild(sp,10);

				break;
			}
		}
		setIsTouchScene(false);
	}else if ( atoi(id)==300006 || atoi(id)==300013  || atoi(id)==300018 || atoi(id)==300022 ){
		heroCreateBtn->setTouchEnabled(false);

		StoreyData *topStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
		Widget* thisCreateRectCtrl1 = topStoreyData->mItem->getChildByName("Image_CreateView1");
		Widget* thisCreateRectCtrl2 = topStoreyData->mItem->getChildByName("Image_CreateView2");

		float originX,originY,_width,_height;
		if (atoi(id)==300006 || atoi(id)==300013){
			originX = thisCreateRectCtrl1->getWorldPosition().x - thisCreateRectCtrl1->getSize().width/2 - originPt.x;
			originY = thisCreateRectCtrl1->getWorldPosition().y - thisCreateRectCtrl1->getSize().height/2 - originPt.y;
			_width  = thisCreateRectCtrl1->getSize().width;
			_height = thisCreateRectCtrl1->getSize().height;

		}else if ( atoi(id)==300018|| atoi(id)==300022){
			originX = thisCreateRectCtrl2->getWorldPosition().x - thisCreateRectCtrl1->getSize().width/2 - originPt.x - 50;
			originY = thisCreateRectCtrl2->getWorldPosition().y - thisCreateRectCtrl1->getSize().height/2 - originPt.y;
			_width  = thisCreateRectCtrl2->getSize().width;
			_height = thisCreateRectCtrl2->getSize().height;
		}
		_rect = Rect(originX,originY,_width,_height);

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(originX + _width/2 + originPt.x , originY + _height -55 + originPt.y));
		addChild(sp,10);
		setIsTouchScene(false);
	}
	//setIsTouchScene(true);
	setPenetrateZone(_rect);

	if ( (atoi(id)==300004 || atoi(id)>=300007) && atoi(id)!=300012 ){
		setTitlePos(2);
	}else{
		if (_rect.origin.y>visibleSize.height/3 || !_hasFinger)
			setTitlePos(1);
		else
			setTitlePos(2);
	}
}

//HeroScene
HeroScene* HeroScene::create(const char* id)
{
	HeroScene* pRet = new HeroScene();
	if ( pRet && pRet->initWithID(id) )
	{
		pRet->initView(id);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void HeroScene::initView(const char* id)
{
	fitZone(id);
	scheduleUpdate();
}

void HeroScene::fitZone(const char* id)
{
	Size visibleSize  = Director::getInstance()->getVisibleSize();
	Point originPoint = Director::getInstance()->getVisibleOrigin();
	float scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	Point pt    = Point(visibleSize.width/2  - 480*scale  + originPoint.x,visibleSize.height/2 - 320*scale + originPoint.y );
	if ( atoi(id)== 400003 ){
		_rect.origin.x -= originPoint.x;
		_rect.origin.y -= originPoint.y;

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x +3 , _rect.origin.y+_rect.size.height + originPoint.y +5 ));
		addChild(sp,10);
	}else if ( atoi(id) == 400005 ){
		Widget *bg = GUIReader::getInstance()->widgetFromJsonFile("Mask/Mask.ExportJson");
		bg->setScaleX(visibleSize.width/960);
		bg->setScaleY(visibleSize.height/640);
		bg->setPosition(originPoint);
		Layout *lay = static_cast<Layout*>(bg->getChildByName("Panel_1"));
		lay->setBackGroundColorOpacity(200);
		this->addChild(bg);

		Sprite *sp = Sprite::create("freshman/YXM.png");
		sp->setPosition(originPoint.x + visibleSize.width/2,originPoint.y + visibleSize.height/2);
		this->addChild(sp,10);

		Sprite *door = Sprite::create("freshman/C3_Hero.png");
		door->setPosition(Point(385,180));
		sp->addChild(door);

		ArmatureDataManager::getInstance()->addArmatureFileInfo("freshman/zhujiemiananniu0.png","freshman/zhujiemiananniu0.plist","freshman/zhujiemiananniu.ExportJson");
		Armature *finger = Armature::create("zhujiemiananniu");
		finger->getAnimation()->play("Animation1");
		finger->setPosition(Point(door->getPositionX(),door->getPositionY()-100));
		sp->addChild(finger,3);

		door->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8f,1.1f),ScaleTo::create(0.8f,1.0f),NULL)));
	}else if ( atoi(id) == 400008 ){
		_rect = Rect(_rect.origin.x * scale + pt.x - originPoint.x ,
					 _rect.origin.y * scale + pt.y - originPoint.y ,
					 _rect.size.width * scale,
					 _rect.size.height * scale
			        );

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}else if ( atoi(id) == 400011 ){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(410*scale + pt.x,75*scale + pt.y);
		sprite ->setPosition(pos);
		sprite ->setContentSize(Size(355*scale,120*scale));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 
	}else if ( atoi(id) == 400012 ){
		_rect = Rect(_rect.origin.x * scale + pt.x - originPoint.x ,
			         _rect.origin.y * scale + pt.y - originPoint.y ,
			         _rect.size.width * scale,
			         _rect.size.height * scale
			        );

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}else if ( atoi(id) == 400016 ){
		_rect = Rect(_rect.origin.x * scale + pt.x - originPoint.x ,
					 _rect.origin.y * scale + pt.y - originPoint.y ,
					 _rect.size.width * scale,
					 _rect.size.height * scale
			        );

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}else if ( atoi(id) == 400017 ){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(540*scale + pt.x,100*scale + pt.y);
		sprite ->setPosition(pos);
		sprite ->setContentSize(Size(245*scale,60*scale));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 
	}else if ( atoi(id) == 400018 ){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(412*scale + pt.x,140*scale + pt.y);
		sprite ->setPosition(pos);
		sprite ->setContentSize(Size(400*scale,90*scale));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 
	}else if ( atoi(id) == 400019 ){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(795*scale + pt.x,147*scale + pt.y);
		sprite ->setPosition(pos);
		sprite ->setContentSize(Size(120*scale,90*scale));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 
	}else if ( atoi(id) == 400020 ){
		_rect = Rect(_rect.origin.x * scale + pt.x - originPoint.x ,
			         _rect.origin.y * scale + pt.y - originPoint.y ,
					 _rect.size.width * scale,
					 _rect.size.height * scale
			        );

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}

	setPenetrateZone(_rect); 

	if ( atoi(id) == 400011 || atoi(id) == 400013 || atoi(id) == 400017 || atoi(id) == 400018 || atoi(id) == 400019 ){
		setTitlePos(2);
	}else{
		if (_rect.origin.y>visibleSize.height/3 || !_hasFinger)
			setTitlePos(1);
		else
			setTitlePos(2);
	}
}

//UpgradeScene
UpgradeScene* UpgradeScene::create(const char* id)
{
	UpgradeScene* pRet = new UpgradeScene();
	if ( pRet && pRet->initWithID(id) )
	{
		pRet->initView(id);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void UpgradeScene::initView(const char* id)
{
	fitZone(id);
	scheduleUpdate();
}

void UpgradeScene::fitZone(const char* id)
{
	Size visibleSize  = Director::getInstance()->getVisibleSize();
	Point originPoint = Director::getInstance()->getVisibleOrigin();
	float scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	Point pt    = Point(visibleSize.width/2  - 480*scale  + originPoint.x,visibleSize.height/2 - 320*scale + originPoint.y );

	if ( atoi(id)== 500004 ){
		_rect.origin.x -= originPoint.x;
		_rect.origin.y -= originPoint.y;

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x +3 , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}else if ( atoi(id) == 500005 ){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(760*scale+pt.x,480*scale+ pt.y);
		sprite ->setPosition(pos);
		sprite->setContentSize(Size(120*scale,60*scale));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 
	}else if ( atoi(id) == 500006 ){
// 		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/YXW_031.png");
// 		sprite->setAnchorPoint(Point::ZERO);
// 		this  ->addChild(sprite,20);
// 
// 		Point pos   = Point(675*scale+pt.x,120*scale+ pt.y);
// 		sprite ->setPosition(pos);
// 		sprite->setContentSize(Size(240*scale,80*scale));
// 
// 		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 

		_rect = Rect(_rect.origin.x * scale + pt.x - originPoint.x ,
					 _rect.origin.y * scale + pt.y - originPoint.y ,
			         _rect.size.width * scale,
			         _rect.size.height * scale
			        );

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}else if ( atoi(id) == 500007 ){
		_rect = Rect(_rect.origin.x * scale + pt.x - originPoint.x ,
					 _rect.origin.y * scale + pt.y - originPoint.y ,
					 _rect.size.width * scale,
					 _rect.size.height * scale
					);

		Sprite *sp = Sprite::create("freshman/ZI_021.png");
		sp->setPosition(Point(_rect.origin.x+_rect.size.width/2 + originPoint.x , _rect.origin.y+_rect.size.height + originPoint.y +10));
		addChild(sp,10);
	}else if ( atoi(id) == 500011 ){
		Scale9Sprite *sprite = Scale9Sprite::createWithSpriteFrameName("image/LFK.png");
		sprite->setAnchorPoint(Point::ZERO);
		this  ->addChild(sprite,20);

		Point pos   = Point(680*scale+pt.x,25*scale+ pt.y);
		sprite ->setPosition(pos);
		sprite->setContentSize(Size(240*scale,80*scale));

		sprite ->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f),FadeIn::create(0.3f),NULL))); 
	}

	setPenetrateZone(_rect);

	if ( atoi(id) >= 500008 ){
		setTitlePos(2);
	}else{
		if (_rect.origin.y>visibleSize.height/3 || !_hasFinger)
			setTitlePos(1);
		else
			setTitlePos(2);
	}
}


void FreshManManager::setIsTouchScene(bool _isTouch)
{
	Ui_MainBack *mainback = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
	if (mainback)
	{
		mainback->mScrollViewWidget->setTouchEnabled(_isTouch);
		mainback->mListViewWidget->setTouchEnabled(_isTouch);
		/*if (!_isTouch)
		{
		if (!mainback->getChildByTag(123))
		{
		Ui_SwallowAllTouchEvents* swallowUi = Ui_SwallowAllTouchEvents::create();
		swallowUi->setPosition(Director::getInstance()->getVisibleOrigin());
		swallowUi->setTag(123);
		swallowUi->setBackGroundColorOpacity(200);
		mainback->addChild(swallowUi, 10);
		}
		}else
		{
		if (mainback->getChildByTag(123))
		{
		mainback->removeChildByTag(123);
		}
		}*/
	}
}