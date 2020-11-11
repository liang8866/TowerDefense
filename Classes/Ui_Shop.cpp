#include "Ui_Shop.h"
#include "Ui_Prompt.h"
#include "UserRecord.h"
#include "PlistManager.h"
#include "Ui_HeroAltar.h"
#include "WindowManager.h"
USING_NS_CC;
using namespace network;
Ui_Shop::Ui_Shop():m_widget(NULL),
listView(NULL),
clickedBtnTag(-1),
noEnoughString("")
{

}

Ui_Shop::~Ui_Shop()
{
	NotificationCenter::getInstance()->removeObserver(this, BUYSOULSUCCESS);
}

bool Ui_Shop::init()
{
	if (!Widget::init())
		return false;
	_isFirstTime=YES;
	mHeroAltarWindow = nullptr;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Shop/GameUI0.plist","Ui_Shop/GameUI0.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Shop_item/GameUI0.plist","Shop_item/GameUI0.png");
	m_widget  = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_Shop/Ui_Shop.ExportJson"));
	listView  = static_cast<ListView*>(Helper::seekWidgetByName(m_widget,"ListView_Shop"));

	sweetBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Sweet"));
	magicBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Magic"));
	materialBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Material"));
	boxBtn    = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Box"));
	soulBtn   = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Soul"));
	goldBtn   = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Gold"));
	clothesBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Clothes"));

	sweetBtn    ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));
	magicBtn    ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));
	materialBtn ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));
	boxBtn      ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));
	goldBtn     ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));
	soulBtn     ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));
	clothesBtn  ->addTouchEventListener(this,toucheventselector(Ui_Shop::itemBtnClickedCallFuc));

	Button *closeBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Close"));
	closeBtn->addTouchEventListener(this,toucheventselector(Ui_Shop::close));
	closeBtn->setPressedActionEnabled(true);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float hscale = visibleSize.height/640;
	closeBtn->setScale(hscale);


	static_cast<Sprite*>(closeBtn->getVirtualRenderer())->getTexture()->setAliasTexParameters();

	Text *soul = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"soul_num"));
	Text *gold = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"gold_num"));
	soul -> setText(String::createWithFormat("%d",UserRecord::getInstance()->getHeroSoul())->getCString());
	gold -> setText(String::createWithFormat("%d",UserRecord::getInstance()->getGold())->getCString());
	soul->setFontName(MyfontName);
	soul->setColor(Color3B(255,255,255));
	soul->setFontSize(19);

	gold->setFontName(MyfontName);
	gold->setColor(Color3B(255,255,255));
	gold->setFontSize(19);

	this->addChild(m_widget);
	m_widget->setTouchEnabled(true);
	m_widget->setTouchPriority(TOUCH_UI_PRIORITY-2);

	fitScreen();

	ImageView* _image21 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_21"));
	WindowManager::getInstance()->setTextureAlias(_image21);

	// 设置动态横条
	ImageView* _imageLine1 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line1"));
	createMoveAction(_imageLine1, 0.7, 7);
	ImageView* _imageLine2 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line2"));
	createMoveAction(_imageLine2, 0.8, 6);
	ImageView* _imageLine3 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line3"));
	createMoveAction(_imageLine3, 0.6, 3);
	ImageView* _imageLine4 = static_cast<ImageView*>(Helper::seekWidgetByName(m_widget, "Image_Line4"));
	createMoveAction(_imageLine4, 1.0, 8);
	// 设置动态按钮
	Button* _buttonLeft = static_cast<Button*>(Helper::seekWidgetByName(m_widget, "Button_Left"));
	//createScaleAction(_buttonLeft, 1.0, 0.02f);
	Button* _buttonRight = static_cast<Button*>(Helper::seekWidgetByName(m_widget, "Button_Right"));
	//createScaleAction(_buttonRight, 1.0, 0.02f);

	
	_buttonRight->setScale(hscale);
	_buttonLeft->setScale(hscale);
	static_cast<Sprite*>(_buttonLeft->getVirtualRenderer())->getTexture()->setAliasTexParameters();
	static_cast<Sprite*>(_buttonRight->getVirtualRenderer())->getTexture()->setAliasTexParameters();

	scheduleOnce(schedule_selector(Ui_Shop::pauseGame),0.5f);

	//添加广播
	NotificationCenter::getInstance()->addObserver( this,
													callfuncO_selector(Ui_Shop::buySoulSuccessCallfuc),
													BUYSOULSUCCESS,
													NULL);

	return true;
}



