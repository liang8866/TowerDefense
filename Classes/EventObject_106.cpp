#pragma once
#include "EventObject_106.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "PlistManager.h"
#include "Ui_MainLayer.h"

EventObject_106::EventObject_106(void)
{

}
EventObject_106::~EventObject_106(void)
{
	removeEventListenerTouch();
}
EventObject_106* EventObject_106::createEvent()
{
	EventObject_106 *sprite = new EventObject_106();
	if (sprite && sprite->init())
	{
		sprite->initEvent();
		sprite->autorelease();

		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
bool EventObject_106::initEvent()
{
	_objectType=TYPE_EVENT106;
	this->setProperty();
	mMagicItems.clear();
	mRemoveTime = 8;			// x����ʧ
	mImageAction = nullptr;
	mImageActionTag = 251;		// tag
	mIsRemoveSelf = true;

	// �������ħ������
	updateView();
	//createEventListenerTouch();
	return true;
}
// ������ʾ
void EventObject_106::updateView()
{
	// 1003 item_box.plist
	auto _curItems = PlistManager::getInstance()->getItem_BoxPlist_Data(1003);
	if (_curItems.size() > 0)
	{
		mMagicItems = _curItems;
	}
	if (mMagicItems.size() <= 0)
	{
		CCLOG("Random mMagicItems size <= 0 !");
		return;
	}
	// ������ʼ���
	float _maxPercent_Float = 0;
	for (int i = 0; i != mMagicItems.size(); ++i)
	{
		_maxPercent_Float = _maxPercent_Float + mMagicItems.at(i).percent;
	}
	int _maxPercent_Int = _maxPercent_Float * 1000;
	// ���
	int _random = (int)(CCRANDOM_0_1() * _maxPercent_Int);
	float _curPercent_Flost = 0;
	int _curPercent_Int = 0;
	int _lastPercent_Int = 0;
	int _curIndex = -1;
	for (int i = 0; i != mMagicItems.size(); ++i)
	{
		_curPercent_Flost = _curPercent_Flost + mMagicItems.at(i).percent;
		_curPercent_Int = _curPercent_Flost * 1000;
		if (_random >= _lastPercent_Int && _random <= _curPercent_Int)
		{
			_curIndex = i;
			break;
		}
		_lastPercent_Int = _curPercent_Int;
	}
	// �������������ʾ
	if (_curIndex >= 0)
	{
		auto _curItemBoxData = mMagicItems.at(_curIndex);
		auto _curItemData = PlistManager::getInstance()->getItemData(_curItemBoxData.id);
		this->initWithFile(_curItemData._icon);
		mCurMagic = _curItemData;
	}
}
// ��������
void EventObject_106::setProperty() 
{
	this->_maxHp = 999999;
	this->_atk = 0;
	this->_lev = 0;
}
// ���������¼�
void EventObject_106::createEventListenerTouch()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	CC_SAFE_RETAIN(listener);
	listener->onTouchBegan = CC_CALLBACK_2(EventObject_106::onTouchBegan, this);  
	listener->onTouchMoved = CC_CALLBACK_2(EventObject_106::onTouchMoved, this);  
	listener->onTouchEnded = CC_CALLBACK_2(EventObject_106::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithFixedPriority(listener, TOUCH_EVENT106); 
}
// ɾ�������¼�
void EventObject_106::removeEventListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
	listener = nullptr;
}
// �¼�
bool EventObject_106::onTouchBegan(Touch* _touch, Event* _unusedEvent)  
{
	if (mCurMagic._id <= 0)
	{
		return false;
	}
		
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

		// ����
		if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight())
		{
			// �������ȡ���Զ�ɾ��
			mIsRemoveSelf = false;
			// �����Լ�
			removeEventListenerTouch();
			this->setVisible(false);
			// ����һ�����붯��
			createFlyAction();

			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}

			return true;
		}
	}
	return false;
} 
// �¼�
void EventObject_106::onTouchMoved(Touch *touch, Event *unused_event)  
{  

}  
// �¼�
void EventObject_106::onTouchEnded(Touch *touch, Event *unused_event)  
{  

}
// ���ų���ʱ��Ķ���
void EventObject_106::showAction() 
{
	auto _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(getfloorNum());
	if (_curStorey && _curStorey->mItem)
	{
		this->setScale(0.01f);
		this->setPositionY(_curStorey->getHorizon()->getPositionY());
		// ��ȥ�Ķ���
		Point _movePos;
		_movePos.x = this->getPositionX();
		_movePos.y = _curStorey->getHorizon()->getPositionY() + this->getContentSize().height * 2;
		auto _moveAction = MoveTo::create(1.1f, _movePos);
		auto _scaleAction = ScaleTo::create(1.1f, 0.8f, 0.8f);
		auto _spawnActions = Spawn::create(_moveAction, _scaleAction, NULL);
		// ͣ�ٵĶ���
		auto _timeAction = DelayTime::create(0.1f);
		// ����Ķ���
		Point _downPos;
		_downPos.x = _movePos.x;
		_downPos.y = _curStorey->getHorizon()->getPositionY() + this->getContentSize().height/2;
		auto _downAction = MoveTo::create(0.3f, _downPos);
		auto _actions = Sequence::create(_spawnActions, _timeAction, _downAction, CallFuncN::create(CC_CALLBACK_1(EventObject_106::onShowFinish, this)),NULL);
		this->runAction(_actions);
	}
}
// ���ų���ʱ��Ķ��� ����
void EventObject_106::onShowFinish(Ref* sender)  
{
	// ��������
	createEventListenerTouch();

	// ����ʱ�ɵ��Լ�
	auto _timeAction = DelayTime::create(mRemoveTime);
	auto _actions = Sequence::create(_timeAction, CallFuncN::create(CC_CALLBACK_1(EventObject_106::removeSelf,this)),NULL);
	this->runAction(_actions);
}
// ���ų���ʱ��Ķ��� ����
void EventObject_106::removeSelf(Ref* sender)  
{
	// �������������ģʽ����ô���Զ�ɾ��
	if (UserRecord::getInstance()->getIsFreshMan())
	{
		mIsRemoveSelf = false;
	}
	// �Զ�ɾ��
	if (mIsRemoveSelf)
	{
		if (mImageAction)
		{
			mImageAction->removeFromParentAndCleanup(true);
		}
		Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
		Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
		_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
	}
}
// �������붯��
void EventObject_106::createFlyAction()  
{  
	// ����һ��������
	if (!mImageAction)
	{
		mImageAction = ImageView::create();
		mImageAction->setTag(mImageActionTag);
		WindowManager::getInstance()->mGameLayer->addChild(mImageAction, mImageActionTag);
		mImageAction->setScale(this->getScale());
	}
	if (mImageAction)
	{
		mImageAction->loadTexture(mCurMagic._icon,UI_TEX_TYPE_LOCAL);
		Point _worldPos = WindowManager::getInstance()->mGameLayer->convertToWorldSpace(this->getPosition());
		Point _newPos;
		_newPos.x = _worldPos.x;
		auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());
		if (_curStoreyData && _curStoreyData->mItem && _curStoreyData->getHorizon())
		{
			_newPos.y = this->getPositionY() - _curStoreyData->getHorizon()->getPositionY();
			auto _thisPos = _curStoreyData->getHorizon()->getWorldPosition();
			_newPos.y = _newPos.y + _thisPos.y;
		}
		mImageAction->setPosition(_newPos);
		// ȡĿ�ĵ�����
		Widget* _moveWidget = WindowManager::getInstance()->getMainLayerMagicWidget();
		Point _movePos;
		_movePos.x = _moveWidget->getWorldPosition().x;
		_movePos.y = _moveWidget->getWorldPosition().y;
		auto _moveAction = MoveTo::create(0.5f, _movePos);
		auto _scaleAction = ScaleTo::create(0.5f, 0.1, 0.1);
		auto _spawnActions = Spawn::create(_moveAction, _scaleAction, NULL);
		auto _actions = Sequence::create(_spawnActions, CallFuncN::create(CC_CALLBACK_1(EventObject_106::onFlyFinish, this)),NULL);
		mImageAction->runAction(_actions);
	}
}
// ���붯������
void EventObject_106::onFlyFinish(Ref* sender)  
{
	// �������
	UserRecord::getInstance()->alterGoodsCount(Value(mCurMagic._id).asString(), 1);
	// �����ʱ����
	UserRecord::getInstance()->alterTemporaryMagic(mCurMagic._id, 1);
	WindowManager::getInstance()->updateMagicNum();
	// ɾ��UI
	mImageAction->removeFromParentAndCleanup(true);
	//this->removeFromParentAndCleanup(true);
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
}