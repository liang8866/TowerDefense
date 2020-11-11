#include "Ui_MainBack.h"
#include "PlistManager.h"
#include "Shake.h"
#include "magicObjects.h"
#include "Ui_SwallowAllTouchEvents.h"
#include "audioPlayer.h"
#include "EventObject_106.h"
#include "Ui_Debug.h"

// 单键
static WindowManager*	thisWindowManager = nullptr;
static SceneData*		thisSceneData = nullptr;
static const int addHeight = 100;

Ui_MainBack::Ui_MainBack(void)
{

}
Ui_MainBack::~Ui_MainBack(void)
{

}


Ui_MainBack* Ui_MainBack::create(const char* fileName)
{
	Ui_MainBack* _newWindow = new Ui_MainBack();
	if (nullptr == _newWindow || !_newWindow->init())
		return nullptr;

	if (_newWindow->initialise(fileName))
	{
		_newWindow->autorelease();
		return _newWindow;
	}
	CC_SAFE_DELETE(_newWindow);
	return nullptr;
}
// 初始化
bool Ui_MainBack::initialise(const char* fileName)
{
	thisWindowManager = WindowManager::getInstance();
	thisSceneData = SceneData::getInstance();
	// 加载json
	mMainWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fileName);
	if (nullptr == mMainWidget)
		return false;
	mJsonName = fileName;
	mWindowName = "mainBackWnd";
	mState = 1;
	mMainWidget->setLocalZOrder(1);

	mMouseDragBeginItemIndex = -1;
	mIsMouseDragMode = false;
	mIsMouseClickPosMoving = false;
	mMoveToRandStoreyIndex = -1000;
	mAddStoreyNum = 0;
	mAddYuJingStoreyIndex = -1;
	mAddStoreyCost = 0;
	mIsEvent105 = false;
	_createObjListener = nullptr;
	mMouseClickDragBeginPos = Point(0, 0);

	
	mMouseClickDragObject = nullptr;
	mMouseClickDragFirstObjTag = 1008613;
	mIsMouseClickDragCreate = false;
	mMouseClickDragStoreyIndex = -1;

	curWave=0;
	TotalWave=0;
	CountTime=0;

	// 加层按钮，注册事件
	//Button* _starButtonAdd			= static_cast<Button*>(findChild("Button_StarAdd"));
	//_starButtonAdd->addTouchEventListener(this, toucheventselector(Ui_MainBack::eventTouchButton));
	//_starButtonAdd->removeFromParentAndCleanup(true);


	// 初始化滚动层
	// 取scroll view控件
	mScrollViewWidget = static_cast<ui::ScrollView*>(findChild("ScrollView_Back"));
	mScrollViewWidget->addEventListenerScrollView(this, scrollvieweventselector(Ui_MainBack::eventScrollView));
	// 取list view控件
	mListViewWidget = static_cast<ui::ListView*>(findChild("ListView_Layer"));


	ImageView* _imageSp = static_cast<ImageView*>(findChild("Image_Sp"));
	WindowManager::getInstance()->setTextureAlias(_imageSp);

	ImageView* _imageSp1 = static_cast<ImageView*>(findChild("Image_1"));
	WindowManager::getInstance()->setTextureAlias(_imageSp1);
	
	ImageView* _imageSp2 = static_cast<ImageView*>(findChild("Image_BackGround2"));
	WindowManager::getInstance()->setTextureAlias(_imageSp2);
	
	// 层与层UI间隔
	mListViewItemsMargin = 0;	
	mListViewWidget->setItemsMargin(mListViewItemsMargin);

	mGroundPanelWidget = findChild("Panel_GroundBack");
	thisSceneData->initGorundData(mGroundPanelWidget);
	Layout* _curPanel = static_cast<Layout*>(mGroundPanelWidget);
	_curPanel->setBackGroundColorType(LAYOUT_COLOR_NONE);
	//下一波按钮UI
	mGroundButtonWidget = Ui_NextRound::create();
	Layout *lay = static_cast<Layout*>(findChild("Panel_groundBtn"));
	lay->setBackGroundColorType(LAYOUT_COLOR_NONE);
	lay->addChild(mGroundButtonWidget,100);

	fitScreen();
	initAnimation();
	updateSceneStorey();
	updateAddStoreyCost();
	doEvent105();
	this->schedule(schedule_selector(Ui_MainBack::update),(float)1.0/60);
	this->addChild(mMainWidget);
	// 一秒后执行是否创建英雄
	WindowManager::getInstance()->setSwallowAllUi(true);
	scheduleOnce(schedule_selector(Ui_MainBack::createFirstHero), 1.0f);

	return true;
}

void Ui_MainBack::resetCloundsPosition()
{
	ImageView *cloud_1 = static_cast<ImageView*>(findChild("ImageView_cloud_1"));
	ImageView *cloud_2 = static_cast<ImageView*>(findChild("ImageView_cloud_2"));
	ImageView *cloud_3 = static_cast<ImageView*>(findChild("ImageView_cloud_3"));
	ImageView *cloud_4 = static_cast<ImageView*>(findChild("ImageView_cloud_4"));

	if (cloud_1->getPositionX() >= 480+cloud_1->getSize().width)
		cloud_1->setPositionX(-480);
	else
		cloud_1->setPositionX(cloud_1->getPositionX()+0.1f);
	
	if (cloud_2->getPositionX() >= 480+cloud_2->getSize().width)
		cloud_2->setPositionX(-480);
	else 
		cloud_2->setPositionX(cloud_2->getPositionX()+0.5f);

	if (cloud_3->getPositionX() >= 480+cloud_3->getSize().width)
		cloud_3->setPositionX(-480);
	else
		cloud_3->setPositionX(cloud_3->getPositionX()+1.0f);


	if (cloud_4->getPositionX() >= 480+cloud_4->getSize().width)
		cloud_4->setPositionX(-480);
	else 
		cloud_4->setPositionX(cloud_4->getPositionX()+0.3f);
}

// 更新滚动层
void Ui_MainBack::updateSceneStorey()
{
	// 删除所有层, 默认0高度
	mListViewWidget->removeAllItems();
	mListViewWidget->setSize(Size(mListViewWidget->getSize().width, addHeight));
	mListViewWidget->setPositionY(640);
	mGroundPanelWidget->removeAllChildren();
	
	// 设置显示区域
	ImageView* _imageBackGround1 = dynamic_cast<ImageView*>(mScrollViewWidget->getChildByName("Image_BackGround1"));
	ImageView* _imageBackGround2 = dynamic_cast<ImageView*>(mScrollViewWidget->getChildByName("Image_BackGround2"));

	// 默认0高度
	_imageBackGround2->setSize(Size(_imageBackGround2->getSize().width, 0));
	_imageBackGround2->setPositionY(640);
	// 重置scrioll view 高度
	int newHeight = _imageBackGround1->getSize().height + _imageBackGround2->getSize().height;
	mScrollViewWidget->setInnerContainerSize(Size(mScrollViewWidget->getSize().width, newHeight));

	// 清空所有层数据
	thisSceneData->clearScene();
	// 添加默认层
	addStorey(0, SCENE_STOREY_HOME);
	addStorey(0, SCENE_STOREY_TOP);

	// 添加默认关卡门
	updateHomeDoor();
}
// 更新初始化Home关卡门
void Ui_MainBack::updateHomeDoor()
{
	StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
	if (!_curStoreyData)
		return;
	Widget* _curItemCell = _curStoreyData->mItem;
	Widget* _curMovement = _curStoreyData->getMovementWidget();
	Point _curPos = Point(0, 0);
	auto _insideDoor = WindowManager::getInstance()->createDoorObject(InsideDoor, _curPos);
	auto _outDoor    = WindowManager::getInstance()->createDoorObject(OutDoor, _curPos);
	if (_insideDoor && _outDoor)
	{
		Widget* door_warp = Helper::seekWidgetByName(_curItemCell, "door_warp");
		
		//_curPos.x = _curStoreyData->getRightDoor()->getPositionX() - _curStoreyData->getRightDoor()->getSize().width/2;
		////_curPos.y = _curStoreyData->getHorizon()->getPositionY() + _newDoor->getContentSize().height/2;
		//_curPos.y = _curStoreyData->getRightDoor()->getPositionY() ;

		_insideDoor->setPosition(door_warp->getPosition());
		_insideDoor->setAnchorPoint(Point::ZERO);
		_insideDoor->setfloorNum(_curStoreyData->getStoreyIndex());
		_insideDoor->setLocalZOrder(3);
		_curItemCell->addChild(_insideDoor);
		_curStoreyData->addObject(_insideDoor);

		_outDoor->setAnchorPoint(Point::ZERO);
		_outDoor->setPosition(_insideDoor->getPosition());
		_outDoor->setTag(12321);
		_curItemCell->addChild(_outDoor, 11);

		Sprite* attackedEffect = Sprite::create("image/Men_001.png"); 
		attackedEffect->setAnchorPoint(Point::ZERO);
		attackedEffect->setPosition(Point::ZERO);
		attackedEffect->setOpacity(0);
		attackedEffect->setTag(0);
		_outDoor->addChild(attackedEffect);

		if (_curStoreyData->getRightDoor()!=nullptr){
			_curStoreyData->getRightDoor()->setVisible(false);
		}
	}
}

// 添加层
void Ui_MainBack::addStorey(size_t _index, size_t _storeyIndex)
{
	size_t _curListCount = mListViewWidget->getItems().size();
	size_t _curInsertUiIndex = _index;

	// New一个新层
	StoreyData* _newStorey = new StoreyData(_curListCount, _storeyIndex);
	auto _newStoreyItem = _newStorey->mItem;
	_newStorey->autorelease();
	_newStoreyItem->setAnchorPoint(Point(0.5f,0));
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = 1.0f;
	if (visibleSize.height<640)
		_scale = visibleSize.height/640;

	_newStoreyItem->setScale(_scale);
	_newStoreyItem->setSize(Size(_newStoreyItem->getSize().width,_newStoreyItem->getSize().height*_scale));
	UserRecord::getInstance()->setStoreyScale(_scale);
	
	// 设置新的高度
	ImageView* _imageBackGround1 = dynamic_cast<ImageView*>(mScrollViewWidget->getChildByName("Image_BackGround1"));
	ImageView* _imageBackGround2 = dynamic_cast<ImageView*>(mScrollViewWidget->getChildByName("Image_BackGround2"));
	mListViewWidget->setSize(Size(mListViewWidget->getSize().width, mListViewWidget->getSize().height + _newStoreyItem->getSize().height));
	mListViewWidget->setPositionY(640);
	_imageBackGround2->setSize(Size(_imageBackGround2->getSize().width, mListViewWidget->getSize().height));
	_imageBackGround2->setPositionY(640);
	// 设置scroll view高度
	int newHeight = _imageBackGround1->getSize().height + _imageBackGround2->getSize().height ;
	mScrollViewWidget->setInnerContainerSize(Size(mScrollViewWidget->getSize().width, newHeight));

	// 添加到UI
	mListViewWidget->insertCustomItem(_newStoreyItem, _curInsertUiIndex);

	// 添加新层到场景数据
	thisSceneData->addStorey(_curListCount, _newStorey);


	// 添加新层修改关卡门的位置
	if (_storeyIndex != SCENE_STOREY_HOME && _storeyIndex != SCENE_STOREY_TOP)
	{
		// 删除旧的
		float _curHp = 0.0f;
		for (int i = 0; i < thisSceneData->getSceneDataMap().size(); ++i)
		{
			StoreyData* thisStoreyData = thisSceneData->getSceneDataMapStorey(i);
			if (thisStoreyData->getDoor())
			{
				_curHp = thisStoreyData->getDoor()->gethp();
				thisStoreyData->removeDoorObject();
				if (thisStoreyData->mItem->getChildByTag(12321)!=nullptr){
					thisStoreyData->mItem->removeChildByTag(12321);
				}
				if (thisStoreyData->getRightDoor()!=nullptr){
					thisStoreyData->getRightDoor()->setVisible(true);
				}
			}	
		}
		// 新建新的
		Point _curPos = Point(0, 0);
		auto _insideDoor = WindowManager::getInstance()->createDoorObject(InsideDoor, _curPos);
		auto _outDoor    = WindowManager::getInstance()->createDoorObject(OutDoor, _curPos);
		if (_insideDoor)
		{
			_curPos.x = _newStorey->getRightDoor()->getPositionX() - _newStorey->getRightDoor()->getSize().width/2 + _insideDoor->getContentSize().width/2;
			_curPos.y = _newStorey->getHorizon()->getPositionY() + _insideDoor->getContentSize().height/2;
			Widget* door_warp = Helper::seekWidgetByName(_newStoreyItem, "door_warp");
			_insideDoor->setPosition(door_warp->getPosition());
			_insideDoor->setAnchorPoint(Point::ZERO);
			//_newDoor->setPosition(_curPos);
			_insideDoor->setfloorNum(_newStorey->getStoreyIndex());
			_insideDoor->sethp(_curHp);
			_insideDoor->setLocalZOrder(3);
			_newStoreyItem->addChild(_insideDoor);
			_newStorey->addObject(_insideDoor);

			_outDoor->setAnchorPoint(Point::ZERO);
			_outDoor->setPosition(_insideDoor->getPosition());
			_outDoor->setTag(12321);
			_newStoreyItem->addChild(_outDoor, 11);

			Sprite* attackedEffect = Sprite::create("image/Men_001.png"); 
			attackedEffect->setAnchorPoint(Point::ZERO);
			attackedEffect->setPosition(Point::ZERO);
			attackedEffect->setOpacity(0);
			attackedEffect->setTag(0);
			_outDoor->addChild(attackedEffect);


			if (_newStorey->getRightDoor()!=nullptr){
				_newStorey->getRightDoor()->setVisible(false);
			}
		}
	} else if(_storeyIndex == SCENE_STOREY_HOME)
	{
		auto Altar = AltarObject::create();

		Widget* sweet_warp = Helper::seekWidgetByName(_newStoreyItem, "sweet_warp");
		sweet_warp->setTag(AlTAR_PARENT_TAG);
		sweet_warp->addChild(Altar,5);
		Altar->setAnchorPoint(Point::ZERO);
		Altar->createTouchEvent();
		Altar->setTag(AlTAR_MY_TAG);
		//NPC
		auto npc = npcObjects::create();
		_newStoreyItem->addChild(npc,7);
		npc->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		//auto hor = _newStorey->getHorizon();
		auto hor = Helper::seekWidgetByName(_newStoreyItem, "HorizonY");
		npc->setPosition(Point(200,hor->getPositionY()));
		npc->_minX =(int) hor->getPositionX()-hor->getSize().width/2;
		npc->_maxX =(int) hor->getPositionX()+hor->getSize().width/2-300;
		npc->ActionDone();
	}
	
}