void Ui_Shop::onEnter()
{
	Widget::onEnter();
}

void Ui_Shop::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Shop/GameUI0.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Shop_item/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_Shop/GameUI0.png");
	TextureCache::getInstance()->removeTextureForKey("Shop_item/GameUI0.png");
	Widget::onExit();
}

void Ui_Shop::fitScreen()
{
	Size winSize    = Director::getInstance()->getVisibleSize();
	Point originPos = Director::getInstance()->getVisibleOrigin();

	do{
		CC_BREAK_IF(!m_widget);
		Widget* panel_up = Helper::seekWidgetByName(m_widget,"Panel_Up");
		CC_BREAK_IF(!panel_up);
		panel_up->setPosition(Point(originPos.x+winSize.width-panel_up->getSize().width,
								    originPos.y+winSize.height-panel_up->getSize().height));

		Widget* panel_mid = Helper::seekWidgetByName(m_widget,"Panel_Mid"); 
		CC_BREAK_IF(!panel_mid);
		panel_mid->setAnchorPoint(Point(0.5f,0.5f));
		panel_mid->setPosition(Point(originPos.x+winSize.width/2,
									 originPos.y+winSize.height/2
									));
		Widget* bg = m_widget->getChildByName("Panel_bg");
		CC_BREAK_IF(!bg);
		bg->setAnchorPoint(Point::ZERO);
		bg->setPosition(Point(originPos.x,originPos.y));
		bg->setScaleX(winSize.width/960);

		if (winSize.height<640)
		{
			panel_mid->setPositionY(originPos.y+winSize.height/2 - 40);
			return;
		}
		bg->setScaleY(winSize.height/640);
	}while(0);
}

void Ui_Shop::pauseGame(float dt)
{
	NotificationCenter::getInstance()->postNotification(PAUSEKEY, NULL);
}

void Ui_Shop::resumeGame()
{
	NotificationCenter::getInstance()->postNotification(RESUMEKEY, NULL);
}

void Ui_Shop::selectIndex(ShopItem idx)
{
	Button   *btn;
	switch (idx)
	{
	case SweetItem:
		btn  = sweetBtn;
		break;
	case MagicItem:
		btn  = magicBtn;
		break;
	case MaterialItem:
		btn  = materialBtn;
		break;
	case BoxItem:
		btn  = boxBtn;
		break;
	case SoulItem:
		btn  = soulBtn;
		break;
	case GoldItem:
		btn  = goldBtn;
		break;
	case ClothesItem:
		btn  = clothesBtn;
		break;
	default:
		break;
	}
	itemBtnClickedCallFuc(btn , TOUCH_EVENT_ENDED);
}

