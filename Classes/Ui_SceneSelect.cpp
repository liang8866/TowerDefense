#include "Ui_SceneSelect.h"
#include "Ui_HeroAltar.h"
#include "WindowManager.h"
#include "LoadingScene.h"
#include "levelScene.h"
#include "startScene.h"
#include "CachManager.h"
#include "Ui_Bag.h"
#include "audioPlayer.h"

const float EPSINON = 0.02f;
USING_NS_CC;
bool Ui_SceneSelect::init()
{
	if (!Widget::init())
	{
		return false;
	}
	bool Ret = false;
	do{
		
		_maxScale = 1.0f;
		_minScale = 0.5f;
		_distance = 0;
		_friction = 0.8f;
		_gravity = 0.05f;
		_autoScroll = false;
		_autoScale = false;
		_isRuning = false;
		moveEnabled = true;
		maps.clear();
		_cWidget=nullptr;
		_currentSence = UserRecord::getInstance()->getSectionNum();
		if (_currentSence > 3 || _currentSence <= 0)
		{
			_currentSence = 1;
		}
		_senceCount = 3;
		deltaPoint = Point::ZERO;
		_VisibleSize = Director::getInstance()->getVisibleSize();
		_VisibleOrigin =  Director::getInstance()->getVisibleOrigin();
		_mWidget = nullptr;
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/Map0.plist","Map/Map0.png");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/Map1.plist","Map/Map1.png");
		_widget = GUIReader::getInstance()->widgetFromJsonFile("Map/Ui_SceneSelect.ExportJson");
		CC_BREAK_IF(!_widget);
		
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Ui_SceneSelect::myEnabledTouch),"SCENE_OPEN_TOUCH",NULL);
		
		this->addChild(_widget,1);
		auto user = UserRecord::getInstance();
		this->schedule(schedule_selector(Ui_SceneSelect::update));
		
		initMap();

		//CCLOG("###");

		//各类按钮
		auto Image_248 = Helper::seekWidgetByName(_widget,"Image_248");
		for (auto btns : Image_248->getChildren())
		{
			auto btn = static_cast<Button*>(btns);
			if (strcmp(btn->getName(),"btn_1") == 0) //天赋
			{
				auto tips = btn->getChildByName("tips");
				auto num = static_cast<TextBMFont*>( tips->getChildByName("num"));
				if (UserRecord::getInstance()->getIsFreshMan())
				{
					tips->setVisible(false);
				} else {
					if (UserRecord::getInstance()->getStarCount() > 0)
					{
						tips->setVisible(true);
						num->setText(Value(UserRecord::getInstance()->getStarCount()).asString());
						auto repeat = RepeatForever::create( Sequence::create(ScaleTo::create(0.5f,1.3f) ,ScaleTo::create(0.2f,1.0f), NULL));
						tips->runAction(repeat);
					} else {
						tips->setVisible(false);
					}
				}
				
			} else if(strcmp(btn->getName(),"btn_2") == 0) {//英雄
				auto tips = btn->getChildByName("tips");
				if (UserRecord::getInstance()->getIsFreshMan())
				{
					tips->setVisible(false);
					
				} else {
					if (needShowHeroTips())
					{
						tips->setVisible(true);
						auto repeat = RepeatForever::create( 
							Sequence::create(ScaleTo::create(0.5f,1.3f) ,ScaleTo::create(0.2f,1.0f), NULL)
							);
						tips->runAction(repeat);
					} else
					{
						tips->setVisible(false);
					}
				}
			}
			btn->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		}
		
		//设置英雄之魂
		Widget* soul_info = dynamic_cast<Widget*>(Helper::seekWidgetByName(_widget,"soul_info"));
		Text* soul_num = dynamic_cast<Text*>(soul_info->getChildByName("pipe")->getChildByName("soul_num"));
		soul_num->setText( Value(user->getHeroSoul()).asString() );
		soul_num->setFontName("Arial-Bold");
		soul_num->setFontSize(19);
		soul_num->setColor(Color3B(255,255,255));

		_curSoul = UserRecord::getInstance()->getHeroSoul();
		Button* soul_buy = dynamic_cast<Button*>(Helper::seekWidgetByName(soul_info,"btn_soul_buy"));
		soul_buy->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		soul_info->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		//设置金币
		Widget* gold_info = dynamic_cast<Widget*>(Helper::seekWidgetByName(_widget,"gold_info"));
 		Text* gold_num = dynamic_cast<Text*>(gold_info->getChildByName("pipe")->getChildByName("gold_num"));
 		gold_num->setText( Value(user->getGold()).asString() );

 		gold_num->setFontName(MyfontName);
 		gold_num->setFontSize(19);
		gold_num->setColor(Color3B(255,255,255));

		_curGold = UserRecord::getInstance()->getGold();
		Button* gold_buy = dynamic_cast<Button*>(Helper::seekWidgetByName(gold_info,"btn_gold_buy"));
		gold_buy->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		gold_info->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
// 		Widget* p_bg = dynamic_cast<Widget*>(Helper::seekWidgetByName(_widget,"Panel_bg"));
// 		p_bg->setVisible(false);

		Helper::seekWidgetByName(_widget,"item_1")->setVisible(false);
		Helper::seekWidgetByName(_widget,"item_2")->setVisible(false);
		Helper::seekWidgetByName(_widget,"item_3")->setVisible(false);

		Button* add = dynamic_cast<Button*>(Helper::seekWidgetByName(_widget,"btn_back"));
		add->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));

		//下一地图.上一地图 topBar
		Button* nextBtn = static_cast<Button*>(_widget->getChildByName("next"));
		Button* prevBtn = static_cast<Button*>(_widget->getChildByName("prev"));
		Widget* next_warp = static_cast<Widget*>(_widget->getChildByName("next_warp"));
		Widget* prev_warp = static_cast<Widget*>(_widget->getChildByName("prev_warp"));
		next_warp->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		prev_warp->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		prevBtn->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));
		nextBtn->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));

		_freshmanBtn = static_cast<Button*>(_widget->getChildByName("freshmanBtn"));
		_freshmanBtn -> addTouchEventListener(this, toucheventselector(Ui_SceneSelect::eventTouchButton));

		Ret=true;
		fitScreen();
		initAnimation();
		// 恢复英雄所在层级, 死亡状态
		UserRecord::getInstance()->clearHeroValue_isDead_StoreyIndex();

		//播放背景音乐
		UserRecord::getInstance()->setMusicPlay("startScene_Music.mp3");
	}while (0);

	return Ret;
}

void Ui_SceneSelect::onEnter()
{
	Widget::onEnter();
}

void Ui_SceneSelect::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Map/Map0.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Map/Map1.plist");
	TextureCache::getInstance()->removeTextureForKey("Map/Map0.png");
	TextureCache::getInstance()->removeTextureForKey("Map/Map1.png");

	if (UserRecord::getInstance()->getSectionNum() == 2)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Map/td_guanka_shuibo_yu0.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Map/td_guanka_shuibo0.plist");
		TextureCache::getInstance()->removeTextureForKey("Map/td_guanka_shuibo_yu0.png");
		TextureCache::getInstance()->removeTextureForKey("Map/td_guanka_shuibo0.png");
	}

	Widget::onExit();
}

