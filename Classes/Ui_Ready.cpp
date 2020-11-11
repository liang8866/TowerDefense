#include "Ui_Ready.h"
#include "PlistManager.h"
#include "levelScene.h"
#include "LoadingScene.h"
#include "CachManager.h"
#include "audioPlayer.h"
#include "WindowManager.h"
using namespace network;
ReadyItemInfo::ReadyItemInfo():
unit_id("1234"),
lock(true),
price(100),
selected(false),
name("XXX"),
unLockPrice(0)
{

}

ReadyItemInfo::~ReadyItemInfo()
{

}

Ui_Ready::Ui_Ready():
m_widget(NULL),
listView(NULL),
soldierBtn(NULL),
towerBtn(NULL),
trapBtn(NULL),
beginGameBtn(NULL),
addItemBtn(NULL),
occupySeatIndex(0),
mArrows(nullptr)
{
	_vecObj.clear();
	_itemInfo.clear();
}

Ui_Ready::~Ui_Ready()
{

};

bool Ui_Ready::init()
{
	if (!Widget::init())
		return false;

	mDefaultItemScale = 1;
	mDefaultItemCount = -1;
	mIsComingText = "";
	mCurSelectId = "";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Ready/GameUI0.plist","Ui_Ready/GameUI0.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Ready_Item/GameUI0.plist","Ui_Ready_Item/GameUI0.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Ready_Item_1/GameUI0.plist","Ui_Ready_Item_1/GameUI0.png");

	m_widget   = GUIReader::getInstance()->widgetFromJsonFile("Ui_Ready/Ui_Ready.ExportJson");

	listView   = static_cast<ListView*>(Helper::seekWidgetByName(m_widget,"ListView_goods"));

	soldierBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Soldier"));
	towerBtn   = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Tower"));
	trapBtn    = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Trap"));


	Button *closeBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_close"));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float hscale = visibleSize.height/640;
	closeBtn->setScale(hscale);


	Button *shopBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_shop"));
	closeBtn    ->setPressedActionEnabled(true);
	
	soldierBtn  ->addTouchEventListener(this,toucheventselector(Ui_Ready::itemBtnClickedCallFuc));
	towerBtn    ->addTouchEventListener(this,toucheventselector(Ui_Ready::itemBtnClickedCallFuc));
	trapBtn     ->addTouchEventListener(this,toucheventselector(Ui_Ready::itemBtnClickedCallFuc));
	shopBtn     ->addTouchEventListener(this,toucheventselector(Ui_Ready::shopBtnCallFunc));
	closeBtn    ->addTouchEventListener(this,toucheventselector(Ui_Ready::close));

	mBitmapLabelDesc = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"BitmapLabel_Desc"));
	mBitmapLabelDesc->setText("");
	mBitmapLabelName = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"BitmapLabel_Name"));
	mBitmapLabelUnLockCost = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"BitmapLabel_UnLockCost"));
	mButtonUnLock = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_UnLock"));
	mButtonUnLock  ->addTouchEventListener(this,toucheventselector(Ui_Ready::onButtonClick_UnLock));
	

	goldNum    = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"gold_num"));
	goldNum     ->setText(String::createWithFormat("%d",UserRecord::getInstance()->getGold())->getCString());
	goldNum->setFontName(MyfontName);
	goldNum->setColor(Color3B(255,255,255));
	

	bottomPosY = Helper::seekWidgetByName(m_widget,"Panel_5")->getPositionY();

	panel_test = static_cast<Layout*>(Helper::seekWidgetByName(m_widget,"panel_test"));

	addChild(m_widget);
	// 创建动画箭头
	createAnimArrows();

	//适配屏幕
	fitScreen();
	//初始化数据
	initData();
	//设置携带单位数
	setSeatCount(UserRecord::getInstance()->getSeatCount());

	//设置默认选择条目
	selectIndex(ReadySodierItem);

	//设置默认携带兵种
	initObjs();

	// 更新关卡数字
	updateSceneNumber();

	// 设置动态横条
	ImageView* _imageLine1 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line1"));
	createLineAction(_imageLine1, 0.7, 7);
	ImageView* _imageLine2 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line2"));
	createLineAction(_imageLine2, 0.8, 6);
	ImageView* _imageLine3 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line3"));
	createLineAction(_imageLine3, 0.6, 3);
	ImageView* _imageLine4 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line4"));
	createLineAction(_imageLine4, 1.0, 8);
	return true;
}

void Ui_Ready::onEnter()
{
	Widget::onEnter();
}

void Ui_Ready::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Ready/GameUI0.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Ready_Item/GameUI0.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Ready_Item_1/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_Ready/GameUI0.png");
	TextureCache::getInstance()->removeTextureForKey("Ui_Ready_Item/GameUI0.png");
	TextureCache::getInstance()->removeTextureForKey("Ui_Ready_Item_1/GameUI0.png");
	Widget::onExit();
}

void Ui_Ready::initObjs()
{
	if (!UserRecord::getInstance()->_takeUnitsID.empty()){
		for (auto iter=UserRecord::getInstance()->_takeUnitsID.begin();iter!=UserRecord::getInstance()->_takeUnitsID.end();iter++){
			_vecObj.insert(make_pair(iter->second,iter->first));
			insertObjsIcon();
			showSelectFrame(iter->second);
			occupySeatIndex +=1;
		}
	}
}

void Ui_Ready::fitScreen()
{
	Point originPt   = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	do{
		CC_BREAK_IF(!m_widget);
		Widget* panel_mid = m_widget->getChildByName("Panel_mid"); 
		CC_BREAK_IF(!panel_mid);
		panel_mid->setAnchorPoint(Point(0.5f,0.5f));
		panel_mid->setPosition(Point(originPt.x+visibleSize.width/2,
			originPt.y+visibleSize.height/2
			));
		panel_mid->setScaleX(visibleSize.width/960);
		panel_mid->setScaleY(visibleSize.height/640);

		//***********************************************
		float _y = panel_test->getWorldPosition().y;
		listView->setPosition(Point(originPt.x,_y));

		float _scale = visibleSize.height/640;
		if (visibleSize.height>640)
		{
			_scale = visibleSize.width/960;
		}
		mDefaultItemScale = _scale;
		mDefaultItemCount = 5;

		Size _newSize;
		_newSize.width = panel_mid->getSize().width;
		_newSize.height = panel_test->getSize().height * mDefaultItemScale;
		listView->setSize(_newSize);
		//***********************************************


		Widget* panel_bottom = m_widget->getChildByName("Panel_Bottom");
		CC_BREAK_IF(!panel_bottom);
		panel_bottom->setScale(_scale);
		panel_bottom->setPositionX(visibleSize.width/2  - panel_bottom->getSize().width/2*_scale  + originPt.x);
		panel_bottom->setPositionY(originPt.y);
		Widget* panel_up  = m_widget->getChildByName("Panel_up"); 
		CC_BREAK_IF(!panel_up);
		panel_up->setPosition(Point(originPt.x+visibleSize.width-panel_up->getSize().width,
			originPt.y+visibleSize.height-panel_up->getSize().height));

		Widget* bg = m_widget->getChildByName("ui_back");
		CC_BREAK_IF(!bg);
		bg->setAnchorPoint(Point::ZERO);
		bg->setPosition(Point(originPt.x,originPt.y));
		bg->setScaleX(visibleSize.width/960);

		if (visibleSize.height<640)
			return;
		bg->setScaleY(visibleSize.height/640);
	}while (0);
}

