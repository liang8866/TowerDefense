#include "Ui_MainLayer.h"
#include "WindowManager.h"
#include "Ui_MessageBox.h"
#include "UserRecord.h"
#include "magicObjects.h"
#include "Ui_Debug.h"

#define SCA_NOMARL 0.9
#define SCA_SELECT 1.02


ItemInfo::ItemInfo():unit_id("war1"),
totalCDtime(3.0f),
leftCDtime(0),
price(100)
{

}

ItemInfo::~ItemInfo()
{

}


Ui_MainLayer::Ui_MainLayer():
noCDprice(10),
skillCDstatus(false)
{

}

Ui_MainLayer::~Ui_MainLayer()
{
	NotificationCenter::getInstance()->removeObserver(this,CHANGE_UNIT_PRICE);
	NotificationCenter::getInstance()->removeObserver(this,PAUSEKEY);
	NotificationCenter::getInstance()->removeObserver(this, RESUMEKEY);
}


Ui_MainLayer *Ui_MainLayer::create(const char *fileName)
{
	Ui_MainLayer *pRet = new Ui_MainLayer();
	if (pRet->initialise(fileName))
		pRet->autorelease();
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}
	return pRet;
}

bool Ui_MainLayer::initialise(const char *fileName)
{
	// 加载json
	mMainWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fileName);
	if (nullptr == mMainWidget)
		return false;

	waring=nullptr;//打门的时候的闪烁
	skillPrg=nullptr;
	_lastKey = 1;
	mJsonName = fileName;
	mWindowName = "Ui_MainLayerWnd";
	mState =0;
	_magicListener = nullptr;
	this->setZOrder(1);
	this->setTouchEnabled(false);
	mMainWidget->setTouchEnabled(false);

	Widget *bottomPanel = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget, "bottom_bg"));
	m_PauseBtn   = static_cast<Button*>(bottomPanel->getChildByName("Pause_btn"));
	
	Widget *controlPanel = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget, "Control_num_panel"));
	m_ShopBtn    = static_cast<Button*>(controlPanel->getChildByName("shopBtn"));

	m_PauseBtn -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchUiButton));
	m_ShopBtn  -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchUiButton));

	HeroBtn       = static_cast<Button*>(mMainWidget->getChildByName("Button_Hero"));
	HeroBtn    -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchHeroBtn));
	HeroBtn    -> setTouchPriority(-9);
	
	magic_bk      = static_cast<Widget*>(bottomPanel->getChildByName("magic_frame_bk"));
	m_AddMagicBtn = static_cast<Button*>(bottomPanel->getChildByName("magic_add"));
	m_AddMagicBtn -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchMagicButton));

	ImageView* sweetIcon = static_cast<ImageView*>(controlPanel->getChildByName("sp_gold"));
	sweetIcon    -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouch_Debug));
	sweetIcon->setVisible(false);
	auto parent = sweetIcon->getParent();
	ArmatureDataManager::getInstance()->addArmatureFileInfo("candy0.png","candy0.plist","candy.ExportJson");
	_armature = Armature::create("candy");
	_armature->setPosition(sweetIcon->getPosition());
	_armature->setLocalZOrder(sweetIcon->getLocalZOrder());
	_armature->setScale(sweetIcon->getSize().width/_armature->getContentSize().width);
	parent->addChild(_armature);
	
	_soulIcon =  static_cast<ImageView*>(controlPanel->getChildByName("sp_soul"));

	//UI上的位置的跟这里有区别
	m_MagicBtn_1 = static_cast<Button*>(magic_bk->getChildByName("magic_btn_1"));//20001攻击 
	m_MagicBtn_2 = static_cast<Button*>(magic_bk->getChildByName("magic_btn_2"));// 20002 变羊
	m_MagicBtn_3 = static_cast<Button*>(magic_bk->getChildByName("magic_btn_3"));//20003 传送门
	m_MagicBtn_4 = static_cast<Button*>(magic_bk->getChildByName("magic_btn_4"));// 20004 哥斯拉
	m_MagicBtn_1->setScale(SCA_NOMARL);
	m_MagicBtn_2->setScale(SCA_NOMARL);
	m_MagicBtn_3->setScale(SCA_NOMARL);
	m_MagicBtn_4->setScale(SCA_NOMARL);

	m_magicCdTime_1=10;
	m_magicCdTime_2=20;
	m_magicCdTime_3=25;
	m_magicCdTime_4=60;

	////变羊的
	//int magic2SelectIndex = PlistManager::getInstance()->getUpgradesSelect("magic", "item2");
	//if (magic2SelectIndex>=3)
	//{
	//	m_magicCdTime_2-=5;//科技树缩短5秒冷却时间
	//}
	////传送门的
	//int magic3SelectIndex = PlistManager::getInstance()->getUpgradesSelect("magic", "item3");
	//if (magic3SelectIndex>=2 && magic3SelectIndex<4)
	//{
	//	m_magicCdTime_3-=5;//科技树缩短5秒冷却时间
	//}
	//if (magic3SelectIndex>=4)
	//{
	//	m_magicCdTime_3-=10;//科技树缩短10秒冷却时间
	//}
	////哥斯拉的
	//int magic4SelectIndex = PlistManager::getInstance()->getUpgradesSelect("magic", "item4");
	//if (magic4SelectIndex>=3){
	//	m_magicCdTime_4-=20;//科技树缩短20秒冷却时间
	//}

	//20001攻击    20002 变羊  20003 传送门   20004 哥斯拉

	//设置数量
	auto user = UserRecord::getInstance();


	int scetion=user->getSectionNum();

	if (scetion==1)
	{
		UserRecord::getInstance()->setMusicPlay("scene_1.mp3");
		
	}if (scetion==2)
	{
		UserRecord::getInstance()->setMusicPlay("scene_2.mp3");
	}
	if (scetion==3)
	{
		UserRecord::getInstance()->setMusicPlay("scene_3.mp3");
	}


	TextBMFont *magic_num_1 = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_MagicBtn_1, "magic_num"));//20001 攻击 
	magic_num_1->setText(Value(user->getGoodsCountByID("20001")).asString());

	TextBMFont *magic_num_2 = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_MagicBtn_2, "magic_num"));//20002 变羊
	magic_num_2->setText(Value(user->getGoodsCountByID("20002")).asString());

	TextBMFont *magic_num_3 = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_MagicBtn_3, "magic_num"));//20003 传送门 
	magic_num_3->setText(Value(user->getGoodsCountByID("20003")).asString());

	TextBMFont *magic_num_4 = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_MagicBtn_4, "magic_num"));//20004 哥斯拉
	magic_num_4->setText(Value(user->getGoodsCountByID("20004")).asString());

	m_MagicBtn_1 -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchMagicButton));
	m_MagicBtn_2 -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchMagicButton));
	m_MagicBtn_3 -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchMagicButton));
	m_MagicBtn_4 -> addTouchEventListener(this, toucheventselector(Ui_MainLayer::eventTouchMagicButton));
	
	if (!(UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()==3)||UserRecord::getInstance()->getSectionNum()==0){
		if (user->getGoodsCountByID("20001")<=0)
		{
			magic_num_1->setVisible(false);//隐藏数字
			Widget*magic_add=static_cast<Widget*>(Helper::seekWidgetByName(m_MagicBtn_1, "magic_add"));
			magic_add->setVisible(true);//显示加号
		}
		if (user->getGoodsCountByID("20002")<=0)
		{
			magic_num_2->setVisible(false);//隐藏数字
			Widget*magic_add=static_cast<Widget*>(Helper::seekWidgetByName(m_MagicBtn_2, "magic_add"));
			magic_add->setVisible(true);//显示加号
		}
		if (user->getGoodsCountByID("20003")<=0)
		{
			magic_num_3->setVisible(false);//隐藏数字
			Widget*magic_add=static_cast<Widget*>(Helper::seekWidgetByName(m_MagicBtn_3, "magic_add"));
			magic_add->setVisible(true);//显示加号
		}
		if (user->getGoodsCountByID("20004")<=0)
		{
			magic_num_4->setVisible(false);//隐藏数字
			Widget*magic_add=static_cast<Widget*>(Helper::seekWidgetByName(m_MagicBtn_4, "magic_add"));
			magic_add->setVisible(true);//显示加号
		}
	}

	hideHeroInfo();
	
	hideMagics();


	initData();

	
	this->addChild(mMainWidget);

	//适配屏幕
	fitScreen();

	//添加广播
	NotificationCenter::getInstance()->addObserver( this,
													callfuncO_selector(Ui_MainLayer::changePrice),
												    CHANGE_UNIT_PRICE,
									                NULL);


	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Ui_MainLayer::myPauseAllAction),PAUSEKEY,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Ui_MainLayer::myResumeAllAction),RESUMEKEY,NULL);
	
	scheduleUpdate();
	// 隐藏预警
	updateYuJing(false);
	return true;
}