void Ui_SceneSelect::initMapData(const int chapterNum) 
{
	auto currMap = maps.at(chapterNum);
	Widget* btnWarp = static_cast<Widget*>(currMap->getChildByName("Buttons")) ;
	int k = 1;
	vector<Point> pos;
	vector<Point> boxPos;
	pos.clear();
	boxPos.clear();
	UserRecord* user = UserRecord::getInstance();
	for (auto& btns : btnWarp->getChildren())
	{
		Button* btn = static_cast<Button*>(btns);
		int starCount = user->getSceneStarCount(chapterNum,k);
		if (starCount >= 0)
		{
			Widget* item = nullptr;
			if ( starCount == 1 )
			{
				item = Helper::seekWidgetByName(_widget,"item_1")->clone();
			} else if( starCount == 2){
				item = Helper::seekWidgetByName(_widget,"item_2")->clone();
			} else if( starCount == 3) {
				item = Helper::seekWidgetByName(_widget,"item_3")->clone();
			} else if( starCount == 0){
				btn->runAction(RepeatForever::create(Sequence::create(ScaleBy::create(0.8f,1.2f),ScaleBy::create(0.6f,1/1.2f),NULL)));
		//		/*btns->setColor(Color3B::GREEN);*/
				btn->loadTextureNormal("GKXZ_009_c.png");
				Point tempPoint = btns->getPosition();
				pos.push_back(Point(tempPoint.x,tempPoint.y+50));
				if (user->getSectionNum()==1 && btn->getTag()==19860 ){
					user->setIsFirstScene(true);
				}
			} 
			if (item != nullptr)
			{
				item->setPosition(Point(0,5));
				item->setVisible(true);
				btn->addChild(item);
			}
			btn->setName(Value(k).asString().c_str());
			btn->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::buttonTouched));
			
			//user->setBoxEnemySceneBySection(0,chapterNum);
			if (user->getBoxEnemySceneBySection(chapterNum) == k ){
				Point tempPoint = btn->getPosition();
				boxPos.push_back(Point(tempPoint.x,tempPoint.y+50));
			}
			
			
		} else {
			btn->setGrey();
			auto childText = __String::createWithFormat("num_%d",k);
			auto dd = btn->getChildByName(childText->getCString());
			dd->setColor(Color3B::WHITE);
		}
		++k;
	}


	if (!pos.empty()){
		for (size_t i=0;i!=pos.size();++i ){
			Sprite *arrow = Sprite::create("image/JianTou.png");
			arrow->setPosition(pos.at(i));
			btnWarp->addChild(arrow,100);
			arrow->setScale(0.8f);
			arrow->runAction(RepeatForever::create(Sequence::create(
				MoveTo::create(0.8f,Point(arrow->getPositionX(),arrow->getPositionY()+10)),
				MoveTo::create(0.6f,Point(arrow->getPositionX(),arrow->getPositionY()-10)),
				NULL
				)));
		}
	}

	//宝箱怪特效
	if (!boxPos.empty()){
		for (size_t i=0;i!=boxPos.size();++i ){
			Widget *boxEnemyPt = GUIReader::getInstance()->widgetFromJsonFile("BoxEnemyPt/BoxEnemyPt.ExportJson");
			Point boxPt = Point(boxPos.at(i).x - boxEnemyPt->getSize().width/2 , boxPos.at(i).y - boxEnemyPt->getSize().height/2 - 100);
			boxEnemyPt->setPosition(boxPt);
			boxEnemyPt->setTag(9899);
			btnWarp->addChild(boxEnemyPt,100);

			Widget *boxPtIcon = boxEnemyPt->getChildByName("pt_bg");
			boxPtIcon->runAction(RepeatForever::create(Sequence::create(
				ScaleTo::create(0.8f,1.15f,1.15f),
				ScaleTo::create(0.5f,1.0f,1.0f),
				NULL
				)));
		}
	}
}

void Ui_SceneSelect::addBoxEnemyPromptInfo(Button *btn)
{
	Widget* btnWarp = static_cast<Widget*>(btn->getParent());
	if (btnWarp->getChildByTag(9899)){
		Widget *boxEnemyPt = static_cast<Widget*>(btnWarp->getChildByTag(9899));
		boxEnemyPt->setVisible(false);
	}

	Widget* ptInfo = GUIReader::getInstance()->widgetFromJsonFile("BoxEnemyIntro/BoxEnemyIntro.ExportJson");
	Widget *promptBg = ptInfo->getChildByName("prompt_bg");
	ImageView *arrow_up   = static_cast<ImageView*>(promptBg->getChildByName("Image_9"));
	ImageView *arrow_down = static_cast<ImageView*>(promptBg->getChildByName("Image_10"));
	static_cast<Sprite*>(arrow_up  ->getVirtualRenderer())->getTexture()->setAliasTexParameters();
	static_cast<Sprite*>(arrow_down->getVirtualRenderer())->getTexture()->setAliasTexParameters();

	Point pos = btn->getWorldPosition();
	Point ptInfoPos = Point(pos.x - ptInfo->getSize().width/2 - 5 , pos.y - ptInfo->getSize().height +5);

	if (btn->getWorldPosition().y < _VisibleSize.height/2){

		arrow_up->setVisible(false);
		arrow_down->setVisible(true);

		ptInfoPos.y = pos.y + arrow_down->getSize().height -20;
	}

	ptInfo->setPosition(ptInfoPos);
	ptInfo->setTag(9990);
	ptInfo->setTouchEnabled(true);
	ptInfo->setTouchPriority(-20);
	this->addChild(ptInfo,10);

	
	Button *beginBtn = static_cast<Button*>(promptBg->getChildByName("BeginBtn"));
	
	beginBtn->setName(btn->getName());
	beginBtn->addTouchEventListener(this, toucheventselector(Ui_SceneSelect::buttonTouched));
}

void Ui_SceneSelect::hideBoxEnemyPromptInfo()
{
	auto currMap = maps.at(_currentSence);
	Widget* btnWarp = static_cast<Widget*>(currMap->getChildByName("Buttons")) ;

	if (!this->getChildByTag(9990)){
		return;
	}

	do{
		Widget* ptInfo = static_cast<Widget*>(this->getChildByTag(9990));
		CC_BREAK_IF(!ptInfo);
		ptInfo->removeFromParentAndCleanup(true);

		if (btnWarp->getChildByTag(9899)){
			Widget *boxEnemyPt = static_cast<Widget*>(btnWarp->getChildByTag(9899));
			CC_BREAK_IF(!boxEnemyPt);
			boxEnemyPt->setVisible(true);
		}

	}while (0);

}