void Ui_MainBack::freshmanArrowPrompt(const char* id)
{
	Widget *topStorey    = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP)->mItem;
	if (strcmp(id ,"100017")==0 || strcmp(id ,"100018")==0){
		Widget *secondStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP-1)->mItem;
		topStorey = secondStorey;
	}

	auto child = topStorey->getChildByTag(999);
	if (child)
		topStorey->removeChild(child);

	auto ch = topStorey->getChildByTag(1000);
	if (ch)
		topStorey->removeChild(ch);

	Point arrowPos = Point::ZERO;
	char  _createName[32];
	if (strcmp(id ,"100012")==0 || strcmp(id ,"100017")==0 || strcmp(id ,"200006")==0 || strcmp(id ,"200007")==0){

        int index =2 ;
// 		if (strcmp(id ,"100017")!=0){
// 			index = getAnEmptyIndex();
// 		}else{
// 			index = 2;
// 		}
			
		if (strcmp(id ,"200006")==0)
			index = 2;
		else if (strcmp(id ,"200007")==0)
			index = 3;

		sprintf(_createName, "Image_CreateView%d", index);  
		Widget* thisCreateRectCtrl = topStorey->getChildByName(_createName);
		arrowPos.x  = thisCreateRectCtrl->getPositionX();
	}else{
		return;
	} 
	Sprite *arrow = Sprite::create("image/JianTou.png");
	arrow->setAnchorPoint(Point(0.5f,0));
	arrow->setPosition(arrowPos);
	arrow->setTag(999);
	topStorey->addChild(arrow,10);

	arrow->runAction(RepeatForever::create(Sequence::create(
															MoveTo::create(0.8f,Point(arrow->getPositionX(),20)),
															MoveTo::create(0.8f,Point(arrow->getPositionX(),0)),
															NULL
															)));

	if ( strcmp(id ,"100012")==0 || strcmp(id ,"100017")==0 ){
		Sprite *sp = Sprite::create("freshman/ZI_022.png");
		sp->setPosition(Point(arrowPos.x,arrowPos.y+110));
		sp->setTag(1000);
		topStorey->addChild(sp,20);
	}else{
		Sprite *sp = Sprite::create("freshman/ZI_025.png");
		sp->setPosition(Point(arrowPos.x,arrowPos.y+110));
		sp->setTag(1000);
		topStorey->addChild(sp,20);
	}
	
}

int Ui_MainBack::getAnEmptyIndex()
{
	return 2;
// 	StoreyData *topStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP);
// 
// 	if (topStoreyData->mObjectData.empty()){
// 		return 3;
// 	}else{
// 		bool hasObj = false;
// 		for (int idx =3;idx!=0;--idx){
// 			for (GameObjects *sp:topStoreyData->mObjectData){
// 				if (sp){
// 					if (sp->getlocationIndx()==idx){
// 						hasObj = true;
// 					}
// 				}
// 			}
// 			if (!hasObj){
// 				return idx;
// 			}
// 			hasObj = false;
// 		}
// 	}
// 	return 3;
}

// 添加层上逻辑物件
void Ui_MainBack::addStoreyObject(size_t _index, GameObjects* _obj)
{
	thisSceneData->addStoreyObject(_index, _obj);
}
// 添加层上逻辑物件
void Ui_MainBack::addStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj)
{
	StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	if (_curStoreyData)
	{
		thisSceneData->addStoreyObject(_curStoreyData->getUiIndex(), _obj);
	}
}

// 删除层上指定物件, 索引为ui层
void Ui_MainBack::removeStoreyObject(size_t _index, GameObjects* _obj)
{
	StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStorey(_index);
	if (_curStoreyData)
	{
		// 删除数据
		thisSceneData->removeStoreyObject(_index, _obj);
		// 删除UI
		if (_curStoreyData->getStoreyIndex() != SCENE_STOREY_GROUND)
		{
			_curStoreyData->mItem->removeChild(_obj);
		}else
		{
			mGroundPanelWidget->removeChild(_obj);
		}
	}
}

// 删除层上指定物件, 索引为数据索引
void Ui_MainBack::removeStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj)
{
	// 把层级信息保存到plist
	StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	thisSceneData->removeStoreyObjectFormStoreyIndex(_storeyIndex, _obj);
	
	if (_curStoreyData->getStoreyIndex() != SCENE_STOREY_GROUND)
	{
		_curStoreyData->mItem->removeChild(_obj);
		
	}else
	{
		mGroundPanelWidget->removeChild(_obj);
	}
}

// 删除层上所有物件, 索引为数据索引
void Ui_MainBack::removeStoreyObjectFormStoreyIndex(size_t _storeyIndex)
{
	StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	std::list<GameObjects*> lstObjectData = _curStoreyData->getObjectData();
	std::list<GameObjects*>::iterator iter  = lstObjectData.begin();
	while (iter != lstObjectData.end())
	{
		_curStoreyData->mItem->removeChild(*iter);
		++iter;
	}
	_curStoreyData->clearObject();
}
// 删除所有物件...................
void Ui_MainBack::clearAllObject()
{
	// 删除地平线
	mGroundPanelWidget->removeAllChildren();
	// 删除层
	for (int i = 0; i < thisSceneData->getSceneDataMap().size(); ++i)
	{
		StoreyData* _thisStorey = thisSceneData->getSceneDataMapStorey(i);
		std::list<GameObjects*> lstObjectData = _thisStorey->getObjectData();
		std::list<GameObjects*>::iterator iter  = lstObjectData.begin();
		while (iter != lstObjectData.end())
		{
			_thisStorey->mItem->removeChild(*iter);
			++iter;
		}
		_thisStorey->mItem->setTouchEnabled(false);
	}
	thisSceneData->clearSceneAllObject();
}

// 按钮点击事件
void Ui_MainBack::eventTouchButton(Object* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			Button* nowButton = static_cast<Button*>(sender);
			if (nowButton)
			{
				if (nowButton->getTag() == 6)
				{
					//auto _curSweet = UserRecord::getInstance()->getSweet();
					//auto _costSweet = PlistManager::getInstance()->getConfigTypeCString("base","openRound").intValue();

					//auto _curItem5SelectIndex = PlistManager::getInstance()->getUpgradesSelect("tech", "item5");
					//if (_curItem5SelectIndex > 0)
					//{
					//	auto _curLevelString = StringUtils::format("level%d", _curItem5SelectIndex);
					//	auto _costSweetRate = PlistManager::getInstance()->getUpgradesConfigKeyValue("tech", "item5", _curLevelString, "value");
					//	auto _thisObjectValue = PlistManager::getInstance()->toCString(_costSweetRate).floatValue();
					//	auto _newCostSweet = _costSweet * ( 1- _thisObjectValue );
					//	_costSweet = _newCostSweet;
					//}
					//
					//if (_curSweet < _costSweet)
					//	return;
					//// 判断下是否可以添加新层
					//size_t _itemCellCount = mListViewWidget->getItems().size();
					//size_t _itemCellMapCount = thisSceneData->getSceneDataMap().size();
					//// 超过最大层数，不给添加了
					//if (_itemCellMapCount >= thisSceneData->mMaxStoreyCount)
					//	return;
					//UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()-PlistManager::getInstance()->getConfigTypeCString("base","openRound").intValue());
					//// 添加新层
					//int _newStoreyIndex = thisSceneData->getNextCreateStoreyIndex();
					//addStorey(mListViewWidget->getItems().size() -1 , _newStoreyIndex);

					// 添加新层
					/*	int _newStoreyIndex = thisSceneData->getNextCreateStoreyIndex();
					addStorey(mListViewWidget->getItems().size() -1 , _newStoreyIndex);*/

				}else if (nowButton->getTag() == 9)
				{
					removeStoreyObjectFormStoreyIndex(SCENE_STOREY_GROUND);
					removeStoreyObjectFormStoreyIndex(SCENE_STOREY_TOP);
				}
			}
		}
		break;
	default:
		break;
	}
}
void Ui_MainBack::setAddStoreyNum(int _num)
{
	mAddStoreyNum = _num;
}
void Ui_MainBack::doAddStorey(bool _isCost)
{
	// 判断下是否可以添加新层
	size_t _itemCellCount = mListViewWidget->getItems().size();
	size_t _itemCellMapCount = thisSceneData->getSceneDataMap().size();
	// 超过最大层数，不给添加了
	if (_itemCellMapCount >= thisSceneData->mMaxStoreyCount)
	{
		mAddStoreyNum = 0;
		return;
	}
	if (_isCost)	// 是否消耗糖果
	{
		auto _curSweet = UserRecord::getInstance()->getSweet();
		auto _costSweet = mAddStoreyCost;
		if (_curSweet < _costSweet)
			return;
		UserRecord::getInstance()->alterSweet(-_costSweet);
	}
	// 删除升级弹出框
	if (WindowManager::getInstance()->mGameLayer->getChildByTag(250))
	{
		WindowManager::getInstance()->mGameLayer->removeChildByTag(250);
	}
	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY,(Ref*)0);
	mScrollViewWidget->setTouchEnabled(false);
	WindowManager::getInstance()->setSwallowAllUi(true);
	// 添加新层
	int _newStoreyIndex = thisSceneData->getNextCreateStoreyIndex();
	addStorey(mListViewWidget->getItems().size() -1 , _newStoreyIndex);
	mScrollViewWidget->jumpToBottom();
	// 显示随机
	mAddYuJingStoreyIndex = _newStoreyIndex;

	// 当前加入层
	auto _nowStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_newStoreyIndex);
	if (!_nowStoreyData || !_nowStoreyData->mItem)
	{
		mAddStoreyNum = 0;
		return;
	}
	// Home层
	auto _homeStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME);
	if (_homeStoreyData && _homeStoreyData->mItem)
	{
		Point _newPos;
		_newPos.x = 0;
		_newPos.y = _nowStoreyData->mItem->getSize().height/2;
		auto _moveAction = MoveBy::create(0,  _newPos);
		_homeStoreyData->mItem->runAction(_moveAction);
		mScrollViewWidget->jumpToPercentVertical(85);
	}
	// 其他层
	for (int i = _newStoreyIndex; i <= SCENE_STOREY_TOP;  ++i)
	{
		auto _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(i);
		if (_curStoreyData && _curStoreyData->mItem)
		{
			if (_newStoreyIndex == _curStoreyData->getStoreyIndex())
			{

			}else
			{
				Point _newPos;
				_newPos.x = 0;
				_newPos.y = -_curStoreyData->mItem->getSize().height/2;
				auto _moveAction = MoveBy::create(0,  _newPos);
				_curStoreyData->mItem->runAction(_moveAction);
			}
		}
	}

	float _defaultX = 0;
	// 默认当前层, 移动到屏幕外
	if (_nowStoreyData && _nowStoreyData->mItem)
	{
		_defaultX = _nowStoreyData->mItem->getPositionX();
		Point _newPos;
		_newPos.x = Director::getInstance()->getVisibleSize().width;// + _nowStoreyData->mItem->getSize().width;
		_newPos.y = 0;
		auto _moveAction = MoveBy::create(0,  _newPos);
		_nowStoreyData->mItem->runAction(_moveAction);
		_nowStoreyData->createArmature1();
		_nowStoreyData->createArmature2();
	}

	// 处理上面的移动上去
	for (int i = _newStoreyIndex; i <= SCENE_STOREY_TOP;  i++)
	{
		auto _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(i);
		if (_curStoreyData && _curStoreyData->mItem)
		{
			if (_newStoreyIndex == _curStoreyData->getStoreyIndex())
			{

			}else
			{
				Point _newPos;
				_newPos.x = 0;
				_newPos.y = _curStoreyData->mItem->getSize().height/2;
				auto _moveAction = MoveBy::create(0.2,  _newPos);
				_curStoreyData->mItem->runAction(_moveAction);
			}
		}
	}
	// 处理Home移动到下面去
	if (_homeStoreyData && _homeStoreyData->mItem)
	{
		Point _newPos;
		_newPos.x = 0;
		_newPos.y = -_nowStoreyData->mItem->getSize().height/2;
		auto _moveAction = MoveBy::create(0.2,  _newPos);
		_homeStoreyData->mItem->runAction(Sequence::create(
			_moveAction, 
			CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::onMoveCurStorey, this, _nowStoreyData, _defaultX)),
			NULL));

		auto delay=DelayTime::create(0.1f);
		auto ddlay = Sequence::create(
			delay,
			CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::doStoreyArmature, this, _nowStoreyData)),
			NULL);
		_homeStoreyData->mItem->runAction(ddlay);

		// 判断下是否可以添加新层
		size_t _itemCellMapCount1 = thisSceneData->getSceneDataMap().size();
		// 超过最大层数，不给添加了
		if (_homeStoreyData->getIsProAddStorey())
		{
			_homeStoreyData->mButtonAddStorey->setVisible(true);
			if (_itemCellMapCount1 >= thisSceneData->mMaxStoreyCount)
			{
				_homeStoreyData->mButtonAddStorey->setVisible(false);
			}
		}
	}
}
void Ui_MainBack::updateAddStoreyButtonState()
{
	if (UserRecord::getInstance()->getRound()== UserRecord::getInstance()->getMaxRound() && SceneData::getInstance()->isClearAllMonster()){

		return;
	}

	auto _homeStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME);
	if (_homeStoreyData && _homeStoreyData->mItem && _homeStoreyData->mButtonAddStorey)
	{
		if (_homeStoreyData->getIsProAddStorey())
			return;

		bool _isClick = true;
		// 层级满了
		if (thisSceneData->getNextCreateStoreyIndex() == -1)
		{
			_isClick = false;
		}
		// 检测改变状态
		auto _curSweet1 = UserRecord::getInstance()->getSweet();
		auto _costSweet1 = PlistManager::getInstance()->getConfigTypeCString("base","openRound").intValue();
		/*auto _curItem5SelectIndex1 = PlistManager::getInstance()->getUpgradesSelect("tech", "item5");
		if (_curItem5SelectIndex1 > 0)
		{
		auto _curLevelString = StringUtils::format("level%d", _curItem5SelectIndex1);
		auto _costSweetRate = PlistManager::getInstance()->getUpgradesConfigKeyValue("tech", "item5", _curLevelString, "value");
		auto _thisObjectValue = PlistManager::getInstance()->toCString(_costSweetRate).floatValue();
		auto _newCostSweet = _costSweet1 * ( 1- _thisObjectValue );
		_costSweet1 = _newCostSweet;
		}*/
		// 消耗不足
		if (_curSweet1 < _costSweet1)
		{
			_isClick = false;
		}
		//  设置是否可以点击
		if (_isClick)
		{
			setWidgetGray(_homeStoreyData->mButtonAddStorey, false);
			_homeStoreyData->mButtonAddStorey->setTouchEnabled(true);
		}else
		{
			setWidgetGray(_homeStoreyData->mButtonAddStorey, true);
			_homeStoreyData->mButtonAddStorey->setTouchEnabled(false);
		}
	}
}
void Ui_MainBack::onMoveCurStorey(Node* sender, StoreyData* _storeyData, float _x)
{
	if (_storeyData && _storeyData->mItem)
	{
		auto _item = _storeyData->mItem;
		Point _newPos;
		_newPos.x = _x + _item->getSize().width/2;
		_newPos.y = _item->getPositionY();
		auto _moveAction = MoveTo::create(0.2,  _newPos);
		_item->runAction(Sequence::create(
			_moveAction, 
			CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::onMoveCurStoreyFinish, this, _storeyData)),
			NULL));
	}
}
void Ui_MainBack::doStoreyArmature(Node* sender, StoreyData* _storeyData)
{
	if (_storeyData && _storeyData->mItem)
	{
		// 播放动画1，撞击落石
		_storeyData->beginArmature1();
	}
}
void Ui_MainBack::onMoveCurStoreyFinish(Node* sender, StoreyData* _storeyData)
{
	if (_storeyData && _storeyData->mItem)
	{
		// 振屏
		auto _curScene = Director::sharedDirector()->getRunningScene();
		auto _action = Shake::create(1.5, 10);
		_curScene->runAction(_action);
	}
}
void Ui_MainBack::onAddStoreyAinmFinish()
{
	mAddStoreyNum = mAddStoreyNum - 1;
	if (mAddStoreyNum > 0)
	{
		doAddStorey(false);
		return;
	}else
	{
		mAddStoreyNum = 0;
	}
	NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, (Ref*)0);
	//恢复按钮
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	mainLayer->removePauseBtnGray();


	mScrollViewWidget->setTouchEnabled(true);
	WindowManager::getInstance()->setSwallowAllUi(false);
	this->runAction(Sequence::create(DelayTime::create(5.0f),CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::onIsAddStoreyDisYuJingTimeCallBack,this)),NULL));
}
void Ui_MainBack::onIsAddStoreyDisYuJingTimeCallBack(Ref* sender)
{
	mAddYuJingStoreyIndex = -1;
}
void Ui_MainBack::itemBtnClickedCallfuc(Object *pSender, TouchEventType type , int id)
{
/*
	1.按下
		-- 选中id
		-- 取到起始点
		-- 默认点击模式
	2.移动
		-- 是否移动出安全范围
			-- 创建虚拟体
			-- 是否可以创建实体
			-- 变成拖拽模式
	3.弹起
		-- 是否可以创建
			-- 点击模式
				-- 创建监听
			-- 拖拽模式
				-- 创建实体
*/
	Widget* _pWidget = static_cast<Widget*>(pSender);
	if (!_pWidget)
		return;

	if (type == TOUCH_EVENT_BEGAN)
	{
		mMouseDragBeginItemIndex = id;
		mIsMouseDragMode = false;
		mIsMouseClickPosMoving = false;
		// 选中id提示框
		Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
		if (mainLayer)
			mainLayer->setItemSelect(id);
	}else if (type == TOUCH_EVENT_MOVED){
		// 没有选中id，删除所有恢复默认
		if (mMouseDragBeginItemIndex < 1 || mMouseDragBeginItemIndex > READY_MAX_COUNT)
		{
			dragObjectFinish(nullptr);
			return;
		}
		// 默认点击模式
		mIsMouseDragMode = false;
		// 是否移动出安全范围
		Point _touchStarPos =  getScreenPos(_pWidget->getTouchStartPos());
		_touchStarPos.y = 0;
		Point _touchMovePos =  getScreenPos(_pWidget->getTouchMovePos());
		_touchMovePos.y = _touchMovePos.y + mScrollViewWidget->getInnerContainer()->getPositionY();
		// 移除屏幕准备栏的高度
		if (abs(_touchMovePos.x - _touchStarPos.x) > 15 && abs(_touchMovePos.y - _touchStarPos.y) > 15)
		{
			// 拖拽模式
			mIsMouseDragMode = true;
		}
		// 移除屏幕准备栏的高度
		if (abs(_touchMovePos.y - _touchStarPos.y) > 110)
		{
			mIsMouseDragMode = true;
			// 移出安全范围
			mIsMouseClickPosMoving = true;
			// 创建虚拟体
			updateCreateMouseClickDragObject(_pWidget->getTouchMovePos());
		}else
		{
			// 移进安全范围
			mIsMouseClickPosMoving = false;
			if (mMouseClickDragObject)
			{
				updateCreateMouseClickDragObject(_pWidget->getTouchMovePos(), true);
			}
		}
	}else if (type == TOUCH_EVENT_CANCELED || type == TOUCH_EVENT_ENDED){
		// 没有选中id，删除所有恢复默认
		if (mMouseDragBeginItemIndex < 1 || mMouseDragBeginItemIndex > READY_MAX_COUNT)
		{
			dragObjectFinish(nullptr);
			return;
		}
		// 点击模式
		if (!mIsMouseDragMode)
		{
			if (mMouseClickDragObject)
			{
				dragObjectFinish(nullptr);
				return;
			}
			// 创建点击监听
			createCreateObjListenerTouch();
			mMouseClickDragBeginPos = _pWidget->getTouchStartPos();
			return;
		}
		// 拖拽模式
		else
		{
			// 在安全范围外
			if (mIsMouseClickPosMoving)
			{
				// 创建实体
				createMouseClickDragObject(_pWidget->getTouchMovePos());
			}else
			{
				// 安全范围内，删除所有恢复默认
				dragObjectFinish(nullptr);
			}
		}
	}
	
}

