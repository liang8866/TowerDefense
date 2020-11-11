#include "Ui_GameOver.h"
#include "UserRecord.h"
#include "WindowManager.h"
#include "levelScene.h"
#include "progressScene.h"
#include "GameScene.h"
#include "CachManager.h"
#include "LoadingScene.h"
#include "ReadyScene.h"
#include "audioPlayer.h"
#include "startScene.h"
using namespace network;
Ui_GameOver::Ui_GameOver():randNum(0)
{

}

Ui_GameOver::~Ui_GameOver()
{

}

bool Ui_GameOver::init ()
{
	if (!Widget::init())
		return false;

	DelayTimeFlag = false;
	_isWin = false;
	_isHttpFinish = false;
	_isHttpSuccess = false;
	hasBoxWard =false;
	star_cnt=0;
	if (UserRecord::getInstance()->getBoxEnemyDieMethod()==1){
		hasBoxWard = true;
	}
	auto RunningScene = Director::getInstance()->getRunningScene();
	if (RunningScene)
	{
		auto child = RunningScene->getChildByTag(249);
		if (child){
			Director::getInstance()->getRunningScene()->removeChild(child);
		}
	}
	return true;
}

void Ui_GameOver::onEnter()
{
	Widget::onEnter();
}

void Ui_GameOver::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_GameOver/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_GameOver/GameUI0.png");
	Widget::onExit();
}

