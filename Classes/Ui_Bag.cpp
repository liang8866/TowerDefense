#include "Ui_Bag.h"
#include "Ui_Bag_Message.h"
#include "Ui_Shop.h"
#include "PlistManager.h"
#include "UserRecord.h"
#include "Ui_Prompt.h"
#include "levelScene.h"
#include "Ui_SwallowAllTouchEvents.h"
#include "audioPlayer.h"

USING_NS_CC;


Ui_Bag::Ui_Bag(void)
{
}
Ui_Bag::~Ui_Bag(void){

}


bool Ui_Bag::init()
{
	if (!Widget::init()){
		return false;
	}
	_isPlayFinish = false;
	_isHttpFinish = false;

	mCellMaxCount = 999;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Bag/GameUI0.plist","Ui_Bag/GameUI0.png");
	mMainWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_Bag/Ui_Bag.ExportJson"));

	this->addChild(mMainWidget, 1);
	mMainWidget->setTouchEnabled(true);
	mMainWidget->setTouchPriority(TOUCH_UI_PRIORITY);

	Widget *bg = Helper::seekWidgetByName(mMainWidget, "panel_bag");
	bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
	bg->setPosition(Point(mMainWidget->getSize().width/2,mMainWidget->getSize().height/2));
	bg->setScale(bg->getScale()*0.8f);
	bg->runAction(Sequence::create(ScaleBy::create(0.1f,1.02/0.8f),ScaleBy::create(0.05f,1.0/1.02f),NULL));
	
	mButton1 = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn1"));		// 全部
	mButton2 = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn2"));		// 宝箱
	mButton3 = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn3"));		// 材料
	mButton4 = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn4"));		// 魔法
	mButton5 = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn5"));		// 道具
	mButton6 = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn6"));		// 时装
	mButton1->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Tag));
	mButton2->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Tag));
	mButton3->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Tag));
	mButton4->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Tag));
	mButton5->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Tag));
	mButton6->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Tag));

	mButtonUse = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Button_Use"));			// 使用
	mButtonCancle = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Btn_close"));	// 返回
	mButtonShop = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Button_Shop"));
	mButtonUse->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent));
	mButtonCancle->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent));
	mButtonShop->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent));

	// 选中格子详细信息
	mTextSelectDesc = static_cast<TextBMFont*>(Helper::seekWidgetByName(mMainWidget, "Text_SelectDesc"));
	mTextSelectName = static_cast<TextBMFont*>(Helper::seekWidgetByName(mMainWidget, "Text_SelectName"));	
	mTextSelectCount = static_cast<TextBMFont*>(Helper::seekWidgetByName(mMainWidget, "Text_SelectCount"));	
	mImageSelectIcon = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "Image_SelectIcon"));
	// ScrollView
	mScrollViewBag = static_cast<ui::ScrollView*>(Helper::seekWidgetByName(mMainWidget, "ScrollView_Bag"));
	// 显示数量的
	mTextMoney = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget, "Text_Money"));
	mTextHeroSoul = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget, "Text_HeroSoul"));

	// 默认选择0
	selectPage(0);

	fitScreen();
	return true;
}

void Ui_Bag::onEnter()
{
	Widget::onEnter();
}

void Ui_Bag::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Bag/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_Bag/GameUI0.png");
	Widget::onExit();
}