void Ui_MainBack::createHeroToStorey(int _storeyIndex, int _heroType, Point _heroPos, int _heroLevel)
{
	StoreyData* _storeyDatas = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	if (!_storeyDatas || _storeyDatas->getHero())
	{
		return;
	}
	Widget* _storeyItemCell = _storeyDatas->mItem;
	Widget* HorizonY = _storeyItemCell->getChildByName("HorizonY");
	Widget* drag3 = _storeyItemCell->getChildByName("Image_DragView3");

	// 创建新的添加到Cell
	Point _newPos;
	_newPos.x = drag3->getPositionX();
	_newPos.y = HorizonY->getPositionY();// - mScrollViewWidget->getInnerContainer()->getPositionY();
	//auto _newObject = createObject(TYPE_BOSS, _newPos);
	if (_heroPos.x != 0)
	{
		_newPos.x = _heroPos.x;
	}
	auto _newObject = WindowManager::getInstance()->createBossObject(_heroType, _newPos);
	_storeyItemCell->addChild(_newObject, 7);
	_newObject->setPosition(_newPos);
	_newObject->updateLevel(_heroLevel);
	_newObject->setlocationIndx(3);
	_newObject->setfloorNum(_storeyIndex);
	_newObject->setisStart(YES);
	//_newObject->sethp(10);

	addStoreyObjectFormStoreyIndex(_storeyIndex, _newObject);
	// 把层级信息保存到plist
	UserRecord::getInstance()->setHeroKeyValue(_heroType, _HeroDataKey_Index_storeyIndex, _storeyIndex);
}

// 降落地平线完毕回调
void Ui_MainBack::dragObjectFinish(Node* sender)
{
	// 拖动结束，  恢复初始数据
	mMouseDragBeginItemIndex = -1;
	mIsMouseDragMode = false;
	for (int i = 0; i < mListViewWidget->getItems().size(); ++i)
	{
		Widget* thisItemCell = mListViewWidget->getItem(i);
		Widget* thisRectCtrl = thisItemCell->getChildByName("Image_Forbid");
		if (thisRectCtrl)
		{
			thisRectCtrl->setVisible(false);
		}
		Widget* thisNoAddCtrl = thisItemCell->getChildByName("Image_NoAddTag");
		if (thisNoAddCtrl)
		{
			thisNoAddCtrl->setVisible(false);
		}
	}
	// 选中显示
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	if (mainLayer)
	{
		mainLayer->setItemSelect(1, true);
	}
	// 销毁点击模式数据
	if (mMouseClickDragObject)
		mMouseClickDragObject->removeFromParentAndCleanup(true);
	if (mMouseClickDragObject)
		mListViewWidget->removeProtectedChild(mMouseClickDragObject);
	if (mListViewWidget->getProtectedChildByTag(mMouseClickDragFirstObjTag))
		mListViewWidget->removeProtectedChildByTag(mMouseClickDragFirstObjTag);
	mMouseClickDragObject = nullptr;
	mMouseClickDragStoreyIndex = -1;
	mIsMouseClickDragCreate = false;
	mMouseClickDragBeginPos = Point(0, 0);
	mIsMouseClickPosMoving = false;
	// 删除点击监听
	removeCreateObjListenerTouch();
}
// 查找子控件
Widget* Ui_MainBack::findChild(const char* _widgetName)
{
	if (!mMainWidget)
		return nullptr;
	Widget* _thisChild = Helper::seekWidgetByName(mMainWidget, _widgetName);
	if (nullptr == _thisChild)
		return nullptr;
	return _thisChild;
}
// 创建逻辑物件
GameObjects* Ui_MainBack::createObject(int objIndex, Point objPos)
{


	
	GameObjects* _newObject = nullptr;

	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	std::map<int,ItemInfo> _map = mainLayer->m_itemInfo;
	
	int selfType = _map.at(objIndex).selfType;
	int objType  = _map.at(objIndex).objType; 

	int obj=0;
	switch (objType)
	{
	case TYPE_WARRIOR: 
		if (selfType == 1){//  近程战士，小谢的化身来了
			obj = WAR_ITEM(1);
		}else if (selfType == 2){//远程攻击 
			obj = WAR_ITEM(2);
		}else if (selfType == 3){//飞行兵
			obj = WAR_ITEM(3);
		}else if (selfType == 4){//自爆兵
			obj = WAR_ITEM(4);
		}else if (selfType == 5){//吓人狗
			obj = WAR_ITEM(5);
		}
		
		else{//自爆兵种
			obj = WAR_ITEM(1);
		}
		
		_newObject=WindowManager::getInstance()->createWarriorObject(obj,objPos);
		break;
	case TYPE_TOWER:
		if (selfType == 1){
			obj = TOWER_ITEM(1);
		}else if (selfType == 2){
			obj = TOWER_ITEM(2);
		}else if (selfType == 3){
			obj = TOWER_ITEM(3);
		}else if (selfType == 4){
			obj = TOWER_ITEM(4);
		}else{
			obj = TOWER_ITEM(5);
		}
		_newObject=WindowManager::getInstance()->createTowerObject(obj,objPos);
		break;
	case TYPE_TRAP:
		if (selfType == 1){
			obj = TRAP_IETM(1);
		}else if (selfType == 2){
			obj = TRAP_IETM(2);
		}else if (selfType == 3){
			obj = TRAP_IETM(3);
		}else if (selfType == 4){
			obj = TRAP_IETM(4);
		}else{
			obj = TRAP_IETM(5);
		}
		_newObject=WindowManager::getInstance()->createTrapObject(obj,objPos);
		break;
	case TYPE_BOSS:
		if (selfType == 1){
			obj = BOSS_ITEM(1);
		}else if (selfType == 2){
			obj = BOSS_ITEM(2);
		}else if (selfType == 3){
			obj = BOSS_ITEM(3);
		}else if (selfType == 4){
			obj = BOSS_ITEM(4);
		}else{
			obj = BOSS_ITEM(5);
		}
		_newObject=WindowManager::getInstance()->createBossObject(obj,objPos);
		break;
	default:
		break;
	}
	
	return _newObject;
}

// 更新函数
void Ui_MainBack::update(float dt)
{
	updateGroundObjectPos(dt);			// 更新地面怪物行走
	//updateMouseDragItemCD(dt);			// 更新冷却动画
	updateStoreyObjectMoveToNext(dt);	// 更新检测当前层的物件是否可以进入到下一层
	updateStoreyObjectMoveToLast(dt);   // 更新检测当前层的物件是否可以进入到上一层
	//resetCloundsPosition();             //监控云的位置
	StoreyData* _SData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME);
	_SData->updatePropsNum(0.0);//定时更新道具
}

void Ui_MainBack::finishMoveCallback(Node *psender)
{
	GameObjects* _thisObj = static_cast<GameObjects*>(psender);	
	if (_thisObj)
	{
		// 降落到地平线后
		_thisObj->setisStart(YES);
		StoreyData* thisStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_thisObj->getfloorNum());
		if (thisStoreyData)
		{
			// 是否能够Touch
			Widget* _curItemCell = thisStoreyData->mItem;
			_curItemCell->setTouchEnabled(false);
		}
	}
	dragObjectFinish(nullptr);
}


void Ui_MainBack::CreateAnimationForDeathObject(Point pos){

	int max=13;
	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("%s%d.png","death-",i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);


	Sprite *bomb=Sprite::createWithSpriteFrameName("death-1.png");
	bomb->setPosition(Point(pos.x,pos.y+40));
	this->addChild(bomb);

	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::DeathObjectCallBack, this)),
		NULL);
	bomb->runAction(action);
}


void Ui_MainBack::DeathObjectCallBack(Object *psender){
	Sprite *psobject=dynamic_cast<Sprite*>(psender);
	this->removeChild(psobject,true);

}


Point Ui_MainBack::getScreenPos(Point _pos)
{
	Point newPos = Point::ZERO;
	do{
		newPos.x = _pos.x;
		CC_BREAK_IF(!mScrollViewWidget);
		newPos.y = _pos.y - mScrollViewWidget->getInnerContainer()->getPositionY();
	}while (0);
	return newPos;
}

// 创建指定类型的多个怪物
void Ui_MainBack::CreateEnemyForStorey(float posx ,const int enemy_type,const int num, int _batch, int _event106_count)
{
	Point _beginPos;
	_beginPos.x = posx;
	_beginPos.y = 0;
	int mIndex = SCENE_STOREY_GROUND;
	GameObjects* _newObject=nullptr;

	// 创建小兵
	for (int i = 1; i <= num; ++i)
	{
		// 是否106事件
		bool _isEvent106 = false;
		if (_event106_count > 0)
		{
			if (enemy_type != ENEMY_ITEM4 && enemy_type != ENEMY_ITEM8 && enemy_type != ENEMY_ITEM12 && enemy_type != ENEMY_ITEM16 && enemy_type != ENEMY_ITEM20 && enemy_type != ENEMY_ITEM21)
			{
				int _count = 0;
				while (true)
				{
					int _randomNumber = UserRecord::getInstance()->getRandom(num);
					_count = _count + 1;
					//CCLOG("count = %d, _randomNumber = %d, num = %d", _count, _randomNumber, num);
					if (_randomNumber >= i)
					{
						if (_randomNumber == i)
						{
							_isEvent106 = true;
							_event106_count = _event106_count - 1;
						}
						break;
					}
				}
				
			}
		}
		// 创建
		_newObject = WindowManager::getInstance()->createEnemyObject(enemy_type, _beginPos);
		mGroundPanelWidget->addChild(_newObject, 1);
		_newObject->setBatch(_batch);		
		_newObject->setfloorNum(SCENE_STOREY_GROUND);		//设置Object所在层, 地面
		if (_isEvent106)
		{
			enemyObjects* _curObject = static_cast<enemyObjects*>(_newObject);
			if (_curObject)
			{
				_curObject->setEnemyObjectEvent106();
			}
		}
		addStoreyObjectFormStoreyIndex(mIndex, _newObject);	// 添加逻辑物件
		this->EmenyMoveOnGround(_newObject);
		_beginPos.x -=_newObject->getArmature()->getContentSize().width+20;

	}
}
void Ui_MainBack::CreateEnemyForStorey(float dt)
{
	if (!mGroundPanelWidget)
		return;
	curWave++;
	int mIndex = SCENE_STOREY_GROUND;
	
	Point _beginPos;
	_beginPos.x = dt;
	_beginPos.y = 0;

	Point _endPos;
	_endPos.x = _beginPos.x + mGroundPanelWidget->getSize().width - 60;
	_endPos.y = _beginPos.y;

	GameObjects* _newObject = NULL;
	float rand_num =  CCRANDOM_0_1();
	if (rand_num<=0.2)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM1, _beginPos);
	else if (rand_num<=0.3)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM2, _beginPos);
	else if (rand_num<=0.4)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM3, _beginPos);
	else if (rand_num<=0.5)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM5, _beginPos);
	else if (rand_num<=0.6)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM6, _beginPos);
	else if (rand_num<=0.7)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM7, _beginPos);
	else if (rand_num<=0.75)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM13, _beginPos);
	else if (rand_num<=0.85)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM14, _beginPos);
	else if (rand_num<=0.95)
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM15, _beginPos);
	else 
		_newObject = WindowManager::getInstance()->createEnemyObject(ENEMY_ITEM20, _beginPos);

	_newObject->setfloorNum(mIndex);		//设置Object所在层, 地面
	mGroundPanelWidget->addChild(_newObject, 1);
	addStoreyObjectFormStoreyIndex(mIndex, _newObject);	// 添加逻辑物件

	this->EmenyMoveOnGround(_newObject);
} 


