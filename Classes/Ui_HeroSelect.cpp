#include "Ui_HeroSelect.h"
#include "PlistManager.h"
#include "SceneData.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "audioPlayer.h"
#include "UserRecord.h"

Ui_HeroSelect::Ui_HeroSelect():
m_widget(NULL),
mListViewHeroSelect(NULL),
mButtonClose(NULL),
mCurStoreyIndex(-1),
mIsAutoRemove(false)
{

}

Ui_HeroSelect::~Ui_HeroSelect()
{

}

bool Ui_HeroSelect::init()
{
	if (!Widget::init())
		return false;

	mIsFree = false;
	mIsDoEvent103 = false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_HeroSelect/GameUI0.plist","Ui_HeroSelect/GameUI0.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("HeroSelect_Item/GameUI0.plist","HeroSelect_Item/GameUI0.png");
	
	m_widget				= static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_HeroSelect/Ui_HeroSelect.ExportJson"));
	m_widget -> setTouchPriority(-10);
	if (!m_widget)
		return false;
	mListViewHeroSelect		= static_cast<ListView*>(Helper::seekWidgetByName(m_widget,"ListView_HeroSelect"));
	//mListViewHeroSelect->addEventListenerListView(this, listvieweventselector(Ui_HeroSelect::onListViewEvent));

	mButtonClose  = static_cast<Button*>(Helper::seekWidgetByName(m_widget, "Button_Close"));
	mButtonClose  ->addTouchEventListener(this, toucheventselector(Ui_HeroSelect::onButtonEvent));



	mVectorItems.clear();
	this->addChild(m_widget);


	fitScreen();
	return true;
}

void Ui_HeroSelect::onEnter()
{
	Widget::onEnter();
}

void Ui_HeroSelect::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_HeroSelect/GameUI0.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("HeroSelect_Item/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_HeroSelect/GameUI0.png");
	TextureCache::getInstance()->removeTextureForKey("HeroSelect_Item/GameUI0.png");

	Widget::onExit();
}

void Ui_HeroSelect::onListViewEvent(Ref* sender, ListViewEventType type)
{
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:

		break;
	case LISTVIEW_ONSELECTEDITEM_END:
		{
			auto _curIndex = mListViewHeroSelect->getCurSelectedIndex();
			auto _curItem = mListViewHeroSelect->getItem(_curIndex);

			
			break;
		}
	default:
		break;
	}
}
void Ui_HeroSelect::onButtonEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			audioPlayer::getInstance()->playEffect("S1.mp3");
			
			Button* _curButton = static_cast<Button*>(sender);	
			if (_curButton->getName() == mButtonClose->getName())
			{
				hideView();
			}
			break;
		}
	default:
		break;
	}
}


Widget* Ui_HeroSelect::addListViewItem()
{
	const char* jsonFile = "HeroSelect_Item/HeroSelect_Item.ExportJson";
	Layout* _newItemCell = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonFile));
	if (!_newItemCell)
		return nullptr;
	mListViewHeroSelect->setItemModel(_newItemCell);

	mListViewHeroSelect->pushBackDefaultItem();
	return _newItemCell;
}

void Ui_HeroSelect::fadeInAction(Node *pSender , float delay)
{
	if (!pSender->isVisible())
		return;

	pSender->setVisible(false);
	for (Node *child:pSender->getChildren()){
		if (child){
			fadeInAction(child,delay);
		}
	}
	pSender->runAction(Sequence::create(DelayTime::create(delay),CallFunc::create([=](){
																				 pSender->setVisible(true);
																				  }),ScaleBy::create(0.2f,1.05f),ScaleBy::create(0.1f,1.0/1.05f),NULL));
}

void Ui_HeroSelect::showView(int _storeyIndex, bool _isAuto, bool _isFree)
{
	mCurStoreyIndex = _storeyIndex;
	mIsAutoRemove = _isAuto;
	mIsFree = _isFree;

	updateListView();
	this->setVisible(true);
	// 免费模式不给点击退出按钮
	mButtonClose->setVisible(true);
	if (mIsFree)	
	{
		mButtonClose->setVisible(false);
	}
	audioPlayer::getInstance()->pauseAllEffects();
	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY, NULL);
}

void Ui_HeroSelect::hideView()
{
	NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, NULL);
	audioPlayer::getInstance()->resumeAllEffects();
	this->setVisible(false);
	if (mIsAutoRemove)
	{
		removeFromParentAndCleanup(true);
	}
}