void Ui_MainLayer::setPauseBtnGray(){//把暂停按钮灰掉
	//m_PauseBtn->setGrey(0);
	m_PauseBtn->setTouchEnabled(false);
	m_ShopBtn->setTouchEnabled(false);
	HeroBtn->setTouchEnabled(false);
}
void Ui_MainLayer::removePauseBtnGray(){//把暂停按钮h恢复
	//m_PauseBtn->removeGrey();
	m_PauseBtn->setTouchEnabled(true);
	m_ShopBtn->setTouchEnabled(true);
	HeroBtn->setTouchEnabled(true);
}


void Ui_MainLayer::cdBtnProgess(Button*btn,int isPause){

	ProgressTimer*pro=nullptr;

	pro=static_cast<ProgressTimer*>(btn->getChildByTag(1888));
	if (pro!=nullptr)
	{
		if (isPause==YES)
		{
			pro->pause();
		}else{
			pro->resume();
		}
	}
	
}

void Ui_MainLayer::myPauseAllAction(Ref *obj)
{

	    //魔法技能的冷却
	this->cdBtnProgess(m_MagicBtn_1,YES);
	this->cdBtnProgess(m_MagicBtn_2,YES);
	this->cdBtnProgess(m_MagicBtn_3,YES);
	this->cdBtnProgess(m_MagicBtn_4,YES);

	for (auto& x: loadBars)
	{
		static_cast<ProgressTimer*>(x.second)->pauseSchedulerAndActions();
	}
}
void Ui_MainLayer::myResumeAllAction(Ref *obj)
{
	
	//魔法技能的恢复
	this->cdBtnProgess(m_MagicBtn_1,NO);
	this->cdBtnProgess(m_MagicBtn_2,NO);
	this->cdBtnProgess(m_MagicBtn_3,NO);
	this->cdBtnProgess(m_MagicBtn_4,NO);
	 for (auto& x: loadBars)
	 {
		 static_cast<ProgressTimer*>(x.second)->resumeSchedulerAndActions();
	 }
}
void Ui_MainLayer::initData()
{
	std::map<int,string> takeUnits = UserRecord::getInstance()->getTakeUnits();

	m_itemInfo.clear();
	ItemInfo __units;
	std::string plistName;

	UserRecord::getInstance()->_typeWarLevel.clear();

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

	for (auto iter=takeUnits.begin();iter!=takeUnits.end();++iter){
		__units.unit_id    =   iter->second;
		__units.leftCDtime =   0;
		__units.selfType   = atoi(String::createWithFormat("%c",__units.unit_id.at(__units.unit_id.size()-1))->getCString());

		float reducePrice=0.0;
		
		if (__units.unit_id.find("war")!=string::npos){
			plistName       = "WarriorData";
			__units.objType = TYPE_WARRIOR;
			reducePrice=warValue;//科技树影响的
			UserRecord::getInstance()->_typeWarLevel.insert(make_pair(__units.selfType,1));
		}else if (__units.unit_id.find("tower")!=string::npos){
			plistName		= "TowerData";
			reducePrice=towerValue;//科技树影响的
			__units.objType = TYPE_TOWER;
		}else if (__units.unit_id.find("trap")!=string::npos){
			plistName       = "TrapData";
			reducePrice=trapValue;//科技树影响的
			__units.objType = TYPE_TRAP;
		}else
			return;

		__units.totalCDtime=   PlistManager::getInstance()->getAttributeCCString(plistName.c_str(), __units.unit_id.c_str() , 1, "cdTime").floatValue();
		__units.price      =   PlistManager::getInstance()->getAttributeCCString(plistName.c_str(), __units.unit_id.c_str() , 1, "callMoney").intValue()*(1-reducePrice);
		

		m_itemInfo.insert(make_pair(iter->first,__units));
	}
	
	updateObjsIcon();
}

//处理暂停，商店按钮点击事件
void Ui_MainLayer::eventTouchUiButton(Ref* sender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){
		CCLOG("textures:%s",TextureCache::getInstance()->getCachedTextureInfo().c_str()); 

		Button *button = static_cast<Button*>(sender);
		if ( button->getName() == m_PauseBtn->getName() ){
			auto child = Director::getInstance()->getRunningScene()->getChildByTag(249);
			if (child){
				 Director::getInstance()->getRunningScene()->removeChild(child);
			}

			audioPlayer::getInstance()->pauseAllMusicEffect();

			Ui_MessageBox* _messageBox = Ui_MessageBox::create();
			_messageBox->setTag(249);
			Director::getInstance()->getRunningScene()->addChild(_messageBox,400);

		}else if ( button->getName() == m_ShopBtn->getName() ){
			
			WindowManager::getInstance()->createShopFrame();
		}
	}
}