void Ui_Shop::freshData(ShopItem idx)
{
	string str;
	switch (idx)
	{
	case SweetItem:
		str = "Sweet";
		break;
	case MagicItem:
		str = "Magic";
		break;
	case MaterialItem:
		str = "Material";
		break;
	case BoxItem:
		str = "Box";
		break;
	case GoldItem:
		str = "Gold";
		break;
	case SoulItem:
		str = "Soul";
		break;
	case ClothesItem:
		str = "Clothes";
		break;
	default:
		break;
	}
	__Dictionary *dic = PlistManager::getInstance()->addPlistMap("Shop","plist/shop.plist");
	__Dictionary *obj = PlistManager::getInstance()->getPlist(dic,str.c_str());
	int  itemCtn      = atoi(PlistManager::getInstance()->getPlistCString(obj,"itemCount").getCString());


	//给该条目内数据赋值
	_itemInfo.clear();
	ShopItemInfo _info;
	__Dictionary *_obj; 
	__Dictionary *_exObj;
	for (int i=0;i!=itemCtn;++i){
		_obj = PlistManager::getInstance()->getPlist(obj,String::createWithFormat("item%d",i+1)->getCString());
		_info.shop_id = PlistManager::getInstance()->getPlistCString(_obj,"shopId").getCString();
		_info.item_id = PlistManager::getInstance()->getPlistCString(_obj,"itemID").getCString();
		_info.count   = atoi(PlistManager::getInstance()->getPlistCString(_obj,"count").getCString());
		_info.price   = atoi(PlistManager::getInstance()->getPlistCString(_obj,"price").getCString()); 
		_info.payType = atoi(PlistManager::getInstance()->getPlistCString(_obj,"payType").getCString());
		_info.name      =    PlistManager::getInstance()->getPlistCString(_obj,"name").getCString();
		_info.introduce =    PlistManager::getInstance()->getPlistCString(_obj,"introduce").getCString();
		_info.item_icon =	PlistManager::getInstance()->getPlistCString(_obj,"itemIcon").getCString();
		_info.limit_count = UserRecord::getInstance()->getGoodsLimit(Value(_info.shop_id).asInt());
		_info.payid     = PlistManager::getInstance()->getPlistCString(_obj,"payid").getCString();
		auto user = UserRecord::getInstance();
		if (atoi(_info.item_id.c_str())>50000 && atoi(_info.item_id.c_str())<70000 ){
			__Dictionary *_exDic = PlistManager::getInstance()->addPlistMap("Exchange","plist/exChange.plist");
			string ch;
			string convert;
			switch (_info.payType)
			{
			case 2:
				ch = "SoulToGold";
				convert = "goldCount";
				break;
			case 3:
				ch = "RmbToSoul";
				convert = "soulCount";
				break;
			default:
				break;
			}

			_exObj = PlistManager::getInstance()->getPlist( PlistManager::getInstance()->getPlist(_exDic,ch.c_str())
															,String::createWithFormat("change%s",_info.item_id.c_str())->getCString());
			_info.count = atoi(PlistManager::getInstance()->getPlistCString(_exObj,convert.c_str()).getCString());
		}

		_itemInfo.push_back(_info);
	}

	setSelectedItem(idx);
}

void Ui_Shop::itemBtnClickedCallFuc(Ref *pSender , TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){
		initItemZOrder();
		Button *bt = static_cast<Button*>(pSender);
		bt->setTouchEnabled(false);
		bt->setBright(false);
		bt->setLocalZOrder(ShopHighLightZorder);
		if (strcmp(bt->getName(),sweetBtn->getName())==0){
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			
			freshData(SweetItem);
		}else if (strcmp(bt->getName(),magicBtn->getName())==0){
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			freshData(MagicItem);
		}else if (strcmp(bt->getName(),materialBtn->getName())==0){
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			freshData(MaterialItem);
		}else if (strcmp(bt->getName(),boxBtn->getName())==0){
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			freshData(BoxItem);
		}else if (strcmp(bt->getName(),soulBtn->getName())==0){
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			freshData(SoulItem);
		}else if(strcmp(bt->getName(),goldBtn->getName())==0) {
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			freshData(GoldItem);
		}else if(strcmp(bt->getName(),clothesBtn->getName())==0) {
			if (_isFirstTime==NO)
			{
				audioPlayer::getInstance()->playEffect("S1.mp3", false);
			}
			freshData(ClothesItem);
		}
		_isFirstTime=NO;
		scheduleOnce(schedule_selector(Ui_Shop::delayUpdateItemInfo),0.1f);
	}
}

void Ui_Shop::delayUpdateItemInfo(float)
{
	updateItemInfo(getSelectedItem());
}

