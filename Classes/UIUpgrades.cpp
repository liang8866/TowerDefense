#include "UIUpgrades.h"
#include "Ui_Shop.h"
#include "PlistManager.h"
#include "UserRecord.h"
#include "Ui_Prompt.h"
#include "levelScene.h"
#include "Connect.h"
#include "audioPlayer.h"

USING_NS_CC;
using namespace network;

UIUpgrades::UIUpgrades(void)
{
}
UIUpgrades::~UIUpgrades(void){

}


bool UIUpgrades::init()
{
	if (!Widget::init()){
		return false;
	}
	mSkillLevelSprite = nullptr;
	_connecting = false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI_Upgrades/GameUI0.plist","UI_Upgrades/GameUI0.png");
	mainWidget=static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("UI_Upgrades/UI_Upgrades.ExportJson"));
	this->addChild(mainWidget,1);
	mainWidget->setTouchEnabled(true);
	mainWidget->setTouchPriority(TOUCH_UI_PRIORITY);

	//bk
	Widget *bg = mainWidget->getChildByName("panal_upgrade");
	bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
	bg->setPosition(Point(mainWidget->getSize().width/2,mainWidget->getSize().height/2));
	bg->setScale(bg->getScale()*0.8f);
	bg->runAction(Sequence::create(ScaleBy::create(0.1f,1.02/0.8f),ScaleBy::create(0.05f,1.0/1.02f),NULL));

	closeBtn=static_cast<Button*>(bg->getChildByName("closeBtn"));//关闭按钮
	closeBtn->addTouchEventListener(this,toucheventselector(UIUpgrades::closeEventCallFunc));

	//Btn_tech=static_cast<Button*>(bg->getChildByName("Btn1"));//经济
	//Btn_magic=static_cast<Button*>(bg->getChildByName("Btn2"));//魔法
	//Btn_war=static_cast<Button*>(bg->getChildByName("Btn3"));//战士
	//Btn_hero=static_cast<Button*>(bg->getChildByName("Btn4"));//英雄
	//Btn_tower=static_cast<Button*>(bg->getChildByName("Btn5"));//防御塔
	//Btn_trap=static_cast<Button*>(bg->getChildByName("Btn6"));//陷阱
	/*Btn_tech->addTouchEventListener(this,toucheventselector(UIUpgrades::BookMarkEventCallFunc));
	Btn_magic->addTouchEventListener(this,toucheventselector(UIUpgrades::BookMarkEventCallFunc));
	Btn_war->addTouchEventListener(this,toucheventselector(UIUpgrades::BookMarkEventCallFunc));
	Btn_hero->addTouchEventListener(this,toucheventselector(UIUpgrades::BookMarkEventCallFunc));
	Btn_tower->addTouchEventListener(this,toucheventselector(UIUpgrades::BookMarkEventCallFunc));
	Btn_trap->addTouchEventListener(this,toucheventselector(UIUpgrades::BookMarkEventCallFunc));
	Btn_tech->setTouchEnabled(false);
	Btn_tech->setBright(false);
	Btn_tech->setLocalZOrder(7);*/

	resetBtn=static_cast<Button*>(bg->getChildByName("resetBtn"));//洗点按钮
	

    costBtn=static_cast<Button*>(bg->getChildByName("costBtn"));//花费金额的按钮
	costText=static_cast<Text*>(bg->getChildByName("costLabel"));//花费金额的 数目



	costBtn->addTouchEventListener(this,toucheventselector(UIUpgrades::upgradeEventCallFunc));//升级
	costBtn->setPressedActionEnabled(true);


	//youMoneyBg 下面的
	//shopBtn=static_cast<Button*>(Helper::seekWidgetByName(mainWidget,"moneyBtn"));//转去商店的按钮
	MoneyText=static_cast<TextBMFont*>(Helper::seekWidgetByName(bg,"moneyLabel"));
	//shopBtn->addTouchEventListener(this,toucheventselector(UIUpgrades::ShopEventCallFunc));
	auto user = UserRecord::getInstance();
	MoneyText->setText(Value(UserRecord::getInstance()->getStarCount()).asString());

	//describebg下面的
	descWidget=static_cast<Button*>(bg->getChildByName("describebg"));
	descText=static_cast<TextBMFont*>(descWidget->getChildByName("descLabel"));
	descPrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(descWidget,"num"));


	//页签对应的经济、哨塔，陷阱，士兵，英雄，魔法 -》model5(25),model4(20),model3(15),model3(15),model3(15),model4(20)
	model5=static_cast<Widget*>(bg->getChildByName("Page0"));//25
	//model4=static_cast<Widget*>(bg->getChildByName("Page1"));//20	魔法
	//model3=static_cast<Widget*>(bg->getChildByName("Page2"));//15	陷阱

	//注册按钮点击事件
	Button *bt;
	for (int i=0;i!=25;++i){
		bt = static_cast<Button*>(model5->getChildByTag(10000+i));//25
		bt->addTouchEventListener(this,toucheventselector(UIUpgrades::modeleventCallFunc));
	}
	//for (int i=0;i!=20;++i){
	//	bt = static_cast<Button*>(model4->getChildByTag(10000+i));//20
	//	bt->addTouchEventListener(this,toucheventselector(UIUpgrades::modeleventCallFunc));
	//}
	//for (int i=0;i!=15;++i){
	//	bt = static_cast<Button*>(model3->getChildByTag(10000+i));//15
	//	bt->addTouchEventListener(this,toucheventselector(UIUpgrades::modeleventCallFunc));
	//}

	selectFrame = static_cast<ImageView*>(bg->getChildByName("selected_Frame"));
	selectFrame->setVisible(false);
	FixedPos=model5->getPosition();//获取一个坐标当做固定的
	OutPos=Point(-10000,0);

	currentTag = 10000;
	this->refreshMarkBook(0);

	fitScreen();
	
	NotificationCenter::getInstance()->postNotification(PAUSEKEY, (Ref*)1);
	return true;
}