void Ui_GameOver::createLayer(){

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_GameOver/GameUI0.plist","Ui_GameOver/GameUI0.png");
	m_widget = GUIReader::getInstance()->widgetFromJsonFile("Ui_GameOver/Ui_GameOver.ExportJson");
	this->addChild(m_widget,1);
	UserRecord::getInstance()->setIsGameOver(true);
	
	// 如果是在事件104关卡
	if (UserRecord::getInstance()->getIsEvent104())
	{
		UserRecord::getInstance()->takeUnitsInit();
		UserRecord::getInstance()->setIsEvent104(false);
	}
	m_widget->setTouchEnabled(true);
	m_widget->setTouchPriority(TOUCH_UI_PRIORITY);

	fitScreen();
}
//胜利页面
void Ui_GameOver::winTheGame(float times)
{
	if (!DelayTimeFlag)
	{
		auto action = Sequence::create(
			DelayTime::create(times),
			CallFunc::create( 
			[=](){
				DelayTimeFlag = true;
				winTheGame(times);
			}
		),
			NULL);
		this->runAction(action);
		return;
	}

	if (hasBoxWard){
		randNum = randomBoxEnemyReward();
		string filename;
		if (randNum==1){
			filename = "image/GKXZ_013.png";   //金币
		}else if (randNum==2){
			filename = "image/GKXZ_014.png";   //英雄之魂
		}else if (randNum==3){
			filename = "image/Egg_UpStar.png"; //祝福蛋
		}

		int count=0;
		if (UserRecord::getInstance()->getSectionNum()==1){
			if (randNum==1){
				count=300;
			}else if (randNum==2){
				count=30;
			}else if (randNum==3){
				count=10;
			}
		}else if (UserRecord::getInstance()->getSectionNum()==2){
			if (randNum==1){
				count=400;
			}else if (randNum==2){
				count=40;
			}else if (randNum==3){
				count=14;
			}
		}else if (UserRecord::getInstance()->getSectionNum()==3){
			if (randNum==1){
				count=500;
			}else if (randNum==2){
				count=50;
			}else if (randNum==3){
				count=17;
			}
		}

		hasBoxWard = false;
		auto ac = Sequence::create(CallFunc::create([=](){
			boxReward();
		}),CallFunc::create([=](){
			_boxAr->getAnimation()->play("begin");
		}),DelayTime::create(0.57f),CallFunc::create([=](){
			_boxAr->getAnimation()->play("circle");
		}),DelayTime::create(3.0f),CallFunc::create([=](){
			_boxAr->getAnimation()->play("end");
		}),DelayTime::create(1.0f),CallFunc::create([=](){
			Widget* wg = GUIReader::getInstance()->widgetFromJsonFile("Ui_Bag_Message/Ui_Bag_Message.ExportJson"); 
			Widget* btn = wg->getChildByName("Button_EnterMessage");
			btn->removeFromParentAndCleanup(true);
			this->addChild(wg,3);

			ImageView* icon = static_cast<ImageView*>(wg->getChildByName("Image_MessageItemIcon"));
			icon->loadTexture(filename,UI_TEX_TYPE_LOCAL);

			Text* cnt = static_cast<Text*>(wg->getChildByName("Text_MessageItemCount"));
			cnt->setText(StringUtils::format("%d",count).c_str());

			wg->runAction(Sequence::create(FadeIn::create(0.3f),DelayTime::create(3.0f),FadeOut::create(0.2f),NULL));
			wg->runAction(Sequence::create(DelayTime::create(3.5f),CallFunc::create([=](){
						wg->removeFromParentAndCleanup(true);
			}),NULL));
		}),DelayTime::create(3.0f),
			
		CallFunc::create([=]{
			winTheGame(times);
		}),NULL);

		this->runAction(ac);
		return;
	}

	do{
		_isWin =  true;
		NotificationCenter::getInstance()->postNotification(PAUSEKEY,  (Ref*)0);
		this->createLayer();
		starAnimationCount = 0;
		Widget *wg = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"win_panel"));

		Widget *failed_panel = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"failed_panel"));
		failed_panel->removeFromParentAndCleanup(true);
		//wg->setVisible(false);

		ArmatureDataManager::getInstance()->addArmatureFileInfo("td_viktri0.png","td_viktri0.plist","td_viktri.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
		ArmatureDataManager::getInstance()->addArmatureFileInfo("td_viktri_star0.png","td_viktri_star0.plist","td_viktri_star.ExportJson"); 

		Button *newChallengesBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Left"));
		newChallengesBtn->setTag(1835);
		newChallengesBtn->setTouchEnabled(false);
		newChallengesBtn->addTouchEventListener(this, toucheventselector(Ui_GameOver::newChallengesBtnCallBack));
		newChallengesBtn->setOpacity(0);
		newChallengesBtn->getChildByName("Image_3")->setOpacity(0);
		Button *nextPostBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_right"));

		nextPostBtn->setTag(1836);
		nextPostBtn->setTouchEnabled(false);
		nextPostBtn->addTouchEventListener(this, toucheventselector(Ui_GameOver::nextPostBtnCallBack));
		
		nextPostBtn->setOpacity(0);
		nextPostBtn->getChildByName("Image_2")->setOpacity(0);


		ImageView *prompt_bk = static_cast<ImageView*>(Helper::seekWidgetByName(wg,"prompt_bk"));
		prompt_bk->setOpacity(0);

		_warp = LayerColor::create( Color4B(0,0,0,0) );
		this->addChild(_warp);
		
		_warp->runAction(Sequence::create(
			FadeTo::create(0.5,176),
			CallFunc::create( CC_CALLBACK_0(Ui_GameOver::layerCallback,this)),
			DelayTime::create(0.5f),
			CallFunc::create([=](){
				newChallengesBtn->setTouchEnabled(true);
				nextPostBtn->setTouchEnabled(true);
		}),
			NULL));

		//Director::getInstance()->pause();
		NotificationCenter::getInstance()->postNotification(PAUSEKEY, (Ref*)0);

		Widget *panel_win   = wg->getChildByName("Panel_win");

		Text *rewardGoldnum = static_cast<Text*>(panel_win->getChildByName("reward_count1"));
		Text *rewardEggnum  = static_cast<Text*>(panel_win->getChildByName("reward_count2"));

		ImageView *reward_icon2 = static_cast<ImageView*>(panel_win->getChildByName("reward_icon2"));
		ImageView *reward_icon1  = static_cast<ImageView*>(panel_win->getChildByName("reward_icon1"));
		reward_icon1->setOpacity(0);
		reward_icon2->setOpacity(0);
		CC_BREAK_IF(!rewardGoldnum);
		CC_BREAK_IF(!rewardEggnum);
		auto user = UserRecord::getInstance();
		star_cnt = 0;
		int chapterId = user->getSectionNum();
		if (chapterId == 1 && user->getSceneNum() <=3)
		{
			if (user->getSceneNum() == 1)
			{
				rewardGoldnum->setText("50");
				rewardEggnum ->setText("1");
			} else if(user->getSceneNum() == 2) {
				rewardGoldnum->setText("150");
				rewardEggnum ->setText("2");
			} else if(user->getSceneNum() == 3) {
				rewardGoldnum->setText("250");
				rewardEggnum ->setText("3");
			} 
			if (user->getHP() <= 5){
				star_cnt = 1;
			} else if (user->getHP() < 10) {
				star_cnt = 2;
			} else {
				star_cnt = 3;
			}
		} else {
			if (user->getHP() <= 5){
				star_cnt = 1;
				if (chapterId == 1)
				{
					rewardGoldnum->setText("100");
					rewardEggnum ->setText("1");
				}else if (chapterId == 2)
				{
					rewardGoldnum->setText("200");
					rewardEggnum ->setText("2");
				} else if(chapterId == 3) {
					rewardGoldnum->setText("300");
					rewardEggnum ->setText("4");
				}
			}else if (user->getHP() < 10){
				star_cnt = 2;
				if (chapterId == 1)
				{
					rewardGoldnum->setText("300");
					rewardEggnum ->setText("2");
				}else if (chapterId == 2)
				{
					rewardGoldnum->setText("400");
					rewardEggnum ->setText("4");
				} else if(chapterId == 3) {
					rewardGoldnum->setText("600");
					rewardEggnum ->setText("6");
				}
			}else{
				star_cnt = 3;
				if (chapterId == 1)
				{
					rewardGoldnum->setText("500");
					rewardEggnum ->setText("4");
				}else if (chapterId == 2)
				{
					rewardGoldnum->setText("600");
					rewardEggnum ->setText("6");
				} else if(chapterId == 3) {
					rewardGoldnum->setText("900");
					rewardEggnum ->setText("8");
				}
			}
		}
		
		//联网通知服务器
		if (UserRecord::getInstance()->getSectionNum()!=0)
			request();
	
		rewardGoldnum->setOpacity(0);
		rewardEggnum->setOpacity(0);
	}while (0);
	audioPlayer::getInstance()->stopBackgroundMusic();
	audioPlayer::getInstance()->playEffect("S9_2.mp3");
	//// 延迟播放一段音效
	//this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
	//	audioPlayer::getInstance()->playEffect("S9.mp3");
	//}),NULL));
}
void Ui_GameOver::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	_isHttpFinish = true;
	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() <= 0 )
	{
		_isHttpSuccess = false;
		showError();
	}
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		_isHttpSuccess = false;
		showError();
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		_isHttpSuccess = true;
		
		if (_isWin)
		{
			//设置用户过关信息
			auto user = UserRecord::getInstance();
			if (star_cnt == 1){
				user->alterGold(100);
				user->alterGoodsCount("30001",2);
			}else if (star_cnt == 2){
				user->alterGold(300);
				user->alterGoodsCount("30001",4);
			}else if(star_cnt == 3) {
				user->alterGold(500);
				user->alterGoodsCount("30001",6);
			}
			user->setSceneStarCount(user->getSectionNum(),user->getSceneNum(),star_cnt);
		}
	}  else {
		_isHttpSuccess = false;
		showError();
	}
}