void Ui_SceneSelect::initMap()
{
	_widget->getChildByName("map_warp")->setPosition(_VisibleOrigin);
	_mWidget = _widget->getChildByName("map_warp")->getChildByName("warp");
	_mWidget->setSize(Size(_VisibleSize.width*3,_VisibleSize.height));
	_mWidget->setPosition(Point::ZERO);
	int i=1;
	do 
	{
		Widget* mWidget =GUIReader::getInstance()->widgetFromJsonFile(__String::createWithFormat("Map/Map_%d.ExportJson",i)->getCString());
		CC_BREAK_IF(!mWidget);
		mWidget->setTag(i);
		_mWidget->addChild(mWidget);
		mWidget->setPositionX((i-1)*_VisibleSize.width);
		maps.insert(std::make_pair(i,mWidget));
		float scaleX = _VisibleSize.width/mWidget->getSize().width;
		float scaleY = _VisibleSize.height/mWidget->getSize().height;
		float scale = scaleX < scaleY ? scaleX : scaleY;
		mWidget->setScale(scale);
		Point centerPoint  = Point(
			(i-1)*_VisibleSize.width+_VisibleSize.width/2-(mWidget->getSize().width*mWidget->getScale()/2),
			_VisibleSize.height/2-(mWidget->getSize().height*mWidget->getScale()/2));
		mWidget->setPosition(centerPoint);
		initMapData(i);//加载地图上的星星
		initMapAnimation(i); //加载地图上的动画
		++i;
	} while (i<=3);
	loadMap(_currentSence);
	setTouchEnabled(true);
}
void Ui_SceneSelect::myEnabledTouch(Ref* obj)
{
	
	setTouchEnabled(true);
	touchPoints.clear();
}
void Ui_SceneSelect::initAnimation()
{
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//下一地图.上一地图 topBar
	Button* nextBtn = static_cast<Button*>(_widget->getChildByName("next"));
	Button* prevBtn = static_cast<Button*>(_widget->getChildByName("prev"));
	
	Point temp = nextBtn->getPosition();
	nextBtn->setPositionX(originPt.x+visibleSize.width+100);
	nextBtn->runAction(
		Sequence::create(
		DelayTime::create(0.5f),
		MoveTo::create(0.4f,temp),
		NULL)
		);

	temp = prevBtn->getPosition();
	prevBtn->setPositionX(-100);
	prevBtn->runAction(
		Sequence::create(
		DelayTime::create(0.5f),
		MoveTo::create(0.4f,temp),
		NULL)
		
		);
	
	Widget* info_warp = static_cast<Widget*>(_widget->getChildByName("info_warp"));
	//
	temp = info_warp->getPosition();
	info_warp->setPositionY(originPt.y+visibleSize.height);
	info_warp->runAction(
		Sequence::create(
		DelayTime::create(0.5f),
		MoveTo::create(0.4f,temp),
		NULL)
		);



	Widget* Image_248 = static_cast<Widget*>(_widget->getChildByName("Image_248"));
	temp = Image_248->getPosition();
	Image_248->setPositionY(-100);
	Image_248->runAction(
		Sequence::create(
		DelayTime::create(0.5f),
		MoveTo::create(0.4f,temp),
		NULL)
		);

	//云动画
	
	auto Panel_bg = _widget->getChildByName("Panel_bg");
	for (auto & child : Panel_bg->getChildren())
	{
		ImageView* cloud = static_cast<ImageView*>(child);
		marqueeCloud(cloud);
	}
	auto Panel_36 = _widget->getChildByName("Panel_36");
	for (auto & child : Panel_36->getChildren())
	{
		ImageView* cloud = static_cast<ImageView*>(child);
		marqueeCloud(cloud);
	}
}

void Ui_SceneSelect::marqueeCloud(ImageView* cloud)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Point VisibleOrigin =  Director::getInstance()->getVisibleOrigin();
	float currx = cloud->getPositionX();
	float maxRight = VisibleOrigin.x+visibleSize.width+cloud->getSize().width/2;
	float dist = maxRight-currx;
	auto action = Sequence::create(
		MoveTo::create(dist/30,Point(maxRight,cloud->getPositionY())),
		CallFunc::create([=](){
			cloud->setPositionX(VisibleOrigin.x-cloud->getSize().width/2);
			marqueeCloud(cloud);
		}),
		NULL);
	cloud->runAction(action);
}
void Ui_SceneSelect::loadMap(const int chapterNum)
{
	UserRecord::getInstance()->setSectionNum(chapterNum);
	_mWidget->setPosition(Point(-(chapterNum-1)*_VisibleSize.width,0));
	bool scale = true;
	if (_cWidget != nullptr)
	{
		if(_cWidget->getScale() <=  _minScale+(_maxScale-_minScale)/2) {
			scale = false;
		}
		_cWidget->stopAllActions();
		_cWidget->setScale(_minScale);
		Point centerPoint  = Point(
			(_cWidget->getTag()-1)*_VisibleSize.width+_VisibleSize.width/2-(_cWidget->getSize().width*_cWidget->getScale()/2),
			_VisibleSize.height/2-(_cWidget->getSize().height*_cWidget->getScale()/2));
		_cWidget->setPosition(centerPoint);
	}
	afterChangeMap();

	float mscale;
	if (!scale)
	{
		mscale = _minScale+0.2f;
		_widget->getChildByName("map_name")->setVisible(true);
	} else {
		mscale = _maxScale+0.2f;
		_widget->getChildByName("map_name")->setVisible(false);
	}
	_cWidget->setScale(mscale);
	float cd = _cWidget->getSize().width*_cWidget->getScale()/2;
	float bd = _cWidget->getSize().height*_cWidget->getScale()/2;
	//CCLOG("%f,%f,%f",_cWidget->getSize().height,_cWidget->getScale(),bd);
	Point centerPoint  = Point(
		(chapterNum-1)*_VisibleSize.width+_VisibleSize.width/2-(cd),
		_VisibleSize.height/2-(bd));
	_cWidget->setPosition(centerPoint);

	Size mp = _cWidget->getSize()*_cWidget->getScale();
	Point mpp = _cWidget->getPosition();
	float px =mpp.x-(_cWidget->getSize().width*(_cWidget->getScale()-0.2f)-mp.width)/2;
	float py = mpp.y-(_cWidget->getSize().height*(_cWidget->getScale()-0.2f)-mp.height)/2;
	auto action = Spawn::create(
		ScaleTo::create(0.5f,_cWidget->getScale()-0.2f),
		MoveTo::create(0.5f,Point( px,py)),
		NULL);
	_isRuning=true;
	_cWidget->runAction(Sequence::create(action,CallFunc::create([=](){ 
		_isRuning = false;
		if (!scale)
		{
			_cWidget->setScale(_minScale);
		}
	}),NULL));
}
void Ui_SceneSelect::initMapAnimation(const int Sence_id)
{
	auto currMap = maps.at(Sence_id);
	if (Sence_id == 2)
	{
		auto itmewarp = currMap->getChildByName("items");
		auto item1 = itmewarp->getChildByName("item_1");
		auto item2 = itmewarp->getChildByName("item_2");
		auto item3 = itmewarp->getChildByName("item_3");
		auto item4 = itmewarp->getChildByName("item_4");
		loopUpDownAnimation(1,item1);
		loopUpDownAnimation(1,item2);
		loopUpDownAnimation(2,item3);
		loopUpDownAnimation(1,item4);

		//加载水动画
		ArmatureDataManager::getInstance()->addArmatureFileInfo("Map/td_guanka_shuibo0.png","Map/td_guanka_shuibo0.plist","Map/td_guanka_shuibo.ExportJson");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("Map/td_guanka_shuibo_yu0.png","Map/td_guanka_shuibo_yu0.plist","Map/td_guanka_shuibo_yu.ExportJson");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("Map/td_guanka_zhuanquan0.png","Map/td_guanka_zhuanquan0.plist","Map/td_guanka_zhuanquan.ExportJson");
		Armature* _armature1 = Armature::create("td_guanka_shuibo");
		Armature* _armature2 = Armature::create("td_guanka_shuibo_yu");
		currMap->addChild(_armature1,10);
		currMap->addChild(_armature2,10);

		_armature1->getAnimation()->play("play");
		_armature2->getAnimation()->play("play",-1,0);
		auto repeata = RepeatForever::create( 
			Sequence::create(
			DelayTime::create(5.0f),
			CallFunc::create([=](){
				_armature2->getAnimation()->play("play",-1,0);
			}),
			NULL)
			);
		_armature2->runAction(repeata);

		_armature1->setAnchorPoint(Point::ZERO);
		_armature1->setPosition(Point(203,72));
		
		_armature2->setAnchorPoint(Point::ZERO);
		_armature2->setPosition(Point(920,130));

		auto wheel = currMap->getChildByName("wheel");
		Armature* _wheel = Armature::create("td_guanka_zhuanquan");
		wheel->addChild(_wheel);
		_wheel->setPosition(Point::ZERO);
		_wheel->setScale(0.9f);
		_wheel->setAnchorPoint(Point::ZERO);
		_wheel->getAnimation()->play("play");
		

	} else if (Sence_id == 1 || Sence_id == 3) {

	 auto windmill = currMap->getChildByName("windmill");	
	 auto repeat = RepeatForever::create( RotateBy::create(4.0f, 360) );
	 windmill->runAction(repeat);
		//	loopWindmillAnimation(windmill);
	}

	if (Sence_id == 3)
	{
		auto mapImage = currMap->getChildByName("map");
	/*	mapImage->setVisible(false);*/
		//mapImage->setClippingEnabled();
		//	ClippingEnabled
		auto la = Layout::create();
		la->setClippingEnabled(true);
		la->setClippingType(LAYOUT_CLIPPING_STENCIL);
		la->setSize(_VisibleSize);
		la->setPositionX(2*_VisibleSize.width);
		la->setAnchorPoint(Point::ZERO);
		la->setColor(Color3B(0,0,0));
		_mWidget->addChild(la);
		auto Panel_39 = mapImage->getChildByName("Panel_39")->clone();
		la->addChild(Panel_39);
		mapImage->getChildByName("Panel_39")->removeFromParentAndCleanup(true);
		auto child1 = Panel_39->getChildByName("Image_36_1_2");
		auto child2 = Panel_39->getChildByName("Image_36_1");

		CCLOG("1  child1.x=%f,child2.x=%f",child1->getPositionX(),child2->getPositionX());
		child1->setPositionX(child1->getPositionX()-3.0);
	
// 		marqueeSand(child1);
// 		marqueeSand(child2);
		marqueeSand(Panel_39);
	//	CCLOG("2  child1.x=%f,child2.x=%f,panel.x=%f",child1->getPositionX(),child2->getPositionX(),Panel_39->getPositionX());

//  		Panel_39->setPosition(Point(-2020,Panel_39->getPositionY()));
// 		CCLOG("3  child1.x=%f,child2.x=%f,panel.x=%f",child1->getPositionX(),child2->getPositionX(),Panel_39->getPositionX());
// 		Panel_39->setVisible(true);
// 		Panel_39->setOpacity(255);
	}

}