void UIUpgrades::onEnter()
{
	Widget::onEnter();
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(UIUpgrades::listenerCallFunc),
		CHANGE_GOLD_NUM,
		NULL);
}

void UIUpgrades::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("UI_Upgrades/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("UI_Upgrades/GameUI0.png");

	NotificationCenter::getInstance()->postNotification(RESUMEKEY, NULL);
	NotificationCenter::getInstance()->removeObserver(this,CHANGE_GOLD_NUM);
	Widget::onExit();
}

void UIUpgrades::listenerCallFunc(Ref *obj)
{
	do{
		CC_BREAK_IF(!MoneyText);
		MoneyText->setText(String::createWithFormat("%d",UserRecord::getInstance()->getSweet())->getCString());
	}while(0);
}


void UIUpgrades::refreshMarkBook(int index){
	
	model5->setPosition(OutPos);//
	//model4->setPosition(OutPos);//
	//model3->setPosition(OutPos);//

	model5->setVisible(false);//
	//model4->setVisible(false);//
	//model3->setVisible(false);//


	if (index==0)
	{
		model5->setPosition(FixedPos);
		model5->setVisible(true);
		setSelectedItem(TechEconomyItemIdx);
	}else if (index==1)
	{
		/*model4->setPosition(FixedPos);
		model4->setVisible(true);
		setSelectedItem(TechTowerItemIdx);*/
	}else if (index==2)
	{
		/*model3->setPosition(FixedPos);
		model3->setVisible(true);
		setSelectedItem(TechTrapItemIdx);*/
	}else if (index==3)
	{
		/*model3->setPosition(FixedPos);
		model3->setVisible(true);
		setSelectedItem(TechSoliderItemIdx);*/
	}else if (index==4)
	{
		/*model3->setPosition(FixedPos);
		model3->setVisible(true);
		setSelectedItem(TechHeroItemIdx);*/
	}else if (index==5)
	{
		/*model4->setPosition(FixedPos);
		model4->setVisible(true);
		setSelectedItem(TechMagicItemIdx);*/
	}

	updateSelectItemIcon();
	updateSelectItemInfo();
	updateSelectedFramePos(10000);
}