//处理魔法按钮点击事件  
void Ui_MainLayer::eventTouchMagicButton(Ref* sender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){

		auto user = UserRecord::getInstance();
		Button *button = static_cast<Button*>(sender);
		bool isCreateMagicListenerTouch = false;
		//20001攻击    20002 变羊  20003 传送门   20004 哥斯拉
		if ( button->getName() == m_MagicBtn_1->getName() ){//20001攻击 

			if (user->getGoodsCountByID("20001")<=0 && !UserRecord::getInstance()->getIsFreshMan())
			{
				WindowManager::getInstance()->createShopFrame(MagicItem);
			}else{
				isCreateMagicListenerTouch = true;
				magicIndex=1;
				button->setScale(SCA_SELECT);
				button->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f,SCA_NOMARL),ScaleTo::create(0.8f,SCA_SELECT),NULL)));
				if (UserRecord::getInstance()->getIsFreshMan())
					UserRecord::getInstance()->setFreshManMutex(false);
			}

			

		}else if ( button->getName() == m_MagicBtn_2->getName() ){// 20002 变羊 

			if (user->getGoodsCountByID("20002")<=0  && !UserRecord::getInstance()->getIsFreshMan())
			{
				WindowManager::getInstance()->createShopFrame(MagicItem);
			}else{

			isCreateMagicListenerTouch = true;
			magicIndex=2;
			button->setScale(SCA_SELECT);
			button->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f,SCA_NOMARL),ScaleTo::create(0.8f,SCA_SELECT),NULL)));
			if (UserRecord::getInstance()->getIsFreshMan())
				UserRecord::getInstance()->setFreshManMutex(false);
			}

		}else if ( button->getName() == m_MagicBtn_3->getName() ){//20003 传送门

			if (user->getGoodsCountByID("20003")<=0  && !UserRecord::getInstance()->getIsFreshMan())
			{
				WindowManager::getInstance()->createShopFrame(MagicItem);
			}else{
			isCreateMagicListenerTouch = true;
			magicIndex=3;
			button->setScale(SCA_SELECT);
			button->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f,SCA_NOMARL),ScaleTo::create(0.8f,SCA_SELECT),NULL)));
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}
			}
		}else if ( button->getName() == m_MagicBtn_4->getName() ){// 20004 哥斯拉

			if (user->getGoodsCountByID("20004")<=0  && !UserRecord::getInstance()->getIsFreshMan())
			{
				WindowManager::getInstance()->createShopFrame(MagicItem);
			}else{
			isCreateMagicListenerTouch = true;
			magicIndex=4;
			button->setScale(SCA_SELECT);
			button->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f,SCA_NOMARL),ScaleTo::create(0.8f,SCA_SELECT),NULL)));
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}
			}

		}
		
		else if ( button->getName() == m_AddMagicBtn->getName()){//展开魔法按钮的
			if (magic_bk->isVisible()){
				if (UserRecord::getInstance()->getIsFreshMan())
					return;

				hideMagics();
			}else{
				showMagics();

				if (UserRecord::getInstance()->getIsFreshMan())
					UserRecord::getInstance()->setFreshManMutex(false);
			}
		}

		// 创建释放魔法监听
		if (isCreateMagicListenerTouch)
		{
			createMagicListenerTouch();
		}
	}
}



//处理条目内对象点击事件
void Ui_MainLayer::eventTouchItemObjButton(Ref* sender, TouchEventType type)
{
	Button *button = static_cast<Button*>(sender);
	Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));

	if ( (UserRecord::getInstance()->getSweet()<m_itemInfo.at(button->getTag()-1000).price && !UserRecord::getInstance()->getIsObjsNoCD()) ||
		 (UserRecord::getInstance()->getSweet()<noCDprice && UserRecord::getInstance()->getIsObjsNoCD())	){

		float _scale = 0.7f;
		TextBMFont *price = static_cast<TextBMFont*>(button->getChildByName("item_obj_label"));
		price->stopAllActions();
		price->setScale(_scale);
		price->runAction(Sequence::create(ScaleTo::create(0.1f,_scale*1.5f ,_scale*1.5f),
										  ScaleTo::create(0.08f,_scale*1.2f,_scale*1.2f),
										  ScaleTo::create(0.08f,_scale*1.5f,_scale*1.5f),
										  ScaleTo::create(0.1f,_scale*1.0f,_scale*1.0f),
										  NULL
										));
		return;
	}

	
	if ( strcmp(button->getName(),"item_index_btn_1")==0 ){
		mainBack->itemBtnClickedCallfuc(sender, type,1);
	}else if ( strcmp(button->getName(),"item_index_btn_2")==0 ){
		mainBack->itemBtnClickedCallfuc(sender, type,2);
	}else if ( strcmp(button->getName(),"item_index_btn_3")==0  ){
		mainBack->itemBtnClickedCallfuc(sender, type,3);
	}else if ( strcmp(button->getName(),"item_index_btn_4")==0  ){
		mainBack->itemBtnClickedCallfuc(sender, type,4);
	}else if ( strcmp(button->getName(),"item_index_btn_5")==0  ){
		mainBack->itemBtnClickedCallfuc(sender, type,5);
	}else if ( strcmp(button->getName(),"item_index_btn_6")==0  ){
		mainBack->itemBtnClickedCallfuc(sender, type,6);
	}else if ( strcmp(button->getName(),"item_index_btn_7")==0  ){
		mainBack->itemBtnClickedCallfuc(sender, type,7);
	}
}

void Ui_MainLayer::hideMagics()
{
	do{
		if (magic_bk->isVisible()==true)
		{
			CC_BREAK_IF(!magic_bk);
			magic_bk->setVisible(false);
			magic_bk->runAction(ScaleTo::create(0.3f,1,0));
			CC_BREAK_IF(!m_MagicBtn_1);
			m_MagicBtn_1->setEnabled(false);
			CC_BREAK_IF(!m_MagicBtn_2);
			m_MagicBtn_2->setEnabled(false);
			CC_BREAK_IF(!m_MagicBtn_3);
			m_MagicBtn_3->setEnabled(false);
			CC_BREAK_IF(!m_MagicBtn_4);
			m_MagicBtn_4->setEnabled(false);
		}
		
	}while(0);
}

void Ui_MainLayer::showMagics()
{
	do{
		CC_BREAK_IF(!magic_bk);
		magic_bk->setVisible(true);
		magic_bk->setScaleY(0);
		magic_bk->runAction(ScaleTo::create(0.1f,1,1));
		CC_BREAK_IF(!m_MagicBtn_1);
		m_MagicBtn_1->setEnabled(true);
		CC_BREAK_IF(!m_MagicBtn_2);
		m_MagicBtn_2->setEnabled(true);
		CC_BREAK_IF(!m_MagicBtn_3);
		m_MagicBtn_3->setEnabled(true);
		CC_BREAK_IF(!m_MagicBtn_4);
		m_MagicBtn_4->setEnabled(true);
	}while(0);
}

void Ui_MainLayer::hideProgress(Ref *pSender)
{
	do{
		Button *wg = static_cast<Button*>(pSender);
		CC_BREAK_IF(!wg);
		wg->setTouchEnabled(true);

	}while(0);
}

void Ui_MainLayer::showProgress(Ref *pSender, int _indexId)
{
	Button *btn = nullptr;
	if (pSender)
	{
		btn = static_cast<Button*>(pSender);
	}else
	{
		btn = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d", _indexId)->getCString()));
	}
	if (!btn)
	{
		return;
	}
	btn->setTouchEnabled(false);

	Sprite *sp      = Sprite::create(StringUtils::format("roleIcon/icon_%s_cd.png",m_itemInfo.at(btn->getTag()-1000).unit_id.c_str()));
	ImageView *icon = static_cast<ImageView*>(btn->getChildByName("star"));

	ProgressTimer *loadBar = ProgressTimer::create(sp);
	loadBar->setType(ProgressTimer::Type::BAR);
	loadBar->setScale(icon->getScale());
	//    Setup for a bar starting from the top since the midpoint is 1 for the y
	loadBar->setMidpoint(Point(0,1));
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	loadBar->setAnchorPoint(Point(0.5, 0));
	loadBar->setBarChangeRate(Point(0, 1));
	loadBar->setTag(105);
	loadBar->setPosition(icon->getPosition());
	btn->addChild(loadBar,1);
	loadBars.insert(++_lastKey, loadBar);
	// 创建一个转圈的动画
	ImageView* _imageRound = ImageView::create("roleIcon/ZhongDuan.png", UI_TEX_TYPE_LOCAL);
	_imageRound->setTag(106);
	auto _run1 = RotateBy::create(0.9f, 180);
	auto _actions = Sequence::create(_run1, NULL);
	RepeatForever* _repeatActions = RepeatForever::create(_actions); 
	_imageRound->runAction(_repeatActions);
	btn->addChild(_imageRound, 2);
	_imageRound->setScale(0.5f);
	int _x = btn->getSize().width/2;
	int _y = btn->getSize().height/2;
	_imageRound->setPositionX(_x);
	_imageRound->setPositionY(_y + 10);
	loadBar->runAction(Sequence::create(
										ProgressFromTo::create(m_itemInfo.at(btn->getTag()-1000).totalCDtime,100,0),
										CallFunc::create(CC_CALLBACK_0(Ui_MainLayer::progressActionCallFunc,this,btn,_lastKey)),
									    NULL
										));
	
}