void Ui_SceneSelect::marqueeSand(Widget* sand)
{
	float dist = abs(sand->getPositionX());
	float speed = 300;
	auto action  = Sequence::create(
		Spawn::create(
			MoveTo::create((dist-700)/speed,Point(-700,sand->getPositionY())),
			CallFunc::create([=](){
				for (auto node : sand->getChildren()) {
					node->runAction(FadeIn::create(0.5f));
				}
			}),
		NULL),
		Spawn::create(
				MoveTo::create(700/speed,Point(0,sand->getPositionY())),
				CallFunc::create([=](){
					for (auto node : sand->getChildren()) {
						node->runAction(FadeOut::create(700/speed));
					}
				}),
		NULL),
		NULL);
	//sand->runAction(action);
	auto repeata = RepeatForever::create( 
		Sequence::create(
		action,
		CallFunc::create(
		[=](){  sand->setPositionX(-dist); }
		),
		DelayTime::create(5.0f),
		NULL)
		);
	sand->runAction(repeata);
}
void Ui_SceneSelect::loopWindmillAnimation(Widget* windmill)
{

}
void Ui_SceneSelect::loopUpDownAnimation(int type,Node* obj)
{
	Sequence* _action;
	if (type == 1)
	{
		_action = Sequence::create(
			MoveTo::create(1,Point(obj->getPositionX(),obj->getPositionY()+10+1+rand()%9)),
			MoveTo::create(1,Point(obj->getPositionX(),obj->getPositionY())),
			CallFunc::create( std::bind(&Ui_SceneSelect::loopUpDownAnimation,this,1,obj)),
			NULL
			);
	} else if(type == 2) {
		_action = Sequence::create(
			RotateTo::create(1.5f, obj->getRotation()+2+rand()%9),
			RotateTo::create(1.5f, obj->getRotation()),
			CallFunc::create( std::bind(&Ui_SceneSelect::loopUpDownAnimation,this,2,obj)),
			NULL
			);
	}
	obj->runAction(_action);
}