void Ui_Ready::selectIndex(ReadyItem idx)
{
	Button   *btn;
	switch (idx)
	{
	case ReadySodierItem:
		btn  = soldierBtn;
		break;
	case ReadyTowerItem:
		btn  = towerBtn;
		break;
	case ReadyTrapItem:
		btn  = trapBtn;
		break;
	default:
		break;
	}
	itemBtnClickedCallFuc(btn , TOUCH_EVENT_ENDED);
}

void Ui_Ready::initData()
{

	const string root[3] = {"WarriorData","TowerData","TrapData"};
	const string key[3]  = {"war" ,"tower","trap"};
	const string objTypes[3]  = {"4" ,"3","2"};

	// 科技树，防御塔召唤减少
	float _item2_level2 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item2", 2);
	float _item2_level4 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item2", 4);
	float towerValue = _item2_level2 + _item2_level4;
	// 科技树，陷阱召唤减少
	float _item3_level2 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item3", 2);
	float _item3_level4 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item3", 4);
	float trapValue = _item3_level2 + _item3_level4;
	// 科技树，士兵召唤减少
	float _item4_level2 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 2);
	float _item4_level4 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 4);
	float warValue = _item4_level2 + _item4_level4;
	
	_itemInfo.clear();
	ReadyItemInfo _info;
	vector<ReadyItemInfo> vec;
	__Dictionary *dic; 
	__Dictionary *_exObj;
	__Dictionary *_obj; 
	for (int i=0; i!=sizeof(root)/sizeof(string);++i){
		dic    = PlistManager::getInstance()->getPlistMap(root[i].c_str());
		float reduceValue=0.0;
		if (i==0)
		{
			reduceValue=1-warValue;
		}
		if (i==1)
		{
			reduceValue=1-towerValue;
		}
		if (i==2)
		{
			reduceValue=1-trapValue;
		}
	
		vec.clear();
		//给该条目内数据赋值
		for (int j=0;j!=dic->count();++j){
			

			 //由于数据调动，原来的TRAP5->TRAP2 , TRAP2->TRAP3,TRAP3->TRAP4,TRAP4->TRAP5,但exportJson尚未更改，故做以下变更。
			 int idx = j+1;
			 if (i==2){
				 if (j==1)
					 idx = 5;
				 else if (j==2)
					 idx = 2;
				 else if (j==3)
					 idx = 3;
				 else if (j==4)
				     idx = 4;
			 }

			_exObj = PlistManager::getInstance()->getPlist(dic,String::createWithFormat("%s%d",key[i].c_str(),idx)->getCString());
			_obj  = PlistManager::getInstance()->getPlist(_exObj,"level1");

			_info.unit_id      = String::createWithFormat("%s%d",key[i].c_str(),idx)->getCString();
			_info.unLockPrice  = atoi(PlistManager::getInstance()->getPlistCString(_obj,"unlockMoney").getCString());
		
			_info.price        = atoi(PlistManager::getInstance()->getPlistCString(_obj,"callMoney").getCString())*(reduceValue); 
			
			_info.selected     = false;
			_info.objectType   = atoi(objTypes[i].c_str());
			_info.selfType	   = idx;
			_info.close        = false;
			_info.isComing     = atoi(PlistManager::getInstance()->getPlistCString(_exObj,"isComing").getCString());
			_info.desc		   = PlistManager::getInstance()->getPlistCString(_exObj,"desc").getCString();
			_info.name		   = PlistManager::getInstance()->getPlistCString(_exObj,"name").getCString();
			if (_info.isComing == YES)
			{
				mIsComingText = _info.desc;
			}
			//还需判断用户是否解锁
			_info.lock  =  !UserRecord::getInstance()->getObjectsIsUnlock(_info.unit_id);
			vec.push_back(_info);
		}
		_itemInfo.insert(make_pair(i+1,vec));
	}
	// 恢复英雄所在层级, 死亡状态
	UserRecord::getInstance()->clearHeroValue_isDead_StoreyIndex();
}

void Ui_Ready::itemBtnClickedCallFuc(Ref *pSender , TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){

		audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
		initItemZOrder();
		Button *bt = static_cast<Button*>(pSender);
		bt->setTouchEnabled(false);
		bt->setBright(false);
		bt->setLocalZOrder(ReadyHighLightZorder);
		if (strcmp(bt->getName(),soldierBtn->getName())==0){
			setSelectedItem(ReadySodierItem);
			updateItemInfo(1);
		}else if (strcmp(bt->getName(),towerBtn->getName())==0){
			setSelectedItem(ReadyTowerItem);
			updateItemInfo(2);
		}else if (strcmp(bt->getName(),trapBtn->getName())==0){
			setSelectedItem(ReadyTrapItem);
			updateItemInfo(3);
		}
	}
}

void Ui_Ready::initItemZOrder()
{
	do{
		CC_BREAK_IF(!soldierBtn);
		soldierBtn -> setLocalZOrder(ReadySoldierZorder);
		soldierBtn -> setTouchEnabled(true);
		soldierBtn -> setBright(true);
		CC_BREAK_IF(!towerBtn);
		towerBtn   -> setLocalZOrder(ReadyTowerZorder);
		towerBtn   -> setTouchEnabled(true);
		towerBtn   -> setBright(true);
		CC_BREAK_IF(!trapBtn);
		trapBtn    -> setLocalZOrder(ReadyTrapZorder);
		trapBtn    -> setTouchEnabled(true);
		trapBtn    -> setBright(true);
	}while(0);
}