void UIUpgrades::BookMarkEventCallFunc(Ref *pSender , TouchEventType type)//上面的书签页切换的回掉
{


	//if (type == TOUCH_EVENT_ENDED){
	//	do{
	//		CC_BREAK_IF(!Btn_tech);
	//		Btn_tech -> setLocalZOrder(6);
	//		Btn_tech -> setTouchEnabled(true);
	//		Btn_tech -> setBright(true);
	//		 
	//		CC_BREAK_IF(!Btn_magic);
	//		Btn_magic   -> setLocalZOrder(5);
	//		Btn_magic   -> setTouchEnabled(true);
	//		Btn_magic   -> setBright(true);
	//		
	//		CC_BREAK_IF(!Btn_war);
	//		Btn_war    -> setLocalZOrder(4);
	//		Btn_war    -> setTouchEnabled(true);
	//		Btn_war    -> setBright(true);
	//	
	//		CC_BREAK_IF(!Btn_hero);
	//		Btn_hero    -> setLocalZOrder(3);
	//		Btn_hero    -> setTouchEnabled(true);
	//		Btn_hero    -> setBright(true);
	//		
	//		CC_BREAK_IF(!Btn_tower);
	//		Btn_tower    -> setLocalZOrder(2);
	//		Btn_tower    -> setTouchEnabled(true);
	//		Btn_tower    -> setBright(true);
	//		
	//		CC_BREAK_IF(!Btn_trap);
	//		Btn_trap   -> setLocalZOrder(1);
	//		Btn_trap    -> setTouchEnabled(true);
	//		Btn_trap    -> setBright(true);
	//		 
	//	}while(0);
 //   
	//	Button *bt = static_cast<Button*>(pSender);
	//	bt->setTouchEnabled(false);
	//	bt->setBright(false);
	//	bt->setLocalZOrder(7);
	//	
	//	refreshMarkBook(bt->getTag()-1001);//更新标签上的东西

	//	audioPlayer::getInstance()->playEffect("S1.mp3");
	//}
}
void UIUpgrades::ShopEventCallFunc(Ref *pSender , TouchEventType type)//转去商店的回调
{
	Button *bt = static_cast<Button*>(pSender);
	if (type==TOUCH_EVENT_BEGAN)
	{
		bt->setScale(1.1f);
		
	}
	if (type==TOUCH_EVENT_ENDED||type==TOUCH_EVENT_MOVED||type==TOUCH_EVENT_CANCELED)
	{
		bt->setScale(1.0f);
	}
	
	if (type == TOUCH_EVENT_ENDED){
		auto child = Director::getInstance()->getRunningScene()->getChildByTag(124);
		if (child){
			Director::getInstance()->getRunningScene()->removeChild(child);
		}

		Ui_Shop* shopLayer = Ui_Shop::create();
		shopLayer  -> setTag(124);
		Director::getInstance()->getRunningScene() -> addChild(shopLayer,101);
		shopLayer  -> selectIndex(ShopItem::SoulItem);

		audioPlayer::getInstance()->playEffect("S1.mp3");
	}
}
void UIUpgrades::resetEventCallFunc(Ref *pSender , TouchEventType type)//重设的按钮回调
{
	Button *bt = static_cast<Button*>(pSender);
	if (type == TOUCH_EVENT_ENDED){
		auto child = Director::getInstance()->getRunningScene()->getChildByTag(251);
		if (child){
			Director::getInstance()->getRunningScene()->removeChild(child);
		}


		string str = resetPrompt.c_str();
		while ( str.find_first_of("_n")!=string::npos )
		{
			size_t pos1 = str.find_first_of("_n"); 
			str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
		}
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithTwoButtons(str.c_str());
		prompt->setRightSelector(this,comm_selector(UIUpgrades::confirmResetCallBack));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);

		audioPlayer::getInstance()->playEffect("S1.mp3");
	}
}
void UIUpgrades::upgradeEventCallFunc(Ref *pSender , TouchEventType type)//升级的按钮回调
{
	Button *bt = static_cast<Button*>(pSender);
	if (type == TOUCH_EVENT_ENDED){
		if (selectedIndex<0 || selectedIndex>=(int)mSelectItemInfo.size() )
			return;
		if (!mSelectItemInfo.at(selectedIndex).isGradeUp)
			return;
		auto user = UserRecord::getInstance();
		if (user->getStarCount() < mSelectItemInfo.at(selectedIndex).needMoney)
			return;

		bt->setGrey();
	
		//可以升级
		string key,item,level;
		int  itemCtn;
		Widget *curr_model;
		setValue(key,itemCtn,curr_model);
	
		int itemIdx  = (selectedIndex+1)%5==0? (selectedIndex+1)/5:(selectedIndex+1)/5+1;  //选中对象列索引
		int listIdx  = (selectedIndex+1)%5==0? 5:(selectedIndex+1)%5;                      //选中对象行索引
		item   = String::createWithFormat("item%d",itemIdx)->getCString();
		level  = String::createWithFormat("level%d",listIdx)->getCString();

		upType = key;
		upItem = item;
		upLevel = listIdx;
		
		//连接服务器
		doRequest();

		audioPlayer::getInstance()->playEffect("S1.mp3");
	}
}