void Ui_MainBack::EmenyMoveOnGround(GameObjects *obj)//敌人在陆地上行走的运动轨迹
{
	enemyObjects *enemy=static_cast<enemyObjects *>(obj);
	enemy->setLocalZOrder(enemy->getSelfType());
	enemy->setPosition(enemy->getPositionX()-20,enemy->getPositionY());
	if (!UserRecord::getInstance()->getIsFreshMan())
	{
		this->runAction(
				Sequence::create(
				DelayTime::create(CCRANDOM_0_1()/10),
				CallFunc::create([=](){
					enemy->Run();
				}),
				NULL)
			);
	} else {
		enemy->Run();

	}
	float speedTime=enemy->getmoveSpeedScale();

	float time1=(abs(enemy->getPositionX()))/(speedTime*60);
	float h=enemy->getArmature()->getContentSize().height/2*enemy->getArmature()->getScale()-10;

	int type=enemy->getSelfType();
	if (type==ENEMY_ITEM20)//魔法王
	{
		h-=15;
	}

	if (type==ENEMY_ITEM4)//近程敌人精英
	{
		h-=10;
	}
	if (type==ENEMY_ITEM8)//铠甲精英
	{
		h-=12;
	}

	if (type==ENEMY_ITEM9||type==ENEMY_ITEM10||type==ENEMY_ITEM11)//如果是跑的快的小兵
	{
		h+=20;
	}

	if(type==ENEMY_ITEM13||type==ENEMY_ITEM14||type==ENEMY_ITEM15||type==ENEMY_ITEM16){
		h-=10;
	}

	//add Y 40 px
	auto move1=MoveTo::create(time1,Point(-5,43-h));
	PointArray * enemypoints=PointArray::create(10);
	enemypoints->addControlPoint(Point(-5, 43-h));
	enemypoints->addControlPoint(Point(87, 43-h));
	enemypoints->addControlPoint(Point(174, 40-4-h));
	enemypoints->addControlPoint(Point(261, 40-h));
	enemypoints->addControlPoint(Point(348, 48.5-h));
	enemypoints->addControlPoint(Point(435, 47.5-h));
	enemypoints->addControlPoint(Point(522, 61-h));
	enemypoints->addControlPoint(Point(609, 80-h));
	if (UserRecord::getInstance()->getSectionNum() == 1)
	{
		enemypoints->addControlPoint(Point(680, 100-h));
	} else if(UserRecord::getInstance()->getSectionNum() == 2) {
		enemypoints->addControlPoint(Point(680, 130-h));
	} else if(UserRecord::getInstance()->getSectionNum() == 3) {
		enemypoints->addControlPoint(Point(700, 140-h));
	}
	

	
	//根据坐标组创建一个轨迹 三个参数是 移动一轮的时间,坐标组,浮张力(惯性)
	ActionInterval * lineTo=CardinalSplineTo::create((696.0)/(speedTime*60), enemypoints, 0);
	
	float ft=0.25f;
	auto fd=FadeTo::create(ft,0);
	auto scaSmall=ScaleBy::create(ft,0.8,0.8);//变小
	auto spa=Spawn::create(scaSmall,fd,NULL);

	obj->setFlipY(false);
	if (!UserRecord::getInstance()->getIsFreshMan()){
		// 执行行走
		auto action = Sequence::create(move1,lineTo,CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::MoveOnGroundCallBack,this)),NULL);
		obj->runAction(action);

		// 执行提示预警箭头
		//this->runAction(Sequence::create(DelayTime::create(8.0f),CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::onMoveToRandStoreyCallBack,this, obj)),NULL));
		obj->runAction(Sequence::create(DelayTime::create(8.0f),CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::onMoveToRandStoreyCallBack,this)),NULL));
	}else{
		obj->runAction(Sequence::create(DelayTime::create(time1),CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::MoveOnGroundCallBack,this)),NULL));
	}
}

void Ui_MainBack::MoveOnGroundCallBack(Ref *obj)	//敌人在表面上行走的时候的结束回调
{
	  GameObjects *_obj=static_cast<GameObjects*>(obj);
	  int nextStoreyIndex = _obj->getNextFloorNum();
	  if (nextStoreyIndex > SCENE_STOREY_HOME)
	  {
		  // 取到指定的下一层
		  toNextStorey(SCENE_STOREY_GROUND, _obj, nextStoreyIndex);
		  _obj->setNextFloorNum(-1);
	  }else
	  {
		  // 默认下一层
		  toNextStorey(SCENE_STOREY_GROUND, _obj);
	  }
}
void Ui_MainBack::onMoveToRandStoreyCallBack(Ref* _obj)
{
	GameObjects* obj = static_cast<GameObjects*>(_obj);
	if(!obj)
		return;

	if (obj->getobjectType() == TYPE_ENEMY)
	{
		bool _isRandom = true;

		// 这几种类型不随机层
		if (obj->getSelfType() == ENEMY_ITEM4 || obj->getSelfType() == ENEMY_ITEM8 || obj->getSelfType() == ENEMY_ITEM12 || obj->getSelfType() == ENEMY_ITEM16 || obj->getSelfType() == ENEMY_ITEM20
			|| obj->getSelfType() == ENEMY_ITEM21)
		{
			_isRandom = false;
		}
		// 层数只有唯一一层，不需要随机
		int count = thisSceneData->getSceneDataMap().size() - 1;
		if (count == 1)
		{
			_isRandom = false;
		}
		if (mIsEvent105)
		{
			_isRandom = false;
		}
		
		if (_isRandom)
		{
			// 随机的
			int randValue = 0;
			while (true)
			{
				randValue = (int)(CCRANDOM_0_1()*6);
				StoreyData* _thisStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(randValue);
				if (_thisStorey && randValue != mAddYuJingStoreyIndex)
				{
					int nextStoreyIndex = _thisStorey->getStoreyIndex();
					obj->setNextFloorNum(nextStoreyIndex);
					_thisStorey->beginGamTanHaoAction();
					audioPlayer::getInstance()->playEffect("S14.mp3");
					break;
				}
			}
		}else  
		{
			// 不随机的
			StoreyData* _thisStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(obj->getfloorNum());
			if (_thisStorey)
			{
				int _nextStoreyIndex = thisSceneData->getNextStoreyIndex(obj->getfloorNum());
				StoreyData* _nextStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_nextStoreyIndex);
				if (_nextStorey)
				{
					obj->setNextFloorNum(_nextStoreyIndex);
					_nextStorey->beginGamTanHaoAction();
					audioPlayer::getInstance()->playEffect("S14.mp3");
				}
			}
		}
	}
}
void Ui_MainBack::updateGroundObjectPos(float dt)
{
	//StoreyData* _curGroundStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_GROUND);
	//if (_curGroundStoreyData)
	//{
	//	 for (GameObjects* _obj:_curGroundStoreyData->getObjectData())
	//	 {
	//		 _obj->setPositionX(_obj->getPositionX() + 1);
	//		 float _curEndX = mGroundPanelWidget->getPosition().x + mGroundPanelWidget->getSize().width;
	//		 if (_obj->getPositionX() >= _curEndX)
	//		 {
	//			 // 走到位
	//			 toNextStorey(SCENE_STOREY_GROUND, _obj);
	//		 }
	//	 }
	//}
}
/********************************

   进入到上一层

*********************************/
void Ui_MainBack::toUpStorey(GameObjects *_obj){

	GameObjects* _newObject=nullptr;

	size_t _curStoreyIndex=_obj->getfloorNum();//获取它的层
	if (thisSceneData->getUpStoreyIndex(_curStoreyIndex)>=1){
		_newObject = CopyDataToNewEnemy(static_cast<enemyObjects*>(_obj));
	}
	
	// 删除对象
	if (_curStoreyIndex == SCENE_STOREY_GROUND)
	{
		StoreyData* _curStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
		}
		mGroundPanelWidget->removeChild(_obj);
	}
	else{
		removeStoreyObjectFormStoreyIndex(_curStoreyIndex, _obj);
	}

	// 取上一层
	int _upStoreyIndex = thisSceneData->getUpStoreyIndex(_curStoreyIndex);
	if (_upStoreyIndex <= -1)
	{
		return;
	}

	if (_upStoreyIndex==SCENE_STOREY_GROUND)//如果弹到的是上一层是陆地的话
	{
		Point _beginPos;
		_beginPos.x = -20;
		_beginPos.y = 0;
		_newObject->setisStart(NO);
		_newObject->setfloorNum(SCENE_STOREY_GROUND);
		mGroundPanelWidget->addChild(_newObject, 5);
		addStoreyObjectFormStoreyIndex(_upStoreyIndex, _newObject);	// 添加逻辑物件
		_newObject->setPosition(_beginPos);
		this->EmenyMoveOnGround(_newObject);
		
	}else
	{
		// 在上一层创建
		StoreyData* _upStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_upStoreyIndex);
		if (!_upStoreyData)
			return;

		Point _beginPos;
		_beginPos.x = _upStoreyData->getLeftDoor()->getPositionX() + 13;
		_beginPos.y = _upStoreyData->getHorizon()->getPositionY();
		_newObject->setPosition(_beginPos);
		_newObject->setfloorNum(_upStoreyIndex);
		_newObject->setisStart(YES);
		_upStoreyData->mItem->addChild(_newObject, 5);
		addStoreyObjectFormStoreyIndex(_upStoreyIndex, _newObject);

		enemyObjects* enemy = static_cast<enemyObjects*>(_newObject);
		enemy->setLeftDoorPoint(_upStoreyData->getLeftDoor()->getPosition());
		enemy->setRightDoorPoint(_upStoreyData->getRightDoor()->getPosition());
		
	}


}

void Ui_MainBack::toLastStorey(GameObjects *_obj)
{
	enemyObjects* _newObject=nullptr;

	size_t _curStoreyIndex=_obj->getfloorNum();//获取它的层
	int _upStoreyIndex = thisSceneData->getUpStoreyIndex(_curStoreyIndex);
	if (_upStoreyIndex>=1){
		_newObject = CopyDataToNewEnemy(static_cast<enemyObjects*>(_obj));
		_newObject -> setMoveDirection("left");
	}else{
		return;
	}

	
	if (_upStoreyIndex == SCENE_STOREY_GROUND)
	{
		//删除对象，检测游戏是否结束
		_obj->DeathObjectCallBack(_obj, true);

		//宝箱怪成功逃跑
		if (_newObject->getSelfType() == ENEMY_ITEM21){
			UserRecord::getInstance()->setBoxEnemyDieMethod(3);
		}	
		return;
	}

	// 删除对象
	removeStoreyObjectFormStoreyIndex(_curStoreyIndex, _obj);

	// 在上一层创建
	StoreyData* _upStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_upStoreyIndex);
	if (!_upStoreyData)
		return;

	Point _beginPos;
	_beginPos.x = _upStoreyData->getRightDoor()->getPositionX() + 13;
	_beginPos.y = _upStoreyData->getHorizon()->getPositionY();
	_newObject->setPosition(_beginPos);
	_newObject->setfloorNum(_upStoreyIndex);
	_newObject->setisStart(YES);
	_upStoreyData->mItem->addChild(_newObject, 5);
	_newObject->setLeftDoorPoint(_upStoreyData->getLeftDoor()->getPosition());
	_newObject->setRightDoorPoint(_upStoreyData->getRightDoor()->getPosition());
	addStoreyObjectFormStoreyIndex(_upStoreyIndex, _newObject);

}

void Ui_MainBack::toDownNextStorey(GameObjects *_obj){                       //漏洞到下一层的
	// 复制一个Object
	
	GameObjects* _newObject=nullptr;
	Point tempPos=_obj->getPosition();
	size_t _curStoreyIndex=_obj->getfloorNum();//获取它的层
	if (thisSceneData->getUpStoreyIndex(_curStoreyIndex)>=1){
		_newObject = CopyDataToNewEnemy(static_cast<enemyObjects*>(_obj));
	}
	// 取下一层
	int _nextStoreyIndex = thisSceneData->getNextStoreyIndex(_curStoreyIndex);
	// 没有下一层进门，门扣血，播放进门特效，进门的单位视作死亡（调死亡回调）
	if (_nextStoreyIndex <= SCENE_STOREY_HOME)
	{
		StoreyData* _curStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		doorObject* door       = static_cast<doorObject*>(_curStorey->getDoor());
		reduceDoorHp(door, _obj->getSelfType());

		if (_curStorey->mItem->getChildByTag(12321)){
			auto effect        = _curStorey->mItem->getChildByTag(12321)->getChildByTag(0);
			if (effect){
				effect->runAction(Sequence::create(FadeIn::create(0.5f),FadeOut::create(0.3f),NULL));
			}
		}
		_obj->DeathObjectCallBack(_obj, true);
		return;
	}
	// 删除对象
	if (_curStoreyIndex == SCENE_STOREY_GROUND)
	{
		StoreyData* _curStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
		}
		mGroundPanelWidget->removeChild(_obj);
	}
	else{
		removeStoreyObjectFormStoreyIndex(_curStoreyIndex, _obj);
	}
	// 在下一层创建
	StoreyData* _nextStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_nextStoreyIndex);
	if (!_nextStoreyData)
		return;

	Point _beginPos;
	_beginPos.x = tempPos.x;
	_beginPos.y = _nextStoreyData->mItem->getContentSize().height;
	
	_nextStoreyData->mItem->addChild(_newObject, 5);
	_newObject->setPosition(_beginPos);
	_newObject->setfloorNum(_nextStoreyIndex);
	if (_newObject->getobjectType() == TYPE_ENEMY)
	{
		enemyObjects* enemy = static_cast<enemyObjects*>(_newObject);
		enemy->setLeftDoorPoint(_nextStoreyData->getLeftDoor()->getPosition());
		enemy->setRightDoorPoint(_nextStoreyData->getRightDoor()->getPosition());
	}
	_newObject->setisStart(NO);

	addStoreyObjectFormStoreyIndex(_nextStoreyIndex, _newObject);

	Point endPoint=Point(_beginPos.x,_nextStoreyData->getHorizon()->getPositionY());
	auto move=MoveTo::create(0.35f,endPoint);
	auto seq = Sequence::create(
		move,
		CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::toDownNextStoreyCallback,this)),
		NULL);
	_newObject->runAction(seq);
}

void Ui_MainBack::toDownNextStoreyCallback(Ref *f){
	GameObjects *Object=static_cast<GameObjects*>(f);
	Object->setisStart(YES);


	StoreyData* _nextStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(Object->getfloorNum());
	_nextStoreyData->setLineQZVisible(false);
}