void Ui_Ready::updateItemInfo(int idx)
{
	mCurReadyItems.clear();
	__Dictionary* events = doEvent104();
	
	//初始化ListView
	listView -> setItemsMargin(0);
	listView -> removeAllItems();

	/*listView ->setBackGroundColorType(LAYOUT_COLOR_SOLID);
	listView ->setBackGroundColor(Color3B::GREEN);*/

	// 如果存在基础item数量限制，没达到这个数量，自动补全
	if (mDefaultItemCount > 0)
	{
		int _itemCount = _itemInfo.at(idx).size();
		if (_itemCount < mDefaultItemCount)
		{
			ReadyItemInfo _info;
			_info.isComing = YES;
			_info.desc = mIsComingText;
			_itemInfo.at(idx).push_back(_info);
		}
	}

	Widget *_item;
	Widget *imageOpen;
	Widget *imageLock;
	Button *_btn;
	float  delay=0;
	for (size_t i=0; i!=_itemInfo.at(idx).size();++i)
	{
		_ReadyItemCellData _thisOne;
		_thisOne.id = _itemInfo.at(idx).at(i).unit_id;
		_thisOne.type = idx;
		_thisOne.isComing = _itemInfo.at(idx).at(i).isComing;
		_thisOne.isDesc = false;
		_thisOne.desc = _itemInfo.at(idx).at(i).desc;
		_thisOne.name = _itemInfo.at(idx).at(i).name;
		_thisOne.price = _itemInfo.at(idx).at(i).price;
		_thisOne.unLockPrice = _itemInfo.at(idx).at(i).unLockPrice;
		_thisOne.lock = _itemInfo.at(idx).at(i).lock;
		if (_itemInfo.at(idx).at(i).isComing==0)
		{
			_item = GUIReader::getInstance()->widgetFromJsonFile("Ui_Ready_Item/Ui_Ready_Item.ExportJson");
			listView -> insertCustomItem(_item,i);
			_btn  = static_cast<Button*>(Helper::seekWidgetByName(_item,"Button_item"));
			_btn->setPressedActionEnabled(false);
			_btn -> setTag(1000*idx + i);
			_btn -> addTouchEventListener(this,toucheventselector(Ui_Ready::selectedUnitEventCallFunc));

			imageOpen = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"ImageView_Open"));
			imageLock = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"Image_Lock"));

			// 如果有事件
			if (events)
			{
				// 清空道具栏
				UserRecord::getInstance()->takeUnitsInit();
				UserRecord::getInstance()->setIsEvent104(true);
				// 如果找到了禁止条件
				bool isFind = false;
				for (int j = 1; j < events->count(); j++)
				{
					auto _key = String::createWithFormat("type%d", j)->getCString();
					auto _typePlist = PlistManager::getInstance()->getPlist(events, _key);
					if (_typePlist)
					{
						int _objType = PlistManager::getInstance()->getPlistCString(_typePlist, "objectType").intValue();
						int _objSelfType = PlistManager::getInstance()->getPlistCString(_typePlist, "selfType").intValue();
						if (_objType > 0 && _itemInfo.at(idx).at(j).objectType == _objType)
						{
							isFind = true;
							break;
						}
					}			
				}
				// 如果找到了禁止条件
				if (isFind)
				{
					_itemInfo.at(idx).at(i).close = false;
					_itemInfo.at(idx).at(i).lock = false;
				}else
				{
					_itemInfo.at(idx).at(i).close = true;
					_itemInfo.at(idx).at(i).lock = false;
				}
			}

			ImageView* _imageStop = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"Image_Stop"));
			_imageStop -> setVisible(false);
			if (_itemInfo.at(idx).at(i).close)
			{
				_imageStop->setVisible(true);
				_btn->setTouchEnabled(false);
			}else
			{
				if (_itemInfo.at(idx).at(i).lock){
					//_btn->setGrey(0.0f);
					_btn->setTouchEnabled(true);
					_btn->setPressedActionEnabled(false);
					_btn->addTouchEventListener(this,toucheventselector(Ui_Ready::lockItemClickCallFunc));

					imageOpen->setVisible(false);
					imageLock->setVisible(true);
				}
			}

			ImageView* _imageIsReady = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"Image_IsReady"));
			_imageIsReady->setTag(100*idx + i);
			if (_itemInfo.at(idx).at(i).selected){
				_imageIsReady->setVisible(true);
			}

			ImageView *icon = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"unitIcon"));
			icon -> loadTexture(String::createWithFormat("roleIcon/icon_%s.png",_itemInfo.at(idx).at(i).unit_id.c_str())->getCString(),UI_TEX_TYPE_LOCAL);
			//icon->setScale(1.2f);

			Text      *name = static_cast<Text*>(Helper::seekWidgetByName(_item,"unit_name"));
			name -> setText(_itemInfo.at(idx).at(i).name);
			name -> setVisible(false);

			/*TextBMFont      *price = static_cast<TextBMFont*>(Helper::seekWidgetByName(_item,"unitPrice"));
			price ->setText(String::createWithFormat("%d",_itemInfo.at(idx).at(i).price)->getCString());*/

			/*TextBMFont      *unLockMenoy = static_cast<TextBMFont*>(Helper::seekWidgetByName(_item,"openPrice"));
			unLockMenoy ->setText(String::createWithFormat("%d",_itemInfo.at(idx).at(i).unLockPrice)->getCString());*/
		}else{
			_item = GUIReader::getInstance()->widgetFromJsonFile("Ui_Ready_Item_1/Ui_Ready_Item_1.ExportJson");
			listView -> insertCustomItem(_item,i);
			_btn  = static_cast<Button*>(Helper::seekWidgetByName(_item,"Button_item"));
			_btn->setPressedActionEnabled(false);
			_btn -> setTag(1000*idx + i);
			_btn -> addTouchEventListener(this,toucheventselector(Ui_Ready::selectedNullEventCallFunc));

		}
		_thisOne.itemCell = _item;
		mCurReadyItems.push_back(_thisOne);

		fadeInAction(_item,delay);
		/*_btn -> runAction(Sequence::create(DelayTime::create(delay),ScaleBy::create(0.2f,1.05f),ScaleTo::create(0.1f,1.0f),NULL)); */

		delay += 0.15f;
	}

	// 默认选中第一个
	for (int i = 0; i != mCurReadyItems.size(); i++)
	{
		auto _thisOne = mCurReadyItems.at(i);
/*		CCLOG("id = %s %d %d",_thisOne.id.c_str(), _thisOne.type, _thisOne.isComing);*/
		if (_thisOne.isComing != YES)
		{
			selectDesc(_thisOne.id);
			break;
		}
	}
}