void UIUpgrades::doRequest()
{
	auto user = UserRecord::getInstance();
	auto postDatas = __String::createWithFormat("c=upgrades&type=%s&item=%s&level=%d",upType.c_str(),upItem.c_str(),upLevel)->getCString();
	Connect* con = Connect::getInstance();
	con->loadSend(postDatas,this, httpresponse_selector(UIUpgrades::onHttpRequestCompleted));
}
void UIUpgrades::showError(int errType)
{
	Connect::getInstance()->setAutoConnect(false);
	Ui_Prompt *prompt = Ui_Prompt::create();
	prompt->setTag(251);
	prompt->setPosition(Director::getInstance()->getVisibleOrigin());
	auto Text = PlistManager::getInstance()->getPlistMap("Text");
	__String* TextStr  =  static_cast<__String*>(Text->objectForKey("1001"));
	prompt->initWithRetry(TextStr->getCString());

	if (errType == 0)
	{
		//升级
		prompt->setRightSelector(this,comm_selector(UIUpgrades::doRequest));
	} else if(errType == 1) { 
		//重置
		prompt->setRightSelector(this,comm_selector(UIUpgrades::confirmResetCallBack));
	}
	
	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(prompt,300);
}
void UIUpgrades::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	costBtn->removeGrey();
	Connect::getInstance()->closeLoadPanel();
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		showError(0);
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		// 播放音效_成功
		audioPlayer::getInstance()->playEffect("S11.mp3");
		// 显示特效
		beginAnim_UpSkillLevel();
		auto user = UserRecord::getInstance();
		user->setUpgradesItemLevel(upType,upItem,upLevel);
		//更新条目信息
		int index = selectedIndex;
		user->alterStarCount(-mSelectItemInfo.at(selectedIndex).needMoney );
		MoneyText->setText(Value(user->getStarCount()).asString());
		refreshMarkBook(getSelectedItem());
		index = (index+1)%5==0? 10000+index:10000+index+1;
		updateSelectedFramePos(index);
		if (user->getIsFreshMan()){
			user->setFreshManMutex(false);
		}
	} else {
		showError(0);
	}
}
void UIUpgrades::onHttpRequestCompletedReset(HttpClient *sender, HttpResponse *response)
{
	Connect::getInstance()->closeLoadPanel();
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	if (buffer->size() > 0)
	{
		rapidjson::Document doc; 
		doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
		if (doc.HasParseError())  
		{  
			showError(1);
			return ;  
		}  
		rapidjson::Value &v=doc; 
		if (v["status"].GetInt() == 1)
		{
			//删除当前天赋树下所有数据
			UserRecord::getInstance()->removeUpgradesType(upType);
			
			//设置星星数量
			UserRecord::getInstance()->setStarCount(Value( v["info"].GetString() ).asInt());
			
			MoneyText->setText(v["info"].GetString());
			//关闭loadling面
		//	Connect::getInstance()->closeLoadPanel();

			//更新条目信息
			refreshMarkBook(getSelectedItem());
		} else {
			showError(1);
		}
	}
	
}