void Ui_SceneSelect::afterChangeMap()
{
	//下一地图.上一地图 topBar
	auto  nextBtn =_widget->getChildByName("next");
	auto prevBtn = _widget->getChildByName("prev");
	auto next_warp = _widget->getChildByName("next_warp");
	auto  prev_warp = _widget->getChildByName("prev_warp");

	if (_currentSence == _senceCount)
	{
		nextBtn->setGrey();
		nextBtn->getChildByTag(1777)->setScaleX(-nextBtn->getChildByTag(1777)->getScaleX());
		prevBtn->removeGrey();
	} else if(_currentSence == 1) {
		prevBtn->setGrey();
		nextBtn->removeGrey();
	}  else {
		nextBtn->removeGrey();
		prevBtn->removeGrey();
	}
	_cWidget = maps.at(_currentSence);
	auto name = _cWidget->getChildByName("map_name")->clone();
	name->setVisible(true);
	name->setPosition(Point(_widget->getChildByName("map_name")->getSize().width/2,_widget->getChildByName("map_name")->getSize().height-20));
	_widget->getChildByName("map_name")->removeAllChildren();
	_widget->getChildByName("map_name")->addChild(name);

	float scaleX = _VisibleSize.width/_cWidget->getSize().width;
	float scaleY = _VisibleSize.height/_cWidget->getSize().height;
	if (_currentSence == 3)
	{
		_maxScale = 0.8f;
	} else {
		_maxScale = 1.0f;
	}
	_minScale = scaleX < scaleY? scaleX : scaleY;
}
void Ui_SceneSelect::changeSence(const int senceNum)
{
	//stopAutoScroll();
	//setTouchEnabled(false);
	

	loadMap(senceNum);

	return;
	Size VisibleSize = Director::getInstance()->getVisibleSize();
	_mWidget->setPosition(Point(-(senceNum-1)*VisibleSize.width,_mWidget->getPositionY()));
	bool scale = true;


	if (_currentSence != 1) 
	{
		//检查上一次的地图缩放比例
		if ( maps.at(_currentSence)->getScale() <=  _minScale+(_maxScale-_minScale)/2) {
			scale = false; //上一次是最小..不需要放大
		}  else {
			 maps.at(_currentSence)->setScale(_minScale);
		}
	}
	_maxScale = senceNum == 3? 0.8f : 1.0f;
	float scaleX = VisibleSize.width/maps.at(senceNum)->getSize().width;
	float scaleY = VisibleSize.height/maps.at(senceNum)->getSize().height;
	_minScale = scaleX < scaleY ? scaleX : scaleY;
	float mscale;
	if (!scale) //需要放大
	{
		mscale = _minScale+0.2f;
		_widget->getChildByName("map_name")->setVisible(true);
	} else {
		mscale = _maxScale+0.2f;
		_widget->getChildByName("map_name")->setVisible(false);
	}
	maps.at(senceNum)->setScale(mscale);


	auto mWidget = maps.at(senceNum);
	Point centerPoint  = Point(
		(senceNum-1)*VisibleSize.width+_VisibleSize.width/2-(mWidget->getSize().width*mWidget->getScale()/2)+_VisibleOrigin.x,
		_VisibleSize.height/2-(mWidget->getSize().height*mWidget->getScale()/2)+_VisibleOrigin.y);
	mWidget->setPosition(centerPoint);


	auto currMap = maps.at(senceNum);
	Size mp = currMap->getSize()*currMap->getScale();
	Point mpp = currMap->getPosition();
	float px = (currMap->getSize().width*(currMap->getScale()-0.2f)-mp.width)/2;
	float py = (currMap->getSize().height*(currMap->getScale()-0.2f)-mp.height)/2;
	//_mWidget->setPosition(Point( _mWidget->getPositionX()-px,_mWidget->getPositionY()-py));
	auto action = Spawn::create(
		ScaleTo::create(0.5f,currMap->getScale()-0.2f),
		MoveTo::create(0.5f,Point( mpp.x-px,mpp.y-py)),
		NULL);
	currMap->runAction(
		Sequence::create(action,CallFunc::create([=](){
			
		}),NULL)
		);

}
void Ui_SceneSelect::setTouchEnabled(bool enabled)
{

	//修改一下TIPS的数据
	auto Image_248 = Helper::seekWidgetByName(_widget,"Image_248");
	auto tips1 = Image_248->getChildByName("btn_1")->getChildByName("tips");
	auto num1 = static_cast<TextBMFont*>( tips1->getChildByName("num"));
	if (tips1->isVisible())
	{
		if (UserRecord::getInstance()->getStarCount() > 0)
		{
			tips1->setVisible(true);
			num1->setText(Value(UserRecord::getInstance()->getStarCount()).asString());
		} else {
			tips1->setVisible(false);
		}
	} else {
		if (!UserRecord::getInstance()->getIsFreshMan())
		{
			if (UserRecord::getInstance()->getStarCount() > 0)
			{
				tips1->setVisible(true);
				num1->setText(Value(UserRecord::getInstance()->getStarCount()).asString());
				auto repeat = RepeatForever::create( Sequence::create(ScaleTo::create(0.5f,1.3f) ,ScaleTo::create(0.2f,1.0f), NULL));
				tips1->runAction(repeat);
			} 
		} 
	}

	auto tips2 = Image_248->getChildByName("btn_2")->getChildByName("tips");
	auto num2 = static_cast<TextBMFont*>( tips2->getChildByName("num"));
	if (tips2->isVisible() && !needShowHeroTips())
	{
		tips2->setVisible(false);
	} else if(!tips2->isVisible() && needShowHeroTips()) {
		tips2->setVisible(true);
		auto repeat = RepeatForever::create( Sequence::create(ScaleTo::create(0.5f,1.3f) ,ScaleTo::create(0.2f,1.0f), NULL));
		tips2->stopAllActions();
		tips2->runAction(repeat);
	}

	if (UserRecord::getInstance()->getIndexPost() < 4)
	{
		tips2->setVisible(false);
	}
	if (UserRecord::getInstance()->getIndexPost() < 5) {
		tips1->setVisible(false);
	}


	if (enabled == _touchEnabled)
	{
		return;
	}
	_touchEnabled = enabled;
	if (_touchEnabled)
	{
		//多点触摸
		_touchListener = EventListenerTouchAllAtOnce::create();
		CC_SAFE_RETAIN(_touchListener);
		_touchListener->onTouchesBegan = CC_CALLBACK_2(Ui_SceneSelect::onTouchesBegan, this);
		_touchListener->onTouchesMoved = CC_CALLBACK_2(Ui_SceneSelect::onTouchesMoved, this);
		_touchListener->onTouchesEnded = CC_CALLBACK_2(Ui_SceneSelect::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, _mWidget);
	}
	else
	{
		_eventDispatcher->removeEventListener(_touchListener);
		CC_SAFE_RELEASE_NULL(_touchListener);
	}
}
void Ui_SceneSelect::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	hideBoxEnemyPromptInfo();
	stopAutoScroll();
	for ( auto &item: touches )
	{
		auto touch = item;
		auto location = touch->getLocation();
		vector<Point>::iterator itr = std::find(touchPoints.begin(),touchPoints.end(),touch->getPreviousLocation());
		if (itr == touchPoints.end())
		{
			//points.insert(std::make_pair(touch->getID(), location));
			touchPoints.push_back(location);
		} else {
			*itr = location;
		}
	}
}

void Ui_SceneSelect::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
	if (touches.empty() || touchPoints.empty())
	{
		return;
	}

	for ( auto &item: touches )
	{
		auto touch = item;
		auto location = touch->getLocation();
		vector<Point>::iterator itr = std::find(touchPoints.begin(),touchPoints.end(),touch->getPreviousLocation());
		if (itr != touchPoints.end())
		{
			*itr = location;
		}
	}



	if (touchPoints.size() == 1 )
	{
		if (_mWidget->getScale() > _minScale && moveEnabled)
		{
			deltaPoint = touches[0]->getDelta();
			
			Move();
		}
	} else if(touchPoints.size() > 1) {
		Zoom();
	}


	return;
	if (touches.empty() || points.empty())
	{
		return;
	}
	for ( auto &item: touches )
	{
		auto touch = item;
		auto location = touch->getLocation();
		std::map<int,Point>::iterator itr;
		itr = points.begin();
		while (itr != points.end())
		{
			if (itr->second == touch->getPreviousLocation())
			{
				itr->second = location;
			}
			++itr;
		}
		//points.at(touch->getID()) = location;
	}

	if (points.size() == 1 )
	{
		if (_mWidget->getScale() > _minScale && moveEnabled)
		{
			deltaPoint = touches[0]->getDelta();
			Move();
		}
	} else{
		Zoom();
	}
}

