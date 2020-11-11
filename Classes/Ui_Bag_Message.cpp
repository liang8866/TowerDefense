#include "Ui_Bag_Message.h"
#include "Ui_Bag.h"
#include "Ui_Shop.h"
#include "PlistManager.h"
#include "UserRecord.h"
#include "Ui_Prompt.h"
#include "levelScene.h"

USING_NS_CC;


Ui_Bag_Message::Ui_Bag_Message(void)
{
}
Ui_Bag_Message::~Ui_Bag_Message(void){

}


bool Ui_Bag_Message::init()
{
	if (!Widget::init()){
		return false;
	}
	mMainWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_Bag_Message/Ui_Bag_Message.ExportJson"));
	this->addChild(mMainWidget, 1);
	mMainWidget->setTouchEnabled(true);
	mMainWidget->setTouchPriority(TOUCH_UI_BagMessage);
	
	mButtonEnterMessage = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Button_EnterMessage"));
	mImageItemIconMessage = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "Image_MessageItemIcon"));
	mTextItemCountMessage = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget, "Text_MessageItemCount"));
	mButtonEnterMessage->addTouchEventListener(this, toucheventselector(Ui_Bag_Message::onButtonEvent_Message));

	fitScreen();
	return true;
}

void Ui_Bag_Message::onEnter()
{
	Widget::onEnter();
}
void Ui_Bag_Message::onExit()
{
	Widget::onExit();
}

void Ui_Bag_Message::updateView(int _itemId,const int itemCount){
	
	auto _curItemData = PlistManager::getInstance()->getItemData(_itemId);
	mImageItemIconMessage->loadTexture(_curItemData._icon);
	mTextItemCountMessage->setText(Value(itemCount).asString());
	// 增加开箱的物品到数据
	if (_curItemData._type > _BagItemType::ALL && _curItemData._type <= _BagItemType::Other)
	{
		if (_curItemData._type == _BagItemType::Other)
		{
			// 虚拟货币
			if (_curItemData._id == 100001)
			{
				UserRecord::getInstance()->alterGold(itemCount);	// 金币
			}else if (_curItemData._id == 100002)
			{
				UserRecord::getInstance()->alterHeroSoul(itemCount);	// 英雄之魂
			}
		}else
		{
			// 道具
			auto _idAllCount = UserRecord::getInstance()->getGoodsCountByID(Value(_itemId).asString());
			if (_idAllCount >= 0)
			{
				auto _idNewCount = _idAllCount + itemCount;
				auto _idNewCountString = StringUtils::format("%d", _idNewCount);
				UserRecord::getInstance()->setGoodsCount(Value(_itemId).asString(), _idNewCountString);
			}
		}
	}
}
void Ui_Bag_Message::updateView(int _itemId)
{
	if (_itemId < 0)
		return;
	mBoxItems.clear();
	mItemId = _itemId;
	auto _itemData = PlistManager::getInstance()->getItemData(mItemId);
	if (_itemData._id > 0)
	{
		int _boxId = (int)_itemData._value;
		if (_boxId > 0 )
		{
			auto _curItems = PlistManager::getInstance()->getItem_BoxPlist_Data(_boxId);
			if (_curItems.size() > 0)
			{
				mBoxItems = _curItems;
			}
		}
	}

	if (mBoxItems.size() > 0)
	{
		int random = (int)(CCRANDOM_0_1() * mBoxItems.size());
		auto _curItemBoxData = mBoxItems.at(random);
		auto _curItemData = PlistManager::getInstance()->getItemData(_curItemBoxData.id);

		mImageItemIconMessage->loadTexture(_curItemData._icon);
		auto _countString = StringUtils::format("%d", _curItemBoxData.count);
		mTextItemCountMessage->setText(_countString);
		
		// 增加开箱的物品到数据
		if (_curItemData._type > _BagItemType::ALL && _curItemData._type <= _BagItemType::Other)
		{
			if (_curItemData._type == _BagItemType::Other)
			{
				// 虚拟货币
				if (_curItemData._id == 100001)
				{
					UserRecord::getInstance()->alterGold(_curItemBoxData.count);	// 金币
				}else if (_curItemData._id == 100002)
				{
					UserRecord::getInstance()->alterHeroSoul(_curItemBoxData.count);	// 英雄之魂
				}
			}else
			{
				// 道具
				auto _idString = StringUtils::format("%d", _curItemBoxData.id);
				auto _idAllCount = UserRecord::getInstance()->getGoodsCountByID(_idString);
				if (_idAllCount >= 0)
				{
					auto _idNewCount = _idAllCount + _curItemBoxData.count;
					auto _idNewCountString = StringUtils::format("%d", _idNewCount);
					UserRecord::getInstance()->setGoodsCount(_idString, _idNewCountString);
				}
			}
			UserRecord::getInstance()->flush();
		}
	}
}


void Ui_Bag_Message::onButtonEvent_Message(Ref *pSender , TouchEventType type) 
{
	if (type == TOUCH_EVENT_ENDED)
	{
		Button* _curButton = static_cast<Button*>(pSender);
		if (_curButton)
		{
			this->removeFromParentAndCleanup(false);
		}
	}
}

void Ui_Bag_Message::fitScreen()
{	
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	mMainWidget->setScale(_scale);

	Widget* _backImage  = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "Image_6"));
	_backImage->setTouchEnabled(true);
	_backImage->setTouchPriority(TOUCH_UI_BagMessage + 1);
	_backImage->setScaleX(2);
	_backImage->setScaleY(2);

	mMainWidget->setPosition(Point(visibleSize.width/2  - mMainWidget->getSize().width/2*_scale  + originPt.x,
		visibleSize.height/2 - mMainWidget->getSize().height/2*_scale + originPt.y
		));
}


void Ui_Bag_Message::setWidgetGray(Widget* _widget, bool _isGray)
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