// 进入到下一层
void Ui_MainBack::toNextStorey(size_t _curStoreyIndex, GameObjects* _obj, int _nextStoreyIndex)
{
	// 复制一个Object
	GameObjects* _newObject=NULL;
	int type = _obj->getSelfType();
	// // 取下一层 // 是否指定了下一层
	int _thisNextStroeyIndex = 0;
	if (_nextStoreyIndex > SCENE_STOREY_HOME)
	{
		_thisNextStroeyIndex = _nextStoreyIndex;
	}else
	{
		_thisNextStroeyIndex = thisSceneData->getNextStoreyIndex(_curStoreyIndex);
	}
	if (_thisNextStroeyIndex >= 1){
		_newObject = CopyDataToNewEnemy(static_cast<enemyObjects*>(_obj));
	}

	// 没有下一层进门，门扣血，播放进门特效，进门的单位视作死亡（调死亡回调）
	//----------------------------------------------------------------------------------------------
	//**********************************************************************************************
	if (_thisNextStroeyIndex <= SCENE_STOREY_HOME)
	{
		StoreyData* _curStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		doorObject* door       = static_cast<doorObject*>(_curStorey->getDoor());
		reduceDoorHp(door,type);

		if (_curStorey->mItem->getChildByTag(12321)){
			auto effect        = _curStorey->mItem->getChildByTag(12321)->getChildByTag(0);
			if (effect){
				effect->runAction(Sequence::create(FadeIn::create(0.5f),FadeOut::create(0.3f),NULL));
			}
		}
		
		_obj->DeathObjectCallBack(_obj, true);

		//宝箱怪成功进入门内
		if (_obj->getSelfType() == ENEMY_ITEM21){
			UserRecord::getInstance()->setBoxEnemyDieMethod(2);
		}
		return;
	}
	//**********************************************************************************************
	//----------------------------------------------------------------------------------------------

	// 删除走到位的物件
	if (_curStoreyIndex == SCENE_STOREY_GROUND)
	{
		StoreyData* _curStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
		}
		mGroundPanelWidget->removeChild(_obj);
	}
	else{
		removeStoreyObjectFormStoreyIndex(_curStoreyIndex, _obj);
	}

	// 在下一层创建
	StoreyData* _nextStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_thisNextStroeyIndex);
	if (!_nextStoreyData)
		return;
	// 在目的层创建提示感叹号
	//_nextStoreyData->beginGamTanHaoAction();
	
	Point _beginPos;
	_beginPos.x = _nextStoreyData->getLeftDoor()->getPositionX() + 13;
	_beginPos.y = _nextStoreyData->getHorizon()->getPositionY();
	//_beginPos.x = _beginPos.x + _newObject->getArmature()->getContentSize().width/2;
	_nextStoreyData->mItem->addChild(_newObject, 5);
	_newObject->setPosition(_beginPos);
	_newObject->setfloorNum(_thisNextStroeyIndex);
	if (_newObject->getobjectType() == TYPE_ENEMY)
	{
		enemyObjects* enemy = static_cast<enemyObjects*>(_newObject);
		enemy->setLeftDoorPoint(_nextStoreyData->getLeftDoor()->getPosition());
		enemy->setRightDoorPoint(_nextStoreyData->getRightDoor()->getPosition());
	}
	_newObject->setisStart(YES);

	addStoreyObjectFormStoreyIndex(_thisNextStroeyIndex, _newObject);

	//调整飞行敌人的高度
	if (_newObject->getisFly()==YES)//如果是飞行的敌人的时候
	{
		Widget* _sItemCell = _nextStoreyData->mItem;
		float y=_sItemCell->getContentSize().height/2+_newObject->getArmature()->getContentSize().height/2*0.5;
		_newObject->setPosition(_beginPos.x,y);

		if (_newObject->getSelfType()==ENEMY_ITEM16)
		{
			_newObject->setPosition(_beginPos.x,y-30);
		}
	}
}

//一个敌人进门，如果没有下一层，则扣除门的血量
void Ui_MainBack::reduceDoorHp(doorObject *door,int enemyType)
{
	do 
	{
		CC_BREAK_IF(!door);
		door->reduceHpEffect(enemyType);
	} while (0);
}

// 更新检测当前层的物件是否可以进入到下一层
void Ui_MainBack::updateStoreyObjectMoveToNext(float dt)
{
	for (int i = 0; i < thisSceneData->getSceneDataMap().size(); ++i)
	{
		StoreyData* _thisStorey = thisSceneData->getSceneDataMapStorey(i);
		if (_thisStorey)
		{
			Widget* _thisStoreyItemCell = _thisStorey->mItem;
			if (_thisStorey->getRightDoor())
			{
				Widget* _curRightDoorWidget = _thisStorey->getRightDoor();
				float _curRightDoorX = _curRightDoorWidget->getPositionX() - _curRightDoorWidget->getSize().width / 4;
				std::list<GameObjects*> lstObjectData = _thisStorey->getObjectData();
				if (!lstObjectData.empty()){
					std::list<GameObjects*>::iterator iter  = lstObjectData.begin();
					while (iter != lstObjectData.end())
					{
						GameObjects* _curObject = static_cast<GameObjects*>(*iter);
						CC_BREAK_IF(!_curObject);
						if (_curObject->getobjectType()==TYPE_ENEMY ){
							enemyObjects *_enemyObj = static_cast<enemyObjects*>(_curObject);
							CC_BREAK_IF(!_enemyObj);
							if ( _enemyObj->getPosition().x >= _curRightDoorX  && _enemyObj->getMoveDirection()=="right" )
							{
								_curObject->setPosition(_curObject->getPosition());
								toNextStorey(_thisStorey->getStoreyIndex(), _curObject);
								break;
							}
						}			
						++iter;
					}
				}
			}
		}
	}
}

//更新检测当前层的物件是否进入上一层
void Ui_MainBack::updateStoreyObjectMoveToLast(float dt)
{
	for (int i = 0; i < thisSceneData->getSceneDataMap().size(); ++i)
	{
		StoreyData* _thisStorey = thisSceneData->getSceneDataMapStorey(i);
		if (_thisStorey)
		{
			Widget* _thisStoreyItemCell = _thisStorey->mItem;
			if (_thisStorey->getLeftDoor())
			{
				Widget* _curLeftDoorWidget = _thisStorey->getLeftDoor();
				float _curLeftDoorX = _curLeftDoorWidget->getPositionX() - _curLeftDoorWidget->getSize().width / 4;
				std::list<GameObjects*> lstObjectData = _thisStorey->getObjectData();
				std::list<GameObjects*>::iterator iter  = lstObjectData.begin();
				while (iter != lstObjectData.end())
				{
					GameObjects* _curObject = static_cast<GameObjects*>(*iter);
					if (_curObject->getobjectType()==TYPE_ENEMY ){
						enemyObjects *_enemyObj = static_cast<enemyObjects*>(_curObject);
						if ( _enemyObj->getPosition().x <= _curLeftDoorX  && _enemyObj->getMoveDirection()=="left" )
						{
							toLastStorey(_curObject);
							break;
						}
					}			
					++iter;
				}
			}
		}
	}
}

enemyObjects * Ui_MainBack::CopyDataToNewEnemy(enemyObjects *emeny)
{
	enemyObjects *newEnemy=enemyObjects::create(emeny->getoFileName(),emeny->getoName(),emeny->getSelfType(),nullptr);
	if (emeny->getSelfType()==ENEMY_ITEM4||emeny->getSelfType()==ENEMY_ITEM8||emeny->getSelfType()==ENEMY_ITEM12||
		emeny->getSelfType()==ENEMY_ITEM16||emeny->getSelfType()==ENEMY_ITEM20||emeny->getSelfType()==ENEMY_ITEM21)
	{
		newEnemy->sethp(emeny->gethp());
	}
	newEnemy->setisStart(emeny->getisStart());
	newEnemy->setBatch(emeny->getBatch());
	if (emeny->getIsEvent106_Object())
	{
		newEnemy->setEnemyObjectEvent106();
	}
	//其他属性是读取文件。。。。。。。。。
	return newEnemy;
}

// 判断该逻辑物件的类型能否被添加
//战	布防限制——每一层都有布防位置数量限制，无论放置士兵，防御塔或者陷阱均会消耗位置数量。位置数量限制的上限最高与每一层能够最多放置塔或陷阱的数量持平。（最高5个位置）英雄——每层允许最多放置一个英雄，英雄不算在布防数量限制内

int Ui_MainBack::isAddStoreyObjectType(size_t _storeyIndex, GameObjects* _obj)
{
	int _curObjectType = _obj->getobjectType();		// 当前需要创建的obj类型
	int _curObjectCount = 0;						// 当前层内的obj数量
	int _result = CAN_ADD_OBJECT_POS;				// 默认当前可以创建

	// 如果当前层是Home老巢。直接退出
	if ( _storeyIndex == SCENE_STOREY_HOME)
	{
		_result = NOT_ADD_OBJECT_HOME;
		return _result;
	}
	auto _curStoreyData  = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	// 找不到层数据，直接退出
	if (!_curStoreyData || !_curStoreyData->mItem)
	{
		_result = NOT_ADD_OBJECT_STOREY;
		return _result;
	}
	// 当前层obj数量限制
	for (Ref *p:_curStoreyData->getObjectData())//计算有多少个单位了
	{
		GameObjects *sp=static_cast<GameObjects*>(p);

		if (sp->gethp()>0&&sp!=nullptr)
		{
			if ((sp->getobjectType()==TYPE_WARRIOR||sp->getobjectType()==TYPE_TRAP||sp->getobjectType()==TYPE_TOWER)&&(uintptr_t)sp!=(uintptr_t)_obj)//如果不是英雄的话
			{
				_curObjectCount++;
			}
		}
	}

	if (_curObjectType!=TYPE_BOSS && _curObjectCount>=STOREY_MAX_OBJECT)
	{
		_result = NOT_ADD_OBJECT_PK_FULL;
		return _result;
	}

	// 如果是创建Boss，检查是否存在boss
	for (Ref *p:_curStoreyData->getObjectData())//计算是否有BOSS
	{
		GameObjects *sp=static_cast<GameObjects*>(p);

		if (sp->gethp()>0&&sp!=nullptr)
		{
			if (_curObjectType==TYPE_BOSS)//如果传进来的是BOSS
			{
				if (sp->getobjectType()==TYPE_BOSS)//如果这一层已经存在BOOSS了
				{
					_result = NOT_ADD_OBJECT_HERO_FULL;
					return _result;
					break;
				}
			}
		}
	}


	// 如果是创建恐吓的狗的话，检查是否存在有恐吓狗了，每层只能放一个狗
	for (Ref *p:_curStoreyData->getObjectData())//计算是否有狗
	{
		GameObjects *sp=static_cast<GameObjects*>(p);

		if (sp->gethp()>0&&sp!=nullptr)
		{
			if (_curObjectType==TYPE_WARRIOR&&_obj->getSelfType()==WAR_ITEM5)//如果传进来的是狗的话
			{
				if (sp->getobjectType()==TYPE_WARRIOR&&sp->getSelfType()==WAR_ITEM5)//如果这一层已经存在狗
				{
					_result = NOT_ADD_OBJECT_POS;

					return _result;
					break;
				}
			}
		}
	}






	// 计算格子位置，是否占用                              
	for (Ref *p:_curStoreyData->getObjectData())//计算是否占格子了
	{
		GameObjects *sp=static_cast<GameObjects*>(p);

		if (sp->gethp()>0&&sp!=nullptr&&(uintptr_t)sp!=(uintptr_t)_obj)
		{
			if (_curObjectType==TYPE_TOWER||_curObjectType==TYPE_TRAP) {//如果传进来的是塔或者陷阱的话,英雄是不占格子的

				if (sp->getlocationIndx()==_obj->getlocationIndx()&&(sp->getobjectType()==TYPE_TOWER||sp->getobjectType()==TYPE_TRAP))
				{
					_result = NOT_ADD_OBJECT_POS;
					return _result;
					break;
				}
			}
		}

	}



	// 判断是否教学模式
	if (UserRecord::getInstance()->getIsFreshMan())
	{
		if (_obj->getlocationIndx()!=getAnEmptyIndex())
		{
			_result = NOT_ADD_OBJECT_POS;
			return _result;
		}
	}
	// 返回默认，可以创建
	return _result;
}




// 更新鼠标点击动画
void Ui_MainBack::resetTouchAll(bool _isTouch)
{
	if (_isTouch)
	{
		auto _curAllChild = mMainWidget->getChildren();
		for (auto _child : _curAllChild)
		{
			Widget* _childWidget = static_cast<Widget*>(_child);
			if (_childWidget)
			{
				_childWidget->setTouchEnabled(_isTouch);
			}
		}


		for (int i = 1; i <= mListViewWidget->getItems().size(); ++i)
		{
			auto _curItem = mListViewWidget->getItem(i);
			if (_curItem)
			{
				auto _allChilds = _curItem->getChildren();
				for (auto _child : _allChilds)
				{
					Widget* _childWidget = static_cast<Widget*>(_child);
					if (_childWidget)
					{
						_childWidget->setTouchEnabled(_isTouch);
					}
				}
			}
		}
	}
}

void Ui_MainBack::fitScreen()
{
	Point originPt   = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	mMainWidget->setPosition(Point(originPt.x ,originPt.y));

	if (visibleSize.height/640>1){
		ImageView *bg = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget,"Image_Sky"));
		bg->setScaleY(visibleSize.height/640*bg->getScaleY());
		bg->setPositionY(bg->getPositionY()+(visibleSize.height-640)/2);
		mScrollViewWidget->setSize(Size(visibleSize.width,visibleSize.height));
	}
}

void Ui_MainBack::updateCenterStorey(int _storeyIndex)
{
	auto _curStoeyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	if (_curStoeyData)
	{
		auto _curItem = _curStoeyData->mItem;
		if (_curItem)
		{
			Point _worldPoint = mScrollViewWidget->convertToNodeSpace(_curItem->getPosition());
			Point _midPos  = Point(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
			float posY = _curItem->getPosition().y + _curItem->getSize().height/2 - _midPos.y;
			float minY = mScrollViewWidget->getSize().height - mScrollViewWidget->getInnerContainerSize().height;
			float h = - minY;
			float _percent = 100*(-posY -minY)/h;
			mScrollViewWidget->scrollToPercentVertical(_percent, 1, true);
		}
	}
}

void Ui_MainBack::setWidgetGray(Widget* _widget, bool _isGray)
{
	if (_widget)
	{
		if (_isGray)
		{
			_widget->setGrey();
		}else
		{
			_widget->removeGrey();
		}
	}
}

// 处理事件2 免费开层
void Ui_MainBack::doEvent2(int _batch)
{
	UserRecord* user = UserRecord::getInstance();
	int _nowRound = _batch;//user->getRound();
	if (_nowRound != user->getMaxRound())
	{
		__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
		if(!data)
			return;
		Value currentSection(user->getSectionNum());
		Value currentScene(user->getSceneNum());
		__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
		if(!curr_section)
			return;
		__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
		if(!allScene)
			return;
		__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
		if(!curr_scene)
			return;
		__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
		if(!monster)
			return;
		Value currentRound(_nowRound);
		__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
		if(!round)
			return;
		__Dictionary* enemys = dynamic_cast<__Dictionary*>(round->objectForKey("enemy"));
		if(!enemys)
			return;
		
		// 检查是否有事件
		__Dictionary* events = dynamic_cast<__Dictionary*>(round->objectForKey("event"));
		if (events)
		{
			for (int i = 1 ; i <= events->count(); ++i)
			{
				Value _eventKey(i);
				auto _curObject = events->objectForKey(_eventKey.asString());
				if (_curObject)
				{
					// 取事件id
					__String* _objectString = dynamic_cast<__String*>(_curObject);
					int _eventId = _objectString->intValue();
					if (_eventId == 102)
					{
						// 如果事件102 免费开层
						auto _eventPlist = PlistManager::getInstance()->getPlistMap("EventData");
						if (_eventPlist)
						{
							auto _idString = StringUtils::format("%d", _eventId);
							auto _idPlist = PlistManager::getInstance()->getPlist(_eventPlist, _idString.c_str());
							if (_idPlist)
							{
								// 取免费开几层
								auto _count = PlistManager::getInstance()->getPlistCString(_idPlist, "count").intValue();
								if (_count > 0)
								{								
									if (mAddStoreyNum <= 0)
									{
										// 要开的层数<=0表示，当前没有在开层状态，可以立刻进行开层
										mAddStoreyNum = mAddStoreyNum + _count;

										//灰掉按钮
										Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
										mainLayer->setPauseBtnGray();

										// 执行免费开层
										auto delay=DelayTime::create(0.9f);
										auto ddlay = Sequence::create(
											delay,
											CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::doEventCallBack, this)),
											NULL);
										this->runAction(ddlay);
									}else
									{
										// 要开的层数>0表示，当前在开层状态，把要开的层次数累加后，等待开层完毕后继续开层
										mAddStoreyNum = mAddStoreyNum + _count;
									}
								}
							}
						}
						
					}
				}
			}
		}
	}
}
void  Ui_MainBack::doEventCallBack(Ref* sender)
{
	dragObjectFinish(nullptr);
	doAddStorey(false);
}