void Ui_SceneSelect::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
	if (touches.empty() || touchPoints.empty())
	{
		return;
	}
	if (touchPoints.size() == 1)
	{
		if (moveEnabled)
		{
			_autoScroll = true;
		} else {
			moveEnabled = true;
		}
	} else if(touchPoints.size() >=2) {
		if (touches.size() == 1)
		{


		}

		_distance = 0;
		deltaPoint = Point::ZERO;
		_autoScale = true;
		moveEnabled  = false;
		autoScale(touchPoints.at(0),touchPoints.at(1));
	} 

	for ( auto &item: touches )
	{
		auto touch = item;
		vector<Point>::iterator itr = std::find(touchPoints.begin(),touchPoints.end(),touch->getPreviousLocation());
		if (itr != touchPoints.end())
		{
			touchPoints.erase(itr);
		}
		//points.erase(touch->getID());
	}
}

void Ui_SceneSelect::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
	onTouchesEnded(touches,event);
}
Point Ui_SceneSelect::autoAdjustPosition(Point target)
{

	 Size vsizt = Director::getInstance()->getVisibleSize();
	 Point Origin =  Director::getInstance()->getVisibleOrigin();
	 auto widget = maps.at(_currentSence);
	
	 float left,right,top,bottom; 
	  bottom =  Origin.y;
	 if (widget->getScale() <= _minScale)
	 {
		  left   =   0;
		  right  = _VisibleSize.width   -	_mWidget->getSize().width;
		  top    = _VisibleSize.height  - _mWidget->getSize().height;
	 } else {
		  left = (_currentSence-1)*vsizt.width;
		  right = _currentSence*vsizt.width -	widget->getSize().width*widget->getScale() ;
		  top = vsizt.height  - widget->getSize().height*widget->getScale() ;
	 }
	
	float targetX = target.x;
	float targetY = target.y;
	//if (targetX == 1000 && targetY == 1000)
	//{
	//	targetX = vsizt.width/2;
	//	targetY = vsizt.height/2;
	//}
	if ( targetX > left)
	{
		targetX = left;
	}

	if (targetX < right)
	{
		targetX = right;
	}

	if ( targetY < top )
	{
		targetY = top;
	}
	if (targetY > bottom)
	{
		targetY = bottom;
	}
	return Point(targetX,targetY);
}


void Ui_SceneSelect::fitScreen()
{
	Widget *bg = static_cast<Widget*>(Helper::seekWidgetByName(_widget,"Panel_bg"));
	float _scale = MIN(_VisibleSize.width/960 , _VisibleSize.height/640);

	bg->setScaleX(_VisibleSize.width/960);
	bg->setScaleY(_VisibleSize.height/640);
	bg->setPosition(Point(_VisibleOrigin.x , _VisibleOrigin.y));

	Widget *topBar = static_cast<Widget*>(Helper::seekWidgetByName(_widget,"map_name"));
	Widget *info_warp = static_cast<Widget*>(Helper::seekWidgetByName(_widget,"info_warp"));
	topBar->setPosition(Point(_VisibleSize.width/2  - topBar->getSize().width/2*_scale  + _VisibleOrigin.x,
						  _VisibleSize.height- topBar->getSize().height*_scale - _VisibleOrigin.y-15
				   ));

	info_warp->setPosition(Point(_VisibleSize.width-info_warp->getSize().width*_scale +_VisibleOrigin.x,
		_VisibleSize.height-info_warp->getSize().height*_scale + _VisibleOrigin.y
		));
	info_warp->setScale(_scale);



	Widget *prev_warp = static_cast<Widget*>(Helper::seekWidgetByName(_widget,"prev_warp"));
	Widget *next_warp = static_cast<Widget*>(Helper::seekWidgetByName(_widget,"next_warp"));

	prev_warp->setPosition(Point(
		_VisibleOrigin.x+10,
		_VisibleOrigin.y+_VisibleSize.height/2-prev_warp->getSize().height/2
		));
	next_warp->setPosition(Point(
		_VisibleOrigin.x+_VisibleSize.width-(next_warp->getSize().width*next_warp->getScale())-10,
		_VisibleOrigin.y+_VisibleSize.height/2-next_warp->getSize().height/2
		));

	Button *prev = static_cast<Button*>(Helper::seekWidgetByName(_widget,"prev"));
	Button *next = static_cast<Button*>(Helper::seekWidgetByName(_widget,"next"));
	
	prev->setPosition(Point(
		_VisibleOrigin.x+(prev->getSize().width*prev->getScale())/2+10,
		_VisibleOrigin.y+_VisibleSize.height/2
		));
	next->setPosition(Point(
		_VisibleOrigin.x+_VisibleSize.width-(next->getSize().width*next->getScale())/2,
		_VisibleOrigin.y+_VisibleSize.height/2
		));
	Button *backBtn = static_cast<Button*>(Helper::seekWidgetByName(_widget,"btn_back"));
	backBtn->setPosition(Point(backBtn->getPositionX()+Director::getInstance()->getVisibleOrigin().x,
							   backBtn->getPositionY()+Director::getInstance()->getVisibleOrigin().y
							 ));

	_freshmanBtn -> setPosition(Point(_VisibleOrigin.x + _freshmanBtn->getSize().width/2 , 
		_VisibleOrigin.y + _VisibleSize.height - _freshmanBtn->getSize().height/2 +20));
}
void Ui_SceneSelect::buttonTouched(Ref* sender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
	
		Button* btn = dynamic_cast<Button*>(sender);
		Value num(btn->getName());
		
		if (UserRecord::getInstance()->getBoxEnemySceneBySection(_currentSence) == num.asInt() && btn->getParent()->getParent()->getTag() !=9990 ){
			hideBoxEnemyPromptInfo();
			addBoxEnemyPromptInfo(btn);
			return;
		}else{
			//hideBoxEnemyPromptInfo();
		}

		//audioPlayer::getInstance()->pauseBackgroundMusic();
		//audioPlayer::getInstance()->playEffect("S1.mp3");

		
		
		btn->setPressedActionEnabled(false);
		//btn->setTouchEnabled(false);//屏蔽掉，不能继续点击
		UserRecord::getInstance()->setSectionNum(_currentSence);
		UserRecord::getInstance()->setSceneNum(num.asInt());
		UserRecord::getInstance()->setIsGameStart(false);
		UserRecord::getInstance()->setRound(0);

		CachManager::getInstance()->cleanCach();
		//战斗准备场景,第三关教学场景
		if (UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()==3){
			UserRecord::getInstance()->takeUnitsInit();
			std::map<int,std::string> _map;
			_map.insert(make_pair(1,"war1"));
			_map.insert(make_pair(2,"war2"));
			_map.insert(make_pair(3,"tower1"));
			_map.insert(make_pair(4,"trap1"));
			_map.insert(make_pair(5,"trap2"));
			
			UserRecord::getInstance()->setTakeUnits(_map);

			//load plist
			CachManager::getInstance()->freshCach();

			Scene* pTScene =LoadingScene::createScene();
			LoadingScene*  cl = static_cast<LoadingScene*> (pTScene->getChildByTag(6));
			cl->loadScene(GAME_SCENE_TAG);
			TransitionFade *transitionfadeScene=TransitionFade::create(0.6f,pTScene);
			Director::getInstance()->replaceScene(transitionfadeScene);
		}else{

			Scene* pTScene = ReadyScene::createScene();
			TransitionFade* pTranslateScene = TransitionFade::create(0.6f,pTScene);;
			Director::sharedDirector()->replaceScene(pTranslateScene);
		}
	}
}
void Ui_SceneSelect::eventTouchButton(Ref* sender, TouchEventType type)
{
	if (type==TOUCH_EVENT_BEGAN){
		hideBoxEnemyPromptInfo();
	}
//CCLOG("textures:%s",TextureCache::getInstance()->getCachedTextureInfo().c_str()); 
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
			Widget* btn = dynamic_cast<Widget*>(sender);
// 		if ( btn->getTag() != 16035 || btn->getTag() != 16104 || btn->getTag() !=16187)
// 		{
// 			audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
// 		}
		
		if(btn->getTag() == 16437) //返回
		{
			//audioPlayer::getInstance()->stopBackgroundMusic();//停止背景音乐
			Scene* pTScene = startScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(0.6,pTScene);
			Director::sharedDirector()->replaceScene(transitionfadeScene);

		}  else if(btn->getTag() == 16010 ||btn->getTag() == 16100 || btn->getTag() == 16035 || btn->getTag() == 16104 || btn->getTag() ==16187)//商店
		{
			setTouchEnabled(false);
			Ui_Shop* shopLayer = Ui_Shop::create();
			this -> addChild(shopLayer,101);
			if (btn->getTag() == 16104 || btn->getTag() == 16100 ) //购买金币
			{
				shopLayer  -> selectIndex(ShopItem::GoldItem);
			}else {
				shopLayer  -> selectIndex(ShopItem::SoulItem);
			}
		}else if(btn->getTag() == 16185) //选择了英雄
		{
			setTouchEnabled(false);
			Ui_HeroAltar* _curWindow = Ui_HeroAltar::create();
			if (_curWindow)
			{
				_curWindow->setTag(1818);
				this->addChild(_curWindow, 102);
				_curWindow->createView(true);
			}

			//新手引导教学
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}
		}
		else if (btn->getTag()==16183)//如果是科技树(天赋)的话
		{
			setTouchEnabled(false);
			levelScene *lScene=static_cast<levelScene*>(this->getParent());
			lScene->upgradesUiComeIn();
			lScene->selectUiMoveOut();

		} else if(btn->getTag()==15988  || btn->getTag() == 44474) { //上一地图
			if (_currentSence-1 >=1)
			{  
				//Button* prevBtn = static_cast<Button*>(_widget->getChildByName("prev"));
				//Widget* prev_warp = static_cast<Widget*>(_widget->getChildByName("prev_warp"));
			//	prevBtn->setTouchEnabled(false);
			//	prev_warp->setTouchEnabled(false);
				changeSence(--_currentSence);
			}
		} else if(btn->getTag() == 15990 || btn->getTag() == 44476) { //下一地图
			if (_currentSence+1 <= _senceCount)
			{
				//Button* nextBtn = static_cast<Button*>(_widget->getChildByName("next"));
				//Widget* next_warp = static_cast<Widget*>(_widget->getChildByName("next_warp"));
				//nextBtn->setTouchEnabled(false);
				//next_warp->setTouchEnabled(false);
				changeSence(++_currentSence);
			}
		}else if (btn->getTag() == 16189)
		{
			setTouchEnabled(false);
			Ui_Bag* _thisWnd = Ui_Bag::create();
			this-> addChild(_thisWnd, 102);
		}else if (btn->getTag() == 5955){  //教学入口
			levelScene *levScene = static_cast<levelScene*>(this->getParent());
			levScene-> freshmanPrompt();
		}

		audioPlayer::getInstance()->playEffect("S1.mp3");//点击声音
	}
}