void UIUpgrades::closeEventCallFunc(Ref *pSender , TouchEventType type)//关闭按钮的回调
{
	Button *bt = static_cast<Button*>(pSender);
	if (type == TOUCH_EVENT_ENDED){
		if (Director::getInstance()->getRunningScene()->getTag()==LEVEL_SCENE_TAG){
			levelScene *lScene=static_cast<levelScene*>(this->getParent());
			lScene->upgradesUiMoveOut();
			lScene->selectUiComeIn();
		}else{
			this->removeFromParentAndCleanup(true);
		}
		audioPlayer::getInstance()->playEffect("S1.mp3");
	}	
}

//model上的按钮回调
void UIUpgrades::modeleventCallFunc(Ref *pSender , TouchEventType type){
	Button *bt = static_cast<Button*>(pSender);
	if (type == TOUCH_EVENT_ENDED){
		updateSelectedFramePos(bt->getTag());

		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}

		audioPlayer::getInstance()->playEffect("S1.mp3");
	}
}

//更新选中框的位置
void UIUpgrades::updateSelectedFramePos(int tag)
{


	/*auto btn =  static_cast<Button*>(Helper::seekWidgetByTag(model3,currentTag))->getChildByTag(806);
	if (btn != nullptr)
	{
		btn->removeFromParentAndCleanup(true);
	}*/
	 auto btn1 = static_cast<Button*>(Helper::seekWidgetByTag(model5,currentTag))->getChildByTag(806);;
	 if (btn1 != nullptr)
	 {
		 btn1->removeFromParentAndCleanup(true);
	 }
	/*auto btn2 =   static_cast<Button*>(Helper::seekWidgetByTag(model4,currentTag))->getChildByTag(806);;
	 if (btn2 != nullptr)
	 {
		 btn2->removeFromParentAndCleanup(true);
	 }*/
	int selectItemIdx = getSelectedItem();
	Button *clickedBtn;
	switch (selectItemIdx)
	{
	case 0:
		clickedBtn = static_cast<Button*>(Helper::seekWidgetByTag(model5,tag));
		break;
	case 1:
		//clickedBtn = static_cast<Button*>(Helper::seekWidgetByTag(model4,tag));
		break;
	case 2:
		//clickedBtn = static_cast<Button*>(Helper::seekWidgetByTag(model3,tag));
		break;
	case 3:
		//clickedBtn = static_cast<Button*>(Helper::seekWidgetByTag(model3,tag));
		break;
	case 4:
		//clickedBtn = static_cast<Button*>(Helper::seekWidgetByTag(model3,tag));
		break;
	case 5:
		//clickedBtn = static_cast<Button*>(Helper::seekWidgetByTag(model4,tag));
		break;
	default:
		break;
	}

	auto copyframe = selectFrame->clone();
	copyframe->setPosition(Point(-9,-2));
	copyframe->setAnchorPoint(Point::ZERO);
	copyframe->setVisible(true);
	copyframe->setLocalZOrder(0);
	copyframe->setTag(806);
	clickedBtn->addChild(copyframe);
	currentTag = tag;

	//更新科技ICON
	//更新科技描述
	string str = mSelectItemInfo.at(tag-10000).des;
	while ( str.find_first_of("_n")!=string::npos )
	{
		size_t pos1 = str.find_first_of("_n"); 
		str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
	}
	descText ->setText(str);

	//更新价格
	descPrice ->setText(String::createWithFormat("%d",mSelectItemInfo.at(tag-10000).needMoney)->getCString());
	
	if (UserRecord::getInstance()->getStarCount()<mSelectItemInfo.at(tag-10000).needMoney || !mSelectItemInfo.at(tag-10000).isGradeUp){
		costBtn->setGrey();  //升级按钮状态（灰态）
	}else {
		costBtn->removeGrey(); //升级按钮状态（常态）
	}
	//选中的条目索引
	selectedIndex = tag-10000;

	
	ImageView* _icon = static_cast<ImageView*>(descWidget->getChildByName("obj_icon"));
	int index = (selectedIndex+1)%5==0? (selectedIndex+1)/5-1:(selectedIndex+1)/5;
	int _lieIndex = index + 1;
	int _posIndex = selectedIndex-5*index + 1;
	//更改右侧图标
	string strIcon;
	if (_lieIndex == 1)
	{
		strIcon = "icon/eco/eco%d.png";
	}else if (_lieIndex == 2)
	{
		strIcon = "icon/tower/tower%d.png";
	}else if (_lieIndex == 3)
	{
		strIcon = "icon/trap/trap%d.png";
	}else if (_lieIndex == 4)
	{
		strIcon = "icon/war/war%d.png";
	}else if (_lieIndex == 5)
	{
		strIcon = "icon/hero/hero%d.png";
	}
	_icon->loadTexture(StringUtils::format(strIcon.c_str(), _posIndex));
}