void Ui_GameOver::showError()
{

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
	prompt->setRightSelector(this,comm_selector(Ui_GameOver::request));
	prompt->setLeftSelector(this,comm_selector(Ui_GameOver::cancelRequest));
	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(prompt,300);
}
void Ui_GameOver::request()
{

	_isHttpFinish = false;
	_isHttpSuccess = false;
	//联网通知服务器
	//CCLOG("====%d = %d",UserRecord::getInstance()->getBoxEnemyDieMethod(),randNum);
	const char* postDatas;
	auto user = UserRecord::getInstance();
	//如果是有宝箱怪的关卡
	if (UserRecord::getInstance()->getBoxEnemySceneBySection(user->getSectionNum()) == user->getSceneNum())
	{
		postDatas = __String::createWithFormat("c=game&a=win&iswin=%d&star=%d&cid=%d&sid=%d&die=%d&aid=%d",
			_isWin,
			star_cnt,
			UserRecord::getInstance()->getSectionNum(), 
			UserRecord::getInstance()->getSceneNum(),
			UserRecord::getInstance()->getBoxEnemyDieMethod(),
			randNum
			)->getCString();
		user->setRandomScene(user->getSectionNum(),0);
	} else {
		postDatas = __String::createWithFormat("c=game&a=win&star=%d&cid=%d&sid=%d",
			star_cnt,
			UserRecord::getInstance()->getSectionNum(), 
			UserRecord::getInstance()->getSceneNum()
			)->getCString();
	}
	Connect* handle = Connect::getInstance();
	handle->send(postDatas,this,httpresponse_selector(Ui_GameOver::onHttpRequestCompleted));
}
void Ui_GameOver::cancelRequest()
{
	auto pTScene = startScene::createScene();
	TransitionFade* pTranslateScene = TransitionFade::create(0.6f,pTScene);;
	Director::getInstance()->replaceScene(pTranslateScene);
}
void Ui_GameOver::layerCallback(){

	_warp->runAction(FadeTo::create(0.5,200));
	
	Widget *win_widget  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"win_panel"));
	Widget *panel_win   = win_widget ->getChildByName("Panel_win");


	//这里是胜利出来的动画
	_armature = cocostudio::Armature::create("td_viktri");   
	auto animation = panel_win->getChildByName("animation");
	animation->addChild(_armature);
	_armature->setPositionX(animation->getSize().width*animation->getScale()/2+animation->getAnchorPointInPoints().x);
	_armature->getAnimation()->play("shengli",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Ui_GameOver::onFrameEvent));
}