GameObjects * Ui_MainBack::CreateMagicObjets(int stype){

	magicObjects*magic=nullptr;
	if (stype==MAGIC_TRANSMIT)//传送的魔法技能
	{
		magic=magicObjects::create("td_Magic3.ExportJson","td_Magic3",stype);
	}
	if (stype==MAGIC_SHEEP)
	{
	}

	if (stype==MAGIC_ATK)
	{
		magic=magicObjects::create("td_Magic1.ExportJson","td_Magic1",stype);
	}
	if (stype==MAGIC_GODZILLA)
	{
		magic=magicObjects::create("td_Magic4.ExportJson","td_Magic4",stype);
	}

	if (stype==MAGIC_BOSS5SKILL)
	{
		magic=magicObjects::create("td_boss_05_jingling.ExportJson","td_boss_05_jingling",stype);
	}


	return magic;
}
void Ui_MainBack::CreateMagicTransmit(int stype,Point pos,int floor){ //第一个参数是魔法技能的那种类型 第二是摆放的位置,第三个是表示第几层上面的
	
	magicObjects *magic1=static_cast<magicObjects*>(this->CreateMagicObjets(stype));//这个是被吸进去的
	magicObjects *magic2=static_cast<magicObjects*>(this->CreateMagicObjets(stype));//这个是出来的
	
	StoreyData* _StoreyData1 = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(floor);
	StoreyData* _StoreyData2 = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_GROUND);//这个是放在陆地上的
	float _y = _StoreyData1->getHorizon()->getPositionY();
	magic1->setPosition(pos.x,_y+25+20);
	magic2->setPosition(140, 40);
	magic1->setfloorNum(floor);//设置为可以当前的层数
	
   magic2->setfloorNum(SCENE_STOREY_GROUND);//设置为可以当前的层数
   magic2->setisStart(YES);

	magic1->setotherType(TRANSMIT_IN);
	magic2->setotherType(TRANSMIT_OUT);

	_StoreyData1->mItem->addChild(magic1, 4);
	_StoreyData2->mItem->addChild(magic2,0);
	addStoreyObjectFormStoreyIndex(floor, magic1);
	addStoreyObjectFormStoreyIndex(SCENE_STOREY_GROUND, magic2);
	magic1->playTransmitIn();
	magic2->playTransmitOut();
}
void Ui_MainBack::magicTransmitUpToGround(GameObjects *_obj){//传送到陆地上去
	GameObjects* _newObject=nullptr;
	size_t _curStoreyIndex=_obj->getfloorNum();//获取它的层
	if (thisSceneData->getUpStoreyIndex(_curStoreyIndex)>=1){
		_newObject = CopyDataToNewEnemy(static_cast<enemyObjects*>(_obj));
	}
	// 删除对象
	if (_curStoreyIndex == SCENE_STOREY_GROUND)
	{
		StoreyData* _curStorey = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
		}
		mGroundPanelWidget->removeChild(_obj);
	}
	else{
		removeStoreyObjectFormStoreyIndex(_curStoreyIndex, _obj);
	}
	// 传送到的是在陆地
	int _upStoreyIndex =SCENE_STOREY_GROUND;
	
	if (_upStoreyIndex==SCENE_STOREY_GROUND)//如果弹到的是上一层是陆地的话
	{
		Point _beginPos;
		_beginPos.x = 160;
		_beginPos.y = 50-35;

		mGroundPanelWidget->addChild(_newObject, 1);
		addStoreyObjectFormStoreyIndex(_upStoreyIndex, _newObject);	// 添加逻辑物件
		_newObject->setPosition(_beginPos);
		_newObject->setisStart(NO);
		_newObject->setfloorNum(SCENE_STOREY_GROUND);
		_newObject->setOpacity(0);
		//_newObject->getArmature()->setOpacity(80);


	    _newObject->setScale(0.6);
		float f=0.25f;
		auto fade=FadeIn::create(f);
		auto sca=ScaleTo::create(f,1.0);
		auto spa=Spawn::create(fade,sca,NULL);
		auto action = Sequence::create(
			spa,
			CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::magicTransmitMoveOnGround,this)),
			NULL);
		_newObject->runAction(action);

// 		auto fade=FadeIn::create(0.3);
// 		_newObject->getArmature()->runAction(fade);
		//this->magicTransmitMoveOnGround(_newObject);
	}
}
void Ui_MainBack::magicTransmitMoveOnGround(Ref *obj){//新的位置的移动
	GameObjects *_obj=static_cast<GameObjects*>(obj);
	enemyObjects *enemy=static_cast<enemyObjects *>(_obj);
	enemy->Run();
	float speedTime=enemy->getmoveSpeedScale();
	float time1=(abs(enemy->getPositionX()))/(speedTime*60);
	float h=enemy->getArmature()->getContentSize().height/2*enemy->getArmature()->getScale()-10;

	int type=enemy->getSelfType();
	if (type==ENEMY_ITEM20)//魔法王
	{
		h-=15;
	}
	if (type==ENEMY_ITEM4)//近程敌人精英
	{
		h-=15;
	}
	if (type==ENEMY_ITEM8)//铠甲精英
	{
		h-=12;
	}
	if (type==ENEMY_ITEM9||type==ENEMY_ITEM10||type==ENEMY_ITEM11)//如果是跑的快的小兵
	{
		h+=20;
	}
	if(type==ENEMY_ITEM13||type==ENEMY_ITEM14||type==ENEMY_ITEM15||type==ENEMY_ITEM16){
		h-=10;
	}

	//add Y 40 px
	auto move1=MoveTo::create(time1,Point(-5,43-h));
	PointArray * enemypoints=PointArray::create(8);
	enemypoints->addControlPoint(ccp(194, 40-4-h));
	enemypoints->addControlPoint(ccp(261, 40-h));
	enemypoints->addControlPoint(ccp(348, 48.5-h));
	enemypoints->addControlPoint(ccp(435, 47.5-h));
	enemypoints->addControlPoint(ccp(522, 61-h));
	enemypoints->addControlPoint(ccp(609, 80-h));
	if (UserRecord::getInstance()->getSectionNum() == 1)
	{
		enemypoints->addControlPoint(ccp(680, 100-h));
	} else if(UserRecord::getInstance()->getSectionNum() == 2) {
		enemypoints->addControlPoint(ccp(680, 130-h));
	}else if(UserRecord::getInstance()->getSectionNum() == 3) {
		enemypoints->addControlPoint(ccp(700, 140-h));
	}



	//根据坐标组创建一个轨迹 三个参数是 移动一轮的时间,坐标组,浮张力(惯性)
	ActionInterval * lineTo=CardinalSplineTo::create((696-194.0)/(speedTime*60), enemypoints, 0);

	float ft=0.25f;
	auto fd=FadeTo::create(ft,0);
	auto scaSmall=ScaleBy::create(ft,0.8,0.8);//变小
	auto spa=Spawn::create(scaSmall,fd,NULL);
	
   auto action = Sequence::create(lineTo,CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::MoveOnGroundCallBack,this)),NULL);
		//执行这个行动 CCRepeatForever是一直执行
    enemy->runAction(action);
	
}
void Ui_MainBack::eventScrollView(Ref* sender, ScrollviewEventType event)
{
	// 发生Scroll View滚动事件
	if (event == SCROLLVIEW_EVENT_SCROLLING)
	{
		updateYuJing();
	}
}
// 更新预警提示
void Ui_MainBack::updateYuJing()
{
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	bool _isYuJing = thisSceneData->isYuJing();
	if (mainLayer)
	{
		int _upCount = 0;
		int _downCount = 0;
		for (int i = 0; i < thisSceneData->getSceneDataMap().size(); ++i)
		{
			StoreyData* thisStoreyData = thisSceneData->getSceneDataMapStorey(i);
			Widget* thisItemCell = thisStoreyData->mItem;
			Rect _screenRect;
			_screenRect.origin.x = Director::getInstance()->getVisibleOrigin().x;
			_screenRect.origin.y = Director::getInstance()->getVisibleOrigin().y;
			_screenRect.size.width= Director::getInstance()->getVisibleSize().width;
			_screenRect.size.height = Director::getInstance()->getVisibleSize().height;

			if (thisStoreyData->isYuJing())
			{
				float _disHeight = (thisItemCell->getSize().height * thisItemCell->getScale());
				// 屏外  上
				if (thisItemCell->getWorldPosition().y + 102  > _screenRect.origin.y + _screenRect.size.height)
				{
					//CCLOG("up index: %d", thisStoreyData->getStoreyIndex());
					_upCount = _upCount + 1;
				}
				// 下
				if (thisItemCell->getWorldPosition().y + _disHeight - 102 < _screenRect.origin.y)
				{
					//CCLOG("down index: %d", thisStoreyData->getStoreyIndex());
					mainLayer->updateYuJing(true, false);	
					_downCount = _downCount + 1;
				}
			}
		}
		mainLayer->updateYuJing(false);
		if (_upCount > 0)
		{
			mainLayer->updateYuJing(true, true);
		}
		if (_downCount > 0)
		{
			mainLayer->updateYuJing(true, false);
		}
	}
}