void Ui_Shop::initItemZOrder()
{
	do{
		CC_BREAK_IF(!sweetBtn);
		sweetBtn -> setLocalZOrder(SweetItemZorder);
		sweetBtn -> setTouchEnabled(true);
		sweetBtn -> setBright(true);
		CC_BREAK_IF(!magicBtn);
		magicBtn   -> setLocalZOrder(MagicItemZorder);
		magicBtn   -> setTouchEnabled(true);
		magicBtn   -> setBright(true);
		CC_BREAK_IF(!materialBtn);
		materialBtn    -> setLocalZOrder(MaterialItemZorder);
		materialBtn    -> setTouchEnabled(true);
		materialBtn    -> setBright(true);
		CC_BREAK_IF(!boxBtn);
		boxBtn    -> setLocalZOrder(BoxItemZorder);
		boxBtn    -> setTouchEnabled(true);
		boxBtn    -> setBright(true);
		CC_BREAK_IF(!soulBtn);
		soulBtn    -> setLocalZOrder(SoulItemZorder);
		soulBtn    -> setTouchEnabled(true);
		soulBtn    -> setBright(true);
		CC_BREAK_IF(!goldBtn);
		goldBtn    -> setLocalZOrder(GoldItemZorder);
		goldBtn    -> setTouchEnabled(true);
		goldBtn    -> setBright(true);
		CC_BREAK_IF(!clothesBtn);
		clothesBtn    -> setLocalZOrder(ClothesItemZorder);
		clothesBtn    -> setTouchEnabled(true);
		clothesBtn    -> setBright(true);

	}while(0);
}

void Ui_Shop::updateItemInfo(int idx)
{
	//初始化ListView
	listView -> setItemsMargin(0);
	listView -> removeAllItems();
	listView -> setSize(Size(960,320));
	
	Widget *_item;
	float  delay = 0;
	for (size_t i=0; i!=_itemInfo.size();++i){

		_item = GUIReader::getInstance()->widgetFromJsonFile("Shop_item/Shop_item.ExportJson");
		listView -> insertCustomItem(_item,i);
		_itemInfo.at(i).widget = _item;
		ImageView *icon = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"item_Icon"));	
		icon->loadTexture(_itemInfo.at(i).item_icon);

			Size visibleSize = Director::getInstance()->getVisibleSize();
			float hscale = visibleSize.height/640;
			icon->setScale(hscale);
// 		if (atoi(_itemInfo.at(i).item_id.c_str())>70000 ){
// 			icon->setScale(0.6f);
// 		}

		TextBMFont      *name = static_cast<TextBMFont*>(Helper::seekWidgetByName(_item,"item_name"));	
		name -> setText(_itemInfo.at(i).name); 

		TextBMFont      *info = static_cast<TextBMFont*>(Helper::seekWidgetByName(_item,"item_info"));	
		string str = _itemInfo.at(i).introduce;
		while ( str.find_first_of("_n")!=string::npos )
		{
			size_t pos1 = str.find_first_of("_n"); 
			str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
		}
		/*CCLOG("str=%s",str.c_str());*/
		info -> setText(str);

		ImageView *bg     = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"Goods_Item"));
		ImageView *bottom = static_cast<ImageView*>(bg->getChildByName("Image_6"));

		
		Button *btn = static_cast<Button*>(_item->getChildByName("Button_Buy"));
		btn->setTag(1000+i);
		btn->addTouchEventListener(this,toucheventselector(Ui_Shop::buyBtnEventCallFunc));
		btn->setPressedActionEnabled(true);
		btn->setTouchPriority(-15);

		TextBMFont      *price = static_cast<TextBMFont*>(Helper::seekWidgetByName(_item,"item_price"));	
		price -> setText(String::createWithFormat("%d",_itemInfo.at(i).price)->getCString());

		ImageView *goldIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"gold_icon"));	
		string fileName;
		if (_itemInfo.at(i).payType==1){
			fileName = "image/shop_UI_zjm_007.png";
		}else if (_itemInfo.at(i).payType==2){
			fileName = "image/shop_UI_zjm_008.png";
		}else if (_itemInfo.at(i).payType==3){
			fileName = "image/shop_RMB.png";
		}
		goldIcon->loadTexture(fileName.c_str());

		//如果是限购商品
		TextBMFont* limit = static_cast<TextBMFont*>(Helper::seekWidgetByName(_item,"item_limit"));	
		limit->setVisible(false);
		if (_itemInfo.at(i).limit_count != -1)
		{
			//item_limit
			limit->setVisible(true);
			std::string strs = limit->getStringValue();
			while (strs.find("_n") != -1) {
				std::string::size_type index = strs.find("_n");
				int _count = _itemInfo.at(i).limit_count;
				strs.replace(index,2,Value(_itemInfo.at(i).limit_count).asString());
			}
			limit ->setText(strs); 
			limit ->setVisible(true);
			limit ->setZOrder(5);
			if (_itemInfo.at(i).limit_count == 0)
			{
				btn->setTouchEnabled(false);
				Button *Button_no = static_cast<Button*>(Helper::seekWidgetByName(_item,"Button_no"));	
				Button_no->setVisible(true);
				Button_no->setTouchEnabled(false);
			}
		}

		//如果是时装
		if (atoi(_itemInfo.at(i).item_id.c_str())>70000 && atoi(_itemInfo.at(i).item_id.c_str())<80000){
			if ( UserRecord::getInstance()->getGoodsCountByID(_itemInfo.at(i).item_id)>0 ){
				ImageView* ownIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_item,"Image_7"));		
				ownIcon->setVisible(true);
				btn->setGrey();
			}
		}

		fadeInAction(_item,delay);
		bg -> runAction(Sequence::create(DelayTime::create(delay),ScaleTo::create(0.2f,1.05f),ScaleTo::create(0.1f,1.0f),NULL)); 
		delay += 0.15f;
	}
}