void Ui_Bag::addOpenBoxAnimation(const char* id)
{
	string fileName;
	string boneName;

	if (strcmp(id,"40001")==0)//普通宝箱
	{
		fileName = "box/baoxiao_Ordinary.ExportJson";
		boneName = "baoxiao_Ordinary";
	}else if (strcmp(id,"40002")==0 ){
		fileName = "box/baoxiao_superior.ExportJson";
		boneName = "baoxiao_superior";
	}else{
		return;
	}
	Ui_SwallowAllTouchEvents* swallowUi = Ui_SwallowAllTouchEvents::create();
	swallowUi->setPosition(Director::getInstance()->getVisibleOrigin());
	swallowUi->setTag(123);
	this->addChild(swallowUi, 1);

	//发送数据
	auto postDatas = __String::createWithFormat("c=props&a=openbox&bid=%s",id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->send(postDatas,this,httpresponse_selector(Ui_Bag::onHttpRequestCompleted));

	Size visibleSize  = Director::getInstance()->getVisibleSize();
	Point originPoint = Director::getInstance()->getVisibleOrigin();
	ArmatureDataManager::getInstance()->addArmatureFileInfo(fileName);
	_boxArmature = Armature::create(boneName);
	_boxArmature ->setPosition(visibleSize.width/2 + originPoint.x , visibleSize.height/2 + originPoint.y);
	this->addChild(_boxArmature,50);

	audioPlayer::getInstance()->playEffect("S5.mp3");
	audioPlayer::getInstance()->playEffect("S11.mp3");
	_boxArmature->getAnimation()->play("ON");
	_boxArmature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Ui_Bag::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	_isPlayFinish = false;
	_isHttpFinish = false;
}
void Ui_Bag::showError()
{
	auto _ui = this->getChildByTag(123);
	if (_ui)
	{
		_ui->removeFromParentAndCleanup(true);
	}

	Ui_Prompt *prompt = Ui_Prompt::create();
	prompt->setTag(251);
	prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	auto Text = PlistManager::getInstance()->getPlistMap("Text");
	__String* TextStr =  static_cast<__String*>(Text->objectForKey("1001"));
	string str = TextStr->getCString();
	while ( str.find_first_of("_n")!=string::npos )
	{
		size_t pos1 = str.find_first_of("_n"); 
		str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
	}
	prompt->initWithTwoButtons(str.c_str());
	prompt->setRightSelector(this,comm_selector(Ui_Bag::request));
	prompt->setLeftSelector(this,comm_selector(Ui_Bag::cancelRequest));
	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(prompt,300);
}
void Ui_Bag::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{

	if (!response || !response->isSucceed() || response->getResponseCode() != 200)
	{
		showError();
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	CCLOG("%s",&(*buffer->begin()));
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		//报错
		showError();
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{       

		auto _curItemData = mShowItems.at(mCurSelectItemIndex);
		if (_curItemData._type == _BagItemType::Box)
		{
			_messagWidget = Ui_Bag_Message::create();
			if (_messagWidget)
			{
				if ( v.HasMember("info")) {
					rapidjson::Value &val=v["info"];
					// 扣除宝箱数量
					UserRecord::getInstance()->alterGoodsCount(val["bid"].GetString(),-1);
					Director::getInstance()->getRunningScene()->addChild(_messagWidget, 200);
					_messagWidget->updateView(Value(val["item_id"].GetString()).asInt(), Value(val["num"].GetString()).asInt() );
					if (_isPlayFinish)
					{
						removeArmature(0.1f);
					}  else {
						_isHttpFinish = true;
						_messagWidget->setVisible(false);
					}
				}
			}
		} else if (_curItemData._type == _BagItemType::Clothes) {
			UserRecord::getInstance()->setClothesItemId(_curItemData._id);
			selectPage(mCurPageIndex);
		}

		
	} else {
		showError();
	}
}
void Ui_Bag::request(){
	auto _curItemData = mShowItems.at(mCurSelectItemIndex);
	if (_curItemData._type == _BagItemType::Box)
	{
		_boxArmature->removeFromParentAndCleanup(true);
		addOpenBoxAnimation(Value(_curItemData._id).asString().c_str());
	} else if (_curItemData._type == _BagItemType::Clothes) {
		equipClothes(_curItemData._id);
	}
}
void Ui_Bag::cancelRequest()
{
	auto _curItemData = mShowItems.at(mCurSelectItemIndex);
	if (_curItemData._type == _BagItemType::Box)
	{
		_boxArmature->removeFromParentAndCleanup(true);
	}
}
void Ui_Bag::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"open_end")==0){
		
		_isPlayFinish = true;
		if (_isHttpFinish)
		{
			scheduleOnce(schedule_selector(Ui_Bag::removeArmature),0.2f);
		}
	}
}