void Ui_MainLayer::progressActionCallFunc(Ref *pSender,const int key)
{
	Button *btn = static_cast<Button*>(pSender);
	if (btn->getChildByTag(105))
		btn->removeChildByTag(105);
	if (btn->getChildByTag(106))
		btn->removeChildByTag(106);
	hideProgress(btn);
	loadBars.erase(key);
}


//设置回合数
void Ui_MainLayer::setRoundNum(int currRound , int totalRound)
{
	currRound = currRound>totalRound? totalRound:currRound;
	do{
		Text * round = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget,"label_round"));
		round->setFontName(MyfontName);
		round->setColor(Color3B(255,255,255));
		

		CC_BREAK_IF(!round);
		m_round = currRound;
		m_totalRound = totalRound;
		round->setText(String::createWithFormat("%d/%d",currRound,totalRound)->getCString());
	}while(0);
}

//设置生命值
void Ui_MainLayer::setLifeNum(int life)
{
	do{
		Text * lf = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget,"label_life"));
		lf->setFontName(MyfontName);
		lf->setColor(Color3B(255,255,255));
		
		CC_BREAK_IF(!lf);
		m_life = life;
		lf->setText(String::createWithFormat("%d",life)->getCString());
	}while(0);
}

//设置金币数
void Ui_MainLayer::setSweetNum(int sweet , int limit) 
{
	sweet = sweet>limit? limit:sweet;
	do{
		Text * gd = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget,"label_gold"));
		gd->setFontName(MyfontName);
		gd->setColor(Color3B(255,255,255));
		
		CC_BREAK_IF(!gd);
		m_sweet = sweet;
		m_totalSweet = limit;
		gd->setText(String::createWithFormat("%d/%d",sweet,limit)->getCString());
	}while(0);
}

void Ui_MainLayer::setDynamicSweetNum(int addsweet , Point pos)
{
	//设置1秒钟，糖果数增加完毕
	int sec = 1;
	int addAffterNum = m_sweet+addsweet>m_totalSweet ? m_totalSweet:m_sweet+addsweet;
	addsweet = m_sweet+addsweet>m_totalSweet ? m_totalSweet-m_sweet:addsweet;

	Widget *controlPanel = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget, "Control_num_panel"));
	Point point = Point(controlPanel->getPositionX()+_armature->getPositionX(),controlPanel->getPositionY()+_armature->getPositionY());

	//增加骨骼动画效果
	Sprite *sp = Sprite::createWithSpriteFrameName("image/UI_zjm_24.png");
	sp  ->setPosition(pos);
	sp  ->setTag(65432);
	this->addChild(sp,100);
	sp  ->runAction(Sequence::create(MoveTo::create(0.3f,point),CallFunc::create([=](){
		
	this->removeChildByTag(65432);
	_armature->getAnimation()->play("Animation1");


		Text * gd = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget,"label_gold"));
		if (addsweet<=sec*10){
			UserRecord::getInstance()->setSweet(addAffterNum);
		}else{
			int per = addsweet/(10*sec);
			float delay = 0;
			for (int i=1;i<sec*10+1;++i){
				gd->runAction(Sequence::create(DelayTime::create(delay),CallFunc::create([=]{
					UserRecord::getInstance()->alterSweet(per);

					if (i==sec*10 && getSweetNum()<addAffterNum){
						UserRecord::getInstance()->setSweet(addAffterNum);
					}
				}),NULL));
				delay += 0.1f;
			}
		}
	}),NULL));
}

//设置英雄之魂数量
void Ui_MainLayer::setSoulNum(int soul)
{
	do{
		Text * sl = static_cast<Text*>(Helper::seekWidgetByName(mMainWidget,"label_soul"));
		sl->setFontName(MyfontName);
		sl->setColor(Color3B(255,255,255));
		
		CC_BREAK_IF(!sl);
		m_soul = soul;
		sl->setText(String::createWithFormat("%d",soul)->getCString());
	}while(0);
}

//设置魔法数量(从左往右,1-4)
void Ui_MainLayer::setMagicNum(int index ,int num)
{
	do{
		Widget *wg = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,String::createWithFormat("magic_num_frame_%d",index)->getCString()));
		CC_BREAK_IF(!wg);
		Text * magicPrice = static_cast<Text*>(wg->getChildByName(String::createWithFormat("magic_num_%d",index)->getCString()));
		CC_BREAK_IF(!magicPrice);
		magicPrice->setText(String::createWithFormat("%d",num)->getCString());
	}while(0);
}


//设置菜单条目内对象价格，如果玩家钱低于购买菜单条目内对象的钱则显示红色(从左到右)
void Ui_MainLayer::setItemObjPrice(Widget *obj ,int price)
{
	do{
		Button *Obj = static_cast<Button*>(obj);
		CC_BREAK_IF(!Obj);
		TextBMFont * _objPrice = static_cast<TextBMFont*>(Obj->getChildByName("item_obj_label"));
		CC_BREAK_IF(!_objPrice);
		_objPrice->setText(String::createWithFormat("%d",price)->getCString());
		
		if ( getSweetNum()< atoi(_objPrice->getStringValue().c_str()) ){
			_objPrice->setColor(Color3B::RED);

		}else{
			_objPrice->setColor(Color3B(92,31,40));
		}
	}while(0);
}

//刷新菜单条目内所有对象价格
void Ui_MainLayer::updateAllObjsPrice()
{
	Button *btn;
	int price;
	for (auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter){
		btn   =static_cast<Button*>(currPanel->getChildByTag(1000+iter->first));
		if (!UserRecord::getInstance()->getIsObjsNoCD()){
			price = iter->second.price;
		}else
		{
			price = noCDprice;
		}
		setItemObjPrice(btn,price);
	}
}


void Ui_MainLayer::FindObjToNoCd(int oType,int sType){

	 							
	auto action = Sequence::create(
		DelayTime::create(0.05),
		CallFunc::create([=](){
			int i=0;
			for ( auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter )
			{
				
				if (iter->second.objType==oType&&iter->second.selfType==sType)
				{

					int leftSweet = UserRecord::getInstance()->getSweet()+iter->second.price;
					leftSweet = leftSweet<0? 0:leftSweet;
					UserRecord::getInstance()->setSweet(leftSweet);

					Button    *btn;
					ImageView *icon;
					
					btn  = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d",iter->first)->getCString()));
					icon = static_cast<ImageView*>(btn->getChildByName("star"));
					if (btn->getChildByTag(105)){
						btn->removeChildByTag(105);
						btn->setTouchEnabled(true);
					}
					if (btn->getChildByTag(106))
						btn->removeChildByTag(106);
					
					break;
				}

			}

	}),
		NULL);
	this->runAction(action);



	
}