// 取整个场景重复的英雄
bool Ui_HeroSelect::getDisHeroInScene(int _selfType)
{
	// 从列表里取出所有的英雄
	auto _curPlist = PlistManager::getInstance()->getPlistMap("BossData");
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		int _curTypeIndex = i + 1;
		if (_curTypeIndex == _selfType)
		{
			auto _curValue = UserRecord::getInstance()->getHeroKeyValue(_curTypeIndex, _HeroDataKey_Index_storeyIndex);
			if (_curValue > 0)
			{
				return true;
			}
		}
	}
	return false;
}
// 当前层是否有英雄，墓碑
bool Ui_HeroSelect::isStoreyHero(int _storeyIndex)
{
	auto _curPlist = PlistManager::getInstance()->getPlistMap("BossData");
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		int _curTypeIndex = i + 1;
		auto _curName = StringUtils::format("boss%d", _curTypeIndex);
		auto _curValue = UserRecord::getInstance()->getHeroKeyValue(_curTypeIndex, _HeroDataKey_Index_storeyIndex);
		if (_curValue > 0 && _curValue == _storeyIndex)
		{
			return true;
		}
	}
	return false;
}
void Ui_HeroSelect::updateListView()
{
	auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(mCurStoreyIndex);
	if (!_curStoreyData)
	{
		return;
	}
	auto _curObject = _curStoreyData->getHero();
	mListViewHeroSelect->removeAllItems();

	// 从列表里取出所有的英雄
	float delay=0;
	auto _curPlist = PlistManager::getInstance()->getPlistMap("BossData");
	auto _curArray = _curPlist->allKeys();
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		int _curTypeIndex = i + 1;
		auto _curName = StringUtils::format("boss%d", _curTypeIndex);
		auto _curNameObject = PlistManager::getInstance()->getPlistObject(_curPlist, _curName.c_str());
		if (!_curNameObject)
			break;
		addListViewItem();
		Widget* _curItemCtrl = mListViewHeroSelect->getItem(i);
		if (_curItemCtrl)
		{
			auto _haveData = UserRecord::getInstance()->getHeroData(_curTypeIndex);
			bool _isUnLock = false;
			if (_haveData.unLock == YES)
			{
				_isUnLock = true;
			}
			bool _isDeadBoss = false;
			bool _isDead = UserRecord::getInstance()->getHeroKeyValue(_curTypeIndex, _HeroDataKey_Index_isDead) == YES;
			HeroSelectItemInfo _itemData;
			_itemData.index = i;
			_itemData.heroType = _curTypeIndex;
			_itemData.item = _curItemCtrl;
			_itemData.name = _haveData.name;
			_itemData.keyName = _curName;
			_itemData.isUnLock = _isUnLock;
			_itemData.isDead = _isDead;
			auto _iconName = StringUtils::format("roleIcon/icon_%s.png", _itemData.keyName.c_str());
			if (_isDead)
			{
				_iconName = StringUtils::format("image/YXJH_MuBei.png");
				_isDeadBoss = _isDead;
			}
			_itemData.iconName = _iconName;
			mVectorItems.push_back(_itemData);

			// 设置英雄是否解锁
			Widget* _panelBack = Helper::seekWidgetByName(_itemData.item, "Image_1");
			setWidgetGray(_panelBack, !_isUnLock);

			// 设置名字
			TextBMFont* _nameCtrl = static_cast<TextBMFont*>(Helper::seekWidgetByName(_itemData.item, "Text_Name"));
			_nameCtrl->setText(_itemData.name);
			// 设置等级
			TextBMFont* _levelCtrl = static_cast<TextBMFont*>(Helper::seekWidgetByName(_itemData.item, "Text_Level"));
			string _heroLevelString = StringUtils::format("Lv.%d", _haveData.level);
			_levelCtrl->setText(_heroLevelString);

			//星级
			int full_star_num = _haveData.star/2;
			int half_star_num = _haveData.star%2; // (0 or 1)
			for (int i=1;i!=6;++i){
				ImageView *star = static_cast<ImageView*>(_panelBack->getChildByName(StringUtils::format("star_%d",i).c_str()));
				if (i<=full_star_num){
					star->loadTexture("image/GK_002.png");
				}else if (i==full_star_num+1 && half_star_num!=0){
					star->loadTexture("image/XYW_XX_02.png");
				}else{
					star->loadTexture("image/XYW_XX_01.png");
				} 
			}

			// 科技树，英雄召唤价格减少
			int _curCallMoney = PlistManager::getInstance()->getAttributeCCString("BossData", _itemData.keyName.c_str(), 1, "callMoney").intValue();
			float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech","item5", 3);
			int _costMoney = (1.0 - _level3) * _curCallMoney;
			if (mIsFree)	// 如果免费
			{
				_costMoney = 0;
			}
			auto _levelString = StringUtils::format("%d", _costMoney);
			TextBMFont* _valueCtrl = static_cast<TextBMFont*>(Helper::seekWidgetByName(_itemData.item, "Text_Value"));
			_valueCtrl->setText(_levelString);
			_valueCtrl->setColor(Color3B::BLACK);
			// 更新头像
			ImageView* _imageIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_itemData.item, "Image_Icon"));
			if (_imageIcon)
			{
				_imageIcon->loadTexture(_itemData.iconName.c_str(),UI_TEX_TYPE_LOCAL);
			}

			// 设置是否出战
			_itemData.item->setTouchPriority(-15);
			Button* _buttonIn = static_cast<Button*>(Helper::seekWidgetByName(_itemData.item, "Button_In"));
			Button* _buttonOut = static_cast<Button*>(Helper::seekWidgetByName(_itemData.item, "Button_Out"));
			_buttonIn->addTouchEventListener(this, toucheventselector(Ui_HeroSelect::onButtonEvent_In));
			_buttonOut->addTouchEventListener(this, toucheventselector(Ui_HeroSelect::onButtonEvent_Out));
			_buttonIn->setVisible(false);
			_buttonOut->setVisible(false);
			_buttonIn->setTouchEnabled(false);
			_buttonOut->setTouchEnabled(false);
			setWidgetGray(_buttonIn, true);
			setWidgetGray(_buttonOut, true);

			_buttonIn->setVisible(true);

			bool _isDisHero = isStoreyHero(mCurStoreyIndex);
			// 如果当前层存在英雄，墓碑   灰色其他所有
			if (_isDisHero)
			{
				setWidgetGray(_panelBack, true);
				setWidgetGray(_buttonIn, true);
			}
			
			
			if (_isDeadBoss)
			{
				// 死亡的英雄, 变成墓碑
				setWidgetGray(_panelBack, true);
				if (_isUnLock && _isDead)
				{
					setWidgetGray(_panelBack, false);
				}
				setWidgetGray(_imageIcon, true);
				_buttonIn->setVisible(false);
				_buttonOut->setVisible(false);
				_buttonIn->setTouchEnabled(false);
				_buttonOut->setTouchEnabled(false);
				setWidgetGray(_buttonIn, true);
				setWidgetGray(_buttonOut, true);
				
				_buttonOut->setVisible(true);
				_buttonOut->setTouchEnabled(false);
				setWidgetGray(_buttonOut, true);
			}
			
			// 解锁的 , 没死亡的
			if (_isUnLock && !_isDeadBoss)
			{
				if (_curObject)
				{
					// 存在活着的英雄，只能召回
					auto _curBossName = StringUtils::format("boss%d", _curObject->getSelfType());
					if (_curBossName == _itemData.keyName)
					{
						_buttonIn->setVisible(false);
						_buttonOut->setVisible(false);
						_buttonIn->setTouchEnabled(false);
						_buttonOut->setTouchEnabled(false);
						setWidgetGray(_buttonIn, true);
						setWidgetGray(_buttonOut, true);

						_buttonOut->setVisible(true);
						setWidgetGray(_buttonOut, false);
						_buttonOut->setTouchEnabled(true);
					}else
					{
						setWidgetGray(_panelBack, true);
					}
				}else
				{
					// 不存在英雄，可以出战
					// 检测是否整个场景有重复英雄
					bool isRepeat = getDisHeroInScene(_curTypeIndex);
					//bool isRepeat = SceneData::getInstance()->isRepeatHero(_curTypeIndex);
					if (isRepeat)
					{
						_buttonIn->setVisible(false);
						_buttonOut->setVisible(false);
						_buttonIn->setTouchEnabled(false);
						_buttonOut->setTouchEnabled(false);
						setWidgetGray(_buttonIn, true);
						setWidgetGray(_buttonOut, true);

						_buttonIn->setVisible(true);
						/*setWidgetGray(_buttonIn, false);
						_buttonIn->setTouchEnabled(true);*/
					}else
					{
						if (!_isDisHero)
						{
							// 没有重复英雄，可以出战
							_buttonIn->setTouchEnabled(true);
							setWidgetGray(_buttonIn, false);
							_buttonIn->setVisible(true);
						}
					}
				}
				//setWidgetGray(_panelBack, !_isUnLock);
			}

			// 如果是可点击状态.检测当前糖果数是否足够创建
			if (_buttonIn->isTouchEnabled())
			{
				auto _curSweet = UserRecord::getInstance()->getSweet();
				auto _costSweet = _costMoney;
				if (_curSweet < _costSweet)
				{
					_buttonIn->setTouchEnabled(false);
					setWidgetGray(_buttonIn, true);
					_valueCtrl->setColor(Color3B::RED);
				}
			}

			fadeInAction(_curItemCtrl,delay);
			delay += 0.15f;
		}
	}
}