void Ui_Shop::fadeInAction(Node *pSender,float delay)
{
	pSender->setOpacity(0);
	for (Node *child:pSender->getChildren()){
		if (child){
			fadeInAction(child,delay);
		}
	}
	pSender->runAction(Sequence::create(DelayTime::create(delay),FadeIn::create(0.2f),NULL));
}

void Ui_Shop::buyBtnEventCallFunc(Ref *pSender , TouchEventType type)
{
	if (type==TOUCH_EVENT_ENDED){
		Button *btn   = static_cast<Button*>(pSender);
		clickedBtnTag = btn->getTag();

		if (_itemInfo.at(clickedBtnTag-1000).payType==3){
			//购买英雄之魂,人民币支付=============================================

			string paycode="0";

			//判断支付平台
			string str = checkSIM();
			if (checkSIM()=="cmcc"){
				paycode = _itemInfo.at(clickedBtnTag-1000).payid;
			}else if (checkSIM()=="cucc"){
				CCLog("cucc");
				paycode="1";
			}else if (checkSIM()=="ctcc"){
				CCLog("ctcc");
				paycode="1";
			}





			/*CCLog("==============================checkSIM()=%s",str.c_str());*/
			if (strcmp(paycode.c_str(),"0")!=0){
				this->runAction(CallFunc::create([=](){
					mobilePay(_itemInfo.at(clickedBtnTag-1000).payid.c_str(),StringUtils::format("%d",UserRecord::getInstance()->getUid()).c_str());
				}));
			}
			return;
		}

		auto m_child = Director::getInstance()->getRunningScene()->getChildByTag(251);
		if (m_child!=NULL){
			Director::getInstance()->getRunningScene()->removeChild(m_child);
		}

		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->setTag(251);
		auto Text = PlistManager::getInstance()->getPlistMap("Text");
		__String* TextStr = static_cast<__String*>(Text->objectForKey("1007"));
		prompt->initWithTwoButtons(TextStr->getCString());
		prompt->setRightSelector(this,comm_selector(Ui_Shop::buyPromptCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);

		audioPlayer::getInstance() -> playEffect("S1.mp3");
	}
}


void Ui_Shop::close(Ref *pSender , TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){
		if (mHeroAltarWindow)
		{
			mHeroAltarWindow->updateHeroViewPage(mHeroAltarWindow->mCurPageViewIndex);
			mHeroAltarWindow = nullptr;
		}
		removeFromParentAndCleanup(true);
		
		//通知ui_sceneselect开启 touch 事件
		NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
		resumeGame();

		audioPlayer::getInstance() -> playEffect("S1.mp3");
	}
}