void Ui_GameOver::boxReward()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_baoxiangtexiao.ExportJson");
	_boxAr = Armature::create("td_baoxiangtexiao"); 
	//导出来的时候被缩小到了0.8
	_boxAr->setScale(1.10);
	_boxAr->setPosition(visibleSize.width/2,visibleSize.height/2);

	this->addChild(_boxAr,10);
}

int Ui_GameOver::randomBoxEnemyReward()
{
	float r = CCRANDOM_0_1();
	if (r<0.2f){
		return 3;
	}else if(r<0.4f){
		return 2;
	}else{
		return 1;
	}
	return 0;
}


void Ui_GameOver::starAnimation()
{
	float delay=0;
	for (int i=0;i<3;++i){
		this->runAction(Sequence::create(DelayTime::create(delay),
								   CallFunc::create(CC_CALLBACK_0(Ui_GameOver::starAnimationCreate,this,this,i)),
								   NULL));
		delay += 0.2f; 
	}
	
	//Button *newChallengesBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Left"));
	//Button *nextPostBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_right"));
	//newChallengesBtn->setTouchEnabled(true);
	//nextPostBtn     ->setTouchEnabled(true);
}

void Ui_GameOver::starAnimationCreate(Widget *widget , int i)
{
	Widget *wg = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"win_panel"));
	Widget *panel_win   = wg->getChildByName("Panel_win");

	Armature *starArmature = cocostudio::Armature::create("td_viktri_star");
	Point pos = panel_win->getChildByName(StringUtils::format("star_%d",i).c_str())->getPosition();
	panel_win->addChild(starArmature,2);
	pos.y-=5;
	if (star_cnt > i)
	{
		starArmature->getAnimation()->play("play",-1,0);
	} else {
		starArmature->getAnimation()->play("play_01",-1,0);
		pos.y-=7;
	}
	
	starArmature->setPosition(pos);
	starArmature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Ui_GameOver::onFrameEvent));
}