void Ui_Bag::removeArmature(float dt)
{
	_boxArmature->removeFromParentAndCleanup(true);
	_messagWidget->setVisible(true);
	selectPage(mCurPageIndex);
	auto _ui = this->getChildByTag(123);
	if (_ui)
	{
		_ui->removeFromParentAndCleanup(true);
	}
}

// 按钮事件
void Ui_Bag::onButtonEvent(Ref *pSender , TouchEventType type) 
{
	if (type == TOUCH_EVENT_ENDED)
	{
		Button* _curButton = static_cast<Button*>(pSender);
		if (_curButton)
		{
			if (_curButton->getName() == mButtonUse->getName())
			{
				audioPlayer::getInstance()->playEffect("S2.mp3");//点击声音
				if (mCurSelectItemIndex < 0)
					return;
				auto _curItemData = mShowItems.at(mCurSelectItemIndex);
				auto _curItemWidget = _curItemData._itemCell;
				if (_curItemWidget)
				{
					if (_curItemData._isUse == YES)
					{
						if (_curItemData._type == _BagItemType::Box)		// 道具
						{	
							// 使用宝箱 // 打开消息
							addOpenBoxAnimation(StringUtils::format("%d", _curItemData._id).c_str());
						}else if (_curItemData._type == _BagItemType::Clothes){
							// 装备时装
							equipClothes(_curItemData._id);
						}
					}
				}
			}else if (_curButton->getName() == mButtonCancle->getName())
			{
				audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
				//通知ui_sceneselect开启 touch 事件
				NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
				this->removeFromParentAndCleanup(true);
			}else if (_curButton->getName() == mButtonShop->getName())
			{
				audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
				Ui_Shop* shopLayer = Ui_Shop::create();
				shopLayer  -> setTag(124);
				Director::getInstance()->getRunningScene() -> addChild(shopLayer,101);
				shopLayer  -> selectIndex(ShopItem::SoulItem);
			}
		}
	}
}

// 标签页切换
void Ui_Bag::onButtonEvent_Tag(Ref *pSender , TouchEventType type) 
{
	if (type == TOUCH_EVENT_ENDED)
	{
		Button* _curButton = static_cast<Button*>(pSender);
		if (_curButton)
		{
			audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
			if (_curButton->getName() == mButton1->getName())
			{
				mCurPageIndex = _BagItemType::ALL;
			}else if (_curButton->getName() == mButton2->getName())
			{
				mCurPageIndex = _BagItemType::Box;
			}else if (_curButton->getName() == mButton3->getName())
			{
				mCurPageIndex = _BagItemType::Material;
			}else if (_curButton->getName() == mButton4->getName())
			{
				mCurPageIndex = _BagItemType::Magic;
			}else if (_curButton->getName() == mButton5->getName())
			{
				mCurPageIndex = _BagItemType::Property;
			}else if (_curButton->getName() == mButton6->getName()){
				mCurPageIndex = _BagItemType::Clothes;
			}
			selectPage(mCurPageIndex);
		}
	}
}
// item按钮点击
void Ui_Bag::onButtonEvent_Item(Ref *pSender , TouchEventType type) 
{
	if (type == TOUCH_EVENT_ENDED)
	{
		Button* _curButton = static_cast<Button*>(pSender);
		if (_curButton)
		{
			audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
			int _tag = _curButton->getTag();
			selectItem(_tag);
		}
	}
}