//更新当前选择的条目内Icon
void UIUpgrades::updateSelectItemIcon()
{
	string key;
	int  itemCtn;
	Widget *curr_model;
	setValue(key,itemCtn,curr_model);

	string strIcon;
	string strLineIcon;

	strIcon = "icon/tower/tower_%d_%d.png";
	strLineIcon = "icon/tower/tower_line_%d.png";

	switch (getSelectedItem())
	{
	case TechEconomyItemIdx:
		//5列模板
		strIcon = "icon/eco/eco_%d_%d.png";
		strLineIcon = "icon/eco/eco_line_%d.png";
		break;
	case TechTowerItemIdx:
		strIcon = "icon/tower/tower_%d_%d.png";
		strLineIcon = "icon/tower/tower_line_%d.png";

		break;
	case TechTrapItemIdx:
		strIcon = "icon/trap/trap_%d_%d.png";
		strLineIcon = "icon/trap/trap_line_%d.png";
	
		break;
	case TechSoliderItemIdx:
		strIcon = "icon/war/war_%d_%d.png";
		strLineIcon = "icon/war/war_line_%d.png";

		break;
	case TechHeroItemIdx:
		strIcon = "icon/hero/hero_%d_%d.png";
		strLineIcon = "icon/hero/hero_line_%d.png";

		break;
	case TechMagicItemIdx:
		strIcon = "icon/magic/magic_%d_%d.png";
		strLineIcon = "icon/magic/magic_line_%d.png";

		break;
	default:
		return;
		break;
	}

	Button *bt;   //条目Btn
	ImageView *sp;//条目ICON
	ImageView *line;	// 底类型
	for (int i=0;i!=itemCtn;++i){
		for(int j=0;j!=5;++j){
			bt = static_cast<Button*>(Helper::seekWidgetByTag(curr_model,10000+5*i+j));
			sp = static_cast<ImageView*>(bt->getChildByName("btnIcon"));
			//auto _curIconImage = StringUtils::format(strIcon.c_str(), i, j);
			//sp ->loadTexture(_curIconImage);
			/*line = static_cast<ImageView*>(Helper::seekWidgetByTag(curr_model,512 + i));	
			if (line)
			{
			auto _curLineIconImage = StringUtils::format(strLineIcon.c_str(), i);
			line->loadTexture(_curLineIconImage);
			}*/
		}
	}
}