void Ui_Ready::fadeInAction(Node *pSender,float delay)
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
		//audioPlayer::getInstance()->playEffect("S6.mp3");

	}),ScaleBy::create(0.2f,1.05f),ScaleBy::create(0.1f,1.0/1.05f),NULL));
}

void Ui_Ready::selectedUnitEventCallFunc(Ref *pSender , TouchEventType type)
{
	if (type==TOUCH_EVENT_BEGAN){
		Button    *clickedUnit = static_cast<Button*>(pSender);
		Widget    *panel       = static_cast<Widget*>(clickedUnit->getParent());
		ImageView* _imageIsReady = static_cast<ImageView*>(Helper::seekWidgetByName(clickedUnit,"Image_IsReady"));

		int idx = _imageIsReady->getTag()/100;
		int _i  = _imageIsReady->getTag()-100*idx;
		bool _isDesc = isDesc(_itemInfo.at(idx).at(_i).unit_id);

		if (_imageIsReady->isVisible() && _itemInfo.at(idx).at(_i).selected){
			if (_isDesc)
			{
				audioPlayer::getInstance()->playEffect("S8.mp3");//点击声音
				CCLOG("1");
				_imageIsReady->setVisible(false);
				_itemInfo.at(idx).at(_i).selected=false;
				//seatidx change
				changeSeatIdx(_itemInfo.at(idx).at(_i).unit_id);
			}else
			{
				CCLOG("2");
				audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
				// 选中项，说明状态
				selectDesc(_itemInfo.at(idx).at(_i).unit_id);	
			}
		}else if (!_imageIsReady->isVisible() && !_itemInfo.at(idx).at(_i).selected){
			if (_isDesc)
			{
				audioPlayer::getInstance()->playEffect("S7.mp3");//点击声音
				CCLOG("3");
				if (_vecObj.size()<seatCount){
					_imageIsReady->setVisible(true);	
					_itemInfo.at(idx).at(_i).selected=true;
					_vecObj.insert(make_pair(_itemInfo.at(idx).at(_i).unit_id,occupySeatIndex+1));
					insertObjsIcon();
					occupySeatIndex +=1;
				}
			}else
			{
				// 选中项，说明状态
				CCLOG("4");
				audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
				selectDesc(_itemInfo.at(idx).at(_i).unit_id);	
			}
		}
	}
}

void Ui_Ready::selectedNullEventCallFunc(Ref *pSender , TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;
	// 先取id
	string _curId = "";
	Button* _curButton = static_cast<Button*>(pSender);
	for (int i = 0; i != mCurReadyItems.size(); i++)
	{
		auto _thisOne = mCurReadyItems.at(i);
		Button* _cellButton = static_cast<Button*>(Helper::seekWidgetByName(_thisOne.itemCell,"Button_item"));
		ImageView* _imageIsSelect = static_cast<ImageView*>(Helper::seekWidgetByName(_thisOne.itemCell,"Image_IsSelect"));
		if (_imageIsSelect)
		{
			WindowManager::getInstance()->setTextureAlias(_imageIsSelect);
			if (_curButton == _cellButton)
			{
				_curId = _thisOne.id;
				break;
			}
		}
	}
	if (_curId == "")
		return;
	// 判断是否说明状态
	bool _isDesc = isDesc(_curId);
	if (!_isDesc)
	{
		// 进入说明状态
		selectDesc(_curId);
		return;
	}else
	{
		// 非说明状态没有功能
	}
}


void Ui_Ready::lockItemClickCallFunc(Ref *pSender , TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;
	// 先取id
	string _curId = "";
	Button* _curButton = static_cast<Button*>(pSender);
	for (int i = 0; i != mCurReadyItems.size(); i++)
	{
		auto _thisOne = mCurReadyItems.at(i);
		Button* _cellButton = static_cast<Button*>(Helper::seekWidgetByName(_thisOne.itemCell,"Button_item"));
		Widget* _imageIsSelect = static_cast<Widget*>(Helper::seekWidgetByName(_thisOne.itemCell,"Image_IsSelect"));
		if (_imageIsSelect)
		{
			if (_curButton == _cellButton)
			{
				_curId = _thisOne.id;
				break;
			}
		}
	}
	if (_curId == "")
		return;
	// 判断是否说明状态
	bool _isDesc = isDesc(_curId);
	if (!_isDesc)
	{
		// 进入说明状态
		selectDesc(_curId);
		return;
	}else
	{
		// 解锁换地方了
	}
}

void Ui_Ready::confirmUnLockItem()
{
	int idx = clickedItemTag/1000;
	int _i  = clickedItemTag - idx*1000;

	if ( UserRecord::getInstance()->getGold() >= _itemInfo.at(idx).at(_i).unLockPrice ){
		//连接服务器
		auto postDatas = __String::createWithFormat("c=unlock&oid=%s",_itemInfo.at(idx).at(_i).unit_id.c_str())->getCString();
		Connect* handle = Connect::getInstance();
		handle->loadSend(postDatas, this, httpresponse_selector(Ui_Ready::onHttpRequestCompleted));
	}else{
		auto Text = PlistManager::getInstance()->getPlistMap("Text");
		__String* TextStr = static_cast<__String*>(Text->objectForKey("1003"));
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->setTag(251);
		prompt->setRightSelector(this,comm_selector(Ui_Ready::goldNoEnoughCallfunc));
		prompt->initWithTwoButtons(TextStr->getCString());
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	} 
}