//失败页面 
void Ui_GameOver::loseTheGame()
{

	this->createLayer();//创建页面
	do{
		_isWin = false;
		CC_BREAK_IF(!m_widget);
		Widget *wg = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"win_panel"));
		wg->removeFromParentAndCleanup(true);
		UserRecord::getInstance()->setIsGameOver(true);
		UserRecord::getInstance()->setIsGameStart(false);

		Widget *_widget = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"failed_panel"));
		_widget->setVisible(true);

		Widget *panel_fail  = _widget->getChildByName("Panel_fail");
		panel_fail->setVisible(true);
		Widget *warp = static_cast<Widget*>(Helper::seekWidgetByName(panel_fail,"warp"));
		warp->setVisible(false);
		_warp = LayerColor::create( Color4B(0,0,0,0) );
		this->addChild(_warp);

		_warp->runAction(Sequence::create(
			FadeTo::create(0.7,200),
			FadeTo::create(0.3,255),
			NULL
			));

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_fail/game_fail0.plist");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("game_fail/game_fail.ExportJson");
		_armature = Armature::create("game_fail");
		//panel_fail->addNode(_armature);
		panel_fail->addChild(_armature,10);
		_armature->setPosition(panel_fail->getSize().width/2,panel_fail->getSize().height/2);
		_armature->getAnimation()->play("fail_1",-1,0);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Ui_GameOver::onFrameEvent));

		Button *exitBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_return"));
		exitBtn->addTouchEventListener(this, toucheventselector(Ui_GameOver::exitBtnCallBack));
		exitBtn->setTouchEnabled(false);

		Button* newChallengesBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_again"));
		newChallengesBtn->addTouchEventListener(this, toucheventselector(Ui_GameOver::newChallengesBtnCallBack));
		newChallengesBtn->setTouchEnabled(false);


		//如果有宝箱怪
		auto user = UserRecord::getInstance();
		if ( UserRecord::getInstance()->getBoxEnemySceneBySection(user->getSectionNum()) == user->getSceneNum() ){
			request();
			UserRecord::getInstance()->setRandomScene(user->getSectionNum(),0);
		}
		NotificationCenter::getInstance()->postNotification(PAUSEKEY,  (Ref*)0);
	}while (0);

	audioPlayer::getInstance()->stopBackgroundMusic();
	audioPlayer::getInstance()->playEffect("S10_2.mp3");

          
}

//重新开始，再次挑战按钮回调
void Ui_GameOver::newChallengesBtnCallBack(Ref* sender, TouchEventType type)
{

	if (type == TOUCH_EVENT_ENDED){
		if (_isWin && UserRecord::getInstance()->getSectionNum()!=0)
		{
			//检查网络数据是否返回
			if (!_isHttpFinish && !_isHttpSuccess)
			{
				return;
			} else 
			{
				//如果返回错误数据.重新请求服务器
				if (_isHttpFinish && !_isHttpSuccess)
				{
					request();
					return;
				}
			}
		}
		
		CachManager::getInstance()->cleanCach();
		if ( (UserRecord::getInstance()->getIsFreshMan() && UserRecord::getInstance()->getSectionNum()==0
			&& UserRecord::getInstance()->getSceneNum()<=2) 
			|| 
			(UserRecord::getInstance()->getIsFreshMan()
			&&UserRecord::getInstance()->getSectionNum()==1
			&& UserRecord::getInstance()->getSceneNum()==3 )  ){
				UserRecord::getInstance()->setIsGameStart(false);
				UserRecord::getInstance()->setRound(0);
				//战斗准备场景,前三关教学场景
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
				TransitionFade* pTranslateScene = TransitionFade::create(0.6f,pTScene);;
				Director::getInstance()->replaceScene(pTranslateScene);
				return;
		}

// 		Ui_MainBack* mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
// 		mainBack->updateSceneStorey();
		Button *btn=static_cast<Button*>(sender);
		btn->setTouchEnabled(false);
		Scene* pTScene = ReadyScene::createScene();
		TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
		Director::getInstance()->replaceScene(transitionfadeScene);

	}
}

//退出按钮回调
void Ui_GameOver::exitBtnCallBack(Ref* sender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){
		//Director::getInstance()->resume();
		//NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, NULL);
		CachManager::getInstance()->cleanCach();
		Button *btn=static_cast<Button*>(sender);
		btn->setTouchEnabled(false);
		Scene* pTScene = levelScene::createScene();
		TransitionFade *transitionfadeScene=TransitionFade::create(0.6f,pTScene);
		Director::getInstance()->replaceScene(transitionfadeScene);
	}
}