//更新当前选择的条目内的数据
void UIUpgrades::updateSelectItemInfo()
{
	string key;
	int  itemCtn;
	Widget *curr_model;
	setValue(key,itemCtn,curr_model);

	__Dictionary *dic = PlistManager::getInstance()->getPlistMap("Upgrades");
	__Dictionary *obj = PlistManager::getInstance()->getPlist(dic,key.c_str());
	
	resetPrompt = static_cast<__String*>(dic->objectForKey("resetPointPrompt"))->getCString();

	//给该条目内数据赋值
	mSelectItemInfo.clear();
	TechItemInfo _info;
	__Dictionary *_obj; 
	__Dictionary *_exObj;
	Button *bt;
	ImageView *sp;

	bool resetFlag = true;
	bool nextShowFlag = true;
	Widget* price = static_cast<Widget*>(Helper::seekWidgetByName(descWidget,"tBtn_price"));


	for (int i=0;i!=itemCtn;++i){
		_obj = PlistManager::getInstance()->getPlist(obj,String::createWithFormat("item%d",i+1)->getCString());
		
		//服务器保存的用户天赋数据
		std::string itemId = String::createWithFormat("item%d",i+1)->getCString();
		//当前item的等级
		auto itemLevel = UserRecord::getInstance()->getUpgradesItemLevel(key,itemId);

		nextShowFlag = true;
		for (int j=0;j!=5;++j){
			_exObj = PlistManager::getInstance()->getPlist(_obj,String::createWithFormat("level%d",j+1)->getCString());
			_info.needMoney   = atoi(PlistManager::getInstance()->getPlistCString(_exObj,"needMoney").getCString());
			_info.des         =		 PlistManager::getInstance()->getPlistCString(_exObj,"des").getCString();
			_info.val         = atof(PlistManager::getInstance()->getPlistCString(_exObj,"value").getCString());
			_info.isGradeUp   = false;

			if (j+1 <= itemLevel)
			{
				_info.isSelect = true;
			} else {
				_info.isSelect = false;
			}


			//条目背景
			bt = static_cast<Button*>(Helper::seekWidgetByTag(curr_model,10000+5*i+j));
			//tBtn_price mainWidget
			auto copyPrice = bt->getChildByName("tBtn_price");
			

			//刷新条目内按钮UI,//此项还未升级
			if (!_info.isSelect){  
				//右下角小框
				if (copyPrice == nullptr)
				{
					copyPrice = price->clone();
					bt->addChild(copyPrice);
				}
				copyPrice->setPosition(Point(bt->getAnchorPointInPoints().x+39,-31+bt->getAnchorPointInPoints().y));
				static_cast<TextBMFont*>(copyPrice->getChildByName("num"))->setText(Value(_info.needMoney).asString());
				if (!nextShowFlag || UserRecord::getInstance()->getStarCount() < _info.needMoney)
				{
					copyPrice->setGrey();
				} else {
					copyPrice->removeGrey();
				}
				nextShowFlag = false;
				bt -> loadTextureNormal("image/KJS__TF_02.png");
				//条目ICON
				sp = static_cast<ImageView*>(bt->getChildByName("btnIcon"));
				sp->setGrey();
				//此项是当前List第一项，或者上一项已经升级，则此项可以升级
				if (j==0 || mSelectItemInfo.at(5*i+j-1).isSelect){  
					_info.isGradeUp = true;
				}
			}else{
				resetFlag = false;
				nextShowFlag = true;
				bt -> loadTextureNormal("image/KJS__TF_01.png");
				//条目ICON
				sp = static_cast<ImageView*>(bt->getChildByName("btnIcon"));
				sp->removeGrey();
				//右下角小框
				if (copyPrice != nullptr)
				{
					copyPrice->removeFromParentAndCleanup(true);
				}
			}
			if (bt && bt->getParent())
			{
				_info.itemBack = static_cast<Widget*>(bt);
			}
			mSelectItemInfo.push_back(_info);
		}
	}
	auto resetPrice = static_cast<__String*>(dic->objectForKey("resetPointPrice"));
	// 如果本页没有升过级的 洗点按钮置灰 
	if (resetFlag || UserRecord::getInstance()->getGold() < resetPrice->intValue())
	{
		resetBtn->setGrey();
	} else {
		resetBtn->removeGrey();
		resetBtn->addTouchEventListener(this,toucheventselector(UIUpgrades::resetEventCallFunc));
		resetBtn->setPressedActionEnabled(true);
	}
}