//变羊技能的。。
void Ui_MainBack::magicChangeToSheep(Point  pos,int floornum){
	StoreyData* _storeydata = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(floornum);

	float keepTime=8.0;//起始是8.0秒
	int Num=5; //初始化变羊的数目
	float sdistance=200;

	//int _curItem1SelectIndex = PlistManager::getInstance()->getUpgradesSelect("magic", "item2");
	//if (_curItem1SelectIndex==1)//增加3秒
	//{
	//	keepTime+=3.0;
	//}
	//if (_curItem1SelectIndex==2||_curItem1SelectIndex==3)
	//{
	//	keepTime+=3.0;
	//	Num+=2;
	//	sdistance=200;
	//}
	//
	//if (_curItem1SelectIndex==4)
	//{
	//	keepTime+=5.0+3.0;
	//	Num+=2;
	//	sdistance=200;
	//}
	//if (_curItem1SelectIndex==5)
	//{
	//	keepTime+=5.0+3.0;
	//	Num+=(2+2);
	//	sdistance=350;
	//}

	for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_ENEMY&&obj->gethp()>0&&Num>0&&obj->getisFly()==NO&&obj->getisIntimidate()==NO)
		{
			float dist=fabs(obj->getPositionX()-pos.x);
			if (dist<200)
			{
				enemyObjects*enemy=static_cast<enemyObjects*>(obj);
				enemy->changeToSheep(keepTime);
				Num--;
			}
		}
	}
	

}

 void Ui_MainBack::CreateMagicAtk(int stype,Point pos,int floor){//第一个参数是魔法技能的那种类型 第二是摆放的位置,第三个是表示第几层上面的

	 magicObjects *magic=static_cast<magicObjects*>(this->CreateMagicObjets(stype));
	
	 StoreyData* _StoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(floor);
	
	 float _y = _StoreyData->getHorizon()->getPositionY();
	 magic->setPosition(pos.x,_y);
	 magic->setfloorNum(floor);//设置为可以当前的层数
	 magic->setisStart(YES);
	 magic->setTag(88888);
	 _StoreyData->mItem->addChild(magic, 5);

	 addStoreyObjectFormStoreyIndex(floor, magic);

	 
	 //判断摆放区域的
	Widget* _swg=_StoreyData->getMovementWidget();//获取活动区域
	float moveWidthCenterX=_swg->getPositionX();
	float leftX=moveWidthCenterX-_swg->getSize().width/2+100;
	float rightX=moveWidthCenterX+_swg->getSize().width/2-100;
	
	if (pos.x<leftX)
	{
		 magic->setPosition(leftX,_y);
	}
	else if (pos.x>rightX)
	{
		 magic->setPosition(rightX,_y);
	}
	else{
		 magic->setPosition(pos.x,_y);
	}
	
 }


 //变成哥斯拉的魔法技能
 void Ui_MainBack::CreateMagicGosla(int stype,Point pos,int floor){//第一个参数是魔法技能的那种类型 第二是摆放的位置,第三个是表示第几层上面的

	 magicObjects *magic=static_cast<magicObjects*>(this->CreateMagicObjets(stype));

	 StoreyData* _StoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(floor);

	 float _y = _StoreyData->getHorizon()->getPositionY();
	 magic->setPosition(pos.x,_y);
	 magic->setfloorNum(floor);//设置为可以当前的层数
	 magic->setisStart(YES);
	 _StoreyData->mItem->addChild(magic,6);
	 addStoreyObjectFormStoreyIndex(floor, magic);
	
	 magic->dragObjectSmoke();//显示烟雾
	 //判断摆放区域的
	 Widget* _swg=_StoreyData->getMovementWidget();//获取活动区域
	 float moveWidthCenterX=_swg->getPositionX();
	 float leftX=moveWidthCenterX-_swg->getSize().width/2+150;
	 float rightX=moveWidthCenterX+_swg->getSize().width/2-150;

	 if (pos.x<leftX)
	 {
		 magic->setPosition(leftX,_y);
	 }
	 else if (pos.x>rightX)
	 {
		 magic->setPosition(rightX,_y);
	 }
	 else{
		 magic->setPosition(pos.x,_y);
	 }
 }

 // 自动创建唯一一个英雄。否则打开英雄选择界面
 void Ui_MainBack::createFirstHero(float _dt)
 {
	 // 如果是教学模式，那么退出
	 bool isFreshMan = UserRecord::getInstance()->getIsFreshMan();
	 if (isFreshMan)
	 {
		 WindowManager::getInstance()->setSwallowAllUi(false);
		 return;
	 }
	 int _unLockCount = 0;	// 解锁的数量
	 int _unLockId = 0;		// 解锁的id
	 int _unLockLevel = 0;	// 解锁的level
	 // 从本地列表中取出所有的英雄
	 auto _curPlist = PlistManager::getInstance()->getPlistMap("BossData");
	 auto _curArray = _curPlist->allKeys();
	 for (size_t i=0; i!=_curPlist->count(); ++i)
	 {
		 int _curTypeIndex = i + 1;
		 auto _curName = StringUtils::format("boss%d", _curTypeIndex);
		 auto _curNameObject = PlistManager::getInstance()->getPlistObject(_curPlist, _curName.c_str());
		 if (!_curNameObject)
			break;
		auto _haveData = UserRecord::getInstance()->getHeroData(_curTypeIndex);
		if (_haveData.id > 0)
		{
			if (_haveData.unLock == YES)	// 是否解锁
			{
				_unLockCount = _unLockCount + 1;
				_unLockId = _curTypeIndex;
				_unLockLevel = _haveData.level;
			}
		}
	 }
	 auto _lastStoreyIndex = thisSceneData->getLastStoreyIndex();
	 //CCLOG("----------createFirstHero--------_lastStoreyIndex :%d----------------------------", _lastStoreyIndex);
	 mScrollViewWidget->scrollToPercentVertical(80, 1, true);
	 WindowManager::getInstance()->setSwallowAllUi(false);
	 // 如果只有一个英雄，那么创建它自己, 且创建不扣钱
	 if (_unLockCount == 1)
	 {
		createHeroToStorey(_lastStoreyIndex, _unLockId, Point(0,0), _unLockLevel);
		doEvent103();
	 }
	 // 如果有多个英雄，那么打开英雄界面，给玩家选择，且创建不扣钱
	 else if (_unLockCount > 1)
	 {
		 thisWindowManager->createHeroSelect(_lastStoreyIndex, true, true);
	 }
	 
 }
 // 处理事件3, 限制手动开层
 void Ui_MainBack::doEvent103()
 {
	 UserRecord* user = UserRecord::getInstance();
	 int _nowRound = user->getRound();
	 __Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	 if(!data)
		 return;
	 Value currentSection(user->getSectionNum());
	 Value currentScene(user->getSceneNum());
	 __Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	 if(!curr_section)
		 return;
	 __Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	 if(!allScene)
		 return;
	 __Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	 if(!curr_scene)
		 return;
	 __Dictionary* events = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("event"));
	 if(!events)
		 return;

	 for (int i = 1 ; i <= events->count(); ++i)
	 {
		 Value _eventKey(i);
		 auto _curObject = events->objectForKey(_eventKey.asString());
		 if (_curObject)
		 {
			 // 取事件id
			 __String* _objectString = dynamic_cast<__String*>(_curObject);
			 int _eventId = _objectString->intValue();
			 if (_eventId == 103)
			 {
				 // 如果事件103 限制手动开层
				 auto _homeStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME);
				 if (_homeStoreyData)
				 {
					 //_homeStoreyData->setIsProAddStorey(true);
					 _homeStoreyData->doEvent103Action();
				 }	 
			 }
		 }
	 }
 }

  void Ui_MainBack::createJinglingAttribute(GameObjects *_obj,GameObjects *jing){

	  magicObjects *jingling=static_cast<magicObjects*>(jing);
	  jingling->setfloorNum(_obj->getfloorNum());//设置为可以当前的层数
	  jingling->setmaxHp(_obj->getskill1_callMonster_Hp());
	  jingling->sethp(_obj->getskill1_callMonster_Hp());
	  jingling->setdfe(_obj->getskill1_callMonster_dfe());
	  jingling->setdfeOption(_obj->getskill1_callMonster_dfeOption());
	  jingling->setatk(_obj->getskill1_callMonster_atk());
	  jingling->setatkLen(40.0);
	  jingling->delayKeepHere(_obj->getskill1_skillKeepTime());
	  jingling->setmoveSpeedScale(2.0);

	  jingling->setitsCallObjectAddr((uintptr_t)(_obj));
  }
 void Ui_MainBack::CreateBoss5jingling(GameObjects *_obj){

	 int objlevel=_obj->getbossSkillOneLevel();
	  magicObjects *jingling1=nullptr;
	  magicObjects *jingling2=nullptr;
	  magicObjects *jingling3=nullptr;
	    StoreyData* _StoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_obj->getfloorNum());
	  if (objlevel<= bossSkillOne_Level_1)
	  {
		   jingling1=magicObjects::create("td_boss_05_jingling1.ExportJson","td_boss_05_jingling1",MAGIC_BOSS5SKILL);
		   Point pos; 
		   pos=Point(_obj->getPositionX()-60,0); 
		   jingling1->setFatherPos(Point(60,0));
		   jingling1->setPosition(pos);
		   _StoreyData->mItem->addChild(jingling1,6);
		   addStoreyObjectFormStoreyIndex(_obj->getfloorNum(), jingling1);
		   this->createJinglingAttribute(_obj,jingling1);

	  }else if (objlevel> bossSkillOne_Level_1&&objlevel<= bossSkillOne_Level_2)
	  {
		   jingling1=magicObjects::create("td_boss_05_jingling1.ExportJson","td_boss_05_jingling1",MAGIC_BOSS5SKILL);
		   jingling2=magicObjects::create("td_boss_05_jingling2.ExportJson","td_boss_05_jingling2",MAGIC_BOSS5SKILL);

		   jingling1->setPosition(Point(_obj->getPositionX()-60,0));
		   jingling1->setFatherPos(Point(60,0));
		   _StoreyData->mItem->addChild(jingling1,6);
		   addStoreyObjectFormStoreyIndex(_obj->getfloorNum(), jingling1);
		   this->createJinglingAttribute(_obj,jingling1);

		   jingling2->setPosition(Point(_obj->getPositionX()-90,0));
		   jingling2->setFatherPos(Point(90,0));
		   _StoreyData->mItem->addChild(jingling2,6);
		   addStoreyObjectFormStoreyIndex(_obj->getfloorNum(), jingling2);
		   this->createJinglingAttribute(_obj,jingling2);



	  }else if (objlevel> bossSkillOne_Level_2)
	  {
		  jingling1=magicObjects::create("td_boss_05_jingling1.ExportJson","td_boss_05_jingling1",MAGIC_BOSS5SKILL);
		  jingling2=magicObjects::create("td_boss_05_jingling2.ExportJson","td_boss_05_jingling2",MAGIC_BOSS5SKILL);
		  jingling3=magicObjects::create("td_boss_05_jingling3.ExportJson","td_boss_05_jingling3",MAGIC_BOSS5SKILL);

		  jingling1->setPosition(Point(_obj->getPositionX()-60,0));
		    jingling1->setFatherPos(Point(60,0));
		  _StoreyData->mItem->addChild(jingling1,6);
		  addStoreyObjectFormStoreyIndex(_obj->getfloorNum(), jingling1);
		  this->createJinglingAttribute(_obj,jingling1);

		  jingling2->setPosition(Point(_obj->getPositionX()-90,0));
		    jingling2->setFatherPos(Point(90,0));
		  _StoreyData->mItem->addChild(jingling2,6);
		  addStoreyObjectFormStoreyIndex(_obj->getfloorNum(), jingling2);
		  this->createJinglingAttribute(_obj,jingling2);

		  jingling3->setPosition(Point(_obj->getPositionX()-120,0));
		  jingling3->setFatherPos(Point(120,0));
		  _StoreyData->mItem->addChild(jingling3,6);
		  addStoreyObjectFormStoreyIndex(_obj->getfloorNum(), jingling3);
		  this->createJinglingAttribute(_obj,jingling3);

	  }
	

  }

void Ui_MainBack::initAnimation()
{
	if (UserRecord::getInstance()->getSectionNum() == 1)
	{

	} else if(UserRecord::getInstance()->getSectionNum() == 2) {
		//Image_54
		//updown_img
		auto Image_54 = static_cast<ImageView*>(findChild("Image_54"));
		auto updown_img = static_cast<ImageView*>(findChild("updown_img"));

		auto move = MoveTo::create(2.0f, updown_img->getPosition()+Point(0,15));
		auto move_back = MoveTo::create(2.0f, updown_img->getPosition());
		auto seq1 = Sequence::create(move, move_back, NULL);
		auto repeat1 = RepeatForever::create( seq1 );
		updown_img->runAction(repeat1);


		auto move1 = MoveTo::create(2.2f, Image_54->getPosition()+Point(0,15));
		auto move_back1 = MoveTo::create(2.2f, Image_54->getPosition()-Point(0,2));
		auto seq2 = Sequence::create(move1, move_back1, NULL);
		auto repeat2 = RepeatForever::create( seq2 );
		Image_54->runAction(repeat2);
	} else if(UserRecord::getInstance()->getSectionNum() == 3) {
		auto Image_54 = static_cast<ImageView*>(findChild("Image_54"));
		auto move1 = MoveTo::create(2.2f, Image_54->getPosition()+Point(0,15));
		auto move_back1 = MoveTo::create(2.2f, Image_54->getPosition()-Point(0,2));
		auto seq2 = Sequence::create(move1, move_back1, NULL);
		auto repeat2 = RepeatForever::create( seq2 );
		Image_54->runAction(repeat2);

		auto Image_52  = static_cast<ImageView*>(findChild("Image_52"));
		Image_52->setAnchorPoint(Point(1,0));
		marqueeCloud(Image_52);
	}
	auto towerfeng = static_cast<ImageView*>(findChild("tower_leaf"));
	auto repeat = RepeatForever::create( RotateBy::create(15.0f, 360) );
	towerfeng->runAction(repeat);
	auto ImageView_cloud_1  = static_cast<ImageView*>(findChild("ImageView_cloud_1"));
	auto ImageView_cloud_2  = static_cast<ImageView*>(findChild("ImageView_cloud_2"));
	auto ImageView_cloud_3  = static_cast<ImageView*>(findChild("ImageView_cloud_3"));
	auto ImageView_cloud_4	= static_cast<ImageView*>(findChild("ImageView_cloud_4"));
	marqueeCloud(ImageView_cloud_1);
	marqueeCloud(ImageView_cloud_2);
	marqueeCloud(ImageView_cloud_3);
	marqueeCloud(ImageView_cloud_4);


}
void Ui_MainBack::marqueeCloud(ImageView* cloud)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Point VisibleOrigin =  Director::getInstance()->getVisibleOrigin();
	float currx = cloud->getPositionX();
	float maxRight = VisibleOrigin.x+visibleSize.width+cloud->getSize().width;
	float dist = maxRight-currx;
	auto action = Sequence::create(
		MoveTo::create(dist/30,Point(maxRight,cloud->getPositionY())),
		CallFunc::create([=](){
			cloud->setPositionX(VisibleOrigin.x);
			marqueeCloud(cloud);
		}),
		NULL);
	cloud->runAction(action);

}
// 更新开层消耗
void Ui_MainBack::updateAddStoreyCost()
{
	// 取基础消耗值
	auto _costSweet = PlistManager::getInstance()->getConfigTypeCString("base","openRound").intValue();
	if (_costSweet > 0)
	{
		mAddStoreyCost = _costSweet;
	}
	//// 计算科技树改变值
	//float upgradeValue = UserRecord::getInstance()->getUpgradesItemAllAddValue("tech","item5");
	//mAddStoreyCost*= (1 - upgradeValue);
	// 将消耗值更新到界面显示
	auto _homeStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME);
	if (_homeStoreyData)
	{
		_homeStoreyData->updateAddStoreyCostView(mAddStoreyCost);
	}
}
// 处理事件5, 限制怪物随机层
void Ui_MainBack::doEvent105()
{
	UserRecord* user = UserRecord::getInstance();
	int _nowRound = user->getRound();
	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	if(!data)
		return;
	Value currentSection(user->getSectionNum());
	Value currentScene(user->getSceneNum());
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	if(!curr_section)
		return;
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	if(!allScene)
		return;
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	if(!curr_scene)
		return;
	__Dictionary* events = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("event"));
	if(!events)
		return;

	for (int i = 1 ; i <= events->count(); ++i)
	{
		Value _eventKey(i);
		auto _curObject = events->objectForKey(_eventKey.asString());
		if (_curObject)
		{
			// 取事件id
			__String* _objectString = dynamic_cast<__String*>(_curObject);
			int _eventId = _objectString->intValue();
			if (_eventId == 105)
			{
				// 如果事件105 限制怪物随机入层
				auto _homeStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME);
				if (_homeStoreyData)
				{
					mIsEvent105 = true;
				}	 
			}
		}
	}
}

// 创建魔法释放监听
void Ui_MainBack::createCreateObjListenerTouch()
{
	if (!_createObjListener)
	{
		_createObjListener = EventListenerTouchOneByOne::create();
		_createObjListener->setSwallowTouches(true); 
		_createObjListener->onTouchBegan		= CC_CALLBACK_2(Ui_MainBack::onCreateObjTouchBegan, this);
		_createObjListener->onTouchMoved		= CC_CALLBACK_2(Ui_MainBack::onCreateObjTouchMove, this);  
		_createObjListener->onTouchEnded		= CC_CALLBACK_2(Ui_MainBack::onCreateObjTouchEnd, this);  
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_createObjListener, TOUCH_CREATE_OBJECT); 

		//CCLOG(" create Listener ..... ");
	}
	return;
}
// 删除魔法释放监听
void Ui_MainBack::removeCreateObjListenerTouch()
{
	if (_createObjListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_createObjListener);
		_createObjListener = nullptr;

		//CCLOG(" remove Listener ..... ");
	}
}

// 魔法释放监听，点击到某一层
bool Ui_MainBack::onCreateObjTouchBegan(Touch* _touch, Event* _unusedEvent)  
{  
	// CCLOG("Ui_MainBack:: move............");
	Point _touchPos		= _touch->getLocation();
	// 是否移动出安全范围
	Point _touchStarPos =  getScreenPos(mMouseClickDragBeginPos);
	_touchStarPos.y = 0;
	Point _touchMovePos =  getScreenPos(_touchPos);
	_touchMovePos.y = _touchMovePos.y + mScrollViewWidget->getInnerContainer()->getPositionY();
	// 移出屏幕准备栏的高度
	if (abs(_touchMovePos.y - _touchStarPos.y) > 110)
	{
	}else
	{
		dragObjectFinish(nullptr);
		return true;
	}
	updateCreateMouseClickDragObject(_touchPos);
	return true;
} 

void Ui_MainBack::onCreateObjTouchMove(Touch* _touch, Event* _unusedEvent)
{
	// CCLOG("Ui_MainBack:: move............");
	Point _touchPos		= _touch->getLocation();

	// 是否移动出安全范围
	Point _touchStarPos =  getScreenPos(mMouseClickDragBeginPos);
	_touchStarPos.y = 0;
	Point _touchMovePos =  getScreenPos(_touchPos);
	_touchMovePos.y = _touchMovePos.y + mScrollViewWidget->getInnerContainer()->getPositionY();
	// 移出屏幕准备栏的高度
	if (abs(_touchMovePos.y - _touchStarPos.y) > 110)
	{
		updateCreateMouseClickDragObject(_touchPos);
	}else
	{
		if (mMouseClickDragObject)
		{
			updateCreateMouseClickDragObject(_touchPos, true);
		}
	}
}