void Ui_MainLayer::enterObjsNoCDstatus()
{
	Button    *btn;
	ImageView *icon;
	Animate   *animate; 
	for ( auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter )
	{
		btn  = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d",iter->first)->getCString()));
		icon = static_cast<ImageView*>(btn->getChildByName("star"));
		if (btn->getChildByTag(105)){
			btn->removeChildByTag(105);
			btn->setTouchEnabled(true);
		}
		if (btn->getChildByTag(106))
			btn->removeChildByTag(106);

		effectsObject *effect = effectsObject::create();
		effect->setPosition(btn->getSize().width/2,btn->getSize().height/2);
		effect->setTag(12345);
		btn   ->addChild(effect,50);
		effect->noCDEffects();

		Sprite *sp = Sprite::createWithSpriteFrameName("zjmdaoju_effets_01.png");
		sp->setPosition(icon->getPosition());
		btn->addChild(sp,icon->getLocalZOrder());

		animate = noCDeffect();
		sp->runAction(Sequence::create(animate,CallFunc::create([=](){
																sp->removeFromParentAndCleanup(true);
		                                                       }),NULL));
	}
	loadBars.clear();
	updateAllObjsPrice();
}

void Ui_MainLayer::leaveObjsNoCDstatus()
{
	updateAllObjsPrice();

	Button    *btn;
	for ( auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter ){
		btn  = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d",iter->first)->getCString()));

		if (btn->getChildByTag(12345)){
			btn->removeChildByTag(12345);
		}
	}
}

Animate* Ui_MainLayer::noCDeffect()
{
	
	Vector<SpriteFrame*> aniFrames(11);
	auto cache = SpriteFrameCache::getInstance();
	for(int i = 1; i <= 11; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("zjmdaoju_effets_%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);

	return animate;
}

void Ui_MainLayer::createObjCost(Ref *obj, int _indexId)
{
	do{
		Button *btn = nullptr;
		if (obj)
		{
			btn = static_cast<Button*>(obj);
		}else
		{
			btn = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d", _indexId)->getCString()));
		}
		CC_BREAK_IF(!btn);
		int cost;     
		if (!UserRecord::getInstance()->getIsObjsNoCD())
			cost= m_itemInfo.at(btn->getTag()-1000).price;
		else
			cost= noCDprice;

		int leftSweet = UserRecord::getInstance()->getSweet()-cost;
		leftSweet = leftSweet<0? 0:leftSweet;
		UserRecord::getInstance()->setSweet(leftSweet);
	}while (0);
}


//适配屏幕
void Ui_MainLayer::fitScreen()
{
	Size sz = Director::getInstance()->getVisibleSize();
	Point orginPt = Director::getInstance()->getVisibleOrigin();

	Widget *panel =  static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"Control_num_panel"));
	Widget *bottom = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"bottom_bg"));
	bottom->setTouchEnabled(true);

	Button *heroBtn = static_cast<Button*>(mMainWidget->getChildByName("Button_Hero"));
	heroBtn->setPosition(Point(orginPt.x+66,orginPt.y+sz.height-50));


	panel      -> setPosition(Point(sz.width-panel->getSize().width+orginPt.x,sz.height-panel->getSize().height+orginPt.y));
	bottom     -> setPosition(Point(sz.width/2+orginPt.x,bottom->getSize().height/2+orginPt.y));


	panel   -> setPositionY(panel->getPositionY()+300);
	bottom  -> setPositionY(bottom->getPositionY()-300);
	heroBtn -> setPositionX(heroBtn->getPositionX()-300);
	panel   -> runAction(MoveBy::create(0.8f,Point(0,-300)));
	bottom  -> runAction(MoveBy::create(0.8f,Point(0,300)));
	heroBtn -> runAction(MoveBy::create(0.8f,Point(300,0)));
	this    -> runAction(Sequence::create(DelayTime::create(0.8f),CallFunc::create([=](){
																		Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
																		if (mainBack && mainBack->isVisible())
																	    {
																			mainBack->createFirstHero(1.0f);
																		 }											
	                                                        }),NULL));
}

void Ui_MainLayer::update(float dt)
{

	//糖果数量变化时刷新数据，并刷新菜单内条目信息
	if ( getSweetNum() != UserRecord::getInstance()->getSweet() || getMaxSweet() != UserRecord::getInstance()->getMaxSweet() ){
		setSweetNum(UserRecord::getInstance()->getSweet(),UserRecord::getInstance()->getMaxSweet());
		updateAllObjsPrice();
		Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
		if (mainBack && mainBack->isVisible())
		{
			mainBack->updateAddStoreyButtonState();
		}
	};

	//刷新生命值
	if ( getLifeNum()  != UserRecord::getInstance()->getHP() ){                              
	   setLifeNum(UserRecord::getInstance()->getHP());
	}

	//刷新英雄之魂
	if ( getSoulNum()  != UserRecord::getInstance()->getHeroSoul() ){
		setSoulNum(UserRecord::getInstance()->getHeroSoul());
	}

	//刷新回合数
	if ( getCurrRound() != UserRecord::getInstance()->getRound() || getMaxRound() != UserRecord::getInstance()->getMaxRound() ){
		setRoundNum(UserRecord::getInstance()->getRound() , UserRecord::getInstance()->getMaxRound());
	}

	//刷新魔法技能数目
	this->refreshMagic("20001",m_MagicBtn_1);
	this->refreshMagic("20002",m_MagicBtn_2);
	this->refreshMagic("20003",m_MagicBtn_3);
	this->refreshMagic("20004",m_MagicBtn_4);


}


void Ui_MainLayer::changePrice(Ref *obj)
{
    string unit_id;

	GameObjects *_obj = static_cast<GameObjects*>(obj);
	if (_obj->getobjectType() != TYPE_WARRIOR) //只有士兵升级会影响造价
		return;

	if (_obj->getobjectType() == TYPE_WARRIOR)
		unit_id = String::createWithFormat("war%d",_obj->getSelfType())->getCString();
	else if (_obj->getobjectType() == TYPE_TOWER)
		unit_id = String::createWithFormat("tower%d",_obj->getSelfType())->getCString();
	else if (_obj->getobjectType() == TYPE_TRAP)
		unit_id = String::createWithFormat("trap%d",_obj->getSelfType())->getCString();
	else
		return;

	for (auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter){
		if (iter->second.unit_id == unit_id){

			// 科技树，士兵召唤减少
			float _item4_level2 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 2);
			float _item4_level4 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 4);
			float _value = 1.0 - _item4_level2 - _item4_level4;
			int _valueInt = (int)floor(_value*1000);
			float _valueFloat = _valueInt/1000.0f;
			m_itemInfo.at(iter->first).price       = _obj->getcallMoney() * _valueFloat;
			m_itemInfo.at(iter->first).totalCDtime = _obj->getcdTime();
			
			Button *objBtn = static_cast<Button*>(currPanel->getChildByTag(1000+iter->first));
			TextBMFont     *cost = static_cast<TextBMFont*>(objBtn->getChildByName("item_obj_label"));
			cost ->setText(String::createWithFormat("%d",iter->second.price)->getCString());
			break;
		}
	}
}