void Ui_SceneSelect::Move()
{
	if (_isRuning )
	{
		return;
	}
	//CCLOG("%f,_minScale=%f",maps.at(_currentSence)->getScale(),_minScale);
	if (maps.at(_currentSence)->getScale() > _minScale)
	{
		Point totalPoint =autoAdjustPosition(maps.at(_currentSence)->getPosition()+deltaPoint) ;
		maps.at(_currentSence)->setPosition(totalPoint);
	} else {
		Point totalPoint =autoAdjustPosition(_mWidget->getPosition()+deltaPoint);
		_mWidget->setPosition(totalPoint);
	}

	
}

void Ui_SceneSelect::Zoom()
{
	if (_isRuning)
	{
		return;
	}
	_widget->getChildByName("map_name")->setVisible(false);
	Point p1 = touchPoints.at(0);
	Point p2 = touchPoints.at(1);
	float mdistance = p1.getDistance(p2);
	if (_distance == 0)
	{
		_distance = mdistance;
		return;
	}

	if (mdistance == _distance)
	{
		return;
	}
	auto widget = maps.at(_currentSence);
	float scale = mdistance/_distance * widget->getScale();
	if (scale <_minScale-0.1f)
	{
		scale = _minScale-0.1f;
	} else if(scale >_maxScale+0.3f) {
		scale = _maxScale+0.3f;
	}
	_distance = mdistance;
	Size temp = widget->getSize()*widget->getScale();
	widget->setScale(scale);
	Point tempp = Point(
		widget->getPositionX() + (temp.width-widget->getSize().width*scale)/2,
		widget->getPositionY() + (temp.height-widget->getSize().height*scale)/2
		);
	widget->setPosition(tempp);
}
void Ui_SceneSelect::autoScale(cocos2d::Point p1,cocos2d::Point p2)
{
	if (_isRuning)
	{
		return;
	}
	auto widget = maps.at(_currentSence);
	float d = _minScale+ (_maxScale-_minScale)/2;
	float speed  = 2500.0f;
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size VisibleSize = Director::getInstance()->getVisibleSize();
	if (widget->getScale() < d) //到最小
	{
		_isRuning = true;

		Point centerPoint  = Point(
			(_currentSence-1)*VisibleSize.width+VisibleSize.width/2-(widget->getSize().width*_minScale/2),
			VisibleSize.height/2-(widget->getSize().height*_minScale/2));
		float dist = widget->getPosition().getDistance(centerPoint);
		auto action = Spawn::create(
			MoveTo::create(dist/speed,centerPoint),
			ScaleTo::create( dist/speed,_minScale),
			NULL);
		auto ss = Sequence::create(
			action,
			CallFunc::create(
			[&](){
				moveEnabled = true;
				_isRuning = false;
				_widget->getChildByName("map_name")->setVisible(true);
		} ),
			NULL
			);
		widget->runAction(ss);
	} else if(widget->getScale() >d ) { //最大
		_isRuning = true;
		Size temp = widget->getSize()*widget->getScale();
		Point tempp = Point(
			widget->getPositionX() + (temp.width-widget->getSize().width*_maxScale)/2,
			widget->getPositionY() + (temp.height-widget->getSize().height*_maxScale)/2
			);
		float dist = widget->getPosition().getDistance(tempp);

		auto action = Spawn::create(
			MoveTo::create(dist/speed,tempp),
			ScaleTo::create( dist/speed,_maxScale),
			NULL);
		auto ss = Sequence::create(
			action,
			CallFunc::create(
			[&](){
				moveEnabled = true;
				_isRuning = false;
				_widget->getChildByName("map_name")->setVisible(false);
		} ),
			NULL
			);
		widget->runAction(ss);
	}
}