void UIUpgrades::confirmResetCallBack()
{
	string key;
	int  itemCtn;
	Widget *curr_model;
	setValue(key,itemCtn,curr_model);
	upType = key;
	//连接服务器
	auto postDatas  = __String::createWithFormat("c=upgrades&a=reset&type=%s",upType.c_str())->getCString();
	Connect::getInstance()->loadSend(postDatas,this, httpresponse_selector(UIUpgrades::onHttpRequestCompletedReset));
}

void UIUpgrades::setValue(string &key , int &itemCtn , Widget* &curr_model)
{
	switch (getSelectedItem())
	{
	case TechEconomyItemIdx:
		key = "tech";
		itemCtn = 5;
		curr_model = model5;
		break;
	case TechTowerItemIdx:
		key = "tower";
		itemCtn = 4;
		//curr_model = model4;
		break;
	case TechTrapItemIdx:
		key = "trap";
		itemCtn = 3;
		//curr_model = model3;
		break;
	case TechSoliderItemIdx:
		key = "war";
		itemCtn = 3;
		//curr_model = model3;
		break;
	case TechHeroItemIdx:
		key = "hero";
		itemCtn = 3;
		//curr_model = model3;
		break;
	case TechMagicItemIdx:
		key = "magic";
		itemCtn = 4;
		//curr_model = model4;
		break;
	default:
		key = "tech";
		itemCtn = 5;
		curr_model = model5;
		break;
	}
}

// 技能升级特效开始
void UIUpgrades::beginAnim_UpSkillLevel()
{
	
	Widget* _curParentCtrl = mSelectItemInfo.at(selectedIndex).itemBack;
	if(!_curParentCtrl)
		return;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero_wu_Effects_03.plist");
	Sprite* _curSprite = Sprite::createWithSpriteFrameName("hero_wu_Effects_03_update_01.png");		
	_curSprite->setTag(40);
	_curParentCtrl->setLocalZOrder(100);
	_curParentCtrl->addChild(_curSprite, 101);
	_curSprite->setPositionX(_curParentCtrl->getSize().width/2);
	_curSprite->setPositionY(_curParentCtrl->getSize().height/2);

	if (_curSprite)
	{
		// 播放动画
		_curSprite->setVisible(true);
		Animate* ani = createAnimation("hero_wu_Effects_03_update_", 12);
		auto action = Sequence::create(ani,
			CallFuncN::create(CC_CALLBACK_1(UIUpgrades::endAnim_UpSkillLevel, this)),
			NULL);
		_curSprite->runAction(action);
	}
}
// 技能升级特效结束
void UIUpgrades::endAnim_UpSkillLevel(Ref *sender)
{
	Sprite* _curSprite = static_cast<Sprite*>(sender);
	if (_curSprite)
	{
		_curSprite->setVisible(false);
		_curSprite->removeFromParentAndCleanup(true);
		_curSprite = nullptr;
	}
}
// 创建序列帧动画
Animate* UIUpgrades::createAnimation(const char*FrameName,int max)
{
	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(__String::createWithFormat("%s%02d.png",FrameName,i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.05f);
	Animate *animate = CCAnimate::create(animation);
	return animate;
}
void UIUpgrades::fitScreen()
{	
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	mainWidget->setScale(_scale);

	Widget* _backImage  = static_cast<ImageView*>(Helper::seekWidgetByName(mainWidget,"Image_6"));
	_backImage->setTouchEnabled(true);
	_backImage->setTouchPriority(-9);
	_backImage->setScaleX(2);
	_backImage->setScaleY(2);

	mainWidget->setPosition(Point(visibleSize.width/2  - mainWidget->getSize().width/2*_scale  + originPt.x,
								  visibleSize.height/2 - mainWidget->getSize().height/2*_scale + originPt.y
							));
}