void Ui_Ready::setSeatCount(int num)
{
	Widget *panel5 = Helper::seekWidgetByName(m_widget,"Panel_5");
	Widget *panel6 = Helper::seekWidgetByName(m_widget,"Panel_6");
	Widget *panel7 = Helper::seekWidgetByName(m_widget,"Panel_7");

	switch (num){

	case 5:
		panel6 ->setVisible(false);
		panel7 ->setVisible(false);
		panel6 ->setPositionY(-1000);
		panel7 ->setPositionY(-1000);
		currWidget=panel5;
		seatCount=5;
		panel5 ->setVisible(true);
		panel5 ->setPositionY(bottomPosY);
		break;
	case 6:

		panel5 ->setVisible(false);
		panel7 ->setVisible(false);
		panel5 ->setPositionY(-1000);
		panel7 ->setPositionY(-1000);
		currWidget=panel6;
		seatCount=6;
		panel6 ->setVisible(true);
		panel6 ->setPositionY(bottomPosY);
		break;
	case 7:
		panel5 ->setVisible(false);
		panel6 ->setVisible(false);
		panel5 ->setPositionY(-1000);
		panel6 ->setPositionY(-1000);
		currWidget=panel7;
		seatCount=7;
		panel7 ->setVisible(true);
		panel7 ->setPositionY(bottomPosY);
		break;
	default:
		panel6 ->setVisible(false);
		panel7 ->setVisible(false);
		panel6 ->setPositionY(-1000);
		panel7 ->setPositionY(-1000);
		currWidget=panel5;
		seatCount=5;
		panel5 ->setVisible(true);
		panel5 ->setPositionY(bottomPosY);
		break;
	}

	beginGameBtn  = static_cast<Button*>(Helper::seekWidgetByName(currWidget,"Button_begin")); 
	addItemBtn = static_cast<Button*>(Helper::seekWidgetByName(currWidget,"Button_add"));
	beginGameBtn->setPressedActionEnabled(true);
	addItemBtn  ->setPressedActionEnabled(true); 
	beginGameBtn->addTouchEventListener(this,toucheventselector(Ui_Ready::GameStartCallFunc));
	addItemBtn  ->addTouchEventListener(this,toucheventselector(Ui_Ready::addSeatCountCallFunc));

	Button *objBtn;
	for (int i=1;i<seatCount+1;++i){
		objBtn = static_cast<Button*>(Helper::seekWidgetByName(currWidget,String::createWithFormat("Button_ready%d",i)->getCString()));
		objBtn ->setPressedActionEnabled(false);
		objBtn ->addTouchEventListener(this,toucheventselector(Ui_Ready::objBtnsEventCallFuc));

		ImageView*unitIcon    = static_cast<ImageView*>(objBtn->getChildByName("Image_icon"));
		unitIcon->setPositionY(unitIcon->getPositionY()-2.0);
	}

	//更新栏内携带数据
	insertObjsIcon();

	//如果携带数已经为最大携带数
	if (seatCount==7){
		addItemBtn->setVisible(false);
		addItemBtn->setTouchEnabled(false);
	}
}

void Ui_Ready::shopBtnCallFunc(Ref *pSender , TouchEventType type)
{
	if (type==TOUCH_EVENT_ENDED){
		audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
		auto child = Director::getInstance()->getRunningScene()->getChildByTag(124);
		if (child){
			Director::getInstance()->getRunningScene()->removeChild(child);
		}

		Ui_Shop* shopLayer = Ui_Shop::create();
		shopLayer  -> setTag(124);
		Director::getInstance()->getRunningScene() -> addChild(shopLayer,101);
		shopLayer  -> selectIndex(ShopItem::SoulItem);
	}
}

void Ui_Ready::goldNoEnoughCallfunc()
{
	auto child = Director::getInstance()->getRunningScene()->getChildByTag(124);
	if (child){
		Director::getInstance()->getRunningScene()->removeChild(child);
	}

	Ui_Shop* shopLayer = Ui_Shop::create();
	shopLayer  -> setTag(124);
	Director::getInstance()->getRunningScene() -> addChild(shopLayer,101);
	shopLayer  -> selectIndex(ShopItem::SoulItem);
}

void Ui_Ready::GameStartCallFunc(Ref *pSender , TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;
	audioPlayer::getInstance()->playEffect("S2.mp3");
	UserRecord::getInstance()->takeUnitsInit();
	std::map<int,std::string> _map;
	for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
		_map.insert(make_pair(iter->second,iter->first));
	}
	UserRecord::getInstance()->setTakeUnits(_map);

	beginGameBtn->setGrey();

	//load plist
	CachManager::getInstance()->freshCach();

	Scene* pTScene =LoadingScene::createScene();
	LoadingScene*  cl = static_cast<LoadingScene*> (pTScene->getChildByTag(6));
	cl->loadScene(GAME_SCENE_TAG);
	TransitionFade* pTranslateScene = TransitionFade::create(0.6f,pTScene);;
	Director::getInstance()->replaceScene(pTranslateScene);

	//fourth post finger
	if  (!UserRecord::getInstance()->getIsFourthScene() && UserRecord::getInstance()->getSceneNum()>=4){
		UserRecord::getInstance()->setIsFourthScene(true);
		UserRecord::getInstance()->flush();
	}
}

void Ui_Ready::addSeatCountCallFunc(Ref *pSender , TouchEventType type)
{
	auto m_child = Director::getInstance()->getRunningScene()->getChildByTag(251);
	if (m_child!=NULL){
		Director::getInstance()->getRunningScene()->removeChild(m_child);
	}
	audioPlayer::getInstance()->playEffect("S1.mp3");
	Ui_Prompt *prompt = Ui_Prompt::create();
	prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	prompt->setTag(251);
	prompt->setRightSelector(this,comm_selector(Ui_Ready::buyPromptCallFunc));
	auto Text = PlistManager::getInstance()->getPlistMap("Text");
	__String* TextStr =  static_cast<__String*>(Text->objectForKey("1005"));
	prompt->initWithTwoButtons(TextStr->getCString());
	Director::getInstance()->getRunningScene()->addChild(prompt,300);
}

void Ui_Ready::buyPromptCallFunc()
{
	//当金币数量足够时，购买英雄携带栏
	if (UserRecord::getInstance()->getGold()>=addSeatCost){
		if (seatCount<7){
			//连接服务器
			auto postDatas = __String::createWithFormat("c=unlock&a=seat&num=%d",UserRecord::getInstance()->getSeatCount())->getCString();
			Connect* handle = Connect::getInstance();
			handle->loadSend(postDatas,this,httpresponse_selector(Ui_Ready::onHttpRequestAddSeat));
		}
	}else{

		auto Text = PlistManager::getInstance()->getPlistMap("Text");
		__String* TextStr = static_cast<__String*>(Text->objectForKey("1003"));
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->setTag(251);
		prompt->setRightSelector(this,comm_selector(Ui_Ready::goldNoEnoughCallfunc));
		prompt->initWithTwoButtons(TextStr->getCString());
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}

void Ui_Ready::objBtnsEventCallFuc(Ref *pSender , TouchEventType type)
{
	if (type==TOUCH_EVENT_ENDED){
		Button *btn = static_cast<Button*>(pSender);
		audioPlayer::getInstance()->playEffect("S8.mp3");
		if (strcmp(btn->getName(),"Button_ready1")==0){

			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==1){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}else if(strcmp(btn->getName(),"Button_ready2")==0){
			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==2){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}else if(strcmp(btn->getName(),"Button_ready3")==0){
			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==3){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}else if(strcmp(btn->getName(),"Button_ready4")==0){
			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==4){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}else if(strcmp(btn->getName(),"Button_ready5")==0){
			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==5){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}else if(strcmp(btn->getName(),"Button_ready6")==0){
			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==6){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}else if(strcmp(btn->getName(),"Button_ready7")==0){
			for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
				if (iter->second==7){
					hideSelectFrame(iter->first);
					changeSeatIdx(iter->first);
					break;
				}
			}
		}
	}
}