void Ui_HeroSelect::onButtonEvent_In(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			Button* _curButton = static_cast<Button*>(sender);	
			if (_curButton)
			{
				auto _curSweet = UserRecord::getInstance()->getSweet();

				auto _curIndex = mListViewHeroSelect->getCurSelectedIndex();
				HeroSelectItemInfo _curItemData = mVectorItems.at(_curIndex);
				auto _baseCostSweet = PlistManager::getInstance()->getAttributeCCString("BossData", _curItemData.keyName.c_str(), 1, "callMoney").intValue();
				// 科技树，英雄召唤价格减少
				float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item5", 3);
				int _costSweet = (1 - _level3) * _baseCostSweet;

				if (mIsFree)
				{
					// 如果免费
					_costSweet = 0;
				}
				if (_curSweet >= _costSweet)
				{
					audioPlayer::getInstance()->playEffect("S2.mp3");
				
					// 删除消耗的糖果
					UserRecord::getInstance()->alterSweet(-_costSweet);
					UserRecord::getInstance()->flush();
					
					auto _haveData = UserRecord::getInstance()->getHeroData(_curItemData.heroType);
					// 取等级
					auto _curLevelValue = _haveData.level;
					if(_curLevelValue <= 0)
						_curLevelValue = 0;
					// 点击创建英雄进场景
					WindowManager::getInstance()->createHeroToHeroSelect(mCurStoreyIndex, _curItemData.heroType, Point(0,0), _curLevelValue);
					//新手引导教学
					if (UserRecord::getInstance()->getIsFreshMan()){
						UserRecord::getInstance()->setFreshManMutex(false);
					}else
					{
						if (mIsDoEvent103)
						{
							Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
							Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
							if (_curBackGroundWnd)
							{
								mIsDoEvent103 = false;
								_curBackGroundWnd->doEvent103();	
							}
						}
					}
					hideView();
				}				
			}
			break;
		}
	default:
		break;
	}
}
void Ui_HeroSelect::onButtonEvent_Out(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			Button* _curButton = static_cast<Button*>(sender);	
			if (_curButton)
			{
				// CCLOG(" click .. onButtonEvent_Out ..");
				// 点击从场景召回英雄

				int _lastStroeyIndex = mCurStoreyIndex;
				hideView();
				auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_lastStroeyIndex);
				if (!_curStoreyData)
				{
					return;
				}
				auto _curObject = _curStoreyData->getHero();
				if (_curObject)
				{
					UserRecord::getInstance()->setHeroKeyValue(_curObject->getSelfType(), _HeroDataKey_Index_storeyIndex, -1);
					Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
					Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
					if (_curBackGroundWnd)
					{
						_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(_lastStroeyIndex, _curObject);
					}
				}
			}
			break;
		}
	default:
		break;
	}
}