// 读取背包里的物品
void Ui_Bag::updateData() 
{
	// 清空显示
	mTextSelectName->setText("");
	mTextSelectDesc->setText("");
	mImageSelectIcon->setVisible(false);
	mTextSelectCount->setText("");
	mButtonUse->setTouchEnabled(false);
	setWidgetGray(mButtonUse, true);
	mScrollViewBag->removeAllChildrenWithCleanup(true);
	mBagItems.clear();

	auto _itemPlist = PlistManager::getInstance()->getPlistMap("Items");
	if (_itemPlist)
	{
		auto _curArray = _itemPlist->allKeys();
		for (size_t i=0; i!=_itemPlist->count(); ++i)
		{
			auto _itemIdString = dynamic_cast<__String*>(_curArray->objectAtIndex(i))->getCString();
			int _itemId = atoi(_itemIdString);
			auto _itemCount = UserRecord::getInstance()->getGoodsCountByID(_itemIdString);
			if (_itemCount > 0)
			{
				auto _curItemPlist = PlistManager::getInstance()->getPlist(_itemPlist, _itemIdString);
				if (_curItemPlist)
				{
					auto _curType = PlistManager::getInstance()->getPlistCString(_curItemPlist, "type").intValue();
					auto _curValue = PlistManager::getInstance()->getPlistCString(_curItemPlist, "value").floatValue();
					auto _curName = PlistManager::getInstance()->getPlistCString(_curItemPlist, "name")._string;
					auto _curDesc = PlistManager::getInstance()->getPlistCString(_curItemPlist, "desc")._string;
					auto _curIcon = PlistManager::getInstance()->getPlistCString(_curItemPlist, "icon")._string;
					auto _isUse = PlistManager::getInstance()->getPlistCString(_curItemPlist, "isUse").intValue();

					int _oneCellMaxCount = mCellMaxCount;
					int _makeCellCount = 1;
					if (_itemCount > _oneCellMaxCount)
					{
						int _makeCount = (int)(_itemCount/_oneCellMaxCount);
						_makeCellCount = _makeCellCount + _makeCount;
					}

					for (int i = 1; i <= _makeCellCount ; i++)
					{
						int _curCount = _itemCount;
						 if (_makeCellCount == 1)
						 {
							_curCount = _itemCount;
						 }else if (_makeCellCount > 1)
						 {
							 if (i < _makeCellCount)
							 {
								_curCount = _oneCellMaxCount;
							 }else if (i == _makeCellCount)
							 {
								_curCount = _itemCount - (_makeCellCount - 1)*_oneCellMaxCount;
							 }
						 }


						_BagItemInfo _curItemData;
						_curItemData._index = -1;
						_curItemData._id = _itemId;
						_curItemData._type = _curType;
						_curItemData._count = _curCount;
						_curItemData._name = _curName;
						_curItemData._desc = _curDesc;
						_curItemData._icon = _curIcon;
						_curItemData._value = _curValue;
						_curItemData._isUse = _isUse;
						mBagItems.push_back(_curItemData);
					}
				}
			}
		}
	}
}