void Ui_Ready::insertObjsIcon()
{
	Button    *objBtn;
	ImageView *unitIcon;
	ImageView *_priceBg;
	TextBMFont *price;

	for (int i=1;i!=seatCount+1;++i){
		objBtn      = static_cast<Button*>(Helper::seekWidgetByName(currWidget,String::createWithFormat("Button_ready%d",i)->getCString()));
		objBtn      ->setVisible(true);
		objBtn      ->setPressedActionEnabled(false);
		unitIcon    = static_cast<ImageView*>(objBtn->getChildByName("Image_icon"));
		
		_priceBg    = static_cast<ImageView*>(objBtn->getChildByName("Image_price_bk")); 
		unitIcon    ->setVisible(false);
		_priceBg    ->setVisible(false);
	}


	for(auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
		objBtn   = static_cast<Button*>(Helper::seekWidgetByName(currWidget,String::createWithFormat("Button_ready%d",iter->second)->getCString()));
		objBtn   ->setPressedActionEnabled(true);
		unitIcon = static_cast<ImageView*>(objBtn->getChildByName("Image_icon"));

		unitIcon -> loadTexture(String::createWithFormat("roleIcon/icon_%s.png",iter->first.c_str())->getCString(),UI_TEX_TYPE_LOCAL);
		unitIcon -> setVisible(true);
		//unitIcon -> setScale(0.8f);

		

		_priceBg = static_cast<ImageView*>(objBtn->getChildByName("Image_price_bk")); 
		_priceBg->setLocalZOrder(10);
		_priceBg->setVisible(true);

		price = static_cast<TextBMFont*>(_priceBg->getChildByName("Label_price"));

		for (int i=1;i!=4;++i){
			for (size_t j=0;j!=_itemInfo.at(i).size();++j){
				if (strcmp(_itemInfo.at(i).at(j).unit_id.c_str(),iter->first.c_str())==0){
					price->setText(String::createWithFormat("%d",_itemInfo.at(i).at(j).price)->getCString());
					break;
				}
			}
		}
	}

	if (_vecObj.size() <5 ){
		if (!beginGameBtn->_isgrey)
			beginGameBtn->setGrey();
	}else{
		if(beginGameBtn->_isgrey) 
			beginGameBtn->removeGrey();
	}
}

void Ui_Ready::RemoveObjsIcon(int idx)
{
	Button    *objBtn   = static_cast<Button*>(Helper::seekWidgetByName(currWidget,String::createWithFormat("Button_ready%d",idx)->getCString()));
	ImageView *unitIcon = static_cast<ImageView*>(objBtn->getChildByName("Image_icon"));
	ImageView *_priceBg = static_cast<ImageView*>(objBtn->getChildByName("Image_price_bk")); 
	TextBMFont      *price    = static_cast<TextBMFont*>(_priceBg->getChildByName("Label_price"));
	objBtn->setVisible(false);

	Button    *actionBtn= Button::create();
	actionBtn->loadTextureNormal(objBtn->getFrameName());
	actionBtn->setPosition(objBtn->getPosition());
	objBtn->getParent()->addChild(actionBtn,objBtn->getLocalZOrder()+1);

	ImageView *actionUnitIcon = ImageView::create();
	actionUnitIcon->loadTexture(unitIcon->getFrameName(),UI_TEX_TYPE_LOCAL);
	actionUnitIcon->setPosition(unitIcon->getPosition());
	//actionUnitIcon->setScale(0.8f);
	actionBtn->addChild(actionUnitIcon);
/*
	ImageView *actionPriceBg = ImageView::create();
	actionPriceBg->loadTexture(_priceBg->getFrameName());
	actionPriceBg->setPosition(_priceBg->getPosition());
	actionBtn->addChild(actionPriceBg);

	Text *actionPrice = Text::create();
	actionPrice->setText(price->getStringValue());
	actionPrice->setPosition(price->getPosition());
	actionPrice->setColor(price->getColor());
	actionPriceBg->addChild(actionPrice,2);
*/
	ccBezierConfig bezier;
	bezier.controlPoint_1 = Point(0, objBtn->getSize().height*5);
	bezier.controlPoint_2 = Point(200, -300);
	bezier.endPosition = Point(200,-300);

	actionBtn->runAction(RotateBy::create(1.0f,300));
	actionBtn->runAction(Sequence::create(DelayTime::create(0.3f),
										  CallFunc::create(CC_CALLBACK_0(Ui_Ready::insertObjsIcon,this)),
										  NULL
		                                 ));
	actionBtn->runAction(Sequence::create(BezierBy::create(1.0f,bezier),
										  CallFuncN::create(CC_CALLBACK_1(Ui_Ready::ObjsMoveCallFunc,this)),
										  NULL
										 ));

}

void Ui_Ready::ObjsMoveCallFunc(Ref *pSender)
{
	Button    *actionBtn   = static_cast<Button*>(pSender);
	actionBtn->removeFromParentAndCleanup(true);
}

void Ui_Ready::changeSeatIdx(string key)
{
	int idx = _vecObj.at(key);
	if (idx<1 || idx>seatCount)
		return;

	if (idx<occupySeatIndex){
		for (auto iter=_vecObj.begin();iter!=_vecObj.end();++iter){
			if (iter->second>idx){
				iter->second -= 1;
			}
		}
	}

	_vecObj.erase(key);
	occupySeatIndex -= 1;

	RemoveObjsIcon(idx);
}

