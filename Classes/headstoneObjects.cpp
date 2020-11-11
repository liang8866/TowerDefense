#pragma once
#include "headstoneObjects.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "Ui_ProgressBar.h"
#include "UserRecord.h"
#include "Ui_HeroAltar.h"

headstoneObjects::headstoneObjects(void)
{

}

headstoneObjects::~headstoneObjects(void)
{
	removeEventListenerTouch();
}

bool headstoneObjects::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	setIsGoldReviveMode(false);
	this->initWithFile("image/YXJH_MuBei.png");
	this->setScale(0.5f);
	_objectType=TYPE_HEADSTONE;
	mHeroSelfType = 0;
	this->setProperty();

	//auto cs = LayerColor::create(Color4B(255,0,0,255));
	//this->addChild(cs);
	//cs->setPosition(Point::ZERO);
	createEventListenerTouch();
	return true;
}

void headstoneObjects::setProperty() 
{
	this->_maxHp = 999999;
	this->_atk = 0;
	this->_lev = 0;
}

void headstoneObjects::setHeroSelfType(int _selfType) 
{
	mHeroSelfType = _selfType;
}
void headstoneObjects::setHeroCostMoney(int _costGold, int _costSoul) 
{
	mHeroCostSweet = UserRecord::getInstance()->getReviveSweet();	// 糖果
	mHeroCostSoul = _costSoul;
}
void headstoneObjects::setHeroCostPosition(Point _pos) 
{
	mHeroPosition = _pos;
}

// 创建触摸事件
void headstoneObjects::createEventListenerTouch()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	CC_SAFE_RETAIN(listener);
	listener->onTouchBegan = CC_CALLBACK_2(headstoneObjects::onTouchBegan, this);  
	listener->onTouchMoved = CC_CALLBACK_2(headstoneObjects::onTouchMoved, this);  
	listener->onTouchEnded = CC_CALLBACK_2(headstoneObjects::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithFixedPriority(listener, TOUCH_WARRIOR_ZORDER); 
}
// 删除触摸事件
void headstoneObjects::removeEventListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
	listener = nullptr;
}

bool headstoneObjects::onTouchBegan(Touch* _touch, Event* _unusedEvent)  
{
	if (mHeroSelfType <= 0)
		return false;
	//auto _curData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());

	//if (!_curData)
	//	return false;

	bool isCliced = SceneData::getInstance()->isExistObjSelect();

	if (isCliced==false)
	{
		Point _touchPos		= _touch->getLocation();
		Size _curSize		= this->getContentSize();
		Point _touchNodePos = this->convertToNodeSpace(_touchPos);
		Point _worldPoint   = Director::getInstance()->getRunningScene()->convertToWorldSpace(_touchPos);
		Point _originPoint  = Director::getInstance()->getVisibleOrigin();
		auto _distance = abs(_worldPoint.y - _originPoint.y);

		Point _curPos;
		_curPos.x = 0;
		_curPos.y = 0;
		Rect _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height);

		// 点中
		if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight())
		{
			WindowManager::getInstance()->mGameLayer->removeChildByTag(250);

			auto _widget = this->getChildByTag(11);
			Ui_ProgressBar* _ProgressBar = nullptr;
			if (_widget)
			{
				_ProgressBar = static_cast<Ui_ProgressBar*>(_widget);

				CCLOG("  _ProgressBar %d", _ProgressBar->mProgressBar->getPercent());
			}

			Ui_ObjectClicked *objClickedUI = Ui_ObjectClicked::create();
			objClickedUI->setTag(250);
			WindowManager::getInstance()->mGameLayer->addChild(objClickedUI,50);
			//转世界坐标
			Point _curPos = this->convertToWorldSpaceAR(Point::ZERO);
			//_curPos.y = this->getPosition().y;
			objClickedUI->clickedDeathHero(this, _curPos, mHeroSelfType, mHeroCostSweet, mHeroCostSoul, _IsGoldReviveMode);
			//objClickedUI->setSelector(this,comm_selector(headstoneObjects::objectScheduler));
			return true;
		}
	}
	return false;
} 

void headstoneObjects::onTouchMoved(Touch *touch, Event *unused_event)  
{  
	 
}  

void headstoneObjects::onTouchEnded(Touch *touch, Event *unused_event)  
{  
	
}

void headstoneObjects::objectScheduler()
{
	if (!this)
		return;

	removeEventListenerTouch();
	this->removeFromParentAndCleanup(true);
}

void headstoneObjects::removeSelfAndClearup()
{
	WindowManager::getInstance()->mGameLayer->removeChildByTag(250);
	//UserRecord::getInstance()->setHeroKeyValue(mHeroSelfType, _HeroDataKey_Index_storeyIndex, -1);

	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
}
// 开始倒计时
void headstoneObjects::beginProgressBar(int _value)
{
	Ui_ProgressBar* _ProgressBar = Ui_ProgressBar::createUI();
	if (_ProgressBar)
	{
		UserRecord::getInstance()->setHeroKeyValue(mHeroSelfType, _HeroDataKey_Index_revivieTime, YES);
		_ProgressBar->setTag(11);
		setIsGoldReviveMode(true);
		this->addChild(_ProgressBar, 1);
		int _time = UserRecord::getInstance()->getReviveSweetTime();
		_ProgressBar->setEndTime(_time);
		_ProgressBar->setScale(2.0f);
		_ProgressBar->setPositionX(-20);
		_ProgressBar->setPositionY(this->getContentSize().height/2);
		_ProgressBar->beginReviveProgressToPercent(this, _value);
	}
}
// 消耗糖果复活英雄
void headstoneObjects::reviveHeroByCostGold()
{
	auto _bossName = StringUtils::format("boss%d", mHeroSelfType);
	// 取等级
	auto _haveData = UserRecord::getInstance()->getHeroData(mHeroSelfType);
	auto _curLevelValue = _haveData.level;
	if(_curLevelValue <= 0)
		_curLevelValue = 0;
	Point _newPos;
	_newPos.x = UserRecord::getInstance()->getHeroKeyValue_Float(mHeroSelfType, _HeroDataKey_Index_reviviePosX);
	_newPos.y = 0;
	WindowManager::getInstance()->createHeroToHeroSelect(getfloorNum(), mHeroSelfType, _newPos, _curLevelValue);
	UserRecord::getInstance()->setHeroKeyValue(mHeroSelfType, _HeroDataKey_Index_revivieTime, NO);
	// 将复活状态写入文件
	UserRecord::getInstance()->setHeroKeyValue(mHeroSelfType, _HeroDataKey_Index_isDead, NO);

	Ui_HeroAltar* _heroWnd = static_cast<Ui_HeroAltar*>(WindowManager::getInstance()->getHeroAltar());
	if (_heroWnd)
	{
		int _select = _heroWnd->mCurListViewIndex;
		_heroWnd->updateHeroList();
		_heroWnd->updateHeroView(_select);
	}
	removeSelfAndClearup();
}
// 设置所有墓碑(进度条)暂停。非暂停
void headstoneObjects::isPauseHeadstone(bool _isBool)
{
	auto _widget = this->getChildByTag(11);
	Ui_ProgressBar* _ProgressBar = nullptr;
	if (_widget)
	{
		_ProgressBar = static_cast<Ui_ProgressBar*>(_widget);
	}
	
	if (_isBool)
	{
		this->pause();
		if(_ProgressBar)
		{
			_ProgressBar->isPauseAction(true);
		}
	}else
	{
		this->resume();
		if(_ProgressBar)
		{
			_ProgressBar->isPauseAction(false);
		}
	}
}