// 选择标签页
void Ui_Bag::selectPage(int _pageIndex) 
{
	if (_pageIndex < 0)
		return;
	mCurPageIndex = _pageIndex;
	// 更新钱币，和英雄之魂显示
	auto _curGold = UserRecord::getInstance()->getGold();
	auto _curGoldString = StringUtils::format("%d", _curGold);
	mTextMoney->setText(_curGoldString);
	auto _curSoul = UserRecord::getInstance()->getHeroSoul();
	auto _curSoulString = StringUtils::format("%d", _curSoul);
	mTextHeroSoul->setText(_curSoulString);
	// 清空显示，重置数据 
	updateData();
	mCurSelectItemIndex = -1;
	
	// 按钮状态改变
	Button* _curButton = nullptr;
	if (mCurPageIndex == _BagItemType::ALL)
	{
		_curButton = mButton1;
	}else if (mCurPageIndex == _BagItemType::Box)
	{
		_curButton = mButton2;
	}else if (mCurPageIndex == _BagItemType::Material)
	{
		_curButton = mButton3;
	}else if (mCurPageIndex == _BagItemType::Magic)
	{
		_curButton = mButton4;
	}else if (mCurPageIndex == _BagItemType::Property)
	{
		_curButton = mButton5;
	}else if (mCurPageIndex == _BagItemType::Clothes){
		_curButton = mButton6;
	}

	if (_curButton)
	{
		resetTagButton();
		_curButton->setLocalZOrder(7);
		_curButton->setBright(false);
	}

	mShowItems.clear();
	Size _defaultSize;

	// 取数量
	for (int i = 0; i != mBagItems.size(); i++)
	{
		_BagItemInfo _curItemData;
		_curItemData._id = -1;
		_curItemData._type = -1;
		_curItemData = mBagItems.at(i);
		if (_curItemData._id > 0 && _curItemData._type >= 0)
		{
			if (mCurPageIndex == _BagItemType::ALL || _curItemData._type == mCurPageIndex)
			{
				_curItemData._index = i;
				_curItemData._itemCell = nullptr;
				mShowItems.push_back(_curItemData);
			}
		}
	}
	// 排序
	sort(mShowItems.begin(),mShowItems.end(),CompLess());

	int pageMaxCount = 16;
	int nowCount = 0;
	if (pageMaxCount >= mShowItems.size())
	{
		nowCount = pageMaxCount;
	}else
	{
		nowCount = mShowItems.size();
	}

	// 更新到显示
	for (int i = 0; i != nowCount; i++)
	{
		Widget* _newItemCell = dynamic_cast<Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Ui_Bag_Item/Ui_Bag_Item.ExportJson"));
		if (_newItemCell)
		{
			Button* _buttonItem	= static_cast<Button*>(Helper::seekWidgetByName(_newItemCell, "ButtonItem"));
			ImageView* _imageIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_newItemCell, "Image_Icon"));
			ImageView* _imageSelect = static_cast<ImageView*>(Helper::seekWidgetByName(_newItemCell, "Image_Select"));
			ImageView* _textNumBack = static_cast<ImageView*>(Helper::seekWidgetByName(_newItemCell, "Image_NumBack"));
			if (_imageIcon)
				_imageIcon->setVisible(false);
			if (_textNumBack)
				_textNumBack->setVisible(false);

			if (mShowItems.size() >= i +1)
			{
				mShowItems.at(i)._itemCell = _newItemCell;
			}else
			{
				_BagItemInfo _curItemData;
				_curItemData._id = -1;
				_curItemData._type = -1;
				_curItemData._index = i;
				_curItemData._itemCell = _newItemCell;
				mShowItems.push_back(_curItemData);
			}
			_newItemCell->setTag(i);
			_defaultSize = _newItemCell->getSize();
			mScrollViewBag->addChild(_newItemCell, 10);			
		}
	}
	// 完了更新位置
	int _maxLineItemCount = 4;						// 每行4个格子
	int _maxLineCount = (int)mShowItems.size()/4;	// 计算一共几行

	auto _cellSize = _defaultSize;
	auto _disWidth = 28;//(mScrollViewBag->getSize().width - _cellSize.width * _maxLineItemCount)/ (_maxLineItemCount+1);
	int _x = 0;
	int _y = mScrollViewBag->getSize().height - 100;

	// 更新滚动层
	int viewHeight = _maxLineCount * _cellSize.height + _maxLineCount * 12;
	Size _viewSize;
	_viewSize.width = mScrollViewBag->getInnerContainerSize().width;
	_viewSize.height = viewHeight;
	mScrollViewBag->setInnerContainerSize(_viewSize);

	int _curIndex = 0;

	int j = 1;	// 行
	int k = 1;  // 列
	for (int i = 0; i != mShowItems.size(); i++)
	{
		_x = k * _disWidth + (k -1)*_cellSize.width;

		int _disHeight = 12 * j ;
		int _cellHeight = _cellSize.height * j;
		auto _lineHeight = _disHeight  + _cellHeight ;
		_y = mScrollViewBag->getInnerContainerSize().height - _lineHeight;

		mShowItems.at(i)._itemCell->setPositionX(_x);
		mShowItems.at(i)._itemCell->setPositionY(_y);

		k = k + 1;
		if (k > _maxLineItemCount)
		{
			k = 1;
			j = j + 1;
		}
	}

	// 更新显示
	float delay=0;
	for (int i = 0; i != mShowItems.size(); i++)
	{
		auto _curItemData = mShowItems.at(i);
		auto _itemCell = (Widget*)mScrollViewBag->getChildByTag(i);
		if (_itemCell)
		{
			_curItemData._itemCell = _itemCell;
		}
		auto _curItemWidget = _curItemData._itemCell;
		if (_curItemWidget && _curItemData._id > 0 && _curItemData._type >=0)
		{
			Button* _buttonItem	= static_cast<Button*>(Helper::seekWidgetByName(_curItemWidget, "ButtonItem"));
			ImageView* _imageIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_curItemWidget, "Image_Icon"));
			ImageView* _imageSelect = static_cast<ImageView*>(Helper::seekWidgetByName(_curItemWidget, "Image_Select"));
			TextBMFont* _textNum = static_cast<TextBMFont*>(Helper::seekWidgetByName(_curItemWidget, "Text_Num"));
			ImageView* _textNumBack = static_cast<ImageView*>(Helper::seekWidgetByName(_curItemWidget, "Image_NumBack"));
			ImageView* _equipTag    = static_cast<ImageView*>(Helper::seekWidgetByName(_curItemWidget, "equipTag"));

			if (_buttonItem)
			{
				_buttonItem->setTag(i);
				_buttonItem->setTouchPriority(-15);
				_buttonItem->addTouchEventListener(this, toucheventselector(Ui_Bag::onButtonEvent_Item));
			}
			if (_imageIcon)
			{
				_imageIcon->setVisible(true);
				_imageIcon->loadTexture(_curItemData._icon);

				if (_curItemData._id >70000 && _curItemData._id<80000 ){
					//_imageIcon->setScale(0.6f);
				}
			}

			if (UserRecord::getInstance()->getClothesItemId() == _curItemData._id && _equipTag){
				_equipTag->setVisible(true);
			}


			if (_textNumBack && (_curItemData._id<70000 || _curItemData._id>80000) )
			{
				_textNumBack->setVisible(true);
			}
			if (_textNum)
			{
				
				auto _curString = StringUtils::format("%d", _curItemData._count);
				_textNum->setText(_curString);
			}
			if (_imageSelect)
			{
				_imageSelect->setVisible(false);
			}
		}

		fadeInAction(_itemCell,delay);
		delay += 0.05f;
	}
	// 默认选择第一个格子
	if (mShowItems.size() > 0)
	{
		selectItem(0);
	}
}