//下一关按钮回调
void Ui_GameOver::nextPostBtnCallBack(Ref* sender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED){

		//检查网络数据是否返回
		if ( UserRecord::getInstance()->getSectionNum()!=0 ){
			if (!_isHttpFinish && !_isHttpSuccess)
			{
				return;
			} else 
			{
				//如果返回错误数据.重新请求服务器
				if (_isHttpFinish && !_isHttpSuccess)
				{
					request();
					return;
				}
			}
		}
		
		Button *nextPostBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_right"));
		nextPostBtn->setTouchEnabled(false);
		Button *btn=static_cast<Button*>(sender);
		btn->setTouchEnabled(false);
		CachManager::getInstance()->cleanCach();
		if (UserRecord::getInstance()->getSectionNum()==0 && UserRecord::getInstance()->getSceneNum()==1){
			if (UserRecord::getInstance()->getIndexGuide() < UserRecord::getInstance()->getSceneNum()){
				UserRecord::getInstance()->setIndexGuide( UserRecord::getInstance()->getSceneNum() );
				UserRecord::getInstance()->flush();
			}
			UserRecord::getInstance()->setSceneNum(UserRecord::getInstance()->getSceneNum()+1);
			UserRecord::getInstance()->setIsGameStart(false);
			UserRecord::getInstance()->setRound(0);
			//战斗准备场景,前二关教学场景
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
			return;
		}else if ( UserRecord::getInstance()->getSectionNum()==0 && UserRecord::getInstance()->getSceneNum()==2 ){
			if (UserRecord::getInstance()->getIndexGuide() < UserRecord::getInstance()->getSceneNum()){
				UserRecord::getInstance()->setIndexGuide( UserRecord::getInstance()->getSceneNum() );
				UserRecord::getInstance()->flush();
			}
			UserRecord::getInstance()->setSectionNum(1);
			Scene* pTScene = levelScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
			Director::getInstance()->replaceScene(transitionfadeScene);
		}else if  ( UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()==3 )
		{
			UserRecord::getInstance()->setIndexGuide(3);
			UserRecord::getInstance()->flush();

			Scene* pTScene = levelScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
			Director::getInstance()->replaceScene(transitionfadeScene);
			return;
		}else if (UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()==4)
		{
			//setPost
			if (!UserRecord::getInstance()->getIsHeroGuide()){
				UserRecord::getInstance()->setIndexPost(UserRecord::getInstance()->getSceneNum());
				UserRecord::getInstance()->flush();

				Scene* pTScene = levelScene::createScene();
				TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
				Director::getInstance()->replaceScene(transitionfadeScene);

				return;
			}
		}else if (UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()==5 )
		{
			//setPost
			if (!UserRecord::getInstance()->getIsGradeGuide())
			{
				UserRecord::getInstance()->setIndexPost(5);
				UserRecord::getInstance()->flush();

				Scene* pTScene = levelScene::createScene();
				TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
				Director::getInstance()->replaceScene(transitionfadeScene);
				return;
			}
		}else if (!(UserRecord::getInstance()->getSectionNum()==1 && UserRecord::getInstance()->getSceneNum()<3) ){
			UserRecord::getInstance()->setIndexPost(5);
		}

		NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
// 		Ui_MainBack* mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
// 		mainBack->updateSceneStorey();
		auto user =  UserRecord::getInstance();



		if (user->getSceneNum() == 15)
		{
			if (user->getSectionNum() != 3)
			{
				user->setSectionNum(user->getSectionNum()+1);
			}
			Scene* pTScene = levelScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
			Director::getInstance()->replaceScene(transitionfadeScene);
		} else {
// 			user->setSceneNum(user->getSceneNum()+1);
// 			Scene* pTScene = ReadyScene::createScene();
// 			TransitionFade *transitionfadeScene=TransitionFade::create(0.8f,pTScene);
// 			Director::getInstance()->replaceScene(transitionfadeScene);
			Scene* pTScene = levelScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(1.2f,pTScene);
			Director::getInstance()->replaceScene(transitionfadeScene);
		}
		
	}
}


