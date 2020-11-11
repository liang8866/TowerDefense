#pragma once
#include "Event1Object.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "PlistManager.h"
#include "Ui_Warning.h"

event1Object::event1Object(void)
{

}

event1Object::~event1Object(void)
{
	//removeEventListenerTouch();
}
event1Object* event1Object::createEvent()
{
	event1Object *sprite = new event1Object();
	if (sprite && sprite->init())
	{
		sprite->initEvent();
		sprite->autorelease();

		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
bool event1Object::initEvent()
{
	_objectType=TYPE_EVENT1;
	mEventId = 101;		// 固定id
	mCountdown = 0;
	mCreateStoreyIndex = SceneData::getInstance()->getLastStoreyIndex();
	mCreateStoreyPos = 5;
	mImageTag = 10;
	mImageTag2 = 11;
	mImageTagYuJing = 12;
	//mImage2 = nullptr;
	mSpriteYuJing = nullptr;
	mBeginTime = 30;
	mRemoveTime = 60;

	// 创建动画主体
	isArmature = true;
	_armature = createAnimation();
	if (_armature)
	{
		this->addChild(_armature, 1);
		_armature->getAnimation()->play("load", -1, 1);
	}
	this->setProperty();
	this->createToLastStorey();
	return true;
}
// 创建动画
Armature* event1Object::createAnimation() 
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_shentou0.png","td_shentou0.plist","td_shentou.ExportJson"); 
	auto _curArmature = cocostudio::Armature::create("td_shentou");   
	if (_curArmature)
	{
		return _curArmature;
	}
	return nullptr;
}

void event1Object::setProperty() 
{
	this->_maxHp = 999999;
	this->_atk = 0;
	this->_lev = 0;
}

void event1Object::createToLastStorey()
{
	auto _eventPlist = PlistManager::getInstance()->getPlistMap("EventData");
	if (_eventPlist)
	{
		auto _idString = StringUtils::format("%d", mEventId);
		auto _idPlist = PlistManager::getInstance()->getPlist(_eventPlist, _idString.c_str());
		if (_idPlist)
		{
			mBeginTime = PlistManager::getInstance()->getPlistCString(_idPlist, "beginTime").intValue();
			mRemoveTime = PlistManager::getInstance()->getPlistCString(_idPlist, "removeTime").intValue();
			auto _storeyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(mCreateStoreyIndex);
			if (_storeyData && _storeyData->mItem)
			{
				// 取新位置位置
				Point _newPos;
				Widget* _ctrlCreate = _storeyData->mItem->getChildByName("Image_CreateView5");
				if (_ctrlCreate)
				{
					_newPos.x = _ctrlCreate->getPositionX() + 10;
				}
				Widget* _ctrlHorizon = _storeyData->getHorizon();
				if (_ctrlHorizon)
				{
					_newPos.y = _ctrlHorizon->getPositionY() + 10;
				}

				// 创建魔法
				this->setTag(mImageTag);
				_storeyData->mItem->addChild(this, 4);
				this->setfloorNum(mCreateStoreyIndex);
				this->setisStart(YES);
				SceneData::getInstance()->addStoreyObject(_storeyData->getUiIndex(), this);
				
				// 改变魔法位置	
				Point _mofaPos = _newPos;
				_mofaPos.y = _mofaPos.y + this->getContentSize().height /2 - 15;
				this->setPosition(_mofaPos);

				m_CanDownFlag=false;
				// 30秒开始魔法，提示
				Ui_Warning* uiWarning = Ui_Warning::createUI();
				if (uiWarning)
				{
					uiWarning->setTag(55);
					_storeyData->mItem->addChild(uiWarning, 4);
					uiWarning->setPositionX(this->getPositionX() - this->getArmature()->getContentSize().width/2 + 5);
					uiWarning->setPositionY(-10);
					uiWarning->beginProgress(mBeginTime);
				}
				// 30秒开始魔法
				auto _time = DelayTime::create(mBeginTime);
				auto _action = Sequence::create(
					_time,
					CallFuncN::create(CC_CALLBACK_1(event1Object::onCountdownBeginEvent, this)),
					NULL);
				this->runAction(_action);
			}
		}
	}
	
}
// 30秒显示渗透魔法
void event1Object::onCountdownBeginEvent(Ref* _sender)
{
	_armature->getAnimation()->play("xialuo", -1, 0);
	_armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(event1Object::onAnimationFrameEvent_Down));
}
void event1Object::onAnimationFrameEvent_Down(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		// 播放结束。持续等待动作
		m_CanDownFlag=true;		//这个时候是可以掉落下去了的
		_armature->getAnimation()->play("ground", -1, 1);

		// 60秒删除渗透魔法
		auto _time = DelayTime::create(mRemoveTime);
		auto _action = Sequence::create(
			_time,
			CallFuncN::create(CC_CALLBACK_1(event1Object::onCountdownRemoveEvent, this)),
			NULL);
		this->runAction(_action);
	}
}

// 60秒删除渗透魔法
void event1Object::onCountdownRemoveEvent(Ref* _sender)
{
	m_CanDownFlag=false;	//这个时候是不可以掉落下去了的
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
}

// 暂停loading漏层条
void event1Object::setWarningPause(bool _isBool)
{
	auto _storeyItem =  this->getParent();
	if (_storeyItem)
	{
		auto _widget = _storeyItem->getChildByTag(55);
		if (_widget)
		{
			Ui_Warning* uiWarning = static_cast<Ui_Warning*>(_widget);
			if (uiWarning)
			{
				uiWarning->setPause(_isBool);
			}
		}
	}
}