void Ui_MainBack::onCreateObjTouchEnd(Touch* _touch, Event* _unusedEvent)
{
	Point _touchPos		= _touch->getLocation();
	createMouseClickDragObject(_touchPos);
}
// 更新创建点击模式下的。object
Point Ui_MainBack::updateCreateMouseClickDragObject(Point _touchPos, bool _isDontCreate)
{
	bool _isReturn = false;
	if (_isDontCreate && !mMouseClickDragObject)
	{
		_isReturn = true;
	}
	if (mMouseDragBeginItemIndex < 1 || mMouseDragBeginItemIndex > READY_MAX_COUNT || _isReturn)
	{
		dragObjectFinish(nullptr);
		return Point(0, 0);
	}
	float _mouseX		= _touchPos.x;
	Point _dragPos		= getScreenPos(_touchPos);
	_dragPos.x			= _dragPos.x - Director::getInstance()->getVisibleOrigin().x;
	int	_nowStoreyIndex = -1;
	int _nowStoreyPosIndex = -1;
	bool _nowIsInStorey = false;
	if (!mMouseClickDragObject)
	{
		GameObjects* _newObject = createObject(mMouseDragBeginItemIndex, _dragPos);
		Size _curSize = _newObject->getArmature()->getContentSize();
		Size _newSize = _curSize;
		_newSize.height = 272;
		Point _curPanelPos = Point(0, 0);
		_curPanelPos.y = ( _newSize.height - _curSize.height ) / 2;
		ImageView* _curPanel = ImageView::create();
		//_curPanel->setSize(_newSize);
		//_curPanel->setScale9Enabled(true);
		_curPanel->loadTexture("image/TuoCeng_00.png", UI_TEX_TYPE_LOCAL);
		_curPanel->setTag(1);
		if (_nowStoreyIndex > 0)
		{
			auto _nowStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_nowStoreyIndex);
			if (_nowStoreyData && _nowStoreyData->mItem)
			{
				float _scale = _nowStoreyData->mItem->getScaleY();
				_curPanel->setScaleY(_scale);
			}
		}
		//_curPanel->setOpacity(200);
		_newObject->addChild(_curPanel);
		_curPanel->setPosition(_curPanelPos);

		mMouseClickDragObject = _newObject;
		mMouseClickDragObject->setTag(mMouseClickDragFirstObjTag);
		mListViewWidget->addProtectedChild(mMouseClickDragObject,1);
	}

	// 检测当前层
	for (int i = 0; i < SceneData::getInstance()->getSceneDataMap().size(); ++i)
	{
		StoreyData* thisStoreyData = SceneData::getInstance()->getSceneDataMapStorey(i);
		if (thisStoreyData && thisStoreyData->mItem)
		{
			Widget* thisItemCell = thisStoreyData->mItem;
			Widget* thisRectCtrl = thisItemCell->getChildByName("Image_Rect");
			Rect thisRect;
			thisRect.origin.x = thisRectCtrl->getWorldPosition().x - (thisRectCtrl->getSize().width/2);
			thisRect.origin.y = thisRectCtrl->getWorldPosition().y  - (thisRectCtrl->getSize().height/2);
			thisRect.size.width = thisRectCtrl->getSize().width;
			thisRect.size.height = thisRectCtrl->getSize().height;
			if (thisRect.containsPoint(_touchPos))
			{
				_nowStoreyIndex = thisStoreyData->getStoreyIndex();
				auto _nodePos = thisStoreyData->mItem->convertToNodeSpace(_touchPos);
				if (_nowStoreyIndex != SCENE_STOREY_HOME && _nowStoreyIndex != SCENE_STOREY_GROUND)
				{
					_nowIsInStorey = true;
					break;
				}
			}
		}
	}

	// 检测pos点
	StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_nowStoreyIndex);
	if (_curStoreyData && _curStoreyData->mItem)
	{
		Point _nowDragPos = _curStoreyData->mItem->convertToNodeSpace(_touchPos);
		Widget* _curStoreyItemCell = _curStoreyData->mItem;
		// 是否在点上
		for (int i = 1; i <= 4; i++)
		{
			char _curName[32];
			sprintf(_curName, "Image_DragView%d", i);  
			Widget* thisRectCtrl = _curStoreyItemCell->getChildByName(_curName);
			if (thisRectCtrl)
			{
				Rect thisRect;
				thisRect.origin.x = thisRectCtrl->getPosition().x - (thisRectCtrl->getSize().width/2);
				thisRect.origin.y = thisRectCtrl->getPosition().y - (thisRectCtrl->getSize().height/2);
				thisRect.size.width = thisRectCtrl->getSize().width;
				thisRect.size.height = thisRectCtrl->getSize().height;
				if (thisRect.containsPoint(_nowDragPos))
				{
					_nowStoreyPosIndex = i;
					ImageView* thisDragBack = static_cast<ImageView*>(mMouseClickDragObject->getChildByTag(1));
					if (thisDragBack)
					{
						float newY = (272 - mMouseClickDragObject->getArmature()->getContentSize().height)/2;
						Point updateNewPos;
						updateNewPos.x = thisRectCtrl->getWorldPosition().x - Director::getInstance()->getVisibleOrigin().x;
						updateNewPos.y = thisRectCtrl->getWorldPosition().y - newY -  mScrollViewWidget->getInnerContainer()->getPositionY();	
						_dragPos = updateNewPos;

						//CCLOG("newY = %f,_dragPos:%f,%f", newY, _dragPos.x, _dragPos.y);
					}
					break;
				}
			}
		}
	}
	int _nowIsInStoreyInt = 0;
	if (_nowIsInStorey)
	{
		_nowIsInStoreyInt = 1;
	}
	//CCLOG("_nowStoreyIndex = %d, _nowStoreyPosIndex = %d, _nowIsInStorey = %d", _nowStoreyIndex, _nowStoreyPosIndex, _nowIsInStoreyInt);
	mMouseClickDragObject->setlocationIndx(_nowStoreyPosIndex);
	mMouseClickDragObject->setPosition(_dragPos);
	int _isAddType = isAddStoreyObjectType(_nowStoreyIndex, mMouseClickDragObject);

	// 颜色检测，是否可以创建
	if (mMouseClickDragObject)
	{
		ImageView* thisDragBack = static_cast<ImageView*>(mMouseClickDragObject->getChildByTag(1));
		if (thisDragBack)
		{
			if (_nowIsInStorey)
			{
				if (_isAddType == CAN_ADD_OBJECT_POS)
				{
					if (_nowStoreyPosIndex >= 1 && _nowStoreyPosIndex <= 4)
					{
						thisDragBack->loadTexture("image/TuoCeng_01.png", UI_TEX_TYPE_LOCAL);
						mIsMouseClickDragCreate = true;
					}else
					{
						thisDragBack->loadTexture("image/TuoCeng_00.png", UI_TEX_TYPE_LOCAL);
						mIsMouseClickDragCreate = false;
					}
				}else
				{
					thisDragBack->loadTexture("image/TuoCeng_00.png", UI_TEX_TYPE_LOCAL);
					mIsMouseClickDragCreate = false;
				}
			}else
			{
				thisDragBack->loadTexture("image/TuoCeng_00.png", UI_TEX_TYPE_LOCAL);
				mIsMouseClickDragCreate = false;
			}
			if (_curStoreyData && _curStoreyData->mItem)
			{
				float _scale = _curStoreyData->mItem->getScaleY();
				thisDragBack->setScaleY(_scale);
			}
			//thisDragBack->setOpacity(200);
		}
	}

	// 当前层是否满员，无法创建
	for (int i = 0; i < thisSceneData->getSceneDataMap().size(); ++i)
	{
		StoreyData* thisStoreyData = thisSceneData->getSceneDataMapStorey(i);
		Widget* thisItemCell = thisStoreyData->mItem;
		if (thisStoreyData && thisStoreyData->mItem && mMouseClickDragObject)
		{
			if (thisStoreyData->getStoreyIndex() == _nowStoreyIndex)
			{
				if (_isAddType == NOT_ADD_OBJECT_PK_FULL || _isAddType == NOT_ADD_OBJECT_HERO_FULL)
				{
					thisStoreyData->setForbidVisible(true);
				}
				else
					thisStoreyData->setForbidVisible(false);
			}else
			{
				thisStoreyData->setForbidVisible(false);
			}
		}
	}
	mMouseClickDragStoreyIndex = _nowStoreyIndex;
	return _dragPos;
}
// 创建点击模式下的。object
void Ui_MainBack::createMouseClickDragObject(Point _touchPos)
{
	if (mMouseDragBeginItemIndex < 1 || mMouseDragBeginItemIndex > READY_MAX_COUNT)
	{
		dragObjectFinish(nullptr);
		return;
	}
	// 是否移动出安全范围
	Point _touchStarPos =  getScreenPos(mMouseClickDragBeginPos);
	_touchStarPos.y = 0;
	Point _touchMovePos =  getScreenPos(_touchPos);
	_touchMovePos.y = _touchMovePos.y + mScrollViewWidget->getInnerContainer()->getPositionY();
	// 移出屏幕准备栏的高度
	if (abs(_touchMovePos.y - _touchStarPos.y) > 110)
	{
		//mIsMouseClickDragCreate = true;	// 安全范围外，可以创建
	}else
	{
		dragObjectFinish(nullptr);
		return;
	}

	Point _curPos		= updateCreateMouseClickDragObject(_touchPos);
	// 创建
	// 可以创建
	if (mIsMouseClickDragCreate && mMouseClickDragObject)
	{
		// 保留信息
		int		_curStoreyIndex		= mMouseClickDragStoreyIndex;
		int		_curStoreyPosIndex	= mMouseClickDragObject->getlocationIndx();
		int		_curObjectType		= mMouseClickDragObject->getobjectType();

		StoreyData* _curStoreyData = thisSceneData->getSceneDataMapStoreyFormStoreyIndex(_curStoreyIndex);
		if (_curStoreyData && _curStoreyData->mItem)
		{
			Widget* HorizonY = _curStoreyData->getHorizon();
			// 删除第一次创建的
			if (mMouseClickDragObject)
				mMouseClickDragObject->removeFromParentAndCleanup(true);
			if (mMouseClickDragObject)
				mListViewWidget->removeProtectedChild(mMouseClickDragObject);
			if (mListViewWidget->getProtectedChildByTag(mMouseClickDragFirstObjTag))
				mListViewWidget->removeProtectedChildByTag(mMouseClickDragFirstObjTag);
			mMouseClickDragObject = nullptr;

			// 检测范围有没有超出
			auto _nodePos = _curStoreyData->mItem->convertToNodeSpace(_touchPos);
			Widget* swg = _curStoreyData->getMovementWidget();
			float moveWidthCenterX = swg->getPositionX();
			if (_nodePos.x < (moveWidthCenterX-swg->getSize().width/2)||_nodePos.x>(moveWidthCenterX+swg->getSize().width/2))
			{
				mIsMouseClickDragCreate = false;
			}
			// 寻找这个位置是否被占用，无法创建
			for (Ref *sp:_curStoreyData->getObjectData())
			{
				GameObjects *obj=(GameObjects*)sp;
				if (obj->getlocationIndx()==_curStoreyPosIndex&&(obj->getobjectType()==TYPE_TRAP||obj->getobjectType()==TYPE_TOWER)
					&&(_curObjectType==TYPE_TRAP||_curObjectType==TYPE_TOWER))
				{
					mIsMouseClickDragCreate = false;
				}
			}
			// 可以创建新的
			if (mIsMouseClickDragCreate)
			{
				// 创建新的添加到Cell
				_curPos.y = (_curPos.y - HorizonY->getWorldPosition().y) + HorizonY->getPositionY() + mScrollViewWidget->getInnerContainer()->getPositionY();
				GameObjects* _curObj = createObject(mMouseDragBeginItemIndex, _curPos);
				if (_curObj)
				{
					// 设置基本信息
					_curObj->setfloorNum(_curStoreyIndex);
					_curObj->setlocationIndx(_curStoreyPosIndex);
					// 计算Item的实际位置,storey适配屏幕缩放后，由于锚点在（0.5,0）导致单位向中间偏移 
					Point _newPos = _nodePos;
					char _createName[32];
					sprintf(_createName, "Image_CreateView%d", _curStoreyPosIndex);  
					Widget* thisCreateRectCtrl = _curStoreyData->mItem->getChildByName(_createName);
					if (thisCreateRectCtrl)
						_newPos.x  = thisCreateRectCtrl->getPositionX();
					if (_curStoreyPosIndex<=UserRecord::getInstance()->_pixelShift.size())
						UserRecord::getInstance()->_pixelShift.at(_curStoreyPosIndex) = _newPos.x - _nodePos.x;
					// 设置层级显示
					if (_curObj->getobjectType()==TYPE_BOSS)
					{
						_curStoreyData->mItem->addChild(_curObj, 7);

					}else if(_curObj->getobjectType()==TYPE_WARRIOR)
					{
						_curStoreyData->mItem->addChild(_curObj, 6);
						if (_curObj->getSelfType() == WAR_ITEM5)
						{
							audioPlayer::getInstance()->playEffect("S70.mp3");	// 狗叫
						}
					}
					else if (_curObj->getobjectType()==TYPE_TRAP)
					{
						_curStoreyData->mItem->addChild(_curObj, 4);
						audioPlayer::getInstance()->playEffect("S72.mp3");	// 防御/陷阱 塔摆放音效
					}
					else
					{
						_curStoreyData->mItem->addChild(_curObj, 4);
						if (_curObj->getobjectType()==TYPE_TOWER)
						{
							audioPlayer::getInstance()->playEffect("S72.mp3");	// 防御/陷阱 塔摆放音效
						}
						//激光塔单独处理
						if (_curObj->getobjectType()==TYPE_TOWER && _curObj->getSelfType() == TOWER_ITEM5)
						{
							_curObj->setLocalZOrder(5);
						}
					}
					// 添加逻辑物件到层上
					addStoreyObjectFormStoreyIndex(_curStoreyIndex, _curObj);
					float _curDisY = HorizonY->getPositionY();
					if (_curObj->getobjectType()!=TYPE_TOWER && _curObj->getobjectType()!=TYPE_TRAP)
					{
						if (_curObj->getisFly()==YES)//如果是飞行战士的话
						{
							_curDisY =_curStoreyData->mItem->getContentSize().height/2+_curObj->getArmature()->getContentSize().height/2*0.5;
						}
						if (_curObj->getobjectType()==TYPE_WARRIOR)
						{
							warriorObjects*tobj=static_cast<warriorObjects*>(_curObj);//转换到防御塔
							tobj->dragObjectSmoke();//拖放声音
						}
						_curObj->runAction(
							Sequence::create(
							MoveTo::create(0.01,Point(_newPos.x, _curDisY)), 
							CallFuncN::create(CC_CALLBACK_1(Ui_MainBack::finishMoveCallback,this)), 
							NULL
							)
							);
					}else{
						_curObj->setPosition(Point(_newPos.x, _curDisY));
						if (_curObj->getobjectType()==TYPE_TOWER)
						{
							towerObjects*tobj=static_cast<towerObjects*>(_curObj);//转换到防御塔
							tobj->dragObjectSmoke();//防御塔的烟尘效果
							_curObj->setisStart(YES);

						}if (_curObj->getobjectType()==TYPE_TRAP)
						{
							trapObjects*tobj=static_cast<trapObjects*>(_curObj);//转换到防御塔
							tobj->dragObjectSmoke();//防御塔的烟尘效果
							_curObj->setisStart(YES);
						}
					}
					// 清空层提示
					for (int i = 0; i < mListViewWidget->getItems().size(); ++i)
					{
						Widget* thisItemCell = mListViewWidget->getItem(i);
						Widget* thisRectCtrl = thisItemCell->getChildByName("Image_Forbid");
						if (thisRectCtrl)
						{
							thisRectCtrl->setVisible(false);
						}
						Widget* thisNoAddCtrl = thisItemCell->getChildByName("Image_NoAddTag");
						if (thisNoAddCtrl)
						{
							thisNoAddCtrl->setVisible(false);
						}
					}
					//创建单位成功后创建LoadingBar并刷新钱
					Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
					mainLayer->createObjCost(nullptr, mMouseDragBeginItemIndex);
					if (_curObj->getobjectType()!=TYPE_BOSS && !UserRecord::getInstance()->getIsObjsNoCD())
						mainLayer->showProgress(nullptr, mMouseDragBeginItemIndex);
					//新手引导教学
					if (UserRecord::getInstance()->getIsFreshMan())
					{
						UserRecord::getInstance()->setFreshManMutex(false);
						NotificationCenter::sharedNotificationCenter()->postNotification(FORCESTART, NULL);
					}
				}
			}
		}	
	}
	dragObjectFinish(nullptr);
}