void Ui_Bag::fadeInAction(Node *pSender,float delay)
{
	pSender->setOpacity(0);
	for (Node *child:pSender->getChildren()){
		if (child){
			fadeInAction(child,delay);
		}
	}
	pSender->runAction(Sequence::create(DelayTime::create(delay),FadeIn::create(0.2f),NULL));
}

// 选择某个格子
void Ui_Bag::selectItem(int _itemIndex) 
{
	Widget* _panelDesc = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget, "Panel_Desc"));
	if (_panelDesc)
		_panelDesc->setVisible(false);
	mTextSelectName->setText("");
	mTextSelectDesc->setText("");
	mImageSelectIcon->setVisible(false);
	mTextSelectCount->setText("");
	mButtonUse->setTouchEnabled(false);
	setWidgetGray(mButtonUse, true);
	if (_itemIndex < 0)
	{
		return;
	}
	mCurSelectItemIndex = _itemIndex;

	for (int i = 0; i != mShowItems.size(); i++)
	{
		auto _curItemData = mShowItems.at(i);
		auto _curItemWidget = _curItemData._itemCell;
		if (_curItemWidget)
		{
			ImageView* _imageSelect = static_cast<ImageView*>(Helper::seekWidgetByName(_curItemWidget, "Image_Select"));
			if (_imageSelect)
			{
				if (_curItemData._id > 0 && i == _itemIndex)
				{
					if (_panelDesc)
						_panelDesc->setVisible(true);
					_imageSelect->setVisible(true);
					mTextSelectName->setText(_curItemData._name);
					string str = _curItemData._desc;
					while ( str.find_first_of("_n")!=string::npos )
					{
						size_t pos1 = str.find_first_of("_n"); 
						str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
					}
					mTextSelectDesc->setText(str);
					mImageSelectIcon->setVisible(true);
					/*mImageSelectIcon->loadTexture(_curItemData._icon);*/
					
					if (_curItemData._id >70000 && _curItemData._id<80000 ){
						//时装的显示
						CCLOG("_curItemData._id=%d",_curItemData._id);
						mImageSelectIcon->loadTexture( String::createWithFormat("clothes/clothes_%d.png",_curItemData._id-70001)->getCString());
						mImageSelectIcon->setScale(1.0f);

						ImageView* bg = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "obj_icon_bk"));
						bg->setVisible(false);

						ImageView* bk = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "obj_icon_0"));
						bk->setVisible(false);

						Widget* label = Helper::seekWidgetByName(mMainWidget, "Text_SelectCount");
						label->setVisible(false);

						mTextSelectName->setPositionY(185-80);
						mTextSelectDesc->setPositionY(158-90);
						mImageSelectIcon->setPositionY(247-30);
					}else{
							mImageSelectIcon->loadTexture(_curItemData._icon);
						mImageSelectIcon->setScale(1.0f);

						ImageView* bg = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "obj_icon_bk"));
						bg->setVisible(true);

						ImageView* bk = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "obj_icon_0"));
						bk->setVisible(true);

						Widget* label = Helper::seekWidgetByName(mMainWidget, "Text_SelectCount");
						label->setVisible(true);

						mTextSelectName->setPositionY(185);
						mTextSelectDesc->setPositionY(158);
						mImageSelectIcon->setPositionY(247);
					}

					auto _curString = StringUtils::format("%d", _curItemData._count);
					mTextSelectCount->setText(_curString);

					mButtonUse->setTouchEnabled(false);
					setWidgetGray(mButtonUse, true);
					if (_curItemData._isUse == YES && _curItemData._id != UserRecord::getInstance()->getClothesItemId() )
					{
						mButtonUse->setTouchEnabled(true);
						setWidgetGray(mButtonUse, false);
					}
				}else
				{
					_imageSelect->setVisible(false);
				}
			}
		}
	}
}