//更新条目上的Icon
void Ui_MainLayer::updateObjsIcon()
{
	//确定底部模板
	Widget *panel5 = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"item_bk_ground_5"));
	Widget *panel6 = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"item_bk_ground_6"));
	Widget *panel7 = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"item_bk_ground_7")); 
	switch (m_itemInfo.size())
	{
	case 5:
		currPanel = panel5;
		panel6->removeFromParentAndCleanup(true);    
		panel7->removeFromParentAndCleanup(true);
		break;
	case 6:
		currPanel = panel6;
		panel5->removeFromParentAndCleanup(true);    
		panel7->removeFromParentAndCleanup(true);
		break;
	case 7:
		currPanel = panel7;
		panel5->removeFromParentAndCleanup(true);    
		panel6->removeFromParentAndCleanup(true);
		break;
	default:
		return;
		break;
	}

	currPanel->setVisible(true);
	Button    *btn;
	ImageView *icon;
	TextBMFont      *cost;
	for ( auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter )
	{
		btn  = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d",iter->first)->getCString()));
		btn  ->setTag(1000+iter->first);
		btn  ->addTouchEventListener(this,toucheventselector(Ui_MainLayer::eventTouchItemObjButton));

		icon = static_cast<ImageView*>(btn->getChildByName("star"));
		icon ->loadTexture(String::createWithFormat("roleIcon/icon_%s.png",iter->second.unit_id.c_str())->getCString(),UI_TEX_TYPE_LOCAL);	
				
		//float tSca=0.8f;
		//icon ->setScale(tSca);
	/*icon->setPositionY(icon->getPositionY()-1.6);*/
		cost = static_cast<TextBMFont*>(btn->getChildByName("item_obj_label"));
		cost ->setText(String::createWithFormat("%d",iter->second.price)->getCString());
		//icon->setPositionY(cost->getPositionY()+icon->getContentSize().height/2+5.0);

	}
}

void Ui_MainLayer::showHeroInfo(int selfType,int Level,bossObjects *obj)
{
	HeroBtn->setVisible(true);
	HeroBtn->setTouchEnabled(true);
	if (obj->getcdleftTime()<=0.0)//说明技能冷却时候已到，可以点击
	{
		bossCdPrgCallfunc(HeroBtn);
	}else{//技能在冷却中
		if (skillPrg==nullptr)
		{
		
			Sprite *sp = Sprite::createWithSpriteFrameName("UI_BossSkill_CD.png");
			skillPrg= ProgressTimer::create(sp);
			skillPrg->setType(ProgressTimer::Type::BAR);
			//    Setup for a bar starting from the top since the midpoint is 1 for the y
			skillPrg->setMidpoint(Point(0,1));
			//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
			skillPrg->setBarChangeRate(Point(0, 1));
			skillPrg->setPosition(Point(HeroBtn->getSize().width/2 ,HeroBtn->getSize().height/2));
			HeroBtn->addChild(skillPrg,1);
			float cdlefttime=obj->getcdleftTime();//获取当前的冷却时间
			float  cdalltime=obj->getskill1_skillCDTime();//获取总得冷却
			int precent=(int)(((float)cdlefttime/cdalltime)*100);//求出当前的百分比
			skillPrg->runAction(Sequence::create(
				ProgressFromTo::create(cdlefttime,precent,0),
				CCCallFuncO::create(this,callfuncO_selector(Ui_MainLayer::bossCdPrgCallfunc),HeroBtn),
				NULL));
		}

	}
	

	TextBMFont *level = static_cast<TextBMFont*>(HeroBtn->getChildByName("Level"));
	level  ->setText(String::createWithFormat("%d",Level)->getCString());

	ImageView *photo = static_cast<ImageView*>(HeroBtn->getChildByName("Icon_photo"));
	photo->loadTexture(String::createWithFormat("heroAltar/icon_boss%d.png",selfType)->getCString(),UI_TEX_TYPE_LOCAL);

	//获取到是那个英雄。。。。。。
	targetBoss=obj;

	
}

void Ui_MainLayer::hideHeroInfo()
{
	do{
		HeroBtn->setVisible(false);
		HeroBtn->setTouchEnabled(false);
		if (skillPrg!=nullptr)
		{
			
			skillPrg->removeFromParent();
			skillPrg=nullptr;
		}
		if (targetBoss!=nullptr)
		{
			targetBoss=nullptr;
		}
	
	}while(0);
}


void Ui_MainLayer::setHeroHp(int hp,int maxHp)
{
	LoadingBar *hpBar = static_cast<LoadingBar*>(HeroBtn->getChildByName("ProgressBar"));
	hpBar->setPercent(hp*100/maxHp);
}

void Ui_MainLayer::eventTouchHeroBtn(Ref* sender, TouchEventType type)//说明是在点击英雄技能
{

	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (skillCDstatus)
		return;

	if (targetBoss!=nullptr)
	{
		

		//targetBoss->getArmature()->getAnimation()->play("call",-1,0);

		skillCDstatus = true;
		HeroBtn -> setPressedActionEnabled(false);
		//HeroBtn->setTouchEnabled(false);
		targetBoss->playSkillForBoss();//释放技能
		targetBoss->setcdleftTime(targetBoss->getskill1_skillCDTime());

		Sprite *sp = Sprite::createWithSpriteFrameName("UI_BossSkill_CD.png");
		skillPrg= ProgressTimer::create(sp);
		skillPrg->setType(ProgressTimer::Type::BAR);
		//    Setup for a bar starting from the top since the midpoint is 1 for the y
		skillPrg->setMidpoint(Point(0,1));
		//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
		skillPrg->setBarChangeRate(Point(0, 1));
		skillPrg->setPosition(Point(HeroBtn->getSize().width/2 ,HeroBtn->getSize().height/2));
		HeroBtn->addChild(skillPrg,1);
		float cdlefttime=targetBoss->getcdleftTime();//获取当前的冷却时间
		float  cdalltime=targetBoss->getskill1_skillCDTime();//获取总得冷却
		int precent=(int)(((float)cdlefttime/cdalltime)*100);//求出当前的百分比
		skillPrg->runAction(Sequence::create(
			ProgressFromTo::create(cdlefttime,precent,0),
			CCCallFuncO::create(this,callfuncO_selector(Ui_MainLayer::bossCdPrgCallfunc),HeroBtn),
			NULL));

		//新手引导教学
		if (UserRecord::getInstance()->getIsFreshMan() && UserRecord::getInstance()->getFreshManCurrentID()==200010 ){
			UserRecord::getInstance()->setFreshManMutex(false);
		}else{
			NotificationCenter::getInstance()->postNotification(PAUSEKEY, (Ref*)0);
		}


	}


}
void Ui_MainLayer::bossCdPrgCallfunc(Ref *sender){//冷却技能进度条的回调

	skillCDstatus = false;
	HeroBtn -> setPressedActionEnabled(true);
}