void Ui_SceneSelect::stopAutoScale()
{
	_autoScale = false;
	moveEnabled = true;
}
void Ui_SceneSelect::stopAutoScroll()
{
	_autoScroll = false;
	if (_isRuning)
	{
		return;
	}
	Size VisibleSize = Director::getInstance()->getVisibleSize();
	if ( abs(_mWidget->getPositionX()) > (_currentSence-1)*VisibleSize.width || abs(_mWidget->getPositionX()) < (_currentSence-1)*VisibleSize.width)
	{
		_mWidget->runAction(MoveTo::create(0.2f,Point(
			-(_currentSence-1)*VisibleSize.width,
			_mWidget->getPositionY()
			)));
	}
}

void Ui_SceneSelect::autoScroll(){
	if (_isRuning)
	{
		return;
	}
	//CCLOG("_currentSence = %f,_minScale = %f",maps.at(_currentSence)->getScale(),_minScale);
	if (maps.at(_currentSence)->getScale() <= _minScale)
	{
		_autoScroll = false;
		float speed = 0.3f;
		float moveSpeed = 10.0f;
		float px = abs(_mWidget->getPositionX());

		float totalx=(_currentSence-1)*_VisibleSize.width;
		if (totalx-px >= 0.1f ) //向左
		{
			_isRuning = true;
			if (px <totalx-_VisibleSize.width*0.3f || abs(deltaPoint.x) > moveSpeed)
			{
				_mWidget->runAction(Sequence::create(
					MoveTo::create(speed,Point(-totalx+_VisibleSize.width,0)),
					CallFunc::create([=](){
						_mWidget->setPosition(Point(-totalx+_VisibleSize.width,0));
						--_currentSence;
						_isRuning = false;
						afterChangeMap();
					}),
					NULL));
			} else {
				_mWidget->runAction(Sequence::create(
					MoveTo::create(speed,Point(-totalx,0)),
					CallFunc::create([=](){
						_isRuning = false;
					}),
					NULL));
			}
		} else if (px - totalx > 0.1f)
		{
			_isRuning = true;
			if (px > totalx+_VisibleSize.width*0.3f || abs(deltaPoint.x) > moveSpeed)
			{
				_mWidget->runAction(Sequence::create(
					MoveTo::create(speed,Point(-_currentSence*_VisibleSize.width,0)),
					CallFunc::create([=](){
						_mWidget->setPosition(Point(-_currentSence*_VisibleSize.width,0));
						++_currentSence;
						_isRuning = false;
						afterChangeMap();
					}),
					NULL));
			} else
			{
				_mWidget->runAction(Sequence::create(
					MoveTo::create(speed,Point(-totalx,0)),
					CallFunc::create([=](){
						_isRuning = false;
					}),
					NULL));
			}
		}

	} else {
		float abspeedx = abs(deltaPoint.x);
		float abspeedy = abs(deltaPoint.y);
		if (  (abspeedx <= EPSINON && abspeedx >= -EPSINON) && (abspeedy <= EPSINON && abspeedy >= -EPSINON) )
		{
			deltaPoint = Point::ZERO;
			_autoScroll = false;
			return;
		}
		deltaPoint.x *= _friction;
		deltaPoint.y *= _friction;
		Point totalPoint =autoAdjustPosition(maps.at(_currentSence)->getPosition()+deltaPoint) ;
		maps.at(_currentSence)->setPosition(totalPoint);
	}
	
}

void Ui_SceneSelect::update(float dt)
{
	if (_autoScroll)
	{
		autoScroll();
	}

	if (_curSoul != UserRecord::getInstance()->getHeroSoul())
	{
		//设置英雄之魂
		Widget* soul_info = dynamic_cast<Widget*>(Helper::seekWidgetByName(_widget,"soul_info"));
		Text* soul_num = dynamic_cast<Text*>(soul_info->getChildByName("pipe")->getChildByName("soul_num"));
		soul_num->setText( Value(UserRecord::getInstance()->getHeroSoul()).asString() );
	}

	if (_curGold != UserRecord::getInstance()->getGold())
	{
		//设置金币
		Widget* gold_info = dynamic_cast<Widget*>(Helper::seekWidgetByName(_widget,"gold_info"));
		Text* gold_num = dynamic_cast<Text*>(gold_info->getChildByName("pipe")->getChildByName("gold_num"));
		gold_num->setFontName("Marker Felt");
		gold_num->setText( Value(UserRecord::getInstance()->getGold()).asString() );
	}
}

/**
	* 计算两个手指之间中心点的坐标。
	* 
	* @param event
	*/
Point Ui_SceneSelect:: getPointCenter(Point p1,Point p2)
{
	return Point((p1.x + p2.x) / 2,(p1.y + p2.y) / 2); 
}

Ui_SceneSelect::~Ui_SceneSelect()
{
	NotificationCenter::getInstance()->removeObserver(this, "SCENE_OPEN_TOUCH");

	
	
}


bool Ui_SceneSelect::needShowHeroTips()
{
	bool ret = false;
	auto _curPlist = PlistManager::getInstance()->getPlistMap("BossData");
	auto _curArray = _curPlist->allKeys();
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		if (ret)
		{
			break;
		}
		int _curTypeIndex = i + 1;
		auto _curName = StringUtils::format("boss%d", _curTypeIndex);
		auto _curNameObject = PlistManager::getInstance()->getPlistObject(_curPlist, _curName.c_str());
		if (_curNameObject)
		{
			HeroItemInfo _thisItemData;
			_thisItemData.index = i;
			_thisItemData.id = _curTypeIndex;
			auto _haveData = UserRecord::getInstance()->getHeroData(_thisItemData.id);
			_thisItemData.keyName = _curName;
			auto user = UserRecord::getInstance();
			if (_haveData.unLock > 0)
			{
				//检查是否够钱升级
				auto _costGold_upLevel =  PlistManager::getInstance()->getAttributeCCString("BossData", _curName.c_str(), _haveData.level, "upgradeMoney").intValue();
				if ( _haveData.level < 40 && _costGold_upLevel <= user->getGold())
				{
					ret = true;
					break;
				}

				
				//检查是否够钱升技能
				__String skill1 = PlistManager::getInstance()->getHeroSkill(_curName.c_str(),1,_haveData.skill1_level,"skilllupgradeMoney");
				if (_haveData.skill1_level < 10 && skill1.intValue() <= user->getGold())
				{
					ret = true;
					break;
				}
				__String skill2 = PlistManager::getInstance()->getHeroSkill(_curName.c_str(),2,_haveData.skill2_level,"skilllupgradeMoney");
				if (_haveData.skill2_level < 10 && skill2.intValue() <= user->getGold())
				{
					ret = true;
					break;
				}
				__String skill3 = PlistManager::getInstance()->getHeroSkill(_curName.c_str(),3,_haveData.skill3_level,"skilllupgradeMoney");
				if (_haveData.skill3_level < 10 && skill3.intValue() <= user->getGold())
				{
					ret = true;
					break;
				}
			} else {
				//检查是否够钱解锁




			}
		}
	}
	return ret;
}