void Ui_Shop::buyPromptCallFunc()
{
	if (clickedBtnTag<1000)
		return;

	if (_itemInfo.at(clickedBtnTag-1000).payType==1){
		if (UserRecord::getInstance()->getGold()<_itemInfo.at(clickedBtnTag-1000).price){
			noEnoughString = "NO enough Gold!!!!";
			auto Text = PlistManager::getInstance()->getPlistMap("Text");
			__String* TextStr = static_cast<__String*>(Text->objectForKey("1003"));
			noEnoughString = TextStr->getCString();
			createNoEnoughGoldFrame(0.3f);
			return;
		}
		NotificationCenter::getInstance()->postNotification(CHANGE_GOLD_NUM,(Ref*)(intptr_t)(UserRecord::getInstance()->getGold()));
	}else if (_itemInfo.at(clickedBtnTag-1000).payType==2){
		if (UserRecord::getInstance()->getHeroSoul()<_itemInfo.at(clickedBtnTag-1000).price){
			auto Text = PlistManager::getInstance()->getPlistMap("Text");
			__String* TextStr = static_cast<__String*>(Text->objectForKey("1004"));
			noEnoughString = TextStr->getCString();
			createNoEnoughGoldFrame(0.3f);
			return;
		}
	}

	auto postDatas = __String::createWithFormat("c=shop&itemId=%s",_itemInfo.at(clickedBtnTag-1000).shop_id.c_str())->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas,this, httpresponse_selector(Ui_Shop::onHttpRequestCompleted));
}

void Ui_Shop::buySoulSuccessCallfuc(Ref *obj)
{
	string code = static_cast<__String*>(obj)->getCString();
	int addsoul=0;
	for (int i = 0; i < _itemInfo.size(); i++)
	{
		if (_itemInfo.at(i).payid == code){
			addsoul = _itemInfo.at(i).count;
		}
	}
	UserRecord::getInstance()->alterHeroSoul(addsoul);

	this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
		Text *soul = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"soul_num"));
		soul->setText(StringUtils::format("%d",UserRecord::getInstance()->getHeroSoul()));
	}),NULL));
}

void Ui_Shop::createNoEnoughGoldFrame(float dt)
{
	if (noEnoughString=="")
		return;

	Ui_Prompt *prompt = Ui_Prompt::create();
	prompt->setTag(251);
	prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	prompt->initWithOneButton(noEnoughString.c_str());
	prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
	Director::getInstance()->getRunningScene()->addChild(prompt,300);
}

void Ui_Shop::goldNoEnoughCallFunc()
{
	if (_itemInfo.at(clickedBtnTag-1000).payType==1){
		selectIndex(GoldItem);
	}else if(_itemInfo.at(clickedBtnTag-1000).payType==2) {
		selectIndex(SoulItem);
	}
}