void Ui_MainLayer::refreshMagic(std::string id,Button *btn){

	auto user = UserRecord::getInstance();
	int num=user->getGoodsCountByID(id);
	Button* _curButton = btn;
	if (!_curButton)
	{
		if (id == "20001")
		{
			_curButton = m_MagicBtn_1;
		}else if(id == "20002")
		{
			_curButton = m_MagicBtn_2;
		}else if(id == "20003")
		{
			_curButton = m_MagicBtn_3;
		}else if(id == "20004")
		{
			_curButton = m_MagicBtn_4;
		}
	}

	if (num>0)
	{
		TextBMFont *magic_num = static_cast<TextBMFont*>(Helper::seekWidgetByName(_curButton, "magic_num"));
		magic_num->setText(Value(user->getGoodsCountByID(id)).asString());
		magic_num->setVisible(true);
		Widget*magic_add=static_cast<Widget*>(Helper::seekWidgetByName(_curButton, "magic_add"));
		magic_add->setVisible(false);//显示加号
	}
}


void Ui_MainLayer::reduceOneMagic(std::string id,Button *btn){
	auto user = UserRecord::getInstance();
	int num=user->getGoodsCountByID(id);

	// 扣除临时魔法
	user->alterTemporaryMagic(Value(id).asInt(), -1);
	// 扣除实时魔法
	user->alterGoodsCount(id, -1);
	
	if ((num-1)<=0)
	{ 
		
		TextBMFont *magic_num = static_cast<TextBMFont*>(Helper::seekWidgetByName(btn, "magic_num"));
		magic_num->setVisible(false);//隐藏数字
		Widget*magic_add=static_cast<Widget*>(Helper::seekWidgetByName(btn, "magic_add"));
		magic_add->setVisible(true);//显示加号
	}
	
	//连接服务器扣除魔法数量
	auto postDatas = __String::createWithFormat("c=props&iid=%s",id.c_str())->getCString();
	Connect::getInstance()->send(postDatas,this,httpresponse_selector(Ui_MainLayer::onHttpRequestCompleted));
}
void Ui_MainLayer::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		//报错
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		//处理成功
	}  else {
		//处理失败
		/*NotificationCenter::getInstance()->postNotification(PAUSEKEY, (Ref*)1);
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		auto Text = PlistManager::getInstance()->getPlistMap("Text");
		__String* TextStr = static_cast<__String*>(Text->objectForKey("1001"));
		prompt->initWithRetry(TextStr->getCString());
		Director::getInstance()->getRunningScene()->addChild(prompt,300);*/
	}
}
void  Ui_MainLayer::magic1SkillAction(Point pos,int floor){ //攻击

	audioPlayer::getInstance()->playEffect("S58.mp3");	// 攻击魔法音效
	
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->CreateMagicAtk(MAGIC_ATK,pos,floor);

	this->magicCdAction(m_MagicBtn_1,m_magicCdTime_1);
	if (!UserRecord::getInstance()->getIsFreshMan())
		this->reduceOneMagic("20001",m_MagicBtn_1);
	
}
void  Ui_MainLayer::magic2SkillAction(Point pos,int floor){//变羊

	audioPlayer::getInstance()->playEffect("S59.mp3");	// 变羊魔法音效
	
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->magicChangeToSheep(pos,floor);
	
	this->magicCdAction(m_MagicBtn_2,m_magicCdTime_2);
	if (!UserRecord::getInstance()->getIsFreshMan())
		this->reduceOneMagic("20002",m_MagicBtn_2);
}
void  Ui_MainLayer::magic3SkillAction(Point pos,int floor){//传送门
	audioPlayer::getInstance()->playEffect("S60.mp3");	//传送门魔法音效
	
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->CreateMagicTransmit(MAGIC_TRANSMIT,pos,floor);

	this->magicCdAction(m_MagicBtn_3,m_magicCdTime_3);
	if (!UserRecord::getInstance()->getIsFreshMan())
		this->reduceOneMagic("20003",m_MagicBtn_3);
}
void  Ui_MainLayer::magic4SkillAction(Point pos,int floor){//哥斯拉

	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->CreateMagicGosla(MAGIC_GODZILLA,pos,floor);

	this->magicCdAction(m_MagicBtn_4,m_magicCdTime_4);
	if (!UserRecord::getInstance()->getIsFreshMan())
		this->reduceOneMagic("20004",m_MagicBtn_4);
}
void  Ui_MainLayer::magicCdAction(Button* btn,float cdtime){

	btn->stopAllActions();
	btn->setScale(SCA_NOMARL);//还原大小
	btn->setTouchEnabled(false);

	if (UserRecord::getInstance()->getIsFreshMan())
		return;
	
	//20001攻击    20002 变羊  20003 传送门   20004 哥斯拉
	CCSprite *sp;
	if (magicIndex==1)//20001攻击
	{
		sp=Sprite::create("cd_atk.png");
	}
	if (magicIndex==2)//20002 变羊
	{
		sp=Sprite::create("cd_sheep.png");
	}
	if (magicIndex==3)// 20003 传送门
	{
		sp=Sprite::create("cd_trans.png");
	}
	if (magicIndex==4)// 20004 哥斯拉
	{
		sp=Sprite::create("cd_gesila.png");
	}
	sp->setScale(1.3);
	ProgressTimer*Prg= ProgressTimer::create(sp);
	Prg->setType(ProgressTimer::Type::BAR);
	Prg->setMidpoint(Point(0,1));
	Prg->setBarChangeRate(Point(0, 1));
	Prg->setPosition(Point(btn->getSize().width/2 ,btn->getSize().height/2));
	btn->addChild(Prg,1);
	Prg->setTag(1888);

	//float cdlefttime=targetBoss->getcdleftTime();//获取当前的冷却时间
	//float  cdalltime=targetBoss->getskill1_skillCDTime();//获取总得冷却
	int precent=(int)(((float)cdtime/cdtime)*100);//求出当前的百分比



	Prg->runAction(Sequence::create(
		ProgressFromTo::create(cdtime,precent,0),
		CallFuncN::create(CC_CALLBACK_1(Ui_MainLayer::magicCdCallback,this)),
		NULL));

}
void  Ui_MainLayer::magicCdCallback(Ref *b){
	
	ProgressTimer*Prg=static_cast<ProgressTimer*>(b);//进度条
	Button *btn=static_cast<Button*>(Prg->getParent());//获取父节点
	btn->setTouchEnabled(true); //设计可以触摸
	btn->removeChild(Prg);

	


}



// 创建魔法释放监听
void  Ui_MainLayer::createMagicListenerTouch()
{
	if (!_magicListener)
	{
		_magicListener = EventListenerTouchOneByOne::create();
		//CC_SAFE_RETAIN(_magicListener);
		_magicListener->setSwallowTouches(true); 
		_magicListener->onTouchBegan		= CC_CALLBACK_2(Ui_MainLayer::onMagicTouchBegan, this);
		_magicListener->onTouchMoved		= CC_CALLBACK_2(Ui_MainLayer::onMagicTouchMove, this);  
		_magicListener->onTouchEnded		= CC_CALLBACK_2(Ui_MainLayer::onMagicTouchEnd, this);  
		//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_magicListener, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_magicListener, TOUCH_Do_Magic); 
	}else
	{

		return;
	}
}
// 删除魔法释放监听
void  Ui_MainLayer::removeMagicListenerTouch()
{
	if (_magicListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_magicListener);
		_magicListener = nullptr;
	}
}