void Ui_GameOver::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"showtext")==0)
	{
		Widget *_widget = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"warp"));
		//warp->setVisible(false);
		_widget->setVisible(true);
		_widget->setOpacity(0);
		_widget->runAction(FadeTo::create(0.4f,255));

		Button *exitBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_return"));
		Button* newChallengesBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_again"));
		if (exitBtn!=nullptr)
			exitBtn         ->setTouchEnabled(true);

		if (newChallengesBtn!=nullptr)
			newChallengesBtn->setTouchEnabled(true);

	} else if(strcmp(evt.c_str(),"finish")==0)
	{
		_armature->getAnimation()->stop();
		_armature->getAnimation()->play("fail_2");
	}


	//下面这单是胜利的骨骼动画回调
	if (strcmp(evt.c_str(),"lifinishi")==0){

		//wg->setVisible(true);//显示按钮等
		
	} else if (strcmp(evt.c_str(),"callStar")==0) {
		if (UserRecord::getInstance()->getSectionNum()!=0){
			starAnimation();
		}else{
			showWinButtons();
		}
	} else if(strcmp(evt.c_str(),"showPanel")==0) {
		starAnimationCount++;
		if (starAnimationCount == 2)
		{
			Widget *wg = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"win_panel"));
			Widget *Panel_win = static_cast<Button*>(Helper::seekWidgetByName(wg,"Panel_win"));
			Widget *prompt_bk = static_cast<Button*>(Helper::seekWidgetByName(Panel_win,"prompt_bk"));


			Text *rewardGoldnum = static_cast<Text*>(Panel_win->getChildByName("reward_count1"));
			Text *rewardEggnum  = static_cast<Text*>(Panel_win->getChildByName("reward_count2"));

			ImageView *reward_icon2 = static_cast<ImageView*>(Panel_win->getChildByName("reward_icon2"));
			ImageView *reward_icon1  = static_cast<ImageView*>(Panel_win->getChildByName("reward_icon1"));
			prompt_bk->runAction(
				Sequence::create(
					FadeIn::create(0.3f),
					CallFunc::create(CC_CALLBACK_0(Ui_GameOver::showWinButtons,this)),
					NULL
					)
				);
			reward_icon1->runAction(FadeIn::create(0.3f));
			reward_icon2->runAction(FadeIn::create(0.3f));
			rewardEggnum->runAction(FadeIn::create(0.3f));
			rewardGoldnum->runAction(FadeIn::create(0.3f));
		}
	}
}

void Ui_GameOver::showWinButtons()
{
	Button *newChallengesBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Left"));
	Button *nextPostBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_right"));

	auto action = Spawn::create(
		FadeIn::create(0.2f),
		ScaleTo::create( 0.2f,  1),
		NULL);
	newChallengesBtn->runAction(
			Sequence::create(
			action,
			CallFunc::create([=](){
				nextPostBtn->runAction(action);
				newChallengesBtn->setTouchEnabled(true);
				nextPostBtn->getChildByName("Image_2")->runAction(FadeIn::create(0.2f));
			}),
			NULL)
		);
		newChallengesBtn->getChildByName("Image_3")->runAction(FadeIn::create(0.2f));
		this->runAction(Sequence::create(
				DelayTime::create(0.4F),
				CallFunc::create([=](){
					nextPostBtn->setTouchEnabled(true);
				}),
				NULL
			));
}
void Ui_GameOver::FadeOver()
{
	
}

void Ui_GameOver::fitScreen()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point  VisibleOrigin = Director::getInstance()->getVisibleOrigin();
	_scale = MIN(visibleSize.width/960 , visibleSize.height/640);

	Widget *fail_widget = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"failed_panel"));
	Widget *win_widget  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"win_panel"));

	if (fail_widget != nullptr)
	{
		Widget *panel_fail  = fail_widget->getChildByName("Panel_fail");
		panel_fail->setScale(_scale);
		panel_fail->setPosition(Point(
			visibleSize.width/2  - panel_fail->getSize().width/2*_scale  ,
			visibleSize.height/2 - panel_fail->getSize().height/2*_scale ));
		Widget *fail_bg = static_cast<Button*>(Helper::seekWidgetByName(fail_widget,"Panel_1381"));
			fail_bg->setScaleY( visibleSize.height/640);
	}
	if (win_widget != nullptr)
	{
		Widget *panel_win   = win_widget ->getChildByName("Panel_win");
		panel_win ->setScale(_scale);
		panel_win->setPosition(Point(
			visibleSize.width/2  - panel_win->getSize().width/2*_scale  ,
			visibleSize.height/2 - panel_win->getSize().height/2*_scale ));
		Widget *win_bg  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Panel_mask"));
		win_bg ->setScaleY( visibleSize.height/640);
	}
}