void Ui_HeroSelect::setWidgetGray(Widget* _widget, bool _isGray)
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

// 设置是否免费
void Ui_HeroSelect::setIsFree(bool _isFree)
{
	mIsFree = _isFree;
}
// 是否有103后续事件
void Ui_HeroSelect::setIsDoEvent(bool _isBool)
{
	mIsDoEvent103 = _isBool;
}

void Ui_HeroSelect::fitScreen()
{
	Point originPt   = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//float _scale = MAX( , visibleSize.height/640);
	//m_widget->setScale(_scale);
	/*m_widget->setScaleX(MAX(visibleSize.width/960 , visibleSize.height/640));
	m_widget->setScaleY(MIN(visibleSize.width/960 , visibleSize.height/640));*/

	m_widget->setScaleX(visibleSize.width/960);
	//m_widget->setScaleY(visibleSize.height/640);

	m_widget->setPosition(Point(originPt.x, originPt.y));
	mButtonClose->setPositionY(originPt.y + visibleSize.height - mButtonClose->getSize().height);

	Widget *win_bg  = static_cast<Button*>(Helper::seekWidgetByName(m_widget, "Image_1"));
	win_bg->setScaleX(2);
	win_bg->setScaleY(2);

	/*Widget* buttonClose  = m_widget->getChildByName("Button_Close"); 
	buttonClose->setPosition(Point(visibleSize.width - buttonClose->getSize().width, visibleSize.height - buttonClose->getSize().height/2));*/
}