void Ui_Shop::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	Connect::getInstance()->closeLoadPanel();
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
		this->runAction(Sequence::create(CallFunc::create([=](){
			                                                     //播放购买成功特效
			                                                     buyGoodsSuccessEffect();
		}),CallFunc::create([=](){
			//更改所买的道具数量
			string goodsID = _itemInfo.at(clickedBtnTag-1000).item_id;
			int    count   = 0;
			if (atoi(goodsID.c_str())<50000 || atoi(goodsID.c_str())>=70000){
				count   = UserRecord::getInstance()->getGoodsCountByID(goodsID)+_itemInfo.at(clickedBtnTag-1000).count;
				UserRecord::getInstance()->setGoodsCount(goodsID,String::createWithFormat("%d",count)->getCString());
			}else if (atoi(goodsID.c_str())<60000){  //购买金币
				count   = UserRecord::getInstance()->getGold() + _itemInfo.at(clickedBtnTag-1000).count;
				UserRecord::getInstance()->setGold(count);
				NotificationCenter::getInstance()->postNotification(CHANGE_GOLD_NUM,(Ref*)(intptr_t)(UserRecord::getInstance()->getGold()));
				Text *gold = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"gold_num"));
				gold->setText(String::createWithFormat("%d",count)->getCString());

			}else if (atoi(goodsID.c_str())<70000){  //购买英雄之魂
				//count   = UserRecord::getInstance()->getHeroSoul() + _itemInfo.at(clickedBtnTag-1000).count;
				//UserRecord::getInstance()->setHeroSoul(count);
			} 

			//扣钱
			int payType = _itemInfo.at(clickedBtnTag-1000).payType;
			if (payType == 1) //金币
			{
				UserRecord::getInstance()->alterGold(-_itemInfo.at(clickedBtnTag-1000).price);
				Text *gold = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"gold_num"));
				gold->setText(String::createWithFormat("%d",UserRecord::getInstance()->getGold())->getCString());
			} else if ( payType ==2) {
				UserRecord::getInstance()->alterHeroSoul(-_itemInfo.at(clickedBtnTag-1000).price);
				Text *soul = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"soul_num"));
				soul->setText(String::createWithFormat("%d",UserRecord::getInstance()->getHeroSoul())->getCString());
			} else if( payType ==3 ) {


			}
			//		Connect::getInstance()->closeLoadPanel();
			if (_itemInfo.at(clickedBtnTag-1000).limit_count != -1)
			{
				UserRecord::getInstance()->alterGooldLimit(atoi(_itemInfo.at(clickedBtnTag-1000).shop_id.c_str()),-1);
				this->selectIndex(BoxItem);
			}

			if (atoi(_itemInfo.at(clickedBtnTag-1000).item_id.c_str()) < 80000 && atoi(_itemInfo.at(clickedBtnTag-1000).item_id.c_str()) > 70000 )
			{
				auto widget = _itemInfo.at(clickedBtnTag-1000).widget->getChildByName("Image_7");
				widget->setScale(4);
				widget->setOpacity(0);
				widget->setVisible(true);
				widget->runAction(
					Sequence::create(
					Spawn::create(
					ScaleTo::create(0.2f,0.8f),
					FadeIn::create(0.2f),
					NULL
					),
					ScaleTo::create(0.1f,1.0f),
					NULL)
					);
				static_cast<Button*>(_itemInfo.at(clickedBtnTag-1000).widget->getChildByName("Button_Buy"))->setGrey();
			}
		}),NULL));
		
	} else {
		noEnoughString = v["info"].GetString();
		createNoEnoughGoldFrame(0.3f);
	}
}

void Ui_Shop::buyGoodsSuccessEffect()
{
	Button* btn = static_cast<Button*>(listView->getItem(clickedBtnTag-1000)->getChildByName("Button_Buy"));

	effectsObject *effect = effectsObject::create();
	effect->setPosition(Point(btn->getSize().width/2,btn->getSize().height/2+33));
	effect->setScale(1.5f);
	btn->addChild(effect,10);

	effect->shopBuyGoodsSuccessEffect();
}

void Ui_Shop::heroAltarUpdateCallBack(Widget* _window)
{
	if (_window)
	{
		mHeroAltarWindow = static_cast<Ui_HeroAltar*>(_window);
	}
}
// 创建移动控件动作
void Ui_Shop::createMoveAction(Widget* _widget, float _speedTime, int _disY)
{
	if (!_widget)
		return;
	auto mb1 = MoveBy::create((float)_speedTime,Point(0,_disY));
	auto mb2 = MoveBy::create((float)_speedTime,Point(0,-_disY));
	auto action = Sequence::create(mb1, mb2, NULL);
	RepeatForever *repeat = RepeatForever::create(action); 
	_widget->runAction(repeat);
}
// 创建缩放控件动作
void Ui_Shop::createScaleAction(Widget* _widget, float _speedTime, float _scale)
{
	if (!_widget)
		return;
	auto _act1 = ScaleTo::create(_speedTime, 1.0 + _scale);
	auto _act2 = ScaleTo::create(_speedTime, 1.0 - _scale);
	auto action = Sequence::create(_act1, _act2, NULL);
	RepeatForever *repeat = RepeatForever::create(action); 
	repeat->setTag(3);
	_widget->runAction(repeat);
}