void Ui_Ready::hideSelectFrame(string unit_id)
{
	for (size_t i=1;i!=_itemInfo.size()+1;++i){
		for (size_t j=0;j!=_itemInfo.at(i).size();++j){
			if ( strcmp( _itemInfo.at(i).at(j).unit_id.c_str() ,unit_id.c_str() )==0 ){
				 _itemInfo.at(i).at(j).selected = false;

				 if ((int)getSelectedItem()==i){
					 for (auto item:listView->getItems()){
						 if (item){
							 Widget* _imageIsReady = static_cast<ImageView*>(Helper::seekWidgetByName(item,"Image_IsReady"));
							 if ( _imageIsReady->getTag() == 100*i + j ){
								 _imageIsReady->setVisible(false);
								 break;
							 }
						 }
					 } 
				 }
				 break;
			}
		}
	}
}

void Ui_Ready::showSelectFrame(string unit_id)
{
	for (size_t i=1;i!=_itemInfo.size()+1;++i){
		for (size_t j=0;j!=_itemInfo.at(i).size();++j){
			if ( strcmp( _itemInfo.at(i).at(j).unit_id.c_str() ,unit_id.c_str() )==0 ){
				_itemInfo.at(i).at(j).selected = true;

				if ((int)getSelectedItem()==i){
					for (auto item:listView->getItems()){
						if (item){
							Widget* _imageIsReady = static_cast<ImageView*>(Helper::seekWidgetByName(item,"Image_IsReady"));
							if ( _imageIsReady->getTag() == 100*i + j ){
								_imageIsReady->setVisible(true);
								break;
							}
						}
					} 
				}
				break;
			}
		}
	}
}

void Ui_Ready::close(Ref *pSender , TouchEventType type)
{

	if (type!=TOUCH_EVENT_ENDED)
		return;
	audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
	Scene* pTScene = levelScene::createScene();
	TransitionFade *transitionfadeScene=TransitionFade::create(0.6,pTScene);
	Director::getInstance()->replaceScene(transitionfadeScene);
}