// 魔法释放监听，点击到某一层
bool Ui_MainLayer::onMagicTouchBegan(Touch* _touch, Event* _unusedEvent)  
{  
	Point _touchPos	= _touch->getLocation();
	float _mouseX	= _touchPos.x;
	Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
	if (mainBack)
	{
		int _nowStoreyIndex = 0;
		for (int i = 0; i < SceneData::getInstance()->getSceneDataMap().size(); ++i)
		{
			StoreyData* thisStoreyData = SceneData::getInstance()->getSceneDataMapStorey(i);
			if (thisStoreyData && thisStoreyData->mItem)
			{
				StoreyData* thisStoreyData = SceneData::getInstance()->getSceneDataMapStorey(i);
				Widget* thisItemCell = thisStoreyData->mItem;
				Widget* thisRectCtrl = thisItemCell->getChildByName("Image_Rect");
				Rect thisRect;
				thisRect.origin.x = thisRectCtrl->getWorldPosition().x - (thisRectCtrl->getSize().width/2);
				thisRect.origin.y = thisRectCtrl->getWorldPosition().y  - (thisRectCtrl->getSize().height/2);
				thisRect.size.width = thisRectCtrl->getSize().width;
				thisRect.size.height = thisRectCtrl->getSize().height;
				if (thisRect.containsPoint(_touchPos)){
					_nowStoreyIndex = thisStoreyData->getStoreyIndex();
					auto _nodePos = thisStoreyData->mItem->convertToNodeSpace(_touchPos);
					if (_nowStoreyIndex!=SCENE_STOREY_HOME&&_nowStoreyIndex!=SCENE_STOREY_GROUND)
					{
						clickPos         = _nodePos;
						clickStoreyIndex = _nowStoreyIndex;

						string magicName="";
						int _magicId = 0;
						
						switch (magicIndex)
						{
						case 1:							
							magicName = "The flash";
							break;
						case 2:	
							magicName = "sheep";
							break;
						case 3:
							magicName = "The portal";
							break;
						case 4:
							magicName = "Godzilla";
							break;
						default:
							break;
						}
						audioPlayer::getInstance()->playEffect("S66.mp3");
						Size sz = Director::getInstance()->getVisibleSize();
						Point orginPt = Director::getInstance()->getVisibleOrigin();

						ArmatureDataManager::getInstance()->addArmatureFileInfo("magicskill/td_mofatubiao0.png","magicskill/td_mofatubiao0.plist","magicskill/td_mofatubiao.ExportJson");
						Armature* skillEnterEff = Armature::create("td_mofatubiao");
						skillEnterEff->setPosition(Point(sz.width/2+orginPt.x - 40,sz.height/2+orginPt.y - 40));
						this->addChild(skillEnterEff,100);
						skillEnterEff->getAnimation()->play(magicName);
						skillEnterEff->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Ui_MainLayer::onFrameEvent));

						//如果不是新手引导暂停游戏
						if (!UserRecord::getInstance()->getIsFreshMan())
						{
							NotificationCenter::getInstance()->postNotification(PAUSEKEY,  (Ref*)0);
						}
					}

					return true;
				}
				else{//如果点到其他地方去了
					m_MagicBtn_1->setScale(SCA_NOMARL);
					m_MagicBtn_2->setScale(SCA_NOMARL);
					m_MagicBtn_3->setScale(SCA_NOMARL);
					m_MagicBtn_4->setScale(SCA_NOMARL);
					m_MagicBtn_1->stopAllActions();
					m_MagicBtn_2->stopAllActions();
					m_MagicBtn_3->stopAllActions();
					m_MagicBtn_4->stopAllActions();
					
				}
			}
		}
	}
	removeMagicListenerTouch();
	return false;
} 

void Ui_MainLayer::onMagicTouchMove(Touch* _touch, Event* _unusedEvent)
{
	CCLOG("Ui_MainLayer:: onMagicTouchMove");
}

void Ui_MainLayer::onMagicTouchEnd(Touch* _touch, Event* _unusedEvent)
{
	CCLOG("Ui_MainLayer:: onMagicTouchEnd");
	removeMagicListenerTouch();
}

void Ui_MainLayer::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"flash_finished")==0)
	{
		this->magic1SkillAction(clickPos,clickStoreyIndex);
		 
		if (UserRecord::getInstance()->getIsFreshMan()){
		 		UserRecord::getInstance()->setFreshManMutex(false);
		 		NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}else{
			NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}
	}else if(strcmp(evt.c_str(),"sheep_finished")==0){
		this->magic2SkillAction(clickPos,clickStoreyIndex);

		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}else
		{
			NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}
	}else if(strcmp(evt.c_str(),"portal_finished")==0){
		this->magic3SkillAction(clickPos,clickStoreyIndex);

		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
			NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}else{
			NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}
	}else if(strcmp(evt.c_str(),"godzilla_finished")==0){
		this->magic4SkillAction(clickPos,clickStoreyIndex);

		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
			NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}else{
			NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
		}
	}
}


// 更新预警小提示
void Ui_MainLayer::updateYuJing(bool isVisible, bool isUp)
{
	ImageView* upBack = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "Image_YuJingUpBack"));
	ImageView* downBack = static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "Image_YuJingDownBack"));
	if (!upBack || !downBack)
		return;
	
	if (isVisible)
	{
		if (isUp)
		{
			upBack->setVisible(true);
		}else
		{
			downBack->setVisible(true);
		}
	}else
	{
		upBack->setVisible(false);
		downBack->setVisible(false);
	}
}


void Ui_MainLayer::SetAtkDoorWarning(){

// 	Size visibleSize = Director::getInstance()->getVisibleSize();
// 	Size winSize=Director::getInstance()->getWinSize();
	Size winSize=Director::getInstance()->getWinSize();
	//CCLOG("winSize.width=%f,winSize.height=%f",winSize.width,winSize.height);
	if (waring==nullptr)
	{
		waring=Sprite::create("x_pic_warning.png");
		waring->setPosition(winSize.width/2,winSize.height/2);
		this->addChild(waring,-3);
		waring->setScaleX(winSize.width/waring->getContentSize().width);
		waring->setScaleY(winSize.height/waring->getContentSize().height);
		
		auto fto1=FadeOut::create(0.25f);
		auto fin=FadeIn::create(0.25f);
		auto fto2=FadeOut::create(0.25f);
		auto callback=CallFuncN::create(CC_CALLBACK_1(Ui_MainLayer::WarningCallback, this));
		auto seq=Sequence::create(fto1,fin,fto2,callback,NULL);

		waring->runAction(seq);

	}



}
void Ui_MainLayer::WarningCallback(Ref *f){
	waring->removeFromParentAndCleanup(true);
	waring=nullptr;
}

void Ui_MainLayer::setItemSelect(int _index, bool _isHideAll)
{
	for ( auto iter=m_itemInfo.begin();iter!=m_itemInfo.end();++iter )
	{
		Button* _button = nullptr;
		_button  = static_cast<Button*>(currPanel->getChildByName(String::createWithFormat("item_index_btn_%d",iter->first)->getCString()));
		if (_button)
		{
			Widget* _panelSelect = static_cast<Widget*>(_button->getChildByName("Panel_IsSelect"));
			if (_panelSelect)
			{
				_panelSelect->setVisible(false);
				if (!_isHideAll)
				{
					if (_index == iter->first)
					{
						_panelSelect->setVisible(true);
					}
				}
			}
		}
	}
}


//处理魔法按钮点击事件  
void Ui_MainLayer::eventTouch_Debug(Ref* sender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		ImageView* _curCtrl = static_cast<ImageView*>(sender);
		if (_curCtrl)
		{
			//Ui_Debug::getInstance()->setRun(true);
		}
	}
}