// 重置标签页按钮
void Ui_Bag::resetTagButton() 
{
	mButton1->setLocalZOrder(6);
	mButton2->setLocalZOrder(5);
	mButton3->setLocalZOrder(4);
	mButton4->setLocalZOrder(3);
	mButton5->setLocalZOrder(2);
	mButton6->setLocalZOrder(1);

	mButton1->setBright(true);
	mButton2->setBright(true);
	mButton3->setBright(true);
	mButton4->setBright(true);
	mButton5->setBright(true);
	mButton6->setBright(true);
}


void Ui_Bag::fitScreen()
{	
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	mMainWidget->setScale(_scale);

	Widget* _backImage  = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "Image_6"));
	_backImage->setTouchEnabled(true);
	_backImage->setTouchPriority(-9);
	_backImage->setScaleX(2);
	_backImage->setScaleY(2);

	mMainWidget->setPosition(Point(visibleSize.width/2  - mMainWidget->getSize().width/2*_scale  + originPt.x,
		visibleSize.height/2 - mMainWidget->getSize().height/2*_scale + originPt.y
		));
}


void Ui_Bag::setWidgetGray(Widget* _widget, bool _isGray)
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

int Ui_Bag::getItemNum()
{
	int count = 0;
	for (int i = 0; i != mShowItems.size(); i++)
	{
		auto _curItemData = mShowItems.at(i);
		if (_curItemData._itemCell && _curItemData._id > 0 && _curItemData._type >= 0)
		{
			count = count + 1;
		}
	}
	return count;
}

void Ui_Bag::equipClothes(int itemid)
{

	//发送数据
	auto postDatas = __String::createWithFormat("c=props&a=clothes&id=%d",itemid)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas,this,httpresponse_selector(Ui_Bag::onHttpRequestCompleted));
	CCLOG("equipClothes!!!!");
}