//增加栏位回调
void  Ui_Ready::onHttpRequestAddSeat(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		setSeatCount(seatCount+1);
		goldNum->setText(String::createWithFormat("%d",UserRecord::getInstance()->getGold()-addSeatCost)->getCString());
		UserRecord::getInstance()->setGold(UserRecord::getInstance()->getGold()-addSeatCost);
		UserRecord::getInstance()->setSeatCount(seatCount);
	} else {
		
		//服务器返回错误
	}
}
//解锁单位回调
void Ui_Ready::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	CCLOG("%s",&(*buffer->begin()));
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		int idx = clickedItemTag/1000;
		int _i  = clickedItemTag - idx*1000;
		_itemInfo.at(idx).at(_i).lock = false;
		selectIndex(getSelectedItem());
		goldNum->setText(String::createWithFormat("%d",UserRecord::getInstance()->getGold()- _itemInfo.at(idx).at(_i).unLockPrice)->getCString());
		UserRecord::getInstance()->setGold(UserRecord::getInstance()->getGold()- _itemInfo.at(idx).at(_i).unLockPrice);
		UserRecord::getInstance()->setObjectsUnLock(_itemInfo.at(idx).at(_i).unit_id,_itemInfo.at(idx).at(_i).name);
	} else {
		string _prompt = v["info"].GetString();
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->setTag(251);
		//prompt->setRightSelector(this,comm_selector(Ui_Ready::goldNoEnoughCallfunc));
		prompt->initWithTwoButtons(_prompt.c_str());
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
	return;
}
// event104 事件
__Dictionary* Ui_Ready::doEvent104()
{
	UserRecord* user = UserRecord::getInstance();
	int _nowRound = user->getRound();
	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	if(!data)
		return nullptr;
	Value currentSection(user->getSectionNum());
	Value currentScene(user->getSceneNum());
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	if(!curr_section)
		return nullptr;
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	if(!allScene)
		return nullptr;
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	if(!curr_scene)
		return nullptr;
	__Dictionary* events = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("event"));
	if(!events)
		return nullptr;

	for (int i = 1 ; i <= events->count(); ++i)
	{
		Value _eventKey(i);
		auto _curObject = events->objectForKey(_eventKey.asString());
		if (_curObject)
		{
			// 取事件id
			__String* _objectString = dynamic_cast<__String*>(_curObject);
			int _eventId = _objectString->intValue();
			if (_eventId == 104 || _eventId == 107 || _eventId == 108)
			{
				auto _eventPlist = PlistManager::getInstance()->getPlistMap("EventData");
				if (_eventPlist)
				{
					auto _idPlist = PlistManager::getInstance()->getPlist(_eventPlist, _objectString->getCString());
					if (_idPlist)
					{
						return _idPlist;
					}
				}
			}
		}
	}
	return nullptr;
}
// 选中Desc备注项
void Ui_Ready::selectDesc(string unit_id)
{
	mCurSelectId = unit_id;
	for (int i = 0; i != mCurReadyItems.size(); i++)
	{
		auto _thisOne = mCurReadyItems.at(i);
		Widget* _imageIsSelect = static_cast<Widget*>(Helper::seekWidgetByName(_thisOne.itemCell,"Image_IsSelect"));
		Widget* _imageIsBlack = static_cast<Widget*>(Helper::seekWidgetByName(_thisOne.itemCell,"Image_IsBlack"));
		if (_imageIsSelect)
		{
			if (_thisOne.id == unit_id)
			{
				_imageIsSelect->setVisible(true);
				_imageIsBlack->setVisible(false);
				selectDescAction(_thisOne.itemCell, true);
				mCurReadyItems.at(i).isDesc = true;
				string str = _thisOne.desc;
				while ( str.find_first_of("_n")!=string::npos )
				{
					size_t pos1 = str.find_first_of("_n"); 
					str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
				}
				mBitmapLabelDesc->setText(str);
				mBitmapLabelName->setText(_thisOne.name);
				// 默认解锁和创建价格都不显示
				mButtonUnLock->setVisible(false);
				mButtonUnLock->setTouchEnabled(false);
				mButtonUnLock->setGrey(0);
				ImageView* _imageCreateBack = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_CreateBack"));
				_imageCreateBack->setVisible(false);
				// 是否可以显示解锁
				bool _isShow_UnLock = false;
				if (_thisOne.lock)
				{
					_isShow_UnLock = true;
				}
				if (_isShow_UnLock)
				{
					mButtonUnLock->setVisible(true);
					// 更新解锁价格
					int _costGold = _thisOne.unLockPrice;
					auto _unlockString = String::createWithFormat("%d", _costGold)->getCString();
					mBitmapLabelUnLockCost->setText(_unlockString);
					// 是否可以点击解锁
					bool _isClick_UnLock = false;
					int _curGold = UserRecord::getInstance()->getGold();	
					if (_curGold >= _costGold && !_thisOne.isComing)
					{
						_isClick_UnLock = true;
						mButtonUnLock->setTouchEnabled(true);
						mButtonUnLock->removeGrey();
					}
				}else
				{
					_imageCreateBack->setVisible(true);
					// 更新创建价格
					TextBMFont* _textCreateCost = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"BitmapLabel_CreateCost"));
					if (_textCreateCost)
					{
						auto _priceString = String::createWithFormat("%d",_thisOne.price)->getCString();
						_textCreateCost->setText(_priceString);
					}
				}
			}else
			{
				_imageIsSelect->setVisible(false);
				_imageIsBlack->setVisible(true);
				selectDescAction(_thisOne.itemCell, false);
				mCurReadyItems.at(i).isDesc = false;
			}
		}	
	}
}
// 选中Desc备注项
void Ui_Ready::selectDescAction(Widget* _widget, bool _isBig)
{
	if (!_widget)
		return;
	float _scale = 0;
	_widget->stopActionByTag(3);
	_widget->setAnchorPoint(Point(0.5f, 0.5f));
	if (_isBig)
	{
		_scale = mDefaultItemScale + 0.1f;
		auto _act = ScaleTo::create(0.15f, _scale, _scale);
		auto _actions = Sequence::create(_act, CallFuncN::create(CC_CALLBACK_1(Ui_Ready::selectDescActionCallBack_Big, this)), NULL);
		_widget->runAction(_actions);
	}else
	{
		_scale = mDefaultItemScale;
		auto _act = ScaleTo::create(0.15f, _scale, _scale);
		_widget->runAction(_act);
	}
}
// 选中Desc备注项动画回调, 放大回调
void Ui_Ready::selectDescActionCallBack_Big(Ref* sender)
{
	// 继续播放来回收缩
	Button* _curButton = static_cast<Button*>(sender);
	float _scale1 = _curButton->getScale() + 0.05f;
	float _scale2 = _curButton->getScale() + 0.03f;
	auto _act1 = ScaleTo::create(0.5f, _scale1, _scale1);
	auto _act2 = ScaleTo::create(0.5f, _scale2, _scale2);
	auto action = Sequence::create(_act1, _act2, NULL);
	RepeatForever *repeat = RepeatForever::create(action); 
	repeat->setTag(3);
	_curButton->runAction(repeat);
}
// 是否Desc说明状态
bool Ui_Ready::isDesc(string unit_id)
{
	for (int i = 0; i != mCurReadyItems.size(); i++)
	{
		auto _thisOne = mCurReadyItems.at(i);
		if (_thisOne.isDesc)
		{
			if (_thisOne.id == unit_id)
			{
				return true;
			}
		}
	}
	return false;
}
// 创建动态箭头
void Ui_Ready::createAnimArrows()
{
	SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("bossBaseImage.plist");//添加箭头的
	mArrows = Sprite::createWithSpriteFrameName("JianTou.png");
	ImageView* _imageAnimBack = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_18"));
	_imageAnimBack->addChild(mArrows, 1);
	mArrows->setPositionX(_imageAnimBack->getSize().width/2);
	mArrows->setPositionY(10);
	/*mArrows->setScale((float)0.5);*/
	auto mb1 = MoveBy::create((float)0.5,Point(0,5));
	auto mb2 = MoveBy::create((float)0.5,Point(0,-5));
	auto action = Sequence::create(mb1, mb2, NULL);
	RepeatForever *repeat = RepeatForever::create(action); 
	mArrows->runAction(repeat);
}
// 取当前显示备注项的选中
int Ui_Ready::getDescSelectIndex(string _id)
{
	int _index = -1;

	for (int i = 0; i != mCurReadyItems.size(); i++)
	{
		auto _thisOne = mCurReadyItems.at(i);
		Widget* _imageIsSelect = static_cast<Widget*>(Helper::seekWidgetByName(_thisOne.itemCell,"Image_IsSelect"));
		if (_imageIsSelect)
		{
			if (_thisOne.id == mCurSelectId)
			{
				_index = i;
				return _index;
			}
		}
	}
	return _index;
}
// 点击了解锁按钮
void Ui_Ready::onButtonClick_UnLock(Ref *pSender , TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	int _index = getDescSelectIndex(mCurSelectId);
	if (_index < 0)
		return;

	auto _thisOne = mCurReadyItems.at(_index);
	if (!_thisOne.itemCell)
		return;

	Button* _cellButton = static_cast<Button*>(Helper::seekWidgetByName(_thisOne.itemCell,"Button_item"));
	auto m_child = Director::getInstance()->getRunningScene()->getChildByTag(251);
	if (m_child!=NULL){
		Director::getInstance()->getRunningScene()->removeChild(m_child);
	}
	clickedItemTag = _cellButton->getTag();

	Ui_Prompt *prompt = Ui_Prompt::create();
	prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	prompt->setTag(251);

	auto Text = PlistManager::getInstance()->getPlistMap("Text");
	__String* TextStr = static_cast<__String*>(Text->objectForKey("1006"));

	prompt->initWithTwoButtons(TextStr->getCString());
	prompt->setRightSelector(this,comm_selector(Ui_Ready::confirmUnLockItem));
	Director::getInstance()->getRunningScene()->addChild(prompt,300);
}
// 更新关卡数字
void Ui_Ready::updateSceneNumber()
{
	ImageView* _imageBatch = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Batch"));
	if (_imageBatch)
	{
		int _number = UserRecord::getInstance()->getSceneNum();
		if (_number > 0 && _number <= 15)
		{
			string _numberString = "image/zi_0001.png";
			if (_number > 0 && _number <= 9)
			{
				_numberString = String::createWithFormat("image/zi_000%d.png",_number)->getCString();
			}else if (_number >= 10 && _number <= 15)
			{
				_numberString = String::createWithFormat("image/zi_00%d.png",_number)->getCString();
			}
			_imageBatch->loadTexture(_numberString);
		}
	}
}
// 创建控件动作
void Ui_Ready::createLineAction(Widget* _widget, float _speedTime, int _disY)
{
	if (!_widget)
		return;
	auto mb1 = MoveBy::create((float)_speedTime,Point(0,_disY));
	auto mb2 = MoveBy::create((float)_speedTime,Point(0,-_disY));
	auto action = Sequence::create(mb1, mb2, NULL);
	RepeatForever *repeat = RepeatForever::create(action); 
	_widget->runAction(repeat);
}
